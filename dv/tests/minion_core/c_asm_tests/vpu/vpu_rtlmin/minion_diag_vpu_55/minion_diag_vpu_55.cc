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
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00000
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00004
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00008
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00010
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00018
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00024
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00028
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0002c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x80011111,                                                  // -9.7958E-41                                 /// 00034
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0003c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00044
			 0xbf028f5c,                                                  // -0.51                                       /// 00048
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00058
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0005c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00060
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00064
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00068
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00070
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00078
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00080
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00084
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00090
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00094
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00098
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0009c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000a0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00100
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00104
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x3f028f5c,                                                  // 0.51                                        /// 00110
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00114
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00118
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0011c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00128
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0012c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00130
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00134
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0013c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00144
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00148
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0014c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00158
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0015c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00164
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00168
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0016c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00170
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00174
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00184
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00188
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0018c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00190
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00194
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00198
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001c8
			 0x3f028f5c,                                                  // 0.51                                        /// 001cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 001e8
			 0x00000000,                                                  // zero                                        /// 001ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 001f0
			 0x3f028f5c,                                                  // 0.51                                        /// 001f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 001fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00200
			 0x80000000,                                                  // -zero                                       /// 00204
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00208
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0020c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00210
			 0x00000000,                                                  // zero                                        /// 00214
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00218
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0021c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00220
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0x3f028f5c,                                                  // 0.51                                        /// 00228
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0022c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00230
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00234
			 0x0e000003,                                                  // Trailing 1s:                                /// 00238
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0023c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00248
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0024c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0e000007,                                                  // Trailing 1s:                                /// 00254
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00258
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0025c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00260
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00264
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00268
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0026c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00270
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00274
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x7f800000,                                                  // inf                                         /// 00280
			 0x3f028f5c,                                                  // 0.51                                        /// 00284
			 0x00000000,                                                  // zero                                        /// 00288
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0028c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00290
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00298
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0029c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002c4
			 0x00000000,                                                  // zero                                        /// 002c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00300
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00308
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0030c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00310
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00318
			 0x0c400000,                                                  // Leading 1s:                                 /// 0031c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00320
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00328
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00330
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00334
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00338
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0033c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00340
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00348
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0034c
			 0x7fc00001,                                                  // signaling NaN                               /// 00350
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00354
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00358
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00360
			 0x00011111,                                                  // 9.7958E-41                                  /// 00364
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00368
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00370
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00374
			 0x80011111,                                                  // -9.7958E-41                                 /// 00378
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0037c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00380
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x00000000,                                                  // zero                                        /// 00390
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00394
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00398
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0xbf028f5c,                                                  // -0.51                                       /// 003a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x33333333,                                                  // 4 random values                             /// 003b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 003d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 003ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00400
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00404
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00408
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0040c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00410
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00414
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00418
			 0x0e000007,                                                  // Trailing 1s:                                /// 0041c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0042c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00430
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00434
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0043c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00440
			 0x0c400000,                                                  // Leading 1s:                                 /// 00444
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0044c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00450
			 0x7fc00001,                                                  // signaling NaN                               /// 00454
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0045c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00464
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00468
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00470
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00474
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00478
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00484
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0048c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00490
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00498
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004d4
			 0xcb000000,                                                  // -8388608.0                                  /// 004d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00500
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00504
			 0x3f028f5c,                                                  // 0.51                                        /// 00508
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0050c
			 0x7fc00001,                                                  // signaling NaN                               /// 00510
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0051c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00520
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00528
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0052c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00530
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00534
			 0x0c700000,                                                  // Leading 1s:                                 /// 00538
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0053c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00540
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00544
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00548
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0054c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0055c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00560
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00564
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0056c
			 0xcb000000,                                                  // -8388608.0                                  /// 00570
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00574
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0057c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00580
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00584
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0058c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00590
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x33333333,                                                  // 4 random values                             /// 00598
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0059c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005c0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005e0
			 0x4b000000,                                                  // 8388608.0                                   /// 005e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00600
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0060c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00610
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00614
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00618
			 0x00011111,                                                  // 9.7958E-41                                  /// 0061c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00620
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00628
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0062c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00634
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0063c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00640
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00644
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00648
			 0x0e000001,                                                  // Trailing 1s:                                /// 0064c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00650
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00654
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00658
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0065c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00664
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00668
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0066c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00670
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00674
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00678
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00684
			 0x80000000,                                                  // -zero                                       /// 00688
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0068c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00694
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00698
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0069c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 006b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006c0
			 0x80000000,                                                  // -zero                                       /// 006c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e0
			 0x7fc00001,                                                  // signaling NaN                               /// 006e4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006f0
			 0xffc00001,                                                  // -signaling NaN                              /// 006f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00700
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00704
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00710
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00714
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00718
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00720
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00728
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0072c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00730
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00734
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00740
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0074c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00754
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0075c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00760
			 0xff800000,                                                  // -inf                                        /// 00764
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00768
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0076c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00770
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00774
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00778
			 0x0e000007,                                                  // Trailing 1s:                                /// 0077c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00780
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00784
			 0x0c400000,                                                  // Leading 1s:                                 /// 00788
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0078c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00794
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00798
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x33333333,                                                  // 4 random values                             /// 007b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007b4
			 0xffc00001,                                                  // -signaling NaN                              /// 007b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00800
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00804
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0xcb000000,                                                  // -8388608.0                                  /// 0080c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00810
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0081c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00824
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00828
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0082c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00830
			 0x80000000,                                                  // -zero                                       /// 00834
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00840
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00844
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00848
			 0x55555555,                                                  // 4 random values                             /// 0084c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00858
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0085c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00860
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00868
			 0xbf028f5c,                                                  // -0.51                                       /// 0086c
			 0xcb000000,                                                  // -8388608.0                                  /// 00870
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00874
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0087c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00884
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0088c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00890
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00894
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00898
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0089c
			 0x0e000001,                                                  // Trailing 1s:                                /// 008a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008b4
			 0xffc00001,                                                  // -signaling NaN                              /// 008b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008e8
			 0xbf028f5c,                                                  // -0.51                                       /// 008ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x7fc00001,                                                  // signaling NaN                               /// 008f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008fc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00900
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00904
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00910
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00914
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00918
			 0x00011111,                                                  // 9.7958E-41                                  /// 0091c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00920
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00924
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00930
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00934
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00938
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0093c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00940
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00944
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00948
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0094c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00950
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00954
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00958
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00964
			 0x0e000003,                                                  // Trailing 1s:                                /// 00968
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0096c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00970
			 0x80011111,                                                  // -9.7958E-41                                 /// 00974
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0097c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00984
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00990
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00998
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009d4
			 0xbf028f5c,                                                  // -0.51                                       /// 009d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a08
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a0c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a18
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a1c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a2c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a40
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a44
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a5c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x7fc00001,                                                  // signaling NaN                               /// 00a6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a78
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a80
			 0x33333333,                                                  // 4 random values                             /// 00a84
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a8c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00aa0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00aa4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00aa8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ab0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ab4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0xbf028f5c,                                                  // -0.51                                       /// 00abc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ac4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ad0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0xffc00001,                                                  // -signaling NaN                              /// 00adc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ae0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00aec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00af0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00af8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00afc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b00
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x80000000,                                                  // -zero                                       /// 00b0c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b10
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x4b000000,                                                  // 8388608.0                                   /// 00b18
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b24
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b38
			 0xbf028f5c,                                                  // -0.51                                       /// 00b3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b4c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b6c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b78
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b88
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ba4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ba8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bb8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bcc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bd0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bd4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bdc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00be0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x3f028f5c,                                                  // 0.51                                        /// 00be8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bf4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c28
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c34
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c38
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c44
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c48
			 0xcb000000,                                                  // -8388608.0                                  /// 00c4c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c54
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c60
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c68
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c6c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c70
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c7c
			 0x55555555,                                                  // 4 random values                             /// 00c80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c88
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c8c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c90
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ca0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ca4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ca8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cb0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cb4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cb8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cbc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cc0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cc4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ccc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cd0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cd4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cd8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cdc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ce0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ce4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ce8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cf0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cf4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cfc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d18
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d1c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d20
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d30
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d50
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d5c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d68
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d6c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d70
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d80
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d84
			 0x33333333,                                                  // 4 random values                             /// 00d88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d8c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00da0
			 0x55555555,                                                  // 4 random values                             /// 00da4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00da8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00db0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00db4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00db8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dbc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dc0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dc8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dcc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dd4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ddc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00de0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00de4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00df4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00df8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dfc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x00000000,                                                  // zero                                        /// 00e10
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e20
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e34
			 0xbf028f5c,                                                  // -0.51                                       /// 00e38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e58
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e5c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e64
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e70
			 0xbf028f5c,                                                  // -0.51                                       /// 00e74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e80
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e84
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e8c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e94
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e9c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ea0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ea4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ea8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00eb0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00eb4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00eb8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ebc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ec8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ed0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x7f800000,                                                  // inf                                         /// 00edc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x00000000,                                                  // zero                                        /// 00ee4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ee8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00eec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ef0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ef4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00efc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f00
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f10
			 0x55555555,                                                  // 4 random values                             /// 00f14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x00000000,                                                  // zero                                        /// 00f20
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f34
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f3c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f4c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f58
			 0xcb000000,                                                  // -8388608.0                                  /// 00f5c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f6c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f74
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f88
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f8c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f90
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f94
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fa0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fa8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fb0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fb4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fb8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fc4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fc8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fcc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fd4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fd8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fdc
			 0xbf028f5c,                                                  // -0.51                                       /// 00fe0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fe8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ff0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ff4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ff8
			 0x0e0001ff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x6aeb05e4, /// 0x0
			 0x79525193, /// 0x4
			 0x5a010d93, /// 0x8
			 0xc8ee633d, /// 0xc
			 0xf2e94568, /// 0x10
			 0xdeec6744, /// 0x14
			 0x595f7395, /// 0x18
			 0xbf33789e, /// 0x1c
			 0x531a8bbd, /// 0x20
			 0xde96ad08, /// 0x24
			 0x7679c5a0, /// 0x28
			 0x964e7541, /// 0x2c
			 0x5be309e5, /// 0x30
			 0x649b5b66, /// 0x34
			 0x73c5c229, /// 0x38
			 0xf50a63b7, /// 0x3c
			 0x36a97c1a, /// 0x40
			 0x9fb9ef4b, /// 0x44
			 0x2b0c978c, /// 0x48
			 0xb70f3650, /// 0x4c
			 0x72173ef2, /// 0x50
			 0x93082fc6, /// 0x54
			 0x347b543e, /// 0x58
			 0xb2c395c2, /// 0x5c
			 0xfc4d7247, /// 0x60
			 0x834d4103, /// 0x64
			 0x5c93bcaf, /// 0x68
			 0x47ccab8d, /// 0x6c
			 0x51b54e26, /// 0x70
			 0xdd15ae68, /// 0x74
			 0x88c723c4, /// 0x78
			 0x9e663ddf, /// 0x7c
			 0x2fed428d, /// 0x80
			 0xec6d291c, /// 0x84
			 0x8335be9d, /// 0x88
			 0xdaccec56, /// 0x8c
			 0x238218ea, /// 0x90
			 0xcca59db2, /// 0x94
			 0x10a1f3d4, /// 0x98
			 0xd982816f, /// 0x9c
			 0x9c8d263f, /// 0xa0
			 0xaf5a70e4, /// 0xa4
			 0x426e8552, /// 0xa8
			 0xc1f02517, /// 0xac
			 0x1850688b, /// 0xb0
			 0xbed80ffd, /// 0xb4
			 0xbcc07f3a, /// 0xb8
			 0xceb6cea4, /// 0xbc
			 0xd5d49bd9, /// 0xc0
			 0x5b11bf24, /// 0xc4
			 0x3393af10, /// 0xc8
			 0x135f0c74, /// 0xcc
			 0xada1a5ae, /// 0xd0
			 0xe0f47f8e, /// 0xd4
			 0x50dc0edb, /// 0xd8
			 0x13fda4ea, /// 0xdc
			 0xb2f84227, /// 0xe0
			 0xc747b567, /// 0xe4
			 0x0a0955fb, /// 0xe8
			 0xaf76ec26, /// 0xec
			 0x00c7d586, /// 0xf0
			 0xe3b1026d, /// 0xf4
			 0x702106af, /// 0xf8
			 0xe3f91b02, /// 0xfc
			 0x7cb8d6e0, /// 0x100
			 0x08dc3dd0, /// 0x104
			 0x92b69187, /// 0x108
			 0xe8f21497, /// 0x10c
			 0x705a1c65, /// 0x110
			 0xb4a40464, /// 0x114
			 0x345be4ef, /// 0x118
			 0x8e1881b5, /// 0x11c
			 0x80645b19, /// 0x120
			 0xf65ad545, /// 0x124
			 0x32f66f90, /// 0x128
			 0x9d1b32ea, /// 0x12c
			 0x33868147, /// 0x130
			 0x09aa27b3, /// 0x134
			 0x3d70848a, /// 0x138
			 0xadb4bb5b, /// 0x13c
			 0x8f701d9c, /// 0x140
			 0x670ad494, /// 0x144
			 0xdadf5f1e, /// 0x148
			 0x67eaff53, /// 0x14c
			 0xa0d153fa, /// 0x150
			 0x91bb81ff, /// 0x154
			 0x37b6abbc, /// 0x158
			 0xd22bb0a0, /// 0x15c
			 0x5580aff5, /// 0x160
			 0x696e3b49, /// 0x164
			 0xa96b6a10, /// 0x168
			 0x93231cca, /// 0x16c
			 0x4d8d580e, /// 0x170
			 0xc2ceb980, /// 0x174
			 0xfb5a45f1, /// 0x178
			 0x4f0c5882, /// 0x17c
			 0xf43019d4, /// 0x180
			 0x3d95d661, /// 0x184
			 0x3533d8ca, /// 0x188
			 0xa287907c, /// 0x18c
			 0xd651ea11, /// 0x190
			 0x4d65f61e, /// 0x194
			 0xa9d11307, /// 0x198
			 0x2d94e925, /// 0x19c
			 0xbeee4eb9, /// 0x1a0
			 0x518d6df3, /// 0x1a4
			 0x0a49b44d, /// 0x1a8
			 0xc7add5bb, /// 0x1ac
			 0xb11b9a74, /// 0x1b0
			 0x6dabd131, /// 0x1b4
			 0x608b786c, /// 0x1b8
			 0xf8e82b59, /// 0x1bc
			 0xe2ddc20e, /// 0x1c0
			 0xdff81c0a, /// 0x1c4
			 0x760ac351, /// 0x1c8
			 0xf81b1ee2, /// 0x1cc
			 0x59a7e1f1, /// 0x1d0
			 0x3b61dff8, /// 0x1d4
			 0x01bc3e88, /// 0x1d8
			 0xd77e7543, /// 0x1dc
			 0x6d71fb4e, /// 0x1e0
			 0x7fe10aef, /// 0x1e4
			 0xad51977d, /// 0x1e8
			 0x45dc9e6c, /// 0x1ec
			 0x51ef9a33, /// 0x1f0
			 0x9d9aa972, /// 0x1f4
			 0xbf74ead7, /// 0x1f8
			 0x01395ea8, /// 0x1fc
			 0x03fd9026, /// 0x200
			 0xb9a524db, /// 0x204
			 0x26134999, /// 0x208
			 0x8e176329, /// 0x20c
			 0x27aabdb8, /// 0x210
			 0xe5746297, /// 0x214
			 0x753ab7ee, /// 0x218
			 0x9ebccaf6, /// 0x21c
			 0x61448de5, /// 0x220
			 0x9d40b21d, /// 0x224
			 0x3610ac5d, /// 0x228
			 0x7ce5a377, /// 0x22c
			 0xe4a5ca41, /// 0x230
			 0xa2b1cfcf, /// 0x234
			 0x8121f50a, /// 0x238
			 0x0d8f9190, /// 0x23c
			 0x95701471, /// 0x240
			 0x958cfc9c, /// 0x244
			 0x3486160a, /// 0x248
			 0x26112d1b, /// 0x24c
			 0x7af60794, /// 0x250
			 0x9cf64737, /// 0x254
			 0x87e81a19, /// 0x258
			 0xd9344730, /// 0x25c
			 0x91e9a6b5, /// 0x260
			 0x50a7fb4f, /// 0x264
			 0xace080f6, /// 0x268
			 0x81c50be7, /// 0x26c
			 0x048c01a2, /// 0x270
			 0x54adc093, /// 0x274
			 0x4d6c4089, /// 0x278
			 0xac24df19, /// 0x27c
			 0x908efa40, /// 0x280
			 0xc753bb84, /// 0x284
			 0x6d1160f5, /// 0x288
			 0xe67d43ac, /// 0x28c
			 0x9ee108df, /// 0x290
			 0x8f857400, /// 0x294
			 0x47db4002, /// 0x298
			 0x955eda03, /// 0x29c
			 0x063aa373, /// 0x2a0
			 0x00f3581f, /// 0x2a4
			 0xddbc2aed, /// 0x2a8
			 0xc19bdce0, /// 0x2ac
			 0x9a5c8206, /// 0x2b0
			 0xf7b1e9ae, /// 0x2b4
			 0x0f8568eb, /// 0x2b8
			 0x72cdd061, /// 0x2bc
			 0x8fa7c553, /// 0x2c0
			 0x29308207, /// 0x2c4
			 0xa6341b88, /// 0x2c8
			 0x08316679, /// 0x2cc
			 0x2c4c6f11, /// 0x2d0
			 0x3e0cdd0a, /// 0x2d4
			 0xbc97ede5, /// 0x2d8
			 0x9d705f1f, /// 0x2dc
			 0x899ea063, /// 0x2e0
			 0x2e9ad0ed, /// 0x2e4
			 0x967dd643, /// 0x2e8
			 0x1922f28b, /// 0x2ec
			 0x309116da, /// 0x2f0
			 0xc666571e, /// 0x2f4
			 0xdb18df71, /// 0x2f8
			 0x748e2d0a, /// 0x2fc
			 0x43a8b97a, /// 0x300
			 0xd1aa7031, /// 0x304
			 0xfe5bef36, /// 0x308
			 0xbe182a37, /// 0x30c
			 0xf5f327f7, /// 0x310
			 0xf116286b, /// 0x314
			 0x5d066dca, /// 0x318
			 0xac3db6f9, /// 0x31c
			 0xee0d8679, /// 0x320
			 0x878d4037, /// 0x324
			 0x3c13877d, /// 0x328
			 0x4cb63e17, /// 0x32c
			 0xe4b54d53, /// 0x330
			 0x218903b5, /// 0x334
			 0x3ce58cd9, /// 0x338
			 0xa180ddbc, /// 0x33c
			 0xb9ccf61b, /// 0x340
			 0x92566492, /// 0x344
			 0xf5250bb6, /// 0x348
			 0x8a3f10d4, /// 0x34c
			 0x4a3aae34, /// 0x350
			 0x775d2bd2, /// 0x354
			 0x14f7827e, /// 0x358
			 0x46ef3b1e, /// 0x35c
			 0x63151080, /// 0x360
			 0x474ff76e, /// 0x364
			 0xfcdf0442, /// 0x368
			 0x093cadc0, /// 0x36c
			 0xc9a3c0f2, /// 0x370
			 0xcf591f63, /// 0x374
			 0x9bdc2679, /// 0x378
			 0x51b6b9f3, /// 0x37c
			 0x59250ec6, /// 0x380
			 0x929aaea6, /// 0x384
			 0x793d445d, /// 0x388
			 0x98fff769, /// 0x38c
			 0xd4b0c503, /// 0x390
			 0x86593229, /// 0x394
			 0x81f999e8, /// 0x398
			 0x22364766, /// 0x39c
			 0x41d90c2a, /// 0x3a0
			 0x29d1d3f0, /// 0x3a4
			 0xc897e777, /// 0x3a8
			 0x9f5341d2, /// 0x3ac
			 0x8872a80f, /// 0x3b0
			 0xf794fcb3, /// 0x3b4
			 0x546d2fb7, /// 0x3b8
			 0xfc12f745, /// 0x3bc
			 0x389e932e, /// 0x3c0
			 0xda5771bb, /// 0x3c4
			 0x9357dd32, /// 0x3c8
			 0x781a7d0d, /// 0x3cc
			 0x29325c63, /// 0x3d0
			 0x33a41500, /// 0x3d4
			 0xcd98059c, /// 0x3d8
			 0xe3eaa155, /// 0x3dc
			 0x3c258fe1, /// 0x3e0
			 0x1acbfa85, /// 0x3e4
			 0x40a17e72, /// 0x3e8
			 0xe2f59d98, /// 0x3ec
			 0xb52ba40d, /// 0x3f0
			 0xa7a2a9e4, /// 0x3f4
			 0x28f817a2, /// 0x3f8
			 0xc3424b5d, /// 0x3fc
			 0xb5ef710e, /// 0x400
			 0xc658b800, /// 0x404
			 0x11c50ca6, /// 0x408
			 0xe88d65a2, /// 0x40c
			 0x1e4dc6c9, /// 0x410
			 0x3eb6b34b, /// 0x414
			 0xc42d0838, /// 0x418
			 0x498f3efe, /// 0x41c
			 0x02752d0e, /// 0x420
			 0x58d9beda, /// 0x424
			 0xc2f55bdd, /// 0x428
			 0xe734b60a, /// 0x42c
			 0x0a69d169, /// 0x430
			 0xc9ce0d31, /// 0x434
			 0x09223a91, /// 0x438
			 0x3cb62496, /// 0x43c
			 0x5c6482ab, /// 0x440
			 0x300a6620, /// 0x444
			 0x69148d7d, /// 0x448
			 0x858cf891, /// 0x44c
			 0xbfad2d81, /// 0x450
			 0x85a97fe9, /// 0x454
			 0x44d43ff3, /// 0x458
			 0xfaeb067b, /// 0x45c
			 0xa8eee8c3, /// 0x460
			 0x57b6a3e2, /// 0x464
			 0x0f8677bb, /// 0x468
			 0x4f51aa32, /// 0x46c
			 0x4442507a, /// 0x470
			 0x90c35afc, /// 0x474
			 0xb436dc9e, /// 0x478
			 0xed08596a, /// 0x47c
			 0xf9b0d624, /// 0x480
			 0xb698475f, /// 0x484
			 0x1e72cc5c, /// 0x488
			 0x30f84c05, /// 0x48c
			 0x8b0294c3, /// 0x490
			 0xfddddca2, /// 0x494
			 0x504ffc52, /// 0x498
			 0x13422ad9, /// 0x49c
			 0x646ef880, /// 0x4a0
			 0x520e5480, /// 0x4a4
			 0x03bec57a, /// 0x4a8
			 0xf6620c21, /// 0x4ac
			 0x0a62dbbb, /// 0x4b0
			 0x19efdb49, /// 0x4b4
			 0x41e8cab7, /// 0x4b8
			 0x7b79ba21, /// 0x4bc
			 0xe135c4e3, /// 0x4c0
			 0x3e47045e, /// 0x4c4
			 0x5736eeda, /// 0x4c8
			 0x54f6889f, /// 0x4cc
			 0x685690ae, /// 0x4d0
			 0xf94f577d, /// 0x4d4
			 0x15560b5c, /// 0x4d8
			 0x8dc8d468, /// 0x4dc
			 0x215b7f34, /// 0x4e0
			 0x59589ea0, /// 0x4e4
			 0x4d6502b1, /// 0x4e8
			 0xb0cdfd65, /// 0x4ec
			 0x62a9d39a, /// 0x4f0
			 0x12293b3c, /// 0x4f4
			 0xfbc6e1a6, /// 0x4f8
			 0xbe778b60, /// 0x4fc
			 0x5c82e5e2, /// 0x500
			 0xa0583c13, /// 0x504
			 0x9ef085ad, /// 0x508
			 0x19284a7e, /// 0x50c
			 0x76ea7d8a, /// 0x510
			 0x1262a19d, /// 0x514
			 0x539029ef, /// 0x518
			 0x28d36845, /// 0x51c
			 0x357c8f60, /// 0x520
			 0x22bc568d, /// 0x524
			 0xa5c24326, /// 0x528
			 0x155acb82, /// 0x52c
			 0xabc14402, /// 0x530
			 0x8413367c, /// 0x534
			 0xee11b110, /// 0x538
			 0xe5977952, /// 0x53c
			 0xa968fa9a, /// 0x540
			 0xab3a71a9, /// 0x544
			 0x41568f8b, /// 0x548
			 0x0784fe19, /// 0x54c
			 0x88228770, /// 0x550
			 0x1479a2b3, /// 0x554
			 0x9480abec, /// 0x558
			 0x4d5761eb, /// 0x55c
			 0x02b409cd, /// 0x560
			 0x0ca9d5a9, /// 0x564
			 0xa059c6be, /// 0x568
			 0x1b1a9eff, /// 0x56c
			 0x5f8fda6f, /// 0x570
			 0x6c4ed46b, /// 0x574
			 0x1f2e261e, /// 0x578
			 0x1a36cfce, /// 0x57c
			 0xff56ffd0, /// 0x580
			 0xa94a32a8, /// 0x584
			 0x621c58ab, /// 0x588
			 0x5cd3656d, /// 0x58c
			 0x721c97a3, /// 0x590
			 0x47e5adf2, /// 0x594
			 0x2e7c8c65, /// 0x598
			 0x772f2b31, /// 0x59c
			 0xe8641b01, /// 0x5a0
			 0x42331517, /// 0x5a4
			 0x34caf145, /// 0x5a8
			 0x93ed6704, /// 0x5ac
			 0x894a5c19, /// 0x5b0
			 0x68265a03, /// 0x5b4
			 0x349be075, /// 0x5b8
			 0x3129be0c, /// 0x5bc
			 0x35daa76f, /// 0x5c0
			 0x7a43dc4b, /// 0x5c4
			 0x47456bcd, /// 0x5c8
			 0x17625370, /// 0x5cc
			 0x028650c8, /// 0x5d0
			 0xdde1bbe1, /// 0x5d4
			 0x2fdacb49, /// 0x5d8
			 0x0be9f793, /// 0x5dc
			 0x3d63f22f, /// 0x5e0
			 0x5cbdd13a, /// 0x5e4
			 0xfeb8d623, /// 0x5e8
			 0xbf0d1b9c, /// 0x5ec
			 0x68594c64, /// 0x5f0
			 0x34778c65, /// 0x5f4
			 0x7726cb20, /// 0x5f8
			 0x4af9ed3c, /// 0x5fc
			 0xfe655b5d, /// 0x600
			 0x28de2ec7, /// 0x604
			 0x4a8f29bd, /// 0x608
			 0x65d442a7, /// 0x60c
			 0xdf34f547, /// 0x610
			 0xb2d4960c, /// 0x614
			 0x338b19fb, /// 0x618
			 0x67738504, /// 0x61c
			 0x79ee99ce, /// 0x620
			 0x07615bb0, /// 0x624
			 0xb071c7ca, /// 0x628
			 0xcd96a09d, /// 0x62c
			 0xa6b39ebe, /// 0x630
			 0xf18b2470, /// 0x634
			 0xb13f53b3, /// 0x638
			 0x802589f1, /// 0x63c
			 0x4f7a8537, /// 0x640
			 0x9dada189, /// 0x644
			 0xe1094c4e, /// 0x648
			 0x23108f43, /// 0x64c
			 0x7111c1e0, /// 0x650
			 0xbefda043, /// 0x654
			 0x51111057, /// 0x658
			 0x074b1005, /// 0x65c
			 0xbe9ea555, /// 0x660
			 0x8c765272, /// 0x664
			 0xf4e615cd, /// 0x668
			 0xd15d640a, /// 0x66c
			 0x2a7ef60e, /// 0x670
			 0x71d247b0, /// 0x674
			 0x9bcfafa6, /// 0x678
			 0x445fd47e, /// 0x67c
			 0xff18c5a7, /// 0x680
			 0xde27f39d, /// 0x684
			 0xdb8c76a7, /// 0x688
			 0xa5fab134, /// 0x68c
			 0x517ee9e9, /// 0x690
			 0x97df0418, /// 0x694
			 0xeb754f8a, /// 0x698
			 0xfa388085, /// 0x69c
			 0x418b5413, /// 0x6a0
			 0xdf00d253, /// 0x6a4
			 0xfe4d7236, /// 0x6a8
			 0xaeb1adbc, /// 0x6ac
			 0xc672d6f8, /// 0x6b0
			 0x00ffc04b, /// 0x6b4
			 0x91f2f59f, /// 0x6b8
			 0x955d6dc1, /// 0x6bc
			 0xfdbc8863, /// 0x6c0
			 0x38ca4e9e, /// 0x6c4
			 0x67c7fe6f, /// 0x6c8
			 0x84d7dffb, /// 0x6cc
			 0xf5d28085, /// 0x6d0
			 0xca358d17, /// 0x6d4
			 0x2e8bb434, /// 0x6d8
			 0xa5e65db6, /// 0x6dc
			 0xe85eda99, /// 0x6e0
			 0x26e1f3cf, /// 0x6e4
			 0xb5c40349, /// 0x6e8
			 0x3e20d836, /// 0x6ec
			 0xa9be48dc, /// 0x6f0
			 0xbc3ffca5, /// 0x6f4
			 0xf6d88918, /// 0x6f8
			 0xe8e0723a, /// 0x6fc
			 0x71a3efff, /// 0x700
			 0xb53040ec, /// 0x704
			 0x8c67e452, /// 0x708
			 0xd1ca96dd, /// 0x70c
			 0xb74c6121, /// 0x710
			 0xd41f1244, /// 0x714
			 0x5bceea6f, /// 0x718
			 0xf92f329f, /// 0x71c
			 0xbfecec2c, /// 0x720
			 0xe5f5b30c, /// 0x724
			 0xe57d107f, /// 0x728
			 0x14f1ebba, /// 0x72c
			 0x927c523a, /// 0x730
			 0x361de311, /// 0x734
			 0x644f10ff, /// 0x738
			 0x8a7b8fdb, /// 0x73c
			 0x043c15db, /// 0x740
			 0xdd362873, /// 0x744
			 0x5d2a9520, /// 0x748
			 0x6124a7dc, /// 0x74c
			 0x684838db, /// 0x750
			 0xd79b1b73, /// 0x754
			 0x7b2db2ed, /// 0x758
			 0x3df65fcb, /// 0x75c
			 0xc0ff7d14, /// 0x760
			 0x74aa02a1, /// 0x764
			 0x185cce03, /// 0x768
			 0x501426b3, /// 0x76c
			 0x9b15fc4a, /// 0x770
			 0x65b0fe91, /// 0x774
			 0x1c231671, /// 0x778
			 0x18ba64cb, /// 0x77c
			 0x006fec94, /// 0x780
			 0xead4416a, /// 0x784
			 0xc02da5f1, /// 0x788
			 0x06da2b04, /// 0x78c
			 0x05c647aa, /// 0x790
			 0x9ac8b3e0, /// 0x794
			 0x4a824f13, /// 0x798
			 0xe560ffc5, /// 0x79c
			 0x33151383, /// 0x7a0
			 0x85eab1e1, /// 0x7a4
			 0x1639ce49, /// 0x7a8
			 0xcd112127, /// 0x7ac
			 0x7650139b, /// 0x7b0
			 0x0f9ab9ea, /// 0x7b4
			 0x3429f822, /// 0x7b8
			 0x79fd43aa, /// 0x7bc
			 0x1859d95b, /// 0x7c0
			 0x3db6c457, /// 0x7c4
			 0x6d56a38c, /// 0x7c8
			 0xfdcf40cf, /// 0x7cc
			 0x516407ce, /// 0x7d0
			 0x76ba45d5, /// 0x7d4
			 0x09d6c74e, /// 0x7d8
			 0x331d0a4f, /// 0x7dc
			 0x5035d146, /// 0x7e0
			 0x71cf913c, /// 0x7e4
			 0x141607d0, /// 0x7e8
			 0x0015650b, /// 0x7ec
			 0x66ba10a3, /// 0x7f0
			 0xb171188a, /// 0x7f4
			 0x98496771, /// 0x7f8
			 0xacbd726a, /// 0x7fc
			 0x20952cbb, /// 0x800
			 0xae0e5bc2, /// 0x804
			 0x33276eb2, /// 0x808
			 0x8be8eb7c, /// 0x80c
			 0xf8392922, /// 0x810
			 0xe62bf26c, /// 0x814
			 0x7399b287, /// 0x818
			 0xcb33cbda, /// 0x81c
			 0x38b4b71e, /// 0x820
			 0xa604c05e, /// 0x824
			 0x865328a2, /// 0x828
			 0xc869f0f8, /// 0x82c
			 0x0049cf10, /// 0x830
			 0x7b0e54a8, /// 0x834
			 0x57aa444c, /// 0x838
			 0x74dc1cd5, /// 0x83c
			 0x81a5a97e, /// 0x840
			 0x7c11cb79, /// 0x844
			 0xb10e4fef, /// 0x848
			 0x2e9dd5bd, /// 0x84c
			 0x9ca280eb, /// 0x850
			 0xc3a82688, /// 0x854
			 0xa006f1e8, /// 0x858
			 0x0d3a859b, /// 0x85c
			 0x96600af0, /// 0x860
			 0x061afd45, /// 0x864
			 0x62f933ba, /// 0x868
			 0xab5abfb7, /// 0x86c
			 0x5cd67c67, /// 0x870
			 0x2169b877, /// 0x874
			 0x969ea2e4, /// 0x878
			 0x67543997, /// 0x87c
			 0x92cdd3c7, /// 0x880
			 0xa3cee9e0, /// 0x884
			 0xc9d186f1, /// 0x888
			 0xa79d8e60, /// 0x88c
			 0xebc63905, /// 0x890
			 0x1b805220, /// 0x894
			 0x6dba709f, /// 0x898
			 0x96b4f141, /// 0x89c
			 0xe40efabe, /// 0x8a0
			 0x6e31ee26, /// 0x8a4
			 0x674b40c2, /// 0x8a8
			 0x82b877b6, /// 0x8ac
			 0x13ea1e06, /// 0x8b0
			 0xaeb5668e, /// 0x8b4
			 0x65bad523, /// 0x8b8
			 0xa6deb56c, /// 0x8bc
			 0x0c189599, /// 0x8c0
			 0x79183f47, /// 0x8c4
			 0x989350c5, /// 0x8c8
			 0x63f22a2b, /// 0x8cc
			 0xdd76be68, /// 0x8d0
			 0xf618f730, /// 0x8d4
			 0x465b080e, /// 0x8d8
			 0x1adcbe69, /// 0x8dc
			 0x913a91c3, /// 0x8e0
			 0xc43c460e, /// 0x8e4
			 0xcec9127d, /// 0x8e8
			 0x30838d6c, /// 0x8ec
			 0x4772621a, /// 0x8f0
			 0x6eeb5544, /// 0x8f4
			 0x98679d68, /// 0x8f8
			 0xf8280049, /// 0x8fc
			 0xfbfc2d0a, /// 0x900
			 0x94cc687c, /// 0x904
			 0x5e4c9877, /// 0x908
			 0xd33519d0, /// 0x90c
			 0xa2ee4ae1, /// 0x910
			 0x5b8c7344, /// 0x914
			 0xbf16d425, /// 0x918
			 0xaa681afa, /// 0x91c
			 0xe8f54b5b, /// 0x920
			 0x5dd5892d, /// 0x924
			 0x2b14e0cf, /// 0x928
			 0x33c3cf5d, /// 0x92c
			 0xac65d510, /// 0x930
			 0x2b16032c, /// 0x934
			 0x033e7450, /// 0x938
			 0xe8e10cbd, /// 0x93c
			 0x130fc97c, /// 0x940
			 0x069ece74, /// 0x944
			 0xf2b93ea1, /// 0x948
			 0x0f4f19d8, /// 0x94c
			 0x0975bc85, /// 0x950
			 0x2b39c7c7, /// 0x954
			 0x3ed20a49, /// 0x958
			 0x615fe3e2, /// 0x95c
			 0x4cdb9437, /// 0x960
			 0xdb425d6d, /// 0x964
			 0x9493a74b, /// 0x968
			 0x0c4454c1, /// 0x96c
			 0x0ec4efbd, /// 0x970
			 0x034406d4, /// 0x974
			 0x13c20349, /// 0x978
			 0x0f9bbab1, /// 0x97c
			 0x2838d6aa, /// 0x980
			 0x9ba2af62, /// 0x984
			 0xa26009e2, /// 0x988
			 0x257d2ef3, /// 0x98c
			 0x86f6ef6c, /// 0x990
			 0x134f1d3b, /// 0x994
			 0xc8865957, /// 0x998
			 0xe8e863cc, /// 0x99c
			 0x8f1e13c7, /// 0x9a0
			 0x2b5c078f, /// 0x9a4
			 0xfa11738a, /// 0x9a8
			 0x946a8abd, /// 0x9ac
			 0x70bf08d4, /// 0x9b0
			 0xa166d002, /// 0x9b4
			 0xaff97422, /// 0x9b8
			 0x8987023f, /// 0x9bc
			 0xbc16fccb, /// 0x9c0
			 0x8b60bcbd, /// 0x9c4
			 0xb290f740, /// 0x9c8
			 0x894ca775, /// 0x9cc
			 0x31d9621c, /// 0x9d0
			 0xb0c4ab8b, /// 0x9d4
			 0xfba2cd52, /// 0x9d8
			 0xaadd0eea, /// 0x9dc
			 0xc13ff8da, /// 0x9e0
			 0x1ac55e0e, /// 0x9e4
			 0x1da7fbf5, /// 0x9e8
			 0x14d33e69, /// 0x9ec
			 0x0afcbeed, /// 0x9f0
			 0x7e41e79e, /// 0x9f4
			 0x8b7276fb, /// 0x9f8
			 0xb5e2f1b9, /// 0x9fc
			 0x79b7a87c, /// 0xa00
			 0xbf86da97, /// 0xa04
			 0xffc4ea98, /// 0xa08
			 0x9a5db2de, /// 0xa0c
			 0xd6b42a17, /// 0xa10
			 0xfc265656, /// 0xa14
			 0x1d3fe84e, /// 0xa18
			 0xa6aee9f0, /// 0xa1c
			 0xf0aeee6a, /// 0xa20
			 0x89aaef04, /// 0xa24
			 0xaa8c0e0f, /// 0xa28
			 0xd083072c, /// 0xa2c
			 0x221b88c7, /// 0xa30
			 0xca86e325, /// 0xa34
			 0x7563e085, /// 0xa38
			 0xcded3c0c, /// 0xa3c
			 0xac55cbee, /// 0xa40
			 0xf745d01a, /// 0xa44
			 0x4a254a98, /// 0xa48
			 0x8ef56edb, /// 0xa4c
			 0x9346b404, /// 0xa50
			 0x8ebf4a5d, /// 0xa54
			 0x12670d72, /// 0xa58
			 0x4fb3a593, /// 0xa5c
			 0x5a478e22, /// 0xa60
			 0x15a70d7b, /// 0xa64
			 0x0518da67, /// 0xa68
			 0xa5755f3d, /// 0xa6c
			 0x7bfcaec3, /// 0xa70
			 0xf1ab2fcd, /// 0xa74
			 0x87c938e8, /// 0xa78
			 0xa1bd4fb6, /// 0xa7c
			 0xf3e61f52, /// 0xa80
			 0xfb9ff568, /// 0xa84
			 0x31f8c5b4, /// 0xa88
			 0xbd7c8c89, /// 0xa8c
			 0xe2f88a56, /// 0xa90
			 0x5466c0ee, /// 0xa94
			 0xc1de3e7f, /// 0xa98
			 0xb1b24e63, /// 0xa9c
			 0x75daac72, /// 0xaa0
			 0xe211a363, /// 0xaa4
			 0xa9c54c35, /// 0xaa8
			 0xe5242fe8, /// 0xaac
			 0x4b91c877, /// 0xab0
			 0x8a2f4c67, /// 0xab4
			 0x9c7c2c67, /// 0xab8
			 0x78502375, /// 0xabc
			 0x06a23ca2, /// 0xac0
			 0x64585040, /// 0xac4
			 0xed880c55, /// 0xac8
			 0x6cfd649a, /// 0xacc
			 0x9d47b155, /// 0xad0
			 0xb90a4644, /// 0xad4
			 0x850971fa, /// 0xad8
			 0xaa9820aa, /// 0xadc
			 0x123408ad, /// 0xae0
			 0x9337ecee, /// 0xae4
			 0xec252ebe, /// 0xae8
			 0xc538309f, /// 0xaec
			 0x04e44a8d, /// 0xaf0
			 0x993700c2, /// 0xaf4
			 0x5de4a6a9, /// 0xaf8
			 0x6003fe8d, /// 0xafc
			 0xf053613f, /// 0xb00
			 0x814da782, /// 0xb04
			 0x52f0dcbe, /// 0xb08
			 0x973b815b, /// 0xb0c
			 0x39f1bda2, /// 0xb10
			 0xe792b354, /// 0xb14
			 0xe7173658, /// 0xb18
			 0x87d86d49, /// 0xb1c
			 0xe2a8739f, /// 0xb20
			 0x80fc6fd8, /// 0xb24
			 0xbfd0e4a2, /// 0xb28
			 0x85a323fc, /// 0xb2c
			 0x8f50e517, /// 0xb30
			 0x1a7db768, /// 0xb34
			 0xbdcdf47e, /// 0xb38
			 0xe313e113, /// 0xb3c
			 0xf70bb989, /// 0xb40
			 0x34e25e1d, /// 0xb44
			 0x547e8667, /// 0xb48
			 0x1cbb8668, /// 0xb4c
			 0x5eb123d0, /// 0xb50
			 0xbe794ef1, /// 0xb54
			 0xfb26df65, /// 0xb58
			 0xf2b518ad, /// 0xb5c
			 0x42b38e68, /// 0xb60
			 0xfccd1fff, /// 0xb64
			 0x47921c99, /// 0xb68
			 0x0bfc04be, /// 0xb6c
			 0xa981968b, /// 0xb70
			 0xca4d9279, /// 0xb74
			 0x845ca114, /// 0xb78
			 0x0b92a1d5, /// 0xb7c
			 0x46afd6cf, /// 0xb80
			 0x2d005592, /// 0xb84
			 0x1a68fa21, /// 0xb88
			 0x25d3f513, /// 0xb8c
			 0x1efb3af1, /// 0xb90
			 0x29e9da10, /// 0xb94
			 0x77dbbb51, /// 0xb98
			 0x2e569f36, /// 0xb9c
			 0x1bf1fbd1, /// 0xba0
			 0x9bbb0811, /// 0xba4
			 0xd9fe35fe, /// 0xba8
			 0x39b74002, /// 0xbac
			 0x95104904, /// 0xbb0
			 0x2384c2db, /// 0xbb4
			 0xb45becaa, /// 0xbb8
			 0x8b8d49c7, /// 0xbbc
			 0x1fa4ccf1, /// 0xbc0
			 0x429a5a85, /// 0xbc4
			 0xb26ea49b, /// 0xbc8
			 0xb4214a28, /// 0xbcc
			 0x576eedc6, /// 0xbd0
			 0x93df5bc6, /// 0xbd4
			 0x60223cb8, /// 0xbd8
			 0x96ad7540, /// 0xbdc
			 0xb14faf7e, /// 0xbe0
			 0x68a72dfb, /// 0xbe4
			 0xbe36a57c, /// 0xbe8
			 0x0e254fdb, /// 0xbec
			 0x1c96371e, /// 0xbf0
			 0xe52fca18, /// 0xbf4
			 0x8aab076d, /// 0xbf8
			 0xdea0651e, /// 0xbfc
			 0xccc4a1e7, /// 0xc00
			 0xd7107c85, /// 0xc04
			 0xec44a236, /// 0xc08
			 0x8ab1d3b0, /// 0xc0c
			 0xe3f843f9, /// 0xc10
			 0x8e1cac66, /// 0xc14
			 0xcd3b2fe2, /// 0xc18
			 0x88c33c17, /// 0xc1c
			 0x60617041, /// 0xc20
			 0xe380e38f, /// 0xc24
			 0xb0669c78, /// 0xc28
			 0x00b50dda, /// 0xc2c
			 0x0d81d03a, /// 0xc30
			 0xd9367c25, /// 0xc34
			 0xe1062106, /// 0xc38
			 0xb3aa836d, /// 0xc3c
			 0xec798cdd, /// 0xc40
			 0x94ac79b2, /// 0xc44
			 0xf70abcf7, /// 0xc48
			 0x5c4250a3, /// 0xc4c
			 0x5e778460, /// 0xc50
			 0xc410f55e, /// 0xc54
			 0x91ec3244, /// 0xc58
			 0x2e8b346f, /// 0xc5c
			 0xffe4b9a0, /// 0xc60
			 0xd1bfcaa1, /// 0xc64
			 0x0a9a8bf3, /// 0xc68
			 0x42de294c, /// 0xc6c
			 0xf21f14a0, /// 0xc70
			 0xe3b38a05, /// 0xc74
			 0xdbd73e36, /// 0xc78
			 0x33e0009b, /// 0xc7c
			 0xc047bdcc, /// 0xc80
			 0x3d0e5c57, /// 0xc84
			 0xb7ef46a0, /// 0xc88
			 0xad46b6ea, /// 0xc8c
			 0xe01563a8, /// 0xc90
			 0x747db14b, /// 0xc94
			 0xbaab8ee2, /// 0xc98
			 0x666929b4, /// 0xc9c
			 0xfa473c77, /// 0xca0
			 0x2e10704d, /// 0xca4
			 0x28841fd9, /// 0xca8
			 0x180d1438, /// 0xcac
			 0xcb384fe5, /// 0xcb0
			 0x4a8196c3, /// 0xcb4
			 0x11762f4e, /// 0xcb8
			 0xb355d3dd, /// 0xcbc
			 0xa7fe74ed, /// 0xcc0
			 0x18c40d2c, /// 0xcc4
			 0x91164955, /// 0xcc8
			 0x010f6001, /// 0xccc
			 0xd5734c2f, /// 0xcd0
			 0xaadb61c8, /// 0xcd4
			 0x00f25791, /// 0xcd8
			 0xcc92f477, /// 0xcdc
			 0x731212bd, /// 0xce0
			 0xdb887522, /// 0xce4
			 0x9cae796c, /// 0xce8
			 0x3e57dbee, /// 0xcec
			 0x3276c346, /// 0xcf0
			 0x2dc74ff8, /// 0xcf4
			 0x20587729, /// 0xcf8
			 0x5b851490, /// 0xcfc
			 0xf83a1071, /// 0xd00
			 0x901f67ac, /// 0xd04
			 0xf5de8729, /// 0xd08
			 0xb0ecef08, /// 0xd0c
			 0x2859233c, /// 0xd10
			 0x1da801cc, /// 0xd14
			 0xd50a4868, /// 0xd18
			 0x0814d5c3, /// 0xd1c
			 0x7926d241, /// 0xd20
			 0x02bd83f0, /// 0xd24
			 0x1c90e30d, /// 0xd28
			 0x5da6550c, /// 0xd2c
			 0xfc24f7ba, /// 0xd30
			 0x66260e3c, /// 0xd34
			 0x80366749, /// 0xd38
			 0x2c2e5e73, /// 0xd3c
			 0xce43b63d, /// 0xd40
			 0x772d0442, /// 0xd44
			 0xfe1dcb11, /// 0xd48
			 0x5959348f, /// 0xd4c
			 0x7db88597, /// 0xd50
			 0x81b2e2fe, /// 0xd54
			 0x3ef4f63c, /// 0xd58
			 0xc2721981, /// 0xd5c
			 0x529b9b06, /// 0xd60
			 0x16ae029f, /// 0xd64
			 0x0aebb5cb, /// 0xd68
			 0xa8f8daf5, /// 0xd6c
			 0x50cb29b6, /// 0xd70
			 0x5d999f23, /// 0xd74
			 0x215f9dbb, /// 0xd78
			 0xbcdbe6e7, /// 0xd7c
			 0x45bd9157, /// 0xd80
			 0xbee294c7, /// 0xd84
			 0x092e10fa, /// 0xd88
			 0x3d54d159, /// 0xd8c
			 0x6095a9a0, /// 0xd90
			 0x8dad449d, /// 0xd94
			 0xad074ea0, /// 0xd98
			 0x5ff440e3, /// 0xd9c
			 0x8b8f4829, /// 0xda0
			 0x4cfbf4f7, /// 0xda4
			 0xb017ac9c, /// 0xda8
			 0x7ef5537f, /// 0xdac
			 0x17a3ee43, /// 0xdb0
			 0xf30484f6, /// 0xdb4
			 0x01d5d411, /// 0xdb8
			 0xb5594bdf, /// 0xdbc
			 0x6340ec06, /// 0xdc0
			 0xe0f2776c, /// 0xdc4
			 0x00919279, /// 0xdc8
			 0xc037aa56, /// 0xdcc
			 0x7de89307, /// 0xdd0
			 0xae9aeaa8, /// 0xdd4
			 0xd625ee6b, /// 0xdd8
			 0xb43bb12a, /// 0xddc
			 0x4059b483, /// 0xde0
			 0x95703359, /// 0xde4
			 0xc2cce40c, /// 0xde8
			 0x2da2b844, /// 0xdec
			 0x941b1fe2, /// 0xdf0
			 0x667b432e, /// 0xdf4
			 0x3022cc6f, /// 0xdf8
			 0x9d954515, /// 0xdfc
			 0x6841a09e, /// 0xe00
			 0xaf682721, /// 0xe04
			 0xe34ce395, /// 0xe08
			 0x722ba6c1, /// 0xe0c
			 0x9ef40e5f, /// 0xe10
			 0x3e5f329b, /// 0xe14
			 0x4fa45776, /// 0xe18
			 0xfbd8d2d4, /// 0xe1c
			 0xf1f0d227, /// 0xe20
			 0x494cb436, /// 0xe24
			 0x435b5ad7, /// 0xe28
			 0x56915c01, /// 0xe2c
			 0x40fa77f7, /// 0xe30
			 0x136a4434, /// 0xe34
			 0xbab444bb, /// 0xe38
			 0x124bcd6f, /// 0xe3c
			 0x9fe752a1, /// 0xe40
			 0x0b688eb6, /// 0xe44
			 0x7570a8a6, /// 0xe48
			 0x64c4692f, /// 0xe4c
			 0xa09e0aa8, /// 0xe50
			 0x7c2cf5c2, /// 0xe54
			 0x362b9b2a, /// 0xe58
			 0x6b431b62, /// 0xe5c
			 0x74ec86c4, /// 0xe60
			 0xeb6b9cb5, /// 0xe64
			 0x518a81be, /// 0xe68
			 0x221e82b4, /// 0xe6c
			 0xb27e5756, /// 0xe70
			 0x335b3366, /// 0xe74
			 0xb1bea59b, /// 0xe78
			 0xbae0eb73, /// 0xe7c
			 0xa04ca7fb, /// 0xe80
			 0xd8767665, /// 0xe84
			 0xd26e4047, /// 0xe88
			 0x41779862, /// 0xe8c
			 0x244d4cee, /// 0xe90
			 0x44667812, /// 0xe94
			 0x37af1e5f, /// 0xe98
			 0x85e63cea, /// 0xe9c
			 0xaa521a2b, /// 0xea0
			 0xbe2bd35f, /// 0xea4
			 0xfda8d764, /// 0xea8
			 0x44935dd4, /// 0xeac
			 0x6ea1d577, /// 0xeb0
			 0x82b90654, /// 0xeb4
			 0x4cfe482d, /// 0xeb8
			 0xb2dd2fd7, /// 0xebc
			 0x2eabcd71, /// 0xec0
			 0xa977b38e, /// 0xec4
			 0xdb83b664, /// 0xec8
			 0xd9897045, /// 0xecc
			 0x741cc2ac, /// 0xed0
			 0x476b4fd4, /// 0xed4
			 0xdbc24079, /// 0xed8
			 0x43695bf1, /// 0xedc
			 0x9f83311b, /// 0xee0
			 0x2728277b, /// 0xee4
			 0x6a7261f6, /// 0xee8
			 0xe1d3cec9, /// 0xeec
			 0x41c77253, /// 0xef0
			 0xce070fe2, /// 0xef4
			 0xa92022f5, /// 0xef8
			 0x6bb23339, /// 0xefc
			 0xd3a89642, /// 0xf00
			 0xc00fe4df, /// 0xf04
			 0x00841b2f, /// 0xf08
			 0x56490d15, /// 0xf0c
			 0x789c7490, /// 0xf10
			 0x9eadee63, /// 0xf14
			 0xe13ce492, /// 0xf18
			 0xc9fb728e, /// 0xf1c
			 0x61b46e83, /// 0xf20
			 0xb27f0d90, /// 0xf24
			 0x265f51ef, /// 0xf28
			 0xdd36407d, /// 0xf2c
			 0x34c8e6ba, /// 0xf30
			 0xbc9c323d, /// 0xf34
			 0x5a3c0124, /// 0xf38
			 0x998f450f, /// 0xf3c
			 0xaba4c6f2, /// 0xf40
			 0x754ea2c3, /// 0xf44
			 0x37813784, /// 0xf48
			 0x063f2104, /// 0xf4c
			 0x91b3ce09, /// 0xf50
			 0xe6af192d, /// 0xf54
			 0x5aecec31, /// 0xf58
			 0x537ca05f, /// 0xf5c
			 0x1f08cfef, /// 0xf60
			 0x60315b6f, /// 0xf64
			 0xea022aff, /// 0xf68
			 0x39ab2508, /// 0xf6c
			 0x949e3107, /// 0xf70
			 0x6c8e0c20, /// 0xf74
			 0x271feba6, /// 0xf78
			 0x4c446fe6, /// 0xf7c
			 0x6ae86f14, /// 0xf80
			 0xd3a035cc, /// 0xf84
			 0xdd04cc0f, /// 0xf88
			 0xf7c29cf7, /// 0xf8c
			 0x1178956f, /// 0xf90
			 0x41155478, /// 0xf94
			 0x1bf8ac02, /// 0xf98
			 0xda37ef1a, /// 0xf9c
			 0x00b09d37, /// 0xfa0
			 0x8e37d4cc, /// 0xfa4
			 0xd1a6a5a8, /// 0xfa8
			 0x99da515b, /// 0xfac
			 0xceebce8a, /// 0xfb0
			 0xf8398c0a, /// 0xfb4
			 0xae91853a, /// 0xfb8
			 0xa9c4b2e1, /// 0xfbc
			 0xb80fcfa5, /// 0xfc0
			 0x203b766b, /// 0xfc4
			 0x3bdefa59, /// 0xfc8
			 0x1a641f70, /// 0xfcc
			 0x365e64f2, /// 0xfd0
			 0x6810b877, /// 0xfd4
			 0x739eb685, /// 0xfd8
			 0xe59881c6, /// 0xfdc
			 0x54979d83, /// 0xfe0
			 0x337713b7, /// 0xfe4
			 0xc083dd24, /// 0xfe8
			 0x74f0829b, /// 0xfec
			 0x40d38df2, /// 0xff0
			 0x2d5f10f3, /// 0xff4
			 0x32ae5e97, /// 0xff8
			 0x766e4f3d /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x861b7656, /// 0x0
			 0x050c4463, /// 0x4
			 0xf328774a, /// 0x8
			 0xac4f463d, /// 0xc
			 0xb519fb14, /// 0x10
			 0xc446ac98, /// 0x14
			 0xd06853ba, /// 0x18
			 0x34f2e9d0, /// 0x1c
			 0xa79db4b1, /// 0x20
			 0x0da735b3, /// 0x24
			 0x0fa1b6f3, /// 0x28
			 0xf9c4d5a4, /// 0x2c
			 0x5fc0b078, /// 0x30
			 0x0dfe8224, /// 0x34
			 0x49afb171, /// 0x38
			 0x9ae64a54, /// 0x3c
			 0x4e0161b4, /// 0x40
			 0x55de42af, /// 0x44
			 0xbc8f5baf, /// 0x48
			 0x6fb3bb3f, /// 0x4c
			 0x75ba683e, /// 0x50
			 0x463320cd, /// 0x54
			 0xc82f2cdf, /// 0x58
			 0x68fe2fac, /// 0x5c
			 0xc7b55aca, /// 0x60
			 0x89183359, /// 0x64
			 0xab8466e3, /// 0x68
			 0x9c404d21, /// 0x6c
			 0x6d95f3d4, /// 0x70
			 0xed19776c, /// 0x74
			 0x29550e5e, /// 0x78
			 0x76a6f446, /// 0x7c
			 0xf37077c4, /// 0x80
			 0x9fdcdd73, /// 0x84
			 0x5bc025b1, /// 0x88
			 0xb9a57a34, /// 0x8c
			 0xd1f17bb4, /// 0x90
			 0xc8624485, /// 0x94
			 0x12032391, /// 0x98
			 0xd70553c7, /// 0x9c
			 0x8879f22d, /// 0xa0
			 0xfc02e5c4, /// 0xa4
			 0x829f5b6b, /// 0xa8
			 0x90d63ec6, /// 0xac
			 0x9d38e78f, /// 0xb0
			 0xf690a0c5, /// 0xb4
			 0xdfed0402, /// 0xb8
			 0xbe3c87fa, /// 0xbc
			 0x86e9c17d, /// 0xc0
			 0x5c12df4c, /// 0xc4
			 0xf7fd95c4, /// 0xc8
			 0x874241e5, /// 0xcc
			 0xa00972c0, /// 0xd0
			 0xb075e415, /// 0xd4
			 0x59cca3fc, /// 0xd8
			 0x1fb976af, /// 0xdc
			 0x887510b0, /// 0xe0
			 0x8cf12f49, /// 0xe4
			 0x8a0d2624, /// 0xe8
			 0xc7f5375b, /// 0xec
			 0x6b3355aa, /// 0xf0
			 0x475d87b0, /// 0xf4
			 0xb57318ac, /// 0xf8
			 0xc4591aa4, /// 0xfc
			 0x576a310c, /// 0x100
			 0x176731e1, /// 0x104
			 0x5c56eaf7, /// 0x108
			 0x22906eca, /// 0x10c
			 0xe90a474d, /// 0x110
			 0x5edfce31, /// 0x114
			 0x13754e03, /// 0x118
			 0xe1747cd0, /// 0x11c
			 0x6f5975b6, /// 0x120
			 0x575a5b79, /// 0x124
			 0xca12c484, /// 0x128
			 0xbe95d7f7, /// 0x12c
			 0x501f5358, /// 0x130
			 0x4bcdbac1, /// 0x134
			 0x11efc1ea, /// 0x138
			 0xd473c985, /// 0x13c
			 0x75a28ca7, /// 0x140
			 0xa523c685, /// 0x144
			 0xf8040024, /// 0x148
			 0xa4ebeb2a, /// 0x14c
			 0x9872c1a9, /// 0x150
			 0x5325fb3e, /// 0x154
			 0x43516b6a, /// 0x158
			 0x55c4845e, /// 0x15c
			 0x7ca656eb, /// 0x160
			 0x5d61e743, /// 0x164
			 0xfad54b19, /// 0x168
			 0x6ac42e67, /// 0x16c
			 0xa635de40, /// 0x170
			 0xd79727c1, /// 0x174
			 0x35e9b08a, /// 0x178
			 0x00f35181, /// 0x17c
			 0xe2015d6c, /// 0x180
			 0x9ecbd552, /// 0x184
			 0xad215432, /// 0x188
			 0x1b643fee, /// 0x18c
			 0x61cde67f, /// 0x190
			 0x97de16d2, /// 0x194
			 0x48f42a51, /// 0x198
			 0x3e34ff70, /// 0x19c
			 0xb1618b8d, /// 0x1a0
			 0xf9f540d3, /// 0x1a4
			 0x2d98b584, /// 0x1a8
			 0x27b55143, /// 0x1ac
			 0x27fbf5df, /// 0x1b0
			 0xf2ea60a4, /// 0x1b4
			 0xa4a4cddf, /// 0x1b8
			 0xce5b6ee6, /// 0x1bc
			 0xf642c186, /// 0x1c0
			 0xddab3ed6, /// 0x1c4
			 0xc6e0b3c7, /// 0x1c8
			 0x13e9c347, /// 0x1cc
			 0x1907b82e, /// 0x1d0
			 0xfbed5e5c, /// 0x1d4
			 0xda14d89f, /// 0x1d8
			 0xea6ce693, /// 0x1dc
			 0x7721069f, /// 0x1e0
			 0xa83b8c5b, /// 0x1e4
			 0x1681cb58, /// 0x1e8
			 0x36af08af, /// 0x1ec
			 0x415fa215, /// 0x1f0
			 0x92a8822f, /// 0x1f4
			 0x414af03e, /// 0x1f8
			 0x972279fd, /// 0x1fc
			 0x536a0fec, /// 0x200
			 0x3a2de8f5, /// 0x204
			 0xc8a023be, /// 0x208
			 0xe0c2416f, /// 0x20c
			 0xef11191e, /// 0x210
			 0xe66f137c, /// 0x214
			 0x660dc765, /// 0x218
			 0xe14c7657, /// 0x21c
			 0x1d059c06, /// 0x220
			 0x3e06b953, /// 0x224
			 0xb18503a8, /// 0x228
			 0x3b7131ff, /// 0x22c
			 0x16290303, /// 0x230
			 0x64ea507d, /// 0x234
			 0x6399df1a, /// 0x238
			 0xc69330c7, /// 0x23c
			 0x6ab384d9, /// 0x240
			 0x4247d9ee, /// 0x244
			 0x0729dd4d, /// 0x248
			 0x028bac9e, /// 0x24c
			 0x3cf07bc8, /// 0x250
			 0x079174d1, /// 0x254
			 0x765479d9, /// 0x258
			 0xc00d8cda, /// 0x25c
			 0x649980a2, /// 0x260
			 0xc7d0086a, /// 0x264
			 0xd4d3a1de, /// 0x268
			 0xcb3b87d0, /// 0x26c
			 0x0b522d20, /// 0x270
			 0xceaef552, /// 0x274
			 0x39358218, /// 0x278
			 0x885a0ecf, /// 0x27c
			 0x4fb611ec, /// 0x280
			 0x58460def, /// 0x284
			 0x603a1af6, /// 0x288
			 0xcbc7125e, /// 0x28c
			 0x94192076, /// 0x290
			 0xf6c442ec, /// 0x294
			 0xdf65ceb9, /// 0x298
			 0x63680e4d, /// 0x29c
			 0xbbc8b936, /// 0x2a0
			 0xdadb2b58, /// 0x2a4
			 0x80dd608e, /// 0x2a8
			 0x1581e2ae, /// 0x2ac
			 0x11860270, /// 0x2b0
			 0x869e1f89, /// 0x2b4
			 0xa5780bc7, /// 0x2b8
			 0x2cb8da18, /// 0x2bc
			 0x6ca3dd00, /// 0x2c0
			 0xc92cc437, /// 0x2c4
			 0x7e3ef32a, /// 0x2c8
			 0xb7efa2d4, /// 0x2cc
			 0xffff7402, /// 0x2d0
			 0xb5aed983, /// 0x2d4
			 0x03b34650, /// 0x2d8
			 0x3be759d2, /// 0x2dc
			 0x36a9edb2, /// 0x2e0
			 0x30149de6, /// 0x2e4
			 0xa536ff3f, /// 0x2e8
			 0xd9fc44b6, /// 0x2ec
			 0x3046ab71, /// 0x2f0
			 0x20dc1f2b, /// 0x2f4
			 0x31b88646, /// 0x2f8
			 0x4ef96a34, /// 0x2fc
			 0x8f540dd1, /// 0x300
			 0x2cb16fbb, /// 0x304
			 0xa779a41f, /// 0x308
			 0xcda717de, /// 0x30c
			 0x422fe9d1, /// 0x310
			 0xde64f838, /// 0x314
			 0xe2b05e94, /// 0x318
			 0x3c244d83, /// 0x31c
			 0x6343824f, /// 0x320
			 0x4535beea, /// 0x324
			 0x9e5bb8a8, /// 0x328
			 0x79da83ff, /// 0x32c
			 0xd9430c9a, /// 0x330
			 0xaf5b2154, /// 0x334
			 0x3a06dd13, /// 0x338
			 0x1c3d3e0e, /// 0x33c
			 0xe2bfa1ab, /// 0x340
			 0x1b257bb2, /// 0x344
			 0x33d825e3, /// 0x348
			 0x06f4b833, /// 0x34c
			 0x0b5b3902, /// 0x350
			 0xab135d58, /// 0x354
			 0xe22f1f79, /// 0x358
			 0x70cd4928, /// 0x35c
			 0x26af925b, /// 0x360
			 0x5ab9c467, /// 0x364
			 0x25e354cb, /// 0x368
			 0xc17bea51, /// 0x36c
			 0xdff61107, /// 0x370
			 0xd80c78bb, /// 0x374
			 0x633c1cc3, /// 0x378
			 0x76675e9d, /// 0x37c
			 0x7cf04ef6, /// 0x380
			 0xb94f92f3, /// 0x384
			 0x84968f97, /// 0x388
			 0x19511f32, /// 0x38c
			 0x1c693cfc, /// 0x390
			 0x993f6811, /// 0x394
			 0x4f424b51, /// 0x398
			 0x322d4091, /// 0x39c
			 0x083bde4e, /// 0x3a0
			 0xac976466, /// 0x3a4
			 0x1a088458, /// 0x3a8
			 0x07c47676, /// 0x3ac
			 0x4b8d75ae, /// 0x3b0
			 0x7ac60cf2, /// 0x3b4
			 0x8bf007e2, /// 0x3b8
			 0x9e3cb62f, /// 0x3bc
			 0x47503046, /// 0x3c0
			 0x70ecbfdc, /// 0x3c4
			 0x9ce39b08, /// 0x3c8
			 0xbe73a2b4, /// 0x3cc
			 0x3cafdd99, /// 0x3d0
			 0xd582e9a4, /// 0x3d4
			 0x6a440b15, /// 0x3d8
			 0xc0ca67c2, /// 0x3dc
			 0xf61e8132, /// 0x3e0
			 0x6b79a4ac, /// 0x3e4
			 0xa88cb380, /// 0x3e8
			 0x444ba359, /// 0x3ec
			 0x521a2657, /// 0x3f0
			 0x7c0ace91, /// 0x3f4
			 0xda422c5a, /// 0x3f8
			 0xabfa787c, /// 0x3fc
			 0x6c543bc6, /// 0x400
			 0x4b064e5d, /// 0x404
			 0x40c800b7, /// 0x408
			 0x75360c8a, /// 0x40c
			 0x0b892797, /// 0x410
			 0x0fe26ba6, /// 0x414
			 0x46572372, /// 0x418
			 0x4641298b, /// 0x41c
			 0x3dc97e21, /// 0x420
			 0xad657998, /// 0x424
			 0x46703ab3, /// 0x428
			 0xc015b1b2, /// 0x42c
			 0x61c486af, /// 0x430
			 0x84cc0587, /// 0x434
			 0x87e4cc06, /// 0x438
			 0xe5e6eb1a, /// 0x43c
			 0x3dc8439e, /// 0x440
			 0xce8888dd, /// 0x444
			 0x42a218fa, /// 0x448
			 0x822c5596, /// 0x44c
			 0x02d69a10, /// 0x450
			 0x137746dc, /// 0x454
			 0x8c7cac37, /// 0x458
			 0x8d506ad8, /// 0x45c
			 0x1e91d86f, /// 0x460
			 0x0371f823, /// 0x464
			 0xdbb78038, /// 0x468
			 0x78b062b4, /// 0x46c
			 0x022c2e8a, /// 0x470
			 0x50271b50, /// 0x474
			 0x4bdfe3d9, /// 0x478
			 0x469a23c1, /// 0x47c
			 0x27236072, /// 0x480
			 0xa0bee900, /// 0x484
			 0x2b27f071, /// 0x488
			 0x0e3b921d, /// 0x48c
			 0x27c3c669, /// 0x490
			 0xba3d764c, /// 0x494
			 0x8dd4eeb0, /// 0x498
			 0x85df873e, /// 0x49c
			 0x3323e8f3, /// 0x4a0
			 0x6cb6f544, /// 0x4a4
			 0xad30ecd9, /// 0x4a8
			 0x3b776341, /// 0x4ac
			 0xd5cfcc65, /// 0x4b0
			 0xecf77ff9, /// 0x4b4
			 0x02d4fb04, /// 0x4b8
			 0xdf0ec758, /// 0x4bc
			 0x69e4ab0d, /// 0x4c0
			 0x9173cd36, /// 0x4c4
			 0x379af15e, /// 0x4c8
			 0x2ce134f8, /// 0x4cc
			 0xbea7c1a1, /// 0x4d0
			 0x08dc5238, /// 0x4d4
			 0x4e7a817f, /// 0x4d8
			 0xf6353b9f, /// 0x4dc
			 0xabb99737, /// 0x4e0
			 0x30d4dc36, /// 0x4e4
			 0xac4592e1, /// 0x4e8
			 0x5bfcc48f, /// 0x4ec
			 0x56bad831, /// 0x4f0
			 0x5a23b860, /// 0x4f4
			 0xdc1b0da2, /// 0x4f8
			 0x10c605e7, /// 0x4fc
			 0x6138a50b, /// 0x500
			 0xf7bdba6f, /// 0x504
			 0xfd7dbb37, /// 0x508
			 0x4e4af36f, /// 0x50c
			 0x3402403e, /// 0x510
			 0x564c5183, /// 0x514
			 0x3687f98d, /// 0x518
			 0xf5c07f3f, /// 0x51c
			 0x3c14d6e3, /// 0x520
			 0xe43c6529, /// 0x524
			 0x41a4ac5d, /// 0x528
			 0x930c9702, /// 0x52c
			 0x67ba0178, /// 0x530
			 0x2ed8885b, /// 0x534
			 0xb529b681, /// 0x538
			 0xa41217ee, /// 0x53c
			 0x3b13633e, /// 0x540
			 0x25a957c6, /// 0x544
			 0xb2c9a9df, /// 0x548
			 0x4d860878, /// 0x54c
			 0x1cf8f815, /// 0x550
			 0x104a01a4, /// 0x554
			 0x4afbb107, /// 0x558
			 0x9f124497, /// 0x55c
			 0x0878f104, /// 0x560
			 0xbb4ea479, /// 0x564
			 0xd0c5d9e8, /// 0x568
			 0xf28d643e, /// 0x56c
			 0xc1c1a383, /// 0x570
			 0xd6256aae, /// 0x574
			 0x4c3074df, /// 0x578
			 0x7d42dda0, /// 0x57c
			 0xbcad6722, /// 0x580
			 0x23517d53, /// 0x584
			 0xdb8c089a, /// 0x588
			 0x267ff214, /// 0x58c
			 0xd848b36b, /// 0x590
			 0x129078f5, /// 0x594
			 0x373fdefa, /// 0x598
			 0x45ea87a1, /// 0x59c
			 0xb9765ec5, /// 0x5a0
			 0xcd2e7ed2, /// 0x5a4
			 0xa961b98c, /// 0x5a8
			 0xc0f9b323, /// 0x5ac
			 0x8dc861e3, /// 0x5b0
			 0x3537526f, /// 0x5b4
			 0x11f2caf9, /// 0x5b8
			 0x084ddbb5, /// 0x5bc
			 0x68b9200a, /// 0x5c0
			 0x368e21cf, /// 0x5c4
			 0x7ac059ec, /// 0x5c8
			 0x1b67c212, /// 0x5cc
			 0xf54a5074, /// 0x5d0
			 0x6c5f7b1c, /// 0x5d4
			 0x0d90d24b, /// 0x5d8
			 0xd477d4aa, /// 0x5dc
			 0x349586e7, /// 0x5e0
			 0xf32845d2, /// 0x5e4
			 0x05093a07, /// 0x5e8
			 0xc78dcaea, /// 0x5ec
			 0x5348f3d2, /// 0x5f0
			 0x70acae1a, /// 0x5f4
			 0xa3cf2498, /// 0x5f8
			 0x4058dd5f, /// 0x5fc
			 0x235bbc02, /// 0x600
			 0x9f02b032, /// 0x604
			 0x7f55170e, /// 0x608
			 0x291508a1, /// 0x60c
			 0x0b757e38, /// 0x610
			 0x22cacb05, /// 0x614
			 0xf1db0276, /// 0x618
			 0xa612ad12, /// 0x61c
			 0x18b18246, /// 0x620
			 0x30902f6a, /// 0x624
			 0x04831ada, /// 0x628
			 0x8d696f72, /// 0x62c
			 0x9c313db3, /// 0x630
			 0x1ba7ba5a, /// 0x634
			 0x89834b19, /// 0x638
			 0x756bf8ac, /// 0x63c
			 0x744f902d, /// 0x640
			 0xdbcf7a7a, /// 0x644
			 0x3550a43f, /// 0x648
			 0xf038d9ff, /// 0x64c
			 0xb3b14745, /// 0x650
			 0x0ff4ac15, /// 0x654
			 0x5a9365da, /// 0x658
			 0xd80a7023, /// 0x65c
			 0xaefd2130, /// 0x660
			 0x1a3fc920, /// 0x664
			 0xc318ad4a, /// 0x668
			 0x89d8ec66, /// 0x66c
			 0x5f85ebb0, /// 0x670
			 0xa065936d, /// 0x674
			 0x6b0036b6, /// 0x678
			 0xdc86502d, /// 0x67c
			 0x45920e17, /// 0x680
			 0xaf15f3b4, /// 0x684
			 0x23b56876, /// 0x688
			 0xa86fda19, /// 0x68c
			 0xdd5ec215, /// 0x690
			 0x887972c1, /// 0x694
			 0xf001efc1, /// 0x698
			 0x5be21297, /// 0x69c
			 0x8185f8ec, /// 0x6a0
			 0x509c0e05, /// 0x6a4
			 0x8f654838, /// 0x6a8
			 0xdc653b8d, /// 0x6ac
			 0x6e8888a2, /// 0x6b0
			 0xb4947991, /// 0x6b4
			 0x6962cf77, /// 0x6b8
			 0x176b6635, /// 0x6bc
			 0x077278f8, /// 0x6c0
			 0x593a3084, /// 0x6c4
			 0x90036f0b, /// 0x6c8
			 0x06c92a2d, /// 0x6cc
			 0x62906a5b, /// 0x6d0
			 0xed2c42dd, /// 0x6d4
			 0x4a22f970, /// 0x6d8
			 0x45343977, /// 0x6dc
			 0xaae64953, /// 0x6e0
			 0x71177793, /// 0x6e4
			 0x189f8ade, /// 0x6e8
			 0x127c0954, /// 0x6ec
			 0xf4f307fa, /// 0x6f0
			 0x26982c71, /// 0x6f4
			 0x8d82ed86, /// 0x6f8
			 0xbb696068, /// 0x6fc
			 0x702dc042, /// 0x700
			 0x8f0a9b3d, /// 0x704
			 0x58cea9a3, /// 0x708
			 0x3a95d9c9, /// 0x70c
			 0xea5db3f0, /// 0x710
			 0x18a66870, /// 0x714
			 0x113fdf3e, /// 0x718
			 0x62f0fa44, /// 0x71c
			 0x1ac00b5d, /// 0x720
			 0x48a850dc, /// 0x724
			 0x6576060c, /// 0x728
			 0x0caf7314, /// 0x72c
			 0x471158f0, /// 0x730
			 0x17182295, /// 0x734
			 0x325c0f9a, /// 0x738
			 0xd4f6c147, /// 0x73c
			 0x7ad0b3ae, /// 0x740
			 0xff702b3e, /// 0x744
			 0xaed08ccd, /// 0x748
			 0x707ba093, /// 0x74c
			 0xcab6f510, /// 0x750
			 0x2ebcba09, /// 0x754
			 0xbd1d6710, /// 0x758
			 0x8b17560c, /// 0x75c
			 0x7d6c7054, /// 0x760
			 0x4eaadc74, /// 0x764
			 0x401edd92, /// 0x768
			 0x3b1f38fb, /// 0x76c
			 0x69cc2a81, /// 0x770
			 0xd9795e2c, /// 0x774
			 0x52e7bfb5, /// 0x778
			 0xe1d33f22, /// 0x77c
			 0x67be09ac, /// 0x780
			 0xbf1f6810, /// 0x784
			 0x5c187404, /// 0x788
			 0x752bc8e5, /// 0x78c
			 0x0111eaf9, /// 0x790
			 0xd63a2046, /// 0x794
			 0x8fc7178f, /// 0x798
			 0xf662fb4e, /// 0x79c
			 0x92b212df, /// 0x7a0
			 0x5ff1a661, /// 0x7a4
			 0xdcc19c55, /// 0x7a8
			 0x4b316835, /// 0x7ac
			 0xfd15b651, /// 0x7b0
			 0xcff964ae, /// 0x7b4
			 0xd8b40fc8, /// 0x7b8
			 0x15d7973e, /// 0x7bc
			 0x109f9b16, /// 0x7c0
			 0x34318774, /// 0x7c4
			 0x5e78617a, /// 0x7c8
			 0x6ccc5f23, /// 0x7cc
			 0xf99077df, /// 0x7d0
			 0x52a51ae1, /// 0x7d4
			 0x253b60c1, /// 0x7d8
			 0x2d7633f1, /// 0x7dc
			 0x8a331811, /// 0x7e0
			 0xb0a718ea, /// 0x7e4
			 0xdef05fbe, /// 0x7e8
			 0x6c5eead5, /// 0x7ec
			 0x67e2d43e, /// 0x7f0
			 0xb7a59e7d, /// 0x7f4
			 0x845fffca, /// 0x7f8
			 0x84eda04f, /// 0x7fc
			 0xf77fe445, /// 0x800
			 0xe65e8bb6, /// 0x804
			 0x89c53896, /// 0x808
			 0xa7a2c77c, /// 0x80c
			 0xc891dae2, /// 0x810
			 0xac52a293, /// 0x814
			 0x616babd3, /// 0x818
			 0x3541080d, /// 0x81c
			 0x906fde96, /// 0x820
			 0xce485e08, /// 0x824
			 0x43b35102, /// 0x828
			 0xc7018ee0, /// 0x82c
			 0xf20d5d1d, /// 0x830
			 0xc1f23d28, /// 0x834
			 0x4018aadb, /// 0x838
			 0x35cac77f, /// 0x83c
			 0xc0040802, /// 0x840
			 0x80433394, /// 0x844
			 0xcaf3ebd0, /// 0x848
			 0xc76d1b4c, /// 0x84c
			 0x5189d469, /// 0x850
			 0xf68200d6, /// 0x854
			 0x5e20efd7, /// 0x858
			 0x7cf1a620, /// 0x85c
			 0x0422dc85, /// 0x860
			 0xf7087fef, /// 0x864
			 0x45c422fd, /// 0x868
			 0x710f50c8, /// 0x86c
			 0xefbad993, /// 0x870
			 0x1115fa51, /// 0x874
			 0x21334147, /// 0x878
			 0x7c060a02, /// 0x87c
			 0x1764c596, /// 0x880
			 0x72863951, /// 0x884
			 0x58533cd9, /// 0x888
			 0x1f75bcf0, /// 0x88c
			 0x3f2fa538, /// 0x890
			 0x9b16fb82, /// 0x894
			 0xd7f25e21, /// 0x898
			 0xb7bf19f2, /// 0x89c
			 0x538aa478, /// 0x8a0
			 0xa7176ceb, /// 0x8a4
			 0xab33520a, /// 0x8a8
			 0x8dd228d3, /// 0x8ac
			 0x2bcf3708, /// 0x8b0
			 0x0963cdf3, /// 0x8b4
			 0x72c54c5d, /// 0x8b8
			 0x24b03f18, /// 0x8bc
			 0x725fa685, /// 0x8c0
			 0xec36e514, /// 0x8c4
			 0xb42a580c, /// 0x8c8
			 0xc4819b0a, /// 0x8cc
			 0xd1b44755, /// 0x8d0
			 0x8cf677bb, /// 0x8d4
			 0x3f125f86, /// 0x8d8
			 0x4e4f9d2f, /// 0x8dc
			 0xff902697, /// 0x8e0
			 0xe2964609, /// 0x8e4
			 0x012e8533, /// 0x8e8
			 0x3bc7f597, /// 0x8ec
			 0xfa1a2ed8, /// 0x8f0
			 0xaaa7c3d5, /// 0x8f4
			 0x20993338, /// 0x8f8
			 0xb7172fa5, /// 0x8fc
			 0x1dfab31b, /// 0x900
			 0x5592e111, /// 0x904
			 0xd187a454, /// 0x908
			 0xfdfba678, /// 0x90c
			 0x4d478518, /// 0x910
			 0xa983c782, /// 0x914
			 0xb71283d4, /// 0x918
			 0x5ca940e2, /// 0x91c
			 0xd1eb98e9, /// 0x920
			 0xc3bdb7b7, /// 0x924
			 0xdae6d7ec, /// 0x928
			 0xe13c2a33, /// 0x92c
			 0x805223b4, /// 0x930
			 0x3e0fb168, /// 0x934
			 0x9193526e, /// 0x938
			 0x7afe43ab, /// 0x93c
			 0xdda1ae44, /// 0x940
			 0x8737ab61, /// 0x944
			 0x08816d7c, /// 0x948
			 0x2e590a09, /// 0x94c
			 0x2241e279, /// 0x950
			 0x64dad5c6, /// 0x954
			 0x345dafbd, /// 0x958
			 0x6fd6601d, /// 0x95c
			 0xa41d1c35, /// 0x960
			 0x3f7d8c81, /// 0x964
			 0xbbef1024, /// 0x968
			 0x932d206e, /// 0x96c
			 0x68ffe086, /// 0x970
			 0xff4dd62d, /// 0x974
			 0x07e6098d, /// 0x978
			 0x108239da, /// 0x97c
			 0xb6b70ae3, /// 0x980
			 0x4e7bc5f5, /// 0x984
			 0x64edb45c, /// 0x988
			 0x0b0530ca, /// 0x98c
			 0x8b59db73, /// 0x990
			 0x1921ad16, /// 0x994
			 0x43acfbc4, /// 0x998
			 0xc7cdd294, /// 0x99c
			 0xf1399ef5, /// 0x9a0
			 0xedfc833d, /// 0x9a4
			 0x8fa1f98b, /// 0x9a8
			 0x5b967928, /// 0x9ac
			 0x3a149998, /// 0x9b0
			 0x6b7db150, /// 0x9b4
			 0x39736064, /// 0x9b8
			 0x7f260718, /// 0x9bc
			 0x73780bc5, /// 0x9c0
			 0xb23315f9, /// 0x9c4
			 0x936f792c, /// 0x9c8
			 0x4f47e52c, /// 0x9cc
			 0x90787a88, /// 0x9d0
			 0xb8118ebc, /// 0x9d4
			 0x53271c7f, /// 0x9d8
			 0xf1f3c139, /// 0x9dc
			 0x41ac8e8d, /// 0x9e0
			 0x262a4c68, /// 0x9e4
			 0xecf06478, /// 0x9e8
			 0xe38bce2f, /// 0x9ec
			 0xb5ff4009, /// 0x9f0
			 0x33cbacd0, /// 0x9f4
			 0x5b307de0, /// 0x9f8
			 0x48a00bb4, /// 0x9fc
			 0x88c719f1, /// 0xa00
			 0x6852a92e, /// 0xa04
			 0x51ef9f9e, /// 0xa08
			 0x2d3e4d2d, /// 0xa0c
			 0x78a53fb0, /// 0xa10
			 0x8eaab25a, /// 0xa14
			 0x24110bc1, /// 0xa18
			 0x6e58e889, /// 0xa1c
			 0xf4314952, /// 0xa20
			 0x7d8197cf, /// 0xa24
			 0x40b8af5d, /// 0xa28
			 0x8bc38597, /// 0xa2c
			 0x6f5c5680, /// 0xa30
			 0xbef1f345, /// 0xa34
			 0xb1775fbb, /// 0xa38
			 0xf607b4f8, /// 0xa3c
			 0x062c0d34, /// 0xa40
			 0xd1138be0, /// 0xa44
			 0x08d36fd8, /// 0xa48
			 0x1c8e3b7a, /// 0xa4c
			 0xc3f19a0f, /// 0xa50
			 0x94814cf6, /// 0xa54
			 0x4cd5bec4, /// 0xa58
			 0x5f153953, /// 0xa5c
			 0x09db84f4, /// 0xa60
			 0x2dabf239, /// 0xa64
			 0x37ccae43, /// 0xa68
			 0xd692b119, /// 0xa6c
			 0x8810b61f, /// 0xa70
			 0xc3e01ca1, /// 0xa74
			 0xa85ab117, /// 0xa78
			 0x99615285, /// 0xa7c
			 0xaac671f3, /// 0xa80
			 0x79755d0c, /// 0xa84
			 0xbd462011, /// 0xa88
			 0x708cf7f3, /// 0xa8c
			 0xc04ccc9b, /// 0xa90
			 0x1bc86091, /// 0xa94
			 0x439168fa, /// 0xa98
			 0x90be2c39, /// 0xa9c
			 0x4d831fee, /// 0xaa0
			 0xeb2c14f5, /// 0xaa4
			 0x133d890b, /// 0xaa8
			 0xba98b02e, /// 0xaac
			 0x82973e2e, /// 0xab0
			 0x6746195c, /// 0xab4
			 0xd5322dfa, /// 0xab8
			 0xa4cf7478, /// 0xabc
			 0x68a1da56, /// 0xac0
			 0x2e3b321e, /// 0xac4
			 0x6e9d2c33, /// 0xac8
			 0xbc7bcaaf, /// 0xacc
			 0x478b23e0, /// 0xad0
			 0x0c35a6d6, /// 0xad4
			 0x65ca22e8, /// 0xad8
			 0x277e3908, /// 0xadc
			 0x40820672, /// 0xae0
			 0x97d4a912, /// 0xae4
			 0x6fc7b5cb, /// 0xae8
			 0x3932f2fb, /// 0xaec
			 0xd603221b, /// 0xaf0
			 0xd41cf9bb, /// 0xaf4
			 0x3f283a83, /// 0xaf8
			 0x2a820a9f, /// 0xafc
			 0xfaef3d12, /// 0xb00
			 0xd1b3ca1d, /// 0xb04
			 0xbdea88a6, /// 0xb08
			 0x9c800c93, /// 0xb0c
			 0x91cb0db7, /// 0xb10
			 0xb443441d, /// 0xb14
			 0xb218a0ae, /// 0xb18
			 0x94dbc145, /// 0xb1c
			 0x2cb14aca, /// 0xb20
			 0x37e6e158, /// 0xb24
			 0xb944ecc8, /// 0xb28
			 0x0262e74d, /// 0xb2c
			 0x01b44b27, /// 0xb30
			 0xe24c3401, /// 0xb34
			 0x7566a56f, /// 0xb38
			 0x711d9a5a, /// 0xb3c
			 0xb8232557, /// 0xb40
			 0x066e8026, /// 0xb44
			 0xe8783393, /// 0xb48
			 0x7d0bae16, /// 0xb4c
			 0xff748e9a, /// 0xb50
			 0xdb82515c, /// 0xb54
			 0xaf497854, /// 0xb58
			 0x1ecd0c33, /// 0xb5c
			 0x00961738, /// 0xb60
			 0x61fc3f4c, /// 0xb64
			 0x51213b7b, /// 0xb68
			 0xb154d758, /// 0xb6c
			 0x39351483, /// 0xb70
			 0x89b999c1, /// 0xb74
			 0xbfc4d70c, /// 0xb78
			 0xca1ffebd, /// 0xb7c
			 0xdf6f4ba0, /// 0xb80
			 0x3f7237f8, /// 0xb84
			 0x4a563099, /// 0xb88
			 0xcc8691e3, /// 0xb8c
			 0x805c40c0, /// 0xb90
			 0x5d53de88, /// 0xb94
			 0x7a601cfa, /// 0xb98
			 0x000a1026, /// 0xb9c
			 0x17f3eacb, /// 0xba0
			 0xe673d86b, /// 0xba4
			 0x2587f805, /// 0xba8
			 0xac1880fd, /// 0xbac
			 0x5141f7ae, /// 0xbb0
			 0x29cf9c63, /// 0xbb4
			 0x45c38e0f, /// 0xbb8
			 0x9bdcea34, /// 0xbbc
			 0xf90b15aa, /// 0xbc0
			 0xd8d45cbe, /// 0xbc4
			 0x9c273047, /// 0xbc8
			 0x692078f5, /// 0xbcc
			 0xe2344b83, /// 0xbd0
			 0x6ad2dbb9, /// 0xbd4
			 0x6d43820e, /// 0xbd8
			 0x745bec33, /// 0xbdc
			 0x99076284, /// 0xbe0
			 0x68ef36ef, /// 0xbe4
			 0x403967e5, /// 0xbe8
			 0xffdf1433, /// 0xbec
			 0xf0a68210, /// 0xbf0
			 0x0e851054, /// 0xbf4
			 0x420e7772, /// 0xbf8
			 0x945cfdba, /// 0xbfc
			 0x0e0ccb1e, /// 0xc00
			 0x788e162f, /// 0xc04
			 0x1f4a43ab, /// 0xc08
			 0x20e8ea0d, /// 0xc0c
			 0x89a65392, /// 0xc10
			 0xcbacfb57, /// 0xc14
			 0x98219646, /// 0xc18
			 0x85175ea4, /// 0xc1c
			 0x0c362960, /// 0xc20
			 0xff11c437, /// 0xc24
			 0x9d30ef6b, /// 0xc28
			 0xd63d8ffd, /// 0xc2c
			 0x9a02c6ef, /// 0xc30
			 0x8c02172e, /// 0xc34
			 0x9254f2a2, /// 0xc38
			 0x406f3af9, /// 0xc3c
			 0x61e23643, /// 0xc40
			 0x45da1a57, /// 0xc44
			 0x9b1ea482, /// 0xc48
			 0xb0c8ad90, /// 0xc4c
			 0x8bab912c, /// 0xc50
			 0xacde7f5a, /// 0xc54
			 0x71c097b7, /// 0xc58
			 0x66f67504, /// 0xc5c
			 0x1cb26a90, /// 0xc60
			 0x31fb11d6, /// 0xc64
			 0xa2e8b95b, /// 0xc68
			 0x8dba865d, /// 0xc6c
			 0x823c54a2, /// 0xc70
			 0x3df087e0, /// 0xc74
			 0x59c05d6e, /// 0xc78
			 0xf3d026a2, /// 0xc7c
			 0x7a7d7b07, /// 0xc80
			 0x958017e4, /// 0xc84
			 0xd927f775, /// 0xc88
			 0x1d297a52, /// 0xc8c
			 0xfdc680d8, /// 0xc90
			 0x8873dd63, /// 0xc94
			 0x1c5d9af7, /// 0xc98
			 0xffb3e5cc, /// 0xc9c
			 0xf5503dab, /// 0xca0
			 0xd8d4fc94, /// 0xca4
			 0x24433b39, /// 0xca8
			 0x2623fc27, /// 0xcac
			 0xdfc41744, /// 0xcb0
			 0x67224d2b, /// 0xcb4
			 0x7d47b4b6, /// 0xcb8
			 0x7b2e1d10, /// 0xcbc
			 0x9062388b, /// 0xcc0
			 0x0d424e37, /// 0xcc4
			 0x6dc208ee, /// 0xcc8
			 0x062de100, /// 0xccc
			 0x1b48ed5a, /// 0xcd0
			 0x4d37b0c5, /// 0xcd4
			 0x9a46e909, /// 0xcd8
			 0xd251a31a, /// 0xcdc
			 0x7189d300, /// 0xce0
			 0xfdb25b9d, /// 0xce4
			 0x7cf19a34, /// 0xce8
			 0x34672cfb, /// 0xcec
			 0xdeebccf0, /// 0xcf0
			 0xdf8bc30d, /// 0xcf4
			 0x68e4a613, /// 0xcf8
			 0xb72cb3f4, /// 0xcfc
			 0xaa59a0df, /// 0xd00
			 0x2bdbb7a9, /// 0xd04
			 0x2eabc387, /// 0xd08
			 0xa2f0e8d7, /// 0xd0c
			 0x67ff0f1b, /// 0xd10
			 0xd3a467cd, /// 0xd14
			 0xeea992ff, /// 0xd18
			 0xaa7601db, /// 0xd1c
			 0x48896708, /// 0xd20
			 0xb6a36b90, /// 0xd24
			 0xe3c331e7, /// 0xd28
			 0xd815c50d, /// 0xd2c
			 0x8db71331, /// 0xd30
			 0xd758c8cf, /// 0xd34
			 0x27fa0d88, /// 0xd38
			 0xcbd3780d, /// 0xd3c
			 0x7ebb4cac, /// 0xd40
			 0x2e5a52c2, /// 0xd44
			 0x51370299, /// 0xd48
			 0x49f763f2, /// 0xd4c
			 0x9c3da37e, /// 0xd50
			 0xe789b303, /// 0xd54
			 0xf5b39bf9, /// 0xd58
			 0x786df97f, /// 0xd5c
			 0xf961d366, /// 0xd60
			 0x3e97fd63, /// 0xd64
			 0xa07a3e3b, /// 0xd68
			 0xb9ed28e2, /// 0xd6c
			 0xa776e9b4, /// 0xd70
			 0x12c92063, /// 0xd74
			 0x5fec3d2e, /// 0xd78
			 0x8fa0f04a, /// 0xd7c
			 0x7bdd3d67, /// 0xd80
			 0x9bb38e6d, /// 0xd84
			 0xb2c6e1eb, /// 0xd88
			 0x9a94f716, /// 0xd8c
			 0x4f15aab9, /// 0xd90
			 0xad8c151f, /// 0xd94
			 0xb0b079f3, /// 0xd98
			 0xb93f6003, /// 0xd9c
			 0xcf6ad1f5, /// 0xda0
			 0xa3a955a3, /// 0xda4
			 0x35ed90f2, /// 0xda8
			 0x1bf7edd6, /// 0xdac
			 0xe0572ccf, /// 0xdb0
			 0x534ece3f, /// 0xdb4
			 0xab3374f6, /// 0xdb8
			 0xa7bb1521, /// 0xdbc
			 0xa30fb4cc, /// 0xdc0
			 0xf1d30323, /// 0xdc4
			 0x7784621a, /// 0xdc8
			 0x41b717d0, /// 0xdcc
			 0x4ebad695, /// 0xdd0
			 0x26565227, /// 0xdd4
			 0xcb55b984, /// 0xdd8
			 0xd47b5dab, /// 0xddc
			 0x94421247, /// 0xde0
			 0xd6f62490, /// 0xde4
			 0x71f2b029, /// 0xde8
			 0x0f40e09f, /// 0xdec
			 0x0f932d0e, /// 0xdf0
			 0xaf94289a, /// 0xdf4
			 0x1906ae89, /// 0xdf8
			 0x681bb2e3, /// 0xdfc
			 0x74ed3509, /// 0xe00
			 0x6cfc16fe, /// 0xe04
			 0x435c0736, /// 0xe08
			 0xfda3771a, /// 0xe0c
			 0x0cf86b32, /// 0xe10
			 0x91a66205, /// 0xe14
			 0xad4ca611, /// 0xe18
			 0x4db44f5f, /// 0xe1c
			 0xf1ab2d08, /// 0xe20
			 0x817d8a3a, /// 0xe24
			 0xd459e7ac, /// 0xe28
			 0x580dab26, /// 0xe2c
			 0xc96bdf62, /// 0xe30
			 0x69551833, /// 0xe34
			 0xfb4955ff, /// 0xe38
			 0x5a62ef76, /// 0xe3c
			 0x94f29571, /// 0xe40
			 0x1721cb29, /// 0xe44
			 0x00a65e08, /// 0xe48
			 0x662f9620, /// 0xe4c
			 0xebdf3e18, /// 0xe50
			 0x4f88dc62, /// 0xe54
			 0x18e5df86, /// 0xe58
			 0x7b0b7e78, /// 0xe5c
			 0xab6ccdab, /// 0xe60
			 0x61030265, /// 0xe64
			 0x6427008c, /// 0xe68
			 0x534b980f, /// 0xe6c
			 0x5b956247, /// 0xe70
			 0xdc31a5be, /// 0xe74
			 0xe20b32ac, /// 0xe78
			 0xdcf17722, /// 0xe7c
			 0xaa083b2f, /// 0xe80
			 0x398568e1, /// 0xe84
			 0xcf59b85d, /// 0xe88
			 0x6ff51340, /// 0xe8c
			 0xa4e0a654, /// 0xe90
			 0x31cc0277, /// 0xe94
			 0x319848a1, /// 0xe98
			 0x9bac208a, /// 0xe9c
			 0x20f6ec38, /// 0xea0
			 0x80b5293b, /// 0xea4
			 0xce2aef3a, /// 0xea8
			 0x3f66410f, /// 0xeac
			 0x786ddb52, /// 0xeb0
			 0x3ac13ff9, /// 0xeb4
			 0x54cf3da6, /// 0xeb8
			 0x10f5bbab, /// 0xebc
			 0x08aa6deb, /// 0xec0
			 0x622716bc, /// 0xec4
			 0xf5999a72, /// 0xec8
			 0xa7cc9209, /// 0xecc
			 0xaa554ff7, /// 0xed0
			 0x4e2446f3, /// 0xed4
			 0xbb5c4d9b, /// 0xed8
			 0xf8375f35, /// 0xedc
			 0x8e370272, /// 0xee0
			 0xf6d992a4, /// 0xee4
			 0xa416061e, /// 0xee8
			 0xfe145b10, /// 0xeec
			 0xe9bbda54, /// 0xef0
			 0xd7af1a2d, /// 0xef4
			 0x927af06b, /// 0xef8
			 0xac8d2323, /// 0xefc
			 0xafc30375, /// 0xf00
			 0x2a58e008, /// 0xf04
			 0xcfda1e7d, /// 0xf08
			 0x00c828b1, /// 0xf0c
			 0x5144cc25, /// 0xf10
			 0xd1e6a13d, /// 0xf14
			 0x3f94632b, /// 0xf18
			 0x10797a96, /// 0xf1c
			 0x4eabe179, /// 0xf20
			 0xb50b5298, /// 0xf24
			 0x020e0dc2, /// 0xf28
			 0x094f64e1, /// 0xf2c
			 0x2650a144, /// 0xf30
			 0x479305a4, /// 0xf34
			 0xe54da439, /// 0xf38
			 0x35cd563a, /// 0xf3c
			 0x0e672853, /// 0xf40
			 0x74f8277b, /// 0xf44
			 0xb60ec68a, /// 0xf48
			 0x3cfcd141, /// 0xf4c
			 0x68440c9b, /// 0xf50
			 0x43028140, /// 0xf54
			 0xbb3d530e, /// 0xf58
			 0xde59dadb, /// 0xf5c
			 0x1d78a866, /// 0xf60
			 0x62480d47, /// 0xf64
			 0xa2dd233c, /// 0xf68
			 0x04fc994f, /// 0xf6c
			 0x49d4a5ae, /// 0xf70
			 0xa6af5385, /// 0xf74
			 0x77203b92, /// 0xf78
			 0xdbb636a6, /// 0xf7c
			 0x2aad6700, /// 0xf80
			 0x9bf7aa42, /// 0xf84
			 0x8e89c370, /// 0xf88
			 0xd58b2597, /// 0xf8c
			 0x861d7cec, /// 0xf90
			 0xa17f2021, /// 0xf94
			 0x9c90fb2c, /// 0xf98
			 0xf1fe15b1, /// 0xf9c
			 0x96a8b513, /// 0xfa0
			 0xc6aa810f, /// 0xfa4
			 0x7cb22b23, /// 0xfa8
			 0x12435837, /// 0xfac
			 0x26654250, /// 0xfb0
			 0xeb7f8fce, /// 0xfb4
			 0x36a6b3d4, /// 0xfb8
			 0x1c6db0bb, /// 0xfbc
			 0xbb8c123f, /// 0xfc0
			 0x4d142ab6, /// 0xfc4
			 0x01b246bc, /// 0xfc8
			 0xcd4eb3f1, /// 0xfcc
			 0xaa7bbcee, /// 0xfd0
			 0xdaefb785, /// 0xfd4
			 0x6573ea0b, /// 0xfd8
			 0x2c416b0a, /// 0xfdc
			 0xc68516cf, /// 0xfe0
			 0xe6d8b758, /// 0xfe4
			 0x035b11bd, /// 0xfe8
			 0x19f58941, /// 0xfec
			 0x0fec962e, /// 0xff0
			 0xcbd88fc7, /// 0xff4
			 0xd7ffb81c, /// 0xff8
			 0xea930200 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00000
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00004
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00008
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0000c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00010
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00014
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00018
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0001c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00020
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00024
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00028
			 0x00011111,                                                  // 9.7958E-41                                  /// 0002c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00030
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00034
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00038
			 0x0c780000,                                                  // Leading 1s:                                 /// 0003c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00040
			 0x0c700000,                                                  // Leading 1s:                                 /// 00044
			 0x80011111,                                                  // -9.7958E-41                                 /// 00048
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00054
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00060
			 0x0c600000,                                                  // Leading 1s:                                 /// 00064
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00068
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00070
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00078
			 0x33333333,                                                  // 4 random values                             /// 0007c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00080
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00084
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00088
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00090
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00094
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00098
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x0c600000,                                                  // Leading 1s:                                 /// 000a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000ac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x33333333,                                                  // 4 random values                             /// 000d8
			 0xbf028f5c,                                                  // -0.51                                       /// 000dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00100
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00104
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00108
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00110
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00114
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00118
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0011c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00120
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00128
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0012c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00130
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0013c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00140
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00144
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00148
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0014c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00150
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00154
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00158
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0015c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00160
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00164
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00168
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0016c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x00000000,                                                  // zero                                        /// 00174
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0017c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00180
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00184
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00188
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0018c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00190
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00194
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00198
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0019c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001c8
			 0x7fc00001,                                                  // signaling NaN                               /// 001cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 001f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00200
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00208
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0020c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00210
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00214
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00218
			 0x00000000,                                                  // zero                                        /// 0021c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00220
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00224
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00230
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00234
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00238
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00240
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00244
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x4b000000,                                                  // 8388608.0                                   /// 0024c
			 0x4b000000,                                                  // 8388608.0                                   /// 00250
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00258
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0025c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00260
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00264
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00268
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0026c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00270
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00274
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x4b000000,                                                  // 8388608.0                                   /// 00280
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00284
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0028c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00290
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00294
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00298
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0029c
			 0x80011111,                                                  // -9.7958E-41                                 /// 002a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002e0
			 0x80000000,                                                  // -zero                                       /// 002e4
			 0x55555555,                                                  // 4 random values                             /// 002e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00300
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00308
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x7fc00001,                                                  // signaling NaN                               /// 00310
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00314
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00318
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0031c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00328
			 0x7f800000,                                                  // inf                                         /// 0032c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00330
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00338
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00344
			 0xffc00001,                                                  // -signaling NaN                              /// 00348
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0034c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00350
			 0x80011111,                                                  // -9.7958E-41                                 /// 00354
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00358
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00364
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00370
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00374
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00378
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00380
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00384
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00388
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00390
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00398
			 0xbf028f5c,                                                  // -0.51                                       /// 0039c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 003e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00400
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00408
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0040c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00414
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0041c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00420
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00428
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0042c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00434
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00438
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x0c600000,                                                  // Leading 1s:                                 /// 00444
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0044c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00454
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0045c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00460
			 0x00011111,                                                  // 9.7958E-41                                  /// 00464
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00468
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0046c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00474
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00478
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0047c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00484
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0048c
			 0xcb000000,                                                  // -8388608.0                                  /// 00490
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00498
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004b8
			 0xbf028f5c,                                                  // -0.51                                       /// 004bc
			 0xbf028f5c,                                                  // -0.51                                       /// 004c0
			 0xbf028f5c,                                                  // -0.51                                       /// 004c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0xffc00001,                                                  // -signaling NaN                              /// 0050c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00510
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00514
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00518
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0051c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00520
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00528
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00530
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0c600000,                                                  // Leading 1s:                                 /// 00538
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0053c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00540
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00544
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00548
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0054c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00550
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00554
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00558
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0055c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00568
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0056c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00574
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0057c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00580
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00584
			 0x0c700000,                                                  // Leading 1s:                                 /// 00588
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00594
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0059c
			 0x0c400000,                                                  // Leading 1s:                                 /// 005a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005f0
			 0xcb000000,                                                  // -8388608.0                                  /// 005f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00600
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00604
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00608
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0060c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00614
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00618
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0061c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00620
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00628
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0062c
			 0xff800000,                                                  // -inf                                        /// 00630
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00638
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0063c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00640
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00644
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00648
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0064c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00650
			 0x7f800000,                                                  // inf                                         /// 00654
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00658
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0065c
			 0xffc00001,                                                  // -signaling NaN                              /// 00660
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00664
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0066c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00670
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00680
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00694
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0069c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006f4
			 0xff800000,                                                  // -inf                                        /// 006f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00700
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00708
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00714
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00718
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0071c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00720
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0x0e000003,                                                  // Trailing 1s:                                /// 00728
			 0xcb000000,                                                  // -8388608.0                                  /// 0072c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00730
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0073c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00740
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00758
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00760
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00764
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00768
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0076c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00770
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00778
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0077c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00788
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00790
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00794
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00798
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0079c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007bc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00804
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0080c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00810
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00814
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0081c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00820
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00824
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00828
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0082c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0xff800000,                                                  // -inf                                        /// 00834
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00838
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0083c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00840
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00844
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00848
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0084c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00854
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0x0c600000,                                                  // Leading 1s:                                 /// 0085c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00860
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0xbf028f5c,                                                  // -0.51                                       /// 00868
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0086c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00870
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00874
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00878
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0087c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00880
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00884
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00888
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0088c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00890
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00898
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0089c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008b8
			 0xff800000,                                                  // -inf                                        /// 008bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 008c8
			 0xff800000,                                                  // -inf                                        /// 008cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008f8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00904
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00908
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00910
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00914
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00918
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0091c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00920
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00924
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00928
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0092c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00930
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x0c600000,                                                  // Leading 1s:                                 /// 0093c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00940
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00948
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00950
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00954
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00958
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0095c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00970
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00974
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0097c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00980
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00984
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00988
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00990
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00998
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0099c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 009b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 009c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009ec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009fc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a00
			 0x3f028f5c,                                                  // 0.51                                        /// 00a04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a0c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a14
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a1c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0xcb000000,                                                  // -8388608.0                                  /// 00a34
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a38
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a44
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a50
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a64
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a6c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00aa0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00aa4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aa8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00aac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ab4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ab8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00abc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ac0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ac8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00acc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ad0
			 0xff800000,                                                  // -inf                                        /// 00ad4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ad8
			 0x7fc00001,                                                  // signaling NaN                               /// 00adc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ae0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ae4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ae8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00aec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00af0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00af4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00afc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b10
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b18
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b20
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b30
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b38
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b3c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b44
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b48
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b50
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b54
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b58
			 0xffc00001,                                                  // -signaling NaN                              /// 00b5c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x3f028f5c,                                                  // 0.51                                        /// 00b68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b70
			 0x00000000,                                                  // zero                                        /// 00b74
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b80
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b88
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b8c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b9c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ba4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ba8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bb0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bb4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bb8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bbc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bc0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bc4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bcc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bd8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00be0
			 0x55555555,                                                  // 4 random values                             /// 00be4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bf0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bf4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bfc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c18
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c1c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c24
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0xbf028f5c,                                                  // -0.51                                       /// 00c2c
			 0x80000000,                                                  // -zero                                       /// 00c30
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0xcb000000,                                                  // -8388608.0                                  /// 00c38
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00c40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c44
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c54
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c58
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c5c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c60
			 0x55555555,                                                  // 4 random values                             /// 00c64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c68
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c78
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c90
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c9c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ca0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ca4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0xffc00001,                                                  // -signaling NaN                              /// 00cb0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cb8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cc0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cc4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cc8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ccc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cd4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ce8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cf0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cf4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cfc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d00
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d04
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d10
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d1c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0xbf028f5c,                                                  // -0.51                                       /// 00d24
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d28
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d40
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d48
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0xcb000000,                                                  // -8388608.0                                  /// 00d64
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d7c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d84
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d90
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d98
			 0x33333333,                                                  // 4 random values                             /// 00d9c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00da0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00da8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00db0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00db8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dc0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dc8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dcc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dd0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dd4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ddc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00de4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00de8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00df0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00df4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00df8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dfc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e00
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e04
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e28
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e2c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e5c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e60
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e68
			 0xbf028f5c,                                                  // -0.51                                       /// 00e6c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e70
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e7c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e84
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e8c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e9c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ea0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00eac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00eb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00eb4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00eb8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ebc
			 0x7f800000,                                                  // inf                                         /// 00ec0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ec4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ecc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ed0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ee4
			 0x00000000,                                                  // zero                                        /// 00ee8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ef0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ef4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ef8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f10
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f24
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f30
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f34
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f3c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f48
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f58
			 0x4b000000,                                                  // 8388608.0                                   /// 00f5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f68
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f6c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f74
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f7c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f80
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f84
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f90
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f94
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fa0
			 0x4b000000,                                                  // 8388608.0                                   /// 00fa4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fb8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fbc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fc0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fcc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fd0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fd4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fdc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fe0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fe8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ff4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0xcb000000                                                  // -8388608.0                                  /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00000
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00004
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00008
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0000c
			 0x55555555,                                                  // 4 random values                             /// 00010
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00018
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x33333333,                                                  // 4 random values                             /// 00024
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0002c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00030
			 0x7f800000,                                                  // inf                                         /// 00034
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0003c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00044
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00048
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x3f028f5c,                                                  // 0.51                                        /// 00050
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00058
			 0xbf028f5c,                                                  // -0.51                                       /// 0005c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00068
			 0x0e000003,                                                  // Trailing 1s:                                /// 0006c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00074
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00078
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0007c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00084
			 0x4b000000,                                                  // 8388608.0                                   /// 00088
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0008c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00090
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00094
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00098
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0009c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x55555555,                                                  // 4 random values                             /// 000fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00100
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00104
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0010c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00110
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00114
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0011c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00120
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00124
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00128
			 0x4b000000,                                                  // 8388608.0                                   /// 0012c
			 0x80000000,                                                  // -zero                                       /// 00130
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00138
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00140
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00144
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00148
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0014c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00154
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00160
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00164
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00168
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0c400000,                                                  // Leading 1s:                                 /// 00184
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00188
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0018c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00190
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00194
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00198
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c4
			 0xbf028f5c,                                                  // -0.51                                       /// 001c8
			 0xffc00001,                                                  // -signaling NaN                              /// 001cc
			 0x7f800000,                                                  // inf                                         /// 001d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001e8
			 0xff800000,                                                  // -inf                                        /// 001ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00208
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0021c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00220
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0022c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00238
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00240
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00248
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x00000000,                                                  // zero                                        /// 00250
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00254
			 0x0c400000,                                                  // Leading 1s:                                 /// 00258
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00264
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0026c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00270
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00274
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0027c
			 0xffc00001,                                                  // -signaling NaN                              /// 00280
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00288
			 0x0c400000,                                                  // Leading 1s:                                 /// 0028c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00290
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0c780000,                                                  // Leading 1s:                                 /// 00298
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 002f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00300
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00304
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00308
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0030c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00310
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00314
			 0x0e000007,                                                  // Trailing 1s:                                /// 00318
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0031c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00320
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00324
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00328
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00334
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00340
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00344
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00348
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0034c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00350
			 0xcb000000,                                                  // -8388608.0                                  /// 00354
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00358
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00360
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00364
			 0x0c400000,                                                  // Leading 1s:                                 /// 00368
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00378
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0037c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00380
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00390
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00394
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00398
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003a0
			 0x7fc00001,                                                  // signaling NaN                               /// 003a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003c0
			 0x7fc00001,                                                  // signaling NaN                               /// 003c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003d4
			 0x7f800000,                                                  // inf                                         /// 003d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003f0
			 0xcb000000,                                                  // -8388608.0                                  /// 003f4
			 0x4b000000,                                                  // 8388608.0                                   /// 003f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00400
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x3f028f5c,                                                  // 0.51                                        /// 00408
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00410
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00414
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0041c
			 0x4b000000,                                                  // 8388608.0                                   /// 00420
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00424
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00428
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0042c
			 0x7f800000,                                                  // inf                                         /// 00430
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00434
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00438
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0043c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00448
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0044c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00454
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0045c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00460
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00464
			 0x4b000000,                                                  // 8388608.0                                   /// 00468
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00470
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x55555555,                                                  // 4 random values                             /// 00478
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0047c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00480
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00484
			 0xff800000,                                                  // -inf                                        /// 00488
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0048c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00494
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00498
			 0x80011111,                                                  // -9.7958E-41                                 /// 0049c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0x55555555,                                                  // 4 random values                             /// 004c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004e8
			 0xffc00001,                                                  // -signaling NaN                              /// 004ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004f8
			 0x3f028f5c,                                                  // 0.51                                        /// 004fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00500
			 0x00000000,                                                  // zero                                        /// 00504
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x0c400000,                                                  // Leading 1s:                                 /// 00514
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00518
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0051c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x0c600000,                                                  // Leading 1s:                                 /// 0052c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00530
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0053c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00544
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0054c
			 0x7f800000,                                                  // inf                                         /// 00550
			 0xff800000,                                                  // -inf                                        /// 00554
			 0x0e000001,                                                  // Trailing 1s:                                /// 00558
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00564
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00574
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00578
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0057c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00580
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00584
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0058c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00590
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00598
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005b0
			 0xcb000000,                                                  // -8388608.0                                  /// 005b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005d0
			 0x4b000000,                                                  // 8388608.0                                   /// 005d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 005e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 005f4
			 0x7f800000,                                                  // inf                                         /// 005f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00600
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00610
			 0xff800000,                                                  // -inf                                        /// 00614
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00618
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0061c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00620
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00624
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00628
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0062c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00630
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00634
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00638
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00640
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00648
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00650
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00658
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0065c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00660
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00664
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00670
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00674
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00678
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0067c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00684
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0068c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00690
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006a0
			 0xcb000000,                                                  // -8388608.0                                  /// 006a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006ac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x4b000000,                                                  // 8388608.0                                   /// 006fc
			 0x7f800000,                                                  // inf                                         /// 00700
			 0xbf028f5c,                                                  // -0.51                                       /// 00704
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00710
			 0x0e000003,                                                  // Trailing 1s:                                /// 00714
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0071c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00724
			 0x00000000,                                                  // zero                                        /// 00728
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00730
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00734
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00738
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00740
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00744
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00748
			 0x4b000000,                                                  // 8388608.0                                   /// 0074c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00750
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00754
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0075c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00760
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00764
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x00000000,                                                  // zero                                        /// 0076c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00770
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0xcb000000,                                                  // -8388608.0                                  /// 0077c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0x0e000001,                                                  // Trailing 1s:                                /// 00784
			 0x55555555,                                                  // 4 random values                             /// 00788
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0078c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00790
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00798
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0079c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007a0
			 0xbf028f5c,                                                  // -0.51                                       /// 007a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 007c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 007d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0xbf028f5c,                                                  // -0.51                                       /// 007fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00800
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00804
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00808
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0080c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00810
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00814
			 0x0c700000,                                                  // Leading 1s:                                 /// 00818
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0081c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x33333333,                                                  // 4 random values                             /// 00824
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00828
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0082c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0c600000,                                                  // Leading 1s:                                 /// 00834
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00838
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0083c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00840
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00848
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00850
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00854
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00858
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0085c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00860
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00864
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00868
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0086c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00874
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00878
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0087c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00880
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00884
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00888
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00894
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0089c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 008ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008b4
			 0x4b000000,                                                  // 8388608.0                                   /// 008b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 008c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 008cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 008f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x80000000,                                                  // -zero                                       /// 00908
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00910
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00914
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00918
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00924
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00928
			 0x7fc00001,                                                  // signaling NaN                               /// 0092c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00930
			 0x7fc00001,                                                  // signaling NaN                               /// 00934
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0093c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00940
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00944
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00948
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0094c
			 0xcb000000,                                                  // -8388608.0                                  /// 00950
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00954
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00960
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00964
			 0x0e000007,                                                  // Trailing 1s:                                /// 00968
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0096c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00970
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00978
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0097c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00984
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00988
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00994
			 0x0c400000,                                                  // Leading 1s:                                 /// 00998
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0099c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009a4
			 0xcb000000,                                                  // -8388608.0                                  /// 009a8
			 0x7f800000,                                                  // inf                                         /// 009ac
			 0x7f800000,                                                  // inf                                         /// 009b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009c4
			 0x7f800000,                                                  // inf                                         /// 009c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x7fc00001,                                                  // signaling NaN                               /// 009d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a00
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a10
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a18
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a1c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a2c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a4c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a54
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a5c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00000000,                                                  // zero                                        /// 00a70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a7c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a80
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a84
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a88
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a8c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a90
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a98
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00aa4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00aa8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00aac
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ab0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ab4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ac8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ad0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ad4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ad8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00adc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ae0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ae4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ae8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00aec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00af4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00af8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b08
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b24
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b30
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b3c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b40
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b50
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b54
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b60
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b6c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b74
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b7c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b80
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b88
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b90
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b94
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ba4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ba8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0xffc00001,                                                  // -signaling NaN                              /// 00bb4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bc4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bc8
			 0xbf028f5c,                                                  // -0.51                                       /// 00bcc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bd0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bd4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bd8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bdc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00be0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bf0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bf8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bfc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c00
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c10
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c18
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c20
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c24
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c28
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c34
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c44
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c48
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c4c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c54
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c58
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c5c
			 0x4b000000,                                                  // 8388608.0                                   /// 00c60
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c70
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c74
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c80
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c8c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c98
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c9c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ca8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cb0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cb4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cbc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cc4
			 0x7fc00001,                                                  // signaling NaN                               /// 00cc8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ccc
			 0x33333333,                                                  // 4 random values                             /// 00cd0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cd8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ce0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cf4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d0c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d10
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d14
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d18
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d20
			 0x7fc00001,                                                  // signaling NaN                               /// 00d24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d2c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d50
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d54
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d60
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d70
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d74
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d80
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d84
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d98
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d9c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00da4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00db8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dcc
			 0xcb000000,                                                  // -8388608.0                                  /// 00dd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dd4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dd8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ddc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00de0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00df0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00df4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dfc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e00
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e18
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e24
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e38
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e44
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e48
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e5c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e88
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e8c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e90
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e94
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e9c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ea0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ea8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00eac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00eb0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eb4
			 0x4b000000,                                                  // 8388608.0                                   /// 00eb8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ebc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ec0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ec4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ec8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ed8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ee0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ee8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ef0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ef4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f04
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x55555555,                                                  // 4 random values                             /// 00f0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f10
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x7fc00001,                                                  // signaling NaN                               /// 00f18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f2c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f3c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f44
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f4c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f50
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f58
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f64
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x80000000,                                                  // -zero                                       /// 00f70
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f7c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f8c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f90
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fa0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fa4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fa8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fb0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fb8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fc0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fc4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fcc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fd4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fe0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fe4
			 0x33333333,                                                  // 4 random values                             /// 00fe8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ff4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ff8
			 0x00400000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x0000042c,
			 0x000003ec,
			 0x00000668,
			 0x00000058,
			 0x00000024,
			 0x000003f8,
			 0x000002d8,
			 0x000002bc,

			 /// vpu register f2
			 0x41200000,
			 0x41b00000,
			 0x41000000,
			 0x40400000,
			 0x3f800000,
			 0x41f80000,
			 0x41a80000,
			 0x41200000,

			 /// vpu register f3
			 0x42000000,
			 0x41600000,
			 0x41800000,
			 0x41800000,
			 0x41b80000,
			 0x41900000,
			 0x41e00000,
			 0x41f80000,

			 /// vpu register f4
			 0x41980000,
			 0x41000000,
			 0x41900000,
			 0x41000000,
			 0x40800000,
			 0x40400000,
			 0x41200000,
			 0x41b00000,

			 /// vpu register f5
			 0x41900000,
			 0x41e00000,
			 0x41e80000,
			 0x42000000,
			 0x41100000,
			 0x41300000,
			 0x41a00000,
			 0x41400000,

			 /// vpu register f6
			 0x41000000,
			 0x41880000,
			 0x41d80000,
			 0x40000000,
			 0x41600000,
			 0x41200000,
			 0x41700000,
			 0x41000000,

			 /// vpu register f7
			 0x41c80000,
			 0x40400000,
			 0x41b00000,
			 0x40800000,
			 0x41100000,
			 0x41400000,
			 0x41b00000,
			 0x41900000,

			 /// vpu register f8
			 0x41c00000,
			 0x41880000,
			 0x40400000,
			 0x41000000,
			 0x41d80000,
			 0x41b80000,
			 0x40000000,
			 0x41b80000,

			 /// vpu register f9
			 0x41b80000,
			 0x41c00000,
			 0x40000000,
			 0x40000000,
			 0x41a80000,
			 0x41880000,
			 0x41880000,
			 0x41700000,

			 /// vpu register f10
			 0x41f00000,
			 0x41800000,
			 0x41d00000,
			 0x41600000,
			 0x41800000,
			 0x41a80000,
			 0x41c80000,
			 0x41880000,

			 /// vpu register f11
			 0x41800000,
			 0x41e00000,
			 0x41100000,
			 0x41d00000,
			 0x41a00000,
			 0x41e80000,
			 0x41b80000,
			 0x40a00000,

			 /// vpu register f12
			 0x40000000,
			 0x41980000,
			 0x40800000,
			 0x40c00000,
			 0x40800000,
			 0x41e00000,
			 0x41a00000,
			 0x41980000,

			 /// vpu register f13
			 0x41b00000,
			 0x3f800000,
			 0x41e80000,
			 0x41980000,
			 0x40000000,
			 0x41f00000,
			 0x40c00000,
			 0x41880000,

			 /// vpu register f14
			 0x41d80000,
			 0x41d80000,
			 0x41000000,
			 0x41800000,
			 0x41200000,
			 0x41800000,
			 0x40000000,
			 0x40e00000,

			 /// vpu register f15
			 0x41a80000,
			 0x41d00000,
			 0x40c00000,
			 0x41300000,
			 0x40000000,
			 0x41500000,
			 0x41800000,
			 0x40e00000,

			 /// vpu register f16
			 0x41700000,
			 0x41c80000,
			 0x41e00000,
			 0x41500000,
			 0x41c80000,
			 0x40e00000,
			 0x41700000,
			 0x41700000,

			 /// vpu register f17
			 0x41c80000,
			 0x41e00000,
			 0x41e80000,
			 0x41a00000,
			 0x41980000,
			 0x41d80000,
			 0x41700000,
			 0x42000000,

			 /// vpu register f18
			 0x41500000,
			 0x41f80000,
			 0x40c00000,
			 0x41a00000,
			 0x41b80000,
			 0x41d80000,
			 0x41d00000,
			 0x40000000,

			 /// vpu register f19
			 0x41f80000,
			 0x41000000,
			 0x40800000,
			 0x41b00000,
			 0x41a00000,
			 0x41f00000,
			 0x41e00000,
			 0x41980000,

			 /// vpu register f20
			 0x40800000,
			 0x41800000,
			 0x41c00000,
			 0x41600000,
			 0x41200000,
			 0x40400000,
			 0x41400000,
			 0x41c80000,

			 /// vpu register f21
			 0x41e00000,
			 0x41e00000,
			 0x41100000,
			 0x41b00000,
			 0x41980000,
			 0x41e00000,
			 0x42000000,
			 0x41880000,

			 /// vpu register f22
			 0x40800000,
			 0x40e00000,
			 0x41880000,
			 0x41d80000,
			 0x3f800000,
			 0x40800000,
			 0x41e80000,
			 0x41400000,

			 /// vpu register f23
			 0x41d00000,
			 0x41f00000,
			 0x41c00000,
			 0x41e80000,
			 0x41200000,
			 0x41b00000,
			 0x3f800000,
			 0x41900000,

			 /// vpu register f24
			 0x41d80000,
			 0x41a80000,
			 0x40c00000,
			 0x3f800000,
			 0x41e80000,
			 0x41980000,
			 0x41200000,
			 0x41400000,

			 /// vpu register f25
			 0x41b00000,
			 0x41600000,
			 0x41f00000,
			 0x41e00000,
			 0x41800000,
			 0x41b00000,
			 0x41c00000,
			 0x41a80000,

			 /// vpu register f26
			 0x41800000,
			 0x41600000,
			 0x41000000,
			 0x41200000,
			 0x41c80000,
			 0x40000000,
			 0x40400000,
			 0x41880000,

			 /// vpu register f27
			 0x3f800000,
			 0x41b00000,
			 0x41500000,
			 0x41a80000,
			 0x41400000,
			 0x41d00000,
			 0x41f80000,
			 0x40a00000,

			 /// vpu register f28
			 0x41000000,
			 0x40400000,
			 0x41100000,
			 0x41980000,
			 0x41d80000,
			 0x40a00000,
			 0x40e00000,
			 0x41400000,

			 /// vpu register f29
			 0x41e80000,
			 0x41d00000,
			 0x40800000,
			 0x41880000,
			 0x40800000,
			 0x41b80000,
			 0x40a00000,
			 0x41e80000,

			 /// vpu register f30
			 0x41e00000,
			 0x41900000,
			 0x41900000,
			 0x41800000,
			 0x41f00000,
			 0x41800000,
			 0x41980000,
			 0x41300000,

			 /// vpu register f31
			 0x41f00000,
			 0x41d00000,
			 0x40e00000,
			 0x41d80000,
			 0x41e80000,
			 0x41200000,
			 0x40800000,
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
		"fcmovm.ps f25, f11, f9\n"                            ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f28, f13, f14\n"                           ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f24, f2, f7\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f28, f1, f28\n"                            ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f11, f5, f1\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f3, f7, f17\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f9, f9, f1\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f25, f7, f30\n"                            ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f16, f21, f17\n"                           ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f23, f24, f11\n"                           ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f3, f9, f16\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f29, f19, f0\n"                            ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f8, f22, f1\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f5, f18, f10\n"                            ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f15, f10, f22\n"                           ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f30, f13, f25\n"                           ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f20, f23, f2\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f11, f20, f3\n"                            ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f27, f21, f14\n"                           ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f8, f5, f9\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f6, f15, f30\n"                            ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f5, f0, f0\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f22, f5, f12\n"                            ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f18, f11, f2\n"                            ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f24, f0, f4\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f20, f3, f21\n"                            ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f2, f18, f13\n"                            ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f13, f9, f25\n"                            ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f17, f24, f16\n"                           ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f20, f5, f8\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f6, f11, f26\n"                            ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f5, f29, f19\n"                            ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f9, f14, f23\n"                            ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f22, f7, f15\n"                            ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f17, f17, f28\n"                           ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f4, f1, f8\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f21, f2, f25\n"                            ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f18, f7, f6\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f11, f15, f18\n"                           ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f2, f4, f10\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f12, f8, f20\n"                            ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f14, f29, f11\n"                           ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f3, f2, f7\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f8, f7, f19\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f8, f21, f26\n"                            ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f10, f22, f3\n"                            ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f5, f23, f2\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f16, f8, f19\n"                            ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f21, f20, f23\n"                           ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f19, f18, f23\n"                           ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f20, f20, f3\n"                            ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f20, f24, f7\n"                            ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f9, f22, f20\n"                            ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f4, f30, f8\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f21, f29, f7\n"                            ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f11, f14, f17\n"                           ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f10, f7, f19\n"                            ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f10, f14, f19\n"                           ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f14, f30, f14\n"                           ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f3, f15, f5\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f29, f9, f10\n"                            ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f10, f15, f22\n"                           ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f11, f10, f20\n"                           ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f24, f7, f7\n"                             ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f16, f3, f25\n"                            ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f27, f19, f0\n"                            ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f21, f8, f8\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f9, f23, f10\n"                            ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f9, f4, f21\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f30, f14, f28\n"                           ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f12, f23, f19\n"                           ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f11, f29, f26\n"                           ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f5, f15, f8\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f10, f6, f13\n"                            ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f9, f1, f25\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f5, f11, f22\n"                            ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f27, f28, f19\n"                           ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f4, f22, f14\n"                            ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f29, f29, f28\n"                           ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f4, f27, f15\n"                            ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f14, f29, f30\n"                           ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f15, f23, f17\n"                           ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f24, f4, f24\n"                            ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f24, f24, f24\n"                           ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f7, f14, f7\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f6, f21, f17\n"                            ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f28, f1, f15\n"                            ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f11, f2, f27\n"                            ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f24, f8, f16\n"                            ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f4, f14, f13\n"                            ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f28, f24, f28\n"                           ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f14, f14, f16\n"                           ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f29, f22, f28\n"                           ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f23, f23, f20\n"                           ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f20, f12, f4\n"                            ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f19, f15, f22\n"                           ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f17, f15, f6\n"                            ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f26, f7, f12\n"                            ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcmovm.ps f20, f29, f11\n"                           ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
