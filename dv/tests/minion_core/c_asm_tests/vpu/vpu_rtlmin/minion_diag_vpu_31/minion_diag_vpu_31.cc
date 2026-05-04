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
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00004
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00010
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00014
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00018
			 0xbf028f5c,                                                  // -0.51                                       /// 0001c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00024
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00028
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0002c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00034
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00038
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00040
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00011111,                                                  // 9.7958E-41                                  /// 00048
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0004c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00050
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00058
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0005c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00060
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00064
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0006c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00074
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00080
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00084
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00088
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0008c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00090
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00094
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00098
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0009c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000e4
			 0x33333333,                                                  // 4 random values                             /// 000e8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00100
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0010c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00110
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00118
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0011c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00124
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0012c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00130
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00134
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00138
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x00000000,                                                  // zero                                        /// 00140
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00144
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0014c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00150
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00154
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00158
			 0x33333333,                                                  // 4 random values                             /// 0015c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00164
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00168
			 0x4b000000,                                                  // 8388608.0                                   /// 0016c
			 0x4b000000,                                                  // 8388608.0                                   /// 00170
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00174
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0017c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00180
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00184
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00188
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x0e000007,                                                  // Trailing 1s:                                /// 00194
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0xff800000,                                                  // -inf                                        /// 001b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 001e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00204
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00208
			 0x7fc00001,                                                  // signaling NaN                               /// 0020c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00214
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00218
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0021c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00220
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00224
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00228
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0022c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00234
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00238
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0023c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00240
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00244
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0024c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00250
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00254
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00260
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00264
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0026c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00274
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0027c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00288
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00290
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0029c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002a0
			 0xff800000,                                                  // -inf                                        /// 002a4
			 0x55555555,                                                  // 4 random values                             /// 002a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 002c8
			 0xffc00001,                                                  // -signaling NaN                              /// 002cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002d8
			 0xffc00001,                                                  // -signaling NaN                              /// 002dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002e0
			 0xbf028f5c,                                                  // -0.51                                       /// 002e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00300
			 0x0c600000,                                                  // Leading 1s:                                 /// 00304
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00308
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0030c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00310
			 0x0c780000,                                                  // Leading 1s:                                 /// 00314
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00324
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00328
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00330
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00338
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0033c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00340
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00344
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00348
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00354
			 0x00011111,                                                  // 9.7958E-41                                  /// 00358
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00360
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00370
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00378
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0037c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00380
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00384
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0038c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00390
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00394
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00398
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 003b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003d4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00404
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00408
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0040c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00410
			 0x0e000007,                                                  // Trailing 1s:                                /// 00414
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80011111,                                                  // -9.7958E-41                                 /// 0041c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00424
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0042c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00430
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00434
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0043c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x7f800000,                                                  // inf                                         /// 00444
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00448
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x7fc00001,                                                  // signaling NaN                               /// 00450
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00454
			 0x3f028f5c,                                                  // 0.51                                        /// 00458
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0045c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00464
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00468
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0046c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00470
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00474
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0047c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00480
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00484
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0048c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00498
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0049c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 004a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 004b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 004c4
			 0xff800000,                                                  // -inf                                        /// 004c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004f0
			 0x7f800000,                                                  // inf                                         /// 004f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00504
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00508
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00510
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00514
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x7fc00001,                                                  // signaling NaN                               /// 00524
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00534
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00538
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0053c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00540
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00544
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00548
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00550
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x33333333,                                                  // 4 random values                             /// 00558
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00570
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0057c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00580
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00584
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00594
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00598
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x0e000001,                                                  // Trailing 1s:                                /// 005a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005c4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005dc
			 0xbf028f5c,                                                  // -0.51                                       /// 005e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005f4
			 0x00000000,                                                  // zero                                        /// 005f8
			 0xffc00001,                                                  // -signaling NaN                              /// 005fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00600
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00604
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00610
			 0xcb000000,                                                  // -8388608.0                                  /// 00614
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00618
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0061c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00620
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00628
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0062c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00630
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00638
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0063c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00640
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00644
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00648
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0064c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00654
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00658
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00664
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00668
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0066c
			 0x7fc00001,                                                  // signaling NaN                               /// 00670
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00674
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00678
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00680
			 0x0c600000,                                                  // Leading 1s:                                 /// 00684
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0068c
			 0x80000000,                                                  // -zero                                       /// 00690
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00694
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00698
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 006c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006cc
			 0xffc00001,                                                  // -signaling NaN                              /// 006d0
			 0xffc00001,                                                  // -signaling NaN                              /// 006d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006f0
			 0x55555555,                                                  // 4 random values                             /// 006f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x00000000,                                                  // zero                                        /// 00700
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00704
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0070c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00710
			 0x0e000001,                                                  // Trailing 1s:                                /// 00714
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00718
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x80000000,                                                  // -zero                                       /// 00728
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0072c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00738
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0073c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00748
			 0x3f028f5c,                                                  // 0.51                                        /// 0074c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00750
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00754
			 0x0e000007,                                                  // Trailing 1s:                                /// 00758
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0075c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x00011111,                                                  // 9.7958E-41                                  /// 00764
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0076c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00770
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00774
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00778
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0077c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00780
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00784
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0078c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00794
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00798
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0079c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x55555555,                                                  // 4 random values                             /// 007c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007d4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0xffc00001,                                                  // -signaling NaN                              /// 007f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00814
			 0xff800000,                                                  // -inf                                        /// 00818
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00820
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00824
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0082c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00830
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00838
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0083c
			 0xbf028f5c,                                                  // -0.51                                       /// 00840
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00848
			 0x00000000,                                                  // zero                                        /// 0084c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00850
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00858
			 0xcb000000,                                                  // -8388608.0                                  /// 0085c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00860
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00864
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00868
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00870
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00878
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0087c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00880
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00884
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00888
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0088c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00894
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00898
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0089c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008e4
			 0xcb000000,                                                  // -8388608.0                                  /// 008e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00900
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00908
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0090c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00910
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00914
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00920
			 0x0c400000,                                                  // Leading 1s:                                 /// 00924
			 0x4b000000,                                                  // 8388608.0                                   /// 00928
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00938
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0093c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00940
			 0x7fc00001,                                                  // signaling NaN                               /// 00944
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00948
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00950
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00958
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0095c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00960
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00964
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00968
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0096c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00970
			 0x0c700000,                                                  // Leading 1s:                                 /// 00974
			 0x55555555,                                                  // 4 random values                             /// 00978
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0097c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00984
			 0x3f028f5c,                                                  // 0.51                                        /// 00988
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00994
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00998
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0099c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 009a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009dc
			 0x55555555,                                                  // 4 random values                             /// 009e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a08
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a1c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a20
			 0x33333333,                                                  // 4 random values                             /// 00a24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a34
			 0xff800000,                                                  // -inf                                        /// 00a38
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x80000000,                                                  // -zero                                       /// 00a40
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a48
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a5c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a64
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a6c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a70
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x7f800000,                                                  // inf                                         /// 00a78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a80
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a84
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a88
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a8c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a98
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a9c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00aa4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ab0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ab4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ab8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ac8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00acc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ad8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ae0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00af0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00af8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00afc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b00
			 0xcb000000,                                                  // -8388608.0                                  /// 00b04
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b08
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b18
			 0xff800000,                                                  // -inf                                        /// 00b1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b30
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b38
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b48
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b4c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b64
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b68
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x3f028f5c,                                                  // 0.51                                        /// 00b74
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b78
			 0x4b000000,                                                  // 8388608.0                                   /// 00b7c
			 0x00000000,                                                  // zero                                        /// 00b80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b84
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b88
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b8c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b94
			 0x55555555,                                                  // 4 random values                             /// 00b98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b9c
			 0xff800000,                                                  // -inf                                        /// 00ba0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ba4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ba8
			 0xbf028f5c,                                                  // -0.51                                       /// 00bac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bb0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bc0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bc4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bd4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bd8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bdc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00be0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00be8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bf0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bf4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bf8
			 0x80000000,                                                  // -zero                                       /// 00bfc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c08
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c0c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c18
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c1c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c20
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c2c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c38
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c3c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c48
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c50
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c70
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c78
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c7c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c84
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c98
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ca4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ca8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cb4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cbc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cc0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cc4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cc8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ccc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cd0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cd4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cd8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cdc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ce4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ce8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cf0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cf4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cf8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cfc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d08
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d10
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d14
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d18
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d1c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d24
			 0x80000000,                                                  // -zero                                       /// 00d28
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d2c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d30
			 0x3f028f5c,                                                  // 0.51                                        /// 00d34
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d40
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0xbf028f5c,                                                  // -0.51                                       /// 00d48
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d54
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d58
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d70
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d84
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d90
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d94
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d98
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00da0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00da8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00db0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00db4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0xff800000,                                                  // -inf                                        /// 00dc4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dd0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dd4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ddc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00de0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00de4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00df0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00df4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00df8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dfc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e08
			 0x4b000000,                                                  // 8388608.0                                   /// 00e0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e10
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e18
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e20
			 0x4b000000,                                                  // 8388608.0                                   /// 00e24
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e28
			 0x00000000,                                                  // zero                                        /// 00e2c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e34
			 0x55555555,                                                  // 4 random values                             /// 00e38
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e68
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e78
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e7c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x3f028f5c,                                                  // 0.51                                        /// 00e8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e90
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e98
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ea0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ea4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ea8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eb0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00eb4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ebc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ec0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ec8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ecc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ed0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ed4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ee0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ee4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ee8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00eec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ef0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ef4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00efc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f04
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f0c
			 0x80000000,                                                  // -zero                                       /// 00f10
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f14
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f30
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f38
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f48
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f4c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f78
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f98
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fa4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fa8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fb0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fb4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fb8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fc0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fe8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ff4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x7fbfffff // SNaN                                          // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00000
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00004
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00010
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00014
			 0x00011111,                                                  // 9.7958E-41                                  /// 00018
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0001c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00020
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00024
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0002c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00034
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00038
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0003c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00040
			 0x55555555,                                                  // 4 random values                             /// 00044
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00048
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00054
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00058
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0005c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00060
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00064
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00068
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0006c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00078
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00080
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0009c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 000f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00104
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00108
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0010c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00110
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00114
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00118
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0011c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00120
			 0x0c400000,                                                  // Leading 1s:                                 /// 00124
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00130
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0013c
			 0x00000000,                                                  // zero                                        /// 00140
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00150
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00158
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0015c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00160
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x55555555,                                                  // 4 random values                             /// 00168
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0016c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00170
			 0x55555555,                                                  // 4 random values                             /// 00174
			 0xbf028f5c,                                                  // -0.51                                       /// 00178
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00184
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00188
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00190
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00194
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00198
			 0x80011111,                                                  // -9.7958E-41                                 /// 0019c
			 0xffc00001,                                                  // -signaling NaN                              /// 001a0
			 0x7f800000,                                                  // inf                                         /// 001a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 001a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 001c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001e0
			 0x00000000,                                                  // zero                                        /// 001e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00200
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00204
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0020c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00214
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00228
			 0x0e000001,                                                  // Trailing 1s:                                /// 0022c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00230
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00234
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00238
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0023c
			 0x4b000000,                                                  // 8388608.0                                   /// 00240
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00248
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0024c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00250
			 0xcb000000,                                                  // -8388608.0                                  /// 00254
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00258
			 0x3f028f5c,                                                  // 0.51                                        /// 0025c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00260
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00264
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00268
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0026c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00274
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x33333333,                                                  // 4 random values                             /// 0027c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x7fc00001,                                                  // signaling NaN                               /// 00284
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00288
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0028c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00294
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00298
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0029c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002c8
			 0xff800000,                                                  // -inf                                        /// 002cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002d4
			 0xbf028f5c,                                                  // -0.51                                       /// 002d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 002f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002f8
			 0x55555555,                                                  // 4 random values                             /// 002fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00304
			 0x0e000003,                                                  // Trailing 1s:                                /// 00308
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00310
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00314
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00318
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00320
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00324
			 0x80011111,                                                  // -9.7958E-41                                 /// 00328
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00330
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00334
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0033c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00344
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00348
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0034c
			 0x7f800000,                                                  // inf                                         /// 00350
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00358
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x0e000007,                                                  // Trailing 1s:                                /// 0036c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00374
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00378
			 0x0c700000,                                                  // Leading 1s:                                 /// 0037c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00384
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0038c
			 0x55555555,                                                  // 4 random values                             /// 00390
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x0c400000,                                                  // Leading 1s:                                 /// 00398
			 0x0e000007,                                                  // Trailing 1s:                                /// 0039c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x7fc00001,                                                  // signaling NaN                               /// 003c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003f4
			 0x80000000,                                                  // -zero                                       /// 003f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00400
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00404
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00408
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0041c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00424
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00428
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0042c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00430
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00434
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00444
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00448
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x33333333,                                                  // 4 random values                             /// 00450
			 0x4b000000,                                                  // 8388608.0                                   /// 00454
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x0e000003,                                                  // Trailing 1s:                                /// 0045c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00460
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00464
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00468
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00470
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00474
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0047c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00480
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00484
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00488
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00494
			 0x0e000001,                                                  // Trailing 1s:                                /// 00498
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0049c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004cc
			 0x80011111,                                                  // -9.7958E-41                                 /// 004d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004f0
			 0x7f800000,                                                  // inf                                         /// 004f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00500
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00504
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00508
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0050c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00514
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00518
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0051c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00528
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x33333333,                                                  // 4 random values                             /// 00530
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0053c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00540
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00544
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00548
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0054c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00554
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00558
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00560
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00564
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0056c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00570
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00580
			 0x7f800000,                                                  // inf                                         /// 00584
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00588
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0058c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00590
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00594
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00598
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0059c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x00000000,                                                  // zero                                        /// 005b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00604
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0060c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00610
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00614
			 0x3f028f5c,                                                  // 0.51                                        /// 00618
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00630
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x3f028f5c,                                                  // 0.51                                        /// 00638
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0063c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00640
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00650
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00654
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00658
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0065c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00660
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00668
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0066c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00670
			 0x0e000003,                                                  // Trailing 1s:                                /// 00674
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00680
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00684
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00688
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0068c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00690
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00694
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00698
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0069c
			 0x80000000,                                                  // -zero                                       /// 006a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x4b000000,                                                  // 8388608.0                                   /// 006dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00708
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00714
			 0x00000000,                                                  // zero                                        /// 00718
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00720
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00728
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0072c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00738
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0073c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00740
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00744
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00748
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0074c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00754
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0075c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00760
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00764
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00768
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0076c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00770
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0c780000,                                                  // Leading 1s:                                 /// 00778
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00780
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0078c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00790
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0079c
			 0x33333333,                                                  // 4 random values                             /// 007a0
			 0x3f028f5c,                                                  // 0.51                                        /// 007a4
			 0x7f800000,                                                  // inf                                         /// 007a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 007fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00800
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00804
			 0xffc00001,                                                  // -signaling NaN                              /// 00808
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0080c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00814
			 0xffc00001,                                                  // -signaling NaN                              /// 00818
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0081c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00820
			 0x4b000000,                                                  // 8388608.0                                   /// 00824
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00828
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00838
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0083c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00840
			 0xcb000000,                                                  // -8388608.0                                  /// 00844
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00848
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00850
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00854
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00858
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0085c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00860
			 0x80000000,                                                  // -zero                                       /// 00864
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00868
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0086c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00870
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00874
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00878
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00880
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00884
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00888
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0088c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00898
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0089c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008d4
			 0x33333333,                                                  // 4 random values                             /// 008d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00900
			 0xffc00001,                                                  // -signaling NaN                              /// 00904
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0090c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00910
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00914
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0091c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00924
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x0e000007,                                                  // Trailing 1s:                                /// 0092c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00930
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00938
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0093c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00940
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00944
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00948
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00950
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x0e000007,                                                  // Trailing 1s:                                /// 00958
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0095c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00960
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00964
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00968
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0096c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00978
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0097c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00980
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00988
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00990
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00998
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0099c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009e8
			 0x7f800000,                                                  // inf                                         /// 009ec
			 0x55555555,                                                  // 4 random values                             /// 009f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a00
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a08
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a0c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a24
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a28
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a2c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a44
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a4c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a54
			 0x80000000,                                                  // -zero                                       /// 00a58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a5c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a74
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a94
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aa0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00aa4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00aa8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ab0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ab4
			 0x00000000,                                                  // zero                                        /// 00ab8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00abc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ac0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00acc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ad0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ad4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ad8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00adc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ae0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ae8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00aec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00af0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00af4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00afc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b08
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b0c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b1c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b20
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b30
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b34
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b38
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b44
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b5c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b64
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b68
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b80
			 0x80000000,                                                  // -zero                                       /// 00b84
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b88
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x33333333,                                                  // 4 random values                             /// 00b94
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b98
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ba4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ba8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bb0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bbc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0xff800000,                                                  // -inf                                        /// 00bc4
			 0xffc00001,                                                  // -signaling NaN                              /// 00bc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bcc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bd4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bd8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bdc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00be8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bf4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bf8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bfc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c00
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c24
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c30
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c34
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c38
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c44
			 0x33333333,                                                  // 4 random values                             /// 00c48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c50
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c58
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c70
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c7c
			 0xff800000,                                                  // -inf                                        /// 00c80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c88
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c8c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c90
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c9c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ca0
			 0x33333333,                                                  // 4 random values                             /// 00ca4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x4b000000,                                                  // 8388608.0                                   /// 00cac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cb0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cb8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cbc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cc4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cc8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ccc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cd0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cd8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cdc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ce0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ce8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cfc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d00
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x4b000000,                                                  // 8388608.0                                   /// 00d08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d0c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d10
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d18
			 0x33333333,                                                  // 4 random values                             /// 00d1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d20
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d24
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d30
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d3c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d54
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d58
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x7f800000,                                                  // inf                                         /// 00d64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d70
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d8c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d94
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d98
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d9c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00da0
			 0xff800000,                                                  // -inf                                        /// 00da4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00da8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00db0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00db8
			 0x00000000,                                                  // zero                                        /// 00dbc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dc0
			 0xff800000,                                                  // -inf                                        /// 00dc4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dd8
			 0x7f800000,                                                  // inf                                         /// 00ddc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00de0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00de4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00de8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00df4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0xffc00001,                                                  // -signaling NaN                              /// 00e00
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e04
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e10
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e1c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e20
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e2c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e50
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e54
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e5c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e60
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e78
			 0x55555555,                                                  // 4 random values                             /// 00e7c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e90
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e98
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e9c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ea0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ea8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ebc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ec0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ec4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ec8
			 0x7fc00001,                                                  // signaling NaN                               /// 00ecc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ed0
			 0x33333333,                                                  // 4 random values                             /// 00ed4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ed8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ee0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ee8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ef0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ef4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ef8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00efc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f08
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f0c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f10
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f14
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f20
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f28
			 0xff800000,                                                  // -inf                                        /// 00f2c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f30
			 0x80000000,                                                  // -zero                                       /// 00f34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f4c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f58
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f60
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f70
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f78
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f84
			 0x4b000000,                                                  // 8388608.0                                   /// 00f88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f98
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fa0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fa4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fb0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fb4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fbc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fcc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fd4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fdc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fe0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fe8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ff0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ff4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ff8
			 0x7f000000 // norm with max exp min mant                   // SP +ve numbers                              /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00000
			 0xffc00001,                                                  // -signaling NaN                              /// 00004
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00008
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0000c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00010
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00014
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00018
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0001c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00020
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00024
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0002c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00038
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0003c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00040
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00044
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00048
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00054
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00058
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0005c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00060
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00064
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00068
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0006c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00070
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00074
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00080
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00088
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00094
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00098
			 0x80011111,                                                  // -9.7958E-41                                 /// 0009c
			 0x80000000,                                                  // -zero                                       /// 000a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000bc
			 0x55555555,                                                  // 4 random values                             /// 000c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 000f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00100
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00108
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00110
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00114
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0011c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00128
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0012c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00130
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00134
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00138
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0013c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00148
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0014c
			 0x33333333,                                                  // 4 random values                             /// 00150
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00158
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0015c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00164
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00174
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00178
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00180
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00184
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00190
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00198
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0019c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001c0
			 0x55555555,                                                  // 4 random values                             /// 001c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00208
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00210
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00214
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0021c
			 0x7f800000,                                                  // inf                                         /// 00220
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00224
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00228
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0022c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00234
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x0c780000,                                                  // Leading 1s:                                 /// 0023c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00248
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00250
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00254
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00260
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x0e000003,                                                  // Trailing 1s:                                /// 0026c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00270
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00274
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00278
			 0x4b000000,                                                  // 8388608.0                                   /// 0027c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00280
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0x00011111,                                                  // 9.7958E-41                                  /// 00288
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0028c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00290
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00294
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00298
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0029c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002e4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00300
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00304
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00310
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00314
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00318
			 0x0c700000,                                                  // Leading 1s:                                 /// 0031c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0032c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00334
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00338
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0034c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00354
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0035c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00360
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00364
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0036c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00370
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00374
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00378
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0037c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00380
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00384
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00394
			 0x7fc00001,                                                  // signaling NaN                               /// 00398
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0xff800000,                                                  // -inf                                        /// 003bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00400
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00404
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00410
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00418
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00424
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0042c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00430
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0043c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00440
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00444
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00448
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0044c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00454
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00458
			 0x4b000000,                                                  // 8388608.0                                   /// 0045c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00460
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00468
			 0x0c400000,                                                  // Leading 1s:                                 /// 0046c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00474
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00478
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00488
			 0x4b000000,                                                  // 8388608.0                                   /// 0048c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00490
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00494
			 0x0c780000,                                                  // Leading 1s:                                 /// 00498
			 0xbf028f5c,                                                  // -0.51                                       /// 0049c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0xbf028f5c,                                                  // -0.51                                       /// 004c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00510
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00514
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00518
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0051c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x4b000000,                                                  // 8388608.0                                   /// 00530
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00534
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00540
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00544
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00548
			 0x4b000000,                                                  // 8388608.0                                   /// 0054c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00550
			 0x0e000003,                                                  // Trailing 1s:                                /// 00554
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00558
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0055c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0056c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00578
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0057c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00580
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00584
			 0x3f028f5c,                                                  // 0.51                                        /// 00588
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0058c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00590
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00594
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00598
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0059c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x80000000,                                                  // -zero                                       /// 005a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 005b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005dc
			 0xff800000,                                                  // -inf                                        /// 005e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005f0
			 0x55555555,                                                  // 4 random values                             /// 005f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0060c
			 0xbf028f5c,                                                  // -0.51                                       /// 00610
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00614
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00618
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0061c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00624
			 0xcb000000,                                                  // -8388608.0                                  /// 00628
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0062c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00634
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00638
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0063c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00644
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00648
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x7f800000,                                                  // inf                                         /// 00650
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00654
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0065c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00660
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00664
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00668
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0066c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00670
			 0x80011111,                                                  // -9.7958E-41                                 /// 00674
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00680
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00688
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0069c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x33333333,                                                  // 4 random values                             /// 006a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0xff800000,                                                  // -inf                                        /// 006b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 006c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 006e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x7fc00001,                                                  // signaling NaN                               /// 006f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00700
			 0x00011111,                                                  // 9.7958E-41                                  /// 00704
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00708
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00710
			 0x4b000000,                                                  // 8388608.0                                   /// 00714
			 0x7f800000,                                                  // inf                                         /// 00718
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00720
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0e000001,                                                  // Trailing 1s:                                /// 00728
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0072c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00730
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00734
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00738
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00740
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00744
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00748
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0074c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00754
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0075c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00760
			 0xffc00001,                                                  // -signaling NaN                              /// 00764
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00770
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00774
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00778
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0077c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00780
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00784
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00788
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0078c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00790
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00794
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00798
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007b4
			 0x7fc00001,                                                  // signaling NaN                               /// 007b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007bc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007e0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0xff800000,                                                  // -inf                                        /// 007e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00800
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00804
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00808
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0080c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00810
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00814
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00818
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00820
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00824
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00828
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00838
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0083c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x0e000001,                                                  // Trailing 1s:                                /// 00844
			 0x80011111,                                                  // -9.7958E-41                                 /// 00848
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0085c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00860
			 0x0c780000,                                                  // Leading 1s:                                 /// 00864
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00870
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00874
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0087c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00880
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00884
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00890
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00894
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00898
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008a0
			 0xbf028f5c,                                                  // -0.51                                       /// 008a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008bc
			 0xffc00001,                                                  // -signaling NaN                              /// 008c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008d0
			 0xcb000000,                                                  // -8388608.0                                  /// 008d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008dc
			 0x3f028f5c,                                                  // 0.51                                        /// 008e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00900
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00904
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00908
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0090c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00914
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00918
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0091c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00920
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00924
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00928
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0092c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00930
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0xbf028f5c,                                                  // -0.51                                       /// 00944
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00948
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00954
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00958
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0095c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00960
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00964
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00968
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0096c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00974
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00980
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0098c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00998
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0099c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009b4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009e0
			 0x3f028f5c,                                                  // 0.51                                        /// 009e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0xffc00001,                                                  // -signaling NaN                              /// 009f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a04
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a10
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a14
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0xff800000,                                                  // -inf                                        /// 00a20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a28
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a30
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a3c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a60
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a6c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a78
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a7c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a80
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a9c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00aa0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00aa4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00aac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ab8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00abc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ac4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ac8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ad0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ad4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00aec
			 0xff800000,                                                  // -inf                                        /// 00af0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x55555555,                                                  // 4 random values                             /// 00af8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00afc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b00
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b04
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b08
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b0c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b18
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b2c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b48
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b58
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b5c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b60
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b64
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b68
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b6c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b74
			 0xff800000,                                                  // -inf                                        /// 00b78
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b9c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ba0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ba4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bb4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bbc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bc4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bc8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bcc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bd4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bd8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bdc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00be4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bf0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bf4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bf8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c18
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c20
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c24
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c30
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c38
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c3c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c40
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c5c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c68
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c6c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c78
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c80
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c88
			 0x33333333,                                                  // 4 random values                             /// 00c8c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c94
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c98
			 0x33333333,                                                  // 4 random values                             /// 00c9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ca0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ca4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cb0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cb4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cb8
			 0x7f800000,                                                  // inf                                         /// 00cbc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cd0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cec
			 0xff800000,                                                  // -inf                                        /// 00cf0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cf8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cfc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d00
			 0x55555555,                                                  // 4 random values                             /// 00d04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d14
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d20
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d24
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d28
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d34
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d3c
			 0x00000000,                                                  // zero                                        /// 00d40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d50
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d5c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d64
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d68
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d6c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d80
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d8c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d9c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00da4
			 0x80000000,                                                  // -zero                                       /// 00da8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00db0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00db4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dbc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dc0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dc4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dd0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dd4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ddc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00de4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00de8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dec
			 0x0c780000,                                                  // Leading 1s:                                 /// 00df0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00df4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00df8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dfc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e14
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e20
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0xcb000000,                                                  // -8388608.0                                  /// 00e48
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e58
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x7f800000,                                                  // inf                                         /// 00e64
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e6c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e78
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e88
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e94
			 0xbf028f5c,                                                  // -0.51                                       /// 00e98
			 0x33333333,                                                  // 4 random values                             /// 00e9c
			 0x80000000,                                                  // -zero                                       /// 00ea0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ea8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00eb0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00eb8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ebc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ec0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ec8
			 0x55555555,                                                  // 4 random values                             /// 00ecc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ed0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ed4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00edc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ee0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ee4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ef4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00efc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f14
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f18
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f1c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f28
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f38
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f40
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f44
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f48
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f4c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f54
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f5c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f68
			 0x3f028f5c,                                                  // 0.51                                        /// 00f6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x33333333,                                                  // 4 random values                             /// 00f8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f90
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x3f028f5c,                                                  // 0.51                                        /// 00fb4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fb8
			 0x4b000000,                                                  // 8388608.0                                   /// 00fbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fc4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fc8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fcc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fd0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ff0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff8
			 0x80800003                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xcb000000,                                                  // -8388608.0                                  /// 00000
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00004
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00008
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00010
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00018
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0001c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00020
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00024
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00028
			 0x80011111,                                                  // -9.7958E-41                                 /// 0002c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0c600000,                                                  // Leading 1s:                                 /// 0003c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00044
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00048
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0004c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00050
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00054
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00060
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00064
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0006c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00070
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00074
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00078
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00080
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00084
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00088
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0008c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00090
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00094
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00098
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000a4
			 0xff800000,                                                  // -inf                                        /// 000a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 000f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00110
			 0x00000000,                                                  // zero                                        /// 00114
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00118
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0011c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00128
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0012c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x80000000,                                                  // -zero                                       /// 00138
			 0x00000000,                                                  // zero                                        /// 0013c
			 0xff800000,                                                  // -inf                                        /// 00140
			 0x00011111,                                                  // 9.7958E-41                                  /// 00144
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00148
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0014c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00150
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0015c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00160
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0016c
			 0xcb000000,                                                  // -8388608.0                                  /// 00170
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00174
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00178
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00180
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00184
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0018c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00194
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00198
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0019c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001b4
			 0xbf028f5c,                                                  // -0.51                                       /// 001b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 001d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 001d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00204
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00210
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00214
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00218
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0021c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00228
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0022c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00230
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00234
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00240
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00248
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00250
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00254
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00258
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0025c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00260
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00268
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0026c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00270
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00274
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00278
			 0x0c400000,                                                  // Leading 1s:                                 /// 0027c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00284
			 0x0e000001,                                                  // Trailing 1s:                                /// 00288
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0028c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00290
			 0xffc00001,                                                  // -signaling NaN                              /// 00294
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00298
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0029c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 002e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 002f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002f4
			 0xbf028f5c,                                                  // -0.51                                       /// 002f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00300
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00304
			 0x3f028f5c,                                                  // 0.51                                        /// 00308
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00310
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00320
			 0x0e000007,                                                  // Trailing 1s:                                /// 00324
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0032c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00334
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00338
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0033c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00344
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0034c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00350
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00354
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00358
			 0x00000000,                                                  // zero                                        /// 0035c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00360
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00364
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0xffc00001,                                                  // -signaling NaN                              /// 0036c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00370
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00374
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00378
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0037c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00384
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00388
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0038c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00394
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00398
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0039c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 003c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0xcb000000,                                                  // -8388608.0                                  /// 003fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x00011111,                                                  // 9.7958E-41                                  /// 00404
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x00011111,                                                  // 9.7958E-41                                  /// 0040c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00410
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00414
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00418
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0041c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00424
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00428
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0042c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00430
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00438
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0043c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x33333333,                                                  // 4 random values                             /// 00444
			 0x00011111,                                                  // 9.7958E-41                                  /// 00448
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0044c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00450
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00458
			 0x80000000,                                                  // -zero                                       /// 0045c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00460
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0xbf028f5c,                                                  // -0.51                                       /// 0046c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00470
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x0e000001,                                                  // Trailing 1s:                                /// 00478
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0047c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00480
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00488
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0048c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00494
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0049c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004b8
			 0x80000000,                                                  // -zero                                       /// 004bc
			 0x7f800000,                                                  // inf                                         /// 004c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x55555555,                                                  // 4 random values                             /// 004e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00500
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00504
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00508
			 0xff800000,                                                  // -inf                                        /// 0050c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x80000000,                                                  // -zero                                       /// 00514
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00520
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00528
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0052c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00530
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0053c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00540
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00548
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x55555555,                                                  // 4 random values                             /// 00550
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00554
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00558
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0055c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00560
			 0x0c780000,                                                  // Leading 1s:                                 /// 00564
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00568
			 0xff800000,                                                  // -inf                                        /// 0056c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00570
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x55555555,                                                  // 4 random values                             /// 0057c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00588
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00590
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0059c
			 0x7f800000,                                                  // inf                                         /// 005a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x7f800000,                                                  // inf                                         /// 005d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00600
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00608
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0060c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00610
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00618
			 0x0c700000,                                                  // Leading 1s:                                 /// 0061c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00620
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00624
			 0xcb000000,                                                  // -8388608.0                                  /// 00628
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00630
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00634
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00638
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0063c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00644
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00648
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x3f028f5c,                                                  // 0.51                                        /// 00650
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00654
			 0x55555555,                                                  // 4 random values                             /// 00658
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0065c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00660
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00664
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00668
			 0x0e000001,                                                  // Trailing 1s:                                /// 0066c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00670
			 0x80011111,                                                  // -9.7958E-41                                 /// 00674
			 0x0c600000,                                                  // Leading 1s:                                 /// 00678
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0067c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00680
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00684
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00688
			 0xbf028f5c,                                                  // -0.51                                       /// 0068c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00690
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00694
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0xbf028f5c,                                                  // -0.51                                       /// 0069c
			 0x0c700000,                                                  // Leading 1s:                                 /// 006a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x7f800000,                                                  // inf                                         /// 006d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00700
			 0x80011111,                                                  // -9.7958E-41                                 /// 00704
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00708
			 0x0c780000,                                                  // Leading 1s:                                 /// 0070c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00710
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00714
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0071c
			 0x00000000,                                                  // zero                                        /// 00720
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00724
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0072c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00740
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00744
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0074c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00750
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00754
			 0xcb000000,                                                  // -8388608.0                                  /// 00758
			 0x7fc00001,                                                  // signaling NaN                               /// 0075c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00764
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00768
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0076c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00770
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x4b000000,                                                  // 8388608.0                                   /// 0077c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00780
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00784
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00788
			 0x7f800000,                                                  // inf                                         /// 0078c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00794
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00798
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0079c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 007dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0xbf028f5c,                                                  // -0.51                                       /// 007f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00800
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00804
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00810
			 0xff800000,                                                  // -inf                                        /// 00814
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00818
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0081c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00820
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00828
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0082c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00830
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00834
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00838
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x33333333,                                                  // 4 random values                             /// 00844
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00848
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0084c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00854
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0xbf028f5c,                                                  // -0.51                                       /// 0085c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00864
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00870
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00874
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00878
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00880
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00890
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00894
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00898
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0089c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 008b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 008c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008c8
			 0x55555555,                                                  // 4 random values                             /// 008cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008d0
			 0x33333333,                                                  // 4 random values                             /// 008d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00900
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00904
			 0x80011111,                                                  // -9.7958E-41                                 /// 00908
			 0x7fc00001,                                                  // signaling NaN                               /// 0090c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00910
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00918
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0091c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00920
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00924
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00930
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00934
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00940
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00944
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00948
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0094c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00950
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00954
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00958
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0095c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00960
			 0x55555555,                                                  // 4 random values                             /// 00964
			 0x0c700000,                                                  // Leading 1s:                                 /// 00968
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00974
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00978
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0097c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00980
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00984
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00988
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0098c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00994
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0099c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 009a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a00
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a04
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a08
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a0c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a1c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x7fc00001,                                                  // signaling NaN                               /// 00a28
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a2c
			 0xff800000,                                                  // -inf                                        /// 00a30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a34
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a38
			 0x4b000000,                                                  // 8388608.0                                   /// 00a3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00a60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a6c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a88
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x00000000,                                                  // zero                                        /// 00a98
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00aa0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aa8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ab0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x33333333,                                                  // 4 random values                             /// 00ab8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ac0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ac4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ac8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ad0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ad8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ae8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00aec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00af4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00afc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b00
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b04
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x00000000,                                                  // zero                                        /// 00b10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b18
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b1c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b20
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b24
			 0x55555555,                                                  // 4 random values                             /// 00b28
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b30
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b3c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b44
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b60
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b6c
			 0x7f800000,                                                  // inf                                         /// 00b70
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b78
			 0x4b000000,                                                  // 8388608.0                                   /// 00b7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b94
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b98
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b9c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ba4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ba8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bb8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bc0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bc4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bcc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bdc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00be8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bf0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bf4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bf8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c00
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c04
			 0x7fc00001,                                                  // signaling NaN                               /// 00c08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c0c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c10
			 0x7f800000,                                                  // inf                                         /// 00c14
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c1c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c20
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c24
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c34
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c38
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c40
			 0x55555555,                                                  // 4 random values                             /// 00c44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c54
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c58
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c7c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c84
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c88
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c90
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c98
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c9c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ca0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cb0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cb4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cb8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cc0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cc4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cc8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ccc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cd8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cdc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ce0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cf8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cfc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d00
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d08
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d0c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d10
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d1c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d2c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d30
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d5c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d7c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d84
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d88
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00da4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00da8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00db4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00db8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x00000000,                                                  // zero                                        /// 00dc4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dc8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dcc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dd0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dd4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dd8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ddc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00de0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00de4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00de8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00df0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00df4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00df8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e08
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e0c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e10
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e14
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e4c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e64
			 0xbf028f5c,                                                  // -0.51                                       /// 00e68
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e70
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e80
			 0x00000000,                                                  // zero                                        /// 00e84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e88
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e90
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e9c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ea4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ea8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00eb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eb8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x7f800000,                                                  // inf                                         /// 00ec0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ec4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ec8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ecc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ed0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ed4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ed8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00edc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ee0
			 0x80000000,                                                  // -zero                                       /// 00ee4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ee8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00eec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ef0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ef4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00efc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f04
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f0c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f10
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f20
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0x33333333,                                                  // 4 random values                             /// 00f30
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x00000000,                                                  // zero                                        /// 00f38
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f44
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f48
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f4c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f50
			 0xcb000000,                                                  // -8388608.0                                  /// 00f54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f60
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f64
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f68
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f8c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fa0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fa4
			 0x7fc00001,                                                  // signaling NaN                               /// 00fa8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fb0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fb8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fbc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fc4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fd0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fd4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fd8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fe4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fe8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ff4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x0e0000ff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x42763e3c, /// 0x0
			 0x843f935c, /// 0x4
			 0x3a040bc8, /// 0x8
			 0xa7f889d9, /// 0xc
			 0x7f0d0bba, /// 0x10
			 0x6ba92f86, /// 0x14
			 0x154212f9, /// 0x18
			 0xfcd7addb, /// 0x1c
			 0x8e78529b, /// 0x20
			 0x9aeef3a0, /// 0x24
			 0x49315bf1, /// 0x28
			 0x6d991d90, /// 0x2c
			 0x11fc894e, /// 0x30
			 0x8dd0d7e1, /// 0x34
			 0x59079b53, /// 0x38
			 0x47612dea, /// 0x3c
			 0x73fcea39, /// 0x40
			 0x11f1c672, /// 0x44
			 0xe2aac260, /// 0x48
			 0x6994eb91, /// 0x4c
			 0x02ea9659, /// 0x50
			 0x26d8e232, /// 0x54
			 0xa2655b7f, /// 0x58
			 0xc41ddb30, /// 0x5c
			 0x3c84c90b, /// 0x60
			 0xc61a7a81, /// 0x64
			 0xb3fadb05, /// 0x68
			 0x566149b8, /// 0x6c
			 0x6f7cc808, /// 0x70
			 0x2b76ffe6, /// 0x74
			 0x37513d98, /// 0x78
			 0xc88511f3, /// 0x7c
			 0x336db213, /// 0x80
			 0xc9c631bc, /// 0x84
			 0xf6c5a851, /// 0x88
			 0xa8c5b942, /// 0x8c
			 0xd131b3a3, /// 0x90
			 0xec1bfd02, /// 0x94
			 0x8f8125ea, /// 0x98
			 0xbf334c65, /// 0x9c
			 0x55fafb8c, /// 0xa0
			 0x2644d66d, /// 0xa4
			 0x361537b4, /// 0xa8
			 0x5637e2de, /// 0xac
			 0x248e0584, /// 0xb0
			 0x28435d0d, /// 0xb4
			 0x1dac8c1a, /// 0xb8
			 0xa7b877eb, /// 0xbc
			 0xdb647f9c, /// 0xc0
			 0xa1bd2821, /// 0xc4
			 0x67a60e34, /// 0xc8
			 0x10b2ff25, /// 0xcc
			 0x332c0429, /// 0xd0
			 0xa99557ad, /// 0xd4
			 0x7fc78a16, /// 0xd8
			 0xeaf43b3a, /// 0xdc
			 0x050c94de, /// 0xe0
			 0xa36603a0, /// 0xe4
			 0xb9e3a99d, /// 0xe8
			 0xaa492b93, /// 0xec
			 0xa2de365a, /// 0xf0
			 0x51229c3a, /// 0xf4
			 0x56bb7789, /// 0xf8
			 0x5b9fc84b, /// 0xfc
			 0x61b0b9de, /// 0x100
			 0x33546395, /// 0x104
			 0x5a57930d, /// 0x108
			 0x455ddeef, /// 0x10c
			 0xea7ce5f7, /// 0x110
			 0x79b99218, /// 0x114
			 0xb1749370, /// 0x118
			 0x6e22055b, /// 0x11c
			 0x9e470aa7, /// 0x120
			 0xd75cfeee, /// 0x124
			 0x13765fe8, /// 0x128
			 0x7f0b7b05, /// 0x12c
			 0x6c15b25b, /// 0x130
			 0x08b31aec, /// 0x134
			 0x5bac484b, /// 0x138
			 0x6db89789, /// 0x13c
			 0x5cfb73c9, /// 0x140
			 0x3ab8f281, /// 0x144
			 0x08c4b769, /// 0x148
			 0xca7b0154, /// 0x14c
			 0x626414d1, /// 0x150
			 0xdff31453, /// 0x154
			 0x0fc3c304, /// 0x158
			 0x686338e7, /// 0x15c
			 0x44be9ae6, /// 0x160
			 0x81ebf824, /// 0x164
			 0x2d8b75c7, /// 0x168
			 0x5126da33, /// 0x16c
			 0x2fad595b, /// 0x170
			 0x9119a55d, /// 0x174
			 0xe968444d, /// 0x178
			 0xf6cc4734, /// 0x17c
			 0xabcfc589, /// 0x180
			 0xbe944903, /// 0x184
			 0x177141a8, /// 0x188
			 0xad021896, /// 0x18c
			 0x7550af4a, /// 0x190
			 0xf1df0e48, /// 0x194
			 0xf005d7f5, /// 0x198
			 0xc74aa205, /// 0x19c
			 0x6f5e1303, /// 0x1a0
			 0x51bc472d, /// 0x1a4
			 0x7300fb05, /// 0x1a8
			 0x6d9122ba, /// 0x1ac
			 0x4f0b3b5a, /// 0x1b0
			 0xdc270e7d, /// 0x1b4
			 0x2aa072b6, /// 0x1b8
			 0xb84fb442, /// 0x1bc
			 0xf75a01b1, /// 0x1c0
			 0x42e8534a, /// 0x1c4
			 0x69fb68d4, /// 0x1c8
			 0xbc951881, /// 0x1cc
			 0x3d711c77, /// 0x1d0
			 0xfbd61a9a, /// 0x1d4
			 0x35bb74ee, /// 0x1d8
			 0x47637cf8, /// 0x1dc
			 0x7b3b027b, /// 0x1e0
			 0x324ca491, /// 0x1e4
			 0x19f6e405, /// 0x1e8
			 0x90e8f92a, /// 0x1ec
			 0x3ffdff44, /// 0x1f0
			 0x1f0ab401, /// 0x1f4
			 0xedec9135, /// 0x1f8
			 0xd3b7773d, /// 0x1fc
			 0x069cb316, /// 0x200
			 0x8a1478dc, /// 0x204
			 0x1c2f5156, /// 0x208
			 0xb9df4288, /// 0x20c
			 0x069926af, /// 0x210
			 0xf97d0528, /// 0x214
			 0x26b3a151, /// 0x218
			 0x248c7fec, /// 0x21c
			 0x5fa07e79, /// 0x220
			 0xa5c9bb32, /// 0x224
			 0xbbf0af9f, /// 0x228
			 0x8266f098, /// 0x22c
			 0x62435024, /// 0x230
			 0x245f2b10, /// 0x234
			 0xd427ce49, /// 0x238
			 0x644eec97, /// 0x23c
			 0x96219b98, /// 0x240
			 0x438443ea, /// 0x244
			 0xe4033f02, /// 0x248
			 0xccf9b570, /// 0x24c
			 0xfe34fec3, /// 0x250
			 0xefa517b2, /// 0x254
			 0x2e3573e5, /// 0x258
			 0xe52b38d4, /// 0x25c
			 0x1a63f0de, /// 0x260
			 0x6d9c2d1d, /// 0x264
			 0x12fdc723, /// 0x268
			 0xd2a1d337, /// 0x26c
			 0xd8af1f31, /// 0x270
			 0x841cb9e2, /// 0x274
			 0x278523d2, /// 0x278
			 0x9879528b, /// 0x27c
			 0xc69b00d7, /// 0x280
			 0x7745e07d, /// 0x284
			 0x7c3ad3ed, /// 0x288
			 0x4c0cfea4, /// 0x28c
			 0xa3689f06, /// 0x290
			 0x20cbc231, /// 0x294
			 0xa0f63c7b, /// 0x298
			 0x2e792938, /// 0x29c
			 0x1c48cba4, /// 0x2a0
			 0x51466b59, /// 0x2a4
			 0xbe0ecf69, /// 0x2a8
			 0xa3deffdb, /// 0x2ac
			 0xc256d604, /// 0x2b0
			 0x36d7cf13, /// 0x2b4
			 0x56ced805, /// 0x2b8
			 0xc57037f8, /// 0x2bc
			 0x08566b55, /// 0x2c0
			 0x1bffeb42, /// 0x2c4
			 0xd3fe1581, /// 0x2c8
			 0x689859c0, /// 0x2cc
			 0x3a34d6ce, /// 0x2d0
			 0x6d9c9463, /// 0x2d4
			 0x7dc9867b, /// 0x2d8
			 0x3772dab1, /// 0x2dc
			 0x1ac4b8ac, /// 0x2e0
			 0xd8090a7d, /// 0x2e4
			 0x6fe164fd, /// 0x2e8
			 0x3e45f4bf, /// 0x2ec
			 0x63c8cbab, /// 0x2f0
			 0xde15b7eb, /// 0x2f4
			 0xec4c2453, /// 0x2f8
			 0x8590d468, /// 0x2fc
			 0x55d61193, /// 0x300
			 0x98e74a70, /// 0x304
			 0xe3359d8e, /// 0x308
			 0x98561a36, /// 0x30c
			 0xa9ef1a7d, /// 0x310
			 0x1047b266, /// 0x314
			 0x0b4dc1b2, /// 0x318
			 0xcff81932, /// 0x31c
			 0xf020eed4, /// 0x320
			 0x87c13940, /// 0x324
			 0x98812822, /// 0x328
			 0x57e89510, /// 0x32c
			 0xafcc7e88, /// 0x330
			 0x30024a57, /// 0x334
			 0xd737373e, /// 0x338
			 0x0881a7a6, /// 0x33c
			 0x990373e1, /// 0x340
			 0x86b54e1f, /// 0x344
			 0x84709492, /// 0x348
			 0x15fbd309, /// 0x34c
			 0xb8fe2fdf, /// 0x350
			 0xac62f7f7, /// 0x354
			 0x404ea182, /// 0x358
			 0x5da2eea8, /// 0x35c
			 0x0e592ffd, /// 0x360
			 0x71d64134, /// 0x364
			 0x38d2f26b, /// 0x368
			 0x392d7dec, /// 0x36c
			 0xb77c0fad, /// 0x370
			 0xe1de1ce6, /// 0x374
			 0xafacf6db, /// 0x378
			 0x8e50b6e7, /// 0x37c
			 0xdba677c0, /// 0x380
			 0x36f95461, /// 0x384
			 0x6189586e, /// 0x388
			 0xae9500dc, /// 0x38c
			 0x1618cc44, /// 0x390
			 0x27e9e2d3, /// 0x394
			 0x3c77ff21, /// 0x398
			 0x461d7d3e, /// 0x39c
			 0xb9f6f613, /// 0x3a0
			 0x7e58fc61, /// 0x3a4
			 0x937fa168, /// 0x3a8
			 0x532985ee, /// 0x3ac
			 0x3e597cf2, /// 0x3b0
			 0x5479e1aa, /// 0x3b4
			 0x8bb670c6, /// 0x3b8
			 0x39e10b24, /// 0x3bc
			 0x47caa907, /// 0x3c0
			 0x24bbd960, /// 0x3c4
			 0xe7cfcbe3, /// 0x3c8
			 0x94288038, /// 0x3cc
			 0xc1aff367, /// 0x3d0
			 0x8e974aae, /// 0x3d4
			 0xaf119d8a, /// 0x3d8
			 0x1d8b257d, /// 0x3dc
			 0x0661cab7, /// 0x3e0
			 0xf046bfc2, /// 0x3e4
			 0x63c7b813, /// 0x3e8
			 0xff851ac5, /// 0x3ec
			 0x78a01b8a, /// 0x3f0
			 0x5d6f2b90, /// 0x3f4
			 0x50f1f0a6, /// 0x3f8
			 0xd22def4b, /// 0x3fc
			 0xc61d8f9a, /// 0x400
			 0xe13d8f47, /// 0x404
			 0x79fc5116, /// 0x408
			 0xf22e396e, /// 0x40c
			 0xbe9af839, /// 0x410
			 0x50526851, /// 0x414
			 0xf0bf2304, /// 0x418
			 0x1e1db661, /// 0x41c
			 0x559b0e3f, /// 0x420
			 0xb1f65585, /// 0x424
			 0xcac83ecc, /// 0x428
			 0x7fa57d01, /// 0x42c
			 0x8f4aca69, /// 0x430
			 0x82873b6a, /// 0x434
			 0x0e0edbe5, /// 0x438
			 0x801e6818, /// 0x43c
			 0xbfba435e, /// 0x440
			 0x4334c40d, /// 0x444
			 0x10fb1198, /// 0x448
			 0x8506868b, /// 0x44c
			 0x1ffd0bae, /// 0x450
			 0xb9c29d02, /// 0x454
			 0xd0400581, /// 0x458
			 0xc2be927c, /// 0x45c
			 0xf362589e, /// 0x460
			 0x21d0cbc4, /// 0x464
			 0x29ae2f22, /// 0x468
			 0xdcc7806a, /// 0x46c
			 0x12d97236, /// 0x470
			 0xecca0339, /// 0x474
			 0xfb6f88bb, /// 0x478
			 0xc173021b, /// 0x47c
			 0xa43a39ad, /// 0x480
			 0x496d5e6c, /// 0x484
			 0xc6a7fba1, /// 0x488
			 0x8f2321ce, /// 0x48c
			 0xca484b08, /// 0x490
			 0xf8c885dc, /// 0x494
			 0xb4a4af5d, /// 0x498
			 0x0f7639f0, /// 0x49c
			 0xcbea0c81, /// 0x4a0
			 0x8e198a0c, /// 0x4a4
			 0xa8e6bbba, /// 0x4a8
			 0x195e9f3c, /// 0x4ac
			 0x428d1333, /// 0x4b0
			 0x1e4ac31d, /// 0x4b4
			 0xfa58bae4, /// 0x4b8
			 0xb703a8a3, /// 0x4bc
			 0xb028c85d, /// 0x4c0
			 0xb84eb7e6, /// 0x4c4
			 0xbcca7bde, /// 0x4c8
			 0xf349b2e4, /// 0x4cc
			 0x9c2549ef, /// 0x4d0
			 0xd32bbf5f, /// 0x4d4
			 0x73e0e515, /// 0x4d8
			 0x626bb415, /// 0x4dc
			 0xa5267be9, /// 0x4e0
			 0x0547c7dd, /// 0x4e4
			 0x41dc0711, /// 0x4e8
			 0x0ad83eb0, /// 0x4ec
			 0xb2cdd0fd, /// 0x4f0
			 0x2567868b, /// 0x4f4
			 0xa686cb63, /// 0x4f8
			 0xef63e5dd, /// 0x4fc
			 0x2745258f, /// 0x500
			 0x89532212, /// 0x504
			 0x355d366e, /// 0x508
			 0x0032d1d4, /// 0x50c
			 0x8ace5c84, /// 0x510
			 0xac0f6fae, /// 0x514
			 0x899fa1c7, /// 0x518
			 0x8eacfe92, /// 0x51c
			 0x2d0de496, /// 0x520
			 0xcbe5be2b, /// 0x524
			 0x0a2f4da5, /// 0x528
			 0x67feb52f, /// 0x52c
			 0x47d232d0, /// 0x530
			 0xbdc737d4, /// 0x534
			 0x6fca5646, /// 0x538
			 0xe2f8d911, /// 0x53c
			 0x6230863b, /// 0x540
			 0x444a1979, /// 0x544
			 0x18d8b61c, /// 0x548
			 0x408ef23c, /// 0x54c
			 0xe61a8a8a, /// 0x550
			 0x766a89d1, /// 0x554
			 0x0f2990be, /// 0x558
			 0x11d1ce08, /// 0x55c
			 0xfa014182, /// 0x560
			 0xad152ab6, /// 0x564
			 0x15a3cec4, /// 0x568
			 0xe20c3c2d, /// 0x56c
			 0xe36fa8b6, /// 0x570
			 0x21a582e0, /// 0x574
			 0xfa07a28f, /// 0x578
			 0x84ad3f2b, /// 0x57c
			 0xe267d704, /// 0x580
			 0x7f1a07d5, /// 0x584
			 0x7f52d178, /// 0x588
			 0x7a530ca8, /// 0x58c
			 0x4e6e0271, /// 0x590
			 0xa341d9c2, /// 0x594
			 0xc4edf2ae, /// 0x598
			 0x968ebf95, /// 0x59c
			 0x4e944fdf, /// 0x5a0
			 0xe930e2bc, /// 0x5a4
			 0xd33639fe, /// 0x5a8
			 0xfb1790fe, /// 0x5ac
			 0xbdae782a, /// 0x5b0
			 0x94172bd5, /// 0x5b4
			 0x60f0e953, /// 0x5b8
			 0x92884528, /// 0x5bc
			 0x3a6e0df3, /// 0x5c0
			 0x722694ea, /// 0x5c4
			 0x3c38a39d, /// 0x5c8
			 0x4b6d8f67, /// 0x5cc
			 0xad63bfa7, /// 0x5d0
			 0x964d773d, /// 0x5d4
			 0x45fb9bbd, /// 0x5d8
			 0x98ac995a, /// 0x5dc
			 0x775bf8c2, /// 0x5e0
			 0x13af1d33, /// 0x5e4
			 0x4efd6b73, /// 0x5e8
			 0x7f61da30, /// 0x5ec
			 0x6102a8f6, /// 0x5f0
			 0xf5b40468, /// 0x5f4
			 0x0612bff1, /// 0x5f8
			 0x44eea9e7, /// 0x5fc
			 0x451ff4ea, /// 0x600
			 0x5086ec17, /// 0x604
			 0xfef6b962, /// 0x608
			 0x83aa9fd6, /// 0x60c
			 0x89f763b5, /// 0x610
			 0x9bbe7ebb, /// 0x614
			 0xc6ea1c8a, /// 0x618
			 0xd4383921, /// 0x61c
			 0xda8491d0, /// 0x620
			 0xcb679ec0, /// 0x624
			 0xaa540dad, /// 0x628
			 0x1866bd16, /// 0x62c
			 0x2d8ea94a, /// 0x630
			 0x5e2c7830, /// 0x634
			 0x0c8c0e40, /// 0x638
			 0xad093a3e, /// 0x63c
			 0x6332712d, /// 0x640
			 0xd4b2ca69, /// 0x644
			 0x13a79389, /// 0x648
			 0x36871673, /// 0x64c
			 0x34a5c825, /// 0x650
			 0x0ebb884d, /// 0x654
			 0xd7e6ad32, /// 0x658
			 0x83ee6c20, /// 0x65c
			 0x34a7d164, /// 0x660
			 0x01617133, /// 0x664
			 0x815b38f7, /// 0x668
			 0x71e7f57c, /// 0x66c
			 0x58b66603, /// 0x670
			 0x97ba23ad, /// 0x674
			 0x18fac30c, /// 0x678
			 0xd3c63d92, /// 0x67c
			 0x6aeebac9, /// 0x680
			 0xf38ccb77, /// 0x684
			 0x1f1c6831, /// 0x688
			 0xcce00a15, /// 0x68c
			 0x2e05d1f3, /// 0x690
			 0xc8f7b246, /// 0x694
			 0xedf0b986, /// 0x698
			 0x59c2da05, /// 0x69c
			 0x8e478283, /// 0x6a0
			 0x0a299707, /// 0x6a4
			 0x701476db, /// 0x6a8
			 0x2cc3c84e, /// 0x6ac
			 0x580793a6, /// 0x6b0
			 0xd7151de0, /// 0x6b4
			 0x0eacb8e4, /// 0x6b8
			 0xb37ba1df, /// 0x6bc
			 0xac00d236, /// 0x6c0
			 0x35904892, /// 0x6c4
			 0xa85c35c1, /// 0x6c8
			 0xcaaef7c9, /// 0x6cc
			 0x85b2fff2, /// 0x6d0
			 0x13e0c233, /// 0x6d4
			 0xcccfce7a, /// 0x6d8
			 0x8a510326, /// 0x6dc
			 0xf5da38f9, /// 0x6e0
			 0x081fea31, /// 0x6e4
			 0x7a0ede36, /// 0x6e8
			 0x014f8467, /// 0x6ec
			 0x3ff54571, /// 0x6f0
			 0x2d4ce563, /// 0x6f4
			 0xb39a7c50, /// 0x6f8
			 0x74333ee8, /// 0x6fc
			 0x07f36017, /// 0x700
			 0x59c64628, /// 0x704
			 0x18844bb4, /// 0x708
			 0x2fbae4c6, /// 0x70c
			 0x90487c4c, /// 0x710
			 0xc568967f, /// 0x714
			 0xfde512ec, /// 0x718
			 0xf9a4ef4a, /// 0x71c
			 0xad2dea52, /// 0x720
			 0xab2a9b44, /// 0x724
			 0xad64425c, /// 0x728
			 0xcae809e2, /// 0x72c
			 0xab24ee83, /// 0x730
			 0x3059a445, /// 0x734
			 0x900afcd8, /// 0x738
			 0x935d1b60, /// 0x73c
			 0xd67370db, /// 0x740
			 0x83327c4f, /// 0x744
			 0xd88d9a4d, /// 0x748
			 0x2ce2edc0, /// 0x74c
			 0x59ed3286, /// 0x750
			 0x96fe2a38, /// 0x754
			 0x4bf9a7b8, /// 0x758
			 0x313b311e, /// 0x75c
			 0xdcac42c8, /// 0x760
			 0x3fde2002, /// 0x764
			 0xee5387fb, /// 0x768
			 0xe6cadde1, /// 0x76c
			 0x1fff4bf9, /// 0x770
			 0x60e5f19d, /// 0x774
			 0x025e05bb, /// 0x778
			 0x3acfdee8, /// 0x77c
			 0x6f211eb5, /// 0x780
			 0x02fb468a, /// 0x784
			 0x7fb22474, /// 0x788
			 0x396581cb, /// 0x78c
			 0xf441e7aa, /// 0x790
			 0x781798ad, /// 0x794
			 0xc4224ba0, /// 0x798
			 0xd067cb21, /// 0x79c
			 0x12c06a85, /// 0x7a0
			 0xb51f2e22, /// 0x7a4
			 0xf5e1f0e9, /// 0x7a8
			 0x79e57fd4, /// 0x7ac
			 0x9cbe52e0, /// 0x7b0
			 0xaceedeef, /// 0x7b4
			 0x7ff7561a, /// 0x7b8
			 0x524f368b, /// 0x7bc
			 0x53b4e454, /// 0x7c0
			 0x31e9c6a4, /// 0x7c4
			 0xbc9f171b, /// 0x7c8
			 0xeb3c1b1a, /// 0x7cc
			 0x095368c6, /// 0x7d0
			 0xd58dcd64, /// 0x7d4
			 0x7637a61d, /// 0x7d8
			 0xcec85752, /// 0x7dc
			 0x6895ee1c, /// 0x7e0
			 0x469961a6, /// 0x7e4
			 0xbac50c8a, /// 0x7e8
			 0xca8ab33e, /// 0x7ec
			 0xe485dcf4, /// 0x7f0
			 0x97989fe0, /// 0x7f4
			 0x14694237, /// 0x7f8
			 0xcd0d8e7b, /// 0x7fc
			 0x2576e3fc, /// 0x800
			 0x6ac22644, /// 0x804
			 0x6c15be76, /// 0x808
			 0xf2a23219, /// 0x80c
			 0xe9de2466, /// 0x810
			 0x58cbae87, /// 0x814
			 0xbdb62609, /// 0x818
			 0x7382c1ff, /// 0x81c
			 0xbea95160, /// 0x820
			 0x19510879, /// 0x824
			 0xdc44b379, /// 0x828
			 0xb0434d1c, /// 0x82c
			 0xe4fe65ea, /// 0x830
			 0x17a49256, /// 0x834
			 0xfceb1d9b, /// 0x838
			 0xf5f9f1c0, /// 0x83c
			 0x17b88240, /// 0x840
			 0x971bf4ea, /// 0x844
			 0x0197493e, /// 0x848
			 0x8fb78e81, /// 0x84c
			 0x595d8bd0, /// 0x850
			 0x1487931b, /// 0x854
			 0xcd52182e, /// 0x858
			 0xb73936b8, /// 0x85c
			 0x84ac3a35, /// 0x860
			 0xc60bc9fe, /// 0x864
			 0x643e8bbc, /// 0x868
			 0x30f48ed7, /// 0x86c
			 0x0330a82f, /// 0x870
			 0x6c3fe4b8, /// 0x874
			 0xbe66793c, /// 0x878
			 0x613cdf54, /// 0x87c
			 0x9e7d14bb, /// 0x880
			 0x7d912ab1, /// 0x884
			 0x1bb90514, /// 0x888
			 0xb475a545, /// 0x88c
			 0xbb992320, /// 0x890
			 0x43a46536, /// 0x894
			 0x4873e4b8, /// 0x898
			 0xc67e0a12, /// 0x89c
			 0x95cc9d4e, /// 0x8a0
			 0xdb945659, /// 0x8a4
			 0x753eee05, /// 0x8a8
			 0x81a2f8cf, /// 0x8ac
			 0x67a94dc2, /// 0x8b0
			 0x6f856451, /// 0x8b4
			 0x26c51f9c, /// 0x8b8
			 0xbb3e63ed, /// 0x8bc
			 0x53270856, /// 0x8c0
			 0xb90795e9, /// 0x8c4
			 0xa043ffa8, /// 0x8c8
			 0xa02916ae, /// 0x8cc
			 0x52183cf2, /// 0x8d0
			 0x3d00e986, /// 0x8d4
			 0x8363ba45, /// 0x8d8
			 0x0e01f8e6, /// 0x8dc
			 0x0289648e, /// 0x8e0
			 0x7f4c652f, /// 0x8e4
			 0x06b13bbe, /// 0x8e8
			 0x96fa3ca0, /// 0x8ec
			 0x7e77e50b, /// 0x8f0
			 0x1a85f1a8, /// 0x8f4
			 0x16167ee5, /// 0x8f8
			 0xfc5a66ce, /// 0x8fc
			 0xfa8a1882, /// 0x900
			 0x447e780e, /// 0x904
			 0x39fe97c3, /// 0x908
			 0x2383c492, /// 0x90c
			 0xb416f5e6, /// 0x910
			 0x593e4cbc, /// 0x914
			 0x805cd367, /// 0x918
			 0x74982d34, /// 0x91c
			 0x1581afa1, /// 0x920
			 0x163517c2, /// 0x924
			 0xfeb7bbd7, /// 0x928
			 0xb1d5bfce, /// 0x92c
			 0xffe96879, /// 0x930
			 0x746cb495, /// 0x934
			 0x893291b8, /// 0x938
			 0x377c2a4e, /// 0x93c
			 0x1ccfde91, /// 0x940
			 0xddbaaabb, /// 0x944
			 0x3b9cd09a, /// 0x948
			 0xd4fe491b, /// 0x94c
			 0x11dc993b, /// 0x950
			 0xed854352, /// 0x954
			 0x485e28d2, /// 0x958
			 0x87b3b758, /// 0x95c
			 0xe1fbcfab, /// 0x960
			 0xe473c627, /// 0x964
			 0x49a36fad, /// 0x968
			 0x7f91b9ba, /// 0x96c
			 0x1ef29758, /// 0x970
			 0xf0b5fa7c, /// 0x974
			 0x08f062eb, /// 0x978
			 0x202be132, /// 0x97c
			 0x01134469, /// 0x980
			 0x17598d80, /// 0x984
			 0x94a13bc5, /// 0x988
			 0xb324b31c, /// 0x98c
			 0x9bcd0acf, /// 0x990
			 0xfae7b8f7, /// 0x994
			 0xb0dbbefc, /// 0x998
			 0x47c07c21, /// 0x99c
			 0x4609b8ae, /// 0x9a0
			 0x3aaeb232, /// 0x9a4
			 0x7012fd9c, /// 0x9a8
			 0x8bb42f39, /// 0x9ac
			 0x54532084, /// 0x9b0
			 0x7ad1deed, /// 0x9b4
			 0xd32a0e39, /// 0x9b8
			 0xe6f27b63, /// 0x9bc
			 0xe610828f, /// 0x9c0
			 0x71376470, /// 0x9c4
			 0x194a4b88, /// 0x9c8
			 0x93bfffc1, /// 0x9cc
			 0x1c0988fa, /// 0x9d0
			 0xa6fab3ac, /// 0x9d4
			 0xcacefce1, /// 0x9d8
			 0xfd9fa44d, /// 0x9dc
			 0x88ab1c7b, /// 0x9e0
			 0xa5a0e03a, /// 0x9e4
			 0x39aaf927, /// 0x9e8
			 0xb91f8fc2, /// 0x9ec
			 0xc1b32cad, /// 0x9f0
			 0x94fe13bd, /// 0x9f4
			 0x65d663c3, /// 0x9f8
			 0x648d8624, /// 0x9fc
			 0x85d1c187, /// 0xa00
			 0xb4ceaba6, /// 0xa04
			 0x5a61e6d5, /// 0xa08
			 0xa4359096, /// 0xa0c
			 0xf14e3155, /// 0xa10
			 0x837322f2, /// 0xa14
			 0xc7b721bb, /// 0xa18
			 0xdac6ea95, /// 0xa1c
			 0x1b680b4d, /// 0xa20
			 0x0f37e963, /// 0xa24
			 0xae882380, /// 0xa28
			 0x942b80ee, /// 0xa2c
			 0xa8895097, /// 0xa30
			 0xdb181d02, /// 0xa34
			 0xb1bd8c92, /// 0xa38
			 0xe07d584d, /// 0xa3c
			 0xbb21fb9c, /// 0xa40
			 0x6a9eb594, /// 0xa44
			 0x0ec9edcb, /// 0xa48
			 0xef0c0881, /// 0xa4c
			 0xdfe45cfb, /// 0xa50
			 0x43e78427, /// 0xa54
			 0x64322b34, /// 0xa58
			 0xf151404e, /// 0xa5c
			 0x3c596803, /// 0xa60
			 0xe34bc5c2, /// 0xa64
			 0x3102b15c, /// 0xa68
			 0x6305c777, /// 0xa6c
			 0x8f9f09e4, /// 0xa70
			 0x7f211876, /// 0xa74
			 0x9715d4d7, /// 0xa78
			 0x2fe7c742, /// 0xa7c
			 0x12f577e8, /// 0xa80
			 0x4d73eaca, /// 0xa84
			 0x49ca9ac0, /// 0xa88
			 0xeae02d0f, /// 0xa8c
			 0xe6ef9751, /// 0xa90
			 0xbc9a2b29, /// 0xa94
			 0x0eb2ffe2, /// 0xa98
			 0x81f30643, /// 0xa9c
			 0x1c16a012, /// 0xaa0
			 0x795f6fa1, /// 0xaa4
			 0xbf7845af, /// 0xaa8
			 0x8deebe9e, /// 0xaac
			 0xfc5c543f, /// 0xab0
			 0xa4740903, /// 0xab4
			 0x16a9c034, /// 0xab8
			 0xaa7b3509, /// 0xabc
			 0xf4569005, /// 0xac0
			 0xb55e5439, /// 0xac4
			 0x15ad2a75, /// 0xac8
			 0x1eb98078, /// 0xacc
			 0x873619b3, /// 0xad0
			 0xedd186a9, /// 0xad4
			 0x605f6cd4, /// 0xad8
			 0x312ac93a, /// 0xadc
			 0x267603cf, /// 0xae0
			 0xd6b08eb9, /// 0xae4
			 0x1cb90303, /// 0xae8
			 0x5db60f56, /// 0xaec
			 0xa57f14ac, /// 0xaf0
			 0x5d52fafe, /// 0xaf4
			 0xfbbecf0c, /// 0xaf8
			 0xcdea7629, /// 0xafc
			 0x04a5dcba, /// 0xb00
			 0xc66f6f68, /// 0xb04
			 0xca6ca721, /// 0xb08
			 0x122c4868, /// 0xb0c
			 0xa44b6011, /// 0xb10
			 0x1509e6d5, /// 0xb14
			 0x2bed6eac, /// 0xb18
			 0x1a970733, /// 0xb1c
			 0x7c1633ab, /// 0xb20
			 0xb9a28bab, /// 0xb24
			 0x353bca78, /// 0xb28
			 0x6f2b3479, /// 0xb2c
			 0xea12573a, /// 0xb30
			 0x15ad3437, /// 0xb34
			 0x7e1bc909, /// 0xb38
			 0xdfd8bf8e, /// 0xb3c
			 0x89c6b024, /// 0xb40
			 0x163e3e2d, /// 0xb44
			 0xd8e77c08, /// 0xb48
			 0xdc896f1c, /// 0xb4c
			 0xd43a53f6, /// 0xb50
			 0xb09f1abd, /// 0xb54
			 0x5e9dc43d, /// 0xb58
			 0x1e767123, /// 0xb5c
			 0xa343227a, /// 0xb60
			 0x43a34387, /// 0xb64
			 0xb7f1c1f3, /// 0xb68
			 0x2156018f, /// 0xb6c
			 0x5ebe2b4c, /// 0xb70
			 0xd51efd26, /// 0xb74
			 0x3d7608aa, /// 0xb78
			 0xa3fcb38b, /// 0xb7c
			 0x11110889, /// 0xb80
			 0x3a992508, /// 0xb84
			 0x78f8d25d, /// 0xb88
			 0x21dbb9d7, /// 0xb8c
			 0xfa11376a, /// 0xb90
			 0xc79f8308, /// 0xb94
			 0xc933e88f, /// 0xb98
			 0xd35f60a0, /// 0xb9c
			 0x7f95de85, /// 0xba0
			 0xb4bb4ca2, /// 0xba4
			 0x666d47d3, /// 0xba8
			 0x33698a43, /// 0xbac
			 0x65e5c772, /// 0xbb0
			 0x617b4674, /// 0xbb4
			 0x971acc72, /// 0xbb8
			 0x1c565f5c, /// 0xbbc
			 0xb4b8aee3, /// 0xbc0
			 0xbe187fd5, /// 0xbc4
			 0x831c4299, /// 0xbc8
			 0x039bba2c, /// 0xbcc
			 0x340326e2, /// 0xbd0
			 0x94aa3785, /// 0xbd4
			 0x57ce65b0, /// 0xbd8
			 0xd926957f, /// 0xbdc
			 0xf7952127, /// 0xbe0
			 0x56d4827c, /// 0xbe4
			 0x36492adb, /// 0xbe8
			 0x13d688a8, /// 0xbec
			 0xde25a66c, /// 0xbf0
			 0xb67efee6, /// 0xbf4
			 0x333c09f0, /// 0xbf8
			 0xfa4960e4, /// 0xbfc
			 0x67f36203, /// 0xc00
			 0xcdc7ee66, /// 0xc04
			 0x3c7efe2e, /// 0xc08
			 0xa682e0a8, /// 0xc0c
			 0x9d56d142, /// 0xc10
			 0x32b4c38b, /// 0xc14
			 0x7e286be4, /// 0xc18
			 0x671898de, /// 0xc1c
			 0x524fb25f, /// 0xc20
			 0xbe4f0580, /// 0xc24
			 0x1c6a83f6, /// 0xc28
			 0x9dbdaa1c, /// 0xc2c
			 0x9ffacfea, /// 0xc30
			 0x795cdf63, /// 0xc34
			 0x0f1ac161, /// 0xc38
			 0x3fed50af, /// 0xc3c
			 0x0efb4a6a, /// 0xc40
			 0xf9bf9b10, /// 0xc44
			 0x4daf17b2, /// 0xc48
			 0x14c65854, /// 0xc4c
			 0x88eb52e7, /// 0xc50
			 0x6ebedcff, /// 0xc54
			 0x0d463eb4, /// 0xc58
			 0xe94a8183, /// 0xc5c
			 0xa1e58246, /// 0xc60
			 0x9c43ae2f, /// 0xc64
			 0xb65a2a97, /// 0xc68
			 0xc8aed57e, /// 0xc6c
			 0x82cba264, /// 0xc70
			 0xbd952f50, /// 0xc74
			 0x4eddd232, /// 0xc78
			 0xcb83f79b, /// 0xc7c
			 0x4e43c0c6, /// 0xc80
			 0x95772510, /// 0xc84
			 0x483e5378, /// 0xc88
			 0x1e9af671, /// 0xc8c
			 0x3e8b64f2, /// 0xc90
			 0x7cabc41c, /// 0xc94
			 0x521abfa2, /// 0xc98
			 0x21eb540d, /// 0xc9c
			 0x4888c5fa, /// 0xca0
			 0xc4fe5778, /// 0xca4
			 0x50a824b3, /// 0xca8
			 0x7c2fa7ff, /// 0xcac
			 0x1524c998, /// 0xcb0
			 0x34716496, /// 0xcb4
			 0x134e0f7f, /// 0xcb8
			 0x30dfaf8a, /// 0xcbc
			 0x028cad2c, /// 0xcc0
			 0xc2975501, /// 0xcc4
			 0xbe457dcb, /// 0xcc8
			 0x132b38ab, /// 0xccc
			 0xf4b7ae9b, /// 0xcd0
			 0x3f4957d8, /// 0xcd4
			 0x6be18c3e, /// 0xcd8
			 0x4913cfaf, /// 0xcdc
			 0x1e848fe1, /// 0xce0
			 0x9fa43ba8, /// 0xce4
			 0x71fddde6, /// 0xce8
			 0x76a0b3b1, /// 0xcec
			 0xc61ed6f6, /// 0xcf0
			 0x21aacf01, /// 0xcf4
			 0x1d60439e, /// 0xcf8
			 0x21c7e018, /// 0xcfc
			 0x244cb9d8, /// 0xd00
			 0x9a7bba80, /// 0xd04
			 0xfa2b85be, /// 0xd08
			 0xc5908356, /// 0xd0c
			 0x97745d49, /// 0xd10
			 0x8bfc8041, /// 0xd14
			 0xf14b9d0e, /// 0xd18
			 0x01176cd9, /// 0xd1c
			 0x023411b8, /// 0xd20
			 0xa3865b47, /// 0xd24
			 0x07bdf084, /// 0xd28
			 0xf764419e, /// 0xd2c
			 0x56e3e1d4, /// 0xd30
			 0xf870703c, /// 0xd34
			 0xa2fbb00a, /// 0xd38
			 0xb603b3fa, /// 0xd3c
			 0x975b5436, /// 0xd40
			 0x6cd378e6, /// 0xd44
			 0x89a7529e, /// 0xd48
			 0x04724e20, /// 0xd4c
			 0xda4318c9, /// 0xd50
			 0x5015eea7, /// 0xd54
			 0x87b712bd, /// 0xd58
			 0x5410e845, /// 0xd5c
			 0x9eec0705, /// 0xd60
			 0x2380787d, /// 0xd64
			 0xc384abcc, /// 0xd68
			 0xa9684aa8, /// 0xd6c
			 0x16d83804, /// 0xd70
			 0x27218407, /// 0xd74
			 0x4158d82f, /// 0xd78
			 0x113c7715, /// 0xd7c
			 0xe429a1b6, /// 0xd80
			 0xb226938c, /// 0xd84
			 0xf4673701, /// 0xd88
			 0xb5e2f4f4, /// 0xd8c
			 0xb4a5a304, /// 0xd90
			 0x8d0d309e, /// 0xd94
			 0x1ef38fb9, /// 0xd98
			 0x7e5718f3, /// 0xd9c
			 0x1aec5405, /// 0xda0
			 0xc430b5cb, /// 0xda4
			 0x6846778c, /// 0xda8
			 0x19d9e1a7, /// 0xdac
			 0x3849cb58, /// 0xdb0
			 0xd2c04302, /// 0xdb4
			 0x1965250f, /// 0xdb8
			 0x6423f742, /// 0xdbc
			 0xa079db2a, /// 0xdc0
			 0x13f4b77d, /// 0xdc4
			 0x7209f430, /// 0xdc8
			 0xc9cda9e5, /// 0xdcc
			 0xe0c53707, /// 0xdd0
			 0x5ca307ba, /// 0xdd4
			 0x20309a7b, /// 0xdd8
			 0x07eb09a6, /// 0xddc
			 0xb97c8935, /// 0xde0
			 0xae38dde7, /// 0xde4
			 0xb5d7eb90, /// 0xde8
			 0x535f7ebe, /// 0xdec
			 0x674b04ac, /// 0xdf0
			 0xba734c31, /// 0xdf4
			 0x0a038745, /// 0xdf8
			 0xee51c5bf, /// 0xdfc
			 0x34dd7ea7, /// 0xe00
			 0xcec5ca02, /// 0xe04
			 0x4b0621eb, /// 0xe08
			 0x4195b6bd, /// 0xe0c
			 0x6a2f1f52, /// 0xe10
			 0xc952ff09, /// 0xe14
			 0x0d423a57, /// 0xe18
			 0xc2622022, /// 0xe1c
			 0x6dd46f8d, /// 0xe20
			 0xa8db00ab, /// 0xe24
			 0x2d26ebb1, /// 0xe28
			 0x08c8c853, /// 0xe2c
			 0x2e0b6267, /// 0xe30
			 0xae9fbc90, /// 0xe34
			 0x55ba9618, /// 0xe38
			 0xd6141f65, /// 0xe3c
			 0xf35b7331, /// 0xe40
			 0x5541dc6c, /// 0xe44
			 0x9b226933, /// 0xe48
			 0xc3c10fce, /// 0xe4c
			 0xcdf29ed3, /// 0xe50
			 0xf28ae635, /// 0xe54
			 0xd8a1c3e1, /// 0xe58
			 0xfc559d45, /// 0xe5c
			 0x8edbc828, /// 0xe60
			 0x9a7cd526, /// 0xe64
			 0x195cb868, /// 0xe68
			 0x52ffe71a, /// 0xe6c
			 0xd6c8b1c3, /// 0xe70
			 0xc82a5fb4, /// 0xe74
			 0xf8cc5abd, /// 0xe78
			 0xc3b8f025, /// 0xe7c
			 0x215a5d56, /// 0xe80
			 0xb8e51de7, /// 0xe84
			 0x728d74f2, /// 0xe88
			 0x7fd32264, /// 0xe8c
			 0x99f9d9b7, /// 0xe90
			 0xd1184344, /// 0xe94
			 0x9296d17e, /// 0xe98
			 0x7a23fbd1, /// 0xe9c
			 0x169b1777, /// 0xea0
			 0xed17e69e, /// 0xea4
			 0x967c00d6, /// 0xea8
			 0x5778d949, /// 0xeac
			 0x8f558f46, /// 0xeb0
			 0x26e7c1b4, /// 0xeb4
			 0x1aa8d1a7, /// 0xeb8
			 0xe6568bb5, /// 0xebc
			 0x9d70d14e, /// 0xec0
			 0x97ef35ea, /// 0xec4
			 0x7e19d5b9, /// 0xec8
			 0xa5b137ac, /// 0xecc
			 0x18bbeae6, /// 0xed0
			 0x0c352ef9, /// 0xed4
			 0xc319bb37, /// 0xed8
			 0x1cd1e534, /// 0xedc
			 0x3f5bd95b, /// 0xee0
			 0x3c421556, /// 0xee4
			 0xd2990fc6, /// 0xee8
			 0xa42544f1, /// 0xeec
			 0xe5defe58, /// 0xef0
			 0xb013d6e3, /// 0xef4
			 0xdbdb36f9, /// 0xef8
			 0x1726b4c7, /// 0xefc
			 0xdede356a, /// 0xf00
			 0x0c7f467f, /// 0xf04
			 0x3f1ba795, /// 0xf08
			 0x05e26f21, /// 0xf0c
			 0x3e2e18a1, /// 0xf10
			 0x0b170d13, /// 0xf14
			 0x2d89253f, /// 0xf18
			 0xab4c2cfa, /// 0xf1c
			 0x1840ee29, /// 0xf20
			 0xf8932465, /// 0xf24
			 0x5f9e8df0, /// 0xf28
			 0xb2cc22c3, /// 0xf2c
			 0x94eba517, /// 0xf30
			 0x3ce886a8, /// 0xf34
			 0xdbc194d0, /// 0xf38
			 0xeed28882, /// 0xf3c
			 0xc67fc3c4, /// 0xf40
			 0x65999f6c, /// 0xf44
			 0x89e4a15b, /// 0xf48
			 0xb4a217f8, /// 0xf4c
			 0x6e3c7686, /// 0xf50
			 0x189c5b4d, /// 0xf54
			 0xea749472, /// 0xf58
			 0x2ff889ed, /// 0xf5c
			 0x3de8533e, /// 0xf60
			 0x461f747c, /// 0xf64
			 0x25322e70, /// 0xf68
			 0x36f86c02, /// 0xf6c
			 0xc0e55901, /// 0xf70
			 0x7df50dc4, /// 0xf74
			 0x78577bf3, /// 0xf78
			 0x0c5515b4, /// 0xf7c
			 0x72f7ab23, /// 0xf80
			 0xf2472313, /// 0xf84
			 0x5bc2addc, /// 0xf88
			 0x9dc86edb, /// 0xf8c
			 0x92e65545, /// 0xf90
			 0xac70057a, /// 0xf94
			 0xbb0adcfa, /// 0xf98
			 0x4d81ad9a, /// 0xf9c
			 0x61b55124, /// 0xfa0
			 0x7507736d, /// 0xfa4
			 0x8e6e2e0a, /// 0xfa8
			 0x2a61bc7d, /// 0xfac
			 0x6dc02301, /// 0xfb0
			 0xd9691d51, /// 0xfb4
			 0xc68082c2, /// 0xfb8
			 0x76913134, /// 0xfbc
			 0x2dc994d3, /// 0xfc0
			 0xe21c7730, /// 0xfc4
			 0x8db81817, /// 0xfc8
			 0x28f5f70b, /// 0xfcc
			 0x8c507272, /// 0xfd0
			 0x7ebfc40b, /// 0xfd4
			 0x48bb5325, /// 0xfd8
			 0xf9ac4951, /// 0xfdc
			 0x6a4522c4, /// 0xfe0
			 0x1740a8f7, /// 0xfe4
			 0x590e801d, /// 0xfe8
			 0xc27c341b, /// 0xfec
			 0x0cab1d63, /// 0xff0
			 0xa6f59b11, /// 0xff4
			 0xda89c1d8, /// 0xff8
			 0xd8c4f77a /// last
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
			 0x000004a8,
			 0x0000063c,
			 0x0000000c,
			 0x000002fc,
			 0x00000064,
			 0x00000450,
			 0x00000190,
			 0x000002d8,

			 /// vpu register f2
			 0x41a80000,
			 0x41d80000,
			 0x41500000,
			 0x41a00000,
			 0x42000000,
			 0x41c80000,
			 0x41f00000,
			 0x41600000,

			 /// vpu register f3
			 0x41d00000,
			 0x41800000,
			 0x41300000,
			 0x41c80000,
			 0x41e00000,
			 0x3f800000,
			 0x40e00000,
			 0x41100000,

			 /// vpu register f4
			 0x41d00000,
			 0x41200000,
			 0x40400000,
			 0x41d00000,
			 0x41e80000,
			 0x41b00000,
			 0x41700000,
			 0x41500000,

			 /// vpu register f5
			 0x41a00000,
			 0x41f00000,
			 0x41500000,
			 0x41980000,
			 0x41980000,
			 0x40400000,
			 0x41200000,
			 0x41900000,

			 /// vpu register f6
			 0x41e80000,
			 0x41c00000,
			 0x40000000,
			 0x40c00000,
			 0x41500000,
			 0x41d00000,
			 0x41200000,
			 0x41200000,

			 /// vpu register f7
			 0x40400000,
			 0x41a80000,
			 0x41b00000,
			 0x3f800000,
			 0x41300000,
			 0x41f80000,
			 0x41e80000,
			 0x41000000,

			 /// vpu register f8
			 0x41900000,
			 0x41e00000,
			 0x42000000,
			 0x41f00000,
			 0x40a00000,
			 0x41600000,
			 0x41300000,
			 0x41b00000,

			 /// vpu register f9
			 0x41900000,
			 0x40800000,
			 0x42000000,
			 0x40a00000,
			 0x3f800000,
			 0x40800000,
			 0x41e00000,
			 0x41300000,

			 /// vpu register f10
			 0x41d00000,
			 0x41600000,
			 0x41980000,
			 0x41000000,
			 0x41700000,
			 0x41e80000,
			 0x3f800000,
			 0x41200000,

			 /// vpu register f11
			 0x41e00000,
			 0x41500000,
			 0x41800000,
			 0x41a00000,
			 0x41600000,
			 0x41700000,
			 0x41000000,
			 0x3f800000,

			 /// vpu register f12
			 0x41b80000,
			 0x40c00000,
			 0x41600000,
			 0x41d80000,
			 0x41b00000,
			 0x41800000,
			 0x40400000,
			 0x40a00000,

			 /// vpu register f13
			 0x41b00000,
			 0x41800000,
			 0x41500000,
			 0x41500000,
			 0x41700000,
			 0x40000000,
			 0x41c00000,
			 0x40800000,

			 /// vpu register f14
			 0x41600000,
			 0x40800000,
			 0x41c80000,
			 0x41b00000,
			 0x40000000,
			 0x41a00000,
			 0x42000000,
			 0x41200000,

			 /// vpu register f15
			 0x40000000,
			 0x41100000,
			 0x41880000,
			 0x41a00000,
			 0x41a80000,
			 0x3f800000,
			 0x41c00000,
			 0x40800000,

			 /// vpu register f16
			 0x41700000,
			 0x41f00000,
			 0x41700000,
			 0x41d00000,
			 0x41100000,
			 0x41900000,
			 0x41b00000,
			 0x41f80000,

			 /// vpu register f17
			 0x41900000,
			 0x40000000,
			 0x41800000,
			 0x41a80000,
			 0x41600000,
			 0x40a00000,
			 0x41600000,
			 0x41c00000,

			 /// vpu register f18
			 0x41300000,
			 0x40400000,
			 0x41100000,
			 0x3f800000,
			 0x41400000,
			 0x41200000,
			 0x40400000,
			 0x41900000,

			 /// vpu register f19
			 0x41a80000,
			 0x41700000,
			 0x41c80000,
			 0x40000000,
			 0x41c80000,
			 0x41c00000,
			 0x41400000,
			 0x41c00000,

			 /// vpu register f20
			 0x40400000,
			 0x40800000,
			 0x41880000,
			 0x41b80000,
			 0x41e80000,
			 0x42000000,
			 0x41c00000,
			 0x41000000,

			 /// vpu register f21
			 0x41400000,
			 0x41100000,
			 0x41300000,
			 0x41000000,
			 0x41a80000,
			 0x41500000,
			 0x41a80000,
			 0x40e00000,

			 /// vpu register f22
			 0x41400000,
			 0x41100000,
			 0x40400000,
			 0x41200000,
			 0x41e80000,
			 0x41d00000,
			 0x41600000,
			 0x40400000,

			 /// vpu register f23
			 0x40400000,
			 0x41200000,
			 0x41600000,
			 0x41d00000,
			 0x40e00000,
			 0x41700000,
			 0x41b00000,
			 0x41b00000,

			 /// vpu register f24
			 0x41700000,
			 0x40400000,
			 0x42000000,
			 0x41800000,
			 0x41d80000,
			 0x41b00000,
			 0x40e00000,
			 0x40a00000,

			 /// vpu register f25
			 0x40400000,
			 0x41300000,
			 0x40800000,
			 0x41500000,
			 0x40000000,
			 0x42000000,
			 0x42000000,
			 0x41e80000,

			 /// vpu register f26
			 0x41880000,
			 0x40400000,
			 0x41900000,
			 0x41800000,
			 0x41e00000,
			 0x41a00000,
			 0x41f80000,
			 0x41880000,

			 /// vpu register f27
			 0x41400000,
			 0x41e00000,
			 0x40800000,
			 0x41100000,
			 0x3f800000,
			 0x41f00000,
			 0x41600000,
			 0x40c00000,

			 /// vpu register f28
			 0x41300000,
			 0x41000000,
			 0x41100000,
			 0x41d00000,
			 0x41d80000,
			 0x41400000,
			 0x41a80000,
			 0x40000000,

			 /// vpu register f29
			 0x41880000,
			 0x3f800000,
			 0x41a00000,
			 0x41e80000,
			 0x41a00000,
			 0x3f800000,
			 0x42000000,
			 0x40800000,

			 /// vpu register f30
			 0x40400000,
			 0x41b80000,
			 0x40000000,
			 0x41600000,
			 0x41f80000,
			 0x41d80000,
			 0x40c00000,
			 0x41880000,

			 /// vpu register f31
			 0x40e00000,
			 0x41900000,
			 0x41e00000,
			 0x41d00000,
			 0x41f80000,
			 0x41f00000,
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
		"fsc32h.ps f24, x14 (x13)\n"                          ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f28, x11 (x12)\n"                          ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f27, x11 (x13)\n"                          ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f10, x14 (x15)\n"                          ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f2, x11 (x15)\n"                           ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f30, x14 (x14)\n"                          ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f0, x14 (x14)\n"                           ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f30, x14 (x14)\n"                          ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f28, x12 (x14)\n"                          ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f18, x11 (x11)\n"                          ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f3, x13 (x15)\n"                           ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f27, x15 (x15)\n"                          ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f29, x15 (x14)\n"                          ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f5, x11 (x15)\n"                           ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f22, x12 (x14)\n"                          ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f23, x14 (x11)\n"                          ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f27, x11 (x13)\n"                          ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f18, x12 (x12)\n"                          ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f23, x14 (x13)\n"                          ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f11, x11 (x12)\n"                          ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f8, x11 (x12)\n"                           ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f19, x11 (x12)\n"                          ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f23, x13 (x15)\n"                          ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f29, x11 (x15)\n"                          ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f20, x15 (x13)\n"                          ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f16, x14 (x14)\n"                          ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f1, x13 (x14)\n"                           ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f24, x12 (x13)\n"                          ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f0, x11 (x14)\n"                           ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f12, x11 (x13)\n"                          ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f11, x13 (x13)\n"                          ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f18, x11 (x15)\n"                          ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f19, x13 (x14)\n"                          ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f11, x13 (x11)\n"                          ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f19, x14 (x12)\n"                          ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f17, x14 (x15)\n"                          ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f27, x14 (x14)\n"                          ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f13, x13 (x11)\n"                          ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f25, x12 (x14)\n"                          ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f1, x14 (x14)\n"                           ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f11, x15 (x12)\n"                          ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f10, x13 (x11)\n"                          ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f4, x11 (x13)\n"                           ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f10, x12 (x13)\n"                          ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f6, x15 (x13)\n"                           ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f26, x12 (x11)\n"                          ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f17, x14 (x12)\n"                          ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f18, x12 (x13)\n"                          ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f13, x15 (x15)\n"                          ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f26, x15 (x14)\n"                          ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f25, x14 (x11)\n"                          ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f9, x12 (x11)\n"                           ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f30, x11 (x11)\n"                          ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f2, x12 (x15)\n"                           ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f9, x14 (x11)\n"                           ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f9, x11 (x12)\n"                           ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f18, x15 (x13)\n"                          ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f13, x12 (x13)\n"                          ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f25, x13 (x12)\n"                          ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f5, x11 (x12)\n"                           ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f30, x14 (x14)\n"                          ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f4, x11 (x11)\n"                           ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f12, x14 (x14)\n"                          ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f18, x13 (x13)\n"                          ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f19, x15 (x13)\n"                          ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f28, x12 (x11)\n"                          ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f7, x12 (x13)\n"                           ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f15, x13 (x15)\n"                          ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f17, x11 (x14)\n"                          ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f22, x12 (x14)\n"                          ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f30, x12 (x15)\n"                          ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f4, x12 (x12)\n"                           ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f21, x13 (x12)\n"                          ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f25, x12 (x15)\n"                          ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f10, x11 (x12)\n"                          ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f23, x11 (x14)\n"                          ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f4, x14 (x15)\n"                           ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f23, x11 (x14)\n"                          ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f14, x15 (x14)\n"                          ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f17, x11 (x15)\n"                          ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f30, x15 (x12)\n"                          ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f2, x12 (x11)\n"                           ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f2, x11 (x15)\n"                           ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f25, x11 (x13)\n"                          ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f5, x11 (x13)\n"                           ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f21, x14 (x15)\n"                          ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f25, x11 (x15)\n"                          ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f15, x15 (x12)\n"                          ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f18, x14 (x15)\n"                          ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f6, x11 (x14)\n"                           ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f21, x11 (x11)\n"                          ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f24, x15 (x15)\n"                          ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f18, x14 (x14)\n"                          ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f22, x12 (x15)\n"                          ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f13, x12 (x12)\n"                          ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f13, x11 (x12)\n"                          ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f10, x13 (x12)\n"                          ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f17, x15 (x11)\n"                          ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsc32h.ps f28, x15 (x13)\n"                          ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
