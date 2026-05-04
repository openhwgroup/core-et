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
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00004
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x7fc00001,                                                  // signaling NaN                               /// 0000c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00010
			 0x0c400000,                                                  // Leading 1s:                                 /// 00014
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00018
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0001c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00020
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00024
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0002c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00030
			 0x00000000,                                                  // zero                                        /// 00034
			 0x0e000001,                                                  // Trailing 1s:                                /// 00038
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00040
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00044
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0004c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00050
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00058
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0005c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00064
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00068
			 0xff800000,                                                  // -inf                                        /// 0006c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x0c600000,                                                  // Leading 1s:                                 /// 00074
			 0x0c400000,                                                  // Leading 1s:                                 /// 00078
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0007c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00084
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00088
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0008c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00090
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00098
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 000b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x33333333,                                                  // 4 random values                             /// 000c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00100
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0010c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00110
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00114
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00118
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0011c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0012c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00134
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00138
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0013c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00144
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00148
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00150
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00158
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00160
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00164
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00168
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0016c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00170
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00178
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0017c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00188
			 0xff800000,                                                  // -inf                                        /// 0018c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00190
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00194
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00198
			 0x7fc00001,                                                  // signaling NaN                               /// 0019c
			 0x80011111,                                                  // -9.7958E-41                                 /// 001a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x33333333,                                                  // 4 random values                             /// 001b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001bc
			 0x7f800000,                                                  // inf                                         /// 001c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 001d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00200
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0020c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00210
			 0x80000000,                                                  // -zero                                       /// 00214
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00220
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00224
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0022c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00230
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00234
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00238
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00244
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0024c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00250
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00254
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0025c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00260
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00264
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00268
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0026c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00270
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00280
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00284
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00294
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00298
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0029c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002e0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 002e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00300
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00304
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00308
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00310
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00320
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00328
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00334
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00338
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0033c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00344
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00348
			 0x55555555,                                                  // 4 random values                             /// 0034c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00350
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0035c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00360
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00364
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00368
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00370
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00378
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0037c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00380
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00384
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0038c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00390
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00394
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00398
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 003c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00404
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00410
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00414
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00420
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0042c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00430
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00434
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00444
			 0x0e000003,                                                  // Trailing 1s:                                /// 00448
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0044c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00450
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00458
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00460
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00464
			 0x80000000,                                                  // -zero                                       /// 00468
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0046c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00478
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0047c
			 0x3f028f5c,                                                  // 0.51                                        /// 00480
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00484
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0048c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00490
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00494
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00498
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004a4
			 0x33333333,                                                  // 4 random values                             /// 004a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004d4
			 0xcb000000,                                                  // -8388608.0                                  /// 004d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004dc
			 0x33333333,                                                  // 4 random values                             /// 004e0
			 0x4b000000,                                                  // 8388608.0                                   /// 004e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00500
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00504
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00508
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00510
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00518
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00520
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00528
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0052c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00530
			 0xcb000000,                                                  // -8388608.0                                  /// 00534
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00538
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0053c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00540
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00544
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0054c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x55555555,                                                  // 4 random values                             /// 00554
			 0x4b000000,                                                  // 8388608.0                                   /// 00558
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00560
			 0x0c400000,                                                  // Leading 1s:                                 /// 00564
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00568
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00570
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00574
			 0x7fc00001,                                                  // signaling NaN                               /// 00578
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00580
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00584
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00588
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0058c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00590
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00594
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00598
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005c0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 005d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005ec
			 0x80000000,                                                  // -zero                                       /// 005f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00600
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00604
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0060c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00614
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00618
			 0xff800000,                                                  // -inf                                        /// 0061c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00624
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00628
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0062c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00638
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0063c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00640
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00650
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00660
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00664
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x3f028f5c,                                                  // 0.51                                        /// 0066c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00670
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00674
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00678
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00680
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00684
			 0x3f028f5c,                                                  // 0.51                                        /// 00688
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0068c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00690
			 0x80011111,                                                  // -9.7958E-41                                 /// 00694
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006ac
			 0x7f800000,                                                  // inf                                         /// 006b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00700
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00704
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00708
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00710
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00718
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00724
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0x7fc00001,                                                  // signaling NaN                               /// 0072c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00734
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00738
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0073c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00740
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00744
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0xcb000000,                                                  // -8388608.0                                  /// 0074c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00750
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00770
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00774
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00778
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00780
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00784
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00788
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0078c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00790
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00794
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 007c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 007c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007e4
			 0x7fc00001,                                                  // signaling NaN                               /// 007e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00808
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00810
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x0e000001,                                                  // Trailing 1s:                                /// 00818
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0x0e000003,                                                  // Trailing 1s:                                /// 00828
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00834
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0083c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00840
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00848
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00850
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x00011111,                                                  // 9.7958E-41                                  /// 00858
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00864
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00868
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0086c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00870
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00878
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0087c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00880
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00884
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00888
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0088c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x7f800000,                                                  // inf                                         /// 00894
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00898
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x7fc00001,                                                  // signaling NaN                               /// 008d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008ec
			 0x80000000,                                                  // -zero                                       /// 008f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0xcb000000,                                                  // -8388608.0                                  /// 008fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00900
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00904
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00908
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0090c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00918
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0091c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00920
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00924
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0092c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00930
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00934
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00938
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0093c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00940
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00944
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0094c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x7fc00001,                                                  // signaling NaN                               /// 00958
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00960
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00964
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00968
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00970
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00974
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0097c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00988
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00990
			 0xffc00001,                                                  // -signaling NaN                              /// 00994
			 0xffc00001,                                                  // -signaling NaN                              /// 00998
			 0x0e000007,                                                  // Trailing 1s:                                /// 0099c
			 0xbf028f5c,                                                  // -0.51                                       /// 009a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009bc
			 0xcb000000,                                                  // -8388608.0                                  /// 009c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 009d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0xffc00001,                                                  // -signaling NaN                              /// 00a04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a14
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a24
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a40
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a48
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a54
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a60
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a68
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a6c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a74
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a78
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a8c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a90
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a98
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00aa0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00aa8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00aac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00000000,                                                  // zero                                        /// 00acc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ad0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ad4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ad8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ae8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00aec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00afc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b00
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b08
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b2c
			 0x80000000,                                                  // -zero                                       /// 00b30
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b3c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b44
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b4c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b58
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b5c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b78
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0x7f800000,                                                  // inf                                         /// 00b88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b8c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ba0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ba4
			 0x7f800000,                                                  // inf                                         /// 00ba8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bb0
			 0xbf028f5c,                                                  // -0.51                                       /// 00bb4
			 0xff800000,                                                  // -inf                                        /// 00bb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bbc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bc0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bc4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bcc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bd4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00be0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00be4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00be8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bf0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bfc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c00
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c04
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c08
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c1c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c24
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c34
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c38
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c40
			 0x00000000,                                                  // zero                                        /// 00c44
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c58
			 0x7fc00001,                                                  // signaling NaN                               /// 00c5c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c64
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c7c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x3f028f5c,                                                  // 0.51                                        /// 00c84
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c88
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c8c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c90
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cb0
			 0xff800000,                                                  // -inf                                        /// 00cb4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cb8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x55555555,                                                  // 4 random values                             /// 00cc4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cc8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ccc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cd4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cd8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ce0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ce4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ce8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cf0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cf8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cfc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d00
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d04
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d08
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d10
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80000000,                                                  // -zero                                       /// 00d18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0xbf028f5c,                                                  // -0.51                                       /// 00d24
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d3c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d40
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d50
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d64
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d68
			 0xff800000,                                                  // -inf                                        /// 00d6c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d84
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00da0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00da4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00da8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00db0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00db8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dc0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dc8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dd0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dd4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00de8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00df4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00df8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dfc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e10
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e14
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e18
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e1c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e24
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e40
			 0x3f028f5c,                                                  // 0.51                                        /// 00e44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e64
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e6c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x80000000,                                                  // -zero                                       /// 00e78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e7c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e84
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e98
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ea0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ea8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00eac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00eb0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ebc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ec0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ec4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ecc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ed4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ed8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ee0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0xcb000000,                                                  // -8388608.0                                  /// 00ef0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00efc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f04
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f08
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f10
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f14
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f18
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f1c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f30
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f34
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f3c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f40
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f44
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0xbf028f5c,                                                  // -0.51                                       /// 00f4c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f50
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f68
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f78
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f7c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f80
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f8c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f98
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fa0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fb0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fb4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fb8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fbc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fc4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fcc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fd4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fdc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fe0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fe4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ff8
			 0x0e00001f                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x564aeb5a, /// 0x0
			 0xd71ae03b, /// 0x4
			 0x19f2504a, /// 0x8
			 0xfc1565fa, /// 0xc
			 0x07c06bf6, /// 0x10
			 0xdf81b960, /// 0x14
			 0x2a214f2a, /// 0x18
			 0xec50764c, /// 0x1c
			 0x4e733b3d, /// 0x20
			 0x57a7a93c, /// 0x24
			 0x0036af25, /// 0x28
			 0x49ad2230, /// 0x2c
			 0x6cbc5333, /// 0x30
			 0x13c4f213, /// 0x34
			 0x70513e14, /// 0x38
			 0xf6ca08a7, /// 0x3c
			 0x1a8d05a0, /// 0x40
			 0xd759803a, /// 0x44
			 0x2be1a2d6, /// 0x48
			 0x764f092a, /// 0x4c
			 0xc1192468, /// 0x50
			 0x7993ceec, /// 0x54
			 0x76075e89, /// 0x58
			 0x3ebf5ff8, /// 0x5c
			 0x1377e741, /// 0x60
			 0x73b69b0a, /// 0x64
			 0xf6566bf5, /// 0x68
			 0x4f279a36, /// 0x6c
			 0x3ec50fa8, /// 0x70
			 0x0a6386ea, /// 0x74
			 0x40bf60c5, /// 0x78
			 0xd5c96a5a, /// 0x7c
			 0x01c27c15, /// 0x80
			 0xd125a991, /// 0x84
			 0x103510cc, /// 0x88
			 0x6641de68, /// 0x8c
			 0x740784f6, /// 0x90
			 0x84ce2f4c, /// 0x94
			 0x86f78129, /// 0x98
			 0x189a0a0d, /// 0x9c
			 0xff699940, /// 0xa0
			 0x3ea215fb, /// 0xa4
			 0x561ed8dd, /// 0xa8
			 0x2c56f22d, /// 0xac
			 0x9f179db7, /// 0xb0
			 0xe27f6e47, /// 0xb4
			 0x97895af8, /// 0xb8
			 0xd3d8de82, /// 0xbc
			 0x33ae7bec, /// 0xc0
			 0xf803fd8f, /// 0xc4
			 0x565e742d, /// 0xc8
			 0xf94db478, /// 0xcc
			 0x91f3b9cc, /// 0xd0
			 0x8339c2a6, /// 0xd4
			 0x7826bcdf, /// 0xd8
			 0x87272b86, /// 0xdc
			 0x6acae093, /// 0xe0
			 0x4dc14eb3, /// 0xe4
			 0x8936a451, /// 0xe8
			 0xa08d363b, /// 0xec
			 0x8fca2032, /// 0xf0
			 0x9cd191e1, /// 0xf4
			 0x61bfc955, /// 0xf8
			 0x097419a5, /// 0xfc
			 0xe6e8ba5b, /// 0x100
			 0x7efd92a0, /// 0x104
			 0x209fd895, /// 0x108
			 0xee6cfee7, /// 0x10c
			 0x34d32413, /// 0x110
			 0x72b10a04, /// 0x114
			 0x946074f4, /// 0x118
			 0xe7c4e0a7, /// 0x11c
			 0x3621f495, /// 0x120
			 0x70b2f893, /// 0x124
			 0x1388ba2b, /// 0x128
			 0xc958cdd1, /// 0x12c
			 0x50d2df3b, /// 0x130
			 0x49009948, /// 0x134
			 0xdda7e070, /// 0x138
			 0xf89291b0, /// 0x13c
			 0x8a2aa4d1, /// 0x140
			 0x8862cf54, /// 0x144
			 0xeef2c7f8, /// 0x148
			 0x4aa224d0, /// 0x14c
			 0x8d99ae32, /// 0x150
			 0x2d88f040, /// 0x154
			 0x8a9eea74, /// 0x158
			 0x760b096c, /// 0x15c
			 0x94e9220a, /// 0x160
			 0xa968c351, /// 0x164
			 0x212aaacd, /// 0x168
			 0xd8283565, /// 0x16c
			 0x55048217, /// 0x170
			 0x85a91c4d, /// 0x174
			 0x9d87c233, /// 0x178
			 0x8505626e, /// 0x17c
			 0xd282dbe6, /// 0x180
			 0x75c84d39, /// 0x184
			 0x072b805f, /// 0x188
			 0x73f1eab4, /// 0x18c
			 0x7ef03f2c, /// 0x190
			 0x0a5d66c2, /// 0x194
			 0x17f8ccda, /// 0x198
			 0xc11ab610, /// 0x19c
			 0x9342926a, /// 0x1a0
			 0x075df528, /// 0x1a4
			 0x8312413c, /// 0x1a8
			 0x443f38b9, /// 0x1ac
			 0xba82b5fa, /// 0x1b0
			 0x0e3f9a80, /// 0x1b4
			 0xb570d8f7, /// 0x1b8
			 0x08d45924, /// 0x1bc
			 0x43b5ab87, /// 0x1c0
			 0x7f2c975a, /// 0x1c4
			 0x80d6abb7, /// 0x1c8
			 0x56a76e79, /// 0x1cc
			 0x6a9a80d6, /// 0x1d0
			 0xd0ed3107, /// 0x1d4
			 0xdc39c969, /// 0x1d8
			 0x212c51df, /// 0x1dc
			 0x4dfa304d, /// 0x1e0
			 0xecb5a6bc, /// 0x1e4
			 0x7f5677f9, /// 0x1e8
			 0x111510cb, /// 0x1ec
			 0x7c0459e0, /// 0x1f0
			 0x3717de0a, /// 0x1f4
			 0x34d4dbe3, /// 0x1f8
			 0x63568e0c, /// 0x1fc
			 0xb8791471, /// 0x200
			 0x27b4acdd, /// 0x204
			 0xadcc2c66, /// 0x208
			 0xe36b335f, /// 0x20c
			 0xf8e6782e, /// 0x210
			 0xe62c92b2, /// 0x214
			 0x6162312f, /// 0x218
			 0xba745f7e, /// 0x21c
			 0x4f573fa2, /// 0x220
			 0x52fc8493, /// 0x224
			 0xc71a9689, /// 0x228
			 0x994e44de, /// 0x22c
			 0xadc961ef, /// 0x230
			 0x22b32be6, /// 0x234
			 0xfda2fce8, /// 0x238
			 0xea48e9d8, /// 0x23c
			 0x43be34d6, /// 0x240
			 0x32041b07, /// 0x244
			 0x9d962c75, /// 0x248
			 0x9e002553, /// 0x24c
			 0x20369bab, /// 0x250
			 0xb0cbc94b, /// 0x254
			 0xf7f518a3, /// 0x258
			 0xeeeef61b, /// 0x25c
			 0x93ad2773, /// 0x260
			 0xdf2c60f2, /// 0x264
			 0xfb7c70c4, /// 0x268
			 0xafd062e2, /// 0x26c
			 0xbb9f890a, /// 0x270
			 0xcaa103ed, /// 0x274
			 0xeb9b07d9, /// 0x278
			 0x71314cc8, /// 0x27c
			 0x7a2f9f21, /// 0x280
			 0x258e27f7, /// 0x284
			 0xe1e09605, /// 0x288
			 0xeda964e0, /// 0x28c
			 0xe83aee27, /// 0x290
			 0xab6dbedd, /// 0x294
			 0x96755265, /// 0x298
			 0x34f7baa3, /// 0x29c
			 0x276b42c1, /// 0x2a0
			 0x6cb4ec22, /// 0x2a4
			 0x0b184d3a, /// 0x2a8
			 0x35349822, /// 0x2ac
			 0xfda05ded, /// 0x2b0
			 0xe673a037, /// 0x2b4
			 0xd9c85df9, /// 0x2b8
			 0xdb241ee3, /// 0x2bc
			 0x75651c93, /// 0x2c0
			 0x1a6bb056, /// 0x2c4
			 0xb637d1c0, /// 0x2c8
			 0x00e3f8fc, /// 0x2cc
			 0xaef98f3a, /// 0x2d0
			 0x7750eda7, /// 0x2d4
			 0x140b023c, /// 0x2d8
			 0x9dd1eed7, /// 0x2dc
			 0x4f9b4c2d, /// 0x2e0
			 0x359fbef6, /// 0x2e4
			 0x50f18ec4, /// 0x2e8
			 0x56ca1f2e, /// 0x2ec
			 0x9a67cc7a, /// 0x2f0
			 0x7c1292e1, /// 0x2f4
			 0x2d444e72, /// 0x2f8
			 0xb692af7b, /// 0x2fc
			 0xda66ac03, /// 0x300
			 0x494a9aca, /// 0x304
			 0x9bcc8444, /// 0x308
			 0xb791e5de, /// 0x30c
			 0x6e9c1060, /// 0x310
			 0x21ee0ff1, /// 0x314
			 0xa999a4b7, /// 0x318
			 0x114841d9, /// 0x31c
			 0x071ddf62, /// 0x320
			 0xa922b7f9, /// 0x324
			 0x6440c13e, /// 0x328
			 0x9f891452, /// 0x32c
			 0xe7e7e44f, /// 0x330
			 0xeee8ceac, /// 0x334
			 0x45a7fed6, /// 0x338
			 0x77894955, /// 0x33c
			 0x164aae46, /// 0x340
			 0x3d044499, /// 0x344
			 0x6edcb7ac, /// 0x348
			 0x6a51856c, /// 0x34c
			 0x17425884, /// 0x350
			 0x59f0af25, /// 0x354
			 0x8afa491e, /// 0x358
			 0x8b077d44, /// 0x35c
			 0x055724ff, /// 0x360
			 0x74b85c24, /// 0x364
			 0x2b9ed2bb, /// 0x368
			 0xdb63418d, /// 0x36c
			 0x90d988f3, /// 0x370
			 0x3946d112, /// 0x374
			 0xf48300c0, /// 0x378
			 0x0da0e7cd, /// 0x37c
			 0x7faac12a, /// 0x380
			 0xebce1370, /// 0x384
			 0xc74d714e, /// 0x388
			 0x4cbee342, /// 0x38c
			 0xcd94820e, /// 0x390
			 0x509a2813, /// 0x394
			 0xd0f28b13, /// 0x398
			 0x70274c37, /// 0x39c
			 0x197016f6, /// 0x3a0
			 0xfaf4acb1, /// 0x3a4
			 0x9e5ad794, /// 0x3a8
			 0x20de2ff7, /// 0x3ac
			 0x237181d5, /// 0x3b0
			 0xf12634f2, /// 0x3b4
			 0xf9dc8221, /// 0x3b8
			 0x6fc6b15f, /// 0x3bc
			 0xf1e11ff8, /// 0x3c0
			 0xf6d9e093, /// 0x3c4
			 0xabde591e, /// 0x3c8
			 0x7fa1e2e5, /// 0x3cc
			 0x7301b2ef, /// 0x3d0
			 0x4b2c9490, /// 0x3d4
			 0xf05adefc, /// 0x3d8
			 0xbb81a563, /// 0x3dc
			 0xda1e1916, /// 0x3e0
			 0x09b9a0cc, /// 0x3e4
			 0x799762bd, /// 0x3e8
			 0x2c6ca12c, /// 0x3ec
			 0x6eb17373, /// 0x3f0
			 0xb44bb6ba, /// 0x3f4
			 0x3d4a6f46, /// 0x3f8
			 0xe159d9f7, /// 0x3fc
			 0xcf23fae9, /// 0x400
			 0xb35943a2, /// 0x404
			 0x973fc1e7, /// 0x408
			 0x3d3f5080, /// 0x40c
			 0x9c30097d, /// 0x410
			 0x629adb3d, /// 0x414
			 0x5ac8a4a8, /// 0x418
			 0x3a374be7, /// 0x41c
			 0x8c2fa361, /// 0x420
			 0x3b62fcab, /// 0x424
			 0x7f707cb8, /// 0x428
			 0x74aa672d, /// 0x42c
			 0xb1d8e1f4, /// 0x430
			 0xd2f1d9d7, /// 0x434
			 0xfb0505b9, /// 0x438
			 0x86abdb30, /// 0x43c
			 0x0e1b231d, /// 0x440
			 0xf892a8a7, /// 0x444
			 0x191a66f8, /// 0x448
			 0x03557d56, /// 0x44c
			 0xd7617600, /// 0x450
			 0xeb8c05d8, /// 0x454
			 0xcf6c07cc, /// 0x458
			 0x096f7156, /// 0x45c
			 0xde97c029, /// 0x460
			 0x6c93e7c5, /// 0x464
			 0x174b04ef, /// 0x468
			 0x544ece05, /// 0x46c
			 0xf796dbae, /// 0x470
			 0xd9114fba, /// 0x474
			 0xf5088885, /// 0x478
			 0xf9efcb67, /// 0x47c
			 0x8e374cb9, /// 0x480
			 0x70811b53, /// 0x484
			 0x3a5ee6c2, /// 0x488
			 0xe0605be5, /// 0x48c
			 0x0c889e6e, /// 0x490
			 0x6559c282, /// 0x494
			 0xe02936f2, /// 0x498
			 0xd52f35db, /// 0x49c
			 0x25eaf2cc, /// 0x4a0
			 0xe4d62358, /// 0x4a4
			 0xfabe344f, /// 0x4a8
			 0x94c7beba, /// 0x4ac
			 0xb5c2d276, /// 0x4b0
			 0x96f12c2d, /// 0x4b4
			 0x9362b7c6, /// 0x4b8
			 0x39a77348, /// 0x4bc
			 0xeee369fe, /// 0x4c0
			 0xc231729d, /// 0x4c4
			 0x54e87f15, /// 0x4c8
			 0x3d1d4604, /// 0x4cc
			 0xeac72f32, /// 0x4d0
			 0xedc844c6, /// 0x4d4
			 0x24b1be71, /// 0x4d8
			 0xf1006dd0, /// 0x4dc
			 0xd6948c61, /// 0x4e0
			 0xaaad4cfd, /// 0x4e4
			 0xd9f87876, /// 0x4e8
			 0xd79dc466, /// 0x4ec
			 0x7f10726e, /// 0x4f0
			 0x867a8507, /// 0x4f4
			 0xb431f967, /// 0x4f8
			 0x33fdb59a, /// 0x4fc
			 0x2545473b, /// 0x500
			 0xdfbcabb5, /// 0x504
			 0x561c6a48, /// 0x508
			 0x7a004a00, /// 0x50c
			 0xc72b53eb, /// 0x510
			 0x1afa2784, /// 0x514
			 0x4f0fc6bd, /// 0x518
			 0x7fc2ab37, /// 0x51c
			 0x9ef29cb3, /// 0x520
			 0x519ec2e7, /// 0x524
			 0x1a4ebd91, /// 0x528
			 0x89fbc0fe, /// 0x52c
			 0xd4f0041d, /// 0x530
			 0x51a270c6, /// 0x534
			 0xf9c5ef08, /// 0x538
			 0x91fe7478, /// 0x53c
			 0xac87978f, /// 0x540
			 0x28d1b133, /// 0x544
			 0xeaf4da9a, /// 0x548
			 0x6d31952a, /// 0x54c
			 0xf28065e2, /// 0x550
			 0x1b47a17f, /// 0x554
			 0x4bf14d16, /// 0x558
			 0x2dde5342, /// 0x55c
			 0x1b5c5a26, /// 0x560
			 0xd836c916, /// 0x564
			 0x237b4bd1, /// 0x568
			 0x995c3205, /// 0x56c
			 0x5006a81a, /// 0x570
			 0x590b12e2, /// 0x574
			 0x9fa892e7, /// 0x578
			 0xa6dde1bf, /// 0x57c
			 0xe20b285f, /// 0x580
			 0x0a53b9ff, /// 0x584
			 0xc03c9543, /// 0x588
			 0xb125cb49, /// 0x58c
			 0x169af4f1, /// 0x590
			 0xf540dcc6, /// 0x594
			 0x91e7fbe6, /// 0x598
			 0x626770c5, /// 0x59c
			 0x506b7d00, /// 0x5a0
			 0xb8b64484, /// 0x5a4
			 0x55120ee6, /// 0x5a8
			 0xaaa79e2c, /// 0x5ac
			 0x5529684f, /// 0x5b0
			 0x8d3ae4b4, /// 0x5b4
			 0xf695d71b, /// 0x5b8
			 0xa5ed30d9, /// 0x5bc
			 0xa65b8944, /// 0x5c0
			 0xe59d02a3, /// 0x5c4
			 0xc21e8b39, /// 0x5c8
			 0xa917d8d0, /// 0x5cc
			 0x768ee6da, /// 0x5d0
			 0x4c4e0bda, /// 0x5d4
			 0xe4548da1, /// 0x5d8
			 0xeaa9b97c, /// 0x5dc
			 0xb38f34e7, /// 0x5e0
			 0x86a537ac, /// 0x5e4
			 0x7153686e, /// 0x5e8
			 0x0a048463, /// 0x5ec
			 0x183a5296, /// 0x5f0
			 0xe284a8aa, /// 0x5f4
			 0xff223a39, /// 0x5f8
			 0xaf42fea1, /// 0x5fc
			 0xa5a89dcc, /// 0x600
			 0xfa55ec4f, /// 0x604
			 0x405ef117, /// 0x608
			 0x638abb98, /// 0x60c
			 0xba666a28, /// 0x610
			 0x1383d8ee, /// 0x614
			 0x0bc3204d, /// 0x618
			 0x6f881d1b, /// 0x61c
			 0xd5516394, /// 0x620
			 0x5c021536, /// 0x624
			 0x1881ec92, /// 0x628
			 0x48658ab8, /// 0x62c
			 0x7216fdd2, /// 0x630
			 0x94b943a1, /// 0x634
			 0x9dd59bdc, /// 0x638
			 0x49bd7d28, /// 0x63c
			 0x601c43df, /// 0x640
			 0x78e3d2d6, /// 0x644
			 0x1be34a6d, /// 0x648
			 0x6da7ec37, /// 0x64c
			 0xeb92dd4d, /// 0x650
			 0x39a4c81d, /// 0x654
			 0x06a39845, /// 0x658
			 0x39491cc5, /// 0x65c
			 0x52260b7d, /// 0x660
			 0x5f59a7c6, /// 0x664
			 0xca894166, /// 0x668
			 0x8442417c, /// 0x66c
			 0xd141ee45, /// 0x670
			 0xd6d70e04, /// 0x674
			 0x15667996, /// 0x678
			 0xde29f0ed, /// 0x67c
			 0x0174f579, /// 0x680
			 0x990d79b3, /// 0x684
			 0x9a4ffb25, /// 0x688
			 0x5578ac8f, /// 0x68c
			 0xb4a7c4db, /// 0x690
			 0xa637022b, /// 0x694
			 0x35046840, /// 0x698
			 0x69af93e2, /// 0x69c
			 0x8aa3748c, /// 0x6a0
			 0xd472ab27, /// 0x6a4
			 0x19c46239, /// 0x6a8
			 0x92210f23, /// 0x6ac
			 0x0e7d92e4, /// 0x6b0
			 0x2c45b635, /// 0x6b4
			 0x5a3a49b9, /// 0x6b8
			 0xc9d8e183, /// 0x6bc
			 0xd3a13b1b, /// 0x6c0
			 0xaebc37a7, /// 0x6c4
			 0x185110a9, /// 0x6c8
			 0xa7ff7798, /// 0x6cc
			 0x40b898f6, /// 0x6d0
			 0x8e0a37ad, /// 0x6d4
			 0x4c65a1d5, /// 0x6d8
			 0xf004ab49, /// 0x6dc
			 0x147b0698, /// 0x6e0
			 0x2c5bcf4c, /// 0x6e4
			 0xa5229b10, /// 0x6e8
			 0x5830b939, /// 0x6ec
			 0x1727be01, /// 0x6f0
			 0x97ed97c5, /// 0x6f4
			 0x5d876984, /// 0x6f8
			 0x9a1db961, /// 0x6fc
			 0x31670800, /// 0x700
			 0xf11ad939, /// 0x704
			 0xb053cda0, /// 0x708
			 0x9ea86c0f, /// 0x70c
			 0x86bb4953, /// 0x710
			 0x117fbff7, /// 0x714
			 0xf485dadf, /// 0x718
			 0x6b649c04, /// 0x71c
			 0xbb9e76c2, /// 0x720
			 0xb95a5e50, /// 0x724
			 0x5b6f5da9, /// 0x728
			 0x52994c47, /// 0x72c
			 0xf97af693, /// 0x730
			 0x09211c1a, /// 0x734
			 0x338ac3de, /// 0x738
			 0xb3a9ca29, /// 0x73c
			 0xd65c5fbd, /// 0x740
			 0x5a569031, /// 0x744
			 0xb17a2852, /// 0x748
			 0xc28d5215, /// 0x74c
			 0xb640eb23, /// 0x750
			 0x601b5249, /// 0x754
			 0xc192ec54, /// 0x758
			 0x2237ab60, /// 0x75c
			 0x8450a86e, /// 0x760
			 0x582a291b, /// 0x764
			 0x2f15a942, /// 0x768
			 0xc235c148, /// 0x76c
			 0xe2efeaa5, /// 0x770
			 0x41f183e8, /// 0x774
			 0x7d277b50, /// 0x778
			 0x3c0a8e22, /// 0x77c
			 0x4a90f25a, /// 0x780
			 0x81ee14cf, /// 0x784
			 0x808c72ce, /// 0x788
			 0x0dd22230, /// 0x78c
			 0x721fa7ca, /// 0x790
			 0xa2989b0a, /// 0x794
			 0xfbd1580b, /// 0x798
			 0xd8094469, /// 0x79c
			 0xe30b4983, /// 0x7a0
			 0xcc15dbe8, /// 0x7a4
			 0xc145f23e, /// 0x7a8
			 0x50a758e3, /// 0x7ac
			 0x330b9f71, /// 0x7b0
			 0x5427edd6, /// 0x7b4
			 0xa13df323, /// 0x7b8
			 0x6f6eafb9, /// 0x7bc
			 0xdf1d73ce, /// 0x7c0
			 0xb2373d09, /// 0x7c4
			 0xd09fefdc, /// 0x7c8
			 0xf5f0d9ad, /// 0x7cc
			 0x40585647, /// 0x7d0
			 0x3da787e2, /// 0x7d4
			 0x0ef041a8, /// 0x7d8
			 0x791c633c, /// 0x7dc
			 0xea15258a, /// 0x7e0
			 0xcd576524, /// 0x7e4
			 0x9dbe8d5c, /// 0x7e8
			 0x5fb69af0, /// 0x7ec
			 0x8dcc44a8, /// 0x7f0
			 0x69c832e1, /// 0x7f4
			 0xf0ad9e23, /// 0x7f8
			 0x0923c4c3, /// 0x7fc
			 0xd5222709, /// 0x800
			 0xce05e135, /// 0x804
			 0x84e10600, /// 0x808
			 0x5bcd569f, /// 0x80c
			 0x2f71c8d3, /// 0x810
			 0x753bf0af, /// 0x814
			 0x757ad545, /// 0x818
			 0xd78f5bb8, /// 0x81c
			 0xa69d9a09, /// 0x820
			 0x08651c9c, /// 0x824
			 0xce85663b, /// 0x828
			 0xebdb42cc, /// 0x82c
			 0xd99bee4a, /// 0x830
			 0x21bc16d3, /// 0x834
			 0xba08ecc1, /// 0x838
			 0xb9c1f354, /// 0x83c
			 0xb00a9f62, /// 0x840
			 0xe944f39f, /// 0x844
			 0xe5a007cb, /// 0x848
			 0x7b896a49, /// 0x84c
			 0xc853422b, /// 0x850
			 0x3dae4bc9, /// 0x854
			 0x150473f7, /// 0x858
			 0xca67dc92, /// 0x85c
			 0x3a4c9191, /// 0x860
			 0x3c8e4cb8, /// 0x864
			 0x4c3127c6, /// 0x868
			 0x59abb90d, /// 0x86c
			 0x4f5dd98a, /// 0x870
			 0xa18f2eeb, /// 0x874
			 0xc59048e5, /// 0x878
			 0x67fb44ab, /// 0x87c
			 0xdb531dc8, /// 0x880
			 0x26c926a7, /// 0x884
			 0x2cd15c3a, /// 0x888
			 0xd274d49c, /// 0x88c
			 0xe4d93ca9, /// 0x890
			 0x1af1f288, /// 0x894
			 0x080976c2, /// 0x898
			 0xa8c5cbb5, /// 0x89c
			 0x135e8747, /// 0x8a0
			 0x325bf1d2, /// 0x8a4
			 0xbc82b0c5, /// 0x8a8
			 0xf70b9856, /// 0x8ac
			 0x77781871, /// 0x8b0
			 0x0673cbde, /// 0x8b4
			 0x317cc367, /// 0x8b8
			 0x0332d1a7, /// 0x8bc
			 0xd7fc33b6, /// 0x8c0
			 0x9f94a3c5, /// 0x8c4
			 0xa5bd2748, /// 0x8c8
			 0xfb7cb9e9, /// 0x8cc
			 0x7fd88b07, /// 0x8d0
			 0x99e712a3, /// 0x8d4
			 0x8c254a13, /// 0x8d8
			 0xac4022d5, /// 0x8dc
			 0x1fb63633, /// 0x8e0
			 0xbba1e5b3, /// 0x8e4
			 0x234253d9, /// 0x8e8
			 0x57d93260, /// 0x8ec
			 0x7d04578d, /// 0x8f0
			 0x3f69d2fc, /// 0x8f4
			 0xf6da53ca, /// 0x8f8
			 0x3ba9105e, /// 0x8fc
			 0x9a1d383c, /// 0x900
			 0x479d5d05, /// 0x904
			 0x97cd159e, /// 0x908
			 0x3b0d5cca, /// 0x90c
			 0x1a996472, /// 0x910
			 0xeb72a469, /// 0x914
			 0xa209a136, /// 0x918
			 0x933f45db, /// 0x91c
			 0x5c1d55b1, /// 0x920
			 0x77c372de, /// 0x924
			 0x08f0658b, /// 0x928
			 0xe2c53f7e, /// 0x92c
			 0xb64a50fc, /// 0x930
			 0xfe3e5797, /// 0x934
			 0x0e6f89b6, /// 0x938
			 0x53cae0a5, /// 0x93c
			 0xe1fa6be6, /// 0x940
			 0x493883bd, /// 0x944
			 0x8417a4f1, /// 0x948
			 0x011e3c54, /// 0x94c
			 0xb7a4b412, /// 0x950
			 0xe216486f, /// 0x954
			 0x68c1b393, /// 0x958
			 0xbe1a6ba0, /// 0x95c
			 0xd74d9225, /// 0x960
			 0x1743681b, /// 0x964
			 0xfc3a489e, /// 0x968
			 0xbb0f8756, /// 0x96c
			 0xc4cd47e2, /// 0x970
			 0x59ce8c25, /// 0x974
			 0x7b5da186, /// 0x978
			 0xa22b50ad, /// 0x97c
			 0x693f73f0, /// 0x980
			 0x22cf22f7, /// 0x984
			 0xd9be0ed8, /// 0x988
			 0xd0610458, /// 0x98c
			 0xa57d5195, /// 0x990
			 0x41303b22, /// 0x994
			 0x0e73e305, /// 0x998
			 0xac4afd66, /// 0x99c
			 0xa906c1c0, /// 0x9a0
			 0xad34111a, /// 0x9a4
			 0x4d512e7b, /// 0x9a8
			 0xc324e77c, /// 0x9ac
			 0xe79a689d, /// 0x9b0
			 0xec0ab119, /// 0x9b4
			 0x2d91b4c7, /// 0x9b8
			 0xbe03d30e, /// 0x9bc
			 0xc1579aa9, /// 0x9c0
			 0x0aabc429, /// 0x9c4
			 0x2b6c6389, /// 0x9c8
			 0x21c33afa, /// 0x9cc
			 0x60d88c48, /// 0x9d0
			 0xbc502654, /// 0x9d4
			 0x5905cec9, /// 0x9d8
			 0x672526ea, /// 0x9dc
			 0xaeabcecf, /// 0x9e0
			 0x788368af, /// 0x9e4
			 0xb5f55b89, /// 0x9e8
			 0x0587bfa6, /// 0x9ec
			 0x42a49a82, /// 0x9f0
			 0x3bf792e0, /// 0x9f4
			 0x564353be, /// 0x9f8
			 0xe4788612, /// 0x9fc
			 0xa875ee69, /// 0xa00
			 0xc434d4a8, /// 0xa04
			 0x6f1a86ab, /// 0xa08
			 0xa29d762e, /// 0xa0c
			 0x0a53aeae, /// 0xa10
			 0x0a6d9795, /// 0xa14
			 0x855c3a09, /// 0xa18
			 0xa38855bb, /// 0xa1c
			 0xe38803a3, /// 0xa20
			 0x09d2d40a, /// 0xa24
			 0x07ff5aab, /// 0xa28
			 0x1a12ec6a, /// 0xa2c
			 0x09608600, /// 0xa30
			 0x68420843, /// 0xa34
			 0x33c1bfaa, /// 0xa38
			 0xf716a0ce, /// 0xa3c
			 0xfcf8ff01, /// 0xa40
			 0x7d8af78a, /// 0xa44
			 0x82bd4408, /// 0xa48
			 0x54d23fbb, /// 0xa4c
			 0x1b2a2737, /// 0xa50
			 0x7948a4fd, /// 0xa54
			 0xdd8468e7, /// 0xa58
			 0x1681b325, /// 0xa5c
			 0x8afb02c1, /// 0xa60
			 0x024e1629, /// 0xa64
			 0x27d47e0f, /// 0xa68
			 0x1df618a0, /// 0xa6c
			 0x03c6bf20, /// 0xa70
			 0xceff934f, /// 0xa74
			 0xd798e515, /// 0xa78
			 0x5569b4ae, /// 0xa7c
			 0xf1a05f2b, /// 0xa80
			 0xc3ed29d3, /// 0xa84
			 0x3daafc31, /// 0xa88
			 0x22166589, /// 0xa8c
			 0xbdddaf55, /// 0xa90
			 0xa84b7d2e, /// 0xa94
			 0xf03c73cc, /// 0xa98
			 0xe7e44821, /// 0xa9c
			 0x56530eb5, /// 0xaa0
			 0x72256046, /// 0xaa4
			 0x569edc40, /// 0xaa8
			 0x2f97e5aa, /// 0xaac
			 0x7d777b13, /// 0xab0
			 0x7d7b4a5e, /// 0xab4
			 0x6463916f, /// 0xab8
			 0x2a3a04fc, /// 0xabc
			 0x7827c1d5, /// 0xac0
			 0x299acdb0, /// 0xac4
			 0x995572ad, /// 0xac8
			 0xf3caa8f7, /// 0xacc
			 0xe62d3df6, /// 0xad0
			 0x22a26620, /// 0xad4
			 0xd0eda12f, /// 0xad8
			 0x895d035a, /// 0xadc
			 0x2a31cbcc, /// 0xae0
			 0x8684d468, /// 0xae4
			 0x5697c1fc, /// 0xae8
			 0x75e88532, /// 0xaec
			 0xf64b678a, /// 0xaf0
			 0xa617599e, /// 0xaf4
			 0xc62f818a, /// 0xaf8
			 0x6926d026, /// 0xafc
			 0x04dff234, /// 0xb00
			 0xd1e07b65, /// 0xb04
			 0x78c8ef5d, /// 0xb08
			 0x086c6cc2, /// 0xb0c
			 0x37e4f959, /// 0xb10
			 0x04b855f3, /// 0xb14
			 0x016f19ae, /// 0xb18
			 0x5a0b26a5, /// 0xb1c
			 0xeff4a075, /// 0xb20
			 0x565d4b61, /// 0xb24
			 0x01be9570, /// 0xb28
			 0x062d3e12, /// 0xb2c
			 0xef31f0ae, /// 0xb30
			 0xbc28a890, /// 0xb34
			 0x29126a01, /// 0xb38
			 0x576cd93c, /// 0xb3c
			 0xcdd208df, /// 0xb40
			 0x0161b273, /// 0xb44
			 0x4466d8fe, /// 0xb48
			 0xad390c0e, /// 0xb4c
			 0xdc8cb877, /// 0xb50
			 0x9d4fa601, /// 0xb54
			 0xf36a2cb4, /// 0xb58
			 0x70578544, /// 0xb5c
			 0xd9a90d18, /// 0xb60
			 0x07156c51, /// 0xb64
			 0x700c9b8b, /// 0xb68
			 0x42daeced, /// 0xb6c
			 0xd42fc435, /// 0xb70
			 0xe084d3c6, /// 0xb74
			 0x4d50d5fb, /// 0xb78
			 0x48a910f9, /// 0xb7c
			 0x1b68ffd9, /// 0xb80
			 0xc49eeebf, /// 0xb84
			 0xe46d6ae5, /// 0xb88
			 0xd9aa8a7b, /// 0xb8c
			 0xc5d1cfcb, /// 0xb90
			 0x04908a33, /// 0xb94
			 0x9acdef38, /// 0xb98
			 0x4e56d60c, /// 0xb9c
			 0x8642982b, /// 0xba0
			 0xc19b2403, /// 0xba4
			 0x2b407ddd, /// 0xba8
			 0x47c1b7f2, /// 0xbac
			 0x49652dc4, /// 0xbb0
			 0x140f29cc, /// 0xbb4
			 0xbed6094a, /// 0xbb8
			 0xd7baef13, /// 0xbbc
			 0xbfe2872b, /// 0xbc0
			 0xe6661cc1, /// 0xbc4
			 0x85db4e6a, /// 0xbc8
			 0xf8728f92, /// 0xbcc
			 0x72b2c3c8, /// 0xbd0
			 0xb304d787, /// 0xbd4
			 0xd1008451, /// 0xbd8
			 0x950afa12, /// 0xbdc
			 0x106a3ddf, /// 0xbe0
			 0xaa14c182, /// 0xbe4
			 0x4d117708, /// 0xbe8
			 0xffd76574, /// 0xbec
			 0xb964fa95, /// 0xbf0
			 0xba8d7fc1, /// 0xbf4
			 0xf8b202d7, /// 0xbf8
			 0xb5469800, /// 0xbfc
			 0x2d64fe29, /// 0xc00
			 0x16e59e2c, /// 0xc04
			 0x8e54feab, /// 0xc08
			 0x3d2cb105, /// 0xc0c
			 0xccb47ec0, /// 0xc10
			 0xf45d5bb9, /// 0xc14
			 0x723199f9, /// 0xc18
			 0x45899d8f, /// 0xc1c
			 0xcaccbddf, /// 0xc20
			 0x78ec7d0b, /// 0xc24
			 0x947f554f, /// 0xc28
			 0xc3bc7c46, /// 0xc2c
			 0xfaa790e2, /// 0xc30
			 0x239ebdde, /// 0xc34
			 0x5ac23589, /// 0xc38
			 0x4ef5063f, /// 0xc3c
			 0xa746e7d9, /// 0xc40
			 0x1b84b07c, /// 0xc44
			 0xd2f91d33, /// 0xc48
			 0x867fe3b5, /// 0xc4c
			 0x8d6315b3, /// 0xc50
			 0x88f3215c, /// 0xc54
			 0x262278c3, /// 0xc58
			 0x86c65a0c, /// 0xc5c
			 0xeb67988d, /// 0xc60
			 0x632444fd, /// 0xc64
			 0x329fa363, /// 0xc68
			 0x8458b2e3, /// 0xc6c
			 0x5c956f07, /// 0xc70
			 0x17ca0ad6, /// 0xc74
			 0x47fffba8, /// 0xc78
			 0xc84fb669, /// 0xc7c
			 0x27303594, /// 0xc80
			 0xb20b1dda, /// 0xc84
			 0x38b7be36, /// 0xc88
			 0x5d74774f, /// 0xc8c
			 0x450cf1e7, /// 0xc90
			 0x470510da, /// 0xc94
			 0xa585a399, /// 0xc98
			 0xf391c93c, /// 0xc9c
			 0x66494185, /// 0xca0
			 0xb82b1f7d, /// 0xca4
			 0xe239baf1, /// 0xca8
			 0x2c1e7940, /// 0xcac
			 0x92f3d3da, /// 0xcb0
			 0x3593e2de, /// 0xcb4
			 0xaacb2029, /// 0xcb8
			 0xbc63b82b, /// 0xcbc
			 0x433b780d, /// 0xcc0
			 0x906876b4, /// 0xcc4
			 0xf0f263f4, /// 0xcc8
			 0xbe19fe8a, /// 0xccc
			 0xaeeee989, /// 0xcd0
			 0xd9d8168a, /// 0xcd4
			 0x74157710, /// 0xcd8
			 0x2d641364, /// 0xcdc
			 0x9f3c92f7, /// 0xce0
			 0x76ede65b, /// 0xce4
			 0x320eeb19, /// 0xce8
			 0x8f82c1d6, /// 0xcec
			 0xd30ec189, /// 0xcf0
			 0x2a9dd786, /// 0xcf4
			 0x8aa7b6ba, /// 0xcf8
			 0x4592050a, /// 0xcfc
			 0x9248892d, /// 0xd00
			 0xacf53260, /// 0xd04
			 0x30841682, /// 0xd08
			 0xc90fd853, /// 0xd0c
			 0x56d15539, /// 0xd10
			 0x6617a825, /// 0xd14
			 0xaa204dc8, /// 0xd18
			 0xbf108b2f, /// 0xd1c
			 0xe99ccb66, /// 0xd20
			 0x38dcbd94, /// 0xd24
			 0xe0a90ae1, /// 0xd28
			 0xdb15c02d, /// 0xd2c
			 0xad741c8d, /// 0xd30
			 0x840f78a5, /// 0xd34
			 0x43f53761, /// 0xd38
			 0x89b6771c, /// 0xd3c
			 0xc5e3f116, /// 0xd40
			 0xafb3e794, /// 0xd44
			 0x722fc17f, /// 0xd48
			 0x15f32e7c, /// 0xd4c
			 0x00ab4fed, /// 0xd50
			 0x983c0dbd, /// 0xd54
			 0xa6662034, /// 0xd58
			 0xddf7f665, /// 0xd5c
			 0x893816c0, /// 0xd60
			 0xcd61eeea, /// 0xd64
			 0x69b51a2d, /// 0xd68
			 0xfc717eee, /// 0xd6c
			 0xbc673c21, /// 0xd70
			 0xf08f98e8, /// 0xd74
			 0x69e6c61d, /// 0xd78
			 0x70fd8437, /// 0xd7c
			 0xc6f5b995, /// 0xd80
			 0x94e097c8, /// 0xd84
			 0x3eff069b, /// 0xd88
			 0xb2575ad3, /// 0xd8c
			 0xc2809112, /// 0xd90
			 0x792278a7, /// 0xd94
			 0x5527db1f, /// 0xd98
			 0xa4edf79e, /// 0xd9c
			 0x8eb4c8dc, /// 0xda0
			 0xfc97a646, /// 0xda4
			 0x8b873a52, /// 0xda8
			 0x7fe02a03, /// 0xdac
			 0xf5b1f8ce, /// 0xdb0
			 0xab7f78ce, /// 0xdb4
			 0xf6d1c948, /// 0xdb8
			 0x7c0ded92, /// 0xdbc
			 0x090d63c9, /// 0xdc0
			 0xfee18742, /// 0xdc4
			 0xa2526e50, /// 0xdc8
			 0xc11c37bb, /// 0xdcc
			 0xd6479119, /// 0xdd0
			 0xf11c8d1a, /// 0xdd4
			 0x8e915565, /// 0xdd8
			 0x97367677, /// 0xddc
			 0xf5559653, /// 0xde0
			 0xeddb2293, /// 0xde4
			 0x28c0d791, /// 0xde8
			 0xdae02f09, /// 0xdec
			 0xcd347e47, /// 0xdf0
			 0xd2925507, /// 0xdf4
			 0x4878da05, /// 0xdf8
			 0x9c0df860, /// 0xdfc
			 0x3f1296ee, /// 0xe00
			 0x470db8e0, /// 0xe04
			 0xc8d2a7e7, /// 0xe08
			 0x335fccb9, /// 0xe0c
			 0x5f6297d5, /// 0xe10
			 0x1b723d56, /// 0xe14
			 0x061e8f1f, /// 0xe18
			 0x50a1e6b7, /// 0xe1c
			 0x1ce84db8, /// 0xe20
			 0x4b8ac63d, /// 0xe24
			 0xdf093f1b, /// 0xe28
			 0xb24e5475, /// 0xe2c
			 0xde11ef74, /// 0xe30
			 0x21d85bbf, /// 0xe34
			 0x5ccc66ee, /// 0xe38
			 0xf10c76e1, /// 0xe3c
			 0x2989d41d, /// 0xe40
			 0x056c6f4a, /// 0xe44
			 0xdeaf7850, /// 0xe48
			 0x4f96af70, /// 0xe4c
			 0x80ab4531, /// 0xe50
			 0x8b1b1935, /// 0xe54
			 0x0ba16de5, /// 0xe58
			 0x158827a3, /// 0xe5c
			 0x168cb8d1, /// 0xe60
			 0xb51d4b74, /// 0xe64
			 0x2032c7fc, /// 0xe68
			 0xdca0132f, /// 0xe6c
			 0xb24b841d, /// 0xe70
			 0x3e3bf9fb, /// 0xe74
			 0x0de80d80, /// 0xe78
			 0xcc36593f, /// 0xe7c
			 0x61a5baeb, /// 0xe80
			 0x0055c8c5, /// 0xe84
			 0x017644c5, /// 0xe88
			 0x7c27663d, /// 0xe8c
			 0x580fb000, /// 0xe90
			 0x9b0e3722, /// 0xe94
			 0xa42ab660, /// 0xe98
			 0x2209dca7, /// 0xe9c
			 0xb402a94a, /// 0xea0
			 0xdda18783, /// 0xea4
			 0xe5e308a3, /// 0xea8
			 0x81b56759, /// 0xeac
			 0x974df00b, /// 0xeb0
			 0x1d55764e, /// 0xeb4
			 0x1cb232c7, /// 0xeb8
			 0xed367014, /// 0xebc
			 0x36e27abd, /// 0xec0
			 0x001e9a93, /// 0xec4
			 0x0a7d61cb, /// 0xec8
			 0x9018b0eb, /// 0xecc
			 0x7bb4e662, /// 0xed0
			 0x0e2c01c0, /// 0xed4
			 0x0070aa11, /// 0xed8
			 0x6159279a, /// 0xedc
			 0x6516cb91, /// 0xee0
			 0x0101cac7, /// 0xee4
			 0x969033de, /// 0xee8
			 0xfc991398, /// 0xeec
			 0x937096dd, /// 0xef0
			 0xeef8ab2b, /// 0xef4
			 0xe49775f1, /// 0xef8
			 0x55e81c95, /// 0xefc
			 0x8be18321, /// 0xf00
			 0xa0b6cedc, /// 0xf04
			 0x39cee341, /// 0xf08
			 0x58b9bb67, /// 0xf0c
			 0xc83478fc, /// 0xf10
			 0x1b2f5db5, /// 0xf14
			 0xcedd06b9, /// 0xf18
			 0x2bbacd60, /// 0xf1c
			 0xfc823adc, /// 0xf20
			 0xaa8b59a0, /// 0xf24
			 0x865e9963, /// 0xf28
			 0x1c75db82, /// 0xf2c
			 0x8c1e5720, /// 0xf30
			 0xe3f30c68, /// 0xf34
			 0xb55a612e, /// 0xf38
			 0x88e773ac, /// 0xf3c
			 0xec9baf8b, /// 0xf40
			 0x8f25aebe, /// 0xf44
			 0xa263d36f, /// 0xf48
			 0xb0b205f7, /// 0xf4c
			 0x3974dc62, /// 0xf50
			 0x204d81b8, /// 0xf54
			 0x6a42d82a, /// 0xf58
			 0xa5764a89, /// 0xf5c
			 0x56a525d7, /// 0xf60
			 0x88659872, /// 0xf64
			 0xdbd9e353, /// 0xf68
			 0xc7ac9c73, /// 0xf6c
			 0x76ba91c1, /// 0xf70
			 0x3a4a285a, /// 0xf74
			 0x7b075450, /// 0xf78
			 0xfc5deff2, /// 0xf7c
			 0x5475d210, /// 0xf80
			 0x3ea49e30, /// 0xf84
			 0xc73654bc, /// 0xf88
			 0x29ddebe4, /// 0xf8c
			 0x82be7a3c, /// 0xf90
			 0x94a1b2ba, /// 0xf94
			 0xdbfbd2a6, /// 0xf98
			 0x328bf9ed, /// 0xf9c
			 0xfe47d2c3, /// 0xfa0
			 0x404c05a7, /// 0xfa4
			 0x457a22a7, /// 0xfa8
			 0x93c1d372, /// 0xfac
			 0x69e638fb, /// 0xfb0
			 0x5337b894, /// 0xfb4
			 0xf8fa4e5b, /// 0xfb8
			 0x57098456, /// 0xfbc
			 0x5ddaccab, /// 0xfc0
			 0xcc97e720, /// 0xfc4
			 0xac83ef28, /// 0xfc8
			 0x527ce8f9, /// 0xfcc
			 0x3d2b4c94, /// 0xfd0
			 0xe6fc1b8a, /// 0xfd4
			 0xa16eeebb, /// 0xfd8
			 0x3c0589bc, /// 0xfdc
			 0x6b053275, /// 0xfe0
			 0x03d8a3b0, /// 0xfe4
			 0x7ff5c09e, /// 0xfe8
			 0x866f28ff, /// 0xfec
			 0xa0a60832, /// 0xff0
			 0x435b1a8c, /// 0xff4
			 0x68dd8136, /// 0xff8
			 0x3dc7dbf2 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00000
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00004
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00008
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0000c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00014
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00018
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0001c
			 0x3f028f5c,                                                  // 0.51                                        /// 00020
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00024
			 0x0c400000,                                                  // Leading 1s:                                 /// 00028
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00040
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00050
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00054
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00058
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0005c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00064
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0006c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00070
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00080
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00084
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00090
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00094
			 0xffc00001,                                                  // -signaling NaN                              /// 00098
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000c0
			 0x7f800000,                                                  // inf                                         /// 000c4
			 0x33333333,                                                  // 4 random values                             /// 000c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x80000000,                                                  // -zero                                       /// 000d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0010c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00110
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00114
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00120
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00128
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0012c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00130
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00134
			 0x55555555,                                                  // 4 random values                             /// 00138
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0013c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00140
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00144
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00148
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0014c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00160
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00168
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0016c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00170
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00174
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00178
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x33333333,                                                  // 4 random values                             /// 00188
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0018c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00190
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00194
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00198
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x80000000,                                                  // -zero                                       /// 001a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x55555555,                                                  // 4 random values                             /// 001d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 001d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x55555555,                                                  // 4 random values                             /// 001fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00200
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00208
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0020c
			 0x80000000,                                                  // -zero                                       /// 00210
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00214
			 0xbf028f5c,                                                  // -0.51                                       /// 00218
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0021c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00220
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00228
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0022c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00230
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00234
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00238
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00240
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00248
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00250
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0025c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00260
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00264
			 0x0c780000,                                                  // Leading 1s:                                 /// 00268
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00270
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00274
			 0x4b000000,                                                  // 8388608.0                                   /// 00278
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0027c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00288
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00290
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0029c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002e0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 002ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00300
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x80011111,                                                  // -9.7958E-41                                 /// 00308
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0030c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00310
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00318
			 0x0e000001,                                                  // Trailing 1s:                                /// 0031c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00320
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00328
			 0x0c780000,                                                  // Leading 1s:                                 /// 0032c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00334
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00338
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0034c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00350
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00358
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0035c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x55555555,                                                  // 4 random values                             /// 00364
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0036c
			 0xff800000,                                                  // -inf                                        /// 00370
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00378
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0037c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00384
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00388
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00390
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 003ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003b8
			 0x3f028f5c,                                                  // 0.51                                        /// 003bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0xff800000,                                                  // -inf                                        /// 003c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00400
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00408
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x0c400000,                                                  // Leading 1s:                                 /// 00418
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0041c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00420
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00424
			 0x0e000003,                                                  // Trailing 1s:                                /// 00428
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0042c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00430
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00434
			 0x33333333,                                                  // 4 random values                             /// 00438
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00444
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00448
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0044c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00454
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00458
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0045c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00464
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00468
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0046c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00470
			 0x7fc00001,                                                  // signaling NaN                               /// 00474
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0xffc00001,                                                  // -signaling NaN                              /// 0047c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00484
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00488
			 0x33333333,                                                  // 4 random values                             /// 0048c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00490
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00494
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00498
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00000000,                                                  // zero                                        /// 004d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00500
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00504
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00508
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0050c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00510
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00520
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00524
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00538
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0053c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00540
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00544
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0055c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00560
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00564
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00568
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0056c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00570
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00574
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00578
			 0x0e000007,                                                  // Trailing 1s:                                /// 0057c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x00011111,                                                  // 9.7958E-41                                  /// 00584
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00588
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0058c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00590
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00594
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00598
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 005c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x80000000,                                                  // -zero                                       /// 005d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0xffc00001,                                                  // -signaling NaN                              /// 005e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 005f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00600
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00604
			 0x7fc00001,                                                  // signaling NaN                               /// 00608
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0060c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0c780000,                                                  // Leading 1s:                                 /// 00614
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0061c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00620
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00624
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0xcb000000,                                                  // -8388608.0                                  /// 00634
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0063c
			 0xcb000000,                                                  // -8388608.0                                  /// 00640
			 0xff800000,                                                  // -inf                                        /// 00644
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00648
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0064c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00650
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00654
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00658
			 0x00000000,                                                  // zero                                        /// 0065c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00660
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0066c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00670
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00674
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00678
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0067c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00680
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00688
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0068c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00690
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00694
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00698
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x55555555,                                                  // 4 random values                             /// 006d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006d8
			 0x4b000000,                                                  // 8388608.0                                   /// 006dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00700
			 0x80000000,                                                  // -zero                                       /// 00704
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00710
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00714
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00718
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00720
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00724
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00728
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00730
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00734
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0073c
			 0xbf028f5c,                                                  // -0.51                                       /// 00740
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00744
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0074c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00750
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00754
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00758
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00760
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00768
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0076c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00770
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00774
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00778
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0077c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00784
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0c600000,                                                  // Leading 1s:                                 /// 0078c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00790
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00800
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00804
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0080c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00810
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00814
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0081c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00820
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00828
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0082c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00834
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0083c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0084c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00850
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00854
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x3f028f5c,                                                  // 0.51                                        /// 0085c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00860
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00864
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00870
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x80011111,                                                  // -9.7958E-41                                 /// 00878
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0087c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00880
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00884
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0088c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x0e000007,                                                  // Trailing 1s:                                /// 00898
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0089c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00900
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00904
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00910
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00914
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00918
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0091c
			 0xffc00001,                                                  // -signaling NaN                              /// 00920
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0092c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00930
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00938
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0093c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00940
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00944
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00948
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0094c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00950
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00954
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00958
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00960
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00964
			 0x4b000000,                                                  // 8388608.0                                   /// 00968
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0096c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00970
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00974
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0097c
			 0x7f800000,                                                  // inf                                         /// 00980
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00984
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00988
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0098c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00994
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00998
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0099c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a08
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a10
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a20
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a30
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a44
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a48
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a5c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a68
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a74
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a84
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a9c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00aa0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00aa4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00aa8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00aac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ab0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ab4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00abc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ac0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ac8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ad4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ad8
			 0x33333333,                                                  // 4 random values                             /// 00adc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ae0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ae8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00afc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b00
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b04
			 0xff800000,                                                  // -inf                                        /// 00b08
			 0x7f800000,                                                  // inf                                         /// 00b0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b24
			 0x00000000,                                                  // zero                                        /// 00b28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b30
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b6c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b74
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b90
			 0x55555555,                                                  // 4 random values                             /// 00b94
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ba0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ba4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bb0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bb4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bbc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bc0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bc8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bcc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bd0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bd4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bd8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bdc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00be4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00be8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c00
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c04
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c08
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c18
			 0x7fc00001,                                                  // signaling NaN                               /// 00c1c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c20
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x7fc00001,                                                  // signaling NaN                               /// 00c28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c2c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c30
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c34
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c3c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c50
			 0xbf028f5c,                                                  // -0.51                                       /// 00c54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c58
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c5c
			 0x00000000,                                                  // zero                                        /// 00c60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c64
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c68
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c6c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c78
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c80
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c84
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c9c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ca0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ca4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ca8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cb0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cb4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cc0
			 0x7f800000,                                                  // inf                                         /// 00cc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cc8
			 0x80000000,                                                  // -zero                                       /// 00ccc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cd0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cd4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cd8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ce0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ce8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00000000,                                                  // zero                                        /// 00cf0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cf4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cf8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cfc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d10
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0xffc00001,                                                  // -signaling NaN                              /// 00d28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d30
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0xffc00001,                                                  // -signaling NaN                              /// 00d58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d5c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d64
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d68
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d6c
			 0x80000000,                                                  // -zero                                       /// 00d70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d74
			 0x3f028f5c,                                                  // 0.51                                        /// 00d78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d8c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d98
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00da4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00da8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00db4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00db8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dbc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dc0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dc4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dcc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dd8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ddc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00de0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00de4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00de8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dec
			 0xcb000000,                                                  // -8388608.0                                  /// 00df0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00df4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00df8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dfc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e14
			 0x33333333,                                                  // 4 random values                             /// 00e18
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e24
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e38
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e44
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e4c
			 0xff800000,                                                  // -inf                                        /// 00e50
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e5c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e64
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e6c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e78
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e84
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e90
			 0x4b000000,                                                  // 8388608.0                                   /// 00e94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ea0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ea8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00eac
			 0x4b000000,                                                  // 8388608.0                                   /// 00eb0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00eb4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00eb8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ebc
			 0x80000000,                                                  // -zero                                       /// 00ec0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ec8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ecc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ed4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ed8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ee0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ee4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00eec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ef0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ef8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f00
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f04
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f18
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f30
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f40
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f68
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f70
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f78
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f8c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f98
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f9c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fac
			 0x33333333,                                                  // 4 random values                             /// 00fb0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fb8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fbc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fc0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fcc
			 0xff800000,                                                  // -inf                                        /// 00fd0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fd8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fdc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0xffc00001,                                                  // -signaling NaN                              /// 00fe4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fec
			 0x3f028f5c,                                                  // 0.51                                        /// 00ff0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ff4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ff8
			 0xff800000                                                  // -inf                                        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xe0c21c20, /// 0x0
			 0x71594a8e, /// 0x4
			 0x7846fbbf, /// 0x8
			 0x14f36445, /// 0xc
			 0x5b9bb5fc, /// 0x10
			 0x712775aa, /// 0x14
			 0x7886d37f, /// 0x18
			 0xeb27dcc4, /// 0x1c
			 0x76e8263f, /// 0x20
			 0xe8c3512a, /// 0x24
			 0x8c7a73c0, /// 0x28
			 0x55198f25, /// 0x2c
			 0x1552a72c, /// 0x30
			 0xbb09d87a, /// 0x34
			 0x38874f01, /// 0x38
			 0x1b9ca207, /// 0x3c
			 0x65aa4d74, /// 0x40
			 0xe28847de, /// 0x44
			 0x20b32447, /// 0x48
			 0xafd92d4a, /// 0x4c
			 0x868aac58, /// 0x50
			 0xe7b12d1d, /// 0x54
			 0x2f0005de, /// 0x58
			 0xa4f039b5, /// 0x5c
			 0x17337e5f, /// 0x60
			 0x8fd7a43a, /// 0x64
			 0x468a7cbd, /// 0x68
			 0x662c0304, /// 0x6c
			 0xe61f0782, /// 0x70
			 0xa46b11df, /// 0x74
			 0xc572978f, /// 0x78
			 0x7ae32d61, /// 0x7c
			 0x33fa0268, /// 0x80
			 0xf565a669, /// 0x84
			 0x13ef4f6f, /// 0x88
			 0x24d3441a, /// 0x8c
			 0xef389033, /// 0x90
			 0xe9ddc638, /// 0x94
			 0xd41412e5, /// 0x98
			 0xf537a972, /// 0x9c
			 0x08e00de4, /// 0xa0
			 0x7b3fa73b, /// 0xa4
			 0xbb2ee9d8, /// 0xa8
			 0x2c608f67, /// 0xac
			 0x51bf1d4a, /// 0xb0
			 0x650ac88f, /// 0xb4
			 0x577464b9, /// 0xb8
			 0xc41dd68c, /// 0xbc
			 0x8302ea8f, /// 0xc0
			 0xee763afa, /// 0xc4
			 0xcdb33ff8, /// 0xc8
			 0x8669a445, /// 0xcc
			 0x7ee176f0, /// 0xd0
			 0x758085bf, /// 0xd4
			 0x6cd0b1aa, /// 0xd8
			 0xfc177b2b, /// 0xdc
			 0x6ea11c16, /// 0xe0
			 0x53b3f7e9, /// 0xe4
			 0x899acb8b, /// 0xe8
			 0xcdb7faa3, /// 0xec
			 0xa105fc87, /// 0xf0
			 0x4c03e69e, /// 0xf4
			 0x35f57c0c, /// 0xf8
			 0xf2177eed, /// 0xfc
			 0xa1516fd0, /// 0x100
			 0x202ed7a7, /// 0x104
			 0xed56486b, /// 0x108
			 0xe630394f, /// 0x10c
			 0x2ebd47b9, /// 0x110
			 0x93c303b2, /// 0x114
			 0x04c004e5, /// 0x118
			 0xa0eeba85, /// 0x11c
			 0xdf2f511e, /// 0x120
			 0xe7568305, /// 0x124
			 0x76e9eb8a, /// 0x128
			 0x19250b43, /// 0x12c
			 0x7fad4e1c, /// 0x130
			 0x0f17b0bf, /// 0x134
			 0x5a782e68, /// 0x138
			 0x6055d841, /// 0x13c
			 0xeccce625, /// 0x140
			 0xfeed9964, /// 0x144
			 0xe4d9d6f1, /// 0x148
			 0x47fe6f4f, /// 0x14c
			 0x47ec7712, /// 0x150
			 0xa81bec6e, /// 0x154
			 0xe3bc4604, /// 0x158
			 0x87c0d0ff, /// 0x15c
			 0x6a024c6f, /// 0x160
			 0x49066221, /// 0x164
			 0x785709ae, /// 0x168
			 0xc3ddac47, /// 0x16c
			 0x42560d1b, /// 0x170
			 0x65fd8246, /// 0x174
			 0x41a41f3e, /// 0x178
			 0x7874cd65, /// 0x17c
			 0x1df93b8f, /// 0x180
			 0x68cafcd8, /// 0x184
			 0xb616f8c5, /// 0x188
			 0x646c53c1, /// 0x18c
			 0x30871878, /// 0x190
			 0xe9723a4c, /// 0x194
			 0x7fd24d9f, /// 0x198
			 0xbf323b32, /// 0x19c
			 0xf817dd8b, /// 0x1a0
			 0xe13fa0bd, /// 0x1a4
			 0xb28dbee8, /// 0x1a8
			 0x8d30b692, /// 0x1ac
			 0x198455af, /// 0x1b0
			 0x4e54f278, /// 0x1b4
			 0x70c0cc81, /// 0x1b8
			 0xe6498a71, /// 0x1bc
			 0x6960fb64, /// 0x1c0
			 0x949ade72, /// 0x1c4
			 0x7561a932, /// 0x1c8
			 0x9cf2481a, /// 0x1cc
			 0xf98a15c4, /// 0x1d0
			 0x9e3fa9b8, /// 0x1d4
			 0x0787a087, /// 0x1d8
			 0x108e990d, /// 0x1dc
			 0xf35391dd, /// 0x1e0
			 0x497e182b, /// 0x1e4
			 0xb05f1223, /// 0x1e8
			 0xad32618d, /// 0x1ec
			 0x2ce7114a, /// 0x1f0
			 0xcffbeeee, /// 0x1f4
			 0xc29b80e4, /// 0x1f8
			 0x20bbfced, /// 0x1fc
			 0x89618e7c, /// 0x200
			 0xf21500d1, /// 0x204
			 0xc60ead57, /// 0x208
			 0xab5a166e, /// 0x20c
			 0x62fcb0a8, /// 0x210
			 0x66af5dd2, /// 0x214
			 0xd2f73dcc, /// 0x218
			 0xa1b57874, /// 0x21c
			 0x4808bb90, /// 0x220
			 0x6e548a09, /// 0x224
			 0xb904c5ae, /// 0x228
			 0xa063f4ab, /// 0x22c
			 0x68347a36, /// 0x230
			 0xbea18d5e, /// 0x234
			 0xbd26c28f, /// 0x238
			 0x30f301f7, /// 0x23c
			 0xf471dc1e, /// 0x240
			 0x520c1e34, /// 0x244
			 0x49f09afa, /// 0x248
			 0x0194ed9d, /// 0x24c
			 0x81b09bfd, /// 0x250
			 0x60d6c30f, /// 0x254
			 0xbbe3096f, /// 0x258
			 0xf28993b7, /// 0x25c
			 0x8fbc9c8e, /// 0x260
			 0xa84d3037, /// 0x264
			 0x60752061, /// 0x268
			 0x2a556b4f, /// 0x26c
			 0x39220b4b, /// 0x270
			 0xe6d7c22c, /// 0x274
			 0x18d2e22c, /// 0x278
			 0x5b5fd769, /// 0x27c
			 0x63b39eb7, /// 0x280
			 0x960fd37e, /// 0x284
			 0x96e769b6, /// 0x288
			 0x6df6e9ef, /// 0x28c
			 0x195474d8, /// 0x290
			 0x30638ec1, /// 0x294
			 0x0c400f93, /// 0x298
			 0xa2b6d66c, /// 0x29c
			 0xf3875839, /// 0x2a0
			 0xe76f06f0, /// 0x2a4
			 0x19a28144, /// 0x2a8
			 0x7029f7be, /// 0x2ac
			 0xb9263095, /// 0x2b0
			 0x46a78311, /// 0x2b4
			 0x277c08d1, /// 0x2b8
			 0x3d5240e7, /// 0x2bc
			 0x0f1dae89, /// 0x2c0
			 0x6fe6aa8b, /// 0x2c4
			 0x08116d0a, /// 0x2c8
			 0x461ed2d9, /// 0x2cc
			 0x94749424, /// 0x2d0
			 0x1358b339, /// 0x2d4
			 0x629eb0ce, /// 0x2d8
			 0xc1c233bb, /// 0x2dc
			 0x065af9ab, /// 0x2e0
			 0x91bb5f16, /// 0x2e4
			 0x96641081, /// 0x2e8
			 0x3f6c047f, /// 0x2ec
			 0x10561a29, /// 0x2f0
			 0xb5f899c5, /// 0x2f4
			 0x202f1560, /// 0x2f8
			 0x4328ecf8, /// 0x2fc
			 0xb63b31f8, /// 0x300
			 0x29a46ce1, /// 0x304
			 0xa18881b1, /// 0x308
			 0x4df90619, /// 0x30c
			 0xbeb3281b, /// 0x310
			 0xd24b5493, /// 0x314
			 0xc9a5b045, /// 0x318
			 0x79bcc51f, /// 0x31c
			 0x8aac0cbd, /// 0x320
			 0xfc58a735, /// 0x324
			 0xe8cd6078, /// 0x328
			 0x4b43832f, /// 0x32c
			 0x2eb13f1b, /// 0x330
			 0xc436d6c7, /// 0x334
			 0xc563e5b9, /// 0x338
			 0xc1c8202d, /// 0x33c
			 0xdd271ce7, /// 0x340
			 0xb8d5a91f, /// 0x344
			 0xa5d6b485, /// 0x348
			 0xc70911d9, /// 0x34c
			 0xd293976b, /// 0x350
			 0xa6bc30f2, /// 0x354
			 0x67f6bd1e, /// 0x358
			 0x732967d7, /// 0x35c
			 0x06a4ea40, /// 0x360
			 0xaffb960b, /// 0x364
			 0xb776a4cc, /// 0x368
			 0xdaa768a3, /// 0x36c
			 0x745dd4b0, /// 0x370
			 0xf79b29f2, /// 0x374
			 0xdfe0b5a8, /// 0x378
			 0xebc94159, /// 0x37c
			 0x31fd1446, /// 0x380
			 0xc4447fc1, /// 0x384
			 0x67ea9b88, /// 0x388
			 0x17504ec8, /// 0x38c
			 0x9c647b43, /// 0x390
			 0x04cf4da8, /// 0x394
			 0x170be5fe, /// 0x398
			 0xb214770a, /// 0x39c
			 0x0a512e52, /// 0x3a0
			 0xa7f4131b, /// 0x3a4
			 0xec925b37, /// 0x3a8
			 0xc3fda5ac, /// 0x3ac
			 0x474b0145, /// 0x3b0
			 0xcbca4c21, /// 0x3b4
			 0xf0e06749, /// 0x3b8
			 0xaa3c5032, /// 0x3bc
			 0x9eeacf2f, /// 0x3c0
			 0x1f99948c, /// 0x3c4
			 0x01a18320, /// 0x3c8
			 0x61cd4cd7, /// 0x3cc
			 0x6fb0509b, /// 0x3d0
			 0x4d1afec5, /// 0x3d4
			 0x2890cce3, /// 0x3d8
			 0x09852e68, /// 0x3dc
			 0xcafcffee, /// 0x3e0
			 0x3c58b2b6, /// 0x3e4
			 0x549e869d, /// 0x3e8
			 0x7e81dbe2, /// 0x3ec
			 0x3de01994, /// 0x3f0
			 0xa04a7838, /// 0x3f4
			 0x012088e0, /// 0x3f8
			 0x39ba4636, /// 0x3fc
			 0x2ff41c5b, /// 0x400
			 0xf84a820b, /// 0x404
			 0xe3bc5d5f, /// 0x408
			 0xb04eb264, /// 0x40c
			 0x4295db75, /// 0x410
			 0x2c5de2d7, /// 0x414
			 0x93225fd4, /// 0x418
			 0xd8deecb0, /// 0x41c
			 0x3f19f868, /// 0x420
			 0x040458a7, /// 0x424
			 0xf1a685d4, /// 0x428
			 0x1800f2f6, /// 0x42c
			 0xedf05036, /// 0x430
			 0xd5fe34ab, /// 0x434
			 0x9ca75e3d, /// 0x438
			 0xc6f3b8b3, /// 0x43c
			 0x7a29f8ae, /// 0x440
			 0xf02e2a10, /// 0x444
			 0x68555365, /// 0x448
			 0x9e67b2ad, /// 0x44c
			 0x608570f7, /// 0x450
			 0x1a39dcfa, /// 0x454
			 0x620cdaf5, /// 0x458
			 0x465ebba2, /// 0x45c
			 0x314e1f8b, /// 0x460
			 0xe79fb600, /// 0x464
			 0xc2cd66bc, /// 0x468
			 0x951b8083, /// 0x46c
			 0xc73197f5, /// 0x470
			 0xaf3b040a, /// 0x474
			 0xdd095ebe, /// 0x478
			 0xc54acf38, /// 0x47c
			 0x7d74bd33, /// 0x480
			 0x020adfdb, /// 0x484
			 0x07fdfbae, /// 0x488
			 0xf2afd67f, /// 0x48c
			 0x0034c50f, /// 0x490
			 0x12a077dc, /// 0x494
			 0x03d5d84e, /// 0x498
			 0x00827f57, /// 0x49c
			 0x0e0696d8, /// 0x4a0
			 0x87795b6f, /// 0x4a4
			 0xaa758b5c, /// 0x4a8
			 0x865ed07f, /// 0x4ac
			 0xcf424839, /// 0x4b0
			 0x94484295, /// 0x4b4
			 0x04a25b2d, /// 0x4b8
			 0xb68e9f71, /// 0x4bc
			 0x81c24ede, /// 0x4c0
			 0xa9956594, /// 0x4c4
			 0xd0f07f03, /// 0x4c8
			 0xd46880ee, /// 0x4cc
			 0x3cdb9c82, /// 0x4d0
			 0x97dd2947, /// 0x4d4
			 0x5be15c2f, /// 0x4d8
			 0x11cd3297, /// 0x4dc
			 0x86867802, /// 0x4e0
			 0x95f3cc48, /// 0x4e4
			 0xda551c6f, /// 0x4e8
			 0xa098825f, /// 0x4ec
			 0x90f17786, /// 0x4f0
			 0xc1181996, /// 0x4f4
			 0x39bd23aa, /// 0x4f8
			 0x55542d74, /// 0x4fc
			 0xe22eea78, /// 0x500
			 0x61649885, /// 0x504
			 0x407eab5a, /// 0x508
			 0x678fe6a0, /// 0x50c
			 0x5bf78405, /// 0x510
			 0xa39f17b1, /// 0x514
			 0xa40a4a99, /// 0x518
			 0xac9dd3e9, /// 0x51c
			 0xb80b5e88, /// 0x520
			 0x76bcde62, /// 0x524
			 0x8c78470f, /// 0x528
			 0x2ccc367c, /// 0x52c
			 0x5744d12b, /// 0x530
			 0x33165900, /// 0x534
			 0xa32c3ac7, /// 0x538
			 0xbc29a321, /// 0x53c
			 0x243c1b4a, /// 0x540
			 0xb58aaa81, /// 0x544
			 0x97cc13cb, /// 0x548
			 0x7a4ef256, /// 0x54c
			 0x5eae7b1f, /// 0x550
			 0x9da283e1, /// 0x554
			 0x61dcd65e, /// 0x558
			 0xc5f499cd, /// 0x55c
			 0x51774a25, /// 0x560
			 0x4aedd4c4, /// 0x564
			 0xd85bb3fb, /// 0x568
			 0x70c45f77, /// 0x56c
			 0x3ab4d2e3, /// 0x570
			 0x0c3a4e40, /// 0x574
			 0x234e8cbe, /// 0x578
			 0x1fbcdd91, /// 0x57c
			 0x9f15a072, /// 0x580
			 0xeade860d, /// 0x584
			 0x47f2d8b8, /// 0x588
			 0xaaec8444, /// 0x58c
			 0xa6134c7e, /// 0x590
			 0x28a57303, /// 0x594
			 0x7652995d, /// 0x598
			 0x36046817, /// 0x59c
			 0x068f2393, /// 0x5a0
			 0x23b667af, /// 0x5a4
			 0xf6effb3f, /// 0x5a8
			 0xaadca12f, /// 0x5ac
			 0xd39f7ed5, /// 0x5b0
			 0xecc15613, /// 0x5b4
			 0x4afdd7cb, /// 0x5b8
			 0xaf9f74f3, /// 0x5bc
			 0xb628805a, /// 0x5c0
			 0x26bae197, /// 0x5c4
			 0xddc62d7c, /// 0x5c8
			 0xc1f94cdc, /// 0x5cc
			 0xbe78def7, /// 0x5d0
			 0x09cf9d4e, /// 0x5d4
			 0x6067c3f4, /// 0x5d8
			 0x036abb4c, /// 0x5dc
			 0x0e90a4c0, /// 0x5e0
			 0xa271461e, /// 0x5e4
			 0x16cc088d, /// 0x5e8
			 0xad1b1939, /// 0x5ec
			 0x09e5a092, /// 0x5f0
			 0x9f256049, /// 0x5f4
			 0xe97dd603, /// 0x5f8
			 0xce1c7bcd, /// 0x5fc
			 0x86623e80, /// 0x600
			 0x7f495fea, /// 0x604
			 0x074f85e6, /// 0x608
			 0xf4ff72f1, /// 0x60c
			 0x10efd333, /// 0x610
			 0x1adb98ce, /// 0x614
			 0x5a99a195, /// 0x618
			 0x80f1504b, /// 0x61c
			 0xe25acc9e, /// 0x620
			 0x394d258a, /// 0x624
			 0x88784aa9, /// 0x628
			 0xd32b907b, /// 0x62c
			 0xde4247a8, /// 0x630
			 0xdd4c8f44, /// 0x634
			 0x817ad8d9, /// 0x638
			 0xe8117f2f, /// 0x63c
			 0xf604cc0a, /// 0x640
			 0xb47a2a64, /// 0x644
			 0x0b89ae80, /// 0x648
			 0x64e853bb, /// 0x64c
			 0x6bfdd0c0, /// 0x650
			 0x400c2fda, /// 0x654
			 0x0cbff688, /// 0x658
			 0x0746ebc7, /// 0x65c
			 0xf097be2e, /// 0x660
			 0xceb890f1, /// 0x664
			 0x9f8b0a93, /// 0x668
			 0x7d4f78c8, /// 0x66c
			 0x59d7a847, /// 0x670
			 0xa4f4e4a0, /// 0x674
			 0xb6492fe0, /// 0x678
			 0x8004b4e0, /// 0x67c
			 0x2e4cd93e, /// 0x680
			 0xf9e7bf34, /// 0x684
			 0x251b3235, /// 0x688
			 0xcb49ba65, /// 0x68c
			 0xb7725963, /// 0x690
			 0x43aa9fdc, /// 0x694
			 0x39b68115, /// 0x698
			 0x304f45a6, /// 0x69c
			 0x15a14de1, /// 0x6a0
			 0x1ae9c6e6, /// 0x6a4
			 0x1c72ef68, /// 0x6a8
			 0x73b2dd06, /// 0x6ac
			 0x3d5a6c2e, /// 0x6b0
			 0x3e91ee7e, /// 0x6b4
			 0x801d7b3a, /// 0x6b8
			 0x90d5cffe, /// 0x6bc
			 0xc8274d4c, /// 0x6c0
			 0x5065649c, /// 0x6c4
			 0x35a1fe81, /// 0x6c8
			 0x7f634b9e, /// 0x6cc
			 0xd6feb311, /// 0x6d0
			 0xf1ff48be, /// 0x6d4
			 0x46ad2f09, /// 0x6d8
			 0x51a60461, /// 0x6dc
			 0xee12a8d0, /// 0x6e0
			 0xfd19d40f, /// 0x6e4
			 0xd941175e, /// 0x6e8
			 0x85695e93, /// 0x6ec
			 0x7f513eaa, /// 0x6f0
			 0x23b8590b, /// 0x6f4
			 0xc38fc0ae, /// 0x6f8
			 0x1d62b9b1, /// 0x6fc
			 0x02507e6e, /// 0x700
			 0x1b1846ff, /// 0x704
			 0x350d5ac4, /// 0x708
			 0x9844e422, /// 0x70c
			 0xc46a9f8b, /// 0x710
			 0xc12b7f22, /// 0x714
			 0xd5be0052, /// 0x718
			 0x9549a53c, /// 0x71c
			 0xda38353f, /// 0x720
			 0xace6b738, /// 0x724
			 0x544b13e4, /// 0x728
			 0xed0dafd0, /// 0x72c
			 0x17cb24f5, /// 0x730
			 0xfca8dda8, /// 0x734
			 0xdc576940, /// 0x738
			 0xda11bde8, /// 0x73c
			 0xaef78592, /// 0x740
			 0x85c671d2, /// 0x744
			 0x5ac7a697, /// 0x748
			 0xb11b6e5e, /// 0x74c
			 0x0195efd9, /// 0x750
			 0x2cf4dd5e, /// 0x754
			 0xda58a08b, /// 0x758
			 0x79354477, /// 0x75c
			 0xfa51b9bf, /// 0x760
			 0xd6fef8ab, /// 0x764
			 0xac32c209, /// 0x768
			 0xf1b49a3f, /// 0x76c
			 0x2138bc86, /// 0x770
			 0xdd29e55f, /// 0x774
			 0xbd575ffa, /// 0x778
			 0x9fedd860, /// 0x77c
			 0x4868482a, /// 0x780
			 0x14407d30, /// 0x784
			 0x3f453b3d, /// 0x788
			 0x8de44394, /// 0x78c
			 0x40990779, /// 0x790
			 0xf313e39c, /// 0x794
			 0xeeec7062, /// 0x798
			 0x515e9429, /// 0x79c
			 0x86f4c8cb, /// 0x7a0
			 0xd8437c05, /// 0x7a4
			 0xfb2f4772, /// 0x7a8
			 0x2db8e739, /// 0x7ac
			 0x2ee40916, /// 0x7b0
			 0x237b96fc, /// 0x7b4
			 0x0e251067, /// 0x7b8
			 0x8ace40bd, /// 0x7bc
			 0x837ad1cd, /// 0x7c0
			 0x9f1e140f, /// 0x7c4
			 0xcf82bb15, /// 0x7c8
			 0xc93e5c58, /// 0x7cc
			 0x1065e839, /// 0x7d0
			 0x91c9c508, /// 0x7d4
			 0x854b2da9, /// 0x7d8
			 0xdef9b358, /// 0x7dc
			 0xbbef04c5, /// 0x7e0
			 0x1a6ccf68, /// 0x7e4
			 0x09fb3a18, /// 0x7e8
			 0x64590a88, /// 0x7ec
			 0x70334a8d, /// 0x7f0
			 0x432ae264, /// 0x7f4
			 0xd327eeeb, /// 0x7f8
			 0xfb8463ff, /// 0x7fc
			 0x1f469338, /// 0x800
			 0x4d79a485, /// 0x804
			 0x6fd09692, /// 0x808
			 0xf328b54e, /// 0x80c
			 0x2db98b98, /// 0x810
			 0x8093bc27, /// 0x814
			 0x79402cc0, /// 0x818
			 0x5b0cc387, /// 0x81c
			 0xb3d93c60, /// 0x820
			 0x45c7f0bc, /// 0x824
			 0x0d6d519d, /// 0x828
			 0xd72de6fb, /// 0x82c
			 0x8e4ef6a6, /// 0x830
			 0xf4d43b26, /// 0x834
			 0x9050e0ef, /// 0x838
			 0xdaae6c61, /// 0x83c
			 0x89ff56b3, /// 0x840
			 0xba351b44, /// 0x844
			 0xe20efd1c, /// 0x848
			 0xd2a7343f, /// 0x84c
			 0x69ed0d51, /// 0x850
			 0xb6a81b6c, /// 0x854
			 0x8b712f93, /// 0x858
			 0xc0f7954a, /// 0x85c
			 0x38f75345, /// 0x860
			 0x3d89347c, /// 0x864
			 0xcae21624, /// 0x868
			 0xe1bfb8e0, /// 0x86c
			 0x9344fbfe, /// 0x870
			 0x17eccf27, /// 0x874
			 0x22ded933, /// 0x878
			 0x4030d16e, /// 0x87c
			 0x62a80ef0, /// 0x880
			 0xb52ff175, /// 0x884
			 0xda2d5230, /// 0x888
			 0xb9df384d, /// 0x88c
			 0xcffc2c2e, /// 0x890
			 0xcf2922a9, /// 0x894
			 0x3b14d252, /// 0x898
			 0x7eaa9703, /// 0x89c
			 0xff2ae187, /// 0x8a0
			 0xedd5130e, /// 0x8a4
			 0x07e42959, /// 0x8a8
			 0x488f0578, /// 0x8ac
			 0x7f003b4c, /// 0x8b0
			 0x8ed08684, /// 0x8b4
			 0x711e7cd8, /// 0x8b8
			 0x38c2a202, /// 0x8bc
			 0xe7e8d8ad, /// 0x8c0
			 0x08322b9d, /// 0x8c4
			 0x8a9b1b38, /// 0x8c8
			 0x2cc81527, /// 0x8cc
			 0x9ed4d341, /// 0x8d0
			 0xfd58d2b2, /// 0x8d4
			 0xfc4ab2df, /// 0x8d8
			 0x6127658d, /// 0x8dc
			 0x432f848a, /// 0x8e0
			 0xcc9b4fd6, /// 0x8e4
			 0xd31451c8, /// 0x8e8
			 0x78d9c682, /// 0x8ec
			 0xb0cb5d2c, /// 0x8f0
			 0x5581ab02, /// 0x8f4
			 0x07fc9af2, /// 0x8f8
			 0xef2371e8, /// 0x8fc
			 0x1ef9d33f, /// 0x900
			 0x928abe20, /// 0x904
			 0x2af5d9f7, /// 0x908
			 0xa3beff1e, /// 0x90c
			 0xf363d8d9, /// 0x910
			 0x95b9db42, /// 0x914
			 0x113dd2a0, /// 0x918
			 0xa336d557, /// 0x91c
			 0xee1d35b4, /// 0x920
			 0x83376440, /// 0x924
			 0x42a25b70, /// 0x928
			 0xd4b561bb, /// 0x92c
			 0xeffe7f00, /// 0x930
			 0xdf16ce33, /// 0x934
			 0xdaf2f0b5, /// 0x938
			 0x06bb706f, /// 0x93c
			 0x06121b90, /// 0x940
			 0xe20b232a, /// 0x944
			 0xc56b0fd4, /// 0x948
			 0x90b1ceaf, /// 0x94c
			 0x4394a48d, /// 0x950
			 0x8ab76d01, /// 0x954
			 0x906b11f1, /// 0x958
			 0x888fae6a, /// 0x95c
			 0xdfd8075e, /// 0x960
			 0x30882134, /// 0x964
			 0x40b8f2da, /// 0x968
			 0x58be866d, /// 0x96c
			 0x9ff6910f, /// 0x970
			 0x47e04fd6, /// 0x974
			 0xe6fdbeac, /// 0x978
			 0x64a48e75, /// 0x97c
			 0x74457c08, /// 0x980
			 0x63413860, /// 0x984
			 0x780d68ee, /// 0x988
			 0x421903c7, /// 0x98c
			 0x388411f4, /// 0x990
			 0xb701dc9c, /// 0x994
			 0x76d277d8, /// 0x998
			 0xf420cb4b, /// 0x99c
			 0x84e94cd3, /// 0x9a0
			 0x0c794dee, /// 0x9a4
			 0x829aca71, /// 0x9a8
			 0x07ad8cd3, /// 0x9ac
			 0x4aa4a809, /// 0x9b0
			 0x4e33123f, /// 0x9b4
			 0x38e8c38c, /// 0x9b8
			 0xd31762ff, /// 0x9bc
			 0x03db7d89, /// 0x9c0
			 0xc586a0c4, /// 0x9c4
			 0x4b682781, /// 0x9c8
			 0xf2d99c37, /// 0x9cc
			 0x2fc28453, /// 0x9d0
			 0x48078e58, /// 0x9d4
			 0xf44b00f7, /// 0x9d8
			 0x3a3a7d06, /// 0x9dc
			 0x787d067e, /// 0x9e0
			 0xb23612ca, /// 0x9e4
			 0xc8427bea, /// 0x9e8
			 0x9adc8385, /// 0x9ec
			 0x14956c9f, /// 0x9f0
			 0x6e779b35, /// 0x9f4
			 0xe8459a81, /// 0x9f8
			 0x8480142c, /// 0x9fc
			 0x15a2c1be, /// 0xa00
			 0x007d0246, /// 0xa04
			 0xe3bc1815, /// 0xa08
			 0xa400487a, /// 0xa0c
			 0x75e027f8, /// 0xa10
			 0x596fdcfb, /// 0xa14
			 0x2e1f719c, /// 0xa18
			 0xda306b03, /// 0xa1c
			 0x3363528e, /// 0xa20
			 0x8f48f271, /// 0xa24
			 0xad119588, /// 0xa28
			 0x2071d564, /// 0xa2c
			 0xbe6ef75e, /// 0xa30
			 0x59841741, /// 0xa34
			 0x5ff119f9, /// 0xa38
			 0xa0e1246c, /// 0xa3c
			 0x6c436b5b, /// 0xa40
			 0xfb4d6251, /// 0xa44
			 0x3f87f44c, /// 0xa48
			 0x6a359b2e, /// 0xa4c
			 0x8f581f5a, /// 0xa50
			 0x174a9f5c, /// 0xa54
			 0xd0911324, /// 0xa58
			 0x0ad205dd, /// 0xa5c
			 0xb53faf95, /// 0xa60
			 0x32feada9, /// 0xa64
			 0x5a4e617b, /// 0xa68
			 0xb8ba6921, /// 0xa6c
			 0x724e552b, /// 0xa70
			 0x0347649d, /// 0xa74
			 0x4691126f, /// 0xa78
			 0x93e01184, /// 0xa7c
			 0xae6ba9cb, /// 0xa80
			 0x4163309e, /// 0xa84
			 0x55ef766d, /// 0xa88
			 0xcf3515cc, /// 0xa8c
			 0x8458926c, /// 0xa90
			 0x2bce2d6a, /// 0xa94
			 0xbe5e5440, /// 0xa98
			 0x698ba4c7, /// 0xa9c
			 0x0a3959ff, /// 0xaa0
			 0x6004f639, /// 0xaa4
			 0xca2cd008, /// 0xaa8
			 0x2be76429, /// 0xaac
			 0x16285fbc, /// 0xab0
			 0xa18df0db, /// 0xab4
			 0x7cbf82d4, /// 0xab8
			 0x86db6ceb, /// 0xabc
			 0x44f6a940, /// 0xac0
			 0x55300388, /// 0xac4
			 0xd06ed8cd, /// 0xac8
			 0xa1b0e0ce, /// 0xacc
			 0xeebe9f19, /// 0xad0
			 0x2bfe3d04, /// 0xad4
			 0x04e902df, /// 0xad8
			 0x73e4b1d7, /// 0xadc
			 0x0daae71a, /// 0xae0
			 0x21ec6c8a, /// 0xae4
			 0x627babd8, /// 0xae8
			 0xc7e0ee9e, /// 0xaec
			 0x8bef396a, /// 0xaf0
			 0x85443d21, /// 0xaf4
			 0x5cb85f7d, /// 0xaf8
			 0x07c34d6e, /// 0xafc
			 0xe565e9b7, /// 0xb00
			 0xcfa73940, /// 0xb04
			 0x254d7dd6, /// 0xb08
			 0x7c151457, /// 0xb0c
			 0x6e740c83, /// 0xb10
			 0x7c874f23, /// 0xb14
			 0x0c2ae18a, /// 0xb18
			 0x1890834f, /// 0xb1c
			 0x30e88616, /// 0xb20
			 0xa375f234, /// 0xb24
			 0x4c65ba99, /// 0xb28
			 0x78947e3e, /// 0xb2c
			 0x1b35f20a, /// 0xb30
			 0xd1c42c40, /// 0xb34
			 0xb634ebc1, /// 0xb38
			 0xa0fe74fd, /// 0xb3c
			 0x2699b8f7, /// 0xb40
			 0x16b5eb6e, /// 0xb44
			 0x91e4fb00, /// 0xb48
			 0xa9da16fe, /// 0xb4c
			 0x117ce31f, /// 0xb50
			 0x034b67f0, /// 0xb54
			 0x7fcf3c30, /// 0xb58
			 0xe13c4fa1, /// 0xb5c
			 0xd45ade03, /// 0xb60
			 0xe0937add, /// 0xb64
			 0x35fffb2d, /// 0xb68
			 0x45e36d1c, /// 0xb6c
			 0x1c2ef808, /// 0xb70
			 0x9d714818, /// 0xb74
			 0xf2024167, /// 0xb78
			 0x61996d3f, /// 0xb7c
			 0x8e9f154a, /// 0xb80
			 0xcac0ad9a, /// 0xb84
			 0xb3f4db56, /// 0xb88
			 0xe0f36b79, /// 0xb8c
			 0xb661266c, /// 0xb90
			 0x1084e7e1, /// 0xb94
			 0xb4246ec6, /// 0xb98
			 0x7a1067e1, /// 0xb9c
			 0xb5ff0dea, /// 0xba0
			 0x72975994, /// 0xba4
			 0x5894b3bb, /// 0xba8
			 0xf91b821f, /// 0xbac
			 0xe6a128c9, /// 0xbb0
			 0xb6dea737, /// 0xbb4
			 0xe678fbd6, /// 0xbb8
			 0xa07a429c, /// 0xbbc
			 0x71162b57, /// 0xbc0
			 0x897c7303, /// 0xbc4
			 0xa61d1c64, /// 0xbc8
			 0x2fea3f7e, /// 0xbcc
			 0x108d7923, /// 0xbd0
			 0xb794d82d, /// 0xbd4
			 0x2005c96e, /// 0xbd8
			 0xbf0a6e02, /// 0xbdc
			 0xc74d25a4, /// 0xbe0
			 0xe1baedc1, /// 0xbe4
			 0x2c3e5685, /// 0xbe8
			 0xc29489d3, /// 0xbec
			 0xe1937113, /// 0xbf0
			 0x32bacbeb, /// 0xbf4
			 0x8744ac16, /// 0xbf8
			 0x0bfb3f8a, /// 0xbfc
			 0xbf621765, /// 0xc00
			 0x96582070, /// 0xc04
			 0x7259f463, /// 0xc08
			 0xe9af47d6, /// 0xc0c
			 0xad8797d6, /// 0xc10
			 0x7db333ce, /// 0xc14
			 0x2305cf5a, /// 0xc18
			 0x5a2d9d08, /// 0xc1c
			 0xadd9e518, /// 0xc20
			 0xf4ca5232, /// 0xc24
			 0xba81fdb2, /// 0xc28
			 0xbe4c43a8, /// 0xc2c
			 0x52938e49, /// 0xc30
			 0x714e37c8, /// 0xc34
			 0x622bddf9, /// 0xc38
			 0xe8a32b0a, /// 0xc3c
			 0x9f8fc08f, /// 0xc40
			 0x12c72031, /// 0xc44
			 0xe516ddcc, /// 0xc48
			 0x2e27e538, /// 0xc4c
			 0xf4db3a75, /// 0xc50
			 0xed1aa816, /// 0xc54
			 0xa27058f5, /// 0xc58
			 0xb9adf284, /// 0xc5c
			 0xf06938cf, /// 0xc60
			 0x5a1e6e7c, /// 0xc64
			 0x93c75407, /// 0xc68
			 0xf045541d, /// 0xc6c
			 0x63f2f631, /// 0xc70
			 0xa92e4cbf, /// 0xc74
			 0x24e94be5, /// 0xc78
			 0x48e7b7f1, /// 0xc7c
			 0xb64f0542, /// 0xc80
			 0x5d759b19, /// 0xc84
			 0xb004380c, /// 0xc88
			 0x9cd1460c, /// 0xc8c
			 0xb843ca39, /// 0xc90
			 0x8b50b9d2, /// 0xc94
			 0x51bc2add, /// 0xc98
			 0x321d5f71, /// 0xc9c
			 0x27eb381e, /// 0xca0
			 0x93f0d505, /// 0xca4
			 0xcb0605d2, /// 0xca8
			 0x26903cb3, /// 0xcac
			 0x25904ba1, /// 0xcb0
			 0x3a7776f4, /// 0xcb4
			 0xd0088363, /// 0xcb8
			 0x2773c82a, /// 0xcbc
			 0xbf65f963, /// 0xcc0
			 0x632f33b2, /// 0xcc4
			 0x0fe99835, /// 0xcc8
			 0x7d5d5514, /// 0xccc
			 0x04395796, /// 0xcd0
			 0xe6a9501e, /// 0xcd4
			 0xee448b1c, /// 0xcd8
			 0xa07d2851, /// 0xcdc
			 0x417f502f, /// 0xce0
			 0x16e8c38a, /// 0xce4
			 0xeb75f8ce, /// 0xce8
			 0x94c982f1, /// 0xcec
			 0xe2761a76, /// 0xcf0
			 0xaf573061, /// 0xcf4
			 0x5b4d7215, /// 0xcf8
			 0x55e1cd39, /// 0xcfc
			 0x1eaddc5a, /// 0xd00
			 0x168eeaa2, /// 0xd04
			 0x465bcc75, /// 0xd08
			 0xcdc610d4, /// 0xd0c
			 0x2387bcec, /// 0xd10
			 0x60884237, /// 0xd14
			 0xdfe622dc, /// 0xd18
			 0xd8fd14c3, /// 0xd1c
			 0x6867fd27, /// 0xd20
			 0xbd46d7aa, /// 0xd24
			 0x38666824, /// 0xd28
			 0x014815a8, /// 0xd2c
			 0x28203075, /// 0xd30
			 0x676d633d, /// 0xd34
			 0x03d9d60c, /// 0xd38
			 0xa9d44aa0, /// 0xd3c
			 0x16ee045b, /// 0xd40
			 0x074b4726, /// 0xd44
			 0xaafbde72, /// 0xd48
			 0x700c69cf, /// 0xd4c
			 0x1636f8bc, /// 0xd50
			 0xe3e8909f, /// 0xd54
			 0x3d649e4b, /// 0xd58
			 0xbcadfc96, /// 0xd5c
			 0x924793ce, /// 0xd60
			 0x6bbe2eb5, /// 0xd64
			 0x8162c78c, /// 0xd68
			 0xee12b6b1, /// 0xd6c
			 0xb4b0a6f0, /// 0xd70
			 0xf5a408b5, /// 0xd74
			 0x8160d06a, /// 0xd78
			 0xf4cc348c, /// 0xd7c
			 0xd46ed758, /// 0xd80
			 0xe502e1ef, /// 0xd84
			 0x7a70e7fd, /// 0xd88
			 0x9b543065, /// 0xd8c
			 0x6f208eab, /// 0xd90
			 0xa60a995c, /// 0xd94
			 0x7b281e08, /// 0xd98
			 0xef18e7be, /// 0xd9c
			 0x6ebfaff6, /// 0xda0
			 0x4d2c4ec2, /// 0xda4
			 0x5205142b, /// 0xda8
			 0x975cbe28, /// 0xdac
			 0x559f0844, /// 0xdb0
			 0xecb9500a, /// 0xdb4
			 0x78570b4d, /// 0xdb8
			 0x2d398ee1, /// 0xdbc
			 0x22e6a269, /// 0xdc0
			 0xb9bc345c, /// 0xdc4
			 0x2357faa7, /// 0xdc8
			 0x3c77ba1e, /// 0xdcc
			 0x2364512d, /// 0xdd0
			 0xc5c6d9c1, /// 0xdd4
			 0x63ceb209, /// 0xdd8
			 0x6b40bfcc, /// 0xddc
			 0x3d2e1e59, /// 0xde0
			 0x3d12f59f, /// 0xde4
			 0x87c54426, /// 0xde8
			 0x983f2b9b, /// 0xdec
			 0xd335c3ba, /// 0xdf0
			 0x7f29f39b, /// 0xdf4
			 0x5bb4ae97, /// 0xdf8
			 0xee90ec3f, /// 0xdfc
			 0xbc75b272, /// 0xe00
			 0x23d80f08, /// 0xe04
			 0x60f0f026, /// 0xe08
			 0x6de0011b, /// 0xe0c
			 0x0917a766, /// 0xe10
			 0x10cf5aa6, /// 0xe14
			 0x98c2f0e5, /// 0xe18
			 0x21858579, /// 0xe1c
			 0xda3e0b72, /// 0xe20
			 0xc86f532f, /// 0xe24
			 0x03d4748c, /// 0xe28
			 0x901c5f12, /// 0xe2c
			 0xab2831ad, /// 0xe30
			 0x9ae20926, /// 0xe34
			 0xf4f8d634, /// 0xe38
			 0x79d7f4da, /// 0xe3c
			 0x0418a993, /// 0xe40
			 0x1b45c2ab, /// 0xe44
			 0x5e6deeb5, /// 0xe48
			 0xdeb1a20c, /// 0xe4c
			 0x239d03c6, /// 0xe50
			 0x0e7c01ae, /// 0xe54
			 0x00f62c11, /// 0xe58
			 0x399c65d6, /// 0xe5c
			 0x0332047e, /// 0xe60
			 0x04658f6c, /// 0xe64
			 0xe73d6edd, /// 0xe68
			 0xc64998d0, /// 0xe6c
			 0x0a30fe00, /// 0xe70
			 0xb65caf23, /// 0xe74
			 0x9150b031, /// 0xe78
			 0x47bf1a5f, /// 0xe7c
			 0xccb0cbb8, /// 0xe80
			 0xe8842e23, /// 0xe84
			 0x81217033, /// 0xe88
			 0x53b5a758, /// 0xe8c
			 0x1367e52e, /// 0xe90
			 0x9b48a4dc, /// 0xe94
			 0xd737ee58, /// 0xe98
			 0x18c10156, /// 0xe9c
			 0x5dc9d05a, /// 0xea0
			 0x462eb45f, /// 0xea4
			 0xb4c32e95, /// 0xea8
			 0x8e229b62, /// 0xeac
			 0x0da7a085, /// 0xeb0
			 0xbbbdcde8, /// 0xeb4
			 0x2e8ec573, /// 0xeb8
			 0xc745152f, /// 0xebc
			 0x509100f1, /// 0xec0
			 0xa052c927, /// 0xec4
			 0xe62fd33a, /// 0xec8
			 0x1131c5b5, /// 0xecc
			 0x14895373, /// 0xed0
			 0xd22aecb6, /// 0xed4
			 0x6c651553, /// 0xed8
			 0x543896e0, /// 0xedc
			 0x3d765e44, /// 0xee0
			 0x18020cbf, /// 0xee4
			 0x987bcceb, /// 0xee8
			 0xdf4e0945, /// 0xeec
			 0xd453c3f9, /// 0xef0
			 0xef0384dc, /// 0xef4
			 0xb8d28727, /// 0xef8
			 0x25afc798, /// 0xefc
			 0x0248b458, /// 0xf00
			 0x805febbb, /// 0xf04
			 0x7e013fa0, /// 0xf08
			 0x2f616dd8, /// 0xf0c
			 0x1b8befde, /// 0xf10
			 0xca08e6fd, /// 0xf14
			 0x49aade95, /// 0xf18
			 0x35bac6ff, /// 0xf1c
			 0x8adb3167, /// 0xf20
			 0xbce89f98, /// 0xf24
			 0x6f789ead, /// 0xf28
			 0x532ee3f9, /// 0xf2c
			 0x74cbdead, /// 0xf30
			 0x49462f3f, /// 0xf34
			 0x7b306809, /// 0xf38
			 0xf9b61581, /// 0xf3c
			 0x411752a6, /// 0xf40
			 0x63dfbd98, /// 0xf44
			 0x74d44110, /// 0xf48
			 0x479c74da, /// 0xf4c
			 0xb80d1a31, /// 0xf50
			 0xbe93ce6b, /// 0xf54
			 0x95759405, /// 0xf58
			 0x609f0622, /// 0xf5c
			 0x8b95d69d, /// 0xf60
			 0x207c5744, /// 0xf64
			 0xa6ce7f57, /// 0xf68
			 0x27d703d4, /// 0xf6c
			 0x175dc872, /// 0xf70
			 0x47cf165e, /// 0xf74
			 0x3d17dbc7, /// 0xf78
			 0x6418804d, /// 0xf7c
			 0xfbe42ed7, /// 0xf80
			 0x57787394, /// 0xf84
			 0x8a4a9cd1, /// 0xf88
			 0x4113ac3e, /// 0xf8c
			 0xb522d2e8, /// 0xf90
			 0xf09b3b2d, /// 0xf94
			 0xda3c77af, /// 0xf98
			 0x06877a38, /// 0xf9c
			 0x3e72d35d, /// 0xfa0
			 0xfbde6a7d, /// 0xfa4
			 0xe1710bfb, /// 0xfa8
			 0x664b3c99, /// 0xfac
			 0x5d4e8bfd, /// 0xfb0
			 0x4ead43fe, /// 0xfb4
			 0x4c9093bb, /// 0xfb8
			 0x67576229, /// 0xfbc
			 0x06c0a2ff, /// 0xfc0
			 0x4430974e, /// 0xfc4
			 0xdcc544c8, /// 0xfc8
			 0x1ab237a4, /// 0xfcc
			 0x4bd027b9, /// 0xfd0
			 0x7d017f72, /// 0xfd4
			 0x9bf8d10f, /// 0xfd8
			 0x70d42723, /// 0xfdc
			 0x7145259f, /// 0xfe0
			 0x286cdfd8, /// 0xfe4
			 0xf259e615, /// 0xfe8
			 0x9aaa9424, /// 0xfec
			 0x3bdeb690, /// 0xff0
			 0xcd83cc23, /// 0xff4
			 0x223bebe0, /// 0xff8
			 0xa194502b /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0000c
			 0xffc00001,                                                  // -signaling NaN                              /// 00010
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00014
			 0x80011111,                                                  // -9.7958E-41                                 /// 00018
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00020
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00024
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0002c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00030
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00034
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0003c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00040
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x00000000,                                                  // zero                                        /// 0004c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00050
			 0x80011111,                                                  // -9.7958E-41                                 /// 00054
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0005c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00060
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00064
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00068
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00070
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00074
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00078
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0007c
			 0xffc00001,                                                  // -signaling NaN                              /// 00080
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0008c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00090
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00098
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0009c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000a8
			 0x33333333,                                                  // 4 random values                             /// 000ac
			 0x7f800000,                                                  // inf                                         /// 000b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 000c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 000f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00100
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00104
			 0x0c700000,                                                  // Leading 1s:                                 /// 00108
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0010c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00110
			 0x0e000003,                                                  // Trailing 1s:                                /// 00114
			 0xbf028f5c,                                                  // -0.51                                       /// 00118
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0011c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00120
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00124
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0012c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00130
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00134
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00138
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0013c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00148
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0014c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00150
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00154
			 0x80000000,                                                  // -zero                                       /// 00158
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0015c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00160
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x0c780000,                                                  // Leading 1s:                                 /// 00168
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0016c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00170
			 0x0c400000,                                                  // Leading 1s:                                 /// 00174
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00180
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00184
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0018c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00190
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00194
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 001f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00204
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0xcb000000,                                                  // -8388608.0                                  /// 0020c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00210
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00218
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0021c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00224
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00228
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00230
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00234
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00240
			 0x7f800000,                                                  // inf                                         /// 00244
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0024c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00250
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00254
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00258
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0025c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00268
			 0x00000000,                                                  // zero                                        /// 0026c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00270
			 0x4b000000,                                                  // 8388608.0                                   /// 00274
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00278
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00280
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00284
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00288
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00290
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00298
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0029c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00304
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x80000000,                                                  // -zero                                       /// 0030c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00314
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0031c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00320
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0c600000,                                                  // Leading 1s:                                 /// 00328
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0032c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00330
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00334
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0xbf028f5c,                                                  // -0.51                                       /// 0033c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00340
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0034c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00358
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0035c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x80011111,                                                  // -9.7958E-41                                 /// 00364
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0036c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00370
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00374
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00378
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00380
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00384
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00388
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0038c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00390
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00394
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00398
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003a0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 003dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 003ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003f8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00400
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00408
			 0x7f800000,                                                  // inf                                         /// 0040c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0xcb000000,                                                  // -8388608.0                                  /// 00414
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00418
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0041c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00420
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00424
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0042c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00430
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00438
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0043c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00440
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00444
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00448
			 0x80011111,                                                  // -9.7958E-41                                 /// 0044c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00454
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0045c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00460
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0046c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00474
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00478
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0047c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00480
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00484
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0048c
			 0x7f800000,                                                  // inf                                         /// 00490
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00498
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0049c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x4b000000,                                                  // 8388608.0                                   /// 004b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004bc
			 0xffc00001,                                                  // -signaling NaN                              /// 004c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004f8
			 0x7fc00001,                                                  // signaling NaN                               /// 004fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00500
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00504
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0050c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00510
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00514
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00518
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0051c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00520
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00524
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0052c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00530
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00534
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0e000007,                                                  // Trailing 1s:                                /// 0053c
			 0xbf028f5c,                                                  // -0.51                                       /// 00540
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00544
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00554
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00558
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00568
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0056c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00570
			 0x7fc00001,                                                  // signaling NaN                               /// 00574
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00578
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0057c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00580
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00588
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00590
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00594
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0059c
			 0x7fc00001,                                                  // signaling NaN                               /// 005a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0xbf028f5c,                                                  // -0.51                                       /// 005b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005cc
			 0x4b000000,                                                  // 8388608.0                                   /// 005d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 005f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00600
			 0xcb000000,                                                  // -8388608.0                                  /// 00604
			 0x0c400000,                                                  // Leading 1s:                                 /// 00608
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00610
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00614
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00620
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00624
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00628
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0062c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00630
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00634
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0e000003,                                                  // Trailing 1s:                                /// 0063c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00644
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00648
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00650
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00658
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0065c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00660
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00664
			 0x0c600000,                                                  // Leading 1s:                                 /// 00668
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0066c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00670
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00678
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00680
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00684
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00688
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x0e000003,                                                  // Trailing 1s:                                /// 00698
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006ac
			 0xff800000,                                                  // -inf                                        /// 006b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00700
			 0x0c780000,                                                  // Leading 1s:                                 /// 00704
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00708
			 0x0e000003,                                                  // Trailing 1s:                                /// 0070c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00718
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0071c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00720
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00724
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0072c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00730
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00738
			 0x3f028f5c,                                                  // 0.51                                        /// 0073c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x00011111,                                                  // 9.7958E-41                                  /// 00744
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x55555555,                                                  // 4 random values                             /// 00758
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00760
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0076c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00770
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00778
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0077c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00780
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00784
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0078c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00790
			 0x4b000000,                                                  // 8388608.0                                   /// 00794
			 0x00011111,                                                  // 9.7958E-41                                  /// 00798
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0079c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007a8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0xff800000,                                                  // -inf                                        /// 007d8
			 0x80000000,                                                  // -zero                                       /// 007dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007e0
			 0x55555555,                                                  // 4 random values                             /// 007e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007fc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00804
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00808
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0xff800000,                                                  // -inf                                        /// 00810
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00820
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00824
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0082c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00830
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00838
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00844
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00848
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00858
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0085c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00864
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00868
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0086c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00874
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0087c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00880
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00884
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00888
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0088c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00890
			 0x00011111,                                                  // 9.7958E-41                                  /// 00894
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00898
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0089c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008cc
			 0x7f800000,                                                  // inf                                         /// 008d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008e8
			 0x4b000000,                                                  // 8388608.0                                   /// 008ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00900
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00904
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00910
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00914
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00918
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0091c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00920
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00924
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00928
			 0x0e000003,                                                  // Trailing 1s:                                /// 0092c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00930
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00934
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0093c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00940
			 0xff800000,                                                  // -inf                                        /// 00944
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0094c
			 0xffc00001,                                                  // -signaling NaN                              /// 00950
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00954
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0095c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00960
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00964
			 0x0e000003,                                                  // Trailing 1s:                                /// 00968
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00970
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00974
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00978
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0097c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00984
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00988
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0098c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00990
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0099c
			 0xff800000,                                                  // -inf                                        /// 009a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009b0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00000000,                                                  // zero                                        /// 009d0
			 0x00000000,                                                  // zero                                        /// 009d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009e0
			 0x80000000,                                                  // -zero                                       /// 009e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 009e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a04
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a0c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a2c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a38
			 0x7f800000,                                                  // inf                                         /// 00a3c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a4c
			 0x7f800000,                                                  // inf                                         /// 00a50
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a58
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a5c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a60
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a68
			 0xffc00001,                                                  // -signaling NaN                              /// 00a6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a80
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a84
			 0x33333333,                                                  // 4 random values                             /// 00a88
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a94
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a98
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a9c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00aa0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00aa4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00aa8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00aac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ab8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ac4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ac8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00acc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ad8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0xff800000,                                                  // -inf                                        /// 00ae0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00aec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00af0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b20
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b30
			 0xcb000000,                                                  // -8388608.0                                  /// 00b34
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b4c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b50
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b54
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b64
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b74
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b78
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b80
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b90
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b94
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b9c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ba0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ba8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bb4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bb8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0xffc00001,                                                  // -signaling NaN                              /// 00bc4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bc8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bd0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bd4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bd8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00be0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00be4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bf4
			 0x33333333,                                                  // 4 random values                             /// 00bf8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c00
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c14
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x3f028f5c,                                                  // 0.51                                        /// 00c24
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c28
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c30
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c38
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c48
			 0xbf028f5c,                                                  // -0.51                                       /// 00c4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c50
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c54
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c5c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c68
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c80
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c84
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c88
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c8c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ca4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ca8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cb4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cb8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x7f800000,                                                  // inf                                         /// 00cc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cc4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cc8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x80000000,                                                  // -zero                                       /// 00cd0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cd4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cd8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cdc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ce0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ce4
			 0x00000000,                                                  // zero                                        /// 00ce8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cf0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cf4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cf8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cfc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d04
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d10
			 0xcb000000,                                                  // -8388608.0                                  /// 00d14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d38
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d44
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d4c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d58
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d5c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d74
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d88
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d8c
			 0x7f800000,                                                  // inf                                         /// 00d90
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d98
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00db0
			 0x7f800000,                                                  // inf                                         /// 00db4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dbc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dc0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dc4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dc8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dcc
			 0xbf028f5c,                                                  // -0.51                                       /// 00dd0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dd8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ddc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00de8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00df4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00df8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dfc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e04
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e0c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e18
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80000000,                                                  // -zero                                       /// 00e28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e30
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e38
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e4c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e64
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x55555555,                                                  // 4 random values                             /// 00e78
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e88
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ea4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ea8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00eac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00eb0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eb8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ebc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ec0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ec4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ec8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ed0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ed4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ee0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ee8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ef0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ef8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f10
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f3c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f48
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f4c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f54
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f60
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f74
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f78
			 0x33333333,                                                  // 4 random values                             /// 00f7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f88
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f98
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f9c
			 0x80000000,                                                  // -zero                                       /// 00fa0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fa4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fa8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fb0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fbc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fc0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fc8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fd4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fd8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fdc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fe0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fe4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ff0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ff4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0e007fff                                                  // Trailing 1s:                                /// last
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
			 0x00000368,
			 0x000004a0,
			 0x00000440,
			 0x00000530,
			 0x00000710,
			 0x00000260,
			 0x0000039c,
			 0x000006b8,

			 /// vpu register f2
			 0x41d80000,
			 0x40c00000,
			 0x41400000,
			 0x40000000,
			 0x42000000,
			 0x41600000,
			 0x41e00000,
			 0x40000000,

			 /// vpu register f3
			 0x41700000,
			 0x41a00000,
			 0x41f80000,
			 0x41900000,
			 0x41f00000,
			 0x42000000,
			 0x40400000,
			 0x41600000,

			 /// vpu register f4
			 0x3f800000,
			 0x41400000,
			 0x41100000,
			 0x40e00000,
			 0x41f80000,
			 0x41500000,
			 0x41500000,
			 0x40c00000,

			 /// vpu register f5
			 0x41a00000,
			 0x41f80000,
			 0x41e00000,
			 0x41000000,
			 0x3f800000,
			 0x3f800000,
			 0x40e00000,
			 0x40e00000,

			 /// vpu register f6
			 0x41e80000,
			 0x41b00000,
			 0x41e80000,
			 0x41980000,
			 0x41c80000,
			 0x41f80000,
			 0x41400000,
			 0x41e80000,

			 /// vpu register f7
			 0x41f00000,
			 0x41b00000,
			 0x41200000,
			 0x40000000,
			 0x41b80000,
			 0x40000000,
			 0x41a00000,
			 0x41f80000,

			 /// vpu register f8
			 0x40e00000,
			 0x41e80000,
			 0x41800000,
			 0x41000000,
			 0x41200000,
			 0x41400000,
			 0x40a00000,
			 0x41980000,

			 /// vpu register f9
			 0x41700000,
			 0x40400000,
			 0x42000000,
			 0x41500000,
			 0x41100000,
			 0x3f800000,
			 0x41e80000,
			 0x41800000,

			 /// vpu register f10
			 0x41a00000,
			 0x41c00000,
			 0x41a80000,
			 0x41900000,
			 0x41a00000,
			 0x41b80000,
			 0x41400000,
			 0x41b80000,

			 /// vpu register f11
			 0x41700000,
			 0x41d00000,
			 0x41900000,
			 0x40000000,
			 0x40400000,
			 0x3f800000,
			 0x41f00000,
			 0x41880000,

			 /// vpu register f12
			 0x41100000,
			 0x41800000,
			 0x3f800000,
			 0x41300000,
			 0x41a00000,
			 0x41b00000,
			 0x42000000,
			 0x41100000,

			 /// vpu register f13
			 0x41880000,
			 0x41600000,
			 0x40800000,
			 0x40400000,
			 0x41a80000,
			 0x41e00000,
			 0x40a00000,
			 0x41c00000,

			 /// vpu register f14
			 0x41300000,
			 0x40400000,
			 0x41b00000,
			 0x40e00000,
			 0x41900000,
			 0x41b80000,
			 0x41600000,
			 0x41100000,

			 /// vpu register f15
			 0x40e00000,
			 0x41e80000,
			 0x42000000,
			 0x40c00000,
			 0x41e00000,
			 0x41700000,
			 0x41880000,
			 0x41800000,

			 /// vpu register f16
			 0x41880000,
			 0x42000000,
			 0x42000000,
			 0x41600000,
			 0x41d00000,
			 0x41600000,
			 0x40800000,
			 0x40c00000,

			 /// vpu register f17
			 0x41980000,
			 0x41b00000,
			 0x41c80000,
			 0x41f00000,
			 0x40800000,
			 0x41500000,
			 0x41e00000,
			 0x41600000,

			 /// vpu register f18
			 0x41880000,
			 0x41400000,
			 0x41880000,
			 0x40800000,
			 0x41c00000,
			 0x41a80000,
			 0x41d00000,
			 0x41c80000,

			 /// vpu register f19
			 0x41b00000,
			 0x3f800000,
			 0x41c00000,
			 0x41980000,
			 0x41000000,
			 0x41b00000,
			 0x41d80000,
			 0x41f00000,

			 /// vpu register f20
			 0x41300000,
			 0x41b80000,
			 0x41300000,
			 0x40800000,
			 0x41e00000,
			 0x40400000,
			 0x41500000,
			 0x41800000,

			 /// vpu register f21
			 0x41c00000,
			 0x41100000,
			 0x41100000,
			 0x41f80000,
			 0x40000000,
			 0x41b80000,
			 0x41800000,
			 0x41700000,

			 /// vpu register f22
			 0x41d80000,
			 0x40e00000,
			 0x40400000,
			 0x41a00000,
			 0x41400000,
			 0x40c00000,
			 0x41e80000,
			 0x41e80000,

			 /// vpu register f23
			 0x41800000,
			 0x41300000,
			 0x40e00000,
			 0x41a80000,
			 0x41500000,
			 0x41980000,
			 0x41100000,
			 0x42000000,

			 /// vpu register f24
			 0x40a00000,
			 0x41980000,
			 0x41b80000,
			 0x41000000,
			 0x40e00000,
			 0x41a80000,
			 0x41f00000,
			 0x40800000,

			 /// vpu register f25
			 0x41e00000,
			 0x41a00000,
			 0x41600000,
			 0x41b80000,
			 0x41200000,
			 0x41c00000,
			 0x40c00000,
			 0x41980000,

			 /// vpu register f26
			 0x41500000,
			 0x41100000,
			 0x40000000,
			 0x40a00000,
			 0x41e00000,
			 0x41400000,
			 0x41900000,
			 0x41c80000,

			 /// vpu register f27
			 0x41e80000,
			 0x41900000,
			 0x40400000,
			 0x41a00000,
			 0x41980000,
			 0x41c00000,
			 0x41000000,
			 0x3f800000,

			 /// vpu register f28
			 0x40c00000,
			 0x3f800000,
			 0x41900000,
			 0x42000000,
			 0x41880000,
			 0x41b00000,
			 0x41980000,
			 0x41e80000,

			 /// vpu register f29
			 0x41400000,
			 0x41d00000,
			 0x41400000,
			 0x41b00000,
			 0x3f800000,
			 0x41b80000,
			 0x41d80000,
			 0x41b00000,

			 /// vpu register f30
			 0x40800000,
			 0x41800000,
			 0x41200000,
			 0x40c00000,
			 0x41b80000,
			 0x41980000,
			 0x41300000,
			 0x41000000,

			 /// vpu register f31
			 0x41d80000,
			 0x40c00000,
			 0x41800000,
			 0x41e80000,
			 0x41d00000,
			 0x41b80000,
			 0x41700000,
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
		"flwg.ps f18, (x13)\n"                                ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f20, (x14)\n"                                ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f26, (x12)\n"                                ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f23, (x11)\n"                                ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f24, (x12)\n"                                ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f2, (x14)\n"                                 ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f8, (x15)\n"                                 ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f4, (x14)\n"                                 ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f7, (x11)\n"                                 ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f25, (x13)\n"                                ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f22, (x11)\n"                                ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f26, (x14)\n"                                ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f26, (x14)\n"                                ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f30, (x15)\n"                                ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f21, (x12)\n"                                ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f28, (x15)\n"                                ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f22, (x12)\n"                                ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f11, (x15)\n"                                ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f12, (x14)\n"                                ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f19, (x13)\n"                                ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f20, (x12)\n"                                ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f16, (x12)\n"                                ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f20, (x15)\n"                                ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f23, (x11)\n"                                ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f19, (x15)\n"                                ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f12, (x14)\n"                                ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f8, (x15)\n"                                 ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f27, (x12)\n"                                ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f15, (x11)\n"                                ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f10, (x12)\n"                                ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f24, (x13)\n"                                ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f26, (x13)\n"                                ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f7, (x12)\n"                                 ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f30, (x15)\n"                                ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f13, (x15)\n"                                ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f17, (x12)\n"                                ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f17, (x11)\n"                                ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f18, (x11)\n"                                ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f19, (x11)\n"                                ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f3, (x15)\n"                                 ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f29, (x11)\n"                                ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f7, (x12)\n"                                 ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f26, (x15)\n"                                ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f16, (x13)\n"                                ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f17, (x13)\n"                                ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f4, (x15)\n"                                 ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f7, (x13)\n"                                 ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f21, (x14)\n"                                ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f21, (x14)\n"                                ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f14, (x14)\n"                                ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f26, (x13)\n"                                ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f26, (x14)\n"                                ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f27, (x11)\n"                                ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f13, (x14)\n"                                ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f5, (x12)\n"                                 ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f19, (x14)\n"                                ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f10, (x11)\n"                                ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f22, (x13)\n"                                ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f6, (x14)\n"                                 ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f17, (x12)\n"                                ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f26, (x11)\n"                                ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f13, (x11)\n"                                ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f16, (x12)\n"                                ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f27, (x14)\n"                                ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f25, (x14)\n"                                ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f27, (x12)\n"                                ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f23, (x14)\n"                                ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f13, (x15)\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f20, (x15)\n"                                ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f27, (x14)\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f12, (x15)\n"                                ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f25, (x12)\n"                                ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f22, (x13)\n"                                ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f27, (x14)\n"                                ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f18, (x11)\n"                                ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f26, (x11)\n"                                ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f28, (x12)\n"                                ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f16, (x11)\n"                                ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f12, (x12)\n"                                ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f7, (x11)\n"                                 ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f5, (x15)\n"                                 ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f25, (x14)\n"                                ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f24, (x15)\n"                                ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f20, (x12)\n"                                ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f12, (x14)\n"                                ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f19, (x15)\n"                                ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f20, (x12)\n"                                ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f17, (x13)\n"                                ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f24, (x12)\n"                                ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f11, (x14)\n"                                ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f24, (x15)\n"                                ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f13, (x12)\n"                                ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f12, (x15)\n"                                ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f6, (x11)\n"                                 ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f5, (x11)\n"                                 ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f3, (x13)\n"                                 ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f23, (x15)\n"                                ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f28, (x13)\n"                                ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flwg.ps f7, (x15)\n"                                 ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
