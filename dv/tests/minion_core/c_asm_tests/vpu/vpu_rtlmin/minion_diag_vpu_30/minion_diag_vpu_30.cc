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
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00004
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x55555555,                                                  // 4 random values                             /// 00010
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00014
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0xff800000,                                                  // -inf                                        /// 0001c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00024
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00028
			 0x00011111,                                                  // 9.7958E-41                                  /// 0002c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00034
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00038
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0003c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00040
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0004c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00050
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00054
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00058
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00060
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00064
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00068
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00070
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x7fc00001,                                                  // signaling NaN                               /// 0007c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x80000000,                                                  // -zero                                       /// 00088
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00094
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00098
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 000dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 000e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 000fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00100
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00104
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00108
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0010c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00124
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00128
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0012c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00130
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00134
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0013c
			 0xffc00001,                                                  // -signaling NaN                              /// 00140
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00144
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00148
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0014c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00154
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00158
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0015c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x7fc00001,                                                  // signaling NaN                               /// 00168
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00170
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00174
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00178
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0017c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00180
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00184
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00188
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00194
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00198
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0019c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0xcb000000,                                                  // -8388608.0                                  /// 001cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001d0
			 0xbf028f5c,                                                  // -0.51                                       /// 001d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001fc
			 0x55555555,                                                  // 4 random values                             /// 00200
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00208
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0020c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x7fc00001,                                                  // signaling NaN                               /// 00214
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x7fc00001,                                                  // signaling NaN                               /// 0021c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00220
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00224
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00228
			 0x0c600000,                                                  // Leading 1s:                                 /// 0022c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00230
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0023c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00244
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00248
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0024c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00250
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00258
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0025c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00264
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00268
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00270
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00274
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00280
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00288
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00290
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00294
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0029c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002a0
			 0x7f800000,                                                  // inf                                         /// 002a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002c0
			 0xffc00001,                                                  // -signaling NaN                              /// 002c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002cc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002f8
			 0x7f800000,                                                  // inf                                         /// 002fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00304
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00308
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0030c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00310
			 0x0c400000,                                                  // Leading 1s:                                 /// 00314
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x7f800000,                                                  // inf                                         /// 0031c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00320
			 0x0e000007,                                                  // Trailing 1s:                                /// 00324
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00328
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0032c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00334
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00338
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0033c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00340
			 0x80011111,                                                  // -9.7958E-41                                 /// 00344
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00348
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0035c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00360
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00364
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00370
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00374
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00378
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0037c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00380
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00388
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0038c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00394
			 0x3f028f5c,                                                  // 0.51                                        /// 00398
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003a8
			 0x3f028f5c,                                                  // 0.51                                        /// 003ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x33333333,                                                  // 4 random values                             /// 003d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x55555555,                                                  // 4 random values                             /// 00408
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00414
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00418
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0041c
			 0x7fc00001,                                                  // signaling NaN                               /// 00420
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00424
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00428
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0042c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00430
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00434
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0043c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00440
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00444
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00448
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0044c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00450
			 0x4b000000,                                                  // 8388608.0                                   /// 00454
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0045c
			 0x3f028f5c,                                                  // 0.51                                        /// 00460
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00464
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00470
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00474
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00478
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0047c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00484
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00488
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0048c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00498
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0049c
			 0xff800000,                                                  // -inf                                        /// 004a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00500
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00508
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0050c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00510
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00518
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00520
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00528
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00534
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00538
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0053c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00544
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00550
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00554
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00558
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0055c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00560
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00564
			 0x0c700000,                                                  // Leading 1s:                                 /// 00568
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0056c
			 0xffc00001,                                                  // -signaling NaN                              /// 00570
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00574
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00578
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0057c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00584
			 0x0c780000,                                                  // Leading 1s:                                 /// 00588
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00590
			 0x3f028f5c,                                                  // 0.51                                        /// 00594
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00598
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005a0
			 0x55555555,                                                  // 4 random values                             /// 005a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005d8
			 0xcb000000,                                                  // -8388608.0                                  /// 005dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005f8
			 0x00000000,                                                  // zero                                        /// 005fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00608
			 0x00000000,                                                  // zero                                        /// 0060c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00610
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x0c600000,                                                  // Leading 1s:                                 /// 00618
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0061c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00620
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00634
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0xcb000000,                                                  // -8388608.0                                  /// 0063c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00644
			 0x0e000007,                                                  // Trailing 1s:                                /// 00648
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00654
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00658
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x00000000,                                                  // zero                                        /// 00660
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00674
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00678
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0067c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00680
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00684
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00688
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00690
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00698
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0069c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 006e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006f4
			 0xcb000000,                                                  // -8388608.0                                  /// 006f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00700
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00704
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00710
			 0x0c780000,                                                  // Leading 1s:                                 /// 00714
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0071c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00720
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00724
			 0x00011111,                                                  // 9.7958E-41                                  /// 00728
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0072c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00730
			 0x00000000,                                                  // zero                                        /// 00734
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00738
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00744
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0074c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00750
			 0x0e000007,                                                  // Trailing 1s:                                /// 00754
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00758
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x3f028f5c,                                                  // 0.51                                        /// 0076c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00770
			 0x80011111,                                                  // -9.7958E-41                                 /// 00774
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00778
			 0xcb000000,                                                  // -8388608.0                                  /// 0077c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00784
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00788
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0078c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00790
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0080c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00810
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00818
			 0x3f028f5c,                                                  // 0.51                                        /// 0081c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00834
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00838
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0083c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00840
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00844
			 0x00011111,                                                  // 9.7958E-41                                  /// 00848
			 0x33333333,                                                  // 4 random values                             /// 0084c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00850
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00854
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00858
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0085c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00868
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0086c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00870
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00874
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00880
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00884
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00888
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0088c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00890
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00898
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0089c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 008b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 008bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x80000000,                                                  // -zero                                       /// 008d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x55555555,                                                  // 4 random values                             /// 008e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00904
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00908
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0090c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00910
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00914
			 0x3f028f5c,                                                  // 0.51                                        /// 00918
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0091c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00930
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00934
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00938
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0093c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00944
			 0x33333333,                                                  // 4 random values                             /// 00948
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0094c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00954
			 0x0e000007,                                                  // Trailing 1s:                                /// 00958
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0095c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00964
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00970
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00974
			 0x80000000,                                                  // -zero                                       /// 00978
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0097c
			 0xbf028f5c,                                                  // -0.51                                       /// 00980
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0098c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00990
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00994
			 0x7f800000,                                                  // inf                                         /// 00998
			 0xffc00001,                                                  // -signaling NaN                              /// 0099c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 009b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0xffc00001,                                                  // -signaling NaN                              /// 009c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a00
			 0xbf028f5c,                                                  // -0.51                                       /// 00a04
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a18
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a1c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a2c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a38
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a3c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a44
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a68
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a88
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a90
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aa8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ab4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ab8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00abc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ac0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ad4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ad8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ae0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00af0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00afc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b04
			 0x7f800000,                                                  // inf                                         /// 00b08
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b24
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b34
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b44
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b48
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b58
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b60
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b70
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b74
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b84
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b88
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b90
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b9c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ba0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ba4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ba8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bb4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bb8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bbc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bc0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bc4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bcc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x3f028f5c,                                                  // 0.51                                        /// 00be0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00be4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bf0
			 0x33333333,                                                  // 4 random values                             /// 00bf4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c00
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c1c
			 0xff800000,                                                  // -inf                                        /// 00c20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c24
			 0xff800000,                                                  // -inf                                        /// 00c28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c34
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c4c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c50
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x4b000000,                                                  // 8388608.0                                   /// 00c64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c74
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c80
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c84
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x80000000,                                                  // -zero                                       /// 00c90
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c94
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c9c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ca0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ca4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cb0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cb4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cbc
			 0xff800000,                                                  // -inf                                        /// 00cc0
			 0x33333333,                                                  // 4 random values                             /// 00cc4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cc8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ccc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cd4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cd8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cdc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ce0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ce4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ce8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cf4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d14
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d18
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d20
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d24
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d28
			 0xff800000,                                                  // -inf                                        /// 00d2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x00000000,                                                  // zero                                        /// 00d34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d3c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d54
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d5c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d68
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d74
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d78
			 0xcb000000,                                                  // -8388608.0                                  /// 00d7c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x00000000,                                                  // zero                                        /// 00d90
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d94
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d98
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00da0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00da4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00da8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80000000,                                                  // -zero                                       /// 00db0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00db4
			 0x55555555,                                                  // 4 random values                             /// 00db8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dbc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dc8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dcc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dd0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dd4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dd8
			 0x33333333,                                                  // 4 random values                             /// 00ddc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00de4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00de8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00df0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00df4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00df8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dfc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e04
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e0c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e14
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e20
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x55555555,                                                  // 4 random values                             /// 00e2c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e50
			 0xffc00001,                                                  // -signaling NaN                              /// 00e54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e60
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e74
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e7c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e84
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e88
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e8c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e90
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e94
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e98
			 0x80000000,                                                  // -zero                                       /// 00e9c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ea4
			 0x00000000,                                                  // zero                                        /// 00ea8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00eb0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eb4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ec0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ed4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ed8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00edc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80000000,                                                  // -zero                                       /// 00ee4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ee8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ef0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ef4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ef8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x4b000000,                                                  // 8388608.0                                   /// 00f00
			 0x00000000,                                                  // zero                                        /// 00f04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f0c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f18
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f20
			 0x7fc00001,                                                  // signaling NaN                               /// 00f24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f38
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f44
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f54
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f58
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f60
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f64
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f68
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f78
			 0xbf028f5c,                                                  // -0.51                                       /// 00f7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f88
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f90
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f9c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fa0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fa4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fb0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fb8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fc4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fc8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fd4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fd8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fe0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fe8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x7fc00001 // QNan                                          // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x5fc79c82, /// 0x0
			 0x2d91955e, /// 0x4
			 0xd5ceda37, /// 0x8
			 0x5265d7fd, /// 0xc
			 0x45ad9007, /// 0x10
			 0x3dbb2421, /// 0x14
			 0xc08fbe38, /// 0x18
			 0xac925f71, /// 0x1c
			 0x4e85a8d8, /// 0x20
			 0x481652dc, /// 0x24
			 0xc6e07f6a, /// 0x28
			 0x73f48332, /// 0x2c
			 0xcaf93981, /// 0x30
			 0x17b7b328, /// 0x34
			 0xdb4627fe, /// 0x38
			 0x935c5e40, /// 0x3c
			 0x9745e041, /// 0x40
			 0x93a76b6c, /// 0x44
			 0x5f4a2405, /// 0x48
			 0x0ff8b641, /// 0x4c
			 0x889c0b8f, /// 0x50
			 0x8d5fdd48, /// 0x54
			 0x7cd6cd5f, /// 0x58
			 0x1edc3110, /// 0x5c
			 0x860f49f0, /// 0x60
			 0xb1673e7b, /// 0x64
			 0xb5fc9b61, /// 0x68
			 0xf5c3c389, /// 0x6c
			 0x7b9373da, /// 0x70
			 0x789a4e95, /// 0x74
			 0x55b6a681, /// 0x78
			 0xe9705eca, /// 0x7c
			 0x4a3fd21a, /// 0x80
			 0x19080dc5, /// 0x84
			 0x2ea0b57d, /// 0x88
			 0x33892020, /// 0x8c
			 0x38ea4044, /// 0x90
			 0x173932dd, /// 0x94
			 0x5f9e7d8a, /// 0x98
			 0xa35f03aa, /// 0x9c
			 0x1d820b23, /// 0xa0
			 0x92b93843, /// 0xa4
			 0x52773303, /// 0xa8
			 0x7113d348, /// 0xac
			 0xe0b74194, /// 0xb0
			 0x1efc9872, /// 0xb4
			 0x94d7f2ae, /// 0xb8
			 0x2770dd68, /// 0xbc
			 0x03d4abcd, /// 0xc0
			 0xf084d0c4, /// 0xc4
			 0xffa15b93, /// 0xc8
			 0x483e8e61, /// 0xcc
			 0x21d7bfe8, /// 0xd0
			 0x5a0070f9, /// 0xd4
			 0x9b9d7259, /// 0xd8
			 0x5617702e, /// 0xdc
			 0x06217a06, /// 0xe0
			 0x378d2d48, /// 0xe4
			 0x37cb40bf, /// 0xe8
			 0x70303807, /// 0xec
			 0x670de77d, /// 0xf0
			 0x854991b4, /// 0xf4
			 0x423dcd74, /// 0xf8
			 0x50b2e7b0, /// 0xfc
			 0x54f947ad, /// 0x100
			 0x5e85a2b5, /// 0x104
			 0x318d39fb, /// 0x108
			 0xad49b979, /// 0x10c
			 0xf4d1a77f, /// 0x110
			 0x1b8a30f6, /// 0x114
			 0x89a75d9e, /// 0x118
			 0x947024f7, /// 0x11c
			 0x8fb51c35, /// 0x120
			 0x67b8b7d7, /// 0x124
			 0x87d68979, /// 0x128
			 0x28b64361, /// 0x12c
			 0xeb950d76, /// 0x130
			 0x13782a6f, /// 0x134
			 0xe98b463a, /// 0x138
			 0xc5536e61, /// 0x13c
			 0x15cc639f, /// 0x140
			 0xb2c7ff4b, /// 0x144
			 0x55cf9b71, /// 0x148
			 0x149149a9, /// 0x14c
			 0xda317555, /// 0x150
			 0x4a6d20bc, /// 0x154
			 0x69d97b36, /// 0x158
			 0x1d638f23, /// 0x15c
			 0xa04a1589, /// 0x160
			 0xdf23f8e6, /// 0x164
			 0xe14019ac, /// 0x168
			 0xe464ab3b, /// 0x16c
			 0x2f955843, /// 0x170
			 0x2172dd68, /// 0x174
			 0x19717ba8, /// 0x178
			 0xea630bb3, /// 0x17c
			 0x91aec70d, /// 0x180
			 0x7b466f2a, /// 0x184
			 0xe2a6c8b9, /// 0x188
			 0xe2f2c8f5, /// 0x18c
			 0xdd5edc21, /// 0x190
			 0x4efd63ab, /// 0x194
			 0x4af9165a, /// 0x198
			 0x70d940d8, /// 0x19c
			 0x6fe83935, /// 0x1a0
			 0xe85b6445, /// 0x1a4
			 0xdb14d8cf, /// 0x1a8
			 0x9a2cc0e4, /// 0x1ac
			 0x43a3015e, /// 0x1b0
			 0x1141ecf2, /// 0x1b4
			 0x7fb1a036, /// 0x1b8
			 0xd4331109, /// 0x1bc
			 0x9a51963e, /// 0x1c0
			 0xcacc8a47, /// 0x1c4
			 0x1df37c6b, /// 0x1c8
			 0x720e68cc, /// 0x1cc
			 0xcc3ecab2, /// 0x1d0
			 0xc2dcb5ea, /// 0x1d4
			 0xbf3f09b4, /// 0x1d8
			 0x1e1f812c, /// 0x1dc
			 0x388f84fd, /// 0x1e0
			 0x2e4e07c8, /// 0x1e4
			 0xfe8a2784, /// 0x1e8
			 0x74a3900b, /// 0x1ec
			 0xfa39ddcc, /// 0x1f0
			 0xb2323871, /// 0x1f4
			 0xeb47a483, /// 0x1f8
			 0xcb3aa9f1, /// 0x1fc
			 0xa3ade062, /// 0x200
			 0x944bb54f, /// 0x204
			 0x2e9b7c8d, /// 0x208
			 0x83aa9332, /// 0x20c
			 0x5050a8a8, /// 0x210
			 0x61ff1bdf, /// 0x214
			 0xb7c76299, /// 0x218
			 0x69b92ca5, /// 0x21c
			 0x0e1b36cf, /// 0x220
			 0xfa28d734, /// 0x224
			 0xebc20661, /// 0x228
			 0xf00b2e18, /// 0x22c
			 0x046da3dd, /// 0x230
			 0x2fa82201, /// 0x234
			 0x3bbfbd54, /// 0x238
			 0xfa00001a, /// 0x23c
			 0x1258c9b2, /// 0x240
			 0x65758dce, /// 0x244
			 0x21ae2330, /// 0x248
			 0x27b55dbd, /// 0x24c
			 0x0db17181, /// 0x250
			 0x8f13e3cb, /// 0x254
			 0xd63fa4a1, /// 0x258
			 0x904e73c6, /// 0x25c
			 0x7865aadd, /// 0x260
			 0xe44caa9e, /// 0x264
			 0xb2b5c49b, /// 0x268
			 0x7f3ef65f, /// 0x26c
			 0xa25c5c88, /// 0x270
			 0x1f0b5b72, /// 0x274
			 0xfeb51bc5, /// 0x278
			 0x858e80a6, /// 0x27c
			 0x839cead3, /// 0x280
			 0x6bf03c59, /// 0x284
			 0x85f8b545, /// 0x288
			 0x3b5a53e7, /// 0x28c
			 0x10a5c235, /// 0x290
			 0x59b9c4bf, /// 0x294
			 0x284915ab, /// 0x298
			 0xdde7b41c, /// 0x29c
			 0xb496102a, /// 0x2a0
			 0x4dee4a29, /// 0x2a4
			 0xfb20a952, /// 0x2a8
			 0xdeec24ff, /// 0x2ac
			 0x72d4082a, /// 0x2b0
			 0xadf9d183, /// 0x2b4
			 0x8ef7bb9b, /// 0x2b8
			 0xf45b72a7, /// 0x2bc
			 0xa44a3f63, /// 0x2c0
			 0xa0bc12b9, /// 0x2c4
			 0x0798ca34, /// 0x2c8
			 0xcf097978, /// 0x2cc
			 0xafdad878, /// 0x2d0
			 0xb80b7c64, /// 0x2d4
			 0x82be8c48, /// 0x2d8
			 0xe637b568, /// 0x2dc
			 0x966f461b, /// 0x2e0
			 0xf6a8957f, /// 0x2e4
			 0x0568aa3a, /// 0x2e8
			 0xf15fad56, /// 0x2ec
			 0xe4409497, /// 0x2f0
			 0x14b8859e, /// 0x2f4
			 0xa3bc051e, /// 0x2f8
			 0xd1f1618d, /// 0x2fc
			 0xa90335ae, /// 0x300
			 0xb43b5edf, /// 0x304
			 0x78b35ec5, /// 0x308
			 0x593bf838, /// 0x30c
			 0x89e737a6, /// 0x310
			 0x3c8377ad, /// 0x314
			 0x078d8e42, /// 0x318
			 0x87be1b1f, /// 0x31c
			 0x121dd3fc, /// 0x320
			 0x9a8e4eb2, /// 0x324
			 0xd4363be4, /// 0x328
			 0x19201fce, /// 0x32c
			 0x74494b7e, /// 0x330
			 0x3c09cd90, /// 0x334
			 0x418dcf78, /// 0x338
			 0x3292d2a9, /// 0x33c
			 0xd8f5be58, /// 0x340
			 0x5a643aae, /// 0x344
			 0x7648dc76, /// 0x348
			 0xe04b4485, /// 0x34c
			 0xdb49eb3a, /// 0x350
			 0xd70cdf72, /// 0x354
			 0xf844a320, /// 0x358
			 0xa1e6cba8, /// 0x35c
			 0x9f1d634a, /// 0x360
			 0x61ba48bd, /// 0x364
			 0xe3ce2121, /// 0x368
			 0x2a5275a0, /// 0x36c
			 0x9c1491d3, /// 0x370
			 0xdd34ae14, /// 0x374
			 0x63c548a2, /// 0x378
			 0xe2348833, /// 0x37c
			 0x3ea2b7d2, /// 0x380
			 0x5f7530bf, /// 0x384
			 0x793d5630, /// 0x388
			 0x60293768, /// 0x38c
			 0x07d70483, /// 0x390
			 0xad231d80, /// 0x394
			 0x2ca33a43, /// 0x398
			 0x2e6a4875, /// 0x39c
			 0x56ec35a5, /// 0x3a0
			 0x07778b10, /// 0x3a4
			 0x11febdb1, /// 0x3a8
			 0x827c1787, /// 0x3ac
			 0x383260cc, /// 0x3b0
			 0x4585a052, /// 0x3b4
			 0x77285f07, /// 0x3b8
			 0x21f76ea4, /// 0x3bc
			 0x1a6e71a4, /// 0x3c0
			 0x72564efa, /// 0x3c4
			 0xddaae189, /// 0x3c8
			 0xa904bcd5, /// 0x3cc
			 0x4ffab0d7, /// 0x3d0
			 0x925783b5, /// 0x3d4
			 0x76779d4e, /// 0x3d8
			 0x24b91311, /// 0x3dc
			 0x6067c0c6, /// 0x3e0
			 0xc4113199, /// 0x3e4
			 0xadad6716, /// 0x3e8
			 0x14d07c5a, /// 0x3ec
			 0xd9db6fb4, /// 0x3f0
			 0x1c303418, /// 0x3f4
			 0x52d44850, /// 0x3f8
			 0xc85efbf6, /// 0x3fc
			 0xcbb2d2a9, /// 0x400
			 0xb7f341d4, /// 0x404
			 0x1cecb604, /// 0x408
			 0xec34f0aa, /// 0x40c
			 0x40e17dc9, /// 0x410
			 0x4f907ef5, /// 0x414
			 0xf6bcde18, /// 0x418
			 0xe048acf9, /// 0x41c
			 0xe9722075, /// 0x420
			 0xf095d7b1, /// 0x424
			 0x44fac034, /// 0x428
			 0xdd137d27, /// 0x42c
			 0x9f9f01b1, /// 0x430
			 0xde0683fc, /// 0x434
			 0x8628432b, /// 0x438
			 0xb9432d82, /// 0x43c
			 0x76070d99, /// 0x440
			 0xa46963f7, /// 0x444
			 0x77130a60, /// 0x448
			 0xa65f6e8f, /// 0x44c
			 0xadaec431, /// 0x450
			 0xdfb9679f, /// 0x454
			 0x5cbaf211, /// 0x458
			 0xc4403d68, /// 0x45c
			 0x8ea2fd21, /// 0x460
			 0x32dd9dea, /// 0x464
			 0x703f60c3, /// 0x468
			 0x946718fa, /// 0x46c
			 0xe22db823, /// 0x470
			 0xfdbca976, /// 0x474
			 0x22a54a20, /// 0x478
			 0x24fa00ed, /// 0x47c
			 0x20a0d8b5, /// 0x480
			 0xdc50beb5, /// 0x484
			 0xa33be421, /// 0x488
			 0x8178b741, /// 0x48c
			 0x88817d0b, /// 0x490
			 0xb064086b, /// 0x494
			 0x280270ef, /// 0x498
			 0x3ee884b1, /// 0x49c
			 0x6a6cc906, /// 0x4a0
			 0xb1bee5c3, /// 0x4a4
			 0x67263e55, /// 0x4a8
			 0xbfd73388, /// 0x4ac
			 0x03becbf3, /// 0x4b0
			 0x0b94ba83, /// 0x4b4
			 0xac7a4f19, /// 0x4b8
			 0xe5040047, /// 0x4bc
			 0x2cf6fbdd, /// 0x4c0
			 0xf1ffd30e, /// 0x4c4
			 0xe48ee83b, /// 0x4c8
			 0x608b0e03, /// 0x4cc
			 0x74d55ec4, /// 0x4d0
			 0xd225d467, /// 0x4d4
			 0x69501a32, /// 0x4d8
			 0x795caaa4, /// 0x4dc
			 0xda680522, /// 0x4e0
			 0xc66b65a8, /// 0x4e4
			 0x37dd3c1c, /// 0x4e8
			 0x5b45d529, /// 0x4ec
			 0xc3473bc4, /// 0x4f0
			 0x28ef3137, /// 0x4f4
			 0xe5694619, /// 0x4f8
			 0xb7b6091b, /// 0x4fc
			 0x2b1f0c52, /// 0x500
			 0xe7af2e91, /// 0x504
			 0xd36f8b1b, /// 0x508
			 0xb5edb9d8, /// 0x50c
			 0x73319c69, /// 0x510
			 0x21003a87, /// 0x514
			 0x53a26159, /// 0x518
			 0x59147b11, /// 0x51c
			 0x747ee896, /// 0x520
			 0xb8980486, /// 0x524
			 0xf6f7e630, /// 0x528
			 0x17a7813f, /// 0x52c
			 0x01d18240, /// 0x530
			 0xa37c0857, /// 0x534
			 0xee18ce94, /// 0x538
			 0x7d57546b, /// 0x53c
			 0xa236ecc9, /// 0x540
			 0x95a02fc3, /// 0x544
			 0xda9ce89c, /// 0x548
			 0xd4f72de1, /// 0x54c
			 0xcc6f463e, /// 0x550
			 0x243fd759, /// 0x554
			 0xd86629ff, /// 0x558
			 0x26764855, /// 0x55c
			 0xaef3d3a0, /// 0x560
			 0x1158fc1c, /// 0x564
			 0x164cf7b2, /// 0x568
			 0x78794649, /// 0x56c
			 0x591eeac4, /// 0x570
			 0xc9d91d24, /// 0x574
			 0xa3278962, /// 0x578
			 0x91e0c6e3, /// 0x57c
			 0xd9624645, /// 0x580
			 0x33abb8b1, /// 0x584
			 0x3f6a2270, /// 0x588
			 0x0c6cb954, /// 0x58c
			 0x47a7aa65, /// 0x590
			 0x6c18cf48, /// 0x594
			 0xbd1a4598, /// 0x598
			 0x3efc264a, /// 0x59c
			 0x93ec5ef3, /// 0x5a0
			 0xf9935cdb, /// 0x5a4
			 0x357ed6cb, /// 0x5a8
			 0x51ecec0b, /// 0x5ac
			 0x3b7b52d2, /// 0x5b0
			 0xa9741ba2, /// 0x5b4
			 0xe218cec3, /// 0x5b8
			 0x7e1831a5, /// 0x5bc
			 0x0bbb0420, /// 0x5c0
			 0xce9a8ac2, /// 0x5c4
			 0xa2f3c35c, /// 0x5c8
			 0x96c353b1, /// 0x5cc
			 0x6f191794, /// 0x5d0
			 0xc083982d, /// 0x5d4
			 0x7ecebb53, /// 0x5d8
			 0x4e4b3771, /// 0x5dc
			 0x0d45259d, /// 0x5e0
			 0xd8011509, /// 0x5e4
			 0xef697642, /// 0x5e8
			 0x14784138, /// 0x5ec
			 0xbb4c7ca9, /// 0x5f0
			 0x30f421db, /// 0x5f4
			 0xf0cc485d, /// 0x5f8
			 0x6865085e, /// 0x5fc
			 0x6f1bfe40, /// 0x600
			 0x7eedb04e, /// 0x604
			 0xffb53e7d, /// 0x608
			 0x502ddf78, /// 0x60c
			 0xbbc7cde5, /// 0x610
			 0xe4392cb4, /// 0x614
			 0xc7b55c90, /// 0x618
			 0xa3f74c14, /// 0x61c
			 0x6f587069, /// 0x620
			 0xa79bc49a, /// 0x624
			 0x437e1727, /// 0x628
			 0x9d6b08a3, /// 0x62c
			 0x926fafc4, /// 0x630
			 0x8d761b37, /// 0x634
			 0x5e26392e, /// 0x638
			 0xb7fc94cb, /// 0x63c
			 0x09af731c, /// 0x640
			 0xba7e39de, /// 0x644
			 0x8b2a3b01, /// 0x648
			 0x2020f6dc, /// 0x64c
			 0xb1b4d464, /// 0x650
			 0x31c62d9e, /// 0x654
			 0x7543d1b0, /// 0x658
			 0xf71d5655, /// 0x65c
			 0xe35a49d6, /// 0x660
			 0xc9ea6378, /// 0x664
			 0xbea20a91, /// 0x668
			 0x6cf5298e, /// 0x66c
			 0x0fd18373, /// 0x670
			 0xff09290e, /// 0x674
			 0x5dda574b, /// 0x678
			 0x5bb08fe4, /// 0x67c
			 0xb34c8ece, /// 0x680
			 0x2234a2b5, /// 0x684
			 0x3de8a61e, /// 0x688
			 0xf44dca78, /// 0x68c
			 0x49d3407e, /// 0x690
			 0xccd1306a, /// 0x694
			 0x4eb90ab0, /// 0x698
			 0xefb58df0, /// 0x69c
			 0xbdf9bc91, /// 0x6a0
			 0xf9a411f3, /// 0x6a4
			 0x1b4e4451, /// 0x6a8
			 0xebb1290f, /// 0x6ac
			 0x8f183029, /// 0x6b0
			 0x06c689db, /// 0x6b4
			 0xc251749f, /// 0x6b8
			 0x44e1eaf8, /// 0x6bc
			 0xc2b9e825, /// 0x6c0
			 0x187e2034, /// 0x6c4
			 0xd89f3e3c, /// 0x6c8
			 0xfd9d03f0, /// 0x6cc
			 0x16216c07, /// 0x6d0
			 0xa966e27f, /// 0x6d4
			 0x28462b97, /// 0x6d8
			 0x9de17822, /// 0x6dc
			 0xd3614425, /// 0x6e0
			 0xa040d407, /// 0x6e4
			 0xcbdf9970, /// 0x6e8
			 0x5191226c, /// 0x6ec
			 0x75c4e473, /// 0x6f0
			 0x4b978a92, /// 0x6f4
			 0xec622ce9, /// 0x6f8
			 0xf85a9c2d, /// 0x6fc
			 0x287d81fa, /// 0x700
			 0x15b33252, /// 0x704
			 0xa22db127, /// 0x708
			 0x127b77fc, /// 0x70c
			 0x96498efd, /// 0x710
			 0x9c506523, /// 0x714
			 0xd93806fe, /// 0x718
			 0xaf2da23d, /// 0x71c
			 0xd9d12bc8, /// 0x720
			 0x9adf88ce, /// 0x724
			 0x75be7754, /// 0x728
			 0x20cc3469, /// 0x72c
			 0xca47700c, /// 0x730
			 0x74cead93, /// 0x734
			 0x9c975c94, /// 0x738
			 0x28897a52, /// 0x73c
			 0x71077e83, /// 0x740
			 0x3a027121, /// 0x744
			 0x5fda67d0, /// 0x748
			 0xd0b0eed3, /// 0x74c
			 0x3a79d230, /// 0x750
			 0xf5e24e91, /// 0x754
			 0xad116a20, /// 0x758
			 0x2634102b, /// 0x75c
			 0x7ba6c25f, /// 0x760
			 0xf5989dc7, /// 0x764
			 0xeaeaf3c6, /// 0x768
			 0x31e6907c, /// 0x76c
			 0xa3aba687, /// 0x770
			 0xa9f48801, /// 0x774
			 0x5e8620fb, /// 0x778
			 0xe920b08f, /// 0x77c
			 0x656c99ae, /// 0x780
			 0xb3985abf, /// 0x784
			 0x8e15a284, /// 0x788
			 0x38315da1, /// 0x78c
			 0x653b1270, /// 0x790
			 0x095ff5d9, /// 0x794
			 0xacb96b10, /// 0x798
			 0x6f7f303c, /// 0x79c
			 0x72b0ddd1, /// 0x7a0
			 0x18346dd9, /// 0x7a4
			 0x6c9297c9, /// 0x7a8
			 0x1882e19b, /// 0x7ac
			 0x3fd1da95, /// 0x7b0
			 0x30d373bb, /// 0x7b4
			 0x699c1a41, /// 0x7b8
			 0x9bd356aa, /// 0x7bc
			 0xf3a4b96e, /// 0x7c0
			 0xe4387244, /// 0x7c4
			 0xc0591360, /// 0x7c8
			 0xa12d6171, /// 0x7cc
			 0x099078fc, /// 0x7d0
			 0x99284ba9, /// 0x7d4
			 0x8c1223b6, /// 0x7d8
			 0x898b5a92, /// 0x7dc
			 0xa7dc778a, /// 0x7e0
			 0x6dfcbfc5, /// 0x7e4
			 0xec154f96, /// 0x7e8
			 0x867da91a, /// 0x7ec
			 0x39eba055, /// 0x7f0
			 0x2e0cd5a3, /// 0x7f4
			 0x5c430d05, /// 0x7f8
			 0xf66e5e54, /// 0x7fc
			 0x0d459154, /// 0x800
			 0xcdd5261e, /// 0x804
			 0xb7b26494, /// 0x808
			 0x3babd6c6, /// 0x80c
			 0x3f623027, /// 0x810
			 0xaa9be25e, /// 0x814
			 0xd49e5237, /// 0x818
			 0x3dc6dc6f, /// 0x81c
			 0x32c29dba, /// 0x820
			 0x4d835074, /// 0x824
			 0x6a54d1bb, /// 0x828
			 0xd5b25fdc, /// 0x82c
			 0x07888346, /// 0x830
			 0x8011a271, /// 0x834
			 0x250c5c2d, /// 0x838
			 0x33c0de1a, /// 0x83c
			 0x6dabcbb2, /// 0x840
			 0xefb81c54, /// 0x844
			 0x0ca77ddc, /// 0x848
			 0x0e0447d4, /// 0x84c
			 0x6717afd8, /// 0x850
			 0x60f4d9e9, /// 0x854
			 0x1a3ca03c, /// 0x858
			 0xb2bd8bee, /// 0x85c
			 0x9c062527, /// 0x860
			 0x3e4a891d, /// 0x864
			 0x981d9c09, /// 0x868
			 0x42915627, /// 0x86c
			 0x3cb3f54c, /// 0x870
			 0x89c3338b, /// 0x874
			 0x72139db1, /// 0x878
			 0xea1143b0, /// 0x87c
			 0x63bc0877, /// 0x880
			 0xdeb9fea2, /// 0x884
			 0xb0e8c5d9, /// 0x888
			 0x440becd7, /// 0x88c
			 0x8a017a8a, /// 0x890
			 0x81118baf, /// 0x894
			 0xc91ce793, /// 0x898
			 0x5c807684, /// 0x89c
			 0x05dd07ec, /// 0x8a0
			 0x42430cbb, /// 0x8a4
			 0x21b9f3a6, /// 0x8a8
			 0x0a7bf763, /// 0x8ac
			 0xdbf3618a, /// 0x8b0
			 0xa88b6887, /// 0x8b4
			 0x8748fbb5, /// 0x8b8
			 0x301885dd, /// 0x8bc
			 0x0647045f, /// 0x8c0
			 0x528ad181, /// 0x8c4
			 0xd0ecf000, /// 0x8c8
			 0x3df05b44, /// 0x8cc
			 0x79dd1249, /// 0x8d0
			 0x751c533d, /// 0x8d4
			 0xe937f1d7, /// 0x8d8
			 0x49ac3649, /// 0x8dc
			 0x41fee73d, /// 0x8e0
			 0x401675a7, /// 0x8e4
			 0x3c0bd6a2, /// 0x8e8
			 0x76bb501e, /// 0x8ec
			 0x1d8ce890, /// 0x8f0
			 0x339ee8aa, /// 0x8f4
			 0x9f1f9b58, /// 0x8f8
			 0x81274ea2, /// 0x8fc
			 0x7d3c7226, /// 0x900
			 0x3ce3542a, /// 0x904
			 0xae872493, /// 0x908
			 0x88bbdf10, /// 0x90c
			 0xb9c3c4f2, /// 0x910
			 0xdb8fb5c8, /// 0x914
			 0x01594930, /// 0x918
			 0xf782a1a6, /// 0x91c
			 0x8d4dbeb7, /// 0x920
			 0xe255819b, /// 0x924
			 0xa1dfab6a, /// 0x928
			 0xe732a973, /// 0x92c
			 0x455c6a2c, /// 0x930
			 0x61e9628e, /// 0x934
			 0xeb7de67b, /// 0x938
			 0x05677ad4, /// 0x93c
			 0x0f7ffab5, /// 0x940
			 0x8a5dce1c, /// 0x944
			 0x66c7ab1d, /// 0x948
			 0xa1c0b404, /// 0x94c
			 0x679e5e96, /// 0x950
			 0xaaf66444, /// 0x954
			 0x5e7059d2, /// 0x958
			 0x815127b4, /// 0x95c
			 0x4364d302, /// 0x960
			 0xcb8f595b, /// 0x964
			 0x3fcf47cd, /// 0x968
			 0x4ca78e59, /// 0x96c
			 0x795753d2, /// 0x970
			 0x1a79b626, /// 0x974
			 0xb58e9f67, /// 0x978
			 0x9cdadc2b, /// 0x97c
			 0x1b7f6098, /// 0x980
			 0x0213d951, /// 0x984
			 0x6ed8daba, /// 0x988
			 0x9ea0693d, /// 0x98c
			 0x8abe607e, /// 0x990
			 0xb759faa0, /// 0x994
			 0x80c0afe0, /// 0x998
			 0x267c0f34, /// 0x99c
			 0xe7977f2f, /// 0x9a0
			 0xc3caf82b, /// 0x9a4
			 0x0f83a1f0, /// 0x9a8
			 0xcc361ed0, /// 0x9ac
			 0x7113b69f, /// 0x9b0
			 0xf7271229, /// 0x9b4
			 0xa8456dbd, /// 0x9b8
			 0x70626b3f, /// 0x9bc
			 0xfa17bfb1, /// 0x9c0
			 0x6ce33909, /// 0x9c4
			 0x0dc26498, /// 0x9c8
			 0x63ac9385, /// 0x9cc
			 0xb0ca9b6e, /// 0x9d0
			 0xd4a523bd, /// 0x9d4
			 0x33d4b106, /// 0x9d8
			 0x69943673, /// 0x9dc
			 0xeecc4db1, /// 0x9e0
			 0x6428d8fe, /// 0x9e4
			 0xe909d0aa, /// 0x9e8
			 0x8d847904, /// 0x9ec
			 0xf716a152, /// 0x9f0
			 0x43939551, /// 0x9f4
			 0xbbaa6f59, /// 0x9f8
			 0x9654d6c2, /// 0x9fc
			 0xb82dda9b, /// 0xa00
			 0x04b9292b, /// 0xa04
			 0xc63054e5, /// 0xa08
			 0xdea7f2c7, /// 0xa0c
			 0xa2f7580d, /// 0xa10
			 0x74098ed3, /// 0xa14
			 0x35e76a65, /// 0xa18
			 0xe99cd2b8, /// 0xa1c
			 0xa19e4a41, /// 0xa20
			 0x7134ffaa, /// 0xa24
			 0x7e46ac7d, /// 0xa28
			 0xfb06ed13, /// 0xa2c
			 0x96dd4934, /// 0xa30
			 0x8ace84d0, /// 0xa34
			 0x06063c0a, /// 0xa38
			 0xcf799218, /// 0xa3c
			 0x99e3d7d2, /// 0xa40
			 0x75cb2c11, /// 0xa44
			 0xafaba4ce, /// 0xa48
			 0xbd845bed, /// 0xa4c
			 0x0b7b3ffe, /// 0xa50
			 0xcb3011b4, /// 0xa54
			 0xb7ab45f3, /// 0xa58
			 0x231c72c9, /// 0xa5c
			 0x68a14e97, /// 0xa60
			 0x7229e26f, /// 0xa64
			 0x3402e274, /// 0xa68
			 0xda076a5a, /// 0xa6c
			 0xf83a16f4, /// 0xa70
			 0xbfabb1e5, /// 0xa74
			 0x19a8ebaa, /// 0xa78
			 0x232398c7, /// 0xa7c
			 0xf8b2cd4a, /// 0xa80
			 0xe2a71192, /// 0xa84
			 0xc3e3eefb, /// 0xa88
			 0xb87796e7, /// 0xa8c
			 0x77c057c9, /// 0xa90
			 0x84be2b57, /// 0xa94
			 0x03beec7f, /// 0xa98
			 0x106b065b, /// 0xa9c
			 0x659d44fd, /// 0xaa0
			 0x45bc808c, /// 0xaa4
			 0x13fa1972, /// 0xaa8
			 0xcd1ec6e1, /// 0xaac
			 0xeb895fdd, /// 0xab0
			 0x1e9932a6, /// 0xab4
			 0xa2eee2ef, /// 0xab8
			 0xf19da1e0, /// 0xabc
			 0x2a06567d, /// 0xac0
			 0x84020edd, /// 0xac4
			 0x883c92fe, /// 0xac8
			 0xd8babcb2, /// 0xacc
			 0xa9234346, /// 0xad0
			 0x33625afe, /// 0xad4
			 0x7b1aa280, /// 0xad8
			 0xc57a14bf, /// 0xadc
			 0x6fc78dac, /// 0xae0
			 0xbe52a90d, /// 0xae4
			 0x585ebdf4, /// 0xae8
			 0xc89b2c93, /// 0xaec
			 0x2e05a7d6, /// 0xaf0
			 0xb2f81d09, /// 0xaf4
			 0xc44f12fd, /// 0xaf8
			 0xf3bc798b, /// 0xafc
			 0xb5cf1751, /// 0xb00
			 0x3e0f31e3, /// 0xb04
			 0x8c5516d2, /// 0xb08
			 0x81b56a08, /// 0xb0c
			 0x096c215a, /// 0xb10
			 0x48c0fe77, /// 0xb14
			 0x8ddc112e, /// 0xb18
			 0xdc976ca2, /// 0xb1c
			 0x034f738d, /// 0xb20
			 0x9fe9645e, /// 0xb24
			 0x8db8fc73, /// 0xb28
			 0x8f366e65, /// 0xb2c
			 0xc26383eb, /// 0xb30
			 0x3860330c, /// 0xb34
			 0x43df561c, /// 0xb38
			 0x0249d1db, /// 0xb3c
			 0x31407f5c, /// 0xb40
			 0xc89ed98f, /// 0xb44
			 0xa48e1f49, /// 0xb48
			 0xf6a53a2b, /// 0xb4c
			 0x6c147fde, /// 0xb50
			 0x922f705e, /// 0xb54
			 0x04257d63, /// 0xb58
			 0xea038fc0, /// 0xb5c
			 0x5cb86d63, /// 0xb60
			 0x80747070, /// 0xb64
			 0x5043e02c, /// 0xb68
			 0xcba6d4d0, /// 0xb6c
			 0xb1a10d4b, /// 0xb70
			 0x2c66c376, /// 0xb74
			 0xc2146bd6, /// 0xb78
			 0x1325405e, /// 0xb7c
			 0x495cfc0a, /// 0xb80
			 0xeba627a8, /// 0xb84
			 0xbf533dd4, /// 0xb88
			 0xd5861230, /// 0xb8c
			 0x9235b94f, /// 0xb90
			 0x7a696077, /// 0xb94
			 0xe1bc8b97, /// 0xb98
			 0x236353fd, /// 0xb9c
			 0xf947501c, /// 0xba0
			 0x1b6288bf, /// 0xba4
			 0x99a1fb28, /// 0xba8
			 0x6ddd12a7, /// 0xbac
			 0x441918ee, /// 0xbb0
			 0x3cf819f9, /// 0xbb4
			 0x82e1a6e7, /// 0xbb8
			 0x83347431, /// 0xbbc
			 0x0297a638, /// 0xbc0
			 0x7be31fb9, /// 0xbc4
			 0xe613fc17, /// 0xbc8
			 0x4439b2d1, /// 0xbcc
			 0x705e878d, /// 0xbd0
			 0x98530af4, /// 0xbd4
			 0xdf54704b, /// 0xbd8
			 0xe77781d8, /// 0xbdc
			 0xd633e4bd, /// 0xbe0
			 0xfff5d21f, /// 0xbe4
			 0x76e54225, /// 0xbe8
			 0xb490e00a, /// 0xbec
			 0xafc9198e, /// 0xbf0
			 0xc2544516, /// 0xbf4
			 0xaebbc1d0, /// 0xbf8
			 0x49df4c5d, /// 0xbfc
			 0x2e030a13, /// 0xc00
			 0x93b3746b, /// 0xc04
			 0x39fce102, /// 0xc08
			 0x3dd75c7d, /// 0xc0c
			 0x48185183, /// 0xc10
			 0x68214f0f, /// 0xc14
			 0x1a71ca19, /// 0xc18
			 0x565c3c05, /// 0xc1c
			 0xb1b4e80c, /// 0xc20
			 0x89e8794f, /// 0xc24
			 0xf8d944ac, /// 0xc28
			 0x134e5a52, /// 0xc2c
			 0x40fc665e, /// 0xc30
			 0x872a389b, /// 0xc34
			 0xccb71500, /// 0xc38
			 0xa6ddc508, /// 0xc3c
			 0x3d5dee25, /// 0xc40
			 0xf709f031, /// 0xc44
			 0x0a33b1e5, /// 0xc48
			 0x703f8be2, /// 0xc4c
			 0x83c35a2b, /// 0xc50
			 0xe05ee084, /// 0xc54
			 0xfecc26d2, /// 0xc58
			 0x1a3d9b32, /// 0xc5c
			 0xb6307c11, /// 0xc60
			 0xa59f2570, /// 0xc64
			 0x93cbfaf5, /// 0xc68
			 0xa4dd6207, /// 0xc6c
			 0xf8b1ad4b, /// 0xc70
			 0x8fa7385a, /// 0xc74
			 0x7a2d8477, /// 0xc78
			 0x6b3b585a, /// 0xc7c
			 0x8bbd8195, /// 0xc80
			 0xf9e22bc1, /// 0xc84
			 0x1c7e682d, /// 0xc88
			 0xb823b7ad, /// 0xc8c
			 0x232e82f3, /// 0xc90
			 0x312503b7, /// 0xc94
			 0xdd2597c0, /// 0xc98
			 0xe38b672a, /// 0xc9c
			 0x84a5fe40, /// 0xca0
			 0xf2081ca5, /// 0xca4
			 0xa45277af, /// 0xca8
			 0x3189768a, /// 0xcac
			 0xcdb08f03, /// 0xcb0
			 0x6cda65ba, /// 0xcb4
			 0xf0ad13c4, /// 0xcb8
			 0x7c8fe9cd, /// 0xcbc
			 0xafe555f2, /// 0xcc0
			 0xb3e8f7cc, /// 0xcc4
			 0x74ae38f3, /// 0xcc8
			 0x5ebec3a2, /// 0xccc
			 0xd10562e1, /// 0xcd0
			 0xbdc323f4, /// 0xcd4
			 0xc6f73ee7, /// 0xcd8
			 0x04773424, /// 0xcdc
			 0x0fb68df5, /// 0xce0
			 0x302ffc25, /// 0xce4
			 0x37a57ac6, /// 0xce8
			 0xf6835681, /// 0xcec
			 0xe7055aed, /// 0xcf0
			 0xb15c6d3e, /// 0xcf4
			 0x487e8819, /// 0xcf8
			 0x082228f7, /// 0xcfc
			 0xffd814de, /// 0xd00
			 0x9be17307, /// 0xd04
			 0x54e53fb8, /// 0xd08
			 0x63a05de6, /// 0xd0c
			 0xe5e62424, /// 0xd10
			 0xda798367, /// 0xd14
			 0xadc4353d, /// 0xd18
			 0x3784cd6b, /// 0xd1c
			 0x85b9b095, /// 0xd20
			 0xf24f636b, /// 0xd24
			 0x75b6887c, /// 0xd28
			 0x8bcca5cc, /// 0xd2c
			 0xc3082ea1, /// 0xd30
			 0xeaa1d359, /// 0xd34
			 0x5fe5a8e5, /// 0xd38
			 0xe61a596a, /// 0xd3c
			 0xe02750f2, /// 0xd40
			 0x054afcac, /// 0xd44
			 0xf2365c22, /// 0xd48
			 0x0b86ec04, /// 0xd4c
			 0x7a14a098, /// 0xd50
			 0xbe2effba, /// 0xd54
			 0x4894d21b, /// 0xd58
			 0x381a71fc, /// 0xd5c
			 0x9919bef0, /// 0xd60
			 0xfc1328d3, /// 0xd64
			 0x29fca5e7, /// 0xd68
			 0xbf3ffac3, /// 0xd6c
			 0xa43d4ae9, /// 0xd70
			 0x557c1532, /// 0xd74
			 0x7256666e, /// 0xd78
			 0x479a4065, /// 0xd7c
			 0x2a848723, /// 0xd80
			 0x25204209, /// 0xd84
			 0x9e7956b9, /// 0xd88
			 0x2776a2bc, /// 0xd8c
			 0x64805e36, /// 0xd90
			 0xaf14ca57, /// 0xd94
			 0x8649a30a, /// 0xd98
			 0x4db12e63, /// 0xd9c
			 0x0af91d99, /// 0xda0
			 0xda8c11c4, /// 0xda4
			 0x662ede41, /// 0xda8
			 0xfd09f200, /// 0xdac
			 0x20bb5cc8, /// 0xdb0
			 0x389d464f, /// 0xdb4
			 0x439bc6cf, /// 0xdb8
			 0x04e996a7, /// 0xdbc
			 0xcf05ee40, /// 0xdc0
			 0x7e0b5e1b, /// 0xdc4
			 0xf4f61655, /// 0xdc8
			 0x70322b71, /// 0xdcc
			 0x62220e11, /// 0xdd0
			 0x3d4377a6, /// 0xdd4
			 0xcb0f494e, /// 0xdd8
			 0x46ab236c, /// 0xddc
			 0x7c66580b, /// 0xde0
			 0x4c99d7ec, /// 0xde4
			 0x81cc1986, /// 0xde8
			 0x8a1532e6, /// 0xdec
			 0x53f752eb, /// 0xdf0
			 0x8220ac03, /// 0xdf4
			 0xddc75fbb, /// 0xdf8
			 0x22f7e736, /// 0xdfc
			 0xc51f8722, /// 0xe00
			 0x912d2cb7, /// 0xe04
			 0xfd0e50f6, /// 0xe08
			 0xf97ffc39, /// 0xe0c
			 0x35f6e416, /// 0xe10
			 0xf75e8742, /// 0xe14
			 0xcecb0d44, /// 0xe18
			 0xbd4dcc58, /// 0xe1c
			 0x3ce1347e, /// 0xe20
			 0xb9e9bb34, /// 0xe24
			 0x3b86962a, /// 0xe28
			 0x95a30ae2, /// 0xe2c
			 0xc3156c77, /// 0xe30
			 0x2e5d5f31, /// 0xe34
			 0x57fbba69, /// 0xe38
			 0xf7b00706, /// 0xe3c
			 0x30e0380a, /// 0xe40
			 0xe246069d, /// 0xe44
			 0xe3162bdb, /// 0xe48
			 0xf645969c, /// 0xe4c
			 0x4db37adf, /// 0xe50
			 0xf99c3da3, /// 0xe54
			 0x212b77e4, /// 0xe58
			 0xe4802aac, /// 0xe5c
			 0x232ab2e6, /// 0xe60
			 0xe408055c, /// 0xe64
			 0xb4d8df40, /// 0xe68
			 0x867056a7, /// 0xe6c
			 0xb19b14cd, /// 0xe70
			 0x0701af80, /// 0xe74
			 0xdb70a96c, /// 0xe78
			 0x190675c7, /// 0xe7c
			 0x2f38f831, /// 0xe80
			 0x0f64a03d, /// 0xe84
			 0x2888c9fb, /// 0xe88
			 0xd4631607, /// 0xe8c
			 0x9be1711a, /// 0xe90
			 0xe37666de, /// 0xe94
			 0xa8defe55, /// 0xe98
			 0x87df3be7, /// 0xe9c
			 0x89f3c860, /// 0xea0
			 0x2440cb53, /// 0xea4
			 0xa55e05fb, /// 0xea8
			 0xae56f707, /// 0xeac
			 0xed994f24, /// 0xeb0
			 0x777e3d64, /// 0xeb4
			 0xfd7f640d, /// 0xeb8
			 0xd209164f, /// 0xebc
			 0x7033154e, /// 0xec0
			 0x83e125c3, /// 0xec4
			 0xffc859c3, /// 0xec8
			 0x7ef0ba70, /// 0xecc
			 0xfafbb0ab, /// 0xed0
			 0xccf6adcd, /// 0xed4
			 0xa0758e0b, /// 0xed8
			 0x577a8c1d, /// 0xedc
			 0x89739434, /// 0xee0
			 0x8e4b193d, /// 0xee4
			 0x2db6b4b1, /// 0xee8
			 0x258367a2, /// 0xeec
			 0x5f3bf643, /// 0xef0
			 0x6fdc5d2b, /// 0xef4
			 0x7a927270, /// 0xef8
			 0xeb5bc8f4, /// 0xefc
			 0x8e9c6c3d, /// 0xf00
			 0x0fb44bf9, /// 0xf04
			 0xb0f2779f, /// 0xf08
			 0x0de1978a, /// 0xf0c
			 0x0f7bd0a9, /// 0xf10
			 0x7960d778, /// 0xf14
			 0xa087d890, /// 0xf18
			 0x8ce3d931, /// 0xf1c
			 0x426c4cf7, /// 0xf20
			 0xdc8db37f, /// 0xf24
			 0x8d75a8e5, /// 0xf28
			 0x0594f210, /// 0xf2c
			 0x5d5fba07, /// 0xf30
			 0x804e7334, /// 0xf34
			 0xc8622392, /// 0xf38
			 0x4238de73, /// 0xf3c
			 0x0af88799, /// 0xf40
			 0x4b84f88e, /// 0xf44
			 0xfc7da506, /// 0xf48
			 0xa6bb79c2, /// 0xf4c
			 0xb96ad5ba, /// 0xf50
			 0x9dcdcd4f, /// 0xf54
			 0x91051c53, /// 0xf58
			 0xbc9f0bae, /// 0xf5c
			 0x3db6f220, /// 0xf60
			 0x864b1f8d, /// 0xf64
			 0x30d364b7, /// 0xf68
			 0xb2499e17, /// 0xf6c
			 0xfd041b58, /// 0xf70
			 0x026d4b26, /// 0xf74
			 0xd8f3ed2f, /// 0xf78
			 0x122fec81, /// 0xf7c
			 0x4f8884bf, /// 0xf80
			 0x1c228ed7, /// 0xf84
			 0x11b00105, /// 0xf88
			 0x56bb3d5e, /// 0xf8c
			 0x4b59db16, /// 0xf90
			 0xcc8aa9cb, /// 0xf94
			 0xc1a2330c, /// 0xf98
			 0xdf7c4bf8, /// 0xf9c
			 0xd691d8a1, /// 0xfa0
			 0xb02ee404, /// 0xfa4
			 0xbe1d5c94, /// 0xfa8
			 0x28a87881, /// 0xfac
			 0x1062522e, /// 0xfb0
			 0xd50dc959, /// 0xfb4
			 0xfd8480be, /// 0xfb8
			 0x3ac56e98, /// 0xfbc
			 0x0f0b5d08, /// 0xfc0
			 0xcf1fd6df, /// 0xfc4
			 0xc99513c1, /// 0xfc8
			 0x097e6da5, /// 0xfcc
			 0xf5bc536e, /// 0xfd0
			 0x8eab9402, /// 0xfd4
			 0xd644d64e, /// 0xfd8
			 0xf880b343, /// 0xfdc
			 0xe211e504, /// 0xfe0
			 0xb75f5369, /// 0xfe4
			 0xf76f0559, /// 0xfe8
			 0xcb072121, /// 0xfec
			 0x120bdb05, /// 0xff0
			 0x609af12d, /// 0xff4
			 0x7e3ef86e, /// 0xff8
			 0x19410f76 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xd643ad66, /// 0x0
			 0x9b6d3259, /// 0x4
			 0xa4fa155b, /// 0x8
			 0x0e372004, /// 0xc
			 0xcb0ec1de, /// 0x10
			 0x7c208691, /// 0x14
			 0x7fc8865c, /// 0x18
			 0x1209f6ac, /// 0x1c
			 0x2cd9ae20, /// 0x20
			 0x6d52436c, /// 0x24
			 0x627b3a05, /// 0x28
			 0x84d57dd9, /// 0x2c
			 0x6e77077b, /// 0x30
			 0x6bc2a720, /// 0x34
			 0x6b22d3cd, /// 0x38
			 0xc3e96805, /// 0x3c
			 0x14470318, /// 0x40
			 0x64d645dc, /// 0x44
			 0xa2c21e87, /// 0x48
			 0x6e9701e6, /// 0x4c
			 0x72f68811, /// 0x50
			 0x2285cc3a, /// 0x54
			 0x2a2b93ea, /// 0x58
			 0x8a3c29ff, /// 0x5c
			 0x50218b72, /// 0x60
			 0x5e6f49f1, /// 0x64
			 0xf4934b87, /// 0x68
			 0x76c4e187, /// 0x6c
			 0xf333e64d, /// 0x70
			 0xe414abcb, /// 0x74
			 0xe8d1009d, /// 0x78
			 0xc4b52b02, /// 0x7c
			 0x01dd071f, /// 0x80
			 0x3854594b, /// 0x84
			 0xa5dc6547, /// 0x88
			 0x2cbefed0, /// 0x8c
			 0x014495df, /// 0x90
			 0x8d5e4faa, /// 0x94
			 0xeb6f2ab2, /// 0x98
			 0xe1b14ff6, /// 0x9c
			 0xa91692f6, /// 0xa0
			 0xec49f559, /// 0xa4
			 0xd194a7c7, /// 0xa8
			 0x35b365e0, /// 0xac
			 0x4ed8b9e8, /// 0xb0
			 0x84b52856, /// 0xb4
			 0xa36de998, /// 0xb8
			 0xcf971632, /// 0xbc
			 0x117b26d1, /// 0xc0
			 0x0232cfb8, /// 0xc4
			 0x6ab11c79, /// 0xc8
			 0xf2af7b5a, /// 0xcc
			 0x88914f91, /// 0xd0
			 0x13ff6c3b, /// 0xd4
			 0x9f1a3fbf, /// 0xd8
			 0xaa15e3f7, /// 0xdc
			 0x66601b53, /// 0xe0
			 0x925cb1fb, /// 0xe4
			 0x1352e047, /// 0xe8
			 0x02e48b99, /// 0xec
			 0x45582556, /// 0xf0
			 0x4801cdfe, /// 0xf4
			 0x042e63d8, /// 0xf8
			 0xd3d1bc19, /// 0xfc
			 0x4f169d38, /// 0x100
			 0xeb85467e, /// 0x104
			 0xf467af41, /// 0x108
			 0xee537480, /// 0x10c
			 0xf0f7d2bd, /// 0x110
			 0xe8dbb925, /// 0x114
			 0xc75d9741, /// 0x118
			 0xe846c9fd, /// 0x11c
			 0x1c59b241, /// 0x120
			 0xb50ea7b0, /// 0x124
			 0x3d326fb9, /// 0x128
			 0xd3e67d59, /// 0x12c
			 0xd78b658e, /// 0x130
			 0x4684542d, /// 0x134
			 0x7bda0be1, /// 0x138
			 0xe9ccb983, /// 0x13c
			 0xdfb84a43, /// 0x140
			 0xd47c03c3, /// 0x144
			 0xc77599b9, /// 0x148
			 0xbecb953b, /// 0x14c
			 0x9c37465c, /// 0x150
			 0x4efc2d76, /// 0x154
			 0xaad60107, /// 0x158
			 0x6e13da47, /// 0x15c
			 0x7e6919f4, /// 0x160
			 0x7f22d4cc, /// 0x164
			 0x09d86212, /// 0x168
			 0xfffde19f, /// 0x16c
			 0x388e173f, /// 0x170
			 0x839c7369, /// 0x174
			 0x4d37297f, /// 0x178
			 0x26a8e784, /// 0x17c
			 0x285d29e9, /// 0x180
			 0x5c9efd0d, /// 0x184
			 0xe0b5e615, /// 0x188
			 0x27bfd9b3, /// 0x18c
			 0xcf09dcbc, /// 0x190
			 0x980ce359, /// 0x194
			 0x0149fa29, /// 0x198
			 0xdf406a5b, /// 0x19c
			 0xd34c4fc7, /// 0x1a0
			 0xc9091578, /// 0x1a4
			 0x33d8b62d, /// 0x1a8
			 0x155692aa, /// 0x1ac
			 0x1a9d7f8e, /// 0x1b0
			 0x3fca4019, /// 0x1b4
			 0x01794403, /// 0x1b8
			 0xbb92209a, /// 0x1bc
			 0x64c941a7, /// 0x1c0
			 0x30739e1e, /// 0x1c4
			 0x9eda65a1, /// 0x1c8
			 0x3c798f44, /// 0x1cc
			 0xf12690a7, /// 0x1d0
			 0xabc08b77, /// 0x1d4
			 0xf392eabe, /// 0x1d8
			 0xcc6c49cf, /// 0x1dc
			 0x9c0b88c5, /// 0x1e0
			 0xfc9d8b03, /// 0x1e4
			 0x80b7c471, /// 0x1e8
			 0x4b231c17, /// 0x1ec
			 0x80900235, /// 0x1f0
			 0x4930e262, /// 0x1f4
			 0xa9cf4bed, /// 0x1f8
			 0x0611cd0c, /// 0x1fc
			 0x96b8aaf5, /// 0x200
			 0x5a3e2833, /// 0x204
			 0x6d809648, /// 0x208
			 0x5f4584b5, /// 0x20c
			 0xe5e99ba4, /// 0x210
			 0x6b586b2d, /// 0x214
			 0x81518cd5, /// 0x218
			 0x0f39faec, /// 0x21c
			 0xacca0195, /// 0x220
			 0xb40ebe06, /// 0x224
			 0x48a2cd62, /// 0x228
			 0x74e876de, /// 0x22c
			 0xf5825122, /// 0x230
			 0x0b146108, /// 0x234
			 0x40114fab, /// 0x238
			 0x26a1951d, /// 0x23c
			 0xacd40ec3, /// 0x240
			 0x72f5feb9, /// 0x244
			 0xb21568f1, /// 0x248
			 0x13891e55, /// 0x24c
			 0x2ca2107e, /// 0x250
			 0x21043cc9, /// 0x254
			 0xd0f00683, /// 0x258
			 0xdb59c5b2, /// 0x25c
			 0xeecf7a1d, /// 0x260
			 0x7e5c1ac7, /// 0x264
			 0x52febc40, /// 0x268
			 0xbba2ad40, /// 0x26c
			 0x23adf666, /// 0x270
			 0x04a73a62, /// 0x274
			 0xec2a6ab2, /// 0x278
			 0x34cb4568, /// 0x27c
			 0xfc576cbe, /// 0x280
			 0x88abbaf5, /// 0x284
			 0xb5b7a651, /// 0x288
			 0x2000fdcf, /// 0x28c
			 0x81ce656e, /// 0x290
			 0xdc538f3b, /// 0x294
			 0xd5838885, /// 0x298
			 0x5959334d, /// 0x29c
			 0xbfb7d3bc, /// 0x2a0
			 0x24897409, /// 0x2a4
			 0x4a1593ee, /// 0x2a8
			 0x717d4576, /// 0x2ac
			 0x86e91539, /// 0x2b0
			 0xa5d79410, /// 0x2b4
			 0xf8e8a054, /// 0x2b8
			 0x80ebe48e, /// 0x2bc
			 0x8cdf15f2, /// 0x2c0
			 0x1c2ccbb2, /// 0x2c4
			 0x5245778b, /// 0x2c8
			 0x35f9ad34, /// 0x2cc
			 0x7ea61d8d, /// 0x2d0
			 0x040f1734, /// 0x2d4
			 0x1a14a8db, /// 0x2d8
			 0xa07a5e17, /// 0x2dc
			 0x495229d3, /// 0x2e0
			 0x97306f93, /// 0x2e4
			 0xc03d4fbf, /// 0x2e8
			 0x1edab01b, /// 0x2ec
			 0x9236655c, /// 0x2f0
			 0x7c2c4b12, /// 0x2f4
			 0x2385ada7, /// 0x2f8
			 0x2236d648, /// 0x2fc
			 0x12b4557d, /// 0x300
			 0xf54ebfa2, /// 0x304
			 0x5d2e773f, /// 0x308
			 0x639c9bd4, /// 0x30c
			 0x8358339b, /// 0x310
			 0x6ce0d735, /// 0x314
			 0x629aa1c9, /// 0x318
			 0x1c98d821, /// 0x31c
			 0xe135a01f, /// 0x320
			 0x00af4ccf, /// 0x324
			 0x29566a8e, /// 0x328
			 0x76e79ee8, /// 0x32c
			 0xa5c889ed, /// 0x330
			 0x4bc5ba57, /// 0x334
			 0x002ffa1b, /// 0x338
			 0x802c79a4, /// 0x33c
			 0x0f193d50, /// 0x340
			 0x1caf7734, /// 0x344
			 0xf3e6016f, /// 0x348
			 0x23efb64a, /// 0x34c
			 0x5f00cc97, /// 0x350
			 0xfd48750f, /// 0x354
			 0x2ceeeb6d, /// 0x358
			 0xe3f42c9a, /// 0x35c
			 0x730e83b5, /// 0x360
			 0x41feb75b, /// 0x364
			 0x47eb5e28, /// 0x368
			 0xe227af3a, /// 0x36c
			 0x81ec0f4c, /// 0x370
			 0xd42fbdc0, /// 0x374
			 0x4d6209f2, /// 0x378
			 0x26c4d347, /// 0x37c
			 0xa004c0ed, /// 0x380
			 0x4f96ef9b, /// 0x384
			 0x7219c3d4, /// 0x388
			 0x8b2a4428, /// 0x38c
			 0x48dbad61, /// 0x390
			 0x45332fd4, /// 0x394
			 0xcd9431d0, /// 0x398
			 0x7e30e756, /// 0x39c
			 0x4a60f91e, /// 0x3a0
			 0x22e5fd81, /// 0x3a4
			 0x34dbd1ce, /// 0x3a8
			 0x81185e49, /// 0x3ac
			 0x09ef6699, /// 0x3b0
			 0x26a8e522, /// 0x3b4
			 0x94631702, /// 0x3b8
			 0x0317c29d, /// 0x3bc
			 0x27dfea0a, /// 0x3c0
			 0x27efb73e, /// 0x3c4
			 0xf2290fa7, /// 0x3c8
			 0xf401fac1, /// 0x3cc
			 0x37d30a35, /// 0x3d0
			 0xa4ed46d9, /// 0x3d4
			 0xe1d89a7c, /// 0x3d8
			 0x6b641ab1, /// 0x3dc
			 0x5420df3f, /// 0x3e0
			 0xb6506d8a, /// 0x3e4
			 0xe749a60e, /// 0x3e8
			 0xf4b720ce, /// 0x3ec
			 0x3f8777e9, /// 0x3f0
			 0xef9e0835, /// 0x3f4
			 0xd910a6ce, /// 0x3f8
			 0xdc405c65, /// 0x3fc
			 0x0207e11b, /// 0x400
			 0x50cdd146, /// 0x404
			 0x62195f29, /// 0x408
			 0x8c1d61bf, /// 0x40c
			 0x76e1b901, /// 0x410
			 0xa684e1fe, /// 0x414
			 0xd5a0774a, /// 0x418
			 0xcb5e67e1, /// 0x41c
			 0x7fa8f0bd, /// 0x420
			 0xfebe1d97, /// 0x424
			 0x753db6c5, /// 0x428
			 0xf6e46d63, /// 0x42c
			 0x1035d7df, /// 0x430
			 0xbc00e1b2, /// 0x434
			 0xf6146aa9, /// 0x438
			 0x9d40676a, /// 0x43c
			 0x71de9f9b, /// 0x440
			 0x02a53a87, /// 0x444
			 0x2017ff3f, /// 0x448
			 0x09a5a55b, /// 0x44c
			 0x9095e0a2, /// 0x450
			 0x0f1ff3d3, /// 0x454
			 0x8490e9e5, /// 0x458
			 0xc5db8b54, /// 0x45c
			 0x0f23fd13, /// 0x460
			 0x9ad39759, /// 0x464
			 0xae94b028, /// 0x468
			 0xa98638c6, /// 0x46c
			 0xaf885312, /// 0x470
			 0x72c679e0, /// 0x474
			 0xca59fa36, /// 0x478
			 0x5966c611, /// 0x47c
			 0x3e4b7184, /// 0x480
			 0x8deefc96, /// 0x484
			 0xb3fb116e, /// 0x488
			 0xe4a3ec50, /// 0x48c
			 0x929efce9, /// 0x490
			 0xffe28f7d, /// 0x494
			 0x51a94028, /// 0x498
			 0x8414db54, /// 0x49c
			 0x94b9851c, /// 0x4a0
			 0x1e65c8d7, /// 0x4a4
			 0x32cbabd8, /// 0x4a8
			 0xcd455ed3, /// 0x4ac
			 0x6c01216b, /// 0x4b0
			 0x39b63329, /// 0x4b4
			 0x6527758e, /// 0x4b8
			 0x051f2068, /// 0x4bc
			 0xfbb4f810, /// 0x4c0
			 0x2c5aa3e2, /// 0x4c4
			 0x34838cae, /// 0x4c8
			 0x1869c572, /// 0x4cc
			 0xaf2ff7ba, /// 0x4d0
			 0x46152e92, /// 0x4d4
			 0x589c490e, /// 0x4d8
			 0x5958df8a, /// 0x4dc
			 0x99986afb, /// 0x4e0
			 0x0ba7b18b, /// 0x4e4
			 0x7a8bd44b, /// 0x4e8
			 0xdf8d31cb, /// 0x4ec
			 0xdd68f9d7, /// 0x4f0
			 0xb738b5cf, /// 0x4f4
			 0xedaa5e24, /// 0x4f8
			 0xec2830af, /// 0x4fc
			 0x8007a491, /// 0x500
			 0xc6ac9d68, /// 0x504
			 0x12c47892, /// 0x508
			 0xcebe07b1, /// 0x50c
			 0x6c2c9b01, /// 0x510
			 0x94dcdc4f, /// 0x514
			 0x9f917ec7, /// 0x518
			 0xbd7c50fe, /// 0x51c
			 0x90545dc0, /// 0x520
			 0x172c672d, /// 0x524
			 0x95fe9a09, /// 0x528
			 0xa6920976, /// 0x52c
			 0x6daf6a42, /// 0x530
			 0x78a95352, /// 0x534
			 0xe24eee43, /// 0x538
			 0x0244897c, /// 0x53c
			 0xb092c5ea, /// 0x540
			 0x45c711ff, /// 0x544
			 0x42516791, /// 0x548
			 0x43a76a33, /// 0x54c
			 0x5d9c8066, /// 0x550
			 0xcfeca0af, /// 0x554
			 0xbd32152c, /// 0x558
			 0xdd9af04b, /// 0x55c
			 0xf4434d72, /// 0x560
			 0xd1e0f969, /// 0x564
			 0xa0a6e827, /// 0x568
			 0xbecf64eb, /// 0x56c
			 0xb3a89568, /// 0x570
			 0xbb98c1d2, /// 0x574
			 0x743cc3e5, /// 0x578
			 0x3ca433b0, /// 0x57c
			 0x3c81c8c6, /// 0x580
			 0x5e34419f, /// 0x584
			 0xe7acfb96, /// 0x588
			 0xdf2f6ba9, /// 0x58c
			 0x9b979e8d, /// 0x590
			 0x0ee4eb8e, /// 0x594
			 0xfcf37657, /// 0x598
			 0xc083eb76, /// 0x59c
			 0xeb91064c, /// 0x5a0
			 0xfca90be1, /// 0x5a4
			 0xd563637c, /// 0x5a8
			 0x5763ff23, /// 0x5ac
			 0x0015e7fa, /// 0x5b0
			 0x9d9c20d3, /// 0x5b4
			 0x74fdd566, /// 0x5b8
			 0x8b2ede6f, /// 0x5bc
			 0xabf22a65, /// 0x5c0
			 0x639366f2, /// 0x5c4
			 0xcacfc348, /// 0x5c8
			 0x65343ef6, /// 0x5cc
			 0xe95c4652, /// 0x5d0
			 0x69a4774d, /// 0x5d4
			 0x36abe0a5, /// 0x5d8
			 0x04b22974, /// 0x5dc
			 0x004577dd, /// 0x5e0
			 0x15af242d, /// 0x5e4
			 0xe8a3307d, /// 0x5e8
			 0x1589e867, /// 0x5ec
			 0x85793bb9, /// 0x5f0
			 0xc61f33a8, /// 0x5f4
			 0x87a00a68, /// 0x5f8
			 0x911c54ee, /// 0x5fc
			 0xcae40349, /// 0x600
			 0x247e187b, /// 0x604
			 0xad6956f4, /// 0x608
			 0xb0d49918, /// 0x60c
			 0xa73b4cda, /// 0x610
			 0x7d0479f5, /// 0x614
			 0x90055393, /// 0x618
			 0x5ea120f5, /// 0x61c
			 0x12a81983, /// 0x620
			 0x93c0a611, /// 0x624
			 0x9e392a22, /// 0x628
			 0x3461e91b, /// 0x62c
			 0x692c17fd, /// 0x630
			 0x6b75ceb9, /// 0x634
			 0x56c61f76, /// 0x638
			 0x03dcf40a, /// 0x63c
			 0xc93f753b, /// 0x640
			 0x121b3482, /// 0x644
			 0x26f7de4b, /// 0x648
			 0xc95afacc, /// 0x64c
			 0xbae7b455, /// 0x650
			 0x416cc3f1, /// 0x654
			 0x04e54cdd, /// 0x658
			 0xe5c5833d, /// 0x65c
			 0x30c85a67, /// 0x660
			 0x48a08582, /// 0x664
			 0x3bf94876, /// 0x668
			 0x2c558bac, /// 0x66c
			 0xb2ea7a07, /// 0x670
			 0xab0d6184, /// 0x674
			 0x4a6f3dc0, /// 0x678
			 0x8658484f, /// 0x67c
			 0xbdab48da, /// 0x680
			 0x51c3be3c, /// 0x684
			 0x5747107a, /// 0x688
			 0x3e23c078, /// 0x68c
			 0xb87e5b79, /// 0x690
			 0x6f5fb04d, /// 0x694
			 0xc0fcbb95, /// 0x698
			 0x956c887d, /// 0x69c
			 0x0bafb47c, /// 0x6a0
			 0x064b4627, /// 0x6a4
			 0x7973ad14, /// 0x6a8
			 0xde7840ce, /// 0x6ac
			 0x4993ac1f, /// 0x6b0
			 0x9305b586, /// 0x6b4
			 0xcd894e17, /// 0x6b8
			 0xe2aba833, /// 0x6bc
			 0x17d5794f, /// 0x6c0
			 0x7aaadb8b, /// 0x6c4
			 0xb2b9f9b0, /// 0x6c8
			 0x0b2bdbdf, /// 0x6cc
			 0xa67a2eed, /// 0x6d0
			 0x94e3cf00, /// 0x6d4
			 0xffaf463c, /// 0x6d8
			 0x5ebf4edf, /// 0x6dc
			 0x20eeda79, /// 0x6e0
			 0x945c49b5, /// 0x6e4
			 0x7d953124, /// 0x6e8
			 0x2b8379f1, /// 0x6ec
			 0x37a9478c, /// 0x6f0
			 0x79c3c15b, /// 0x6f4
			 0xa5df5bef, /// 0x6f8
			 0x63a742c7, /// 0x6fc
			 0x625be6e1, /// 0x700
			 0xd6904622, /// 0x704
			 0x4cbe138c, /// 0x708
			 0x1841d7b8, /// 0x70c
			 0xddc47975, /// 0x710
			 0x030b237f, /// 0x714
			 0x5a25111d, /// 0x718
			 0x0834e782, /// 0x71c
			 0x4ffe0a7d, /// 0x720
			 0xb7dcc1b3, /// 0x724
			 0x5d7cdc56, /// 0x728
			 0xde5f7d7c, /// 0x72c
			 0xf2d83def, /// 0x730
			 0x72790d6e, /// 0x734
			 0xdea2c3d4, /// 0x738
			 0x7eea2b13, /// 0x73c
			 0x55d0f77f, /// 0x740
			 0xddd32ad0, /// 0x744
			 0x969605bc, /// 0x748
			 0x18eccf48, /// 0x74c
			 0x42b768a3, /// 0x750
			 0x18e34cf5, /// 0x754
			 0xaffc8ba7, /// 0x758
			 0x80f21357, /// 0x75c
			 0x1f380a6e, /// 0x760
			 0x86d338a4, /// 0x764
			 0x914eede1, /// 0x768
			 0x0dd4901d, /// 0x76c
			 0xf1cb28e3, /// 0x770
			 0x66ec54fe, /// 0x774
			 0x8d6de141, /// 0x778
			 0x718d2f3a, /// 0x77c
			 0xa60d2248, /// 0x780
			 0xcb15f206, /// 0x784
			 0xd8075d37, /// 0x788
			 0xde7a8955, /// 0x78c
			 0x0a62d5ca, /// 0x790
			 0x1b245d17, /// 0x794
			 0xc574a921, /// 0x798
			 0xac04f14c, /// 0x79c
			 0x20491ef2, /// 0x7a0
			 0x3d65a341, /// 0x7a4
			 0xb8227bb3, /// 0x7a8
			 0xb4b5e57b, /// 0x7ac
			 0xd10e7729, /// 0x7b0
			 0xc0728201, /// 0x7b4
			 0x685117b7, /// 0x7b8
			 0xb41e8529, /// 0x7bc
			 0xebd6f725, /// 0x7c0
			 0xf85b6d6d, /// 0x7c4
			 0x58b4d6f0, /// 0x7c8
			 0x6e49916c, /// 0x7cc
			 0x516776fa, /// 0x7d0
			 0xbbe620ee, /// 0x7d4
			 0x601c0fef, /// 0x7d8
			 0x184a1b63, /// 0x7dc
			 0x00c90cfc, /// 0x7e0
			 0x738cfdaa, /// 0x7e4
			 0xf2f52bf1, /// 0x7e8
			 0x2fda6c3b, /// 0x7ec
			 0xe406deac, /// 0x7f0
			 0x86fea615, /// 0x7f4
			 0x869ef2b8, /// 0x7f8
			 0x4fad5b2b, /// 0x7fc
			 0xf9af380a, /// 0x800
			 0x67c52b1d, /// 0x804
			 0x838078f7, /// 0x808
			 0xfc137224, /// 0x80c
			 0xe217b416, /// 0x810
			 0xcee21b25, /// 0x814
			 0x2d252ba8, /// 0x818
			 0x7cc4dc16, /// 0x81c
			 0xcff84a06, /// 0x820
			 0xc4a1dcdb, /// 0x824
			 0xa0157daa, /// 0x828
			 0x1f0f38ef, /// 0x82c
			 0xc1e1d1bb, /// 0x830
			 0xf5e19521, /// 0x834
			 0xc21fdcfb, /// 0x838
			 0xad22980c, /// 0x83c
			 0x32112a48, /// 0x840
			 0xf41dc4de, /// 0x844
			 0x089c9931, /// 0x848
			 0xff1b7310, /// 0x84c
			 0x7889a0da, /// 0x850
			 0x4c019f31, /// 0x854
			 0x0a041e18, /// 0x858
			 0x84490169, /// 0x85c
			 0x86d98ba8, /// 0x860
			 0xb149fba6, /// 0x864
			 0xc77940e6, /// 0x868
			 0x9eb11e03, /// 0x86c
			 0x0da0cef6, /// 0x870
			 0x03b406c7, /// 0x874
			 0x668324cd, /// 0x878
			 0x2da77db0, /// 0x87c
			 0x3482d19b, /// 0x880
			 0x15378574, /// 0x884
			 0x088b8960, /// 0x888
			 0x7ab9a7a9, /// 0x88c
			 0x5dbf54d1, /// 0x890
			 0x8d5b75c7, /// 0x894
			 0x6f490c26, /// 0x898
			 0xe66170e3, /// 0x89c
			 0x5bcc263c, /// 0x8a0
			 0x2c9b5408, /// 0x8a4
			 0xbbb46f0e, /// 0x8a8
			 0xc76161ac, /// 0x8ac
			 0x63a0ffb1, /// 0x8b0
			 0x9ebd84f1, /// 0x8b4
			 0x33081f47, /// 0x8b8
			 0x532e00ca, /// 0x8bc
			 0x194e94b7, /// 0x8c0
			 0x26f3ce37, /// 0x8c4
			 0xac4516e0, /// 0x8c8
			 0x6b1661c3, /// 0x8cc
			 0xd304ff34, /// 0x8d0
			 0x5d9ce7f0, /// 0x8d4
			 0x149a8a78, /// 0x8d8
			 0xec7c38db, /// 0x8dc
			 0xfee28f61, /// 0x8e0
			 0xa5a2e8aa, /// 0x8e4
			 0xfed8fb82, /// 0x8e8
			 0xc0d4811c, /// 0x8ec
			 0x79e150d9, /// 0x8f0
			 0xba754a1d, /// 0x8f4
			 0xcb9835ec, /// 0x8f8
			 0xeae59a60, /// 0x8fc
			 0x9a6d9037, /// 0x900
			 0x2c48ba94, /// 0x904
			 0x67871f49, /// 0x908
			 0x87eee4ee, /// 0x90c
			 0xf518c3b8, /// 0x910
			 0xaedc6c5a, /// 0x914
			 0xc0914f39, /// 0x918
			 0xed23b7e3, /// 0x91c
			 0xbf2fddf2, /// 0x920
			 0x9a262d4b, /// 0x924
			 0x6181b14c, /// 0x928
			 0xbcd5ba4f, /// 0x92c
			 0x38409680, /// 0x930
			 0x327e92f2, /// 0x934
			 0x6629b1af, /// 0x938
			 0xad717699, /// 0x93c
			 0xae97ae52, /// 0x940
			 0xb7992b74, /// 0x944
			 0x5137a93c, /// 0x948
			 0x2c135bed, /// 0x94c
			 0x0621dc1e, /// 0x950
			 0xbe7bc15e, /// 0x954
			 0x151dfedc, /// 0x958
			 0xb698fe25, /// 0x95c
			 0x9865673e, /// 0x960
			 0xa7254798, /// 0x964
			 0x5b765544, /// 0x968
			 0x00399909, /// 0x96c
			 0x4b5e248e, /// 0x970
			 0x0b96ccf5, /// 0x974
			 0x925da76f, /// 0x978
			 0x014497f6, /// 0x97c
			 0xd172ccbe, /// 0x980
			 0x4af2be03, /// 0x984
			 0x05539915, /// 0x988
			 0xf35a3348, /// 0x98c
			 0x3f9487ff, /// 0x990
			 0xb205151c, /// 0x994
			 0x9b4d4871, /// 0x998
			 0xe27e4cdd, /// 0x99c
			 0x8bcafe1d, /// 0x9a0
			 0xe6157135, /// 0x9a4
			 0xe21e8587, /// 0x9a8
			 0x8ac6cb6a, /// 0x9ac
			 0xc3742499, /// 0x9b0
			 0x71388fd7, /// 0x9b4
			 0x8d795a4d, /// 0x9b8
			 0x0be825a2, /// 0x9bc
			 0xa1862ee5, /// 0x9c0
			 0x967b46c9, /// 0x9c4
			 0x143aefc0, /// 0x9c8
			 0x2a0e89dc, /// 0x9cc
			 0xe4519ca5, /// 0x9d0
			 0x660549b9, /// 0x9d4
			 0x16d099fd, /// 0x9d8
			 0xab329d49, /// 0x9dc
			 0x6423776b, /// 0x9e0
			 0x40dea7ce, /// 0x9e4
			 0x03425d7b, /// 0x9e8
			 0xd8e4cc6e, /// 0x9ec
			 0xcf0c0452, /// 0x9f0
			 0x94d850b3, /// 0x9f4
			 0x4e675c8e, /// 0x9f8
			 0x68e29629, /// 0x9fc
			 0x4fc281bd, /// 0xa00
			 0x97706731, /// 0xa04
			 0x282db352, /// 0xa08
			 0xb6b6732b, /// 0xa0c
			 0x58c3b090, /// 0xa10
			 0xa109837a, /// 0xa14
			 0xc8f441bc, /// 0xa18
			 0x0a4aa4c1, /// 0xa1c
			 0x3f6805f1, /// 0xa20
			 0x9999ff61, /// 0xa24
			 0xc6d1aaac, /// 0xa28
			 0x75e84fca, /// 0xa2c
			 0xf3ddf6ef, /// 0xa30
			 0x65eb217f, /// 0xa34
			 0x3217f684, /// 0xa38
			 0xd509d994, /// 0xa3c
			 0xba24fdfc, /// 0xa40
			 0x6169df0c, /// 0xa44
			 0x23d3a0a7, /// 0xa48
			 0x16059fc9, /// 0xa4c
			 0x8bb2b1e3, /// 0xa50
			 0xed04bdff, /// 0xa54
			 0x5d1558e0, /// 0xa58
			 0x8c65c887, /// 0xa5c
			 0x117a33ad, /// 0xa60
			 0x8391452e, /// 0xa64
			 0xc02326af, /// 0xa68
			 0x9bf20558, /// 0xa6c
			 0x1f198e95, /// 0xa70
			 0x8b769905, /// 0xa74
			 0x72305d1e, /// 0xa78
			 0x1c6bbb35, /// 0xa7c
			 0x7ee467ff, /// 0xa80
			 0x28746dd4, /// 0xa84
			 0xd6099aca, /// 0xa88
			 0xd2a56616, /// 0xa8c
			 0x6ea59289, /// 0xa90
			 0xb3360663, /// 0xa94
			 0x293665d9, /// 0xa98
			 0x41dd4613, /// 0xa9c
			 0xd5ae0154, /// 0xaa0
			 0xd0f038bb, /// 0xaa4
			 0x50357aef, /// 0xaa8
			 0xd81fcf0e, /// 0xaac
			 0xa4bfa4f3, /// 0xab0
			 0x16e1da0e, /// 0xab4
			 0x8f266c9d, /// 0xab8
			 0xb8d91d46, /// 0xabc
			 0xd5f23e51, /// 0xac0
			 0xf5537629, /// 0xac4
			 0xa6ce355d, /// 0xac8
			 0xafc569ec, /// 0xacc
			 0xa5f6441f, /// 0xad0
			 0xcf177e3b, /// 0xad4
			 0x8d992530, /// 0xad8
			 0x54927ff9, /// 0xadc
			 0xdc8eb5b6, /// 0xae0
			 0x81e91f7b, /// 0xae4
			 0x46fc3299, /// 0xae8
			 0x6dbcc2fb, /// 0xaec
			 0xdc4b8064, /// 0xaf0
			 0x8f2110a5, /// 0xaf4
			 0xaea3b195, /// 0xaf8
			 0x92a95950, /// 0xafc
			 0x25850caa, /// 0xb00
			 0x0281e0e0, /// 0xb04
			 0x1b71bdb6, /// 0xb08
			 0xe9e8bedd, /// 0xb0c
			 0xed969958, /// 0xb10
			 0xd9fb08d2, /// 0xb14
			 0x174bc42b, /// 0xb18
			 0xed5f8568, /// 0xb1c
			 0xf051500f, /// 0xb20
			 0x76cfdba8, /// 0xb24
			 0x67d3b2ae, /// 0xb28
			 0xeb26837c, /// 0xb2c
			 0x07eda0c3, /// 0xb30
			 0x66d87d0f, /// 0xb34
			 0xf11d6573, /// 0xb38
			 0xf39e9022, /// 0xb3c
			 0x253940b6, /// 0xb40
			 0xd0770f12, /// 0xb44
			 0xa99af85d, /// 0xb48
			 0x44430c8e, /// 0xb4c
			 0xcf985aa3, /// 0xb50
			 0x89cfd315, /// 0xb54
			 0x747e852d, /// 0xb58
			 0x46a8b6ed, /// 0xb5c
			 0x3c6cd7f5, /// 0xb60
			 0x6d5bc5b1, /// 0xb64
			 0xa83eb650, /// 0xb68
			 0xe146dc5a, /// 0xb6c
			 0xb37c14e0, /// 0xb70
			 0x40db8295, /// 0xb74
			 0xa4d9269f, /// 0xb78
			 0x04cb3615, /// 0xb7c
			 0xe2fbc075, /// 0xb80
			 0xa2b9bbb0, /// 0xb84
			 0x1d3c8dca, /// 0xb88
			 0xd5f7d800, /// 0xb8c
			 0x212429fd, /// 0xb90
			 0xe8766e46, /// 0xb94
			 0x80ac58c9, /// 0xb98
			 0xe06dab5b, /// 0xb9c
			 0x13f0803b, /// 0xba0
			 0x0e0b865e, /// 0xba4
			 0xdaf48513, /// 0xba8
			 0xe4c390f9, /// 0xbac
			 0xa2f7c03a, /// 0xbb0
			 0x3f55c543, /// 0xbb4
			 0x48fc8da8, /// 0xbb8
			 0x11b8de86, /// 0xbbc
			 0x3eef5716, /// 0xbc0
			 0xdc501c39, /// 0xbc4
			 0x967339d6, /// 0xbc8
			 0x12ebc375, /// 0xbcc
			 0x37841ca9, /// 0xbd0
			 0xe447995b, /// 0xbd4
			 0x19a294cf, /// 0xbd8
			 0xde43afbc, /// 0xbdc
			 0x4c9b99ba, /// 0xbe0
			 0x248d08cc, /// 0xbe4
			 0x83dfd1d4, /// 0xbe8
			 0xd525a14f, /// 0xbec
			 0x5b9e9e9d, /// 0xbf0
			 0x5d9fa93e, /// 0xbf4
			 0x5e1bfc34, /// 0xbf8
			 0x33703e74, /// 0xbfc
			 0x16354771, /// 0xc00
			 0x0d0f68df, /// 0xc04
			 0xaba62457, /// 0xc08
			 0xd7009510, /// 0xc0c
			 0x5044a441, /// 0xc10
			 0x37cb7d63, /// 0xc14
			 0xc51f30a5, /// 0xc18
			 0xbcf3f747, /// 0xc1c
			 0xf7b1c9cf, /// 0xc20
			 0x6ffbcc84, /// 0xc24
			 0xcbc2efc3, /// 0xc28
			 0x44824aa8, /// 0xc2c
			 0x972d198d, /// 0xc30
			 0xff5aa935, /// 0xc34
			 0x6876ba11, /// 0xc38
			 0xd68eaae6, /// 0xc3c
			 0xecc11768, /// 0xc40
			 0x9c77b20a, /// 0xc44
			 0x30e81f61, /// 0xc48
			 0x0900c886, /// 0xc4c
			 0xb1ffdd0a, /// 0xc50
			 0xc0030f42, /// 0xc54
			 0xa57bbd96, /// 0xc58
			 0xa223f09c, /// 0xc5c
			 0x92712a15, /// 0xc60
			 0xb2440630, /// 0xc64
			 0x57ad540d, /// 0xc68
			 0x2a9f95bc, /// 0xc6c
			 0x745d1c09, /// 0xc70
			 0x1ebd72f7, /// 0xc74
			 0xe6bb8e8e, /// 0xc78
			 0x97070c49, /// 0xc7c
			 0xda683aa6, /// 0xc80
			 0x65b57309, /// 0xc84
			 0xafd60562, /// 0xc88
			 0xf239e519, /// 0xc8c
			 0xbde40bbe, /// 0xc90
			 0x1357ecf8, /// 0xc94
			 0x8079e427, /// 0xc98
			 0xa0cd8210, /// 0xc9c
			 0xf099b827, /// 0xca0
			 0xb01303c3, /// 0xca4
			 0xa8c4f68a, /// 0xca8
			 0xf57ea273, /// 0xcac
			 0xd2a6ab17, /// 0xcb0
			 0xd191560d, /// 0xcb4
			 0xa455452a, /// 0xcb8
			 0xb640cbb9, /// 0xcbc
			 0x33cc09de, /// 0xcc0
			 0xa0b4588f, /// 0xcc4
			 0x42eb3f28, /// 0xcc8
			 0xb18baa3f, /// 0xccc
			 0xdfa61def, /// 0xcd0
			 0xbcc9c39b, /// 0xcd4
			 0x8fd85e8d, /// 0xcd8
			 0x0cfa54de, /// 0xcdc
			 0x4cbd9f93, /// 0xce0
			 0xdc2d517e, /// 0xce4
			 0x5c7c6f5f, /// 0xce8
			 0xbc5be811, /// 0xcec
			 0x4db1fbe2, /// 0xcf0
			 0x000a9fcb, /// 0xcf4
			 0x16650fdf, /// 0xcf8
			 0xf5d05689, /// 0xcfc
			 0xeed04429, /// 0xd00
			 0xebaf5fdc, /// 0xd04
			 0xdc93a15c, /// 0xd08
			 0xcb74c49a, /// 0xd0c
			 0x477f94ff, /// 0xd10
			 0x3ac96438, /// 0xd14
			 0x7895c43a, /// 0xd18
			 0x81e34c0e, /// 0xd1c
			 0xbfdcb7fc, /// 0xd20
			 0x17e4c8d1, /// 0xd24
			 0xfe07f19e, /// 0xd28
			 0xec54f7d2, /// 0xd2c
			 0x03c6e148, /// 0xd30
			 0x0589d186, /// 0xd34
			 0xe190ba63, /// 0xd38
			 0x0fbaedff, /// 0xd3c
			 0x358142d0, /// 0xd40
			 0xab16f13d, /// 0xd44
			 0x3fa5fefe, /// 0xd48
			 0xb72aadc8, /// 0xd4c
			 0xb95da72e, /// 0xd50
			 0x2e31072c, /// 0xd54
			 0xc66dfd03, /// 0xd58
			 0x0977f28d, /// 0xd5c
			 0x1296367e, /// 0xd60
			 0x302836f5, /// 0xd64
			 0xc495dde4, /// 0xd68
			 0x793ad3fb, /// 0xd6c
			 0xe2a3e406, /// 0xd70
			 0x791fb568, /// 0xd74
			 0xeb01e815, /// 0xd78
			 0x89760750, /// 0xd7c
			 0xf2bab781, /// 0xd80
			 0xbe67fe04, /// 0xd84
			 0x335e41a5, /// 0xd88
			 0x34caecf6, /// 0xd8c
			 0xe0e44cbb, /// 0xd90
			 0x49edbca6, /// 0xd94
			 0x1a3efdf1, /// 0xd98
			 0xca003c6f, /// 0xd9c
			 0x57cbfd55, /// 0xda0
			 0xae41e7e0, /// 0xda4
			 0x1cea59dc, /// 0xda8
			 0x87237964, /// 0xdac
			 0x584002bf, /// 0xdb0
			 0xe69b38d3, /// 0xdb4
			 0x2e07e648, /// 0xdb8
			 0x39998177, /// 0xdbc
			 0x60476ef0, /// 0xdc0
			 0xb680b599, /// 0xdc4
			 0x4bc3c844, /// 0xdc8
			 0xb78938c1, /// 0xdcc
			 0x291b3008, /// 0xdd0
			 0x324fd188, /// 0xdd4
			 0x8fc6bf9a, /// 0xdd8
			 0xa3a64c1d, /// 0xddc
			 0xe970e47f, /// 0xde0
			 0xfca2c025, /// 0xde4
			 0x6245edf5, /// 0xde8
			 0x83d7d7ca, /// 0xdec
			 0x16e7cbdd, /// 0xdf0
			 0xe555c09e, /// 0xdf4
			 0x2b271233, /// 0xdf8
			 0xf08af356, /// 0xdfc
			 0x3ffc0dd8, /// 0xe00
			 0x605c0443, /// 0xe04
			 0xf5942361, /// 0xe08
			 0x8b47c57c, /// 0xe0c
			 0x373d326a, /// 0xe10
			 0x08a1b391, /// 0xe14
			 0x6e3eaf3d, /// 0xe18
			 0x094749ab, /// 0xe1c
			 0x1eeb56b1, /// 0xe20
			 0x2ec9307a, /// 0xe24
			 0x3f3a5f36, /// 0xe28
			 0x113a8b2a, /// 0xe2c
			 0x8d1c031f, /// 0xe30
			 0x6fbd33e0, /// 0xe34
			 0x99560072, /// 0xe38
			 0xee4bc629, /// 0xe3c
			 0x1b9cc3c0, /// 0xe40
			 0x39bb0c05, /// 0xe44
			 0x52c81095, /// 0xe48
			 0x1f359b53, /// 0xe4c
			 0x622861ab, /// 0xe50
			 0x8dbf80e6, /// 0xe54
			 0xfe575059, /// 0xe58
			 0xfd4108ef, /// 0xe5c
			 0xd93a4269, /// 0xe60
			 0x96e25e42, /// 0xe64
			 0x827fd748, /// 0xe68
			 0xe71b000c, /// 0xe6c
			 0x8b0add2b, /// 0xe70
			 0xbb152a7e, /// 0xe74
			 0x80bb120e, /// 0xe78
			 0x4e5341bd, /// 0xe7c
			 0x704a2ec7, /// 0xe80
			 0x0b0bf95d, /// 0xe84
			 0xda13f157, /// 0xe88
			 0x04e14016, /// 0xe8c
			 0x48e81ce7, /// 0xe90
			 0x5d848061, /// 0xe94
			 0x1b0034b3, /// 0xe98
			 0xaaf6c558, /// 0xe9c
			 0x4c6ec88e, /// 0xea0
			 0xd256a4c5, /// 0xea4
			 0xe8aa824c, /// 0xea8
			 0x8d3a1ac4, /// 0xeac
			 0xb7ff500f, /// 0xeb0
			 0x22800e6e, /// 0xeb4
			 0x9b66738c, /// 0xeb8
			 0xf54a5c56, /// 0xebc
			 0x53fa5f6e, /// 0xec0
			 0x7934f4b2, /// 0xec4
			 0x418a69ca, /// 0xec8
			 0x083e2850, /// 0xecc
			 0xf1480a91, /// 0xed0
			 0x7137910e, /// 0xed4
			 0x4143a6ce, /// 0xed8
			 0x90e409d4, /// 0xedc
			 0x266ef3e5, /// 0xee0
			 0x99272d38, /// 0xee4
			 0x395dbc50, /// 0xee8
			 0x3cb31cd2, /// 0xeec
			 0x887249c1, /// 0xef0
			 0x0240cb82, /// 0xef4
			 0x00fe7a7f, /// 0xef8
			 0x3b5713f1, /// 0xefc
			 0x222c1d99, /// 0xf00
			 0x4e61af07, /// 0xf04
			 0x2aabb289, /// 0xf08
			 0xc00a67ea, /// 0xf0c
			 0x6806e46f, /// 0xf10
			 0xa0fb5923, /// 0xf14
			 0x8c2d5ea8, /// 0xf18
			 0x0f624a45, /// 0xf1c
			 0x6ad54710, /// 0xf20
			 0xa852302a, /// 0xf24
			 0xf9320c93, /// 0xf28
			 0x65e7e955, /// 0xf2c
			 0x2c6a60fd, /// 0xf30
			 0xe652f392, /// 0xf34
			 0x1db540ff, /// 0xf38
			 0x310ae97d, /// 0xf3c
			 0x03eee223, /// 0xf40
			 0x9b9802e3, /// 0xf44
			 0x99ba4c7b, /// 0xf48
			 0xd36ea842, /// 0xf4c
			 0x8119935a, /// 0xf50
			 0x13c0e112, /// 0xf54
			 0x43c5b2be, /// 0xf58
			 0x16a1542a, /// 0xf5c
			 0xc07d679d, /// 0xf60
			 0x3ce7ea2d, /// 0xf64
			 0x0db372b7, /// 0xf68
			 0x336057c2, /// 0xf6c
			 0x0beaea58, /// 0xf70
			 0xac6d2253, /// 0xf74
			 0xcebd0100, /// 0xf78
			 0x9bbfa626, /// 0xf7c
			 0x7c45821a, /// 0xf80
			 0xd0607ff6, /// 0xf84
			 0x45dfd39c, /// 0xf88
			 0xac1bfba6, /// 0xf8c
			 0xa8dd19ae, /// 0xf90
			 0xc9efb657, /// 0xf94
			 0x4f1493d0, /// 0xf98
			 0x5a7ffc9e, /// 0xf9c
			 0x7918f300, /// 0xfa0
			 0x777b2b8b, /// 0xfa4
			 0xfe21c3ea, /// 0xfa8
			 0x2720dfd2, /// 0xfac
			 0x126a2293, /// 0xfb0
			 0x4d6e0552, /// 0xfb4
			 0xcb700962, /// 0xfb8
			 0x1b11e536, /// 0xfbc
			 0x8dbcf05b, /// 0xfc0
			 0x3d4bafcb, /// 0xfc4
			 0xdc6a8dd2, /// 0xfc8
			 0xb81a5a01, /// 0xfcc
			 0x35594bdc, /// 0xfd0
			 0x34264ac6, /// 0xfd4
			 0x726c37ce, /// 0xfd8
			 0x11230e58, /// 0xfdc
			 0xa453bc63, /// 0xfe0
			 0x0a7d71f6, /// 0xfe4
			 0x3ab8255e, /// 0xfe8
			 0xd9f95ba1, /// 0xfec
			 0x4d05895a, /// 0xff0
			 0x09a6c628, /// 0xff4
			 0x90472cdf, /// 0xff8
			 0x183bf33a /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00000
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00004
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00010
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00014
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00018
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0001c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00020
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00024
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00028
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00034
			 0x00011111,                                                  // 9.7958E-41                                  /// 00038
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0003c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00044
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00048
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00050
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00054
			 0x3f028f5c,                                                  // 0.51                                        /// 00058
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0005c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x80000000,                                                  // -zero                                       /// 00064
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0006c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00078
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0e000007,                                                  // Trailing 1s:                                /// 0008c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00090
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00098
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0009c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000a0
			 0xff800000,                                                  // -inf                                        /// 000a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 000b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000f8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0010c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00110
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00114
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00118
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0011c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00124
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00128
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0012c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00130
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00138
			 0x0e000007,                                                  // Trailing 1s:                                /// 0013c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00148
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0014c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00150
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00154
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00160
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00164
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00168
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0016c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00178
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0017c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00190
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00198
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0019c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 001c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 001d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00200
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00204
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00208
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0020c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00210
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00214
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00218
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0021c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00220
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00228
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00230
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00234
			 0x3f028f5c,                                                  // 0.51                                        /// 00238
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00240
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00244
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00248
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0024c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00258
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0025c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00264
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0026c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00270
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00274
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00278
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00284
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0028c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00294
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00298
			 0x33333333,                                                  // 4 random values                             /// 0029c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 002ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00300
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00304
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00308
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0030c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00310
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00314
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00318
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0031c
			 0x00000000,                                                  // zero                                        /// 00320
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00324
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00338
			 0x0c600000,                                                  // Leading 1s:                                 /// 0033c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00340
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00344
			 0x0c780000,                                                  // Leading 1s:                                 /// 00348
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0035c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00360
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00368
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00370
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00378
			 0x7f800000,                                                  // inf                                         /// 0037c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00384
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00388
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0038c
			 0x3f028f5c,                                                  // 0.51                                        /// 00390
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0039c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003ac
			 0x80000000,                                                  // -zero                                       /// 003b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003d0
			 0x3f028f5c,                                                  // 0.51                                        /// 003d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003f8
			 0xbf028f5c,                                                  // -0.51                                       /// 003fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0e000007,                                                  // Trailing 1s:                                /// 00404
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00408
			 0x7fc00001,                                                  // signaling NaN                               /// 0040c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00414
			 0x33333333,                                                  // 4 random values                             /// 00418
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0042c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00430
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00434
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00438
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0043c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00444
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00448
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0044c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00450
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0045c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0xcb000000,                                                  // -8388608.0                                  /// 00464
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0046c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00478
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0047c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00490
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00498
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0049c
			 0xbf028f5c,                                                  // -0.51                                       /// 004a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 004ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0xbf028f5c,                                                  // -0.51                                       /// 004c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00508
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0050c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x0e000003,                                                  // Trailing 1s:                                /// 00518
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0051c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00520
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00524
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00528
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00530
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00538
			 0xcb000000,                                                  // -8388608.0                                  /// 0053c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00540
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00544
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00548
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0054c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00554
			 0x0c600000,                                                  // Leading 1s:                                 /// 00558
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0055c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0xbf028f5c,                                                  // -0.51                                       /// 00564
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00568
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0056c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00570
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00574
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0057c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00584
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00588
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00590
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00594
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00598
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0059c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x4b000000,                                                  // 8388608.0                                   /// 005a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 005d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005f0
			 0x3f028f5c,                                                  // 0.51                                        /// 005f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00600
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00604
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0060c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00610
			 0x0c780000,                                                  // Leading 1s:                                 /// 00614
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00618
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00620
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00624
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00630
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00634
			 0x0c600000,                                                  // Leading 1s:                                 /// 00638
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00644
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00648
			 0x55555555,                                                  // 4 random values                             /// 0064c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00650
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00658
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0065c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00660
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00664
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0066c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00670
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00674
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00678
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00680
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00684
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x80011111,                                                  // -9.7958E-41                                 /// 0068c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00690
			 0x0c700000,                                                  // Leading 1s:                                 /// 00694
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00698
			 0x7fc00001,                                                  // signaling NaN                               /// 0069c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0xbf028f5c,                                                  // -0.51                                       /// 006a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 006e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80000000,                                                  // -zero                                       /// 00700
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00708
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0070c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00710
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00718
			 0x55555555,                                                  // 4 random values                             /// 0071c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00720
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00724
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00728
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0072c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00738
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0073c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0074c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00750
			 0x0c780000,                                                  // Leading 1s:                                 /// 00754
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00758
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0075c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00768
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00778
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0077c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x00000000,                                                  // zero                                        /// 00784
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00788
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00790
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00794
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00798
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007ec
			 0x55555555,                                                  // 4 random values                             /// 007f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x3f028f5c,                                                  // 0.51                                        /// 00804
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00808
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0080c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00810
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00814
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00818
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00820
			 0x80011111,                                                  // -9.7958E-41                                 /// 00824
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00828
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0082c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00830
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00834
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x80000000,                                                  // -zero                                       /// 0083c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00848
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0084c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00850
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00854
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00858
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0085c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00868
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0086c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00870
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00874
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00878
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0087c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0x0c400000,                                                  // Leading 1s:                                 /// 00884
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00888
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0088c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00898
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0089c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 008f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x00000000,                                                  // zero                                        /// 00900
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00904
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0090c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00914
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00918
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0091c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00924
			 0x0c700000,                                                  // Leading 1s:                                 /// 00928
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0092c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00930
			 0x80000000,                                                  // -zero                                       /// 00934
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00938
			 0xffc00001,                                                  // -signaling NaN                              /// 0093c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00940
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00944
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00948
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0094c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00950
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00954
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00958
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00968
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0096c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00974
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00978
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0xffc00001,                                                  // -signaling NaN                              /// 00980
			 0x00000000,                                                  // zero                                        /// 00984
			 0x55555555,                                                  // 4 random values                             /// 00988
			 0x00011111,                                                  // 9.7958E-41                                  /// 0098c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00994
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00998
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0099c
			 0xff800000,                                                  // -inf                                        /// 009a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x7f800000,                                                  // inf                                         /// 009b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a00
			 0xff800000,                                                  // -inf                                        /// 00a04
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a20
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a24
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a28
			 0xffc00001,                                                  // -signaling NaN                              /// 00a2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a40
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a44
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a54
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a58
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a6c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a70
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a80
			 0xcb000000,                                                  // -8388608.0                                  /// 00a84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a90
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a94
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a9c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00aa8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ab0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ab4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ac0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ac4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00acc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ad0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ad4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ad8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ae0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ae4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ae8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0xff800000,                                                  // -inf                                        /// 00af0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00af4
			 0xbf028f5c,                                                  // -0.51                                       /// 00af8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00afc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b0c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b10
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b14
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b1c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b24
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b38
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b3c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b50
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b60
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b64
			 0xbf028f5c,                                                  // -0.51                                       /// 00b68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b6c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x80000000,                                                  // -zero                                       /// 00b78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b80
			 0x4b000000,                                                  // 8388608.0                                   /// 00b84
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ba0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ba8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bb0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bb4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bb8
			 0x33333333,                                                  // 4 random values                             /// 00bbc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bcc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bd0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00be4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00be8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bf0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bf4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bf8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c00
			 0xbf028f5c,                                                  // -0.51                                       /// 00c04
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c08
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c10
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c18
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c20
			 0xbf028f5c,                                                  // -0.51                                       /// 00c24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c28
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c30
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c40
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c4c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c50
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c78
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c84
			 0x80000000,                                                  // -zero                                       /// 00c88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c8c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c90
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c98
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ca0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ca4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ca8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cb8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cbc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cd8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cdc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ce0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cf0
			 0xffc00001,                                                  // -signaling NaN                              /// 00cf4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cf8
			 0x80000000,                                                  // -zero                                       /// 00cfc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d0c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d14
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d34
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d3c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d5c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d68
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d8c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d98
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00da0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00da4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0xcb000000,                                                  // -8388608.0                                  /// 00db0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00db4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00db8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x33333333,                                                  // 4 random values                             /// 00dc0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dc4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dcc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dd8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ddc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00de4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00df0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00df4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00df8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dfc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e18
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e24
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e2c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e34
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e54
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e64
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e6c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e70
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x3f028f5c,                                                  // 0.51                                        /// 00e78
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e9c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ea4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ea8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00eac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00eb0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00eb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ec4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ec8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ecc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00edc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ee0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ee8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ef0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ef4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ef8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00efc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f08
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f10
			 0xffc00001,                                                  // -signaling NaN                              /// 00f14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f30
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f34
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f38
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f40
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f48
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x33333333,                                                  // 4 random values                             /// 00f64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f68
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f6c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f70
			 0xbf028f5c,                                                  // -0.51                                       /// 00f74
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f78
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f80
			 0x80000000,                                                  // -zero                                       /// 00f84
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f88
			 0xffc00001,                                                  // -signaling NaN                              /// 00f8c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fa4
			 0x80000000,                                                  // -zero                                       /// 00fa8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fb0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fb4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fb8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fbc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0xffc00001,                                                  // -signaling NaN                              /// 00fc4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fc8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fcc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fd4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fd8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fdc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fe0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fe4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fe8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ff0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ff4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ff8
			 0x0c7c0000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00004
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0000c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00010
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00018
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00020
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x55555555,                                                  // 4 random values                             /// 00028
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00030
			 0x00000000,                                                  // zero                                        /// 00034
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00038
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0003c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00040
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00044
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00048
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0004c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00050
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00054
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x0c400000,                                                  // Leading 1s:                                 /// 0005c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00060
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00064
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00068
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00070
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00074
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00078
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00080
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00084
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00088
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0008c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00094
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00098
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0xcb000000,                                                  // -8388608.0                                  /// 000c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00100
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00104
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00108
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0010c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00120
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00124
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00130
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00138
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0013c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00140
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00144
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00148
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0014c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00150
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00158
			 0x0c700000,                                                  // Leading 1s:                                 /// 0015c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00160
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00164
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0016c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00170
			 0x0e000007,                                                  // Trailing 1s:                                /// 00174
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0017c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00184
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00188
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0018c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00190
			 0xffc00001,                                                  // -signaling NaN                              /// 00194
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0019c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001a0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 001c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00200
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00204
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00208
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0020c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00210
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00218
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00220
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00224
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0022c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00230
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00234
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00238
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0023c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00244
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00248
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0024c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00258
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00260
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00264
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00268
			 0xbf028f5c,                                                  // -0.51                                       /// 0026c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00270
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00274
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00278
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0027c
			 0x3f028f5c,                                                  // 0.51                                        /// 00280
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00290
			 0x33333333,                                                  // 4 random values                             /// 00294
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0029c
			 0x0c600000,                                                  // Leading 1s:                                 /// 002a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 002a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002cc
			 0x7f800000,                                                  // inf                                         /// 002d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002dc
			 0xcb000000,                                                  // -8388608.0                                  /// 002e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00300
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00304
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00310
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00318
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0031c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00320
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00328
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0032c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00330
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00334
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00338
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0033c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00340
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00344
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00348
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0034c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00350
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00358
			 0xcb000000,                                                  // -8388608.0                                  /// 0035c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00360
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00364
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00368
			 0x3f028f5c,                                                  // 0.51                                        /// 0036c
			 0x4b000000,                                                  // 8388608.0                                   /// 00370
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00388
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0038c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00390
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00394
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0039c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x7f800000,                                                  // inf                                         /// 003a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003c8
			 0x80000000,                                                  // -zero                                       /// 003cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00400
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00404
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00408
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0040c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00414
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80000000,                                                  // -zero                                       /// 0041c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0xffc00001,                                                  // -signaling NaN                              /// 00424
			 0x0c780000,                                                  // Leading 1s:                                 /// 00428
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x55555555,                                                  // 4 random values                             /// 00430
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00438
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0043c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00440
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00448
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0044c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00450
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00454
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00458
			 0x80011111,                                                  // -9.7958E-41                                 /// 0045c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00460
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00464
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00468
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0046c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00470
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00484
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0048c
			 0x7fc00001,                                                  // signaling NaN                               /// 00490
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00494
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00498
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0049c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x7f800000,                                                  // inf                                         /// 004d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00504
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00508
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0050c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00510
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00514
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00518
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00524
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0xbf028f5c,                                                  // -0.51                                       /// 00534
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00538
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0053c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00540
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00544
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0054c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00550
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00554
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00558
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00564
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00568
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0056c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00570
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00574
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00578
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0057c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0c400000,                                                  // Leading 1s:                                 /// 00584
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00588
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0058c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00590
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00594
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0059c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 005a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005e0
			 0x3f028f5c,                                                  // 0.51                                        /// 005e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005f0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 005fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00604
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0060c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00618
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00620
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00624
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00628
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00634
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00638
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0063c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x33333333,                                                  // 4 random values                             /// 00644
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0064c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00650
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00654
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00658
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00660
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00664
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00668
			 0x0e000007,                                                  // Trailing 1s:                                /// 0066c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00670
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00674
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00680
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00684
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0068c
			 0x33333333,                                                  // 4 random values                             /// 00690
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00694
			 0x55555555,                                                  // 4 random values                             /// 00698
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x80000000,                                                  // -zero                                       /// 006c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x80000000,                                                  // -zero                                       /// 006d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00700
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00704
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00708
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00710
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00714
			 0x3f028f5c,                                                  // 0.51                                        /// 00718
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0071c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00724
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00728
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00738
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00744
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00748
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00754
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00760
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00764
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x7fc00001,                                                  // signaling NaN                               /// 00770
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00774
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00778
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0077c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00784
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00788
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0078c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00790
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00798
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00800
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00804
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00808
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0080c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00810
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00814
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00818
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0081c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00820
			 0x7fc00001,                                                  // signaling NaN                               /// 00824
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00828
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x80000000,                                                  // -zero                                       /// 00834
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00838
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0083c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00844
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00848
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0084c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00850
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00854
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00858
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00860
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00864
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00868
			 0x00011111,                                                  // 9.7958E-41                                  /// 0086c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00874
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0087c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00880
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00884
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00898
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0089c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008e0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00904
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00908
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0090c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0xbf028f5c,                                                  // -0.51                                       /// 00914
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0091c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00920
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00924
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0092c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00934
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0093c
			 0x3f028f5c,                                                  // 0.51                                        /// 00940
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00944
			 0x0c700000,                                                  // Leading 1s:                                 /// 00948
			 0x0c780000,                                                  // Leading 1s:                                 /// 0094c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00950
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00954
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00958
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00960
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00964
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00968
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0096c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00970
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00980
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00984
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00988
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00990
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00994
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00998
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0099c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0xcb000000,                                                  // -8388608.0                                  /// 009d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a0c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a1c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a50
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a60
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0xbf028f5c,                                                  // -0.51                                       /// 00a78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a8c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a94
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a98
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00aa0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00aac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ab0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ab4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00abc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ac4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ac8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ad0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ad4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ad8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00adc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ae0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ae4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ae8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00aec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00af8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00afc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b08
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b10
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b24
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b40
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b48
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b54
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b58
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b60
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b64
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b7c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b80
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b88
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b90
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b94
			 0x33333333,                                                  // 4 random values                             /// 00b98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b9c
			 0x7fc00001,                                                  // signaling NaN                               /// 00ba0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ba4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bb0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bb4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bbc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bc8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bcc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bd0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bdc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bf4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bf8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bfc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c04
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c0c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c24
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c28
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c2c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c34
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c3c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c54
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c58
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x3f028f5c,                                                  // 0.51                                        /// 00c78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c7c
			 0x7f800000,                                                  // inf                                         /// 00c80
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c84
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c88
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c94
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x80000000,                                                  // -zero                                       /// 00c9c
			 0xbf028f5c,                                                  // -0.51                                       /// 00ca0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ca4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0xff800000,                                                  // -inf                                        /// 00cb4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cb8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x00000000,                                                  // zero                                        /// 00cc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cdc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ce0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ce8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cf0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cf4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cf8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d08
			 0x80000000,                                                  // -zero                                       /// 00d0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d20
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d34
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d44
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d50
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d5c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d88
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d8c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d98
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00da4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00da8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00db0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00db4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00db8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dc0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dc4
			 0x3f028f5c,                                                  // 0.51                                        /// 00dc8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dcc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ddc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00de0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00de4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00de8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00df4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00df8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e00
			 0xff800000,                                                  // -inf                                        /// 00e04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e0c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e14
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x7f800000,                                                  // inf                                         /// 00e34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e48
			 0x7fc00001,                                                  // signaling NaN                               /// 00e4c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e50
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e54
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e58
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e74
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e7c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e88
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x55555555,                                                  // 4 random values                             /// 00e90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e94
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e98
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00ea0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ea4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ea8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00eac
			 0x7fc00001,                                                  // signaling NaN                               /// 00eb0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ebc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ec4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ecc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ed0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ed4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ee0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ee4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ee8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00eec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ef0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ef4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00efc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f00
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f04
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f08
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f14
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f2c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f38
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f3c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f40
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f48
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f5c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f64
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f78
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f8c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fa4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fa8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fb4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fc0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fc8
			 0xcb000000,                                                  // -8388608.0                                  /// 00fcc
			 0x33333333,                                                  // 4 random values                             /// 00fd0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fd8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fe0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fe4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ff4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ff8
			 0xffbfffff // SNaN                                          // SP - ve numbers                             /// last
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
			 0x00000000,
			 0x00000168,
			 0x00000364,
			 0x000004c4,
			 0x00000524,
			 0x0000009c,
			 0x00000740,
			 0x000003a0,

			 /// vpu register f2
			 0x41300000,
			 0x41700000,
			 0x41e00000,
			 0x41500000,
			 0x40a00000,
			 0x40c00000,
			 0x41000000,
			 0x41900000,

			 /// vpu register f3
			 0x41900000,
			 0x41900000,
			 0x40000000,
			 0x41f80000,
			 0x41000000,
			 0x41880000,
			 0x41b80000,
			 0x41e00000,

			 /// vpu register f4
			 0x40000000,
			 0x41a80000,
			 0x41700000,
			 0x41f00000,
			 0x41800000,
			 0x41000000,
			 0x41400000,
			 0x41600000,

			 /// vpu register f5
			 0x41e00000,
			 0x3f800000,
			 0x41c00000,
			 0x40400000,
			 0x41f80000,
			 0x41b00000,
			 0x41b80000,
			 0x41d80000,

			 /// vpu register f6
			 0x41f00000,
			 0x40c00000,
			 0x41980000,
			 0x41600000,
			 0x41b00000,
			 0x41e00000,
			 0x41f00000,
			 0x41e80000,

			 /// vpu register f7
			 0x40400000,
			 0x40a00000,
			 0x41f00000,
			 0x41000000,
			 0x41f80000,
			 0x40800000,
			 0x41300000,
			 0x41500000,

			 /// vpu register f8
			 0x40800000,
			 0x40a00000,
			 0x41000000,
			 0x42000000,
			 0x41400000,
			 0x40c00000,
			 0x41200000,
			 0x41600000,

			 /// vpu register f9
			 0x41e00000,
			 0x40e00000,
			 0x41600000,
			 0x42000000,
			 0x41200000,
			 0x41300000,
			 0x41d80000,
			 0x41f00000,

			 /// vpu register f10
			 0x40000000,
			 0x41f80000,
			 0x41000000,
			 0x40400000,
			 0x40000000,
			 0x41600000,
			 0x41b80000,
			 0x41600000,

			 /// vpu register f11
			 0x41b00000,
			 0x41500000,
			 0x41c80000,
			 0x41f00000,
			 0x41b00000,
			 0x40a00000,
			 0x41d80000,
			 0x41000000,

			 /// vpu register f12
			 0x40e00000,
			 0x40a00000,
			 0x41900000,
			 0x41700000,
			 0x3f800000,
			 0x41880000,
			 0x41500000,
			 0x41800000,

			 /// vpu register f13
			 0x41500000,
			 0x41a80000,
			 0x41980000,
			 0x41c80000,
			 0x41b80000,
			 0x41980000,
			 0x40400000,
			 0x41f80000,

			 /// vpu register f14
			 0x41c00000,
			 0x41200000,
			 0x41700000,
			 0x41200000,
			 0x41500000,
			 0x41e80000,
			 0x40c00000,
			 0x41d80000,

			 /// vpu register f15
			 0x41a00000,
			 0x40a00000,
			 0x41200000,
			 0x41e00000,
			 0x42000000,
			 0x40a00000,
			 0x41100000,
			 0x40800000,

			 /// vpu register f16
			 0x41900000,
			 0x41e80000,
			 0x40400000,
			 0x41c80000,
			 0x41500000,
			 0x41e80000,
			 0x41600000,
			 0x40a00000,

			 /// vpu register f17
			 0x41880000,
			 0x41880000,
			 0x41500000,
			 0x41b00000,
			 0x41c80000,
			 0x40000000,
			 0x41b00000,
			 0x41a80000,

			 /// vpu register f18
			 0x40c00000,
			 0x42000000,
			 0x41000000,
			 0x3f800000,
			 0x41e80000,
			 0x41980000,
			 0x40a00000,
			 0x42000000,

			 /// vpu register f19
			 0x41b00000,
			 0x41500000,
			 0x40c00000,
			 0x41c00000,
			 0x41300000,
			 0x41100000,
			 0x40a00000,
			 0x41600000,

			 /// vpu register f20
			 0x41900000,
			 0x41500000,
			 0x41d00000,
			 0x41f80000,
			 0x41d80000,
			 0x41a00000,
			 0x40000000,
			 0x40c00000,

			 /// vpu register f21
			 0x41700000,
			 0x41d00000,
			 0x41800000,
			 0x40400000,
			 0x41100000,
			 0x41c80000,
			 0x41a80000,
			 0x41e00000,

			 /// vpu register f22
			 0x41f00000,
			 0x40400000,
			 0x42000000,
			 0x41e00000,
			 0x40000000,
			 0x41600000,
			 0x41a00000,
			 0x41d00000,

			 /// vpu register f23
			 0x41a80000,
			 0x41b00000,
			 0x41880000,
			 0x40e00000,
			 0x40400000,
			 0x41e80000,
			 0x41100000,
			 0x41e80000,

			 /// vpu register f24
			 0x41f80000,
			 0x41980000,
			 0x41f80000,
			 0x41100000,
			 0x40a00000,
			 0x40e00000,
			 0x41900000,
			 0x40e00000,

			 /// vpu register f25
			 0x41880000,
			 0x41880000,
			 0x41100000,
			 0x42000000,
			 0x41a80000,
			 0x41300000,
			 0x41000000,
			 0x40400000,

			 /// vpu register f26
			 0x40a00000,
			 0x41e80000,
			 0x42000000,
			 0x41e00000,
			 0x41400000,
			 0x41500000,
			 0x40c00000,
			 0x41a00000,

			 /// vpu register f27
			 0x41f80000,
			 0x41f80000,
			 0x41b80000,
			 0x3f800000,
			 0x41c00000,
			 0x42000000,
			 0x41c00000,
			 0x40800000,

			 /// vpu register f28
			 0x41c00000,
			 0x41f80000,
			 0x41e00000,
			 0x41700000,
			 0x40a00000,
			 0x41c80000,
			 0x40a00000,
			 0x41880000,

			 /// vpu register f29
			 0x41300000,
			 0x41500000,
			 0x41100000,
			 0x41e80000,
			 0x41400000,
			 0x41d00000,
			 0x41500000,
			 0x41500000,

			 /// vpu register f30
			 0x41a00000,
			 0x40000000,
			 0x41b00000,
			 0x40c00000,
			 0x41b00000,
			 0x41e80000,
			 0x40e00000,
			 0x41000000,

			 /// vpu register f31
			 0x41f80000,
			 0x41500000,
			 0x41b80000,
			 0x41400000,
			 0x41d00000,
			 0x41700000,
			 0x41e80000,
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
		"fsc32b.ps f21, x15 (x12)\n"                          ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f28, x13 (x13)\n"                          ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f2, x14 (x13)\n"                           ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f8, x14 (x14)\n"                           ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f25, x11 (x13)\n"                          ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f12, x12 (x14)\n"                          ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f25, x15 (x15)\n"                          ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f6, x13 (x15)\n"                           ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f1, x11 (x15)\n"                           ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f30, x11 (x12)\n"                          ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f21, x15 (x11)\n"                          ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f7, x14 (x11)\n"                           ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f18, x14 (x12)\n"                          ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f16, x15 (x12)\n"                          ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f12, x11 (x11)\n"                          ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f28, x15 (x15)\n"                          ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f10, x11 (x15)\n"                          ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f13, x12 (x14)\n"                          ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f4, x15 (x14)\n"                           ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f17, x15 (x13)\n"                          ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f28, x14 (x12)\n"                          ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f26, x11 (x13)\n"                          ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f0, x14 (x12)\n"                           ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f19, x15 (x15)\n"                          ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f19, x13 (x14)\n"                          ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f5, x15 (x13)\n"                           ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f30, x11 (x14)\n"                          ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f14, x12 (x12)\n"                          ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f2, x15 (x13)\n"                           ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f20, x13 (x11)\n"                          ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f3, x14 (x14)\n"                           ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f4, x13 (x14)\n"                           ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f26, x11 (x12)\n"                          ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f15, x13 (x15)\n"                          ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f22, x14 (x11)\n"                          ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f26, x13 (x12)\n"                          ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f4, x15 (x11)\n"                           ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f12, x11 (x15)\n"                          ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f4, x15 (x12)\n"                           ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f1, x11 (x13)\n"                           ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f16, x14 (x11)\n"                          ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f18, x15 (x12)\n"                          ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f8, x15 (x12)\n"                           ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f15, x11 (x15)\n"                          ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f11, x11 (x14)\n"                          ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f26, x15 (x11)\n"                          ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f17, x12 (x12)\n"                          ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f26, x15 (x12)\n"                          ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f2, x14 (x13)\n"                           ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f13, x15 (x14)\n"                          ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f29, x13 (x14)\n"                          ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f6, x15 (x11)\n"                           ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f3, x14 (x13)\n"                           ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f15, x14 (x13)\n"                          ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f23, x11 (x15)\n"                          ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f14, x12 (x15)\n"                          ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f12, x11 (x15)\n"                          ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f14, x11 (x14)\n"                          ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f2, x15 (x11)\n"                           ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f28, x13 (x12)\n"                          ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f5, x14 (x13)\n"                           ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f7, x15 (x12)\n"                           ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f8, x11 (x12)\n"                           ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f29, x12 (x13)\n"                          ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f30, x11 (x14)\n"                          ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f28, x15 (x14)\n"                          ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f14, x13 (x13)\n"                          ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f11, x15 (x14)\n"                          ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f11, x13 (x11)\n"                          ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f23, x13 (x13)\n"                          ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f27, x11 (x14)\n"                          ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f30, x14 (x13)\n"                          ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f8, x11 (x11)\n"                           ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f24, x12 (x14)\n"                          ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f24, x15 (x14)\n"                          ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f9, x15 (x15)\n"                           ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f10, x11 (x14)\n"                          ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f0, x14 (x13)\n"                           ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f21, x11 (x11)\n"                          ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f17, x14 (x12)\n"                          ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f6, x12 (x13)\n"                           ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f1, x12 (x14)\n"                           ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f15, x15 (x12)\n"                          ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f14, x13 (x12)\n"                          ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f25, x15 (x13)\n"                          ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f20, x11 (x13)\n"                          ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f13, x15 (x15)\n"                          ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f23, x12 (x12)\n"                          ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f7, x11 (x15)\n"                           ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f13, x14 (x15)\n"                          ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f21, x12 (x12)\n"                          ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f13, x11 (x11)\n"                          ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f26, x15 (x11)\n"                          ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f26, x13 (x15)\n"                          ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f10, x12 (x14)\n"                          ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f11, x12 (x11)\n"                          ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f13, x14 (x15)\n"                          ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f4, x11 (x14)\n"                           ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32b.ps f0, x15 (x11)\n"                           ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
