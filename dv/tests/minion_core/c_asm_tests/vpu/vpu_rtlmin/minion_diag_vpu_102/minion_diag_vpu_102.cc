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
			 0x55555555,                                                  // 4 random values                             /// 00000
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00004
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00008
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0000c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00014
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00018
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00020
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00024
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00028
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0002c
			 0x00000000,                                                  // zero                                        /// 00030
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00034
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00038
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0003c
			 0x7f800000,                                                  // inf                                         /// 00040
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00048
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0004c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00054
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00058
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0005c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x80000000,                                                  // -zero                                       /// 00064
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00068
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0006c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00070
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00074
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00084
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00088
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00094
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00098
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0009c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000c4
			 0xcb000000,                                                  // -8388608.0                                  /// 000c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 000f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00100
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00104
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00108
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0010c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00110
			 0xff800000,                                                  // -inf                                        /// 00114
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00118
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00120
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00124
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00128
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00134
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00138
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0013c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00140
			 0xffc00001,                                                  // -signaling NaN                              /// 00144
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0014c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00150
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00154
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x55555555,                                                  // 4 random values                             /// 0015c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00160
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00164
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x7fc00001,                                                  // signaling NaN                               /// 0016c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00178
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0017c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00180
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00184
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00194
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0019c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001bc
			 0x33333333,                                                  // 4 random values                             /// 001c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 001e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00200
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00204
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00208
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0020c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0021c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00220
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00228
			 0x55555555,                                                  // 4 random values                             /// 0022c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00234
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00238
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00240
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0024c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00258
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0025c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00264
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00268
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00270
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00278
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0xbf028f5c,                                                  // -0.51                                       /// 0028c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00290
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00294
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00298
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0029c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80000000,                                                  // -zero                                       /// 002a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 002ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002b4
			 0x55555555,                                                  // 4 random values                             /// 002b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 002f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00300
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00304
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00308
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0030c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00310
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00314
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0031c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00320
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00324
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00328
			 0x0c400000,                                                  // Leading 1s:                                 /// 0032c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00330
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00334
			 0x00000000,                                                  // zero                                        /// 00338
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0033c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00340
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00344
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00348
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0034c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00354
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00358
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0035c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00364
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00374
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00378
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00380
			 0x0c780000,                                                  // Leading 1s:                                 /// 00384
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00388
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0038c
			 0xff800000,                                                  // -inf                                        /// 00390
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00398
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0039c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003c4
			 0x33333333,                                                  // 4 random values                             /// 003c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00400
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00404
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00408
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00410
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00414
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00418
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0041c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00420
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0042c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00430
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00434
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00438
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00440
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00448
			 0x3f028f5c,                                                  // 0.51                                        /// 0044c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00454
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00458
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0045c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00460
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0046c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00470
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00474
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00478
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0047c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00480
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00484
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0048c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00490
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x00011111,                                                  // 9.7958E-41                                  /// 00498
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 004b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004c8
			 0x33333333,                                                  // 4 random values                             /// 004cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00500
			 0x80000000,                                                  // -zero                                       /// 00504
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00508
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00514
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0051c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00520
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00524
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00528
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0052c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00538
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0053c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00540
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00544
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00548
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00554
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00558
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0055c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00560
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00564
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00568
			 0x80011111,                                                  // -9.7958E-41                                 /// 0056c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00570
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00574
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00578
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0057c
			 0x4b000000,                                                  // 8388608.0                                   /// 00580
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00584
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0058c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00594
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0xcb000000,                                                  // -8388608.0                                  /// 0059c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 005a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b4
			 0xcb000000,                                                  // -8388608.0                                  /// 005b8
			 0xffc00001,                                                  // -signaling NaN                              /// 005bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00600
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00604
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00608
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0060c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00610
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00614
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00618
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00620
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00628
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0062c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0063c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00640
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x80000000,                                                  // -zero                                       /// 00650
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00654
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00658
			 0x0c780000,                                                  // Leading 1s:                                 /// 0065c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00660
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00664
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0066c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00670
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00674
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00678
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0067c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00680
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00684
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00688
			 0xff800000,                                                  // -inf                                        /// 0068c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00698
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0069c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x80000000,                                                  // -zero                                       /// 006ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 006bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0xffc00001,                                                  // -signaling NaN                              /// 006d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 006e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00700
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00704
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00708
			 0xff800000,                                                  // -inf                                        /// 0070c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00714
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0071c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00724
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00730
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00734
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00738
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0073c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00740
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00748
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0xcb000000,                                                  // -8388608.0                                  /// 00750
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00758
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0075c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00760
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00768
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0076c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00770
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0077c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00780
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00784
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00790
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00794
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00798
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0079c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 007f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00800
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00804
			 0x0c400000,                                                  // Leading 1s:                                 /// 00808
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0080c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00810
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00814
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00818
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0081c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00820
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00828
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0082c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00830
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00840
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00844
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00850
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00854
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00858
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00860
			 0x7f800000,                                                  // inf                                         /// 00864
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0086c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00870
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00874
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0087c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00880
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00888
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0088c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00890
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00894
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00898
			 0x3f028f5c,                                                  // 0.51                                        /// 0089c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008e0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00900
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00904
			 0x0e000003,                                                  // Trailing 1s:                                /// 00908
			 0x33333333,                                                  // 4 random values                             /// 0090c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00910
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00914
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00918
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0091c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0092c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00930
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x0e000001,                                                  // Trailing 1s:                                /// 00938
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0093c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00940
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00944
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00948
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00950
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00958
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00960
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00964
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00968
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0096c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0xbf028f5c,                                                  // -0.51                                       /// 00974
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00978
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0097c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00980
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00988
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0xffc00001,                                                  // -signaling NaN                              /// 00990
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00994
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00998
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0099c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009c8
			 0xcb000000,                                                  // -8388608.0                                  /// 009cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009f8
			 0x3f028f5c,                                                  // 0.51                                        /// 009fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a00
			 0xbf028f5c,                                                  // -0.51                                       /// 00a04
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a10
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a14
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a34
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a3c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a50
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a60
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a64
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a6c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a88
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a94
			 0xffc00001,                                                  // -signaling NaN                              /// 00a98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00aa0
			 0xff800000,                                                  // -inf                                        /// 00aa4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00aa8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aac
			 0x7f800000,                                                  // inf                                         /// 00ab0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ab4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ab8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00abc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ac4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ac8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00acc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ad0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00adc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ae0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ae4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ae8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00af0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x4b000000,                                                  // 8388608.0                                   /// 00af8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00afc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b00
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b04
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b10
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b14
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b18
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b2c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b34
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b3c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b4c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b58
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b5c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b64
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b6c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b70
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b74
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x4b000000,                                                  // 8388608.0                                   /// 00b7c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b94
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x55555555,                                                  // 4 random values                             /// 00ba0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ba8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bb0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bb4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bbc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bc0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bd0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bd8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bdc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00be0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00be4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00be8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bf8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bfc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c04
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c0c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c10
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c44
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c48
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c54
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c58
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c78
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c88
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c94
			 0xcb000000,                                                  // -8388608.0                                  /// 00c98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c9c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ca4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ca8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cb0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cb4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cbc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x7fc00001,                                                  // signaling NaN                               /// 00ccc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ce0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ce8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cf8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cfc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d1c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d20
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d24
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d28
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d30
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d34
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d48
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d4c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d54
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d60
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d68
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d6c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d78
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d7c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d88
			 0xffc00001,                                                  // -signaling NaN                              /// 00d8c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d98
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x3f028f5c,                                                  // 0.51                                        /// 00da4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00da8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00db4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dbc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dc4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x80000000,                                                  // -zero                                       /// 00dd0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dd4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ddc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00de0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00de4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00df8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dfc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e00
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e18
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e1c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e20
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e24
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e2c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e38
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e4c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e58
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e64
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e78
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e80
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e8c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e94
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ea0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ea4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ea8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00eb0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00eb4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00eb8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ec4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ecc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ed0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ed8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ee0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ee4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ee8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ef4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ef8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00efc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f08
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f18
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f2c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f38
			 0x7f800000,                                                  // inf                                         /// 00f3c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f4c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x80000000,                                                  // -zero                                       /// 00f58
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f5c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f6c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f74
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f7c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f84
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fa0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fa4
			 0x7fc00001,                                                  // signaling NaN                               /// 00fa8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fb4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fb8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fc4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fcc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fd0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fd4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fd8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fdc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fe4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fe8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ff0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ff4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ff8
			 0x0e1fffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xb574ad7d, /// 0x0
			 0x08a6e7bb, /// 0x4
			 0x123ceb6e, /// 0x8
			 0xde25172a, /// 0xc
			 0xcf8e9ce0, /// 0x10
			 0x05ce686c, /// 0x14
			 0x5639868e, /// 0x18
			 0x4fc2c57a, /// 0x1c
			 0x73fd41d9, /// 0x20
			 0x7d893c42, /// 0x24
			 0x3588c066, /// 0x28
			 0x4d87bef1, /// 0x2c
			 0xb133cf1b, /// 0x30
			 0xdfc4af2a, /// 0x34
			 0xb2424601, /// 0x38
			 0x654109c0, /// 0x3c
			 0x38e4d020, /// 0x40
			 0xb579ccf2, /// 0x44
			 0xe628c474, /// 0x48
			 0x2f6e5804, /// 0x4c
			 0x9981adc1, /// 0x50
			 0x7a7fec19, /// 0x54
			 0x283cc1c0, /// 0x58
			 0x2a7e3f12, /// 0x5c
			 0xd812ae5b, /// 0x60
			 0xf3c042d7, /// 0x64
			 0x69c359a4, /// 0x68
			 0xf76d11ca, /// 0x6c
			 0xbfce4f59, /// 0x70
			 0x6871aaec, /// 0x74
			 0x8e74d221, /// 0x78
			 0x3b478269, /// 0x7c
			 0x20bafc6a, /// 0x80
			 0x9d4bd97a, /// 0x84
			 0xd2c6580f, /// 0x88
			 0x17fa1833, /// 0x8c
			 0x1903b9a7, /// 0x90
			 0x9f034246, /// 0x94
			 0x891ffffb, /// 0x98
			 0x8e77e6e4, /// 0x9c
			 0xb947855c, /// 0xa0
			 0x5459485f, /// 0xa4
			 0xb9950fce, /// 0xa8
			 0x19f5b5fb, /// 0xac
			 0xa93a867e, /// 0xb0
			 0x85fbcc1a, /// 0xb4
			 0xfa08fb56, /// 0xb8
			 0xc9e09d69, /// 0xbc
			 0x6abff0ff, /// 0xc0
			 0xc309556d, /// 0xc4
			 0x335768be, /// 0xc8
			 0x17679e0e, /// 0xcc
			 0x2a5d9baa, /// 0xd0
			 0xd8bf7820, /// 0xd4
			 0x36bf0c52, /// 0xd8
			 0x10697e0d, /// 0xdc
			 0x65a8ed0d, /// 0xe0
			 0x4c0e9306, /// 0xe4
			 0xba6c3a59, /// 0xe8
			 0xfe3b27aa, /// 0xec
			 0x7c6757d5, /// 0xf0
			 0xb7d1d79d, /// 0xf4
			 0x4906ef0a, /// 0xf8
			 0xea56c8b8, /// 0xfc
			 0xdabcd662, /// 0x100
			 0x840b82f6, /// 0x104
			 0xd95ef324, /// 0x108
			 0x3485ae03, /// 0x10c
			 0xd79795f4, /// 0x110
			 0x68ce775c, /// 0x114
			 0x63d9ba4f, /// 0x118
			 0xaeb25a07, /// 0x11c
			 0x40329c92, /// 0x120
			 0x1842b4f4, /// 0x124
			 0xbae61c8f, /// 0x128
			 0xfe6dad75, /// 0x12c
			 0x4ca729f7, /// 0x130
			 0x6cd42fa7, /// 0x134
			 0xf125ae88, /// 0x138
			 0x3ba66d04, /// 0x13c
			 0x604fa481, /// 0x140
			 0x092386ea, /// 0x144
			 0xb9858650, /// 0x148
			 0x9b98cdd8, /// 0x14c
			 0xe8cc351e, /// 0x150
			 0x5a74acbc, /// 0x154
			 0x00d0c805, /// 0x158
			 0xa486ee5e, /// 0x15c
			 0x04c6a3d8, /// 0x160
			 0x3f94b9c6, /// 0x164
			 0xdaf3f637, /// 0x168
			 0xbaa9770e, /// 0x16c
			 0x18b578dc, /// 0x170
			 0x0703922b, /// 0x174
			 0x8dd21efa, /// 0x178
			 0x47caf2a5, /// 0x17c
			 0x7cd8614a, /// 0x180
			 0xccddd13a, /// 0x184
			 0x7293341f, /// 0x188
			 0x017b157f, /// 0x18c
			 0xcec667c3, /// 0x190
			 0xccbfc379, /// 0x194
			 0x96e4a4bd, /// 0x198
			 0xc838887d, /// 0x19c
			 0x6bc930ed, /// 0x1a0
			 0x778056bd, /// 0x1a4
			 0xa5a1033c, /// 0x1a8
			 0x2bac3d2c, /// 0x1ac
			 0x6020fd95, /// 0x1b0
			 0xb70cc51c, /// 0x1b4
			 0x4568ff30, /// 0x1b8
			 0x37310a4f, /// 0x1bc
			 0x64bde518, /// 0x1c0
			 0x5ff1b709, /// 0x1c4
			 0x1b1e5817, /// 0x1c8
			 0xf8ac8fa3, /// 0x1cc
			 0x21ba91f9, /// 0x1d0
			 0xa083336f, /// 0x1d4
			 0x593953f8, /// 0x1d8
			 0x99c58af1, /// 0x1dc
			 0x87df082e, /// 0x1e0
			 0x92d51922, /// 0x1e4
			 0xd2b841d1, /// 0x1e8
			 0x1351e5bc, /// 0x1ec
			 0xe2b6c8e6, /// 0x1f0
			 0x55b802cd, /// 0x1f4
			 0x890d4de9, /// 0x1f8
			 0x4d2cb4b9, /// 0x1fc
			 0xa8af5821, /// 0x200
			 0x2ff6c1e4, /// 0x204
			 0xe42dde70, /// 0x208
			 0x777c375c, /// 0x20c
			 0x627e0582, /// 0x210
			 0xd557eccf, /// 0x214
			 0xd4d47c7b, /// 0x218
			 0xf4274234, /// 0x21c
			 0x30cfe802, /// 0x220
			 0x970283c9, /// 0x224
			 0x61d0d2f1, /// 0x228
			 0xeb54c1c2, /// 0x22c
			 0x48a7e9d5, /// 0x230
			 0x13643f5d, /// 0x234
			 0x10823bb4, /// 0x238
			 0x287705b4, /// 0x23c
			 0x3bbeaec7, /// 0x240
			 0x6b10ce27, /// 0x244
			 0xf15053a7, /// 0x248
			 0x604eadee, /// 0x24c
			 0x6bfa3a41, /// 0x250
			 0x20cc0955, /// 0x254
			 0x4d9139ac, /// 0x258
			 0x12993060, /// 0x25c
			 0x0b19c9eb, /// 0x260
			 0xa7be4a95, /// 0x264
			 0x00af370c, /// 0x268
			 0x4821ec3d, /// 0x26c
			 0xe15c6e0b, /// 0x270
			 0x48087078, /// 0x274
			 0xad4d38b7, /// 0x278
			 0x5bf8b080, /// 0x27c
			 0x0fa4e1da, /// 0x280
			 0x8bdae28c, /// 0x284
			 0xd1504849, /// 0x288
			 0x22ec1cfd, /// 0x28c
			 0x0b4bc5c9, /// 0x290
			 0x8c669b96, /// 0x294
			 0xae4bc298, /// 0x298
			 0xb3930a78, /// 0x29c
			 0xfebf75b7, /// 0x2a0
			 0x49b8344a, /// 0x2a4
			 0x35e7ba96, /// 0x2a8
			 0xe5e2e0b5, /// 0x2ac
			 0x83e3be22, /// 0x2b0
			 0x8ca3cb6e, /// 0x2b4
			 0xf90ae232, /// 0x2b8
			 0xf3485f24, /// 0x2bc
			 0x4db5c33c, /// 0x2c0
			 0x7eda1e07, /// 0x2c4
			 0x5f9fbb7d, /// 0x2c8
			 0x3fd5a1d9, /// 0x2cc
			 0x862ff378, /// 0x2d0
			 0xd9e3b516, /// 0x2d4
			 0x26ecda93, /// 0x2d8
			 0x9d0b2552, /// 0x2dc
			 0xa036e6b7, /// 0x2e0
			 0x61c85f3c, /// 0x2e4
			 0xe2e5b016, /// 0x2e8
			 0x296ffe25, /// 0x2ec
			 0xbcf42718, /// 0x2f0
			 0xb586ce86, /// 0x2f4
			 0xab5bd844, /// 0x2f8
			 0x64a15d4c, /// 0x2fc
			 0xc8262ced, /// 0x300
			 0x669a7df7, /// 0x304
			 0x7e1b77a5, /// 0x308
			 0x77353974, /// 0x30c
			 0xb9958164, /// 0x310
			 0x76900027, /// 0x314
			 0xc8b349df, /// 0x318
			 0x7f48bbd4, /// 0x31c
			 0x24453ea0, /// 0x320
			 0x874abdad, /// 0x324
			 0x872f5ed9, /// 0x328
			 0xf95948f8, /// 0x32c
			 0x8bbdf7ba, /// 0x330
			 0xe1f4700a, /// 0x334
			 0xdde1538f, /// 0x338
			 0x91070640, /// 0x33c
			 0x9877e07b, /// 0x340
			 0xe76d5b81, /// 0x344
			 0x4db70125, /// 0x348
			 0x6e6d3656, /// 0x34c
			 0x579f2f0e, /// 0x350
			 0x0802c5fc, /// 0x354
			 0x76d7c307, /// 0x358
			 0x37e95b55, /// 0x35c
			 0x7a725a09, /// 0x360
			 0x2edbaf3c, /// 0x364
			 0xcd8ec2a3, /// 0x368
			 0xe11fa4ad, /// 0x36c
			 0x6863db7e, /// 0x370
			 0x132af5a4, /// 0x374
			 0xbaccfe36, /// 0x378
			 0x7ae6c25f, /// 0x37c
			 0x320386af, /// 0x380
			 0xf35fabac, /// 0x384
			 0x255074e1, /// 0x388
			 0x280d380c, /// 0x38c
			 0x334ae060, /// 0x390
			 0x1961380b, /// 0x394
			 0xcbbe0d8a, /// 0x398
			 0x68045452, /// 0x39c
			 0xad1478bf, /// 0x3a0
			 0x388da232, /// 0x3a4
			 0x17f907dd, /// 0x3a8
			 0x73ed6553, /// 0x3ac
			 0xf77b0b35, /// 0x3b0
			 0x9a5c00b4, /// 0x3b4
			 0x12d8a429, /// 0x3b8
			 0x79c79b30, /// 0x3bc
			 0xbc727f78, /// 0x3c0
			 0xe7459c3b, /// 0x3c4
			 0xde54164b, /// 0x3c8
			 0x32f25c4d, /// 0x3cc
			 0x7d9b89dd, /// 0x3d0
			 0x42daff4a, /// 0x3d4
			 0x01829d8c, /// 0x3d8
			 0x8aadd625, /// 0x3dc
			 0x126bb618, /// 0x3e0
			 0x12d5b261, /// 0x3e4
			 0x37197b3a, /// 0x3e8
			 0x1292dcc4, /// 0x3ec
			 0x720c5de7, /// 0x3f0
			 0xf932ebd9, /// 0x3f4
			 0x4b33b5d2, /// 0x3f8
			 0x31173611, /// 0x3fc
			 0xd0704d15, /// 0x400
			 0x87a9f50b, /// 0x404
			 0x90f3b80a, /// 0x408
			 0xffdf859c, /// 0x40c
			 0x0b22364f, /// 0x410
			 0x7c365748, /// 0x414
			 0xafe78e8f, /// 0x418
			 0x32acea0b, /// 0x41c
			 0x0fdf7b25, /// 0x420
			 0x1b00b611, /// 0x424
			 0xd8e8c9bf, /// 0x428
			 0x07808692, /// 0x42c
			 0xf4ddfdca, /// 0x430
			 0x9ffc1f96, /// 0x434
			 0x545b5e85, /// 0x438
			 0x82a752ec, /// 0x43c
			 0x08579853, /// 0x440
			 0x721a03f9, /// 0x444
			 0x818b10b5, /// 0x448
			 0xc99a54df, /// 0x44c
			 0x4122468b, /// 0x450
			 0x72b5c0f3, /// 0x454
			 0x926e334a, /// 0x458
			 0xcd42957a, /// 0x45c
			 0x5a0fd23f, /// 0x460
			 0xf388aa5b, /// 0x464
			 0xe8abbae8, /// 0x468
			 0xc641fd7b, /// 0x46c
			 0x69cfc634, /// 0x470
			 0x70f9a5b7, /// 0x474
			 0x9f7e0e68, /// 0x478
			 0x7f751d62, /// 0x47c
			 0x2143605d, /// 0x480
			 0xdcdc1efa, /// 0x484
			 0x81f2d48e, /// 0x488
			 0x1c924c4a, /// 0x48c
			 0xf6934f22, /// 0x490
			 0xd2d4a588, /// 0x494
			 0x581605b9, /// 0x498
			 0x1f11a7f2, /// 0x49c
			 0x0c7c019e, /// 0x4a0
			 0x4a5b25e8, /// 0x4a4
			 0xf3e264ef, /// 0x4a8
			 0x63603a10, /// 0x4ac
			 0x465225f6, /// 0x4b0
			 0x795e6644, /// 0x4b4
			 0xf78723c9, /// 0x4b8
			 0x3018fb13, /// 0x4bc
			 0xae5f2303, /// 0x4c0
			 0xe248d2df, /// 0x4c4
			 0x4ef9c24d, /// 0x4c8
			 0x90e05957, /// 0x4cc
			 0x02de1e5a, /// 0x4d0
			 0x67b432c7, /// 0x4d4
			 0x3172b8a7, /// 0x4d8
			 0xdcecca24, /// 0x4dc
			 0x27d54711, /// 0x4e0
			 0x88166e71, /// 0x4e4
			 0x40745146, /// 0x4e8
			 0xa776f48e, /// 0x4ec
			 0x400e8e53, /// 0x4f0
			 0xab9791f2, /// 0x4f4
			 0x1a57ff57, /// 0x4f8
			 0xb8f961bd, /// 0x4fc
			 0xa3fba857, /// 0x500
			 0x21770c45, /// 0x504
			 0x7e328fcb, /// 0x508
			 0x157fc9bb, /// 0x50c
			 0xef9f44b5, /// 0x510
			 0x8fff7ecf, /// 0x514
			 0x92b79fad, /// 0x518
			 0xd2c130ba, /// 0x51c
			 0xe75cf7b6, /// 0x520
			 0x2537d4b0, /// 0x524
			 0xafd561d4, /// 0x528
			 0x05d22693, /// 0x52c
			 0x756be42b, /// 0x530
			 0xaca8f1bc, /// 0x534
			 0x9f11815b, /// 0x538
			 0x97115a35, /// 0x53c
			 0x4daaa192, /// 0x540
			 0x0ec52f83, /// 0x544
			 0x52d49662, /// 0x548
			 0x91086773, /// 0x54c
			 0x57bfe279, /// 0x550
			 0x9a3a089d, /// 0x554
			 0x4abb51b8, /// 0x558
			 0xb3f9474f, /// 0x55c
			 0x6919201a, /// 0x560
			 0x35e175b4, /// 0x564
			 0x1e1596e6, /// 0x568
			 0x90776a3b, /// 0x56c
			 0xb4748373, /// 0x570
			 0x5a513b1c, /// 0x574
			 0x640af5f3, /// 0x578
			 0xb112321d, /// 0x57c
			 0x41ddf8a0, /// 0x580
			 0xa811f5af, /// 0x584
			 0xf9232dc8, /// 0x588
			 0xca7fe1ae, /// 0x58c
			 0xa9ed261e, /// 0x590
			 0x90ab6305, /// 0x594
			 0xa656ffae, /// 0x598
			 0x651052c5, /// 0x59c
			 0x6e1108ca, /// 0x5a0
			 0xd518269f, /// 0x5a4
			 0xc45e5406, /// 0x5a8
			 0x1e78212f, /// 0x5ac
			 0x94c3f8e4, /// 0x5b0
			 0x6fc45a32, /// 0x5b4
			 0xf02a5b61, /// 0x5b8
			 0x698a37c8, /// 0x5bc
			 0x9ac09802, /// 0x5c0
			 0xf4b3084a, /// 0x5c4
			 0x056da373, /// 0x5c8
			 0x90ac282a, /// 0x5cc
			 0x8c7a6533, /// 0x5d0
			 0x1a62394a, /// 0x5d4
			 0xb67d0245, /// 0x5d8
			 0xf4172645, /// 0x5dc
			 0x4bea7a79, /// 0x5e0
			 0x76933eaa, /// 0x5e4
			 0xe7cf49fe, /// 0x5e8
			 0x22a072f4, /// 0x5ec
			 0x8c8e06ff, /// 0x5f0
			 0x09ec3df7, /// 0x5f4
			 0xc1061649, /// 0x5f8
			 0xabac68d6, /// 0x5fc
			 0xad439a57, /// 0x600
			 0x4ce7a49b, /// 0x604
			 0x9d954377, /// 0x608
			 0x45b5c0a5, /// 0x60c
			 0xe7401d15, /// 0x610
			 0xf35c62f0, /// 0x614
			 0x09772728, /// 0x618
			 0x00e37d4c, /// 0x61c
			 0x8b297057, /// 0x620
			 0xa5b9a99c, /// 0x624
			 0x2b85897d, /// 0x628
			 0xc8650294, /// 0x62c
			 0x59658a9c, /// 0x630
			 0xc36ff906, /// 0x634
			 0x7f14e665, /// 0x638
			 0xc73eb34e, /// 0x63c
			 0xf424cea7, /// 0x640
			 0x4df82aba, /// 0x644
			 0x0e4e8bac, /// 0x648
			 0x3280dc64, /// 0x64c
			 0x1e019f7d, /// 0x650
			 0x02938536, /// 0x654
			 0x7040f7a1, /// 0x658
			 0x20c53b9a, /// 0x65c
			 0x2066a9bb, /// 0x660
			 0x0ed248fa, /// 0x664
			 0x79cf2f6d, /// 0x668
			 0xb2591103, /// 0x66c
			 0x8a7d47ec, /// 0x670
			 0x9e145300, /// 0x674
			 0x22de9d26, /// 0x678
			 0x03fd10ad, /// 0x67c
			 0x079c6482, /// 0x680
			 0x8437541d, /// 0x684
			 0x34e2acd2, /// 0x688
			 0x728489f1, /// 0x68c
			 0xaeeca8e7, /// 0x690
			 0x97eb4336, /// 0x694
			 0x9bed1c10, /// 0x698
			 0x1f0d9c92, /// 0x69c
			 0x4b776082, /// 0x6a0
			 0x1aa6192c, /// 0x6a4
			 0x0908c104, /// 0x6a8
			 0x5452bb1f, /// 0x6ac
			 0xcf210d79, /// 0x6b0
			 0x80f564f8, /// 0x6b4
			 0x037f9c46, /// 0x6b8
			 0xad43b4fd, /// 0x6bc
			 0x83c9500d, /// 0x6c0
			 0x075d6208, /// 0x6c4
			 0x71206423, /// 0x6c8
			 0x8dafc342, /// 0x6cc
			 0x3767f0e8, /// 0x6d0
			 0xabbc464e, /// 0x6d4
			 0x9c7813a0, /// 0x6d8
			 0x645cb0c2, /// 0x6dc
			 0x6d3f68fd, /// 0x6e0
			 0xa57fc52d, /// 0x6e4
			 0x07524f4e, /// 0x6e8
			 0xf7f548c5, /// 0x6ec
			 0xf0d1b49d, /// 0x6f0
			 0xa4108651, /// 0x6f4
			 0x9e2c592c, /// 0x6f8
			 0x105c4ee2, /// 0x6fc
			 0x79d7dc80, /// 0x700
			 0xabc1145c, /// 0x704
			 0x973d8e9f, /// 0x708
			 0x37c626af, /// 0x70c
			 0x66b16ab2, /// 0x710
			 0x707f4ed4, /// 0x714
			 0x045acc54, /// 0x718
			 0x56734dfd, /// 0x71c
			 0x066da958, /// 0x720
			 0x3a1f17e5, /// 0x724
			 0x7e38bebb, /// 0x728
			 0x74c0569a, /// 0x72c
			 0x5174b4d8, /// 0x730
			 0xb3618ce5, /// 0x734
			 0x72ad2692, /// 0x738
			 0xfd62db9e, /// 0x73c
			 0x862cafdc, /// 0x740
			 0x185cd6a5, /// 0x744
			 0xf2574d8e, /// 0x748
			 0x756f2aae, /// 0x74c
			 0x84aa6593, /// 0x750
			 0x620d648d, /// 0x754
			 0x046359e8, /// 0x758
			 0xf692f441, /// 0x75c
			 0xa3a7fdad, /// 0x760
			 0x6193e662, /// 0x764
			 0x74c2e590, /// 0x768
			 0xc23c07cf, /// 0x76c
			 0x8f075fd3, /// 0x770
			 0x6a324e79, /// 0x774
			 0x9d5d62d4, /// 0x778
			 0x222a6f71, /// 0x77c
			 0x437ed0ed, /// 0x780
			 0x6c82e95b, /// 0x784
			 0x7fdfb964, /// 0x788
			 0xb3946cef, /// 0x78c
			 0x140d7494, /// 0x790
			 0x3299b52f, /// 0x794
			 0xf180259a, /// 0x798
			 0xc0976fad, /// 0x79c
			 0x437cf04f, /// 0x7a0
			 0x15bd2785, /// 0x7a4
			 0x321f17ce, /// 0x7a8
			 0x09cbed15, /// 0x7ac
			 0xd9e16c9f, /// 0x7b0
			 0x2c9764a8, /// 0x7b4
			 0x3881aa7c, /// 0x7b8
			 0xdf25b7bf, /// 0x7bc
			 0x9b4a51d8, /// 0x7c0
			 0x4b8259e5, /// 0x7c4
			 0xd751be88, /// 0x7c8
			 0x81bc77dd, /// 0x7cc
			 0x95ea57ea, /// 0x7d0
			 0x98c08737, /// 0x7d4
			 0x31e61030, /// 0x7d8
			 0xf2562c2e, /// 0x7dc
			 0x365e30af, /// 0x7e0
			 0xb1f36760, /// 0x7e4
			 0x444aa35e, /// 0x7e8
			 0xd51c8c67, /// 0x7ec
			 0x5f10249c, /// 0x7f0
			 0x9862d2a7, /// 0x7f4
			 0x0a84c080, /// 0x7f8
			 0x590cd6d6, /// 0x7fc
			 0x88758631, /// 0x800
			 0x19fc6e43, /// 0x804
			 0x5000e6c7, /// 0x808
			 0x91f23481, /// 0x80c
			 0x023c3a89, /// 0x810
			 0x413a752d, /// 0x814
			 0x794387cb, /// 0x818
			 0x0490001e, /// 0x81c
			 0xd48112eb, /// 0x820
			 0x7d47da5d, /// 0x824
			 0xa0fdf22f, /// 0x828
			 0x9fd30dad, /// 0x82c
			 0xa0d6ddd0, /// 0x830
			 0xd9ac8c40, /// 0x834
			 0xa87f7475, /// 0x838
			 0xb79996a5, /// 0x83c
			 0x69cf5ba6, /// 0x840
			 0x3c67bede, /// 0x844
			 0x2a8c83e2, /// 0x848
			 0xd5bb4ade, /// 0x84c
			 0x87fc7914, /// 0x850
			 0xbcde9589, /// 0x854
			 0xa258a3fe, /// 0x858
			 0xa0a9160b, /// 0x85c
			 0x0a21e605, /// 0x860
			 0xf70ff575, /// 0x864
			 0xb59aa2cb, /// 0x868
			 0xfd269be8, /// 0x86c
			 0xc11ee0bb, /// 0x870
			 0xd69ba338, /// 0x874
			 0x9c2b1f34, /// 0x878
			 0x0cdba4cc, /// 0x87c
			 0x77295551, /// 0x880
			 0xbf64af56, /// 0x884
			 0xf622965c, /// 0x888
			 0xf97bb31f, /// 0x88c
			 0xfaad797e, /// 0x890
			 0x25bd13c1, /// 0x894
			 0xd6c75646, /// 0x898
			 0xacf7282a, /// 0x89c
			 0xf0cb2beb, /// 0x8a0
			 0xed14374e, /// 0x8a4
			 0x5c202dc3, /// 0x8a8
			 0x0ebca051, /// 0x8ac
			 0xf71d091a, /// 0x8b0
			 0xf0b3ede2, /// 0x8b4
			 0xb20ce378, /// 0x8b8
			 0xc409112b, /// 0x8bc
			 0x2c7cc767, /// 0x8c0
			 0x806fb9f5, /// 0x8c4
			 0xb38f7ae0, /// 0x8c8
			 0xdb8bbf5d, /// 0x8cc
			 0x19738660, /// 0x8d0
			 0xff236e25, /// 0x8d4
			 0x45e577c6, /// 0x8d8
			 0xa3ee263f, /// 0x8dc
			 0xe7d62500, /// 0x8e0
			 0xab253387, /// 0x8e4
			 0x30a73710, /// 0x8e8
			 0x6f7f0e58, /// 0x8ec
			 0x5e1b76cf, /// 0x8f0
			 0x925267d3, /// 0x8f4
			 0x5cdbd733, /// 0x8f8
			 0xa5730a5b, /// 0x8fc
			 0x227cb6c3, /// 0x900
			 0xe0ec01a1, /// 0x904
			 0x81b46f2f, /// 0x908
			 0xd4aa483f, /// 0x90c
			 0xd15e5f59, /// 0x910
			 0x973a56fb, /// 0x914
			 0x475f013a, /// 0x918
			 0x6998999b, /// 0x91c
			 0xf270bc14, /// 0x920
			 0xd0810801, /// 0x924
			 0x3a6e20ec, /// 0x928
			 0xef3a5136, /// 0x92c
			 0x0ff1da4f, /// 0x930
			 0x1a52b809, /// 0x934
			 0xed14175f, /// 0x938
			 0x119d96c9, /// 0x93c
			 0x61dec4c3, /// 0x940
			 0x86567583, /// 0x944
			 0x5403da7d, /// 0x948
			 0xd501e226, /// 0x94c
			 0x70f2928e, /// 0x950
			 0xe3522462, /// 0x954
			 0xdae9d345, /// 0x958
			 0x9fdb81c3, /// 0x95c
			 0x2dddc3bc, /// 0x960
			 0xa4167572, /// 0x964
			 0x3c1e2dfd, /// 0x968
			 0xca4b2785, /// 0x96c
			 0x8bf3ed80, /// 0x970
			 0xc3917e11, /// 0x974
			 0xd4388835, /// 0x978
			 0x49d4c5df, /// 0x97c
			 0xa97cad78, /// 0x980
			 0x58c7e1f9, /// 0x984
			 0x89a428ea, /// 0x988
			 0x0226a66b, /// 0x98c
			 0xebf4cc72, /// 0x990
			 0xcd241588, /// 0x994
			 0xf9cb44e5, /// 0x998
			 0x923965a3, /// 0x99c
			 0x60a05bdd, /// 0x9a0
			 0x23189fb2, /// 0x9a4
			 0x012b39be, /// 0x9a8
			 0xb43ef19d, /// 0x9ac
			 0xb7f0f0ee, /// 0x9b0
			 0x7db63793, /// 0x9b4
			 0xac4a0b2a, /// 0x9b8
			 0x7cb60c2e, /// 0x9bc
			 0x2d092234, /// 0x9c0
			 0xc8ec6e57, /// 0x9c4
			 0x202c1e62, /// 0x9c8
			 0x4ff76312, /// 0x9cc
			 0xf6745e4a, /// 0x9d0
			 0x72390a4a, /// 0x9d4
			 0x4c207543, /// 0x9d8
			 0xeed9012c, /// 0x9dc
			 0x52b083a3, /// 0x9e0
			 0xc978d545, /// 0x9e4
			 0xdcc7f370, /// 0x9e8
			 0x3535022d, /// 0x9ec
			 0x3dec1937, /// 0x9f0
			 0x67098ad0, /// 0x9f4
			 0x773ddb4c, /// 0x9f8
			 0x818e0739, /// 0x9fc
			 0xbf4781ae, /// 0xa00
			 0x00b7ca0b, /// 0xa04
			 0x7d84a3af, /// 0xa08
			 0x0f026058, /// 0xa0c
			 0x42de1adc, /// 0xa10
			 0x9f0f1170, /// 0xa14
			 0xe4654939, /// 0xa18
			 0x335accd5, /// 0xa1c
			 0xc9484607, /// 0xa20
			 0x538fbcdd, /// 0xa24
			 0x52a78f26, /// 0xa28
			 0xaaa999fa, /// 0xa2c
			 0x61121f8f, /// 0xa30
			 0xe8fbdba6, /// 0xa34
			 0xa4f4efe9, /// 0xa38
			 0x354c0815, /// 0xa3c
			 0x16ac292b, /// 0xa40
			 0x77c723f5, /// 0xa44
			 0xabf3b4bb, /// 0xa48
			 0x27ef294e, /// 0xa4c
			 0x99e7e4ab, /// 0xa50
			 0xad24bcad, /// 0xa54
			 0x32aa149d, /// 0xa58
			 0x5dd6ff96, /// 0xa5c
			 0x0da848b2, /// 0xa60
			 0x8fe1f283, /// 0xa64
			 0xd6414fd6, /// 0xa68
			 0xc9fa6d0c, /// 0xa6c
			 0x55a067ff, /// 0xa70
			 0x6045901b, /// 0xa74
			 0x33d54c6b, /// 0xa78
			 0x0a702193, /// 0xa7c
			 0x1acb0050, /// 0xa80
			 0x1771b0c1, /// 0xa84
			 0xc14ac2a8, /// 0xa88
			 0xc3f94644, /// 0xa8c
			 0x6d3f862c, /// 0xa90
			 0xd4f2dae2, /// 0xa94
			 0x67a8a5bc, /// 0xa98
			 0x61448b9c, /// 0xa9c
			 0x11ab834d, /// 0xaa0
			 0xf5363cb3, /// 0xaa4
			 0xefca90ad, /// 0xaa8
			 0xaea8ae50, /// 0xaac
			 0x49cb285f, /// 0xab0
			 0x2ce19a76, /// 0xab4
			 0x19b8d151, /// 0xab8
			 0x9fc3784a, /// 0xabc
			 0xa7886a03, /// 0xac0
			 0xb00b4d08, /// 0xac4
			 0xe7f180e8, /// 0xac8
			 0x4102e5eb, /// 0xacc
			 0x74693a81, /// 0xad0
			 0xe0d4ab22, /// 0xad4
			 0x221cf09e, /// 0xad8
			 0xdff47a18, /// 0xadc
			 0x054cb6d6, /// 0xae0
			 0x3b8017ea, /// 0xae4
			 0x06798e79, /// 0xae8
			 0xff297d3f, /// 0xaec
			 0x26364af7, /// 0xaf0
			 0xcae158ba, /// 0xaf4
			 0x80a41164, /// 0xaf8
			 0x37a74694, /// 0xafc
			 0x9f73fe90, /// 0xb00
			 0x17456417, /// 0xb04
			 0x8ac9014d, /// 0xb08
			 0xfe6d064e, /// 0xb0c
			 0xfe1b47e3, /// 0xb10
			 0x7f04b6f0, /// 0xb14
			 0x259b581f, /// 0xb18
			 0x44fde022, /// 0xb1c
			 0xb6e23d3f, /// 0xb20
			 0x5bb90838, /// 0xb24
			 0x731804cb, /// 0xb28
			 0x5bd51c14, /// 0xb2c
			 0x17777b4c, /// 0xb30
			 0x9ce62000, /// 0xb34
			 0x55d57409, /// 0xb38
			 0x2b7e230c, /// 0xb3c
			 0x06c88acc, /// 0xb40
			 0xd1d3a144, /// 0xb44
			 0x1b1a9a20, /// 0xb48
			 0xac2c0994, /// 0xb4c
			 0xb0e11db1, /// 0xb50
			 0x34a97626, /// 0xb54
			 0x7b5d9196, /// 0xb58
			 0xb069fa30, /// 0xb5c
			 0x9b1f8a94, /// 0xb60
			 0xb299298d, /// 0xb64
			 0x9ccd7e8f, /// 0xb68
			 0xbb467883, /// 0xb6c
			 0x9ece1ba6, /// 0xb70
			 0xa4f212c0, /// 0xb74
			 0x69155763, /// 0xb78
			 0x395b705c, /// 0xb7c
			 0x143ec28a, /// 0xb80
			 0xdccd0009, /// 0xb84
			 0x4d514aa1, /// 0xb88
			 0x0b4cdca7, /// 0xb8c
			 0xd9ba0c1b, /// 0xb90
			 0xaf4207e4, /// 0xb94
			 0xc2ebc5d5, /// 0xb98
			 0x33eab66f, /// 0xb9c
			 0xf29b54d3, /// 0xba0
			 0x9f33aa06, /// 0xba4
			 0x09fe9908, /// 0xba8
			 0x8c8cc0e6, /// 0xbac
			 0x9e59169f, /// 0xbb0
			 0x92a7b306, /// 0xbb4
			 0x2fc6a5a3, /// 0xbb8
			 0xd477874d, /// 0xbbc
			 0x93610887, /// 0xbc0
			 0xd1242dc4, /// 0xbc4
			 0x7737dee4, /// 0xbc8
			 0x0610a625, /// 0xbcc
			 0xeb2cb97b, /// 0xbd0
			 0xcedbd130, /// 0xbd4
			 0x9c06e32b, /// 0xbd8
			 0x138a0a2b, /// 0xbdc
			 0x951f6aaa, /// 0xbe0
			 0xfb5f2ff3, /// 0xbe4
			 0x8101f071, /// 0xbe8
			 0x73b2b659, /// 0xbec
			 0x7703e475, /// 0xbf0
			 0x26dbe1e1, /// 0xbf4
			 0xfd5c5502, /// 0xbf8
			 0x4f4cb980, /// 0xbfc
			 0x86fc77f1, /// 0xc00
			 0xf74cc10d, /// 0xc04
			 0x3561dcf1, /// 0xc08
			 0x4f6633c4, /// 0xc0c
			 0xf7c54c7a, /// 0xc10
			 0x59ad103a, /// 0xc14
			 0xe6f5de1f, /// 0xc18
			 0xcd6d63c1, /// 0xc1c
			 0xfc03bc80, /// 0xc20
			 0x97f01230, /// 0xc24
			 0xb18336fe, /// 0xc28
			 0xa5780831, /// 0xc2c
			 0xf2735cf6, /// 0xc30
			 0x3e59a590, /// 0xc34
			 0x1e3c2d31, /// 0xc38
			 0xa91e53f6, /// 0xc3c
			 0x62e2da97, /// 0xc40
			 0x7f866639, /// 0xc44
			 0x8455e24a, /// 0xc48
			 0x0c74153c, /// 0xc4c
			 0xb215d14a, /// 0xc50
			 0x0be4d400, /// 0xc54
			 0x1e345bdf, /// 0xc58
			 0x4ee4f8cc, /// 0xc5c
			 0xd7f37bbe, /// 0xc60
			 0xa8712772, /// 0xc64
			 0xbf65e875, /// 0xc68
			 0xf776c705, /// 0xc6c
			 0x1e396597, /// 0xc70
			 0xdb1fc606, /// 0xc74
			 0x5d7afb9b, /// 0xc78
			 0xdf1f2dfc, /// 0xc7c
			 0x8570836f, /// 0xc80
			 0x00e61ea9, /// 0xc84
			 0x265fc216, /// 0xc88
			 0x4bb80230, /// 0xc8c
			 0x0b79b809, /// 0xc90
			 0x8acf3d65, /// 0xc94
			 0xebdf9b1e, /// 0xc98
			 0x0cccffd9, /// 0xc9c
			 0xceef2157, /// 0xca0
			 0x8814523e, /// 0xca4
			 0x6a5099ee, /// 0xca8
			 0xf06bd61e, /// 0xcac
			 0x03803124, /// 0xcb0
			 0x84aada20, /// 0xcb4
			 0x258f39f8, /// 0xcb8
			 0xeb65fdf8, /// 0xcbc
			 0x37fac2cd, /// 0xcc0
			 0x8e917d9d, /// 0xcc4
			 0x13f2b86e, /// 0xcc8
			 0x681c6ce9, /// 0xccc
			 0x8aad6890, /// 0xcd0
			 0x738a2626, /// 0xcd4
			 0x1e1e7d0c, /// 0xcd8
			 0x7664159f, /// 0xcdc
			 0x118404a6, /// 0xce0
			 0xd7e0f42c, /// 0xce4
			 0x011f1947, /// 0xce8
			 0x8a909314, /// 0xcec
			 0xfd67f96e, /// 0xcf0
			 0xc123308f, /// 0xcf4
			 0x7925bf3e, /// 0xcf8
			 0x14fb025c, /// 0xcfc
			 0x2829db79, /// 0xd00
			 0x49b80b7a, /// 0xd04
			 0xe9a65ace, /// 0xd08
			 0xe0954bf2, /// 0xd0c
			 0x7944dda9, /// 0xd10
			 0xce78bf83, /// 0xd14
			 0xb8c8992e, /// 0xd18
			 0xeef689d8, /// 0xd1c
			 0x5a1586c0, /// 0xd20
			 0x90c0fde4, /// 0xd24
			 0x1ca3afb1, /// 0xd28
			 0x144bdaad, /// 0xd2c
			 0x68645fb9, /// 0xd30
			 0x7d2de850, /// 0xd34
			 0x30e547e9, /// 0xd38
			 0x9e812b2c, /// 0xd3c
			 0x6cf2e5f6, /// 0xd40
			 0x49ee14e7, /// 0xd44
			 0x823feeca, /// 0xd48
			 0x2057851b, /// 0xd4c
			 0xd95c5186, /// 0xd50
			 0xa75e1d0f, /// 0xd54
			 0xf9a44cd2, /// 0xd58
			 0x0fdb2a6b, /// 0xd5c
			 0xa8e63d69, /// 0xd60
			 0x879e2557, /// 0xd64
			 0xf00da89f, /// 0xd68
			 0xd062afa7, /// 0xd6c
			 0x00679cb4, /// 0xd70
			 0xe996c1a9, /// 0xd74
			 0xc95ccd91, /// 0xd78
			 0xdb598ea0, /// 0xd7c
			 0xbf37c2e8, /// 0xd80
			 0xb622bff6, /// 0xd84
			 0x7c05a0d7, /// 0xd88
			 0x0ff7a3ef, /// 0xd8c
			 0x882bc73e, /// 0xd90
			 0x68120cb3, /// 0xd94
			 0xf6ce97d0, /// 0xd98
			 0x743c0e70, /// 0xd9c
			 0x7e7c250a, /// 0xda0
			 0x51d860b8, /// 0xda4
			 0xcf9dd463, /// 0xda8
			 0xc523e75c, /// 0xdac
			 0xdf82504f, /// 0xdb0
			 0xdd4069a2, /// 0xdb4
			 0xd4810b7a, /// 0xdb8
			 0x49396dc9, /// 0xdbc
			 0x34f44ff7, /// 0xdc0
			 0xb634af76, /// 0xdc4
			 0x7770c09c, /// 0xdc8
			 0x3d5b6895, /// 0xdcc
			 0x5464a2ca, /// 0xdd0
			 0x1006f517, /// 0xdd4
			 0x9e260664, /// 0xdd8
			 0x6cae6817, /// 0xddc
			 0xaf4e6830, /// 0xde0
			 0x8ba7c001, /// 0xde4
			 0xbd26d005, /// 0xde8
			 0xed978fc3, /// 0xdec
			 0x830aa32e, /// 0xdf0
			 0x5701985c, /// 0xdf4
			 0xf35115a6, /// 0xdf8
			 0xb24437ed, /// 0xdfc
			 0x5740e924, /// 0xe00
			 0x8c260169, /// 0xe04
			 0x30f577d2, /// 0xe08
			 0x9893d0a1, /// 0xe0c
			 0x34afafc1, /// 0xe10
			 0xce64118f, /// 0xe14
			 0xc6b421fd, /// 0xe18
			 0x7bc1877d, /// 0xe1c
			 0x0762d479, /// 0xe20
			 0xcfbda740, /// 0xe24
			 0x3b0bfbe0, /// 0xe28
			 0xf35056ca, /// 0xe2c
			 0x01a2c46b, /// 0xe30
			 0xf10cc90a, /// 0xe34
			 0x034ea9a0, /// 0xe38
			 0xa141a0db, /// 0xe3c
			 0x52a98f67, /// 0xe40
			 0x09d3479a, /// 0xe44
			 0x5d772502, /// 0xe48
			 0xcd376ceb, /// 0xe4c
			 0x5bff58c8, /// 0xe50
			 0x0846c9d4, /// 0xe54
			 0x92c3a0e4, /// 0xe58
			 0x521b877a, /// 0xe5c
			 0xcfcb3217, /// 0xe60
			 0x97970c74, /// 0xe64
			 0xc08c0097, /// 0xe68
			 0x2963f4e3, /// 0xe6c
			 0xf0bf2712, /// 0xe70
			 0xe389b298, /// 0xe74
			 0x0d1eddc9, /// 0xe78
			 0xbfdf2450, /// 0xe7c
			 0x4e661f54, /// 0xe80
			 0xb101b0d2, /// 0xe84
			 0xd247535b, /// 0xe88
			 0xb0e91cc1, /// 0xe8c
			 0x327c782b, /// 0xe90
			 0x1a956701, /// 0xe94
			 0xfce7f68b, /// 0xe98
			 0x66c5dcd3, /// 0xe9c
			 0x944740a4, /// 0xea0
			 0x3befd53e, /// 0xea4
			 0x9e19a925, /// 0xea8
			 0x622a4d70, /// 0xeac
			 0x72ea07ae, /// 0xeb0
			 0xadb6fac7, /// 0xeb4
			 0x96b6eb8d, /// 0xeb8
			 0x48299342, /// 0xebc
			 0x80508be4, /// 0xec0
			 0x487ba744, /// 0xec4
			 0x25a971f6, /// 0xec8
			 0xb02162ec, /// 0xecc
			 0x783a0ef5, /// 0xed0
			 0xeaa404fe, /// 0xed4
			 0x5f9a1a41, /// 0xed8
			 0x34f09c44, /// 0xedc
			 0xc8f5c164, /// 0xee0
			 0x6338a3ee, /// 0xee4
			 0x0b78674e, /// 0xee8
			 0x8d3c63e3, /// 0xeec
			 0xb31a625a, /// 0xef0
			 0x49d471f3, /// 0xef4
			 0x645d6533, /// 0xef8
			 0xff9a5f6d, /// 0xefc
			 0x8232f456, /// 0xf00
			 0x6cf6d29e, /// 0xf04
			 0xd6c6ffd7, /// 0xf08
			 0x23f49af8, /// 0xf0c
			 0x7d13d09c, /// 0xf10
			 0xefb5a62f, /// 0xf14
			 0x16005d39, /// 0xf18
			 0x4eb75c84, /// 0xf1c
			 0xc55b1c7b, /// 0xf20
			 0xcd539839, /// 0xf24
			 0xc06fb30c, /// 0xf28
			 0xc2f511cc, /// 0xf2c
			 0x6e74e619, /// 0xf30
			 0xf9249170, /// 0xf34
			 0x8ecc5522, /// 0xf38
			 0x84ce7207, /// 0xf3c
			 0x7d82b997, /// 0xf40
			 0x4991d7a0, /// 0xf44
			 0x263550f2, /// 0xf48
			 0x42eb2437, /// 0xf4c
			 0xab9c1378, /// 0xf50
			 0x52ef0256, /// 0xf54
			 0x3568cb4b, /// 0xf58
			 0xc4941dfd, /// 0xf5c
			 0x61c9f5df, /// 0xf60
			 0x1896473d, /// 0xf64
			 0xa011a296, /// 0xf68
			 0x87355abe, /// 0xf6c
			 0x5f35efb9, /// 0xf70
			 0x04da2b2e, /// 0xf74
			 0x21c8ae6d, /// 0xf78
			 0x4cdf16a0, /// 0xf7c
			 0x9116a303, /// 0xf80
			 0xe005a6d8, /// 0xf84
			 0x76d10f5e, /// 0xf88
			 0x5140500d, /// 0xf8c
			 0xc23f211e, /// 0xf90
			 0x0785d83b, /// 0xf94
			 0x6418fc27, /// 0xf98
			 0xbb51bef5, /// 0xf9c
			 0x163b4acf, /// 0xfa0
			 0xb12fda63, /// 0xfa4
			 0xed60120a, /// 0xfa8
			 0x9e8560e1, /// 0xfac
			 0xf3bbde7c, /// 0xfb0
			 0x974cce99, /// 0xfb4
			 0x6938933d, /// 0xfb8
			 0x69de7c7e, /// 0xfbc
			 0x1cb5d3a3, /// 0xfc0
			 0x36bd72bf, /// 0xfc4
			 0x2b16dcb4, /// 0xfc8
			 0x391e6a51, /// 0xfcc
			 0x7c625321, /// 0xfd0
			 0x716c6a4b, /// 0xfd4
			 0x83ca46c8, /// 0xfd8
			 0xa917b9e7, /// 0xfdc
			 0x0a95d71e, /// 0xfe0
			 0x5a77a4e3, /// 0xfe4
			 0x2ec680aa, /// 0xfe8
			 0xbe2978c2, /// 0xfec
			 0xa3439581, /// 0xff0
			 0x58419b02, /// 0xff4
			 0x0697f98d, /// 0xff8
			 0x1ce0a70f /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x25eb7c94, /// 0x0
			 0x274f689d, /// 0x4
			 0xd709d6ba, /// 0x8
			 0xfab3b1a2, /// 0xc
			 0x3d95ee60, /// 0x10
			 0x8d5f3913, /// 0x14
			 0x26ca5d0c, /// 0x18
			 0xd216180e, /// 0x1c
			 0x7d39a87d, /// 0x20
			 0x8ec08ead, /// 0x24
			 0x807a0e3b, /// 0x28
			 0xa22f9ada, /// 0x2c
			 0x8dc7e802, /// 0x30
			 0xd0ef9c6c, /// 0x34
			 0x079fc102, /// 0x38
			 0x60f4a442, /// 0x3c
			 0x470709c2, /// 0x40
			 0xfb84a3de, /// 0x44
			 0xe4f7fc95, /// 0x48
			 0x8b03d6b7, /// 0x4c
			 0x8ab70a56, /// 0x50
			 0x795c3664, /// 0x54
			 0xee7afc08, /// 0x58
			 0x2249d99e, /// 0x5c
			 0xde65b2bf, /// 0x60
			 0xdd1e79c7, /// 0x64
			 0xea8ab11b, /// 0x68
			 0x83398747, /// 0x6c
			 0x962fbf4c, /// 0x70
			 0x4f90ab9a, /// 0x74
			 0x21fb8615, /// 0x78
			 0x5e821bb9, /// 0x7c
			 0xf3776e6a, /// 0x80
			 0x72779010, /// 0x84
			 0x8d8d06f2, /// 0x88
			 0x170a309d, /// 0x8c
			 0x7039ae66, /// 0x90
			 0xfa229755, /// 0x94
			 0x9a467a7f, /// 0x98
			 0xfb366a3c, /// 0x9c
			 0x67edf678, /// 0xa0
			 0x797ec09e, /// 0xa4
			 0x7c0efbe1, /// 0xa8
			 0xc5ff00c3, /// 0xac
			 0x0299550b, /// 0xb0
			 0xaf3d4d8f, /// 0xb4
			 0x9a8562b9, /// 0xb8
			 0xeebc050c, /// 0xbc
			 0x562828bf, /// 0xc0
			 0x5d53aec8, /// 0xc4
			 0xc4ea30d2, /// 0xc8
			 0xb5c42eba, /// 0xcc
			 0x7b75dfba, /// 0xd0
			 0xaee7e994, /// 0xd4
			 0xc9ed2c2c, /// 0xd8
			 0xee699dbc, /// 0xdc
			 0x6b37aab4, /// 0xe0
			 0x452f26c5, /// 0xe4
			 0x2b097577, /// 0xe8
			 0xa4ffeaef, /// 0xec
			 0x4b22b976, /// 0xf0
			 0xc377c565, /// 0xf4
			 0x8803c286, /// 0xf8
			 0xfd667f1d, /// 0xfc
			 0xe1f8574e, /// 0x100
			 0x625c3879, /// 0x104
			 0x64d8dcf9, /// 0x108
			 0x8f08a177, /// 0x10c
			 0x22c707ce, /// 0x110
			 0x1a57733e, /// 0x114
			 0x2698715f, /// 0x118
			 0x21ac0599, /// 0x11c
			 0x94c24749, /// 0x120
			 0xe5be287b, /// 0x124
			 0xfec7c47c, /// 0x128
			 0x121643bf, /// 0x12c
			 0x498186d9, /// 0x130
			 0x21d9e7c9, /// 0x134
			 0x6ced8ae8, /// 0x138
			 0xdb5c07f7, /// 0x13c
			 0x4aa74f08, /// 0x140
			 0x29ef6ae0, /// 0x144
			 0xc6e6c38a, /// 0x148
			 0x7799252b, /// 0x14c
			 0xd24c2c3f, /// 0x150
			 0x549dde75, /// 0x154
			 0xcc7f142c, /// 0x158
			 0x0d63ff35, /// 0x15c
			 0xa6d8edfc, /// 0x160
			 0x3eedabc3, /// 0x164
			 0xdfcfd4fc, /// 0x168
			 0x7f39a3fb, /// 0x16c
			 0xe7fc2d98, /// 0x170
			 0x92d79786, /// 0x174
			 0x7dcebc18, /// 0x178
			 0x4cfcb500, /// 0x17c
			 0x9c202bf7, /// 0x180
			 0x99a5aac5, /// 0x184
			 0xf02e7d03, /// 0x188
			 0xdcf9cc1e, /// 0x18c
			 0xb64a866b, /// 0x190
			 0xd221b6d9, /// 0x194
			 0xf2dc0e22, /// 0x198
			 0xc7e7076c, /// 0x19c
			 0xda7aa882, /// 0x1a0
			 0x1c93f5db, /// 0x1a4
			 0x4b6e9932, /// 0x1a8
			 0x8524259b, /// 0x1ac
			 0x56d9c54e, /// 0x1b0
			 0x1bc9915f, /// 0x1b4
			 0x58b41f86, /// 0x1b8
			 0x39117da3, /// 0x1bc
			 0x0b8f51c8, /// 0x1c0
			 0xd22ab58c, /// 0x1c4
			 0x9b265b39, /// 0x1c8
			 0xa5919ca9, /// 0x1cc
			 0x6f5f90db, /// 0x1d0
			 0xa7da6f9c, /// 0x1d4
			 0x03ea2e95, /// 0x1d8
			 0xe91dcf99, /// 0x1dc
			 0xe7b3e59f, /// 0x1e0
			 0x6c45fa9d, /// 0x1e4
			 0xbce73521, /// 0x1e8
			 0xe31661c0, /// 0x1ec
			 0x39d889ee, /// 0x1f0
			 0xf2111e12, /// 0x1f4
			 0x33d2c237, /// 0x1f8
			 0x6b6a3df8, /// 0x1fc
			 0xe802d9b1, /// 0x200
			 0x0be431f9, /// 0x204
			 0x990e1e73, /// 0x208
			 0x3e470ead, /// 0x20c
			 0xebbb8105, /// 0x210
			 0x2eb5c79c, /// 0x214
			 0x000b2087, /// 0x218
			 0x27cf7ab7, /// 0x21c
			 0xdf56e352, /// 0x220
			 0xdf2ed2a3, /// 0x224
			 0x5092b394, /// 0x228
			 0xf2a59914, /// 0x22c
			 0xb588b041, /// 0x230
			 0xed65af2e, /// 0x234
			 0x820012cf, /// 0x238
			 0x550b6d93, /// 0x23c
			 0x1aeeadb3, /// 0x240
			 0x598b8760, /// 0x244
			 0x7b38fa53, /// 0x248
			 0xde59ff1e, /// 0x24c
			 0xa9432e0c, /// 0x250
			 0xd4a32ca6, /// 0x254
			 0x7b4f105b, /// 0x258
			 0x04421fa8, /// 0x25c
			 0x284e4d61, /// 0x260
			 0x76ddfa73, /// 0x264
			 0xd698589a, /// 0x268
			 0x5db66ced, /// 0x26c
			 0x55738de0, /// 0x270
			 0x3dfd3d1f, /// 0x274
			 0x2ae4553f, /// 0x278
			 0x7bd114fd, /// 0x27c
			 0x1929f2b1, /// 0x280
			 0x4f226eda, /// 0x284
			 0x77c2c949, /// 0x288
			 0xe6490a11, /// 0x28c
			 0xcf75d107, /// 0x290
			 0x257bc2b0, /// 0x294
			 0xf98dcb28, /// 0x298
			 0x69648a55, /// 0x29c
			 0x49d35c7a, /// 0x2a0
			 0x4527e206, /// 0x2a4
			 0x3ecb340e, /// 0x2a8
			 0x2b6a54c0, /// 0x2ac
			 0x3aca14f3, /// 0x2b0
			 0xb0362444, /// 0x2b4
			 0x2ecfc326, /// 0x2b8
			 0xc7516477, /// 0x2bc
			 0x0e344a3e, /// 0x2c0
			 0x0718e62c, /// 0x2c4
			 0x5bccd3c2, /// 0x2c8
			 0xbd9fccf4, /// 0x2cc
			 0x941f530a, /// 0x2d0
			 0x2b2feba1, /// 0x2d4
			 0x18a0752e, /// 0x2d8
			 0x5940da88, /// 0x2dc
			 0x542200dc, /// 0x2e0
			 0x0b055c78, /// 0x2e4
			 0x3c760855, /// 0x2e8
			 0xb13a689a, /// 0x2ec
			 0xb03624cb, /// 0x2f0
			 0x0a98d3eb, /// 0x2f4
			 0x98c48943, /// 0x2f8
			 0x8deda7a4, /// 0x2fc
			 0xd16beeb2, /// 0x300
			 0x9d13ad74, /// 0x304
			 0x84e2c3d4, /// 0x308
			 0x5b83e2a2, /// 0x30c
			 0x8cd7722d, /// 0x310
			 0x6042a3c0, /// 0x314
			 0x726cacee, /// 0x318
			 0x21e169d1, /// 0x31c
			 0x3bdcedd5, /// 0x320
			 0xf9a6e11f, /// 0x324
			 0x3e4e33c8, /// 0x328
			 0x87142f0d, /// 0x32c
			 0xabc5a323, /// 0x330
			 0xc366bc97, /// 0x334
			 0x4598d373, /// 0x338
			 0x454a1ec1, /// 0x33c
			 0xc59c5e72, /// 0x340
			 0x48b081a1, /// 0x344
			 0x3262f86c, /// 0x348
			 0x42857af8, /// 0x34c
			 0xee385ec9, /// 0x350
			 0x65c3d8a6, /// 0x354
			 0xdf88ee53, /// 0x358
			 0x408eb571, /// 0x35c
			 0x5a18421e, /// 0x360
			 0xe4305d51, /// 0x364
			 0x4bdee650, /// 0x368
			 0xc2a76df9, /// 0x36c
			 0xcdde1e39, /// 0x370
			 0xffc276c3, /// 0x374
			 0xbf1ac951, /// 0x378
			 0x604e5de4, /// 0x37c
			 0xbdddfa10, /// 0x380
			 0x3dcccb37, /// 0x384
			 0x44cfcb3a, /// 0x388
			 0x35d90a66, /// 0x38c
			 0xee60ff69, /// 0x390
			 0x2cce1894, /// 0x394
			 0x8171decc, /// 0x398
			 0xb4981304, /// 0x39c
			 0x6abcff57, /// 0x3a0
			 0xc7d3f6c2, /// 0x3a4
			 0x0cdf081f, /// 0x3a8
			 0xbb1a9b51, /// 0x3ac
			 0xcc80c390, /// 0x3b0
			 0x69d2ce3e, /// 0x3b4
			 0xf69b2c60, /// 0x3b8
			 0xff9cb5eb, /// 0x3bc
			 0xb655834d, /// 0x3c0
			 0xc77d6f54, /// 0x3c4
			 0x875dc6f9, /// 0x3c8
			 0xc574f61b, /// 0x3cc
			 0x02ba17bd, /// 0x3d0
			 0x407e0497, /// 0x3d4
			 0x7269f561, /// 0x3d8
			 0xb323ecc3, /// 0x3dc
			 0xa0008e48, /// 0x3e0
			 0x4522a63f, /// 0x3e4
			 0x4a6d7079, /// 0x3e8
			 0xf5e575e2, /// 0x3ec
			 0x252bb325, /// 0x3f0
			 0x387cb24c, /// 0x3f4
			 0xdcf4cfd7, /// 0x3f8
			 0x80e26f05, /// 0x3fc
			 0x1445e314, /// 0x400
			 0xd8ece2d2, /// 0x404
			 0xd79728eb, /// 0x408
			 0xae20b82f, /// 0x40c
			 0xbffc1b60, /// 0x410
			 0xe7024e96, /// 0x414
			 0x76cc093b, /// 0x418
			 0xadc59727, /// 0x41c
			 0x7cc43db5, /// 0x420
			 0x603979d1, /// 0x424
			 0xd6114c01, /// 0x428
			 0x3091cef2, /// 0x42c
			 0xc8cf013a, /// 0x430
			 0x75cb17c0, /// 0x434
			 0x68e86427, /// 0x438
			 0xdc388061, /// 0x43c
			 0x8b9c10e9, /// 0x440
			 0x1dcbae9f, /// 0x444
			 0x0d9d8282, /// 0x448
			 0x9959c054, /// 0x44c
			 0x21c056ca, /// 0x450
			 0x149d9105, /// 0x454
			 0xb5d65440, /// 0x458
			 0x4ac47b15, /// 0x45c
			 0xaa1e29a2, /// 0x460
			 0x93dc9eb1, /// 0x464
			 0x16ad24bb, /// 0x468
			 0xe498e632, /// 0x46c
			 0x0548ffc0, /// 0x470
			 0x81715c00, /// 0x474
			 0xce246953, /// 0x478
			 0xeb4586dc, /// 0x47c
			 0x2ad72f3b, /// 0x480
			 0xfc6e7c30, /// 0x484
			 0x6365cc74, /// 0x488
			 0xcd80022e, /// 0x48c
			 0x94effacd, /// 0x490
			 0xfce69af0, /// 0x494
			 0x52b1eb59, /// 0x498
			 0xb5ffe17d, /// 0x49c
			 0xc45cfb88, /// 0x4a0
			 0x7066deb3, /// 0x4a4
			 0x2dc36dac, /// 0x4a8
			 0x857a9638, /// 0x4ac
			 0x04f335ff, /// 0x4b0
			 0xcf6282c0, /// 0x4b4
			 0xb1d939ca, /// 0x4b8
			 0xcdb8d069, /// 0x4bc
			 0xecf9a28a, /// 0x4c0
			 0xf77c16e6, /// 0x4c4
			 0x5f4c5bf0, /// 0x4c8
			 0x48bf346c, /// 0x4cc
			 0x7f0e086d, /// 0x4d0
			 0x65604cdd, /// 0x4d4
			 0x3409d82c, /// 0x4d8
			 0xba1840b2, /// 0x4dc
			 0x27c5efc0, /// 0x4e0
			 0x9462358f, /// 0x4e4
			 0x7a2d16ba, /// 0x4e8
			 0x2487e324, /// 0x4ec
			 0x37f995e1, /// 0x4f0
			 0x06d58d8f, /// 0x4f4
			 0xd734c3be, /// 0x4f8
			 0x3d4e9236, /// 0x4fc
			 0x887b368f, /// 0x500
			 0x2f053069, /// 0x504
			 0x8b8fb411, /// 0x508
			 0x708adb79, /// 0x50c
			 0x37f05563, /// 0x510
			 0x8d72c453, /// 0x514
			 0xdeb32d07, /// 0x518
			 0x1438f79c, /// 0x51c
			 0x9a9882db, /// 0x520
			 0xd63d0331, /// 0x524
			 0x1092a636, /// 0x528
			 0x2e0e2a8f, /// 0x52c
			 0x3a677497, /// 0x530
			 0x9c5be3a8, /// 0x534
			 0x799f44e5, /// 0x538
			 0xcefeb445, /// 0x53c
			 0xcef21321, /// 0x540
			 0x0b6d5c3e, /// 0x544
			 0xb7bb0423, /// 0x548
			 0x6a35636a, /// 0x54c
			 0xef47b948, /// 0x550
			 0x7780c237, /// 0x554
			 0xee3d40a0, /// 0x558
			 0x9c2d12af, /// 0x55c
			 0x3e0edf0d, /// 0x560
			 0xfe58a243, /// 0x564
			 0xbff55307, /// 0x568
			 0x1e8d1ffa, /// 0x56c
			 0x00e2ac24, /// 0x570
			 0x891e5bdb, /// 0x574
			 0x8a6959a6, /// 0x578
			 0x6c5ca020, /// 0x57c
			 0xdbc11995, /// 0x580
			 0x643410ed, /// 0x584
			 0x2fa5a8f7, /// 0x588
			 0x35986dc5, /// 0x58c
			 0xc4bb9c63, /// 0x590
			 0xe43532ad, /// 0x594
			 0xa0cedc06, /// 0x598
			 0xd0632ddb, /// 0x59c
			 0x6c2b10fc, /// 0x5a0
			 0x1d68509d, /// 0x5a4
			 0x440d13bc, /// 0x5a8
			 0x66b2742e, /// 0x5ac
			 0x9ac3d923, /// 0x5b0
			 0x8a28dc3e, /// 0x5b4
			 0xcde3ceea, /// 0x5b8
			 0x0d8162a1, /// 0x5bc
			 0x0da67215, /// 0x5c0
			 0x13a7d0a0, /// 0x5c4
			 0x16ee6f8f, /// 0x5c8
			 0x18852cb7, /// 0x5cc
			 0x98c57a4a, /// 0x5d0
			 0x45c603af, /// 0x5d4
			 0xceb21730, /// 0x5d8
			 0xe0921abe, /// 0x5dc
			 0xc5b90597, /// 0x5e0
			 0x2c8a9d7f, /// 0x5e4
			 0x980ca351, /// 0x5e8
			 0xbb31bf2b, /// 0x5ec
			 0xc4eebab1, /// 0x5f0
			 0xe7527141, /// 0x5f4
			 0xf2d8455a, /// 0x5f8
			 0x06e7c8ee, /// 0x5fc
			 0x52224e00, /// 0x600
			 0x4d486a11, /// 0x604
			 0x512f58ae, /// 0x608
			 0x047c48de, /// 0x60c
			 0x520180c2, /// 0x610
			 0xf8a213a9, /// 0x614
			 0xa5678328, /// 0x618
			 0x8182827f, /// 0x61c
			 0x5b01bd88, /// 0x620
			 0x6c2023d4, /// 0x624
			 0xe83140e8, /// 0x628
			 0x72d9e2b1, /// 0x62c
			 0xc11d9ae6, /// 0x630
			 0xe059eef3, /// 0x634
			 0x90e03bb9, /// 0x638
			 0x385efab5, /// 0x63c
			 0xaad95a91, /// 0x640
			 0xb701956b, /// 0x644
			 0xb4b0977b, /// 0x648
			 0xacf7431c, /// 0x64c
			 0xe66c0959, /// 0x650
			 0x701163ce, /// 0x654
			 0xccc98d42, /// 0x658
			 0x6845da85, /// 0x65c
			 0xd90731c5, /// 0x660
			 0xf79666a8, /// 0x664
			 0x93343548, /// 0x668
			 0xe26636e2, /// 0x66c
			 0xea40bd37, /// 0x670
			 0x8bd3abd1, /// 0x674
			 0x914c6343, /// 0x678
			 0xda38360b, /// 0x67c
			 0x0869e3dd, /// 0x680
			 0x1f4d0084, /// 0x684
			 0x222ddb5b, /// 0x688
			 0x921832b7, /// 0x68c
			 0xc1c91692, /// 0x690
			 0xc9521754, /// 0x694
			 0xf9ca4083, /// 0x698
			 0xe848d80e, /// 0x69c
			 0xe15294b7, /// 0x6a0
			 0x39637ee9, /// 0x6a4
			 0x423c82e3, /// 0x6a8
			 0xb316e9fa, /// 0x6ac
			 0x5ea8f1c3, /// 0x6b0
			 0xb0c6358c, /// 0x6b4
			 0x5d4c5fb6, /// 0x6b8
			 0xfc815a66, /// 0x6bc
			 0x21f3d264, /// 0x6c0
			 0x1bf189d1, /// 0x6c4
			 0x3f96c5d4, /// 0x6c8
			 0xad603593, /// 0x6cc
			 0x19072447, /// 0x6d0
			 0xe8a3ab3c, /// 0x6d4
			 0x3109a78f, /// 0x6d8
			 0x766d7bb5, /// 0x6dc
			 0x03296cb2, /// 0x6e0
			 0xec5c202c, /// 0x6e4
			 0x23a1eac3, /// 0x6e8
			 0x273f9b66, /// 0x6ec
			 0xbc705297, /// 0x6f0
			 0x43e1a61b, /// 0x6f4
			 0xc54e5db7, /// 0x6f8
			 0xc178401c, /// 0x6fc
			 0x3566560a, /// 0x700
			 0xb6cdb39e, /// 0x704
			 0x7061d9fe, /// 0x708
			 0xf8eb97d2, /// 0x70c
			 0x47999f40, /// 0x710
			 0xecf214ed, /// 0x714
			 0x981def31, /// 0x718
			 0xcf1e6073, /// 0x71c
			 0xcfbefb94, /// 0x720
			 0x1c122a36, /// 0x724
			 0x032aaa8e, /// 0x728
			 0xc99aad96, /// 0x72c
			 0x70ce8b57, /// 0x730
			 0xc03641ef, /// 0x734
			 0x7d2d40d5, /// 0x738
			 0x2208b73b, /// 0x73c
			 0x881a1a41, /// 0x740
			 0xee24bd3e, /// 0x744
			 0x8f4a91b3, /// 0x748
			 0x150df3b3, /// 0x74c
			 0xf9f38b15, /// 0x750
			 0x9f271908, /// 0x754
			 0x4fc8cfd8, /// 0x758
			 0xa596dd81, /// 0x75c
			 0x54e4dfb5, /// 0x760
			 0x6eee81e6, /// 0x764
			 0x27a42086, /// 0x768
			 0x3eef5117, /// 0x76c
			 0xbe5e0d62, /// 0x770
			 0x6d9c5bba, /// 0x774
			 0x06503e23, /// 0x778
			 0x76e1afcf, /// 0x77c
			 0xab86feb8, /// 0x780
			 0x883205de, /// 0x784
			 0xffcde165, /// 0x788
			 0x6990eeb8, /// 0x78c
			 0xc296ed48, /// 0x790
			 0x17c7cca3, /// 0x794
			 0xad967757, /// 0x798
			 0x74c95d4b, /// 0x79c
			 0x0d2587d0, /// 0x7a0
			 0xca843ea3, /// 0x7a4
			 0x88d550bd, /// 0x7a8
			 0xd372e609, /// 0x7ac
			 0x7e8e5667, /// 0x7b0
			 0x7e23e6b6, /// 0x7b4
			 0xd4828dca, /// 0x7b8
			 0x5a7faacb, /// 0x7bc
			 0x53f957fc, /// 0x7c0
			 0xf19f0084, /// 0x7c4
			 0x9bacfae3, /// 0x7c8
			 0xdacc54f2, /// 0x7cc
			 0xcde36860, /// 0x7d0
			 0xc5a5f2f2, /// 0x7d4
			 0xacf87612, /// 0x7d8
			 0xa84f62a8, /// 0x7dc
			 0x1ccc44d6, /// 0x7e0
			 0x32b856c8, /// 0x7e4
			 0xaa628f82, /// 0x7e8
			 0x3f910324, /// 0x7ec
			 0x7816882f, /// 0x7f0
			 0xbacfb4a0, /// 0x7f4
			 0x05b79844, /// 0x7f8
			 0x9108775a, /// 0x7fc
			 0x3977bee6, /// 0x800
			 0x8b705f9d, /// 0x804
			 0x5bb44034, /// 0x808
			 0xeea949f6, /// 0x80c
			 0xf03ffe29, /// 0x810
			 0xc25f249b, /// 0x814
			 0xf5bcda27, /// 0x818
			 0x4068ba20, /// 0x81c
			 0x292726f3, /// 0x820
			 0x3a56513a, /// 0x824
			 0x539252a2, /// 0x828
			 0x43a5dcae, /// 0x82c
			 0x382a42fc, /// 0x830
			 0x69c519a5, /// 0x834
			 0x033b32bf, /// 0x838
			 0x0e2cc30c, /// 0x83c
			 0xa5b6bb77, /// 0x840
			 0x998e4b14, /// 0x844
			 0x26bc1d0d, /// 0x848
			 0xcc9af063, /// 0x84c
			 0xb710d7c0, /// 0x850
			 0x019fa024, /// 0x854
			 0x4797f77d, /// 0x858
			 0xdba67ef0, /// 0x85c
			 0xddbfdffb, /// 0x860
			 0x1646e2b4, /// 0x864
			 0x17d67a70, /// 0x868
			 0x92046e30, /// 0x86c
			 0x86fb47d4, /// 0x870
			 0x43cf2640, /// 0x874
			 0x39e8ea52, /// 0x878
			 0xcbfe7340, /// 0x87c
			 0x79cb4421, /// 0x880
			 0xa4b8d3d8, /// 0x884
			 0xfbbfb205, /// 0x888
			 0x2eff71b6, /// 0x88c
			 0x7a58833c, /// 0x890
			 0x7ff78717, /// 0x894
			 0x550361ad, /// 0x898
			 0x1ff13f1a, /// 0x89c
			 0x73ace452, /// 0x8a0
			 0x9e862ee3, /// 0x8a4
			 0xaf34e0ec, /// 0x8a8
			 0x19d06258, /// 0x8ac
			 0x576b36df, /// 0x8b0
			 0x92446bcc, /// 0x8b4
			 0xb58ac9b3, /// 0x8b8
			 0x25930eb3, /// 0x8bc
			 0x5f51f422, /// 0x8c0
			 0x35e51469, /// 0x8c4
			 0xe206812d, /// 0x8c8
			 0x04a7bc18, /// 0x8cc
			 0x8b4081f3, /// 0x8d0
			 0x166e64d6, /// 0x8d4
			 0xbb6b4f9c, /// 0x8d8
			 0x72461ff6, /// 0x8dc
			 0x4a45debd, /// 0x8e0
			 0x1c59afa3, /// 0x8e4
			 0x51a93d4d, /// 0x8e8
			 0xb8837506, /// 0x8ec
			 0x9617a858, /// 0x8f0
			 0x1b0bcfe9, /// 0x8f4
			 0x0afb36b5, /// 0x8f8
			 0x07019e27, /// 0x8fc
			 0xb592a228, /// 0x900
			 0x06a8d760, /// 0x904
			 0x92e57b93, /// 0x908
			 0x840cbf2b, /// 0x90c
			 0xc75c9f42, /// 0x910
			 0xb572f072, /// 0x914
			 0xf120a703, /// 0x918
			 0x80542ee0, /// 0x91c
			 0xb73c6b2c, /// 0x920
			 0x129b869a, /// 0x924
			 0x1ecdf79e, /// 0x928
			 0x5648a2b4, /// 0x92c
			 0x37bde20c, /// 0x930
			 0xd89901b3, /// 0x934
			 0x60e20291, /// 0x938
			 0x2dd105cf, /// 0x93c
			 0x5477afa6, /// 0x940
			 0x95857c29, /// 0x944
			 0x28b968ab, /// 0x948
			 0x0052e1ed, /// 0x94c
			 0x2b9df84c, /// 0x950
			 0x79c2a6af, /// 0x954
			 0x97614d55, /// 0x958
			 0x7b9601fb, /// 0x95c
			 0x81ddac1c, /// 0x960
			 0x4384209e, /// 0x964
			 0x7c585f30, /// 0x968
			 0x5f957c18, /// 0x96c
			 0x13a226dd, /// 0x970
			 0xe86d4937, /// 0x974
			 0xf52cc91e, /// 0x978
			 0x54ac1f6e, /// 0x97c
			 0x84bfd89f, /// 0x980
			 0x3100ff09, /// 0x984
			 0x6a0339d5, /// 0x988
			 0x169dec9e, /// 0x98c
			 0xd9477c14, /// 0x990
			 0x394c5821, /// 0x994
			 0x0ad7bc41, /// 0x998
			 0x3db8dcc0, /// 0x99c
			 0xed4e05bd, /// 0x9a0
			 0xe5c8ca1c, /// 0x9a4
			 0x134bd765, /// 0x9a8
			 0xc54eb7a2, /// 0x9ac
			 0x22526c16, /// 0x9b0
			 0x6d731413, /// 0x9b4
			 0xee2892c9, /// 0x9b8
			 0x22a43c0f, /// 0x9bc
			 0xfbd4aed5, /// 0x9c0
			 0xc28bf60d, /// 0x9c4
			 0x59fc2138, /// 0x9c8
			 0x61da92f9, /// 0x9cc
			 0xd85137b5, /// 0x9d0
			 0x83254400, /// 0x9d4
			 0xf151ca7a, /// 0x9d8
			 0x00e566be, /// 0x9dc
			 0xc513a955, /// 0x9e0
			 0x56e9f411, /// 0x9e4
			 0xee37fdb3, /// 0x9e8
			 0x7121e8bf, /// 0x9ec
			 0x1954820b, /// 0x9f0
			 0x6dc41e76, /// 0x9f4
			 0x6981744f, /// 0x9f8
			 0xf9020564, /// 0x9fc
			 0xa3430976, /// 0xa00
			 0xb5f397d4, /// 0xa04
			 0x35738112, /// 0xa08
			 0xf4a6a2e6, /// 0xa0c
			 0xb6fc2818, /// 0xa10
			 0xada30350, /// 0xa14
			 0xd6d30d2d, /// 0xa18
			 0xf600cbfb, /// 0xa1c
			 0xd18d5ba1, /// 0xa20
			 0x4855a627, /// 0xa24
			 0x31c93b97, /// 0xa28
			 0xd9f8e289, /// 0xa2c
			 0x22580925, /// 0xa30
			 0xe7d8fac6, /// 0xa34
			 0x474cbf0a, /// 0xa38
			 0x86dc3e8c, /// 0xa3c
			 0x992a7e6c, /// 0xa40
			 0xf0a8b644, /// 0xa44
			 0xc4365ff7, /// 0xa48
			 0xebfa2e9c, /// 0xa4c
			 0xcf1249c3, /// 0xa50
			 0xcd205ef4, /// 0xa54
			 0x36c60177, /// 0xa58
			 0xb3e9bb1d, /// 0xa5c
			 0x3e5b8f86, /// 0xa60
			 0x41344d45, /// 0xa64
			 0x1eb6f2af, /// 0xa68
			 0x366ba630, /// 0xa6c
			 0x4ff6e209, /// 0xa70
			 0x817a5df0, /// 0xa74
			 0x0cf1894e, /// 0xa78
			 0xe5a84371, /// 0xa7c
			 0xec4ff153, /// 0xa80
			 0x569a0987, /// 0xa84
			 0x94470bce, /// 0xa88
			 0x94c78448, /// 0xa8c
			 0x5ce0af12, /// 0xa90
			 0x4df253f2, /// 0xa94
			 0x65dc6645, /// 0xa98
			 0x4bda8b31, /// 0xa9c
			 0xeb7c8e0c, /// 0xaa0
			 0x04f39bf4, /// 0xaa4
			 0x8dd1543b, /// 0xaa8
			 0xc2a10c99, /// 0xaac
			 0x868d61f1, /// 0xab0
			 0x0d3eb591, /// 0xab4
			 0x3d544cb5, /// 0xab8
			 0x434c5df6, /// 0xabc
			 0x374823c0, /// 0xac0
			 0x67c12b15, /// 0xac4
			 0x66246c14, /// 0xac8
			 0xd173608a, /// 0xacc
			 0x51472ca6, /// 0xad0
			 0xe3059649, /// 0xad4
			 0xedc912c2, /// 0xad8
			 0xad942991, /// 0xadc
			 0x59f8f6bd, /// 0xae0
			 0x0dee9e44, /// 0xae4
			 0xaf917bff, /// 0xae8
			 0x13cbb0b2, /// 0xaec
			 0x023ca5c9, /// 0xaf0
			 0x9787166f, /// 0xaf4
			 0x1c7dcecf, /// 0xaf8
			 0x57cd5eca, /// 0xafc
			 0x3e26f6a5, /// 0xb00
			 0x3fdc7d72, /// 0xb04
			 0xd05c42b2, /// 0xb08
			 0xbe7022ce, /// 0xb0c
			 0xc1dfffb6, /// 0xb10
			 0x3c8bf47d, /// 0xb14
			 0x57543872, /// 0xb18
			 0xa6133ea5, /// 0xb1c
			 0x5caa0c89, /// 0xb20
			 0x55134936, /// 0xb24
			 0xd8601a4b, /// 0xb28
			 0x198452a7, /// 0xb2c
			 0x967de92a, /// 0xb30
			 0xd48c6fbe, /// 0xb34
			 0x80c4b46d, /// 0xb38
			 0x542939c0, /// 0xb3c
			 0x55fd10cf, /// 0xb40
			 0xb367d466, /// 0xb44
			 0x9d6a3ce5, /// 0xb48
			 0x7ee993ef, /// 0xb4c
			 0x4855c8ae, /// 0xb50
			 0x7c3c8817, /// 0xb54
			 0xb25ba97f, /// 0xb58
			 0xf1aa3ef9, /// 0xb5c
			 0xdb88be43, /// 0xb60
			 0x1b6c2934, /// 0xb64
			 0x4e0ffcf6, /// 0xb68
			 0xcc795aa6, /// 0xb6c
			 0x5f225f90, /// 0xb70
			 0x95f1f55f, /// 0xb74
			 0xa0948381, /// 0xb78
			 0xbe15e627, /// 0xb7c
			 0xf64a3377, /// 0xb80
			 0x678009ae, /// 0xb84
			 0x0042b0e4, /// 0xb88
			 0x32188ffd, /// 0xb8c
			 0x30772824, /// 0xb90
			 0x2a888550, /// 0xb94
			 0xa585007e, /// 0xb98
			 0x7084bb7a, /// 0xb9c
			 0xe2c0b6c8, /// 0xba0
			 0x2005d592, /// 0xba4
			 0x84dc5710, /// 0xba8
			 0xd9cac5a7, /// 0xbac
			 0x0c64c54b, /// 0xbb0
			 0x686fc406, /// 0xbb4
			 0x80e4e34c, /// 0xbb8
			 0xd3e80884, /// 0xbbc
			 0x9992519c, /// 0xbc0
			 0x7135bfe1, /// 0xbc4
			 0xbd5ebf8b, /// 0xbc8
			 0xb20b3ce5, /// 0xbcc
			 0x396747a7, /// 0xbd0
			 0x9bc4bb5e, /// 0xbd4
			 0x7db5be36, /// 0xbd8
			 0xcc537707, /// 0xbdc
			 0xeee4026f, /// 0xbe0
			 0xd50f0795, /// 0xbe4
			 0x968c3d67, /// 0xbe8
			 0xa026b2c4, /// 0xbec
			 0xcda366d5, /// 0xbf0
			 0x9eb9738a, /// 0xbf4
			 0x3727fcb7, /// 0xbf8
			 0x157116b8, /// 0xbfc
			 0x2f91320a, /// 0xc00
			 0x9c1ab539, /// 0xc04
			 0x37698fc0, /// 0xc08
			 0x76ad3c74, /// 0xc0c
			 0xe59b7628, /// 0xc10
			 0x4a51e4c8, /// 0xc14
			 0xf9922fdf, /// 0xc18
			 0x39f329b2, /// 0xc1c
			 0x861162b0, /// 0xc20
			 0x180d3ae2, /// 0xc24
			 0x3cd9b439, /// 0xc28
			 0x66ef31c9, /// 0xc2c
			 0xb43691dd, /// 0xc30
			 0x0902544a, /// 0xc34
			 0xa080e66a, /// 0xc38
			 0x4f073906, /// 0xc3c
			 0x12b7e56c, /// 0xc40
			 0x8c813a02, /// 0xc44
			 0x158d3b98, /// 0xc48
			 0x09a9f2fa, /// 0xc4c
			 0xa3fbf902, /// 0xc50
			 0x755ef40e, /// 0xc54
			 0xf30575d0, /// 0xc58
			 0xe3147c46, /// 0xc5c
			 0xcb9e37a3, /// 0xc60
			 0x48e290d4, /// 0xc64
			 0x82c3ec0b, /// 0xc68
			 0x69a38f06, /// 0xc6c
			 0x8ecbc5a4, /// 0xc70
			 0x07e372e3, /// 0xc74
			 0xdfeef437, /// 0xc78
			 0xee76ccdf, /// 0xc7c
			 0xe89ab321, /// 0xc80
			 0x613e4e5f, /// 0xc84
			 0xcbf0e6f4, /// 0xc88
			 0x8240abb4, /// 0xc8c
			 0x6ee6ac5a, /// 0xc90
			 0xe2eb7110, /// 0xc94
			 0x86488fba, /// 0xc98
			 0x17803814, /// 0xc9c
			 0x7b532c13, /// 0xca0
			 0xa3e9cfba, /// 0xca4
			 0x420c8549, /// 0xca8
			 0x156b2927, /// 0xcac
			 0x1b2f7f22, /// 0xcb0
			 0xb0a6ab5b, /// 0xcb4
			 0xdc8d1f3c, /// 0xcb8
			 0x38eaa1db, /// 0xcbc
			 0xbd2bad98, /// 0xcc0
			 0xb7f95c1b, /// 0xcc4
			 0x7c5941f7, /// 0xcc8
			 0xb4a1aeca, /// 0xccc
			 0xa7321641, /// 0xcd0
			 0xad379f9c, /// 0xcd4
			 0x5a5ae65e, /// 0xcd8
			 0x097a8c5f, /// 0xcdc
			 0x56204ba3, /// 0xce0
			 0x8f366594, /// 0xce4
			 0x63a9cfd9, /// 0xce8
			 0x9ebaf92d, /// 0xcec
			 0x727fd00d, /// 0xcf0
			 0x4939b426, /// 0xcf4
			 0xb9dbebbe, /// 0xcf8
			 0x304b739f, /// 0xcfc
			 0x2cac324d, /// 0xd00
			 0x2f434952, /// 0xd04
			 0xe8226e27, /// 0xd08
			 0xf0e7a973, /// 0xd0c
			 0x751aff26, /// 0xd10
			 0x0b36ef5c, /// 0xd14
			 0x293bf6ce, /// 0xd18
			 0x54031302, /// 0xd1c
			 0xc3a37e1f, /// 0xd20
			 0xd172821e, /// 0xd24
			 0xf7285933, /// 0xd28
			 0xee9d0f3f, /// 0xd2c
			 0xe538a706, /// 0xd30
			 0x5da41843, /// 0xd34
			 0x5384002d, /// 0xd38
			 0xc75c4bfe, /// 0xd3c
			 0x1c7c5c7f, /// 0xd40
			 0xc9c4ced3, /// 0xd44
			 0xd4177797, /// 0xd48
			 0x9c49f07d, /// 0xd4c
			 0xe34643ca, /// 0xd50
			 0x13b5e320, /// 0xd54
			 0x7ba27380, /// 0xd58
			 0x5e07952c, /// 0xd5c
			 0x52de11f2, /// 0xd60
			 0x6211a3be, /// 0xd64
			 0xd6c510ac, /// 0xd68
			 0xf2e5e581, /// 0xd6c
			 0x76b3980e, /// 0xd70
			 0xe16a28c6, /// 0xd74
			 0xaf5cef2b, /// 0xd78
			 0x9d90635e, /// 0xd7c
			 0xf9d12ad6, /// 0xd80
			 0x68ae7914, /// 0xd84
			 0x43f2ede0, /// 0xd88
			 0x73184fc6, /// 0xd8c
			 0xab72eec2, /// 0xd90
			 0x3cc9f439, /// 0xd94
			 0x3dff6fe5, /// 0xd98
			 0x5eafb3b3, /// 0xd9c
			 0x42b7e118, /// 0xda0
			 0x596c0055, /// 0xda4
			 0x9f980723, /// 0xda8
			 0x2d1f41b9, /// 0xdac
			 0xe1a6c803, /// 0xdb0
			 0xaddac381, /// 0xdb4
			 0x06e87d53, /// 0xdb8
			 0x4dbf612a, /// 0xdbc
			 0xad3d001b, /// 0xdc0
			 0xf4e1d772, /// 0xdc4
			 0x7ca7f4b5, /// 0xdc8
			 0xbe1960b5, /// 0xdcc
			 0xedce5ee5, /// 0xdd0
			 0xe7107aa2, /// 0xdd4
			 0xc15f3aaf, /// 0xdd8
			 0xf1359aa9, /// 0xddc
			 0x1d29ee6f, /// 0xde0
			 0xfe74c043, /// 0xde4
			 0xc891efed, /// 0xde8
			 0xb91d6113, /// 0xdec
			 0xe2b76bed, /// 0xdf0
			 0x309414f5, /// 0xdf4
			 0x0ad66139, /// 0xdf8
			 0x3ce9a6a5, /// 0xdfc
			 0x57e40cb7, /// 0xe00
			 0x1454709e, /// 0xe04
			 0x8f61fcf5, /// 0xe08
			 0x2f1d61d0, /// 0xe0c
			 0x8c1418cc, /// 0xe10
			 0x6be93c01, /// 0xe14
			 0x163e06d3, /// 0xe18
			 0x293a8bb2, /// 0xe1c
			 0x132b8ed3, /// 0xe20
			 0xef76f4c3, /// 0xe24
			 0x865bb059, /// 0xe28
			 0x245e25c9, /// 0xe2c
			 0xb2f8256e, /// 0xe30
			 0x39985bbb, /// 0xe34
			 0xee27e47d, /// 0xe38
			 0x06416c2c, /// 0xe3c
			 0x0fb2f32a, /// 0xe40
			 0x7911ec51, /// 0xe44
			 0x1b678e1e, /// 0xe48
			 0xb3edc20b, /// 0xe4c
			 0x356610c1, /// 0xe50
			 0x5cffb1dc, /// 0xe54
			 0xc66ae7e4, /// 0xe58
			 0xd9f26c00, /// 0xe5c
			 0xf1c0a394, /// 0xe60
			 0x0b560514, /// 0xe64
			 0x3d8d7044, /// 0xe68
			 0xf075cab6, /// 0xe6c
			 0xfb0c30fb, /// 0xe70
			 0x52735c96, /// 0xe74
			 0x76355f26, /// 0xe78
			 0xdacfda40, /// 0xe7c
			 0xaaca68cc, /// 0xe80
			 0x617b6ab2, /// 0xe84
			 0xe1754bc0, /// 0xe88
			 0x7571616f, /// 0xe8c
			 0x4d7e050b, /// 0xe90
			 0xcd893320, /// 0xe94
			 0x382c0600, /// 0xe98
			 0x094d99e2, /// 0xe9c
			 0x58e1dc2d, /// 0xea0
			 0x631f5a71, /// 0xea4
			 0x10129db5, /// 0xea8
			 0xf9e07e5a, /// 0xeac
			 0xd3adc8c4, /// 0xeb0
			 0x3ec73e19, /// 0xeb4
			 0xd1a1abfe, /// 0xeb8
			 0x55156769, /// 0xebc
			 0x72dd5767, /// 0xec0
			 0x4cb7c339, /// 0xec4
			 0x0ecfe0c6, /// 0xec8
			 0x3c1dfea1, /// 0xecc
			 0xccacbbb7, /// 0xed0
			 0xb240bac0, /// 0xed4
			 0xa5528ad5, /// 0xed8
			 0xe5ff5716, /// 0xedc
			 0x7a783e74, /// 0xee0
			 0x7a9c8489, /// 0xee4
			 0xa16a7dda, /// 0xee8
			 0xfb6ce9e4, /// 0xeec
			 0x44288f28, /// 0xef0
			 0xf21921f3, /// 0xef4
			 0x5d0f0191, /// 0xef8
			 0xd5ff1caa, /// 0xefc
			 0x86511ea1, /// 0xf00
			 0xaf4399ff, /// 0xf04
			 0x81518bbf, /// 0xf08
			 0xdb7c57ab, /// 0xf0c
			 0x9ff89900, /// 0xf10
			 0x7016594a, /// 0xf14
			 0x9998605d, /// 0xf18
			 0x89f1dcb8, /// 0xf1c
			 0x92fdc147, /// 0xf20
			 0x6f6da2db, /// 0xf24
			 0xbb168fae, /// 0xf28
			 0x39af3b2a, /// 0xf2c
			 0xf30286e5, /// 0xf30
			 0xec5225ba, /// 0xf34
			 0xba65b07f, /// 0xf38
			 0x54e3a57b, /// 0xf3c
			 0xa555a380, /// 0xf40
			 0x8833814a, /// 0xf44
			 0xbe48cdc7, /// 0xf48
			 0x7619dc26, /// 0xf4c
			 0x6fb885fa, /// 0xf50
			 0x5f02f74a, /// 0xf54
			 0x5b44b55c, /// 0xf58
			 0x1cdb14e4, /// 0xf5c
			 0xb86ee558, /// 0xf60
			 0x1172d846, /// 0xf64
			 0xedb93d85, /// 0xf68
			 0xe57db2e1, /// 0xf6c
			 0x51d22359, /// 0xf70
			 0x33df21d8, /// 0xf74
			 0x5482c7ba, /// 0xf78
			 0xed317468, /// 0xf7c
			 0x41a1b9c4, /// 0xf80
			 0xe86e3284, /// 0xf84
			 0x425d1466, /// 0xf88
			 0xdf7dc07a, /// 0xf8c
			 0xd8de73e6, /// 0xf90
			 0x42e533bf, /// 0xf94
			 0x96af9a80, /// 0xf98
			 0xcbc82cb7, /// 0xf9c
			 0xa9108cfa, /// 0xfa0
			 0xb4ad2707, /// 0xfa4
			 0x00f8e2f1, /// 0xfa8
			 0x3e052b29, /// 0xfac
			 0xd8f58bd6, /// 0xfb0
			 0xf6c2beb2, /// 0xfb4
			 0x21899c31, /// 0xfb8
			 0x59c1c07d, /// 0xfbc
			 0xfff9d4ae, /// 0xfc0
			 0xee5eee76, /// 0xfc4
			 0x0c6042ae, /// 0xfc8
			 0xbad94cd0, /// 0xfcc
			 0xe15f8ee1, /// 0xfd0
			 0xbc27f76a, /// 0xfd4
			 0x649b19c4, /// 0xfd8
			 0x0243f444, /// 0xfdc
			 0x882917bf, /// 0xfe0
			 0x6b6c92e4, /// 0xfe4
			 0xba20fa55, /// 0xfe8
			 0xd3e38a51, /// 0xfec
			 0xceb74edc, /// 0xff0
			 0xcf2d0ab0, /// 0xff4
			 0xe64b1a44, /// 0xff8
			 0xb72c6603 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00008
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00010
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00018
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0001c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00020
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00024
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00028
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0002c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00030
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00034
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00038
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00040
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00044
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00048
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0004c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00054
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00058
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0005c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00064
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00068
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0006c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00070
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00074
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00078
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0007c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00080
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00084
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0008c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00090
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00094
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0009c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000a0
			 0x4b000000,                                                  // 8388608.0                                   /// 000a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00100
			 0x0e000003,                                                  // Trailing 1s:                                /// 00104
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00108
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0010c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00110
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00118
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0011c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00124
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00128
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0012c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00130
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00134
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00138
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0013c
			 0x4b000000,                                                  // 8388608.0                                   /// 00140
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00144
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00148
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0014c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00150
			 0x0c700000,                                                  // Leading 1s:                                 /// 00154
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00158
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0015c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00160
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00164
			 0x7fc00001,                                                  // signaling NaN                               /// 00168
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0016c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00170
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00174
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0017c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00180
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00184
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00188
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00190
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00194
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0019c
			 0x0c600000,                                                  // Leading 1s:                                 /// 001a0
			 0x4b000000,                                                  // 8388608.0                                   /// 001a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 001cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 001dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00200
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00204
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00208
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00218
			 0xbf028f5c,                                                  // -0.51                                       /// 0021c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00220
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0x33333333,                                                  // 4 random values                             /// 00228
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0022c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00230
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00234
			 0x80000000,                                                  // -zero                                       /// 00238
			 0x00011111,                                                  // 9.7958E-41                                  /// 0023c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00240
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00244
			 0xffc00001,                                                  // -signaling NaN                              /// 00248
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0024c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00254
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0025c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0026c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00270
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00274
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x3f028f5c,                                                  // 0.51                                        /// 0027c
			 0x7f800000,                                                  // inf                                         /// 00280
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00284
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00288
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0029c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 002c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002d0
			 0x7fc00001,                                                  // signaling NaN                               /// 002d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00304
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00308
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0030c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00310
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00314
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00318
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00320
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0032c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00330
			 0x0e000003,                                                  // Trailing 1s:                                /// 00334
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00338
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0033c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00344
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0034c
			 0x7f800000,                                                  // inf                                         /// 00350
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00354
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00358
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x4b000000,                                                  // 8388608.0                                   /// 00364
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00368
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0036c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00370
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00374
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00378
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00384
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0e000001,                                                  // Trailing 1s:                                /// 0038c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00390
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00394
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0xffc00001,                                                  // -signaling NaN                              /// 003a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003b0
			 0x4b000000,                                                  // 8388608.0                                   /// 003b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0c600000,                                                  // Leading 1s:                                 /// 003e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00400
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00404
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00408
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00410
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00418
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0041c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00420
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00424
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00434
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00438
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00440
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00444
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00448
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0044c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00450
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00454
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00458
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0045c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00468
			 0x0e000007,                                                  // Trailing 1s:                                /// 0046c
			 0x00000000,                                                  // zero                                        /// 00470
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00474
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00478
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00480
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00484
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00488
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0048c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00490
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00498
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0049c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004ac
			 0x55555555,                                                  // 4 random values                             /// 004b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00508
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0050c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00518
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0051c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00528
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0052c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00530
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00534
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00540
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00544
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00548
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00550
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00558
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00560
			 0x80000000,                                                  // -zero                                       /// 00564
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0056c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00574
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0057c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00580
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0058c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00590
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00594
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00598
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0059c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005a8
			 0x7f800000,                                                  // inf                                         /// 005ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xff800000,                                                  // -inf                                        /// 005b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00600
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00608
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0060c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00610
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00614
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0061c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00620
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00624
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00628
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00630
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00634
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00638
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0063c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00644
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00648
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0064c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00650
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00658
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0065c
			 0x80000000,                                                  // -zero                                       /// 00660
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00664
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00668
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0066c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00678
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0067c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00680
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00684
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00688
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0068c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00690
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00698
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0069c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006b8
			 0xcb000000,                                                  // -8388608.0                                  /// 006bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 006f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00700
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00704
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0070c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00710
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00718
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00728
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00730
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x7f800000,                                                  // inf                                         /// 00738
			 0x7fc00001,                                                  // signaling NaN                               /// 0073c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00740
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00744
			 0x3f028f5c,                                                  // 0.51                                        /// 00748
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0074c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00750
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00758
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0075c
			 0x80000000,                                                  // -zero                                       /// 00760
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00764
			 0x0e000001,                                                  // Trailing 1s:                                /// 00768
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0076c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00774
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00778
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0077c
			 0xffc00001,                                                  // -signaling NaN                              /// 00780
			 0xff800000,                                                  // -inf                                        /// 00784
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0078c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00790
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00794
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00798
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0079c
			 0x7f800000,                                                  // inf                                         /// 007a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00810
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00814
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00818
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0081c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00820
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00824
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00828
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0082c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00834
			 0x4b000000,                                                  // 8388608.0                                   /// 00838
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0083c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x80011111,                                                  // -9.7958E-41                                 /// 00844
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00848
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0084c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00850
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00854
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0085c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00864
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00868
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0086c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00870
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0087c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00880
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00884
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0xff800000,                                                  // -inf                                        /// 00890
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00894
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0089c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0xbf028f5c,                                                  // -0.51                                       /// 008ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 008c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008c8
			 0x55555555,                                                  // 4 random values                             /// 008cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0xbf028f5c,                                                  // -0.51                                       /// 008dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00900
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00908
			 0x7f800000,                                                  // inf                                         /// 0090c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00910
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0091c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00920
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00924
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00928
			 0x00011111,                                                  // 9.7958E-41                                  /// 0092c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00934
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0xff800000,                                                  // -inf                                        /// 00944
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00948
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0094c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00950
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00954
			 0x00000000,                                                  // zero                                        /// 00958
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0095c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00960
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00968
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0096c
			 0x00000000,                                                  // zero                                        /// 00970
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00974
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00980
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00984
			 0x00000000,                                                  // zero                                        /// 00988
			 0xffc00001,                                                  // -signaling NaN                              /// 0098c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00998
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00a00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a04
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a08
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a14
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a18
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a24
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x7f800000,                                                  // inf                                         /// 00a34
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a38
			 0x33333333,                                                  // 4 random values                             /// 00a3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a40
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a4c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a54
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a5c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a64
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a68
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a6c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a74
			 0xbf028f5c,                                                  // -0.51                                       /// 00a78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a84
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a88
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a8c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a90
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a94
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a98
			 0xbf028f5c,                                                  // -0.51                                       /// 00a9c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00aa0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ab0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ab4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00abc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ad0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ad4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ad8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00adc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ae0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00aec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00af0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00af4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00af8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00afc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b00
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b10
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b14
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b1c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b24
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b28
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b38
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b3c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b44
			 0xff800000,                                                  // -inf                                        /// 00b48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b50
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x3f028f5c,                                                  // 0.51                                        /// 00b64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b78
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b94
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b98
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ba0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ba4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ba8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bbc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bc4
			 0x7fc00001,                                                  // signaling NaN                               /// 00bc8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bd4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bdc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00be0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x4b000000,                                                  // 8388608.0                                   /// 00be8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bf0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bf4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bf8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bfc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c00
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c08
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c10
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c14
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c18
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c1c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c20
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c28
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c2c
			 0xff800000,                                                  // -inf                                        /// 00c30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x7fc00001,                                                  // signaling NaN                               /// 00c3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0xff800000,                                                  // -inf                                        /// 00c48
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c5c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c6c
			 0x55555555,                                                  // 4 random values                             /// 00c70
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c74
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c7c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ca8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cb4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cbc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cc0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cc8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cd0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ce4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ce8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cfc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d00
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d10
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d1c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d2c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d38
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d3c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x7f800000,                                                  // inf                                         /// 00d50
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d54
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d58
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d64
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d70
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d84
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d90
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d94
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d98
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00da0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00da4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00db0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00db8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dc0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dcc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dd4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ddc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00de0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00de4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00de8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00df0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00df8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dfc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e08
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e0c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e10
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e14
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e20
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e24
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e3c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e4c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e50
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e5c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e60
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e64
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e70
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e78
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e94
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e98
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ea4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00eac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eb0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ec0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ec4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ec8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ed0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ed4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00edc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ee0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ee8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00eec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ef4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ef8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00efc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f28
			 0x55555555,                                                  // 4 random values                             /// 00f2c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f30
			 0xffc00001,                                                  // -signaling NaN                              /// 00f34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f40
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f44
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f48
			 0x33333333,                                                  // 4 random values                             /// 00f4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f50
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f54
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f64
			 0x55555555,                                                  // 4 random values                             /// 00f68
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f78
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f7c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f84
			 0x00000000,                                                  // zero                                        /// 00f88
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f8c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x7f800000,                                                  // inf                                         /// 00f98
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fa0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fa8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fcc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fd0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fd4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fd8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fe0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fe4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fec
			 0x33333333,                                                  // 4 random values                             /// 00ff0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ff8
			 0x80004000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00000
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00004
			 0x7fc00001,                                                  // signaling NaN                               /// 00008
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00010
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00014
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00018
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0001c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00020
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00024
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00028
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0002c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00038
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0003c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00040
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00044
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00048
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0004c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00054
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00058
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0005c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00060
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00064
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00068
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0006c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00070
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00074
			 0x0c600000,                                                  // Leading 1s:                                 /// 00078
			 0x00011111,                                                  // 9.7958E-41                                  /// 0007c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00084
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00088
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0008c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00090
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00094
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0009c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x3f028f5c,                                                  // 0.51                                        /// 000c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000d0
			 0x7f800000,                                                  // inf                                         /// 000d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00104
			 0xff800000,                                                  // -inf                                        /// 00108
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0010c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00110
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00118
			 0x3f028f5c,                                                  // 0.51                                        /// 0011c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00124
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0012c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00134
			 0x0e000001,                                                  // Trailing 1s:                                /// 00138
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00140
			 0xffc00001,                                                  // -signaling NaN                              /// 00144
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00148
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0014c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0015c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00164
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00168
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0016c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0e000001,                                                  // Trailing 1s:                                /// 00174
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0017c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00180
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00184
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00188
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00190
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00194
			 0x3f028f5c,                                                  // 0.51                                        /// 00198
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001e8
			 0x33333333,                                                  // 4 random values                             /// 001ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001f4
			 0xcb000000,                                                  // -8388608.0                                  /// 001f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00200
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00204
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00208
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0020c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00210
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00214
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00220
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00230
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00234
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0023c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00240
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00248
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0024c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00250
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00254
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0025c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00264
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0026c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00270
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0027c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00288
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0028c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00290
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00294
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00298
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0029c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002a4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x7fc00001,                                                  // signaling NaN                               /// 002b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 002c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002dc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002f4
			 0x33333333,                                                  // 4 random values                             /// 002f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00300
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00308
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0030c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00314
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00320
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00324
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00328
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00334
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00338
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00340
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00348
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0034c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00350
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00358
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0035c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00360
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00368
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x0e000001,                                                  // Trailing 1s:                                /// 00378
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0037c
			 0x33333333,                                                  // 4 random values                             /// 00380
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00388
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0038c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00390
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00394
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003bc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003e4
			 0x33333333,                                                  // 4 random values                             /// 003e8
			 0xffc00001,                                                  // -signaling NaN                              /// 003ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00404
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0040c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x4b000000,                                                  // 8388608.0                                   /// 00424
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0042c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00434
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00438
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0043c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00440
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00444
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0e000003,                                                  // Trailing 1s:                                /// 0044c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00450
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00460
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00464
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00468
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0046c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00470
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00474
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00478
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0047c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00488
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0048c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00490
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00494
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 004ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004d0
			 0x3f028f5c,                                                  // 0.51                                        /// 004d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004f4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00500
			 0xbf028f5c,                                                  // -0.51                                       /// 00504
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00508
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0050c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00510
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00514
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00518
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00520
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00524
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00528
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0052c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00530
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00538
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0053c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00540
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00548
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0054c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x4b000000,                                                  // 8388608.0                                   /// 00558
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0055c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00560
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00564
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00568
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00578
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00580
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00584
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0xcb000000,                                                  // -8388608.0                                  /// 0058c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x0c400000,                                                  // Leading 1s:                                 /// 00594
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00598
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0059c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 005ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 005f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00600
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00604
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00608
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0060c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00614
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00618
			 0x0e000003,                                                  // Trailing 1s:                                /// 0061c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00620
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00624
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0062c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00630
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00634
			 0x4b000000,                                                  // 8388608.0                                   /// 00638
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00644
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0064c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00650
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00654
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00660
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x80011111,                                                  // -9.7958E-41                                 /// 0066c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00670
			 0x0c600000,                                                  // Leading 1s:                                 /// 00674
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0067c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00684
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00688
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0068c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00690
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00694
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00698
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0069c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00708
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00710
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00714
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00718
			 0x0c780000,                                                  // Leading 1s:                                 /// 0071c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00724
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00728
			 0x55555555,                                                  // 4 random values                             /// 0072c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00738
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0073c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00740
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00744
			 0x0e000003,                                                  // Trailing 1s:                                /// 00748
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0074c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00754
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00768
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0076c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00774
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00778
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00780
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00794
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00798
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0079c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00800
			 0x00011111,                                                  // 9.7958E-41                                  /// 00804
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00808
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00810
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00814
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00818
			 0x80000000,                                                  // -zero                                       /// 0081c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00820
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00824
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00830
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00834
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00838
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0083c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0084c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x0e000001,                                                  // Trailing 1s:                                /// 00854
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00858
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00860
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00868
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0086c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00874
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00880
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00884
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00888
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0088c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00894
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0089c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008f0
			 0xff800000,                                                  // -inf                                        /// 008f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008fc
			 0x33333333,                                                  // 4 random values                             /// 00900
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00904
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00908
			 0x00011111,                                                  // 9.7958E-41                                  /// 0090c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00910
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00914
			 0x4b000000,                                                  // 8388608.0                                   /// 00918
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0091c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00920
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00924
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00928
			 0x0c700000,                                                  // Leading 1s:                                 /// 0092c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00930
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00934
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0093c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00940
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00944
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0094c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00950
			 0x33333333,                                                  // 4 random values                             /// 00954
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0095c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00960
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00964
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00968
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00970
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00978
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00980
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00984
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00988
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0098c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00990
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00998
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009c4
			 0xff800000,                                                  // -inf                                        /// 009c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 009cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 009f0
			 0x7f800000,                                                  // inf                                         /// 009f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a0c
			 0x55555555,                                                  // 4 random values                             /// 00a10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a18
			 0x55555555,                                                  // 4 random values                             /// 00a1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a28
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a2c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a30
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a38
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a40
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a54
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a68
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a7c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a90
			 0x55555555,                                                  // 4 random values                             /// 00a94
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a98
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a9c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00aa0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00aa4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00aa8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ab8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ac4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ac8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00acc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ad0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ad4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ad8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ae0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ae4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ae8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00af0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00af8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b20
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b2c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b30
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b40
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b50
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x7f800000,                                                  // inf                                         /// 00b58
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b68
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b74
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b78
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b7c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b90
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b94
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b9c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ba8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bb0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bc4
			 0x55555555,                                                  // 4 random values                             /// 00bc8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bd4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bd8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bdc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00be4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bf0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bf8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bfc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c00
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c04
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c14
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c28
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c2c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x7f800000,                                                  // inf                                         /// 00c34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c38
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c44
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c60
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c68
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c6c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c74
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c84
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c9c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ca0
			 0x80000000,                                                  // -zero                                       /// 00ca4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0xcb000000,                                                  // -8388608.0                                  /// 00cac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cb0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cbc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cc4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cc8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ccc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cd4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cd8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cdc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ce0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ce4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ce8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cf0
			 0x55555555,                                                  // 4 random values                             /// 00cf4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cf8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d1c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d40
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d44
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d48
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d54
			 0x4b000000,                                                  // 8388608.0                                   /// 00d58
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d5c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d60
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d64
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d6c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d70
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d74
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d8c
			 0x80000000,                                                  // -zero                                       /// 00d90
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00da0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dbc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dc0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dc4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dc8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dd0
			 0x3f028f5c,                                                  // 0.51                                        /// 00dd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dd8
			 0x80000000,                                                  // -zero                                       /// 00ddc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00de4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00df0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00df8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dfc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e18
			 0x7f800000,                                                  // inf                                         /// 00e1c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e20
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e2c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e38
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e3c
			 0x55555555,                                                  // 4 random values                             /// 00e40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e44
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e54
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e5c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e60
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e94
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ea8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00eac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eb8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ebc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ec0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ec8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ecc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ed8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00edc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ee0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ee4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00eec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ef0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00efc
			 0x3f028f5c,                                                  // 0.51                                        /// 00f00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f04
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x80000000,                                                  // -zero                                       /// 00f18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f30
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f34
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f58
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f5c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f68
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00000000,                                                  // zero                                        /// 00f74
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f84
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fa0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fa4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fa8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fb0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fc0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fc4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fc8
			 0x00000000,                                                  // zero                                        /// 00fcc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fd0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fd8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fe0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ff4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x807ffffe // max subnorm - 1ulp                            // SP - ve numbers                             /// last
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
			 0x000006c4,
			 0x00000448,
			 0x00000704,
			 0x00000670,
			 0x000007a4,
			 0x00000374,
			 0x00000588,
			 0x000005a8,

			 /// vpu register f2
			 0x41d00000,
			 0x41700000,
			 0x41d00000,
			 0x41d00000,
			 0x40c00000,
			 0x41e00000,
			 0x41000000,
			 0x40000000,

			 /// vpu register f3
			 0x41b80000,
			 0x41000000,
			 0x40400000,
			 0x41b00000,
			 0x41700000,
			 0x41800000,
			 0x41880000,
			 0x41f80000,

			 /// vpu register f4
			 0x41b00000,
			 0x3f800000,
			 0x41f80000,
			 0x41d80000,
			 0x41300000,
			 0x41900000,
			 0x41500000,
			 0x41c00000,

			 /// vpu register f5
			 0x41980000,
			 0x41700000,
			 0x41a00000,
			 0x40000000,
			 0x41500000,
			 0x41b00000,
			 0x41a80000,
			 0x41a80000,

			 /// vpu register f6
			 0x41500000,
			 0x40c00000,
			 0x41500000,
			 0x41100000,
			 0x41e00000,
			 0x41e80000,
			 0x41d00000,
			 0x41880000,

			 /// vpu register f7
			 0x42000000,
			 0x41a00000,
			 0x40a00000,
			 0x40400000,
			 0x40a00000,
			 0x41300000,
			 0x41f80000,
			 0x41500000,

			 /// vpu register f8
			 0x41800000,
			 0x41500000,
			 0x41900000,
			 0x41d00000,
			 0x40400000,
			 0x40e00000,
			 0x40e00000,
			 0x41980000,

			 /// vpu register f9
			 0x40e00000,
			 0x41700000,
			 0x40400000,
			 0x41980000,
			 0x41000000,
			 0x41b00000,
			 0x41900000,
			 0x41d80000,

			 /// vpu register f10
			 0x41500000,
			 0x41d80000,
			 0x41f00000,
			 0x41a80000,
			 0x41e80000,
			 0x41a00000,
			 0x41f00000,
			 0x41880000,

			 /// vpu register f11
			 0x41100000,
			 0x41980000,
			 0x41600000,
			 0x40e00000,
			 0x41980000,
			 0x40400000,
			 0x40a00000,
			 0x40400000,

			 /// vpu register f12
			 0x42000000,
			 0x40800000,
			 0x41000000,
			 0x3f800000,
			 0x41900000,
			 0x41f00000,
			 0x41700000,
			 0x40a00000,

			 /// vpu register f13
			 0x41d00000,
			 0x41980000,
			 0x41d00000,
			 0x41900000,
			 0x41500000,
			 0x41d00000,
			 0x40800000,
			 0x41a80000,

			 /// vpu register f14
			 0x41d80000,
			 0x42000000,
			 0x41300000,
			 0x42000000,
			 0x40000000,
			 0x41100000,
			 0x41d00000,
			 0x41d00000,

			 /// vpu register f15
			 0x41d80000,
			 0x41a80000,
			 0x41300000,
			 0x41f00000,
			 0x41a00000,
			 0x41400000,
			 0x41900000,
			 0x41500000,

			 /// vpu register f16
			 0x42000000,
			 0x41600000,
			 0x40e00000,
			 0x41e00000,
			 0x41900000,
			 0x41200000,
			 0x41200000,
			 0x41880000,

			 /// vpu register f17
			 0x42000000,
			 0x41b00000,
			 0x41e80000,
			 0x41700000,
			 0x41c00000,
			 0x40a00000,
			 0x41d80000,
			 0x40000000,

			 /// vpu register f18
			 0x41d80000,
			 0x41a80000,
			 0x41200000,
			 0x41d80000,
			 0x41880000,
			 0x41b80000,
			 0x40400000,
			 0x41b80000,

			 /// vpu register f19
			 0x41980000,
			 0x41900000,
			 0x41400000,
			 0x41e00000,
			 0x41a80000,
			 0x41b00000,
			 0x41200000,
			 0x41500000,

			 /// vpu register f20
			 0x41300000,
			 0x41e00000,
			 0x41200000,
			 0x41500000,
			 0x41600000,
			 0x42000000,
			 0x40000000,
			 0x41900000,

			 /// vpu register f21
			 0x41b00000,
			 0x41100000,
			 0x41980000,
			 0x41900000,
			 0x41500000,
			 0x41980000,
			 0x41900000,
			 0x41f00000,

			 /// vpu register f22
			 0x41c80000,
			 0x41600000,
			 0x41200000,
			 0x40e00000,
			 0x41e00000,
			 0x41300000,
			 0x40800000,
			 0x41d80000,

			 /// vpu register f23
			 0x41c80000,
			 0x41e80000,
			 0x41c00000,
			 0x41f00000,
			 0x41800000,
			 0x41800000,
			 0x41980000,
			 0x41b00000,

			 /// vpu register f24
			 0x41f00000,
			 0x41600000,
			 0x41880000,
			 0x41800000,
			 0x40e00000,
			 0x40e00000,
			 0x41d00000,
			 0x40e00000,

			 /// vpu register f25
			 0x41b00000,
			 0x40e00000,
			 0x41400000,
			 0x41c80000,
			 0x41980000,
			 0x42000000,
			 0x40e00000,
			 0x41e80000,

			 /// vpu register f26
			 0x41b80000,
			 0x41d00000,
			 0x41f80000,
			 0x41b00000,
			 0x40800000,
			 0x41f00000,
			 0x41c80000,
			 0x41d80000,

			 /// vpu register f27
			 0x41b00000,
			 0x41f00000,
			 0x41300000,
			 0x41100000,
			 0x40e00000,
			 0x41600000,
			 0x41900000,
			 0x41c00000,

			 /// vpu register f28
			 0x41d80000,
			 0x41f00000,
			 0x41700000,
			 0x41800000,
			 0x41400000,
			 0x41980000,
			 0x41f00000,
			 0x41200000,

			 /// vpu register f29
			 0x42000000,
			 0x42000000,
			 0x40c00000,
			 0x41100000,
			 0x41900000,
			 0x41800000,
			 0x41d80000,
			 0x41500000,

			 /// vpu register f30
			 0x40800000,
			 0x41700000,
			 0x40000000,
			 0x41400000,
			 0x41000000,
			 0x41a80000,
			 0x41500000,
			 0x40000000,

			 /// vpu register f31
			 0x41500000,
			 0x41e80000,
			 0x41e00000,
			 0x41a00000,
			 0x41100000,
			 0x41a00000,
			 0x42000000,
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
		"cubefaceidx.ps f11, f12, f25\n"                      ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f2, f26, f30\n"                       ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f12, f26, f0\n"                       ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f18, f30, f25\n"                      ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f13, f9, f18\n"                       ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f16, f5, f0\n"                        ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f4, f13, f21\n"                       ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f21, f3, f2\n"                        ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f3, f7, f4\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f28, f27, f17\n"                      ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f28, f24, f23\n"                      ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f17, f15, f18\n"                      ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f4, f29, f2\n"                        ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f14, f16, f3\n"                       ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f22, f11, f2\n"                       ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f25, f8, f1\n"                        ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f13, f21, f25\n"                      ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f16, f5, f10\n"                       ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f14, f19, f30\n"                      ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f7, f27, f20\n"                       ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f4, f17, f27\n"                       ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f8, f13, f25\n"                       ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f27, f6, f29\n"                       ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f12, f17, f13\n"                      ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f16, f3, f11\n"                       ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f16, f10, f17\n"                      ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f4, f3, f1\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f16, f13, f2\n"                       ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f29, f7, f23\n"                       ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f2, f22, f7\n"                        ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f12, f20, f0\n"                       ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f6, f21, f1\n"                        ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f11, f20, f15\n"                      ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f16, f18, f29\n"                      ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f8, f1, f0\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f7, f11, f20\n"                       ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f8, f20, f9\n"                        ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f2, f24, f22\n"                       ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f27, f24, f1\n"                       ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f27, f1, f0\n"                        ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f6, f16, f3\n"                        ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f21, f19, f5\n"                       ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f28, f27, f9\n"                       ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f16, f20, f5\n"                       ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f9, f25, f7\n"                        ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f13, f29, f18\n"                      ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f21, f8, f4\n"                        ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f28, f1, f30\n"                       ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f7, f12, f13\n"                       ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f28, f19, f13\n"                      ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f3, f14, f27\n"                       ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f10, f4, f2\n"                        ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f11, f3, f15\n"                       ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f17, f7, f8\n"                        ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f22, f17, f23\n"                      ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f23, f13, f21\n"                      ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f22, f3, f6\n"                        ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f9, f12, f9\n"                        ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f2, f0, f21\n"                        ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f3, f25, f27\n"                       ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f8, f30, f2\n"                        ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f22, f27, f19\n"                      ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f5, f19, f24\n"                       ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f14, f11, f0\n"                       ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f3, f11, f21\n"                       ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f22, f0, f28\n"                       ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f13, f24, f7\n"                       ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f4, f3, f24\n"                        ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f28, f29, f7\n"                       ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f30, f19, f22\n"                      ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f15, f30, f29\n"                      ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f12, f22, f15\n"                      ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f28, f24, f29\n"                      ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f19, f1, f17\n"                       ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f23, f2, f5\n"                        ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f28, f7, f15\n"                       ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f21, f19, f5\n"                       ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f30, f23, f18\n"                      ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f9, f19, f21\n"                       ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f6, f16, f27\n"                       ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f29, f17, f25\n"                      ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f18, f27, f6\n"                       ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f12, f30, f24\n"                      ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f27, f4, f28\n"                       ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f24, f18, f1\n"                       ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f23, f5, f20\n"                       ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f21, f0, f8\n"                        ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f19, f22, f1\n"                       ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f28, f25, f3\n"                       ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f18, f25, f1\n"                       ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f6, f6, f0\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f6, f6, f30\n"                        ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f3, f10, f3\n"                        ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f7, f18, f29\n"                       ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f26, f0, f19\n"                       ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f14, f8, f16\n"                       ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f9, f13, f11\n"                       ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f28, f24, f29\n"                      ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"cubefaceidx.ps f2, f5, f17\n"                        ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
