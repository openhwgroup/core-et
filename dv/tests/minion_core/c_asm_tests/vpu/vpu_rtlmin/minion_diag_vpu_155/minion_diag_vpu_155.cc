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
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00004
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00008
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0000c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00010
			 0x00000000,                                                  // zero                                        /// 00014
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00018
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0001c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00024
			 0x4b000000,                                                  // 8388608.0                                   /// 00028
			 0x0c600000,                                                  // Leading 1s:                                 /// 0002c
			 0xffc00001,                                                  // -signaling NaN                              /// 00030
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00034
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00038
			 0xff800000,                                                  // -inf                                        /// 0003c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00040
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00044
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00048
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0004c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0xffc00001,                                                  // -signaling NaN                              /// 00054
			 0xffc00001,                                                  // -signaling NaN                              /// 00058
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0005c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00064
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00070
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00074
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00078
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00080
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00084
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0008c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00094
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00098
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0009c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00100
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00104
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0010c
			 0x00000000,                                                  // zero                                        /// 00110
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00118
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0011c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00120
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00124
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00130
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00134
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00138
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0013c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00140
			 0x0e000001,                                                  // Trailing 1s:                                /// 00144
			 0xffc00001,                                                  // -signaling NaN                              /// 00148
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0014c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00154
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0015c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00168
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0016c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00170
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00174
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00178
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0017c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00180
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00184
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00188
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0018c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00190
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00194
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00198
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0019c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001e4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00200
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00204
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00208
			 0x33333333,                                                  // 4 random values                             /// 0020c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0e000001,                                                  // Trailing 1s:                                /// 00214
			 0x33333333,                                                  // 4 random values                             /// 00218
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00220
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00224
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00228
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0022c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00230
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00234
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00238
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0023c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00244
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00248
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00250
			 0x0c700000,                                                  // Leading 1s:                                 /// 00254
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00258
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0025c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00260
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00268
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00270
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00274
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0027c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00284
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00288
			 0xffc00001,                                                  // -signaling NaN                              /// 0028c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00294
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00298
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0029c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x3f028f5c,                                                  // 0.51                                        /// 002c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0xbf028f5c,                                                  // -0.51                                       /// 002e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00300
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00304
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0030c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00310
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x3f028f5c,                                                  // 0.51                                        /// 00318
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00324
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x0c400000,                                                  // Leading 1s:                                 /// 0032c
			 0x33333333,                                                  // 4 random values                             /// 00330
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00334
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00338
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00340
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00344
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00348
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0034c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00354
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00358
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00364
			 0x0e000001,                                                  // Trailing 1s:                                /// 00368
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00370
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00374
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00378
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00380
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00384
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00388
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0038c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00390
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00394
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0039c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003c0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00404
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0040c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00410
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00414
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0041c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00420
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00424
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00428
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00430
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00434
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00438
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0043c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0xcb000000,                                                  // -8388608.0                                  /// 00444
			 0x33333333,                                                  // 4 random values                             /// 00448
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0044c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00454
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00458
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0045c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00460
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00464
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00468
			 0x0c780000,                                                  // Leading 1s:                                 /// 0046c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00470
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0047c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00480
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00484
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00488
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00498
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 004a8
			 0x33333333,                                                  // 4 random values                             /// 004ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00504
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00508
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00510
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00514
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00520
			 0x0e000007,                                                  // Trailing 1s:                                /// 00524
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x00000000,                                                  // zero                                        /// 00534
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00538
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0053c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00548
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00550
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00554
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00558
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0055c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0056c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00570
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00578
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0057c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00580
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00588
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0058c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00590
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00598
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0059c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005b4
			 0x0c400000,                                                  // Leading 1s:                                 /// 005b8
			 0x00000000,                                                  // zero                                        /// 005bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0xff800000,                                                  // -inf                                        /// 005c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00600
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00604
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00608
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0060c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00610
			 0xbf028f5c,                                                  // -0.51                                       /// 00614
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00618
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0061c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00620
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00624
			 0x0c400000,                                                  // Leading 1s:                                 /// 00628
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0062c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00630
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00638
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0064c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00650
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0065c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00660
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00664
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00668
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0066c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00670
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00674
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00678
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0067c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00688
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00690
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00694
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00698
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0069c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006b0
			 0x00000000,                                                  // zero                                        /// 006b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 006fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00700
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00704
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0070c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00710
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00718
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0071c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00720
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00724
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00728
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00738
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x3f028f5c,                                                  // 0.51                                        /// 00740
			 0xcb000000,                                                  // -8388608.0                                  /// 00744
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00748
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00750
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00754
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0075c
			 0x00000000,                                                  // zero                                        /// 00760
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00764
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00768
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0076c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00770
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00774
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00778
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0077c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00780
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00784
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00788
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x7f800000,                                                  // inf                                         /// 00790
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00794
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0079c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007d0
			 0x7fc00001,                                                  // signaling NaN                               /// 007d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00804
			 0x0e000003,                                                  // Trailing 1s:                                /// 00808
			 0x0c400000,                                                  // Leading 1s:                                 /// 0080c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00814
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00818
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0081c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00820
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00824
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00828
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0082c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00830
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00834
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00838
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0083c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00840
			 0x0c400000,                                                  // Leading 1s:                                 /// 00844
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00848
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00858
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0085c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00860
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00864
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00868
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00870
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00874
			 0x0c400000,                                                  // Leading 1s:                                 /// 00878
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00880
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00884
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00888
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0088c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00890
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0xcb000000,                                                  // -8388608.0                                  /// 00898
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008ac
			 0xffc00001,                                                  // -signaling NaN                              /// 008b0
			 0x33333333,                                                  // 4 random values                             /// 008b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008c8
			 0x55555555,                                                  // 4 random values                             /// 008cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00904
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00908
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0090c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00910
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00914
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00918
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0091c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00920
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00934
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0093c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00944
			 0x55555555,                                                  // 4 random values                             /// 00948
			 0xbf028f5c,                                                  // -0.51                                       /// 0094c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00950
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00958
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00968
			 0x33333333,                                                  // 4 random values                             /// 0096c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00978
			 0xcb000000,                                                  // -8388608.0                                  /// 0097c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00980
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00984
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00988
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0098c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00990
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00994
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0099c
			 0x80000000,                                                  // -zero                                       /// 009a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009a4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x3f028f5c,                                                  // 0.51                                        /// 009ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 009d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a04
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x55555555,                                                  // 4 random values                             /// 00a14
			 0x00000000,                                                  // zero                                        /// 00a18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a1c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a20
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a24
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a34
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a3c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a58
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a5c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a68
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a70
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a78
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a7c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a80
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a8c
			 0x55555555,                                                  // 4 random values                             /// 00a90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a94
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00aa4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00aa8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00aac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ab0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ab4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ab8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00abc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ac0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ac4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ac8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ad0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ad8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00adc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ae0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00af4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00af8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00afc
			 0xbf028f5c,                                                  // -0.51                                       /// 00b00
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b10
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b40
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b4c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b68
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b70
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b88
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b8c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b90
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b98
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ba0
			 0x55555555,                                                  // 4 random values                             /// 00ba4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ba8
			 0xbf028f5c,                                                  // -0.51                                       /// 00bac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bb0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80000000,                                                  // -zero                                       /// 00bb8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bc0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bc4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bc8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bcc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bd0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bdc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00be0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00be4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00be8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bf0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bf4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bf8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c00
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c0c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c1c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c24
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c28
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c2c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c50
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x7fc00001,                                                  // signaling NaN                               /// 00c58
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c70
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c7c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0xffc00001,                                                  // -signaling NaN                              /// 00c84
			 0x4b000000,                                                  // 8388608.0                                   /// 00c88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c8c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c90
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c9c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cb0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cbc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cc8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ccc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cdc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ce4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cf0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cf8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d00
			 0xbf028f5c,                                                  // -0.51                                       /// 00d04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d10
			 0x80000000,                                                  // -zero                                       /// 00d14
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d24
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d38
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d48
			 0xffc00001,                                                  // -signaling NaN                              /// 00d4c
			 0xff800000,                                                  // -inf                                        /// 00d50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d54
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d58
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d68
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d74
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d78
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d84
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d90
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d94
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00da0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00da4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00da8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00db0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00db8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dbc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dc0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dc4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dc8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dcc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dd8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ddc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00de0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00df0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00df4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dfc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e00
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x3f028f5c,                                                  // 0.51                                        /// 00e0c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e18
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e28
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e2c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e30
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x55555555,                                                  // 4 random values                             /// 00e44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e64
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e8c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x55555555,                                                  // 4 random values                             /// 00ea4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ea8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00eb8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ebc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ec4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ec8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ed0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ed4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ed8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ee0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ee4
			 0x00000000,                                                  // zero                                        /// 00ee8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00eec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ef0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ef8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00efc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f00
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f0c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f10
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f2c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f30
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f34
			 0xffc00001,                                                  // -signaling NaN                              /// 00f38
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f44
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f48
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f4c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f60
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0xcb000000,                                                  // -8388608.0                                  /// 00f7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f84
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f88
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fa0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fa4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fa8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fac
			 0xffc00001,                                                  // -signaling NaN                              /// 00fb0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fc4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fd0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fd8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fe0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fe4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fec
			 0xff800000,                                                  // -inf                                        /// 00ff0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ff4
			 0xff800000,                                                  // -inf                                        /// 00ff8
			 0x0c7fe000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x6a263a46, /// 0x0
			 0x58ca0913, /// 0x4
			 0x83a329de, /// 0x8
			 0x0dbc1922, /// 0xc
			 0xf95184c2, /// 0x10
			 0xd5733749, /// 0x14
			 0x8e4a1c91, /// 0x18
			 0x6bf6e5b7, /// 0x1c
			 0xb34a7f8f, /// 0x20
			 0xb7a72cff, /// 0x24
			 0x03bbe297, /// 0x28
			 0x118e38a9, /// 0x2c
			 0x968403d0, /// 0x30
			 0x3b6f4267, /// 0x34
			 0x54382778, /// 0x38
			 0x503854e7, /// 0x3c
			 0x627db7af, /// 0x40
			 0xdffc5463, /// 0x44
			 0x09a81f07, /// 0x48
			 0x1d02de11, /// 0x4c
			 0xa594a35f, /// 0x50
			 0x4fb04f3f, /// 0x54
			 0xdbe0284e, /// 0x58
			 0xb9f348bc, /// 0x5c
			 0x04663916, /// 0x60
			 0xd4be935a, /// 0x64
			 0x49e578bf, /// 0x68
			 0x743ae3c6, /// 0x6c
			 0x41edebfd, /// 0x70
			 0x6f7273a6, /// 0x74
			 0xecac6bc9, /// 0x78
			 0xac65446a, /// 0x7c
			 0x2173612f, /// 0x80
			 0x1175b77f, /// 0x84
			 0xc61bb72f, /// 0x88
			 0x26c96b97, /// 0x8c
			 0xf9a30aee, /// 0x90
			 0xaf6a6f4f, /// 0x94
			 0x3f62d363, /// 0x98
			 0x6d69f65f, /// 0x9c
			 0x32bfdb7a, /// 0xa0
			 0x95ad282f, /// 0xa4
			 0x249f2b6f, /// 0xa8
			 0x25fa6f07, /// 0xac
			 0xc1782813, /// 0xb0
			 0x1bf40ffd, /// 0xb4
			 0xd3a476da, /// 0xb8
			 0x8cc7c45e, /// 0xbc
			 0x8ec069aa, /// 0xc0
			 0x9c22823c, /// 0xc4
			 0x7862f050, /// 0xc8
			 0x1c19b3db, /// 0xcc
			 0x74bace5c, /// 0xd0
			 0x1628f563, /// 0xd4
			 0xe0070cb2, /// 0xd8
			 0x691b4123, /// 0xdc
			 0xde22110d, /// 0xe0
			 0x4ccb28d3, /// 0xe4
			 0x774cd0c1, /// 0xe8
			 0xe89ee2ec, /// 0xec
			 0xe213bc27, /// 0xf0
			 0xfc63f685, /// 0xf4
			 0xcf115306, /// 0xf8
			 0xb379bbce, /// 0xfc
			 0xf1699acb, /// 0x100
			 0xa485d06a, /// 0x104
			 0xc8683922, /// 0x108
			 0x01053df5, /// 0x10c
			 0xc5eb89ca, /// 0x110
			 0x4a83cbfd, /// 0x114
			 0xbe1d986e, /// 0x118
			 0x49951ceb, /// 0x11c
			 0xcc2de83f, /// 0x120
			 0x58ed14ec, /// 0x124
			 0xd215355c, /// 0x128
			 0x724c6b31, /// 0x12c
			 0x34bf7c4f, /// 0x130
			 0x8fedd82a, /// 0x134
			 0x2890a2dd, /// 0x138
			 0x6a09ce94, /// 0x13c
			 0x10a10655, /// 0x140
			 0x92d84233, /// 0x144
			 0x1d7fce2a, /// 0x148
			 0x4544d04f, /// 0x14c
			 0x02246ac1, /// 0x150
			 0x566146dd, /// 0x154
			 0x1e299a4e, /// 0x158
			 0x67390f94, /// 0x15c
			 0xca941db2, /// 0x160
			 0x3017a4ad, /// 0x164
			 0x66ebc22f, /// 0x168
			 0x60056fc5, /// 0x16c
			 0x96e8d288, /// 0x170
			 0x086c9649, /// 0x174
			 0xe90eed52, /// 0x178
			 0xa912aeb3, /// 0x17c
			 0x0b2511f3, /// 0x180
			 0xd06e7fff, /// 0x184
			 0x9347fe31, /// 0x188
			 0xc68edb45, /// 0x18c
			 0x870fa66b, /// 0x190
			 0x8cb4a57e, /// 0x194
			 0x6ead6e47, /// 0x198
			 0x280f928f, /// 0x19c
			 0x5997d72b, /// 0x1a0
			 0x2632b725, /// 0x1a4
			 0xca412ad6, /// 0x1a8
			 0xb89833e7, /// 0x1ac
			 0xdeacf5c5, /// 0x1b0
			 0xfd271ec0, /// 0x1b4
			 0x08bc1d06, /// 0x1b8
			 0x28ba788e, /// 0x1bc
			 0xf1593ad6, /// 0x1c0
			 0xae740ea1, /// 0x1c4
			 0xccd35f8a, /// 0x1c8
			 0x35fce1f4, /// 0x1cc
			 0xb8ce42c2, /// 0x1d0
			 0x13ba0980, /// 0x1d4
			 0x8b47756f, /// 0x1d8
			 0x29c87a80, /// 0x1dc
			 0xaa0e12cc, /// 0x1e0
			 0xe81a3279, /// 0x1e4
			 0x92def6e7, /// 0x1e8
			 0x4c27e2b6, /// 0x1ec
			 0xc048d327, /// 0x1f0
			 0x2f52fdec, /// 0x1f4
			 0x95616cfd, /// 0x1f8
			 0x885a6e0b, /// 0x1fc
			 0xf830bf23, /// 0x200
			 0x61857785, /// 0x204
			 0xb0e37966, /// 0x208
			 0x6d24fe7a, /// 0x20c
			 0x72a5cabb, /// 0x210
			 0x031d4181, /// 0x214
			 0x76e88f7a, /// 0x218
			 0x24a4d176, /// 0x21c
			 0xd2f4f771, /// 0x220
			 0xa198e6e3, /// 0x224
			 0x1c14d493, /// 0x228
			 0x0affb5ac, /// 0x22c
			 0x904848e4, /// 0x230
			 0x43556a1f, /// 0x234
			 0x60387dea, /// 0x238
			 0xc5ceda31, /// 0x23c
			 0xb816903a, /// 0x240
			 0xcf889f91, /// 0x244
			 0x340758e4, /// 0x248
			 0xcbd10e76, /// 0x24c
			 0xff1520b9, /// 0x250
			 0x1a8b94d4, /// 0x254
			 0x35baf748, /// 0x258
			 0x22675f72, /// 0x25c
			 0x82906862, /// 0x260
			 0x01eb24a8, /// 0x264
			 0x7dfd2c6f, /// 0x268
			 0x562c3043, /// 0x26c
			 0xade169ea, /// 0x270
			 0xd143d613, /// 0x274
			 0x9a5af8c0, /// 0x278
			 0xea613ca4, /// 0x27c
			 0x13239353, /// 0x280
			 0x9811cb61, /// 0x284
			 0xfa9c248f, /// 0x288
			 0x9695fa79, /// 0x28c
			 0x791507ce, /// 0x290
			 0x764a2a42, /// 0x294
			 0xbfe7a314, /// 0x298
			 0xc49866a3, /// 0x29c
			 0xb5b2609a, /// 0x2a0
			 0x746f2c3b, /// 0x2a4
			 0x6daa3be2, /// 0x2a8
			 0x29f12ff7, /// 0x2ac
			 0x274abda5, /// 0x2b0
			 0x5de7e3d9, /// 0x2b4
			 0x65691b60, /// 0x2b8
			 0x13f6b2b3, /// 0x2bc
			 0x44e46b8e, /// 0x2c0
			 0xb3067edc, /// 0x2c4
			 0x4e198bb2, /// 0x2c8
			 0x577fb340, /// 0x2cc
			 0x5c17bcd7, /// 0x2d0
			 0x1d716adf, /// 0x2d4
			 0x29bf9b97, /// 0x2d8
			 0xdd895593, /// 0x2dc
			 0x41865dc9, /// 0x2e0
			 0x20325476, /// 0x2e4
			 0x16fd64f5, /// 0x2e8
			 0x25a4493a, /// 0x2ec
			 0x3c346992, /// 0x2f0
			 0x1dd5bd18, /// 0x2f4
			 0xcacac4c9, /// 0x2f8
			 0xf7aa1719, /// 0x2fc
			 0x08acbee5, /// 0x300
			 0xf169257e, /// 0x304
			 0x2a3f1f50, /// 0x308
			 0x4a5ebc69, /// 0x30c
			 0xab516f7c, /// 0x310
			 0x6dadac99, /// 0x314
			 0x023d9ef3, /// 0x318
			 0x754b8069, /// 0x31c
			 0x9eddb182, /// 0x320
			 0x1f8ea34a, /// 0x324
			 0x882dd73e, /// 0x328
			 0xa4db227a, /// 0x32c
			 0xb08e1ca8, /// 0x330
			 0x5b3289ef, /// 0x334
			 0x84af9a06, /// 0x338
			 0xb9020710, /// 0x33c
			 0xc31303a8, /// 0x340
			 0xb8bf1a90, /// 0x344
			 0xa5c3d0ac, /// 0x348
			 0x1300148b, /// 0x34c
			 0xd35e276f, /// 0x350
			 0x448a5fd2, /// 0x354
			 0xbbb4b671, /// 0x358
			 0x0f3b5934, /// 0x35c
			 0xe3ec46ef, /// 0x360
			 0xdd69bbe1, /// 0x364
			 0xf8a15332, /// 0x368
			 0x91bdb4be, /// 0x36c
			 0x9ac39c72, /// 0x370
			 0x75abce12, /// 0x374
			 0x6d9682f7, /// 0x378
			 0x55826cb4, /// 0x37c
			 0x2db56608, /// 0x380
			 0x7926ef6c, /// 0x384
			 0xde61f806, /// 0x388
			 0x3ef0339f, /// 0x38c
			 0xdd792465, /// 0x390
			 0x19f1cbcb, /// 0x394
			 0x91393365, /// 0x398
			 0x98204573, /// 0x39c
			 0xd088f6bf, /// 0x3a0
			 0x315dc473, /// 0x3a4
			 0x418164a7, /// 0x3a8
			 0xe23d6c6d, /// 0x3ac
			 0x768b48c4, /// 0x3b0
			 0x79f814e1, /// 0x3b4
			 0xd144555d, /// 0x3b8
			 0xa8a5df54, /// 0x3bc
			 0xd56527f2, /// 0x3c0
			 0xc274bcb6, /// 0x3c4
			 0x35967262, /// 0x3c8
			 0x8e208f0d, /// 0x3cc
			 0x8bc31edf, /// 0x3d0
			 0x5e753641, /// 0x3d4
			 0x06ddea52, /// 0x3d8
			 0x0d53311c, /// 0x3dc
			 0x330d9f7f, /// 0x3e0
			 0x6d8461c0, /// 0x3e4
			 0xec537a64, /// 0x3e8
			 0x27124d2c, /// 0x3ec
			 0x1b925566, /// 0x3f0
			 0xa5291425, /// 0x3f4
			 0x4412a5c9, /// 0x3f8
			 0x1a40103c, /// 0x3fc
			 0x36b003d1, /// 0x400
			 0xc0a66a92, /// 0x404
			 0x34f41e16, /// 0x408
			 0x8fb9202f, /// 0x40c
			 0x469c78d1, /// 0x410
			 0x27c212da, /// 0x414
			 0x48aa6337, /// 0x418
			 0x802f75a1, /// 0x41c
			 0x3dba366d, /// 0x420
			 0x4bcd7a16, /// 0x424
			 0x465146f2, /// 0x428
			 0x5ec0e92d, /// 0x42c
			 0xceb06d9d, /// 0x430
			 0x40d0d2fb, /// 0x434
			 0x407a4a81, /// 0x438
			 0xd31d31a8, /// 0x43c
			 0x6a9c8403, /// 0x440
			 0x9a319fd7, /// 0x444
			 0x693ba3a1, /// 0x448
			 0xf2914820, /// 0x44c
			 0x3f1f041d, /// 0x450
			 0x99ebd0a1, /// 0x454
			 0x64a85c4e, /// 0x458
			 0x2ab3c9ca, /// 0x45c
			 0x6abb41aa, /// 0x460
			 0xdabf9cc7, /// 0x464
			 0x5ace4a4b, /// 0x468
			 0x9f54730f, /// 0x46c
			 0x283d75a0, /// 0x470
			 0x24120925, /// 0x474
			 0x4a892d34, /// 0x478
			 0x68dd71fa, /// 0x47c
			 0x5b8c7d4b, /// 0x480
			 0xe11bc100, /// 0x484
			 0x96c64a28, /// 0x488
			 0x943e998b, /// 0x48c
			 0x176e2eae, /// 0x490
			 0x136ec79d, /// 0x494
			 0x13438b8a, /// 0x498
			 0xc5d19196, /// 0x49c
			 0x7595a7d0, /// 0x4a0
			 0x032dd13c, /// 0x4a4
			 0x421c7902, /// 0x4a8
			 0x8c893111, /// 0x4ac
			 0x3079043c, /// 0x4b0
			 0x341bfd51, /// 0x4b4
			 0x268eacad, /// 0x4b8
			 0x2ed9e637, /// 0x4bc
			 0xcde1cff7, /// 0x4c0
			 0xd9967b27, /// 0x4c4
			 0x2f454cd2, /// 0x4c8
			 0xd28823a0, /// 0x4cc
			 0xbcea03d3, /// 0x4d0
			 0xde66e5a5, /// 0x4d4
			 0xf8571f0b, /// 0x4d8
			 0x1f529393, /// 0x4dc
			 0x5f209dd5, /// 0x4e0
			 0xb47ba33c, /// 0x4e4
			 0xad521665, /// 0x4e8
			 0xeb535dc3, /// 0x4ec
			 0x50d4f8f0, /// 0x4f0
			 0x11738707, /// 0x4f4
			 0x942aa39c, /// 0x4f8
			 0x5c201fb8, /// 0x4fc
			 0x28bba7d2, /// 0x500
			 0xbb6dc14c, /// 0x504
			 0x85f08d8f, /// 0x508
			 0x7e762873, /// 0x50c
			 0x65db36f8, /// 0x510
			 0xb6ff4ced, /// 0x514
			 0x1815580f, /// 0x518
			 0x54979a09, /// 0x51c
			 0x1dee9593, /// 0x520
			 0xdf35c99e, /// 0x524
			 0x7bac7d0e, /// 0x528
			 0x58aec448, /// 0x52c
			 0x6760b4fa, /// 0x530
			 0x8ed17483, /// 0x534
			 0x5c5de4a5, /// 0x538
			 0xd7217267, /// 0x53c
			 0xd763fcd9, /// 0x540
			 0xee8cdbfb, /// 0x544
			 0xe846f21f, /// 0x548
			 0x3ae97b28, /// 0x54c
			 0xc9f6a1da, /// 0x550
			 0x86e38c17, /// 0x554
			 0xc7a3996c, /// 0x558
			 0xe08d6ed7, /// 0x55c
			 0x990bcd8a, /// 0x560
			 0x102d9dd1, /// 0x564
			 0xe9d76d81, /// 0x568
			 0x2beb1a29, /// 0x56c
			 0x01524113, /// 0x570
			 0xb9524a51, /// 0x574
			 0xe7be5a15, /// 0x578
			 0xaa820b55, /// 0x57c
			 0x8e31dcd1, /// 0x580
			 0xa496f921, /// 0x584
			 0xcb27c4ae, /// 0x588
			 0xf848d21c, /// 0x58c
			 0x679a29e6, /// 0x590
			 0x807d39c1, /// 0x594
			 0x8f36c6a2, /// 0x598
			 0x1a186e5a, /// 0x59c
			 0x2a199a69, /// 0x5a0
			 0x8c3680ff, /// 0x5a4
			 0xf1cfe32b, /// 0x5a8
			 0x22fa9306, /// 0x5ac
			 0x0a55ecd7, /// 0x5b0
			 0x8d93eb7f, /// 0x5b4
			 0x46f3702e, /// 0x5b8
			 0xb9350602, /// 0x5bc
			 0xab37bec0, /// 0x5c0
			 0x13ac6dfc, /// 0x5c4
			 0x4d2fa458, /// 0x5c8
			 0xea38e5d4, /// 0x5cc
			 0x88884e77, /// 0x5d0
			 0x1256ab4e, /// 0x5d4
			 0xa5dd9fc6, /// 0x5d8
			 0x00d99022, /// 0x5dc
			 0xc8018fcb, /// 0x5e0
			 0x351db14a, /// 0x5e4
			 0xcce4dc49, /// 0x5e8
			 0xfc921e04, /// 0x5ec
			 0x6af8cdae, /// 0x5f0
			 0x5bb89778, /// 0x5f4
			 0x805c3fc4, /// 0x5f8
			 0x9a13dc32, /// 0x5fc
			 0x809e3e8d, /// 0x600
			 0x315c407d, /// 0x604
			 0xde8d1aba, /// 0x608
			 0x6849278c, /// 0x60c
			 0xd5cab675, /// 0x610
			 0xb1f89f98, /// 0x614
			 0x6dbf8089, /// 0x618
			 0x5291a179, /// 0x61c
			 0x57108386, /// 0x620
			 0xc317e8aa, /// 0x624
			 0x37af0cc6, /// 0x628
			 0x77e21172, /// 0x62c
			 0x7015a733, /// 0x630
			 0x2c7d4190, /// 0x634
			 0xfe3d6d77, /// 0x638
			 0x54d8d8c2, /// 0x63c
			 0xeacb32a4, /// 0x640
			 0x4891d70a, /// 0x644
			 0x627aef4d, /// 0x648
			 0x3b6b8dad, /// 0x64c
			 0x456d24f0, /// 0x650
			 0xc7b6fc56, /// 0x654
			 0xf00c6f1d, /// 0x658
			 0x3449afdd, /// 0x65c
			 0xf0488366, /// 0x660
			 0x6a81d7f8, /// 0x664
			 0x21b26a5d, /// 0x668
			 0x405710fb, /// 0x66c
			 0x8b1d0ab2, /// 0x670
			 0x0f6d9349, /// 0x674
			 0xfaf04278, /// 0x678
			 0x29d720a4, /// 0x67c
			 0x791f9682, /// 0x680
			 0x620c6656, /// 0x684
			 0x0cc4802a, /// 0x688
			 0x44f59afb, /// 0x68c
			 0x68793827, /// 0x690
			 0x507a7ba7, /// 0x694
			 0x1c7fe724, /// 0x698
			 0xc538ad62, /// 0x69c
			 0xb199c854, /// 0x6a0
			 0x3c08c09d, /// 0x6a4
			 0x7cd85896, /// 0x6a8
			 0x3bd3596a, /// 0x6ac
			 0x408b6fa0, /// 0x6b0
			 0x612f6ab9, /// 0x6b4
			 0x36eba9f4, /// 0x6b8
			 0xcc22251d, /// 0x6bc
			 0x077909b8, /// 0x6c0
			 0x478cfad0, /// 0x6c4
			 0x796bac25, /// 0x6c8
			 0x648bbb22, /// 0x6cc
			 0x881f49b9, /// 0x6d0
			 0xa379b572, /// 0x6d4
			 0x59d44b11, /// 0x6d8
			 0x0728b655, /// 0x6dc
			 0x2df7de95, /// 0x6e0
			 0xca4be6d8, /// 0x6e4
			 0x10c8eac2, /// 0x6e8
			 0x061acaae, /// 0x6ec
			 0x2cc5aaa2, /// 0x6f0
			 0x9766f745, /// 0x6f4
			 0x71ef5e74, /// 0x6f8
			 0x632ae5e3, /// 0x6fc
			 0x0740fc56, /// 0x700
			 0xaf276bae, /// 0x704
			 0x59000555, /// 0x708
			 0xa37aed1d, /// 0x70c
			 0xe1e8588b, /// 0x710
			 0xf72a669a, /// 0x714
			 0xfffd0c3a, /// 0x718
			 0xc8f6b47b, /// 0x71c
			 0x0a42abf1, /// 0x720
			 0x8e48b1d0, /// 0x724
			 0x4d74543a, /// 0x728
			 0x767cd2ba, /// 0x72c
			 0x1de200a7, /// 0x730
			 0x54a9808f, /// 0x734
			 0x4dc8ed46, /// 0x738
			 0xc94d37b8, /// 0x73c
			 0xda02f0c5, /// 0x740
			 0x526238ad, /// 0x744
			 0x43be42ca, /// 0x748
			 0xc94e87d0, /// 0x74c
			 0x79968a13, /// 0x750
			 0x8634934f, /// 0x754
			 0x058a73f0, /// 0x758
			 0x6daea185, /// 0x75c
			 0xe685dada, /// 0x760
			 0xafa4b457, /// 0x764
			 0x13fc1916, /// 0x768
			 0x9225059e, /// 0x76c
			 0x3ca17cf5, /// 0x770
			 0x6402d41f, /// 0x774
			 0x6b8cdb0f, /// 0x778
			 0x49d3e82c, /// 0x77c
			 0x570b0b10, /// 0x780
			 0x6eb7481d, /// 0x784
			 0xe63f88bb, /// 0x788
			 0xfae11fe6, /// 0x78c
			 0x33033541, /// 0x790
			 0xfabbfab1, /// 0x794
			 0x8adef5ac, /// 0x798
			 0xf68222c4, /// 0x79c
			 0xecfe72e8, /// 0x7a0
			 0x7a14403b, /// 0x7a4
			 0x9588dc16, /// 0x7a8
			 0xba56b44e, /// 0x7ac
			 0xb6d2fb3c, /// 0x7b0
			 0xd3772c6d, /// 0x7b4
			 0x0f40ec94, /// 0x7b8
			 0xf242624c, /// 0x7bc
			 0xe5c17adf, /// 0x7c0
			 0xb65d6671, /// 0x7c4
			 0x4b189f58, /// 0x7c8
			 0xcbdef685, /// 0x7cc
			 0xb548b95a, /// 0x7d0
			 0x8b2d0af9, /// 0x7d4
			 0xe156fc96, /// 0x7d8
			 0x59bdb273, /// 0x7dc
			 0xf20e7020, /// 0x7e0
			 0x44dcccb9, /// 0x7e4
			 0xf1c50792, /// 0x7e8
			 0x4b0cf033, /// 0x7ec
			 0x926cf15c, /// 0x7f0
			 0xc6348e38, /// 0x7f4
			 0x0665215a, /// 0x7f8
			 0x230eab49, /// 0x7fc
			 0x50b2aae7, /// 0x800
			 0xfa37515b, /// 0x804
			 0xf7d35687, /// 0x808
			 0x6b681148, /// 0x80c
			 0xbead3268, /// 0x810
			 0x8a1e748c, /// 0x814
			 0xedd2b443, /// 0x818
			 0x2c991a6d, /// 0x81c
			 0xb9195926, /// 0x820
			 0x7c300005, /// 0x824
			 0x9662164e, /// 0x828
			 0x57e17882, /// 0x82c
			 0xb81b42f7, /// 0x830
			 0xc6d421d5, /// 0x834
			 0x8d76d03b, /// 0x838
			 0x5d4c1c21, /// 0x83c
			 0x94afe650, /// 0x840
			 0x2f64d9c2, /// 0x844
			 0x6aecda01, /// 0x848
			 0xfa970b2c, /// 0x84c
			 0x2785d053, /// 0x850
			 0x2b6b033d, /// 0x854
			 0x4afbedcf, /// 0x858
			 0x7b5f8cbf, /// 0x85c
			 0x84f90c1a, /// 0x860
			 0xac6df28c, /// 0x864
			 0x5e8c31f3, /// 0x868
			 0x3cb7d2a7, /// 0x86c
			 0xe076a01e, /// 0x870
			 0x1cf3dc24, /// 0x874
			 0x0fe66431, /// 0x878
			 0x7852bd4d, /// 0x87c
			 0xbf789983, /// 0x880
			 0x24d33ea0, /// 0x884
			 0x5bcb5bcb, /// 0x888
			 0xd1731fb1, /// 0x88c
			 0x2ed9782e, /// 0x890
			 0x5e026ac9, /// 0x894
			 0xa96d6339, /// 0x898
			 0xc0e51af1, /// 0x89c
			 0x6166bd98, /// 0x8a0
			 0x538fd4be, /// 0x8a4
			 0xd78bcd82, /// 0x8a8
			 0x1a4e91a6, /// 0x8ac
			 0xed2b921e, /// 0x8b0
			 0x15735073, /// 0x8b4
			 0x1daa136a, /// 0x8b8
			 0x11c19806, /// 0x8bc
			 0xa9b9c80d, /// 0x8c0
			 0x04ebd421, /// 0x8c4
			 0x5e9ebb3d, /// 0x8c8
			 0xa914cbc8, /// 0x8cc
			 0xf5769537, /// 0x8d0
			 0xaaa97447, /// 0x8d4
			 0x3adf0043, /// 0x8d8
			 0x23364d82, /// 0x8dc
			 0x57393e0e, /// 0x8e0
			 0x8c3becf7, /// 0x8e4
			 0x8268fdd6, /// 0x8e8
			 0xfc853e4a, /// 0x8ec
			 0xb9f02610, /// 0x8f0
			 0x7d9b9a62, /// 0x8f4
			 0x124180e8, /// 0x8f8
			 0x6adeb050, /// 0x8fc
			 0x09ea58df, /// 0x900
			 0x900f8220, /// 0x904
			 0xba4eb60d, /// 0x908
			 0xeb164f8a, /// 0x90c
			 0x7e5a1541, /// 0x910
			 0xf08fea6b, /// 0x914
			 0xbf16cdca, /// 0x918
			 0xfb613e38, /// 0x91c
			 0xf13b6216, /// 0x920
			 0x7e372080, /// 0x924
			 0x5a6579d9, /// 0x928
			 0xffe6553a, /// 0x92c
			 0x77d3ffcf, /// 0x930
			 0x038f2cc2, /// 0x934
			 0x2b56f839, /// 0x938
			 0xb1b5716d, /// 0x93c
			 0x93aca5da, /// 0x940
			 0x4d820b5f, /// 0x944
			 0xcd458a02, /// 0x948
			 0x89e9c874, /// 0x94c
			 0xc750aaf4, /// 0x950
			 0xeb4e6187, /// 0x954
			 0x32b6f4d8, /// 0x958
			 0x28f3d300, /// 0x95c
			 0x94370a9a, /// 0x960
			 0xe7f1ef20, /// 0x964
			 0xd85f8bf8, /// 0x968
			 0xbedcde43, /// 0x96c
			 0x8a1e2871, /// 0x970
			 0x1d990739, /// 0x974
			 0x321b49ee, /// 0x978
			 0xcf48f2ff, /// 0x97c
			 0x04059f22, /// 0x980
			 0xe95f0dbd, /// 0x984
			 0x97aada2e, /// 0x988
			 0x266ef98c, /// 0x98c
			 0x6a170037, /// 0x990
			 0x94ca3583, /// 0x994
			 0x25709ee8, /// 0x998
			 0x3c81692d, /// 0x99c
			 0x0cc18b2a, /// 0x9a0
			 0xb0acae6d, /// 0x9a4
			 0x73fe2529, /// 0x9a8
			 0x7bbfc40f, /// 0x9ac
			 0x957bd53e, /// 0x9b0
			 0x37afeedd, /// 0x9b4
			 0xdce464aa, /// 0x9b8
			 0x47736b38, /// 0x9bc
			 0x6d167305, /// 0x9c0
			 0xd6ba8f07, /// 0x9c4
			 0x84679a55, /// 0x9c8
			 0x102960dd, /// 0x9cc
			 0xd15b8063, /// 0x9d0
			 0xe8623640, /// 0x9d4
			 0x787d1114, /// 0x9d8
			 0x60086a76, /// 0x9dc
			 0xe418a16a, /// 0x9e0
			 0xeea7f8e6, /// 0x9e4
			 0x13fcea84, /// 0x9e8
			 0xa407a8ee, /// 0x9ec
			 0x138a195e, /// 0x9f0
			 0xd0d73965, /// 0x9f4
			 0x816c0881, /// 0x9f8
			 0x133da0d1, /// 0x9fc
			 0xd1cea95d, /// 0xa00
			 0xd3abe905, /// 0xa04
			 0x706f4639, /// 0xa08
			 0x0f274101, /// 0xa0c
			 0xa20fecaf, /// 0xa10
			 0xeab1e05d, /// 0xa14
			 0x125e389b, /// 0xa18
			 0x9d9417d2, /// 0xa1c
			 0x226e8f9f, /// 0xa20
			 0x34d07719, /// 0xa24
			 0x9491c47b, /// 0xa28
			 0x4c9260fd, /// 0xa2c
			 0x84a08b7e, /// 0xa30
			 0xd10b661f, /// 0xa34
			 0xcc79042d, /// 0xa38
			 0xf67382ac, /// 0xa3c
			 0xf7f6ded6, /// 0xa40
			 0xf1e64a44, /// 0xa44
			 0x83f43e8e, /// 0xa48
			 0x71b81895, /// 0xa4c
			 0x4653e28e, /// 0xa50
			 0x97bcc5e8, /// 0xa54
			 0x64020707, /// 0xa58
			 0x89414f51, /// 0xa5c
			 0x22c90827, /// 0xa60
			 0x1268ad9b, /// 0xa64
			 0x5adbd494, /// 0xa68
			 0xc7f6b11e, /// 0xa6c
			 0xe08bf226, /// 0xa70
			 0xd25371e9, /// 0xa74
			 0x59a30a1e, /// 0xa78
			 0x96bc99b5, /// 0xa7c
			 0x3a56ed64, /// 0xa80
			 0x4bdee177, /// 0xa84
			 0xc8819523, /// 0xa88
			 0xe2bf1fca, /// 0xa8c
			 0x60828d59, /// 0xa90
			 0xcd3012ad, /// 0xa94
			 0xae03e640, /// 0xa98
			 0xabc441a2, /// 0xa9c
			 0x63c92020, /// 0xaa0
			 0x42902c0c, /// 0xaa4
			 0x8c1409b0, /// 0xaa8
			 0xac78f601, /// 0xaac
			 0xa02e2671, /// 0xab0
			 0xa71ae6aa, /// 0xab4
			 0xa34ba738, /// 0xab8
			 0x6d7d4a8e, /// 0xabc
			 0xff823656, /// 0xac0
			 0x58e2dbcf, /// 0xac4
			 0x728ad91f, /// 0xac8
			 0x9cd66a18, /// 0xacc
			 0x519eba2c, /// 0xad0
			 0x41167487, /// 0xad4
			 0xf7caa208, /// 0xad8
			 0x328fe2ff, /// 0xadc
			 0xbb8da127, /// 0xae0
			 0x7ee26187, /// 0xae4
			 0x4a5cc3a6, /// 0xae8
			 0x3d9d90e8, /// 0xaec
			 0x2f76568a, /// 0xaf0
			 0xf6029b44, /// 0xaf4
			 0x0aed2e0e, /// 0xaf8
			 0x7248e961, /// 0xafc
			 0x264c85a5, /// 0xb00
			 0xb7b8da82, /// 0xb04
			 0x78865b9e, /// 0xb08
			 0x3720e10a, /// 0xb0c
			 0x45b8c849, /// 0xb10
			 0x1673abee, /// 0xb14
			 0xbc93a6b7, /// 0xb18
			 0x54c7ffdb, /// 0xb1c
			 0x656f1013, /// 0xb20
			 0x3b3875e2, /// 0xb24
			 0x5cfa056e, /// 0xb28
			 0xaaa310a6, /// 0xb2c
			 0xb71b6df4, /// 0xb30
			 0xb4a2bb67, /// 0xb34
			 0x8f6f99bb, /// 0xb38
			 0x27c1f7b7, /// 0xb3c
			 0xc9b0209b, /// 0xb40
			 0xf0cdd798, /// 0xb44
			 0x669201ae, /// 0xb48
			 0x62bb3752, /// 0xb4c
			 0xca13900e, /// 0xb50
			 0x1998dcde, /// 0xb54
			 0xd627bae1, /// 0xb58
			 0x02300c17, /// 0xb5c
			 0x2b6a25fb, /// 0xb60
			 0x54d212fa, /// 0xb64
			 0xdf2c01b7, /// 0xb68
			 0x6b6aa784, /// 0xb6c
			 0x47b58b0a, /// 0xb70
			 0x6bced062, /// 0xb74
			 0xe440ec85, /// 0xb78
			 0x5b483858, /// 0xb7c
			 0xf8bc6d15, /// 0xb80
			 0x17d90fa4, /// 0xb84
			 0x6021a4c9, /// 0xb88
			 0x366b8fb5, /// 0xb8c
			 0xf0e72973, /// 0xb90
			 0x9e23998a, /// 0xb94
			 0x628f39bd, /// 0xb98
			 0xdcf47d74, /// 0xb9c
			 0xadfc871f, /// 0xba0
			 0xa1a4fca4, /// 0xba4
			 0x42dfd8eb, /// 0xba8
			 0xa5aff904, /// 0xbac
			 0x7a1877ee, /// 0xbb0
			 0x2038bf58, /// 0xbb4
			 0x291b1b4c, /// 0xbb8
			 0xf5975de1, /// 0xbbc
			 0x856cd71b, /// 0xbc0
			 0x6980a1a0, /// 0xbc4
			 0x5158e0fd, /// 0xbc8
			 0xb4ee2f31, /// 0xbcc
			 0x58165361, /// 0xbd0
			 0xca33428a, /// 0xbd4
			 0xd93bcb6e, /// 0xbd8
			 0x1d87a809, /// 0xbdc
			 0x993770da, /// 0xbe0
			 0xb396c92a, /// 0xbe4
			 0x472152f9, /// 0xbe8
			 0x9c965186, /// 0xbec
			 0xc49e1970, /// 0xbf0
			 0x73866251, /// 0xbf4
			 0xba3c060f, /// 0xbf8
			 0x1f4490d9, /// 0xbfc
			 0x853694aa, /// 0xc00
			 0x6616d8d2, /// 0xc04
			 0xe50b994d, /// 0xc08
			 0xc0b3a5dc, /// 0xc0c
			 0x67e53dbb, /// 0xc10
			 0x9f469533, /// 0xc14
			 0xec406d6a, /// 0xc18
			 0xadab5af6, /// 0xc1c
			 0x4115a537, /// 0xc20
			 0xfec9c894, /// 0xc24
			 0x93a3670b, /// 0xc28
			 0x51b8bfab, /// 0xc2c
			 0xaf907a5a, /// 0xc30
			 0xc5c4bea9, /// 0xc34
			 0x2fe18836, /// 0xc38
			 0xb7674331, /// 0xc3c
			 0x1499c944, /// 0xc40
			 0xa87e7756, /// 0xc44
			 0xc010cae3, /// 0xc48
			 0xf40e14d8, /// 0xc4c
			 0x2d23fa3b, /// 0xc50
			 0xe71349a3, /// 0xc54
			 0x74edc481, /// 0xc58
			 0xf84a012d, /// 0xc5c
			 0xaa2e0f28, /// 0xc60
			 0x86899385, /// 0xc64
			 0x6116eaa5, /// 0xc68
			 0x423a81f5, /// 0xc6c
			 0x7f278d67, /// 0xc70
			 0x211c3a7b, /// 0xc74
			 0xa4d16374, /// 0xc78
			 0x2f7506f2, /// 0xc7c
			 0x8f6c23d9, /// 0xc80
			 0x4ed14463, /// 0xc84
			 0x99166d69, /// 0xc88
			 0x17f3e2f1, /// 0xc8c
			 0xeb8a6003, /// 0xc90
			 0xc173f299, /// 0xc94
			 0xa1a349c5, /// 0xc98
			 0xa7c2b392, /// 0xc9c
			 0x08a3aa27, /// 0xca0
			 0x1c68770e, /// 0xca4
			 0x18f90975, /// 0xca8
			 0x83bc003a, /// 0xcac
			 0xb852e32d, /// 0xcb0
			 0x176d0034, /// 0xcb4
			 0x67e49a71, /// 0xcb8
			 0x3686f3c1, /// 0xcbc
			 0x2e5135fa, /// 0xcc0
			 0xc324dd0c, /// 0xcc4
			 0xc7766d58, /// 0xcc8
			 0x8c4a3b5c, /// 0xccc
			 0x05252664, /// 0xcd0
			 0xc54706ed, /// 0xcd4
			 0x5cc10fa6, /// 0xcd8
			 0x8334b224, /// 0xcdc
			 0x78464f84, /// 0xce0
			 0xe78af1eb, /// 0xce4
			 0xb787d55b, /// 0xce8
			 0xab1d7f6b, /// 0xcec
			 0x80350d94, /// 0xcf0
			 0xbf508327, /// 0xcf4
			 0x058d5d02, /// 0xcf8
			 0x255d8ebd, /// 0xcfc
			 0x48da685f, /// 0xd00
			 0xa15a4d9c, /// 0xd04
			 0xbefd60a7, /// 0xd08
			 0x534bb820, /// 0xd0c
			 0x2910e595, /// 0xd10
			 0xa36965bf, /// 0xd14
			 0xc561e5a9, /// 0xd18
			 0xf8392bef, /// 0xd1c
			 0x7ec48299, /// 0xd20
			 0xfd64e593, /// 0xd24
			 0x53b70797, /// 0xd28
			 0x9d00f8c2, /// 0xd2c
			 0xe44e11de, /// 0xd30
			 0x22bf8f8f, /// 0xd34
			 0xf8327fa0, /// 0xd38
			 0xb61f14ff, /// 0xd3c
			 0x944f94ff, /// 0xd40
			 0x439c86f9, /// 0xd44
			 0x720348ed, /// 0xd48
			 0x54b470ea, /// 0xd4c
			 0xc9b3ba23, /// 0xd50
			 0xc8a38fe5, /// 0xd54
			 0x7fb077d2, /// 0xd58
			 0x3cf13fa2, /// 0xd5c
			 0x99868d16, /// 0xd60
			 0x1bce18f3, /// 0xd64
			 0x6da73b87, /// 0xd68
			 0xc477539e, /// 0xd6c
			 0x669c1963, /// 0xd70
			 0xc6489def, /// 0xd74
			 0xea481e4d, /// 0xd78
			 0x8d02b8e4, /// 0xd7c
			 0xea293274, /// 0xd80
			 0x92d2504f, /// 0xd84
			 0xf7a5143c, /// 0xd88
			 0xa2248e46, /// 0xd8c
			 0x0d338c47, /// 0xd90
			 0x81e26195, /// 0xd94
			 0x0bf6e5cb, /// 0xd98
			 0xb522762a, /// 0xd9c
			 0x535559e7, /// 0xda0
			 0x76e86b0e, /// 0xda4
			 0x529bc7e4, /// 0xda8
			 0x6e014a8f, /// 0xdac
			 0xad99ae00, /// 0xdb0
			 0x0066a53e, /// 0xdb4
			 0xbf28e9a8, /// 0xdb8
			 0x9875cbc8, /// 0xdbc
			 0xb84ad9b0, /// 0xdc0
			 0x351c7184, /// 0xdc4
			 0x64989a29, /// 0xdc8
			 0xd62ddcb8, /// 0xdcc
			 0x74453bdd, /// 0xdd0
			 0x1ef53244, /// 0xdd4
			 0x992ba8c9, /// 0xdd8
			 0x33d7796b, /// 0xddc
			 0xe41e104c, /// 0xde0
			 0xd97cd9bb, /// 0xde4
			 0xd3af1d80, /// 0xde8
			 0x79827f7a, /// 0xdec
			 0x8772e69f, /// 0xdf0
			 0xa27628e6, /// 0xdf4
			 0xc4c6769a, /// 0xdf8
			 0xfacb900c, /// 0xdfc
			 0x517442b3, /// 0xe00
			 0x799232ad, /// 0xe04
			 0xad2068f7, /// 0xe08
			 0x36a3d6d0, /// 0xe0c
			 0x4db3f197, /// 0xe10
			 0xde1f55d1, /// 0xe14
			 0x1a84963f, /// 0xe18
			 0xa9de425c, /// 0xe1c
			 0x90e9383b, /// 0xe20
			 0x7f245638, /// 0xe24
			 0x96b7eb2b, /// 0xe28
			 0xb50f695c, /// 0xe2c
			 0xaeec3974, /// 0xe30
			 0x3daba6d1, /// 0xe34
			 0x210af723, /// 0xe38
			 0x8ed97a60, /// 0xe3c
			 0x2f37b186, /// 0xe40
			 0xd05ef2da, /// 0xe44
			 0xdfb357ed, /// 0xe48
			 0xf8351125, /// 0xe4c
			 0xff68f072, /// 0xe50
			 0x449eb1a5, /// 0xe54
			 0x1601b1c4, /// 0xe58
			 0xbb8ce58c, /// 0xe5c
			 0x98660b7c, /// 0xe60
			 0x29de6165, /// 0xe64
			 0xc85c2583, /// 0xe68
			 0x71d6157f, /// 0xe6c
			 0xfc846a22, /// 0xe70
			 0x8c144995, /// 0xe74
			 0xcbe6e137, /// 0xe78
			 0x80c61eeb, /// 0xe7c
			 0xfed67446, /// 0xe80
			 0xf44bf1c0, /// 0xe84
			 0x07e180e7, /// 0xe88
			 0xb5f5fddf, /// 0xe8c
			 0xc8fe9018, /// 0xe90
			 0xd24185f7, /// 0xe94
			 0x1e0e06f2, /// 0xe98
			 0xc3032b9c, /// 0xe9c
			 0x1da1d2d8, /// 0xea0
			 0xa3b0c61d, /// 0xea4
			 0x2af094ac, /// 0xea8
			 0x0a24b43d, /// 0xeac
			 0xcb8125d7, /// 0xeb0
			 0x07950188, /// 0xeb4
			 0x144514d2, /// 0xeb8
			 0x4886b604, /// 0xebc
			 0xd7d29d13, /// 0xec0
			 0xba09a75b, /// 0xec4
			 0xb63eb9af, /// 0xec8
			 0xa5fdef8b, /// 0xecc
			 0x7206e262, /// 0xed0
			 0xa923479d, /// 0xed4
			 0xa46de6d8, /// 0xed8
			 0xf479947b, /// 0xedc
			 0x89fed02d, /// 0xee0
			 0x15228add, /// 0xee4
			 0xa6326b82, /// 0xee8
			 0xc5e5c4b1, /// 0xeec
			 0x7af78830, /// 0xef0
			 0xd585b1f0, /// 0xef4
			 0x0a7f9b7e, /// 0xef8
			 0x5da6cc61, /// 0xefc
			 0x934f4868, /// 0xf00
			 0x9c522c8c, /// 0xf04
			 0x41a5ba55, /// 0xf08
			 0x1df1cf3d, /// 0xf0c
			 0x5d58d80b, /// 0xf10
			 0xc0f8880c, /// 0xf14
			 0x7fbb977c, /// 0xf18
			 0xa3af1f5b, /// 0xf1c
			 0x8945958b, /// 0xf20
			 0x66feddec, /// 0xf24
			 0x980ee661, /// 0xf28
			 0x6374c8fa, /// 0xf2c
			 0xcdca74a7, /// 0xf30
			 0xb1d79365, /// 0xf34
			 0x79895108, /// 0xf38
			 0xc5449240, /// 0xf3c
			 0x9f4542a0, /// 0xf40
			 0x6f2b9052, /// 0xf44
			 0x59b2d0e7, /// 0xf48
			 0x5700f4a6, /// 0xf4c
			 0x2dd8f04d, /// 0xf50
			 0x42c049e1, /// 0xf54
			 0xd028af8c, /// 0xf58
			 0xd1abf7a4, /// 0xf5c
			 0x92748aca, /// 0xf60
			 0xcf82254f, /// 0xf64
			 0x137a66d6, /// 0xf68
			 0xe0371b6a, /// 0xf6c
			 0x7da705b2, /// 0xf70
			 0xd259319f, /// 0xf74
			 0x81a0c923, /// 0xf78
			 0x00c04f04, /// 0xf7c
			 0x8d04f6f5, /// 0xf80
			 0xa46ba083, /// 0xf84
			 0x62ed269e, /// 0xf88
			 0x7f273a94, /// 0xf8c
			 0xd06efb84, /// 0xf90
			 0x0b055196, /// 0xf94
			 0x5743f5dc, /// 0xf98
			 0x7bc334d3, /// 0xf9c
			 0xc33aa9e7, /// 0xfa0
			 0x0d5355c9, /// 0xfa4
			 0x702923d2, /// 0xfa8
			 0x5dfbea69, /// 0xfac
			 0x095c7345, /// 0xfb0
			 0x533e1ad7, /// 0xfb4
			 0xfc7f0665, /// 0xfb8
			 0xdbd5ce7e, /// 0xfbc
			 0x84560a0d, /// 0xfc0
			 0xea95e7fc, /// 0xfc4
			 0x7c2a4062, /// 0xfc8
			 0xc0b88dd2, /// 0xfcc
			 0x67f366b3, /// 0xfd0
			 0xf858223e, /// 0xfd4
			 0xb758c975, /// 0xfd8
			 0x53d9e67f, /// 0xfdc
			 0x7282d78d, /// 0xfe0
			 0x311784d4, /// 0xfe4
			 0x9d390808, /// 0xfe8
			 0xae947503, /// 0xfec
			 0x6abf434c, /// 0xff0
			 0x2912f5af, /// 0xff4
			 0x14e73163, /// 0xff8
			 0xb6f3fdef /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0xbf028f5c,                                                  // -0.51                                       /// 00004
			 0xbf028f5c,                                                  // -0.51                                       /// 00008
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0000c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00014
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00018
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00024
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00028
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00034
			 0x80000000,                                                  // -zero                                       /// 00038
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0003c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00048
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0004c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00054
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00060
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00064
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0006c
			 0x3f028f5c,                                                  // 0.51                                        /// 00070
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00074
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00078
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00080
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00084
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0008c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00090
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00098
			 0x00000000,                                                  // zero                                        /// 0009c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000cc
			 0x4b000000,                                                  // 8388608.0                                   /// 000d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 000e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00100
			 0xff800000,                                                  // -inf                                        /// 00104
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0010c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00110
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00114
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00118
			 0x00011111,                                                  // 9.7958E-41                                  /// 0011c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00120
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00124
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00128
			 0xff800000,                                                  // -inf                                        /// 0012c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00134
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0xff800000,                                                  // -inf                                        /// 0013c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00140
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0014c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x3f028f5c,                                                  // 0.51                                        /// 00154
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00158
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0015c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00160
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00164
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00168
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0016c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00174
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0017c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00180
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00184
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0018c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00190
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00198
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0019c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001b8
			 0x00000000,                                                  // zero                                        /// 001bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x33333333,                                                  // 4 random values                             /// 001c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001dc
			 0x7fc00001,                                                  // signaling NaN                               /// 001e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00200
			 0x7f800000,                                                  // inf                                         /// 00204
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0xff800000,                                                  // -inf                                        /// 00210
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0021c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00224
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0022c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00230
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00234
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00238
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0023c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00240
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00244
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0xffc00001,                                                  // -signaling NaN                              /// 0024c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00250
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80000000,                                                  // -zero                                       /// 00258
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0025c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00260
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00264
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00270
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00274
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00278
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0027c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00280
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00284
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00288
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0029c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002bc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x7f800000,                                                  // inf                                         /// 002e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00300
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00308
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0030c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00314
			 0x0e000001,                                                  // Trailing 1s:                                /// 00318
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00320
			 0x00011111,                                                  // 9.7958E-41                                  /// 00324
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00328
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0032c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00330
			 0x0c700000,                                                  // Leading 1s:                                 /// 00334
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x0c600000,                                                  // Leading 1s:                                 /// 00344
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0034c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00350
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00354
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0035c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00360
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00368
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0036c
			 0x33333333,                                                  // 4 random values                             /// 00370
			 0x00000000,                                                  // zero                                        /// 00374
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0037c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00380
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00384
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00388
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0038c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00390
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x80011111,                                                  // -9.7958E-41                                 /// 00398
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x4b000000,                                                  // 8388608.0                                   /// 003c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00400
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00408
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0040c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00410
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00414
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00418
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0041c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00428
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00430
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00434
			 0x0c780000,                                                  // Leading 1s:                                 /// 00438
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0043c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00448
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0044c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00450
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00454
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00458
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0045c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00460
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00464
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00468
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0046c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00478
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00488
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00490
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x0c400000,                                                  // Leading 1s:                                 /// 0049c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 004a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004bc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004cc
			 0x4b000000,                                                  // 8388608.0                                   /// 004d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 004f8
			 0x55555555,                                                  // 4 random values                             /// 004fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00508
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0050c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00510
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00514
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00524
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0052c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x0c700000,                                                  // Leading 1s:                                 /// 00534
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00538
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00540
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00548
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0054c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00550
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00558
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00560
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00568
			 0xbf028f5c,                                                  // -0.51                                       /// 0056c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00570
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0057c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00580
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00584
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0058c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00590
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00594
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00598
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0059c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005ac
			 0x00000000,                                                  // zero                                        /// 005b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005f4
			 0x33333333,                                                  // 4 random values                             /// 005f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00600
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x55555555,                                                  // 4 random values                             /// 00608
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0060c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00614
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0061c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00620
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00624
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00628
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0062c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00630
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0063c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0xffc00001,                                                  // -signaling NaN                              /// 00644
			 0x7f800000,                                                  // inf                                         /// 00648
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0064c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00650
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00654
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00658
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00660
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00664
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x3f028f5c,                                                  // 0.51                                        /// 00674
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00678
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x4b000000,                                                  // 8388608.0                                   /// 00680
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00684
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00688
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0068c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00690
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00694
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x0c600000,                                                  // Leading 1s:                                 /// 006a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00700
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00708
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0070c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00718
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0071c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00720
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x7f800000,                                                  // inf                                         /// 00728
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0072c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00730
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00734
			 0x33333333,                                                  // 4 random values                             /// 00738
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x3f028f5c,                                                  // 0.51                                        /// 00740
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00744
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00748
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0074c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x7fc00001,                                                  // signaling NaN                               /// 00754
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00758
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00764
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00768
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0076c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00770
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00780
			 0x0e000007,                                                  // Trailing 1s:                                /// 00784
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00788
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00790
			 0x4b000000,                                                  // 8388608.0                                   /// 00794
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0079c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x7f800000,                                                  // inf                                         /// 007a4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 007c0
			 0xffc00001,                                                  // -signaling NaN                              /// 007c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007cc
			 0xcb000000,                                                  // -8388608.0                                  /// 007d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00804
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0080c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00810
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0081c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00824
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00828
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00830
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x80011111,                                                  // -9.7958E-41                                 /// 00838
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0083c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00840
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00848
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x0c780000,                                                  // Leading 1s:                                 /// 00854
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0085c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00860
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00864
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0086c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00874
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00878
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0087c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00880
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00888
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00890
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00894
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00898
			 0x7f800000,                                                  // inf                                         /// 0089c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x80000000,                                                  // -zero                                       /// 008a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00900
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00904
			 0x80000000,                                                  // -zero                                       /// 00908
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0090c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00910
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00914
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00920
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00924
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0092c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00930
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00934
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0093c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00944
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0094c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00950
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00954
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0095c
			 0x33333333,                                                  // 4 random values                             /// 00960
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00968
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00978
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0097c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00984
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00988
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0098c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00994
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00998
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 009d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009f0
			 0x7f800000,                                                  // inf                                         /// 009f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a08
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a0c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a1c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a30
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a48
			 0xcb000000,                                                  // -8388608.0                                  /// 00a4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a50
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a60
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a6c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a70
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a84
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a8c
			 0x80000000,                                                  // -zero                                       /// 00a90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a98
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00aa0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00aa8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00aac
			 0x55555555,                                                  // 4 random values                             /// 00ab0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ab4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ab8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ac0
			 0x55555555,                                                  // 4 random values                             /// 00ac4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ac8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00acc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ad4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ad8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00adc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ae0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ae4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ae8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00aec
			 0x0c700000,                                                  // Leading 1s:                                 /// 00af0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b10
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b14
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b24
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b28
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b30
			 0xffc00001,                                                  // -signaling NaN                              /// 00b34
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b38
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b3c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b4c
			 0xcb000000,                                                  // -8388608.0                                  /// 00b50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b54
			 0xffc00001,                                                  // -signaling NaN                              /// 00b58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b6c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00000000,                                                  // zero                                        /// 00b74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b84
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b9c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ba0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ba4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ba8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bb4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bc0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x7fc00001,                                                  // signaling NaN                               /// 00bc8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bcc
			 0xff800000,                                                  // -inf                                        /// 00bd0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bdc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00be4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00be8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bf0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bf4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bf8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c04
			 0x80000000,                                                  // -zero                                       /// 00c08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c1c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c28
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c30
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0xffc00001,                                                  // -signaling NaN                              /// 00c48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c64
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c74
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c80
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c84
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c88
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c8c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c98
			 0x55555555,                                                  // 4 random values                             /// 00c9c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ca0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ca4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ca8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cb8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cc0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cc4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cc8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ccc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cd0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cd4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cdc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ce0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ce8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cf0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cf4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cf8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cfc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d04
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d0c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d10
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d24
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d2c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d30
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d58
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d64
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d74
			 0x33333333,                                                  // 4 random values                             /// 00d78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d7c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d80
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d8c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d90
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d94
			 0x55555555,                                                  // 4 random values                             /// 00d98
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00da4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00da8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00db0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00db8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dbc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dc8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dcc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dd0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dd4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dd8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ddc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00de0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00de8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00df4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00df8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dfc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x3f028f5c,                                                  // 0.51                                        /// 00e08
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e10
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e14
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e18
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e20
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e38
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e3c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e48
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e54
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e7c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e88
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e8c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e94
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0xbf028f5c,                                                  // -0.51                                       /// 00e9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ea8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00eb0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00eb4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ec0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ec8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00edc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ee0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ee4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ee8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x4b000000,                                                  // 8388608.0                                   /// 00ef0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x00000000,                                                  // zero                                        /// 00efc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f00
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f24
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x3f028f5c,                                                  // 0.51                                        /// 00f2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f3c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f50
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f68
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f80
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f94
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f98
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f9c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fa0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fa4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fa8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fb0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fc0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fc4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fcc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fd0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fd8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fdc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fe0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fe4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x7f800000,                                                  // inf                                         /// 00ff4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0xff7ffffe // max norm - 1ulp                               // SP - ve numbers                             /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x6ff75ed4, /// 0x0
			 0x6f6867f2, /// 0x4
			 0xc92c9b84, /// 0x8
			 0xe9d0954a, /// 0xc
			 0xa24b37fb, /// 0x10
			 0x21e08b64, /// 0x14
			 0x2d64d0c8, /// 0x18
			 0x0b8f40fb, /// 0x1c
			 0x6f30134a, /// 0x20
			 0x5f1d2b1d, /// 0x24
			 0xe0c5163a, /// 0x28
			 0x37783f38, /// 0x2c
			 0x9b733d7d, /// 0x30
			 0x3a7306f8, /// 0x34
			 0xb5fcc765, /// 0x38
			 0xbe066b56, /// 0x3c
			 0xf4f92302, /// 0x40
			 0x526185cf, /// 0x44
			 0x4e294763, /// 0x48
			 0xbc89277c, /// 0x4c
			 0xa31fbebb, /// 0x50
			 0x093dc3d7, /// 0x54
			 0x8eae7278, /// 0x58
			 0xd880031a, /// 0x5c
			 0xd5b42cb1, /// 0x60
			 0x76dd653e, /// 0x64
			 0x86412b93, /// 0x68
			 0xd33fa923, /// 0x6c
			 0x33e37f6f, /// 0x70
			 0xf2105a86, /// 0x74
			 0x45843ca6, /// 0x78
			 0xc45aa92d, /// 0x7c
			 0xe22bb321, /// 0x80
			 0xd5c2daba, /// 0x84
			 0x5718825e, /// 0x88
			 0x7da7e643, /// 0x8c
			 0x9c717b50, /// 0x90
			 0xc2b263c9, /// 0x94
			 0xdaf3ea67, /// 0x98
			 0x083b0de5, /// 0x9c
			 0xe83d5d07, /// 0xa0
			 0xabfba602, /// 0xa4
			 0x99e1bd8d, /// 0xa8
			 0xec22f651, /// 0xac
			 0x7e52ad44, /// 0xb0
			 0xb6adde1c, /// 0xb4
			 0x78119da3, /// 0xb8
			 0x8880b01b, /// 0xbc
			 0xf1b6778d, /// 0xc0
			 0xe9fb471b, /// 0xc4
			 0x6b5bdfd8, /// 0xc8
			 0x744316d0, /// 0xcc
			 0x5994124e, /// 0xd0
			 0x89e5068d, /// 0xd4
			 0x6dc385b2, /// 0xd8
			 0x32f1bdd8, /// 0xdc
			 0xf3d26b17, /// 0xe0
			 0x5bfb0661, /// 0xe4
			 0xb39986de, /// 0xe8
			 0x32ec183a, /// 0xec
			 0x012f1628, /// 0xf0
			 0x5d4669d9, /// 0xf4
			 0x6febd162, /// 0xf8
			 0x3ec4e3e7, /// 0xfc
			 0xaadd9d93, /// 0x100
			 0xe09bb103, /// 0x104
			 0xa355a2c7, /// 0x108
			 0x05aba065, /// 0x10c
			 0x4c882131, /// 0x110
			 0x4277fcdb, /// 0x114
			 0x703e7ba3, /// 0x118
			 0x46ef62ea, /// 0x11c
			 0x0cd66f99, /// 0x120
			 0xa6711e20, /// 0x124
			 0xff1a560e, /// 0x128
			 0x081bf661, /// 0x12c
			 0x769844c3, /// 0x130
			 0xbd79469e, /// 0x134
			 0xd49af3af, /// 0x138
			 0xf0eb0f36, /// 0x13c
			 0x41002b94, /// 0x140
			 0xfe500042, /// 0x144
			 0xae0a558d, /// 0x148
			 0xcfad41eb, /// 0x14c
			 0x9c4d17f1, /// 0x150
			 0xe8e99215, /// 0x154
			 0x7896f397, /// 0x158
			 0x019e438b, /// 0x15c
			 0x0261fb0c, /// 0x160
			 0x9ab9ac8f, /// 0x164
			 0x2a6a9dbe, /// 0x168
			 0xe02ed731, /// 0x16c
			 0x8c07f9db, /// 0x170
			 0xa0cd05f8, /// 0x174
			 0x88661dc1, /// 0x178
			 0x9d44f778, /// 0x17c
			 0xe1668f71, /// 0x180
			 0x1179c68a, /// 0x184
			 0x246f6d71, /// 0x188
			 0x874d298f, /// 0x18c
			 0xc0114f40, /// 0x190
			 0x5449a70c, /// 0x194
			 0xf2144d71, /// 0x198
			 0xf5bca9cd, /// 0x19c
			 0x3a1bf12a, /// 0x1a0
			 0x1a08f29d, /// 0x1a4
			 0xc274b229, /// 0x1a8
			 0xa2f1417e, /// 0x1ac
			 0x174b7ea2, /// 0x1b0
			 0x05ee8003, /// 0x1b4
			 0x2537857e, /// 0x1b8
			 0xcf9f11a3, /// 0x1bc
			 0x2bea5383, /// 0x1c0
			 0x8d463594, /// 0x1c4
			 0xaba9818d, /// 0x1c8
			 0xb2ccf788, /// 0x1cc
			 0xa2d1fd10, /// 0x1d0
			 0xddb28af1, /// 0x1d4
			 0xa251a01f, /// 0x1d8
			 0xe5ed2897, /// 0x1dc
			 0xc441b2c7, /// 0x1e0
			 0x5a4073dd, /// 0x1e4
			 0x25028cfe, /// 0x1e8
			 0x1a6c7aeb, /// 0x1ec
			 0xd50bea28, /// 0x1f0
			 0x4e1438ee, /// 0x1f4
			 0x08707533, /// 0x1f8
			 0x4844f70f, /// 0x1fc
			 0x36df333b, /// 0x200
			 0x0895b68c, /// 0x204
			 0xd19a61f1, /// 0x208
			 0x108c233b, /// 0x20c
			 0xf6476785, /// 0x210
			 0x92beb483, /// 0x214
			 0x16ebd915, /// 0x218
			 0x0785385e, /// 0x21c
			 0x44e3fc22, /// 0x220
			 0xf25b9ba7, /// 0x224
			 0x2a0924af, /// 0x228
			 0xb2336fdb, /// 0x22c
			 0x19b87364, /// 0x230
			 0x25d43330, /// 0x234
			 0x29fd150f, /// 0x238
			 0x9e48d88c, /// 0x23c
			 0x5e28ceb0, /// 0x240
			 0x4c9e0254, /// 0x244
			 0x4b892a02, /// 0x248
			 0x204bff1b, /// 0x24c
			 0xae1b5139, /// 0x250
			 0xfdb5485c, /// 0x254
			 0xb47f019e, /// 0x258
			 0xad0c0ae5, /// 0x25c
			 0x831bea05, /// 0x260
			 0xbe981a53, /// 0x264
			 0x58cac9c7, /// 0x268
			 0x4b3124a7, /// 0x26c
			 0x8ba18090, /// 0x270
			 0xb9103bc8, /// 0x274
			 0xcc4c8fef, /// 0x278
			 0xcef64805, /// 0x27c
			 0xe5fb75b5, /// 0x280
			 0x7391edc9, /// 0x284
			 0xa2f1a226, /// 0x288
			 0x2630cee6, /// 0x28c
			 0x9cab4a3e, /// 0x290
			 0x2c30e7b1, /// 0x294
			 0x5313afac, /// 0x298
			 0x98b42f4b, /// 0x29c
			 0xdee76d24, /// 0x2a0
			 0xbbdd0b86, /// 0x2a4
			 0x9a3f9256, /// 0x2a8
			 0xedcdc004, /// 0x2ac
			 0xe460772c, /// 0x2b0
			 0x83cb8a18, /// 0x2b4
			 0x32c88af0, /// 0x2b8
			 0x1f1ba2ee, /// 0x2bc
			 0xb77d741c, /// 0x2c0
			 0x5f2f8baf, /// 0x2c4
			 0x9b402ecc, /// 0x2c8
			 0xfc0911e4, /// 0x2cc
			 0x7d4627ff, /// 0x2d0
			 0x8ad116b7, /// 0x2d4
			 0x9f74d353, /// 0x2d8
			 0x131edaed, /// 0x2dc
			 0x12df9784, /// 0x2e0
			 0xe20dc92d, /// 0x2e4
			 0xfe797dc3, /// 0x2e8
			 0x6ba1f277, /// 0x2ec
			 0x2a30a223, /// 0x2f0
			 0xa6d16c04, /// 0x2f4
			 0xb7cc831a, /// 0x2f8
			 0x97e56602, /// 0x2fc
			 0x57f7cbdf, /// 0x300
			 0x11ac4201, /// 0x304
			 0x52203587, /// 0x308
			 0xac675bbd, /// 0x30c
			 0x36a76db7, /// 0x310
			 0xf0536a41, /// 0x314
			 0x85a896ec, /// 0x318
			 0x66f16759, /// 0x31c
			 0xb4780834, /// 0x320
			 0x840728f7, /// 0x324
			 0x9fe6af13, /// 0x328
			 0x9b7961e8, /// 0x32c
			 0xa53a98a5, /// 0x330
			 0x271474e7, /// 0x334
			 0x4ba1c52d, /// 0x338
			 0x6b75e393, /// 0x33c
			 0x86e525b0, /// 0x340
			 0xe98cbb87, /// 0x344
			 0xf5df69b6, /// 0x348
			 0x20b946ad, /// 0x34c
			 0x10b01cfc, /// 0x350
			 0x2ee46823, /// 0x354
			 0xd2d03a8e, /// 0x358
			 0x54ed9a76, /// 0x35c
			 0x2caa61ce, /// 0x360
			 0x1ff84b78, /// 0x364
			 0x9fe1f145, /// 0x368
			 0x2223310b, /// 0x36c
			 0x2d2c069c, /// 0x370
			 0xe0201b6f, /// 0x374
			 0xb1278eb5, /// 0x378
			 0x2bed1c4e, /// 0x37c
			 0x4bc261d9, /// 0x380
			 0x2fed57ea, /// 0x384
			 0xff01acac, /// 0x388
			 0x87b3d711, /// 0x38c
			 0x61f3d39c, /// 0x390
			 0x17d18897, /// 0x394
			 0x2ea1855d, /// 0x398
			 0x49c2b09e, /// 0x39c
			 0xdc5f0a80, /// 0x3a0
			 0xcb1521b6, /// 0x3a4
			 0x075bec86, /// 0x3a8
			 0xe43e27ca, /// 0x3ac
			 0xb2f9e442, /// 0x3b0
			 0x8aa0e2db, /// 0x3b4
			 0xb94cd428, /// 0x3b8
			 0xd5d4cef0, /// 0x3bc
			 0xada9dbe7, /// 0x3c0
			 0xfcf081e2, /// 0x3c4
			 0x020f49b5, /// 0x3c8
			 0x7fbdc466, /// 0x3cc
			 0x8cc32672, /// 0x3d0
			 0xcd912fcf, /// 0x3d4
			 0xda0a4cc4, /// 0x3d8
			 0x6710f2bf, /// 0x3dc
			 0xe9f020a2, /// 0x3e0
			 0x188a2201, /// 0x3e4
			 0x3590ceed, /// 0x3e8
			 0x1ed10cd3, /// 0x3ec
			 0xc3c84fd5, /// 0x3f0
			 0xe8772163, /// 0x3f4
			 0x3d414356, /// 0x3f8
			 0x87c95e47, /// 0x3fc
			 0xa93a841d, /// 0x400
			 0x50fe9927, /// 0x404
			 0x5fd6748c, /// 0x408
			 0xf2124d20, /// 0x40c
			 0x231d5e8b, /// 0x410
			 0x38ad0b63, /// 0x414
			 0x339b9d96, /// 0x418
			 0xc6130586, /// 0x41c
			 0x1044fca2, /// 0x420
			 0x8fcc2fa1, /// 0x424
			 0x3e9a5f62, /// 0x428
			 0xade0c1b7, /// 0x42c
			 0xa3dc7589, /// 0x430
			 0xb22142e5, /// 0x434
			 0x4bd4d84f, /// 0x438
			 0xbe072ddb, /// 0x43c
			 0xab80fc61, /// 0x440
			 0x60a40e12, /// 0x444
			 0x7ccadeee, /// 0x448
			 0x2306a0d7, /// 0x44c
			 0xc4822410, /// 0x450
			 0xc471a0ae, /// 0x454
			 0xc13a441c, /// 0x458
			 0x0b17cf7f, /// 0x45c
			 0xbd6b582d, /// 0x460
			 0x6277bdec, /// 0x464
			 0xac108ef8, /// 0x468
			 0xba2b8e3b, /// 0x46c
			 0x5ad5e064, /// 0x470
			 0xda751a88, /// 0x474
			 0xd2eb6d03, /// 0x478
			 0x29633432, /// 0x47c
			 0x47fc1816, /// 0x480
			 0xbfee05bf, /// 0x484
			 0x0e19a9ca, /// 0x488
			 0x41f3e226, /// 0x48c
			 0x09adbc50, /// 0x490
			 0x7124b0d0, /// 0x494
			 0x2de7d9f6, /// 0x498
			 0x3c5b1e90, /// 0x49c
			 0x55dcdef9, /// 0x4a0
			 0x0959f850, /// 0x4a4
			 0x52cb5b3c, /// 0x4a8
			 0x58a1cb33, /// 0x4ac
			 0x441abf49, /// 0x4b0
			 0x79bdc8ee, /// 0x4b4
			 0x8edb61e7, /// 0x4b8
			 0x1b2929a2, /// 0x4bc
			 0xa66e9f52, /// 0x4c0
			 0xb2ccfff6, /// 0x4c4
			 0xc9ce68f0, /// 0x4c8
			 0x1e776276, /// 0x4cc
			 0x221acbc8, /// 0x4d0
			 0x2f85d2b4, /// 0x4d4
			 0x73d1e9da, /// 0x4d8
			 0x83f77239, /// 0x4dc
			 0x119784cb, /// 0x4e0
			 0x929d39b3, /// 0x4e4
			 0x4916ee8d, /// 0x4e8
			 0xbccfbbbf, /// 0x4ec
			 0x84c2bc6b, /// 0x4f0
			 0x0e10504b, /// 0x4f4
			 0xb7e7687d, /// 0x4f8
			 0xaea0b175, /// 0x4fc
			 0xe6fe65e8, /// 0x500
			 0x54fbe2de, /// 0x504
			 0x4643e925, /// 0x508
			 0x0865c88f, /// 0x50c
			 0x9eca6e05, /// 0x510
			 0x62aeb327, /// 0x514
			 0xab866d3d, /// 0x518
			 0x5eb36273, /// 0x51c
			 0xb89b2026, /// 0x520
			 0x63e7760d, /// 0x524
			 0x259d6996, /// 0x528
			 0x0f9ce97b, /// 0x52c
			 0x776a3971, /// 0x530
			 0x81312fc4, /// 0x534
			 0x589d4725, /// 0x538
			 0x16a0156f, /// 0x53c
			 0x5b05cf38, /// 0x540
			 0xdecd6bd9, /// 0x544
			 0x5ed28b25, /// 0x548
			 0x9bd5f60a, /// 0x54c
			 0x12e0e486, /// 0x550
			 0xa6a1d6db, /// 0x554
			 0x40fc805b, /// 0x558
			 0x988fd9d0, /// 0x55c
			 0xd328d1a0, /// 0x560
			 0x85f7b2ea, /// 0x564
			 0x4e45f687, /// 0x568
			 0x09d120ea, /// 0x56c
			 0x3771644b, /// 0x570
			 0x8b7e4686, /// 0x574
			 0xee2ee3b5, /// 0x578
			 0xca08213e, /// 0x57c
			 0xb38741af, /// 0x580
			 0x72fc02f2, /// 0x584
			 0xfdccec87, /// 0x588
			 0x264f9999, /// 0x58c
			 0xce981429, /// 0x590
			 0xad4aeccf, /// 0x594
			 0x0e6c5e86, /// 0x598
			 0xad229d54, /// 0x59c
			 0x898c6866, /// 0x5a0
			 0x30f8d067, /// 0x5a4
			 0x1215b80c, /// 0x5a8
			 0x7161f3ab, /// 0x5ac
			 0x792e5e97, /// 0x5b0
			 0xcbb08eb8, /// 0x5b4
			 0x9c1427ab, /// 0x5b8
			 0x2e18573e, /// 0x5bc
			 0xd5064c14, /// 0x5c0
			 0x5765d082, /// 0x5c4
			 0x0222638d, /// 0x5c8
			 0x38cad716, /// 0x5cc
			 0x264465c1, /// 0x5d0
			 0x0c986dd9, /// 0x5d4
			 0x308af0ab, /// 0x5d8
			 0xc2dfd7d6, /// 0x5dc
			 0xcb4fb98a, /// 0x5e0
			 0x7ff224a0, /// 0x5e4
			 0xf9e560ee, /// 0x5e8
			 0x40431073, /// 0x5ec
			 0x75414dee, /// 0x5f0
			 0xcd2fc00c, /// 0x5f4
			 0x098b4ac8, /// 0x5f8
			 0xf443a35b, /// 0x5fc
			 0xac341f5c, /// 0x600
			 0xe421b8ca, /// 0x604
			 0x13ec9b66, /// 0x608
			 0x9100b067, /// 0x60c
			 0xa6b0ceb4, /// 0x610
			 0x2b93039c, /// 0x614
			 0xa71d6dfe, /// 0x618
			 0xae02fbbe, /// 0x61c
			 0x88c64027, /// 0x620
			 0x3a592703, /// 0x624
			 0xea64ff65, /// 0x628
			 0x147cb6fb, /// 0x62c
			 0x27ed4698, /// 0x630
			 0xd268a963, /// 0x634
			 0x4e9d8bb2, /// 0x638
			 0x60cccba7, /// 0x63c
			 0xd46b8e9e, /// 0x640
			 0xb74c3d62, /// 0x644
			 0x773d6add, /// 0x648
			 0x07ccb11d, /// 0x64c
			 0xfcbf9a17, /// 0x650
			 0xc39153ae, /// 0x654
			 0xde1ded2a, /// 0x658
			 0xe39124ea, /// 0x65c
			 0x047468ce, /// 0x660
			 0xffe9bfda, /// 0x664
			 0xa55b5230, /// 0x668
			 0x597a283e, /// 0x66c
			 0x8a31da38, /// 0x670
			 0xbfbc571b, /// 0x674
			 0x4ddfbde7, /// 0x678
			 0x1708ee06, /// 0x67c
			 0x71021e44, /// 0x680
			 0x15b62ee5, /// 0x684
			 0xf058e576, /// 0x688
			 0x262419b9, /// 0x68c
			 0x241a5c64, /// 0x690
			 0x31180910, /// 0x694
			 0x44e7f6cb, /// 0x698
			 0xadc542bb, /// 0x69c
			 0x2d38f5a2, /// 0x6a0
			 0x812e1214, /// 0x6a4
			 0x1de17a49, /// 0x6a8
			 0x698597b5, /// 0x6ac
			 0x8313d77d, /// 0x6b0
			 0x807ef9ff, /// 0x6b4
			 0xb0c0676a, /// 0x6b8
			 0x179213bf, /// 0x6bc
			 0x3159f480, /// 0x6c0
			 0x3fe6b9b8, /// 0x6c4
			 0xf9cd9417, /// 0x6c8
			 0x17f307f7, /// 0x6cc
			 0xcc67e01b, /// 0x6d0
			 0xe88204aa, /// 0x6d4
			 0xc5e3e9d3, /// 0x6d8
			 0x20813a40, /// 0x6dc
			 0x66b190be, /// 0x6e0
			 0x361193fa, /// 0x6e4
			 0xbdea3724, /// 0x6e8
			 0x0ddd8cf9, /// 0x6ec
			 0xbe4653d7, /// 0x6f0
			 0x8fdccdd0, /// 0x6f4
			 0x1dc31c0a, /// 0x6f8
			 0x4f297f90, /// 0x6fc
			 0xa917a176, /// 0x700
			 0x41c07061, /// 0x704
			 0x0aa482d9, /// 0x708
			 0x46a35692, /// 0x70c
			 0x9236e7ba, /// 0x710
			 0x7b3c88e6, /// 0x714
			 0xee76e937, /// 0x718
			 0x402cfd26, /// 0x71c
			 0x65e89868, /// 0x720
			 0xe673a407, /// 0x724
			 0x3cca0fa4, /// 0x728
			 0x77440eb8, /// 0x72c
			 0x480ee11b, /// 0x730
			 0x7b10bd78, /// 0x734
			 0xb1163cbb, /// 0x738
			 0xe24ab748, /// 0x73c
			 0x37c181b1, /// 0x740
			 0xa1d10777, /// 0x744
			 0x4d274f06, /// 0x748
			 0x38d24d75, /// 0x74c
			 0x45f72366, /// 0x750
			 0x36292393, /// 0x754
			 0x08c45e28, /// 0x758
			 0x7f81bfa2, /// 0x75c
			 0xbad7ac4d, /// 0x760
			 0xb26315ee, /// 0x764
			 0x41bd272e, /// 0x768
			 0x398e552a, /// 0x76c
			 0x7f2ab1fe, /// 0x770
			 0x08a20176, /// 0x774
			 0x9d37a4e5, /// 0x778
			 0xaffa2e1e, /// 0x77c
			 0x28451995, /// 0x780
			 0x37daf098, /// 0x784
			 0x130c8be5, /// 0x788
			 0x59be114a, /// 0x78c
			 0x8a75e3d8, /// 0x790
			 0x531188a3, /// 0x794
			 0x9370fdab, /// 0x798
			 0xe8ed3cb1, /// 0x79c
			 0x38fcced2, /// 0x7a0
			 0x154f2310, /// 0x7a4
			 0x03a83126, /// 0x7a8
			 0x96a1a260, /// 0x7ac
			 0xa5c2e486, /// 0x7b0
			 0xb8fd8044, /// 0x7b4
			 0xdc0c15d6, /// 0x7b8
			 0x8f25b5fe, /// 0x7bc
			 0xc4fc3075, /// 0x7c0
			 0xa14de152, /// 0x7c4
			 0x5be82457, /// 0x7c8
			 0x42ee99bf, /// 0x7cc
			 0x623c2add, /// 0x7d0
			 0xa583e80d, /// 0x7d4
			 0x97be9f60, /// 0x7d8
			 0x56d4e054, /// 0x7dc
			 0x7277f7e4, /// 0x7e0
			 0x45b77e62, /// 0x7e4
			 0xd82600b3, /// 0x7e8
			 0xd3a8bc0b, /// 0x7ec
			 0x2e21189e, /// 0x7f0
			 0x6dd2d15a, /// 0x7f4
			 0xf37403ae, /// 0x7f8
			 0x00f83805, /// 0x7fc
			 0x5824de29, /// 0x800
			 0xd24444ed, /// 0x804
			 0x0447a5e1, /// 0x808
			 0x1015a273, /// 0x80c
			 0x170f07bf, /// 0x810
			 0xc721ddaa, /// 0x814
			 0xc971294f, /// 0x818
			 0x44de1b7d, /// 0x81c
			 0xba3f5d68, /// 0x820
			 0x7b3bd5de, /// 0x824
			 0x5e93e3d2, /// 0x828
			 0xe9facf94, /// 0x82c
			 0x35cca4e9, /// 0x830
			 0x8cbee1e0, /// 0x834
			 0xb2eadacb, /// 0x838
			 0xf693caee, /// 0x83c
			 0x62024888, /// 0x840
			 0x4016bb2f, /// 0x844
			 0x2f5ceaa7, /// 0x848
			 0x62e26c47, /// 0x84c
			 0x86b9e7b8, /// 0x850
			 0x92a2ab32, /// 0x854
			 0x8a438c57, /// 0x858
			 0x00a9b5f0, /// 0x85c
			 0x5d305ac0, /// 0x860
			 0x88b93577, /// 0x864
			 0x5b310b7b, /// 0x868
			 0x5367e0da, /// 0x86c
			 0xfc6550f5, /// 0x870
			 0x0b756be9, /// 0x874
			 0x696ba860, /// 0x878
			 0xb95fac76, /// 0x87c
			 0x8ead88b7, /// 0x880
			 0x5f418a04, /// 0x884
			 0x7b7e749e, /// 0x888
			 0xea8fc52f, /// 0x88c
			 0xbdd2abc4, /// 0x890
			 0x3415c97e, /// 0x894
			 0x1f5699e2, /// 0x898
			 0x304c5e57, /// 0x89c
			 0x5ab0d17d, /// 0x8a0
			 0xe2eef6fb, /// 0x8a4
			 0xe1d4ecb1, /// 0x8a8
			 0xe58e9aba, /// 0x8ac
			 0x3ad43c7f, /// 0x8b0
			 0xc98044f9, /// 0x8b4
			 0x40adae73, /// 0x8b8
			 0xbfe336b8, /// 0x8bc
			 0xc6955643, /// 0x8c0
			 0xc87ec2fd, /// 0x8c4
			 0x8e4180f3, /// 0x8c8
			 0x42b2d748, /// 0x8cc
			 0x4b41e791, /// 0x8d0
			 0xd918b08f, /// 0x8d4
			 0x60c4e871, /// 0x8d8
			 0xfb7fddbb, /// 0x8dc
			 0x3c106219, /// 0x8e0
			 0xcc4f8523, /// 0x8e4
			 0x9351db2d, /// 0x8e8
			 0xc674a360, /// 0x8ec
			 0xb0b87203, /// 0x8f0
			 0xe3664f06, /// 0x8f4
			 0x035d3627, /// 0x8f8
			 0x1c302df2, /// 0x8fc
			 0x6e7c26bd, /// 0x900
			 0xc6a3edd7, /// 0x904
			 0xd34439de, /// 0x908
			 0xe68b10b3, /// 0x90c
			 0x73435654, /// 0x910
			 0x873dde58, /// 0x914
			 0x6bfa2e11, /// 0x918
			 0x59dc2613, /// 0x91c
			 0x5702a6fe, /// 0x920
			 0x043e83ec, /// 0x924
			 0x34ad33ac, /// 0x928
			 0xab377609, /// 0x92c
			 0x3ca0741e, /// 0x930
			 0xb47574a2, /// 0x934
			 0xecae5209, /// 0x938
			 0x7525265a, /// 0x93c
			 0x60ff5061, /// 0x940
			 0xf82b4d9e, /// 0x944
			 0x99e814d0, /// 0x948
			 0x1033e58d, /// 0x94c
			 0xea8df62d, /// 0x950
			 0x2c8cd4ac, /// 0x954
			 0x06356d9e, /// 0x958
			 0xeef88283, /// 0x95c
			 0x3f266109, /// 0x960
			 0x0edd1ff2, /// 0x964
			 0x57a46f47, /// 0x968
			 0xcf37d340, /// 0x96c
			 0xf4dce054, /// 0x970
			 0x88324430, /// 0x974
			 0xd641e1c2, /// 0x978
			 0x5e2905b6, /// 0x97c
			 0xec1466fb, /// 0x980
			 0x947e9973, /// 0x984
			 0x70226d5e, /// 0x988
			 0x8d631ca4, /// 0x98c
			 0xc8982d74, /// 0x990
			 0xab470fa5, /// 0x994
			 0xf62fccf5, /// 0x998
			 0x0f4c7cc3, /// 0x99c
			 0xd8fa4b9d, /// 0x9a0
			 0x2de78a00, /// 0x9a4
			 0xeebdcb8e, /// 0x9a8
			 0x2dc1a104, /// 0x9ac
			 0x2ef95f9f, /// 0x9b0
			 0x9acf3c7c, /// 0x9b4
			 0xe200f1fa, /// 0x9b8
			 0x7c8da495, /// 0x9bc
			 0x94e3fb59, /// 0x9c0
			 0x8cec8119, /// 0x9c4
			 0x8b33bded, /// 0x9c8
			 0x73e9a82b, /// 0x9cc
			 0x9358fbc7, /// 0x9d0
			 0x85e41212, /// 0x9d4
			 0xd56570d7, /// 0x9d8
			 0xb986baff, /// 0x9dc
			 0x5eeab282, /// 0x9e0
			 0x987acf84, /// 0x9e4
			 0xdaa348b0, /// 0x9e8
			 0x36a88619, /// 0x9ec
			 0x66bb3ed1, /// 0x9f0
			 0x471bd28a, /// 0x9f4
			 0xf1eb59a3, /// 0x9f8
			 0xf0c7831c, /// 0x9fc
			 0x2f83e693, /// 0xa00
			 0xe7907554, /// 0xa04
			 0x81601600, /// 0xa08
			 0x0c3f2840, /// 0xa0c
			 0x878bdebc, /// 0xa10
			 0x09d02187, /// 0xa14
			 0xa403f08a, /// 0xa18
			 0x19de71e7, /// 0xa1c
			 0x37405de7, /// 0xa20
			 0xecbd7146, /// 0xa24
			 0x05d6a339, /// 0xa28
			 0xc9b5b0ad, /// 0xa2c
			 0xd69646bc, /// 0xa30
			 0x79c6ae3e, /// 0xa34
			 0x15a5e9a7, /// 0xa38
			 0x2316b8bc, /// 0xa3c
			 0x1df4faad, /// 0xa40
			 0x4340e32f, /// 0xa44
			 0xb6dbfd02, /// 0xa48
			 0x01e0c4e8, /// 0xa4c
			 0x350891ce, /// 0xa50
			 0x2c76b265, /// 0xa54
			 0x760190c5, /// 0xa58
			 0xab3c07f5, /// 0xa5c
			 0x046b1066, /// 0xa60
			 0x689c429b, /// 0xa64
			 0xef9e7d21, /// 0xa68
			 0x035afe10, /// 0xa6c
			 0x4aad0383, /// 0xa70
			 0x0eeddf1c, /// 0xa74
			 0x4a450b04, /// 0xa78
			 0x677bbbe4, /// 0xa7c
			 0x775eb7bf, /// 0xa80
			 0x5e153d97, /// 0xa84
			 0x987ba3ec, /// 0xa88
			 0xa1a458b7, /// 0xa8c
			 0x327d9f4f, /// 0xa90
			 0x5317f172, /// 0xa94
			 0xb07199dc, /// 0xa98
			 0x0c9ee9e4, /// 0xa9c
			 0x1b1a4a3b, /// 0xaa0
			 0x7327dcca, /// 0xaa4
			 0x94959641, /// 0xaa8
			 0x7044bcc7, /// 0xaac
			 0xa6b49f62, /// 0xab0
			 0xf54f8325, /// 0xab4
			 0xd2961070, /// 0xab8
			 0x9a414b99, /// 0xabc
			 0xf1890601, /// 0xac0
			 0x2f65473e, /// 0xac4
			 0xb82344f0, /// 0xac8
			 0x83a59d2d, /// 0xacc
			 0xa64e951c, /// 0xad0
			 0x75fa8a67, /// 0xad4
			 0x2b40b47e, /// 0xad8
			 0xb2c8dba9, /// 0xadc
			 0x2b58e9da, /// 0xae0
			 0x07bf1e16, /// 0xae4
			 0x615e36d1, /// 0xae8
			 0x096f91f3, /// 0xaec
			 0xcf7ded69, /// 0xaf0
			 0x44f11d30, /// 0xaf4
			 0x9637e385, /// 0xaf8
			 0x7f5a745d, /// 0xafc
			 0x3525bb77, /// 0xb00
			 0x8a0ee598, /// 0xb04
			 0x2e111187, /// 0xb08
			 0x33dd0a25, /// 0xb0c
			 0x9ad9c0f3, /// 0xb10
			 0x2d337fee, /// 0xb14
			 0x8640966c, /// 0xb18
			 0x9f24aac8, /// 0xb1c
			 0x49008f42, /// 0xb20
			 0x39389433, /// 0xb24
			 0x22dfc2a4, /// 0xb28
			 0x8d5a46c0, /// 0xb2c
			 0x8a7d4ac7, /// 0xb30
			 0x98fbe1ae, /// 0xb34
			 0x1bd1c604, /// 0xb38
			 0x491faa8a, /// 0xb3c
			 0x394d010a, /// 0xb40
			 0x30501daf, /// 0xb44
			 0x2db3d68a, /// 0xb48
			 0xfa6e8287, /// 0xb4c
			 0x90aeb6c0, /// 0xb50
			 0xe2afc01b, /// 0xb54
			 0xd0b1229e, /// 0xb58
			 0x19daf1b8, /// 0xb5c
			 0x6e7fb0cf, /// 0xb60
			 0xebbca329, /// 0xb64
			 0x074740db, /// 0xb68
			 0x6de2c078, /// 0xb6c
			 0x761d42e8, /// 0xb70
			 0x90d572f1, /// 0xb74
			 0x2ebcb8d1, /// 0xb78
			 0xbdcfde70, /// 0xb7c
			 0xb24ac25d, /// 0xb80
			 0xd6ec7528, /// 0xb84
			 0xcd9c8c00, /// 0xb88
			 0x106d6ab5, /// 0xb8c
			 0x72c7a573, /// 0xb90
			 0x1102fac6, /// 0xb94
			 0xaf4a8645, /// 0xb98
			 0x91e59177, /// 0xb9c
			 0xa2711001, /// 0xba0
			 0x22a0ebe4, /// 0xba4
			 0x2afa42d1, /// 0xba8
			 0xa19a46d5, /// 0xbac
			 0x7cf215da, /// 0xbb0
			 0x0f7c5152, /// 0xbb4
			 0x6016a1ea, /// 0xbb8
			 0x5b990119, /// 0xbbc
			 0x57935380, /// 0xbc0
			 0xd77c76ab, /// 0xbc4
			 0xafc6c18d, /// 0xbc8
			 0xedc95186, /// 0xbcc
			 0x6af815cd, /// 0xbd0
			 0x7be59b42, /// 0xbd4
			 0xd0d1f6ea, /// 0xbd8
			 0x41a95c7e, /// 0xbdc
			 0x61515dce, /// 0xbe0
			 0xd4930e5a, /// 0xbe4
			 0xdf76c42d, /// 0xbe8
			 0xc88fffe4, /// 0xbec
			 0x39f3d5d5, /// 0xbf0
			 0xf666affa, /// 0xbf4
			 0x8957b662, /// 0xbf8
			 0x0130fb60, /// 0xbfc
			 0x0a97e64c, /// 0xc00
			 0x24f567ba, /// 0xc04
			 0xd11d9c3a, /// 0xc08
			 0x45bf9b27, /// 0xc0c
			 0x30fc8925, /// 0xc10
			 0x9ee30926, /// 0xc14
			 0x94ad7fae, /// 0xc18
			 0x708c38f2, /// 0xc1c
			 0x33c4875c, /// 0xc20
			 0xed2a7b7e, /// 0xc24
			 0x95af81fb, /// 0xc28
			 0x16e1bd04, /// 0xc2c
			 0xa8762b27, /// 0xc30
			 0xd19c0554, /// 0xc34
			 0x165f8fa9, /// 0xc38
			 0xdc5cb7f9, /// 0xc3c
			 0xd7765c61, /// 0xc40
			 0x5d8157fd, /// 0xc44
			 0x22020ab9, /// 0xc48
			 0xfbb162bc, /// 0xc4c
			 0xb217243f, /// 0xc50
			 0xace05b5b, /// 0xc54
			 0x237aba0c, /// 0xc58
			 0xcf3646ac, /// 0xc5c
			 0x90310d82, /// 0xc60
			 0xbd144f17, /// 0xc64
			 0xca35faed, /// 0xc68
			 0xa2c2f3a2, /// 0xc6c
			 0xe42a5798, /// 0xc70
			 0x34dd5f61, /// 0xc74
			 0xf5bf4eb8, /// 0xc78
			 0xbfd2a7ad, /// 0xc7c
			 0x4e6be9b8, /// 0xc80
			 0xec7b51a6, /// 0xc84
			 0x462ffe38, /// 0xc88
			 0x2119d071, /// 0xc8c
			 0xbf714d67, /// 0xc90
			 0xba5ef091, /// 0xc94
			 0x33d8a65c, /// 0xc98
			 0x7e1764e2, /// 0xc9c
			 0x76b7ad01, /// 0xca0
			 0x46537f48, /// 0xca4
			 0x67de6255, /// 0xca8
			 0x1ec7e5f0, /// 0xcac
			 0x26cb4ec5, /// 0xcb0
			 0x42955ef3, /// 0xcb4
			 0xd5d2316b, /// 0xcb8
			 0x291098e1, /// 0xcbc
			 0xefbbe2b5, /// 0xcc0
			 0x82d1fec6, /// 0xcc4
			 0x8f63f630, /// 0xcc8
			 0x0deb7324, /// 0xccc
			 0x95194c33, /// 0xcd0
			 0x04ef71c6, /// 0xcd4
			 0x6623dd93, /// 0xcd8
			 0x0a524d29, /// 0xcdc
			 0xee41e577, /// 0xce0
			 0xa63bce97, /// 0xce4
			 0x5186f20c, /// 0xce8
			 0x54f6b287, /// 0xcec
			 0xcfeced15, /// 0xcf0
			 0x021b252c, /// 0xcf4
			 0x91431855, /// 0xcf8
			 0x054ad8d8, /// 0xcfc
			 0x59d02967, /// 0xd00
			 0xc3f2c3ef, /// 0xd04
			 0xd88a08c6, /// 0xd08
			 0x2a25255f, /// 0xd0c
			 0x371c8f50, /// 0xd10
			 0xf000590a, /// 0xd14
			 0xb7f9b7e0, /// 0xd18
			 0xec547097, /// 0xd1c
			 0xb6c09240, /// 0xd20
			 0xa0d68c40, /// 0xd24
			 0xcb090594, /// 0xd28
			 0x34ae9968, /// 0xd2c
			 0xf9a756b2, /// 0xd30
			 0x31a6c4dc, /// 0xd34
			 0xf8a28c78, /// 0xd38
			 0xee19954e, /// 0xd3c
			 0x102e9c28, /// 0xd40
			 0x0722cd5b, /// 0xd44
			 0x0ad0bdc7, /// 0xd48
			 0x8bd0edb8, /// 0xd4c
			 0xfc31bc4f, /// 0xd50
			 0x47f383d1, /// 0xd54
			 0xe1924ae9, /// 0xd58
			 0x8e0370ed, /// 0xd5c
			 0x3bc66fcf, /// 0xd60
			 0x4c2bf905, /// 0xd64
			 0xe23f99aa, /// 0xd68
			 0x1caa570b, /// 0xd6c
			 0xc9e7312a, /// 0xd70
			 0x09b033bf, /// 0xd74
			 0x221d2a76, /// 0xd78
			 0xca7f0a87, /// 0xd7c
			 0x91f0708e, /// 0xd80
			 0x782d8f5e, /// 0xd84
			 0x2db71ad3, /// 0xd88
			 0x3cd1b076, /// 0xd8c
			 0xd17f79cb, /// 0xd90
			 0xa1f32dd8, /// 0xd94
			 0x376a4a51, /// 0xd98
			 0x1bfc1181, /// 0xd9c
			 0x7ed45a0b, /// 0xda0
			 0xf0abb994, /// 0xda4
			 0xe0a99009, /// 0xda8
			 0x7fe70037, /// 0xdac
			 0x2272b1e5, /// 0xdb0
			 0x34a1671e, /// 0xdb4
			 0x7ff6e8f6, /// 0xdb8
			 0x1d8a8d0e, /// 0xdbc
			 0xc0a007e6, /// 0xdc0
			 0x7209009d, /// 0xdc4
			 0x8d1375b1, /// 0xdc8
			 0x9b735763, /// 0xdcc
			 0xc85a9501, /// 0xdd0
			 0x7f3ddef3, /// 0xdd4
			 0x2a670ac7, /// 0xdd8
			 0x10358996, /// 0xddc
			 0x9b890bae, /// 0xde0
			 0x01c61b53, /// 0xde4
			 0x8ff6b349, /// 0xde8
			 0x28d2c3af, /// 0xdec
			 0x3e8ba52c, /// 0xdf0
			 0xfae5c315, /// 0xdf4
			 0x31a0c9f9, /// 0xdf8
			 0xbcfdb14d, /// 0xdfc
			 0x076d864e, /// 0xe00
			 0x2dd91af1, /// 0xe04
			 0x9025f0d0, /// 0xe08
			 0xc6de1253, /// 0xe0c
			 0x8b47f17d, /// 0xe10
			 0xfcd76d1c, /// 0xe14
			 0x41859b4a, /// 0xe18
			 0x743d6eba, /// 0xe1c
			 0x8c9eaaa0, /// 0xe20
			 0x55dba621, /// 0xe24
			 0x16b7fc96, /// 0xe28
			 0xfca29ba9, /// 0xe2c
			 0xd2f8f574, /// 0xe30
			 0xb3e47d5f, /// 0xe34
			 0xb67378b8, /// 0xe38
			 0xf6254e93, /// 0xe3c
			 0x71a16cd9, /// 0xe40
			 0x348e670a, /// 0xe44
			 0x808f4037, /// 0xe48
			 0xac8a9a81, /// 0xe4c
			 0xde2d24fa, /// 0xe50
			 0xf1215114, /// 0xe54
			 0x09cd17d1, /// 0xe58
			 0xd7d39e0d, /// 0xe5c
			 0xfe43bee7, /// 0xe60
			 0xc7890254, /// 0xe64
			 0x442d9f77, /// 0xe68
			 0x0e6ab607, /// 0xe6c
			 0x4f6a6731, /// 0xe70
			 0xb67c7dda, /// 0xe74
			 0xf591b05b, /// 0xe78
			 0xb6e25801, /// 0xe7c
			 0x84fc91f1, /// 0xe80
			 0xa4b38c41, /// 0xe84
			 0x9e30d7fc, /// 0xe88
			 0xc741a42c, /// 0xe8c
			 0x19d67a4b, /// 0xe90
			 0x60b99cf6, /// 0xe94
			 0x13a05a15, /// 0xe98
			 0x2a664382, /// 0xe9c
			 0x26ea04bc, /// 0xea0
			 0x1de487bc, /// 0xea4
			 0x7efabedc, /// 0xea8
			 0x515a99fa, /// 0xeac
			 0x054f23a4, /// 0xeb0
			 0x2c451a72, /// 0xeb4
			 0x8fa607e0, /// 0xeb8
			 0xb0c933b4, /// 0xebc
			 0x571ff692, /// 0xec0
			 0xf1b544b4, /// 0xec4
			 0xbd870d6b, /// 0xec8
			 0xb270d9ad, /// 0xecc
			 0x69bb2540, /// 0xed0
			 0xb34fb7f4, /// 0xed4
			 0x5b1e90e7, /// 0xed8
			 0x75fc9a0b, /// 0xedc
			 0xf406e4b2, /// 0xee0
			 0xb5b1f3e0, /// 0xee4
			 0xb350f1aa, /// 0xee8
			 0xe3393dbf, /// 0xeec
			 0xf50dd70f, /// 0xef0
			 0xb262ec98, /// 0xef4
			 0x4227ec5d, /// 0xef8
			 0xd9850040, /// 0xefc
			 0xedc6a8bf, /// 0xf00
			 0x95f2033d, /// 0xf04
			 0xa6cca14e, /// 0xf08
			 0xf82df46f, /// 0xf0c
			 0xb279eaa8, /// 0xf10
			 0x02f2556b, /// 0xf14
			 0x7f738fc9, /// 0xf18
			 0xa8dd353c, /// 0xf1c
			 0xff7d358c, /// 0xf20
			 0xa37079a5, /// 0xf24
			 0xde2a5919, /// 0xf28
			 0x87f94699, /// 0xf2c
			 0x1a3301ae, /// 0xf30
			 0x02c6291c, /// 0xf34
			 0x58e1ab79, /// 0xf38
			 0xd64feee6, /// 0xf3c
			 0xdfff0216, /// 0xf40
			 0x4f48f6a1, /// 0xf44
			 0x71226601, /// 0xf48
			 0xa9506299, /// 0xf4c
			 0x5660ca8b, /// 0xf50
			 0x6a6fee44, /// 0xf54
			 0x8be2325d, /// 0xf58
			 0x5601ef5f, /// 0xf5c
			 0x2f27ea40, /// 0xf60
			 0x0301b198, /// 0xf64
			 0xb6db5c77, /// 0xf68
			 0x90b07d03, /// 0xf6c
			 0x56e55b1d, /// 0xf70
			 0xc6222ccd, /// 0xf74
			 0x25d30a69, /// 0xf78
			 0x0d535279, /// 0xf7c
			 0x6ac47f3f, /// 0xf80
			 0x3e58e65f, /// 0xf84
			 0x41439674, /// 0xf88
			 0xb0a31970, /// 0xf8c
			 0xd1fe6293, /// 0xf90
			 0xad95083b, /// 0xf94
			 0xba812f76, /// 0xf98
			 0x3efec9ae, /// 0xf9c
			 0xac4f0ee5, /// 0xfa0
			 0x6292dc55, /// 0xfa4
			 0xdfc493bf, /// 0xfa8
			 0x59afc0a6, /// 0xfac
			 0x8052138d, /// 0xfb0
			 0x3e83f060, /// 0xfb4
			 0x2b04ffb3, /// 0xfb8
			 0x81f77e92, /// 0xfbc
			 0x864f2c08, /// 0xfc0
			 0xc252225e, /// 0xfc4
			 0xc7d9682c, /// 0xfc8
			 0x7037d597, /// 0xfcc
			 0xc6565249, /// 0xfd0
			 0x0706f48d, /// 0xfd4
			 0x6553b899, /// 0xfd8
			 0xddbe6acd, /// 0xfdc
			 0x87bed6c6, /// 0xfe0
			 0x9c40a38c, /// 0xfe4
			 0x0c943628, /// 0xfe8
			 0xff43d267, /// 0xfec
			 0x62e20151, /// 0xff0
			 0xc8ac7510, /// 0xff4
			 0x98e33420, /// 0xff8
			 0x3f69da09 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x92aebc85, /// 0x0
			 0xe90fd0a6, /// 0x4
			 0xb848145a, /// 0x8
			 0x21ba37cb, /// 0xc
			 0x4cf8283f, /// 0x10
			 0x516765fa, /// 0x14
			 0xea4331f8, /// 0x18
			 0xf9d3fd9b, /// 0x1c
			 0xba261846, /// 0x20
			 0x7a5e44bf, /// 0x24
			 0x742e451e, /// 0x28
			 0x80f5f133, /// 0x2c
			 0x29f1d6d6, /// 0x30
			 0x98ffd25a, /// 0x34
			 0xc631304e, /// 0x38
			 0x36eeb0ab, /// 0x3c
			 0xed2ebe62, /// 0x40
			 0x196b2d56, /// 0x44
			 0x23ca2b81, /// 0x48
			 0x88ae9273, /// 0x4c
			 0xc305e5db, /// 0x50
			 0x617d7372, /// 0x54
			 0x9c01b993, /// 0x58
			 0x62e655ac, /// 0x5c
			 0x4a75ae05, /// 0x60
			 0x375525a8, /// 0x64
			 0x70b2be97, /// 0x68
			 0xf37c5fbd, /// 0x6c
			 0xa9cd66c8, /// 0x70
			 0x83621e5f, /// 0x74
			 0x10352dae, /// 0x78
			 0xc4fc4612, /// 0x7c
			 0x71aff5e4, /// 0x80
			 0x1e79ea97, /// 0x84
			 0x2a3065d7, /// 0x88
			 0xc2797121, /// 0x8c
			 0x693cb3fb, /// 0x90
			 0x99f45987, /// 0x94
			 0x7286a7f5, /// 0x98
			 0x8e3e92e0, /// 0x9c
			 0x15792792, /// 0xa0
			 0xa4ad1a10, /// 0xa4
			 0xbdca2b9c, /// 0xa8
			 0x21d58c2b, /// 0xac
			 0x87ee17bd, /// 0xb0
			 0x984b59a9, /// 0xb4
			 0x090d7b7b, /// 0xb8
			 0xbef495f8, /// 0xbc
			 0x4c925fdd, /// 0xc0
			 0xdc81b25e, /// 0xc4
			 0xd54d4bb0, /// 0xc8
			 0xa26895f5, /// 0xcc
			 0x8ccd0a13, /// 0xd0
			 0x25e5ceb4, /// 0xd4
			 0x117a6b48, /// 0xd8
			 0xa407ff0c, /// 0xdc
			 0x800054d5, /// 0xe0
			 0x7f8196df, /// 0xe4
			 0x16c42ca2, /// 0xe8
			 0xb7d97405, /// 0xec
			 0x9c6bee41, /// 0xf0
			 0x21c40d91, /// 0xf4
			 0x30085025, /// 0xf8
			 0xed244e78, /// 0xfc
			 0x5cbd7edd, /// 0x100
			 0x237c2ae6, /// 0x104
			 0x90aa0429, /// 0x108
			 0x5d0852d7, /// 0x10c
			 0x57e99b0f, /// 0x110
			 0x6a1650a1, /// 0x114
			 0x868743c0, /// 0x118
			 0xa36ee28e, /// 0x11c
			 0x124f0c46, /// 0x120
			 0xb8a48330, /// 0x124
			 0x2772f705, /// 0x128
			 0xad89def9, /// 0x12c
			 0x070de299, /// 0x130
			 0xd6c9be59, /// 0x134
			 0x4171e9ca, /// 0x138
			 0xbb393a31, /// 0x13c
			 0xc5f61627, /// 0x140
			 0x2e9a1795, /// 0x144
			 0x0cfcbaa3, /// 0x148
			 0xc24c92e0, /// 0x14c
			 0x57c10340, /// 0x150
			 0xe8885a46, /// 0x154
			 0x774b5044, /// 0x158
			 0x5e148d5d, /// 0x15c
			 0xa98f2e04, /// 0x160
			 0x2ab735e7, /// 0x164
			 0x562b97f8, /// 0x168
			 0x8385eb62, /// 0x16c
			 0x2230013c, /// 0x170
			 0xd13a2c9f, /// 0x174
			 0xf0cc63bb, /// 0x178
			 0xb6f62098, /// 0x17c
			 0x3e1cc762, /// 0x180
			 0x7725df54, /// 0x184
			 0x2206d3bd, /// 0x188
			 0x6f0322ff, /// 0x18c
			 0xa343da8e, /// 0x190
			 0xd63d0a1a, /// 0x194
			 0xa4aa86e6, /// 0x198
			 0xae9c70ad, /// 0x19c
			 0xc9be9d59, /// 0x1a0
			 0xcf6f64fa, /// 0x1a4
			 0xbe21cec2, /// 0x1a8
			 0xbba49ed9, /// 0x1ac
			 0x92f6d56b, /// 0x1b0
			 0x9c8b63a0, /// 0x1b4
			 0x082e45e5, /// 0x1b8
			 0x0fece357, /// 0x1bc
			 0x11a70176, /// 0x1c0
			 0x397fd2c1, /// 0x1c4
			 0xca19d8a3, /// 0x1c8
			 0x4ee1b1d6, /// 0x1cc
			 0x3be84990, /// 0x1d0
			 0xfe9d95fa, /// 0x1d4
			 0xc7184d08, /// 0x1d8
			 0x04d29d13, /// 0x1dc
			 0x70af888d, /// 0x1e0
			 0xe9ad4978, /// 0x1e4
			 0x73085e01, /// 0x1e8
			 0x5a9b2c4a, /// 0x1ec
			 0x0e123812, /// 0x1f0
			 0xa5b715a3, /// 0x1f4
			 0x24487e25, /// 0x1f8
			 0x9dc8cfe1, /// 0x1fc
			 0x8737a1a1, /// 0x200
			 0xcd4a8b2b, /// 0x204
			 0x4a273130, /// 0x208
			 0xbfb74066, /// 0x20c
			 0xc82542a3, /// 0x210
			 0x257a6941, /// 0x214
			 0x53a4e598, /// 0x218
			 0x74e241b4, /// 0x21c
			 0x51db1111, /// 0x220
			 0x9b771533, /// 0x224
			 0xb98fdd2b, /// 0x228
			 0x210c51d6, /// 0x22c
			 0xf680f313, /// 0x230
			 0x2659c810, /// 0x234
			 0xa4ea6ba9, /// 0x238
			 0x91a768ef, /// 0x23c
			 0xe5230b94, /// 0x240
			 0xa7afdf29, /// 0x244
			 0x1a52193d, /// 0x248
			 0x2aafbd5b, /// 0x24c
			 0x12741346, /// 0x250
			 0x2a1b3e18, /// 0x254
			 0x15b7daec, /// 0x258
			 0x87a5aec1, /// 0x25c
			 0xa740382a, /// 0x260
			 0x923bbd33, /// 0x264
			 0x7d9b5080, /// 0x268
			 0xe437cc52, /// 0x26c
			 0xc2015e95, /// 0x270
			 0xe597f70a, /// 0x274
			 0x545158e6, /// 0x278
			 0xd39c1177, /// 0x27c
			 0xc26db97b, /// 0x280
			 0xd4593c52, /// 0x284
			 0xb593e372, /// 0x288
			 0x86214676, /// 0x28c
			 0x40b9052b, /// 0x290
			 0xdf60d39a, /// 0x294
			 0x4c5c351c, /// 0x298
			 0xea0d5815, /// 0x29c
			 0x9adc59e0, /// 0x2a0
			 0xdc0f22ae, /// 0x2a4
			 0xdc64d391, /// 0x2a8
			 0x9893602d, /// 0x2ac
			 0xef539364, /// 0x2b0
			 0xb25aef76, /// 0x2b4
			 0xd681c0b0, /// 0x2b8
			 0x309e2fd9, /// 0x2bc
			 0x686a5359, /// 0x2c0
			 0x4d054e37, /// 0x2c4
			 0xd36602f7, /// 0x2c8
			 0xeee289a5, /// 0x2cc
			 0x8c79d047, /// 0x2d0
			 0x5711de97, /// 0x2d4
			 0x9003c384, /// 0x2d8
			 0xcbe8e2c1, /// 0x2dc
			 0xe889348a, /// 0x2e0
			 0x97229820, /// 0x2e4
			 0x9cc01ecf, /// 0x2e8
			 0x29a0a211, /// 0x2ec
			 0x8b1eb0c0, /// 0x2f0
			 0x56ad3d6f, /// 0x2f4
			 0xee68991c, /// 0x2f8
			 0x2431e836, /// 0x2fc
			 0x65fadbd1, /// 0x300
			 0x2432bc00, /// 0x304
			 0x6d1b379a, /// 0x308
			 0xdffa21d7, /// 0x30c
			 0x9988649e, /// 0x310
			 0x1b4e0486, /// 0x314
			 0xb00d2c5c, /// 0x318
			 0xcb8fbee6, /// 0x31c
			 0x600d4c39, /// 0x320
			 0xe9b46402, /// 0x324
			 0x90fa8033, /// 0x328
			 0x51115670, /// 0x32c
			 0xddbc0f1b, /// 0x330
			 0xfe285ae6, /// 0x334
			 0xfc0b899a, /// 0x338
			 0xba2c0bf9, /// 0x33c
			 0x7683651b, /// 0x340
			 0xbc44af63, /// 0x344
			 0x19b30421, /// 0x348
			 0x0721ea5e, /// 0x34c
			 0x9540758f, /// 0x350
			 0xb7343194, /// 0x354
			 0x2f45aff9, /// 0x358
			 0xd6019748, /// 0x35c
			 0x236ea6ca, /// 0x360
			 0xebf46a39, /// 0x364
			 0xd07a3d4c, /// 0x368
			 0x67ee8a8e, /// 0x36c
			 0x55c32a34, /// 0x370
			 0x3dfdbd14, /// 0x374
			 0xde2106a0, /// 0x378
			 0x7bee78f5, /// 0x37c
			 0xad5bf98a, /// 0x380
			 0x76bb5fae, /// 0x384
			 0x9fffbcea, /// 0x388
			 0xb865cf40, /// 0x38c
			 0xd2fdac42, /// 0x390
			 0xa344224a, /// 0x394
			 0x4cd4f1ee, /// 0x398
			 0xfa53e9d5, /// 0x39c
			 0x8443594f, /// 0x3a0
			 0x67951050, /// 0x3a4
			 0xac82d10e, /// 0x3a8
			 0xc77d9c9e, /// 0x3ac
			 0xf181a945, /// 0x3b0
			 0xfd9bb679, /// 0x3b4
			 0x85658e73, /// 0x3b8
			 0x18d015cf, /// 0x3bc
			 0x65a87cc9, /// 0x3c0
			 0xc6f0667a, /// 0x3c4
			 0xa90c290f, /// 0x3c8
			 0x183c1682, /// 0x3cc
			 0x113c096b, /// 0x3d0
			 0xcb4aa0d0, /// 0x3d4
			 0x6cf9af88, /// 0x3d8
			 0x9e23e542, /// 0x3dc
			 0x268a6c38, /// 0x3e0
			 0xa8fd5688, /// 0x3e4
			 0x970eef76, /// 0x3e8
			 0xd9056527, /// 0x3ec
			 0xc2228137, /// 0x3f0
			 0x5b233128, /// 0x3f4
			 0x33c61223, /// 0x3f8
			 0x25875770, /// 0x3fc
			 0x7f324604, /// 0x400
			 0x967848a3, /// 0x404
			 0x2f6d1512, /// 0x408
			 0xcfd82642, /// 0x40c
			 0xcca1b7e3, /// 0x410
			 0xff926053, /// 0x414
			 0x2222018c, /// 0x418
			 0x66609ac7, /// 0x41c
			 0x4609b6bf, /// 0x420
			 0xa8b6c67c, /// 0x424
			 0x83a2bc58, /// 0x428
			 0x307d39eb, /// 0x42c
			 0x3babc117, /// 0x430
			 0x908bca9c, /// 0x434
			 0x6498cf5a, /// 0x438
			 0x3d2e9a16, /// 0x43c
			 0x3465c02c, /// 0x440
			 0xe064ddda, /// 0x444
			 0xacfed523, /// 0x448
			 0x5e75728e, /// 0x44c
			 0xf70ff98e, /// 0x450
			 0xb754b99c, /// 0x454
			 0xc02efdab, /// 0x458
			 0x39d491e9, /// 0x45c
			 0x2da823f1, /// 0x460
			 0xfdc08494, /// 0x464
			 0x4727a0f4, /// 0x468
			 0xf2c43094, /// 0x46c
			 0x3d54a85d, /// 0x470
			 0xe4b81f83, /// 0x474
			 0xbc6425c9, /// 0x478
			 0x272e7d34, /// 0x47c
			 0x35492867, /// 0x480
			 0x77d68ff2, /// 0x484
			 0xe518a9ea, /// 0x488
			 0x8c5d99e0, /// 0x48c
			 0x74fd6419, /// 0x490
			 0xe0a37720, /// 0x494
			 0xa4645058, /// 0x498
			 0xc126d589, /// 0x49c
			 0xdfe35d8c, /// 0x4a0
			 0x97da8f91, /// 0x4a4
			 0x6815b387, /// 0x4a8
			 0x0d568928, /// 0x4ac
			 0x86943a37, /// 0x4b0
			 0xa6e9dd9c, /// 0x4b4
			 0x9fa77487, /// 0x4b8
			 0x99043a6e, /// 0x4bc
			 0xa690fe2c, /// 0x4c0
			 0x7d3d5e30, /// 0x4c4
			 0x5e10437f, /// 0x4c8
			 0xaafb3879, /// 0x4cc
			 0x983d2346, /// 0x4d0
			 0xd3c7fbab, /// 0x4d4
			 0xdeec5b48, /// 0x4d8
			 0xb096aa32, /// 0x4dc
			 0xf995dc4b, /// 0x4e0
			 0xf2c565dd, /// 0x4e4
			 0x47a18d61, /// 0x4e8
			 0xa7b1fe21, /// 0x4ec
			 0x40bf6e4d, /// 0x4f0
			 0x410606bc, /// 0x4f4
			 0x20adbc24, /// 0x4f8
			 0xeb29d9d6, /// 0x4fc
			 0x95505d93, /// 0x500
			 0xf30feb90, /// 0x504
			 0x47481c5a, /// 0x508
			 0xcac947e9, /// 0x50c
			 0xe7662525, /// 0x510
			 0x992a0d49, /// 0x514
			 0x2e3fa0bc, /// 0x518
			 0xf8b8c745, /// 0x51c
			 0xf800ed29, /// 0x520
			 0x96bc32df, /// 0x524
			 0x05f33b3b, /// 0x528
			 0x3026564d, /// 0x52c
			 0x5402f7a2, /// 0x530
			 0x0cb350d7, /// 0x534
			 0x43d42e9a, /// 0x538
			 0xb63d8696, /// 0x53c
			 0x8d72874b, /// 0x540
			 0x1f4b202d, /// 0x544
			 0x5c0d74fe, /// 0x548
			 0x53eec0af, /// 0x54c
			 0xa7893164, /// 0x550
			 0x8a64ed1d, /// 0x554
			 0x525546d4, /// 0x558
			 0xe5f74cc2, /// 0x55c
			 0x22faeab2, /// 0x560
			 0xd9db6f45, /// 0x564
			 0x8579aa33, /// 0x568
			 0x7102f2f8, /// 0x56c
			 0xece11cb7, /// 0x570
			 0x60d975c6, /// 0x574
			 0x302ca30d, /// 0x578
			 0xafdb20fb, /// 0x57c
			 0x531f612a, /// 0x580
			 0xb98945b9, /// 0x584
			 0x496cad8f, /// 0x588
			 0xa9f04ec7, /// 0x58c
			 0xa9d30f1d, /// 0x590
			 0xc741dec1, /// 0x594
			 0xd090ad9a, /// 0x598
			 0xfef8a882, /// 0x59c
			 0xd4b4b7a1, /// 0x5a0
			 0xb70f8e27, /// 0x5a4
			 0xb553c05d, /// 0x5a8
			 0x61bc2f4f, /// 0x5ac
			 0x0121fbc3, /// 0x5b0
			 0xd3f6d021, /// 0x5b4
			 0x926ca95b, /// 0x5b8
			 0x95703add, /// 0x5bc
			 0x565176b7, /// 0x5c0
			 0xca8278fa, /// 0x5c4
			 0x4b750d19, /// 0x5c8
			 0xd91ac346, /// 0x5cc
			 0xad76cc1e, /// 0x5d0
			 0xb877106a, /// 0x5d4
			 0x55dc4d6f, /// 0x5d8
			 0xcc1c0ec5, /// 0x5dc
			 0xc7b3f829, /// 0x5e0
			 0x8b9a9c27, /// 0x5e4
			 0x41b81b0d, /// 0x5e8
			 0x24e9c5b6, /// 0x5ec
			 0x8f44947c, /// 0x5f0
			 0x97ec43c1, /// 0x5f4
			 0xe0b19ec0, /// 0x5f8
			 0x0691c50a, /// 0x5fc
			 0x1434cd0f, /// 0x600
			 0xf29bbcee, /// 0x604
			 0xc415960a, /// 0x608
			 0x8b4a41e2, /// 0x60c
			 0xe0ea0b77, /// 0x610
			 0x2e4e5cc5, /// 0x614
			 0xf3474872, /// 0x618
			 0x89690a3d, /// 0x61c
			 0x22b9d2cf, /// 0x620
			 0xc04b908a, /// 0x624
			 0x84b2f191, /// 0x628
			 0x5e12eff4, /// 0x62c
			 0xac2affa7, /// 0x630
			 0x7529d5a1, /// 0x634
			 0xa1022a40, /// 0x638
			 0xd0f8f12f, /// 0x63c
			 0x736a019b, /// 0x640
			 0x46942069, /// 0x644
			 0x28ebf0d5, /// 0x648
			 0x577c2698, /// 0x64c
			 0x06ebbc85, /// 0x650
			 0x450b9f4f, /// 0x654
			 0x63158856, /// 0x658
			 0x82028843, /// 0x65c
			 0x5f8db251, /// 0x660
			 0x3be20944, /// 0x664
			 0x3ffa60c7, /// 0x668
			 0xa394a55f, /// 0x66c
			 0x4cadeec3, /// 0x670
			 0x7eb771dc, /// 0x674
			 0xd83d2a17, /// 0x678
			 0x94bc2c03, /// 0x67c
			 0x4595eee0, /// 0x680
			 0xad099a07, /// 0x684
			 0x30b8de17, /// 0x688
			 0xb7519581, /// 0x68c
			 0x282676ab, /// 0x690
			 0x9cc01293, /// 0x694
			 0x5d63e68b, /// 0x698
			 0x65ef3067, /// 0x69c
			 0x067f5b03, /// 0x6a0
			 0xb1bf3714, /// 0x6a4
			 0x47bb0d64, /// 0x6a8
			 0xddcca949, /// 0x6ac
			 0x3e264271, /// 0x6b0
			 0xaf4405ad, /// 0x6b4
			 0xe8bf9c0f, /// 0x6b8
			 0xc883c96e, /// 0x6bc
			 0x4d5f8fae, /// 0x6c0
			 0x616dc982, /// 0x6c4
			 0x7dab4885, /// 0x6c8
			 0x254ec214, /// 0x6cc
			 0xb11400a6, /// 0x6d0
			 0x29ba2ff2, /// 0x6d4
			 0x6d0f1417, /// 0x6d8
			 0xf269faa6, /// 0x6dc
			 0xa4957f3e, /// 0x6e0
			 0x36fe8e40, /// 0x6e4
			 0xcce13c88, /// 0x6e8
			 0x52cfe258, /// 0x6ec
			 0x60528d7c, /// 0x6f0
			 0x0e7042db, /// 0x6f4
			 0x48c66398, /// 0x6f8
			 0xb833a876, /// 0x6fc
			 0x68d9e3d2, /// 0x700
			 0x1cdbcadb, /// 0x704
			 0x482fc8b2, /// 0x708
			 0xc56db362, /// 0x70c
			 0x04e5053d, /// 0x710
			 0x545958e4, /// 0x714
			 0xaea3f497, /// 0x718
			 0x3e0810df, /// 0x71c
			 0x91553a2f, /// 0x720
			 0x1dd2bffe, /// 0x724
			 0xb3e7f443, /// 0x728
			 0x119a1c0e, /// 0x72c
			 0xda6cfdee, /// 0x730
			 0x0df15b92, /// 0x734
			 0xb263a515, /// 0x738
			 0x07e95c41, /// 0x73c
			 0xe7781f04, /// 0x740
			 0x2696dba8, /// 0x744
			 0xef22cab9, /// 0x748
			 0x6ea8281b, /// 0x74c
			 0x43bb8acf, /// 0x750
			 0xc739cb2c, /// 0x754
			 0x634bbb5f, /// 0x758
			 0x6981a42b, /// 0x75c
			 0xbcf4e1a0, /// 0x760
			 0xc64d8ba9, /// 0x764
			 0xfc8aeb30, /// 0x768
			 0x2f1b19db, /// 0x76c
			 0x49fef0de, /// 0x770
			 0x9bcb36b7, /// 0x774
			 0x20d464d1, /// 0x778
			 0x75ab947d, /// 0x77c
			 0x638a41ae, /// 0x780
			 0xf623db46, /// 0x784
			 0xa9c8146b, /// 0x788
			 0xc8205c6e, /// 0x78c
			 0xce7efb60, /// 0x790
			 0xf649d511, /// 0x794
			 0xd032f396, /// 0x798
			 0xd4dcac04, /// 0x79c
			 0x97a6b86c, /// 0x7a0
			 0xf558905d, /// 0x7a4
			 0xc108d3f8, /// 0x7a8
			 0x125984ed, /// 0x7ac
			 0x791860c2, /// 0x7b0
			 0x48fe6fd0, /// 0x7b4
			 0x01d0690b, /// 0x7b8
			 0x21416639, /// 0x7bc
			 0x9cc3b39e, /// 0x7c0
			 0x9f822712, /// 0x7c4
			 0xd48f23e3, /// 0x7c8
			 0x3b3ee76f, /// 0x7cc
			 0x053ffd6b, /// 0x7d0
			 0xeeb592e7, /// 0x7d4
			 0x7156cf66, /// 0x7d8
			 0x38ad3947, /// 0x7dc
			 0x16894717, /// 0x7e0
			 0x5b4eed3d, /// 0x7e4
			 0x5ec081cf, /// 0x7e8
			 0xfd4c4673, /// 0x7ec
			 0x4284e902, /// 0x7f0
			 0x927ce099, /// 0x7f4
			 0xc2c2153d, /// 0x7f8
			 0x1265368b, /// 0x7fc
			 0x5e775b53, /// 0x800
			 0xb74654b7, /// 0x804
			 0xd24c78d7, /// 0x808
			 0x6f23b3c9, /// 0x80c
			 0x1e068cdc, /// 0x810
			 0xfe9c306d, /// 0x814
			 0x393de212, /// 0x818
			 0xe8de5ccd, /// 0x81c
			 0x940f4d6f, /// 0x820
			 0xc760a38f, /// 0x824
			 0x80908187, /// 0x828
			 0x05e4384e, /// 0x82c
			 0xc6f84968, /// 0x830
			 0xd3f8fea7, /// 0x834
			 0x356c340f, /// 0x838
			 0xee27b3b2, /// 0x83c
			 0x8fab7730, /// 0x840
			 0x0f2ab267, /// 0x844
			 0x6fe91c94, /// 0x848
			 0xf90f504d, /// 0x84c
			 0xd9a09342, /// 0x850
			 0x371ea6ea, /// 0x854
			 0xb8bbd975, /// 0x858
			 0xeea226cd, /// 0x85c
			 0x938bf972, /// 0x860
			 0xa205b800, /// 0x864
			 0xa560b5f6, /// 0x868
			 0xe5af2b51, /// 0x86c
			 0x33dd0304, /// 0x870
			 0xc74a1dcb, /// 0x874
			 0xfed459c7, /// 0x878
			 0x86a02dcb, /// 0x87c
			 0x286a0940, /// 0x880
			 0xc491b4b6, /// 0x884
			 0xcbd1a268, /// 0x888
			 0xcff05d0c, /// 0x88c
			 0xd36b6802, /// 0x890
			 0xeb3b8ae6, /// 0x894
			 0x1b543133, /// 0x898
			 0xd79757d5, /// 0x89c
			 0xb79343c3, /// 0x8a0
			 0x78bb32cf, /// 0x8a4
			 0x02d3d419, /// 0x8a8
			 0x72925a49, /// 0x8ac
			 0xe0dd52b2, /// 0x8b0
			 0xd5d979f4, /// 0x8b4
			 0x0a8c3057, /// 0x8b8
			 0xc099cf40, /// 0x8bc
			 0x204c2989, /// 0x8c0
			 0x7f396e6e, /// 0x8c4
			 0xc5fcd229, /// 0x8c8
			 0x2ea899a3, /// 0x8cc
			 0xa0b7c8b8, /// 0x8d0
			 0xbc2c1f02, /// 0x8d4
			 0xac30a3ba, /// 0x8d8
			 0xee1f7cdc, /// 0x8dc
			 0x5795920b, /// 0x8e0
			 0xf456e96f, /// 0x8e4
			 0x2bf564ea, /// 0x8e8
			 0x837a9dc4, /// 0x8ec
			 0x648175ee, /// 0x8f0
			 0x5e048d67, /// 0x8f4
			 0x27187851, /// 0x8f8
			 0x5c4c90bd, /// 0x8fc
			 0xa9589f8e, /// 0x900
			 0xaf7bef6d, /// 0x904
			 0x78c89eb1, /// 0x908
			 0x69598b6c, /// 0x90c
			 0x064ee24f, /// 0x910
			 0x7305d705, /// 0x914
			 0x14b2abff, /// 0x918
			 0x556b0a6e, /// 0x91c
			 0xeb456a43, /// 0x920
			 0x53fd4603, /// 0x924
			 0x72ba4bdf, /// 0x928
			 0xcd52c0f7, /// 0x92c
			 0xbc361432, /// 0x930
			 0x8e8a131b, /// 0x934
			 0x85b49e91, /// 0x938
			 0x6c06ca1e, /// 0x93c
			 0x30d14dfe, /// 0x940
			 0x740f4037, /// 0x944
			 0x20fd16ce, /// 0x948
			 0x7ab32652, /// 0x94c
			 0x875c6847, /// 0x950
			 0x83480d8b, /// 0x954
			 0xc7f158a7, /// 0x958
			 0x200de705, /// 0x95c
			 0x1ccabd22, /// 0x960
			 0x9320076c, /// 0x964
			 0x072f482a, /// 0x968
			 0xcb60b6e1, /// 0x96c
			 0xaa1e5b14, /// 0x970
			 0x54fffee4, /// 0x974
			 0xcbd727c8, /// 0x978
			 0xc4f3a8ef, /// 0x97c
			 0x82df1812, /// 0x980
			 0x4162ca86, /// 0x984
			 0xa4b5fa83, /// 0x988
			 0x3697623d, /// 0x98c
			 0x4a907634, /// 0x990
			 0x2ca58964, /// 0x994
			 0x929f6bc0, /// 0x998
			 0xcb100394, /// 0x99c
			 0xd65a5c4a, /// 0x9a0
			 0x8a7bafe9, /// 0x9a4
			 0x81c411d3, /// 0x9a8
			 0x3bf154ab, /// 0x9ac
			 0x0e090e0d, /// 0x9b0
			 0x60348bad, /// 0x9b4
			 0x05ae7532, /// 0x9b8
			 0x8d5fd509, /// 0x9bc
			 0xdf566b97, /// 0x9c0
			 0x23c263bd, /// 0x9c4
			 0xacacb9d9, /// 0x9c8
			 0x171eec34, /// 0x9cc
			 0x231140b8, /// 0x9d0
			 0xe4986fa7, /// 0x9d4
			 0xd3e51184, /// 0x9d8
			 0x96f6afa4, /// 0x9dc
			 0xedbd7160, /// 0x9e0
			 0x8d2a2b74, /// 0x9e4
			 0x3a1197db, /// 0x9e8
			 0x83ec6086, /// 0x9ec
			 0x28ff0c55, /// 0x9f0
			 0xe2f56510, /// 0x9f4
			 0x1a6af010, /// 0x9f8
			 0xa1572c73, /// 0x9fc
			 0x867ec91f, /// 0xa00
			 0x7ebe30ad, /// 0xa04
			 0xa8511c27, /// 0xa08
			 0x4ed88464, /// 0xa0c
			 0x677a3b78, /// 0xa10
			 0xf156a750, /// 0xa14
			 0xf8b1ea3a, /// 0xa18
			 0x6545cb59, /// 0xa1c
			 0x1be1d506, /// 0xa20
			 0x73d81572, /// 0xa24
			 0x42b527f8, /// 0xa28
			 0x86722656, /// 0xa2c
			 0x78977315, /// 0xa30
			 0x366ec14e, /// 0xa34
			 0x6f8ec705, /// 0xa38
			 0x6e523bb5, /// 0xa3c
			 0x0544405f, /// 0xa40
			 0xba70f5ed, /// 0xa44
			 0x5c7c07bd, /// 0xa48
			 0x292222a6, /// 0xa4c
			 0x3f6b0859, /// 0xa50
			 0xd3284538, /// 0xa54
			 0xadaa55c2, /// 0xa58
			 0x33c5e3d2, /// 0xa5c
			 0xded4bb2d, /// 0xa60
			 0x624c0721, /// 0xa64
			 0x139c7db0, /// 0xa68
			 0x1de4d110, /// 0xa6c
			 0x013e3f1c, /// 0xa70
			 0x577b6330, /// 0xa74
			 0x7cec96c8, /// 0xa78
			 0xf127cecd, /// 0xa7c
			 0x088d77a2, /// 0xa80
			 0xb8b3f097, /// 0xa84
			 0x34bcf4b8, /// 0xa88
			 0x2e85c12e, /// 0xa8c
			 0xf78fd508, /// 0xa90
			 0x7b1ebb75, /// 0xa94
			 0xf4041b65, /// 0xa98
			 0x9d5bcbcb, /// 0xa9c
			 0x405a96af, /// 0xaa0
			 0x1e79fdff, /// 0xaa4
			 0x33ccd003, /// 0xaa8
			 0xb39eab82, /// 0xaac
			 0x9e84f95c, /// 0xab0
			 0x2a213abe, /// 0xab4
			 0xca3448ba, /// 0xab8
			 0x87bf185b, /// 0xabc
			 0x7c4fbdf6, /// 0xac0
			 0xe5f47994, /// 0xac4
			 0x16689de7, /// 0xac8
			 0xe9cf3657, /// 0xacc
			 0x4316cdce, /// 0xad0
			 0x55377f74, /// 0xad4
			 0x4131b0ab, /// 0xad8
			 0x1a4f0c43, /// 0xadc
			 0x93fd6408, /// 0xae0
			 0x6d277ce1, /// 0xae4
			 0x61c6f52d, /// 0xae8
			 0x8e634e63, /// 0xaec
			 0x2e071a91, /// 0xaf0
			 0x462346a6, /// 0xaf4
			 0x582d042c, /// 0xaf8
			 0xc499b55b, /// 0xafc
			 0x40859a5e, /// 0xb00
			 0x9801ef62, /// 0xb04
			 0x348c37e7, /// 0xb08
			 0xda5a2cbe, /// 0xb0c
			 0xf93d3802, /// 0xb10
			 0x4c852f7d, /// 0xb14
			 0x5ce11d6a, /// 0xb18
			 0xb7845542, /// 0xb1c
			 0xa667f2f2, /// 0xb20
			 0x264de7e6, /// 0xb24
			 0x164213b1, /// 0xb28
			 0xc3db00f0, /// 0xb2c
			 0x8c150220, /// 0xb30
			 0xa2971136, /// 0xb34
			 0xbb55e8ab, /// 0xb38
			 0xfa3ba5a6, /// 0xb3c
			 0x4379f9c0, /// 0xb40
			 0x04f7fdfd, /// 0xb44
			 0x23561a8f, /// 0xb48
			 0x28686c24, /// 0xb4c
			 0x8e0dad06, /// 0xb50
			 0x5f7e27c3, /// 0xb54
			 0xa25c29f0, /// 0xb58
			 0xa40265c0, /// 0xb5c
			 0x4f14f6ea, /// 0xb60
			 0xe851029c, /// 0xb64
			 0x4592beb7, /// 0xb68
			 0x1663ad3f, /// 0xb6c
			 0x1b37d1d7, /// 0xb70
			 0xdb35b23d, /// 0xb74
			 0x77febfc4, /// 0xb78
			 0x016e193b, /// 0xb7c
			 0x1f71768b, /// 0xb80
			 0x3ba239ba, /// 0xb84
			 0xa75d0b8d, /// 0xb88
			 0x8d9dad13, /// 0xb8c
			 0xcc3ff077, /// 0xb90
			 0x4bc85ba8, /// 0xb94
			 0xe627fc07, /// 0xb98
			 0xddab0de6, /// 0xb9c
			 0x7591554b, /// 0xba0
			 0xa8375018, /// 0xba4
			 0x8eef2a52, /// 0xba8
			 0x28827cfa, /// 0xbac
			 0xe461fa54, /// 0xbb0
			 0x6e79f13a, /// 0xbb4
			 0xf1193915, /// 0xbb8
			 0x15cf5d4f, /// 0xbbc
			 0xe6c42050, /// 0xbc0
			 0x786878d7, /// 0xbc4
			 0x423d6969, /// 0xbc8
			 0x4e73a5e5, /// 0xbcc
			 0xfc6e78e8, /// 0xbd0
			 0xee37344f, /// 0xbd4
			 0xae025fe8, /// 0xbd8
			 0xe92a1bdc, /// 0xbdc
			 0x778848bb, /// 0xbe0
			 0x20a6f443, /// 0xbe4
			 0xed0a5389, /// 0xbe8
			 0x0bf867cf, /// 0xbec
			 0xdedf312e, /// 0xbf0
			 0xac58310a, /// 0xbf4
			 0xda8fa22a, /// 0xbf8
			 0x86c4e515, /// 0xbfc
			 0x4f75f2b6, /// 0xc00
			 0x8636272a, /// 0xc04
			 0x0f276f7d, /// 0xc08
			 0x5b017fec, /// 0xc0c
			 0x5309b65c, /// 0xc10
			 0x43f36875, /// 0xc14
			 0x5869c809, /// 0xc18
			 0xba67d98e, /// 0xc1c
			 0xfca96cbe, /// 0xc20
			 0x7d6bd63a, /// 0xc24
			 0x60235825, /// 0xc28
			 0x322e4c2e, /// 0xc2c
			 0x0464f7be, /// 0xc30
			 0x93e3dd35, /// 0xc34
			 0x095f2de0, /// 0xc38
			 0x8ff2452a, /// 0xc3c
			 0xfe04cfb8, /// 0xc40
			 0x1952a903, /// 0xc44
			 0x8d6b4a97, /// 0xc48
			 0x04f4c3f5, /// 0xc4c
			 0xa3a4cfb2, /// 0xc50
			 0xbd473f27, /// 0xc54
			 0xf8a3bd1b, /// 0xc58
			 0x47199f8d, /// 0xc5c
			 0xd1c11cec, /// 0xc60
			 0xb4c165d5, /// 0xc64
			 0xf9aa087c, /// 0xc68
			 0x965c6213, /// 0xc6c
			 0xff1159af, /// 0xc70
			 0xe7b9de99, /// 0xc74
			 0x668926bb, /// 0xc78
			 0xca42e9c1, /// 0xc7c
			 0xaf677871, /// 0xc80
			 0xfca1cf03, /// 0xc84
			 0xa542825b, /// 0xc88
			 0x8f899a72, /// 0xc8c
			 0x9cc49292, /// 0xc90
			 0xbdf99337, /// 0xc94
			 0x9096e972, /// 0xc98
			 0x0725d7ab, /// 0xc9c
			 0xa18c9785, /// 0xca0
			 0x2d840cc1, /// 0xca4
			 0x6138da1a, /// 0xca8
			 0xbc674c96, /// 0xcac
			 0x9fedbdd5, /// 0xcb0
			 0x6f8a459e, /// 0xcb4
			 0x690f2d04, /// 0xcb8
			 0xd23b526a, /// 0xcbc
			 0xd2eddf3d, /// 0xcc0
			 0xeae19355, /// 0xcc4
			 0xc35e2fca, /// 0xcc8
			 0x7507f218, /// 0xccc
			 0x139a892c, /// 0xcd0
			 0x6d732cf3, /// 0xcd4
			 0x1c1c3883, /// 0xcd8
			 0x2160b68b, /// 0xcdc
			 0xa9cf0a06, /// 0xce0
			 0xc8d80a8d, /// 0xce4
			 0xa64bcc65, /// 0xce8
			 0xf33821ff, /// 0xcec
			 0x5599135a, /// 0xcf0
			 0xac6d1e4b, /// 0xcf4
			 0x9302125c, /// 0xcf8
			 0x21fe459b, /// 0xcfc
			 0x634cd829, /// 0xd00
			 0xb3d505b8, /// 0xd04
			 0x35206a2d, /// 0xd08
			 0xfc3f33f4, /// 0xd0c
			 0x3c2a9846, /// 0xd10
			 0x494f7e23, /// 0xd14
			 0x8bea4f82, /// 0xd18
			 0x2361f6b3, /// 0xd1c
			 0xa490bb5b, /// 0xd20
			 0x415e2914, /// 0xd24
			 0x24fbb18c, /// 0xd28
			 0x1c44a03a, /// 0xd2c
			 0x38e919a3, /// 0xd30
			 0xc8a3dd97, /// 0xd34
			 0xb0a69a5a, /// 0xd38
			 0xf024e5a9, /// 0xd3c
			 0x5cee9b7d, /// 0xd40
			 0x7ffe9d45, /// 0xd44
			 0x15ffe42d, /// 0xd48
			 0x5dc65c06, /// 0xd4c
			 0xa819782c, /// 0xd50
			 0x6d92e066, /// 0xd54
			 0xe08a1b91, /// 0xd58
			 0x5b73a50c, /// 0xd5c
			 0x9ac5ea9d, /// 0xd60
			 0xeea2e578, /// 0xd64
			 0xfe57fe1d, /// 0xd68
			 0x3b3a7c25, /// 0xd6c
			 0x3be9d1ed, /// 0xd70
			 0x51cffa67, /// 0xd74
			 0xe91cb6b0, /// 0xd78
			 0x513a334a, /// 0xd7c
			 0xa1b7045c, /// 0xd80
			 0x4fb1d5e5, /// 0xd84
			 0xcb9f7c2f, /// 0xd88
			 0x2b64d532, /// 0xd8c
			 0xcd6d0050, /// 0xd90
			 0x8be6d279, /// 0xd94
			 0xd0888111, /// 0xd98
			 0xdece43cf, /// 0xd9c
			 0x4468cc6d, /// 0xda0
			 0x78595a88, /// 0xda4
			 0x5e6590d5, /// 0xda8
			 0x6f27fc84, /// 0xdac
			 0x744fbfcb, /// 0xdb0
			 0xacbcc9ae, /// 0xdb4
			 0xf41c725c, /// 0xdb8
			 0x038a5a58, /// 0xdbc
			 0x658937a7, /// 0xdc0
			 0x4d5ee038, /// 0xdc4
			 0x2cece8e3, /// 0xdc8
			 0x194622ed, /// 0xdcc
			 0x160ba2b1, /// 0xdd0
			 0x31f76367, /// 0xdd4
			 0x42980fb0, /// 0xdd8
			 0x4e7b642c, /// 0xddc
			 0xaf18e02e, /// 0xde0
			 0x36f857e1, /// 0xde4
			 0x6c5e3691, /// 0xde8
			 0x9d70dc5b, /// 0xdec
			 0xa6de16ba, /// 0xdf0
			 0xffa4aefc, /// 0xdf4
			 0x155d51a7, /// 0xdf8
			 0xa58e586f, /// 0xdfc
			 0x0443dfc2, /// 0xe00
			 0xf20c40af, /// 0xe04
			 0x58910b79, /// 0xe08
			 0x64f3eda8, /// 0xe0c
			 0x558f14c6, /// 0xe10
			 0x718e25a1, /// 0xe14
			 0x20e1b1ac, /// 0xe18
			 0xa90013b6, /// 0xe1c
			 0xa1c86467, /// 0xe20
			 0xf989abb4, /// 0xe24
			 0x4c00d513, /// 0xe28
			 0x6c7856c7, /// 0xe2c
			 0xf41fd17e, /// 0xe30
			 0x2ceaee99, /// 0xe34
			 0x0221d026, /// 0xe38
			 0x25103bd3, /// 0xe3c
			 0x47002f92, /// 0xe40
			 0x880fb413, /// 0xe44
			 0x9f8ce9e2, /// 0xe48
			 0x16b4ceef, /// 0xe4c
			 0xec774d35, /// 0xe50
			 0x433b5400, /// 0xe54
			 0xdadb6881, /// 0xe58
			 0xe0287dbc, /// 0xe5c
			 0x977a4dd0, /// 0xe60
			 0x7ce353cd, /// 0xe64
			 0x3ff37e88, /// 0xe68
			 0xbd527469, /// 0xe6c
			 0x0df0d3b3, /// 0xe70
			 0x99b646cf, /// 0xe74
			 0xd2ace5e5, /// 0xe78
			 0x16071ca0, /// 0xe7c
			 0x90a2261c, /// 0xe80
			 0xbba9959d, /// 0xe84
			 0xaef8b51c, /// 0xe88
			 0xbaea820a, /// 0xe8c
			 0x9471da06, /// 0xe90
			 0xcb012e58, /// 0xe94
			 0xf9d93e8c, /// 0xe98
			 0x3f938577, /// 0xe9c
			 0xecf2da24, /// 0xea0
			 0x3340de9c, /// 0xea4
			 0x322e059f, /// 0xea8
			 0x13627313, /// 0xeac
			 0x6cb86f1c, /// 0xeb0
			 0x8dbc1efb, /// 0xeb4
			 0xb83a3ba0, /// 0xeb8
			 0xd4902451, /// 0xebc
			 0xfc3a7991, /// 0xec0
			 0x825610cc, /// 0xec4
			 0x7906d44d, /// 0xec8
			 0xeacabe9c, /// 0xecc
			 0x0cb7d6d5, /// 0xed0
			 0xfa8cb0b6, /// 0xed4
			 0xc9ef60bf, /// 0xed8
			 0xe08359ae, /// 0xedc
			 0x153ea6d4, /// 0xee0
			 0x14360c59, /// 0xee4
			 0xfc752d4d, /// 0xee8
			 0xc8d4205d, /// 0xeec
			 0xf22229d1, /// 0xef0
			 0x47bcbefb, /// 0xef4
			 0xd18a04bd, /// 0xef8
			 0x6c6674ac, /// 0xefc
			 0xac488220, /// 0xf00
			 0xeafbfaea, /// 0xf04
			 0x6ad9d9db, /// 0xf08
			 0x04ab20d9, /// 0xf0c
			 0x11acc548, /// 0xf10
			 0xc0d93f51, /// 0xf14
			 0xfba3b746, /// 0xf18
			 0x22eac71e, /// 0xf1c
			 0x73ad7a19, /// 0xf20
			 0x7b22a55c, /// 0xf24
			 0xb34768ac, /// 0xf28
			 0x38ec1ad2, /// 0xf2c
			 0x3e7f239a, /// 0xf30
			 0x506e313f, /// 0xf34
			 0x416d289b, /// 0xf38
			 0x6601a45b, /// 0xf3c
			 0x29dcf0b8, /// 0xf40
			 0x67f93f92, /// 0xf44
			 0x368f94e6, /// 0xf48
			 0x84763f9f, /// 0xf4c
			 0x1a9360e0, /// 0xf50
			 0x1453f7fb, /// 0xf54
			 0x1394789f, /// 0xf58
			 0xbad14180, /// 0xf5c
			 0x59114ea2, /// 0xf60
			 0x7567b1d6, /// 0xf64
			 0x90a1e8ef, /// 0xf68
			 0xcf3e3170, /// 0xf6c
			 0xa6cde866, /// 0xf70
			 0x7d685896, /// 0xf74
			 0x865ac0f7, /// 0xf78
			 0xccc408ba, /// 0xf7c
			 0x74927858, /// 0xf80
			 0x9e08e671, /// 0xf84
			 0xb9341cb5, /// 0xf88
			 0x6d919b8a, /// 0xf8c
			 0xd6ee6426, /// 0xf90
			 0x35f4fd1e, /// 0xf94
			 0x0d51abf0, /// 0xf98
			 0x12c8ac0c, /// 0xf9c
			 0x2a872688, /// 0xfa0
			 0x26a3bf87, /// 0xfa4
			 0x30274846, /// 0xfa8
			 0x62c5584a, /// 0xfac
			 0x7f38b232, /// 0xfb0
			 0xe27f3128, /// 0xfb4
			 0x937e2db0, /// 0xfb8
			 0xfb52dbf6, /// 0xfbc
			 0x59593022, /// 0xfc0
			 0x55a55bd7, /// 0xfc4
			 0xc4d7a2d5, /// 0xfc8
			 0xb844e7c4, /// 0xfcc
			 0x3f3c06b0, /// 0xfd0
			 0x9ddd8985, /// 0xfd4
			 0x291d33c5, /// 0xfd8
			 0x940fe39c, /// 0xfdc
			 0x38201c76, /// 0xfe0
			 0xa3afbe9b, /// 0xfe4
			 0x7fb495db, /// 0xfe8
			 0x595772ed, /// 0xfec
			 0xfa1482ce, /// 0xff0
			 0xe53d2347, /// 0xff4
			 0x16d6d4ad, /// 0xff8
			 0x78e19964 /// last
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
			 0x0000037c,
			 0x0000004c,
			 0x0000076c,
			 0x00000464,
			 0x00000224,
			 0x000004e4,
			 0x00000490,
			 0x000004c4,

			 /// vpu register f2
			 0x41f00000,
			 0x41500000,
			 0x41a00000,
			 0x41500000,
			 0x41d00000,
			 0x41800000,
			 0x40c00000,
			 0x41800000,

			 /// vpu register f3
			 0x41500000,
			 0x41e80000,
			 0x41d80000,
			 0x41600000,
			 0x41e80000,
			 0x41d00000,
			 0x41b00000,
			 0x41500000,

			 /// vpu register f4
			 0x41900000,
			 0x41700000,
			 0x41100000,
			 0x41f00000,
			 0x3f800000,
			 0x41980000,
			 0x41d00000,
			 0x41f00000,

			 /// vpu register f5
			 0x41e80000,
			 0x41600000,
			 0x40800000,
			 0x41880000,
			 0x41a00000,
			 0x41c00000,
			 0x3f800000,
			 0x41700000,

			 /// vpu register f6
			 0x41400000,
			 0x41000000,
			 0x40400000,
			 0x41e00000,
			 0x41c00000,
			 0x40800000,
			 0x41e00000,
			 0x41c80000,

			 /// vpu register f7
			 0x41400000,
			 0x40a00000,
			 0x40800000,
			 0x41b00000,
			 0x41000000,
			 0x40400000,
			 0x41100000,
			 0x41400000,

			 /// vpu register f8
			 0x3f800000,
			 0x41b00000,
			 0x41700000,
			 0x41d80000,
			 0x40c00000,
			 0x41900000,
			 0x41e00000,
			 0x41d00000,

			 /// vpu register f9
			 0x41a00000,
			 0x41c80000,
			 0x41900000,
			 0x41000000,
			 0x41c00000,
			 0x41d00000,
			 0x42000000,
			 0x41900000,

			 /// vpu register f10
			 0x41b00000,
			 0x42000000,
			 0x41600000,
			 0x41e80000,
			 0x41880000,
			 0x40000000,
			 0x40a00000,
			 0x40c00000,

			 /// vpu register f11
			 0x41100000,
			 0x41400000,
			 0x41c80000,
			 0x41700000,
			 0x41400000,
			 0x41c00000,
			 0x41100000,
			 0x41800000,

			 /// vpu register f12
			 0x41100000,
			 0x41e80000,
			 0x41e80000,
			 0x40800000,
			 0x41500000,
			 0x41400000,
			 0x3f800000,
			 0x41a80000,

			 /// vpu register f13
			 0x41500000,
			 0x41800000,
			 0x41d00000,
			 0x41000000,
			 0x41000000,
			 0x41100000,
			 0x41980000,
			 0x41880000,

			 /// vpu register f14
			 0x40a00000,
			 0x41e00000,
			 0x41880000,
			 0x41b00000,
			 0x41100000,
			 0x41f80000,
			 0x41900000,
			 0x41980000,

			 /// vpu register f15
			 0x41b00000,
			 0x41900000,
			 0x41c80000,
			 0x41880000,
			 0x41880000,
			 0x41c80000,
			 0x40a00000,
			 0x41700000,

			 /// vpu register f16
			 0x41c80000,
			 0x41980000,
			 0x41c00000,
			 0x41900000,
			 0x41e80000,
			 0x41c00000,
			 0x41f00000,
			 0x40c00000,

			 /// vpu register f17
			 0x40c00000,
			 0x41700000,
			 0x3f800000,
			 0x41880000,
			 0x41800000,
			 0x41c80000,
			 0x41c00000,
			 0x41400000,

			 /// vpu register f18
			 0x41000000,
			 0x40000000,
			 0x40400000,
			 0x41f00000,
			 0x41800000,
			 0x41700000,
			 0x41f00000,
			 0x41e00000,

			 /// vpu register f19
			 0x3f800000,
			 0x41d80000,
			 0x41f00000,
			 0x41300000,
			 0x41e80000,
			 0x41100000,
			 0x40800000,
			 0x41800000,

			 /// vpu register f20
			 0x41980000,
			 0x41f80000,
			 0x40c00000,
			 0x41d00000,
			 0x41400000,
			 0x40a00000,
			 0x40000000,
			 0x41980000,

			 /// vpu register f21
			 0x41e00000,
			 0x41d00000,
			 0x41980000,
			 0x41b80000,
			 0x41a80000,
			 0x41e80000,
			 0x41700000,
			 0x41a00000,

			 /// vpu register f22
			 0x41b80000,
			 0x41a80000,
			 0x40800000,
			 0x41400000,
			 0x40400000,
			 0x41800000,
			 0x41f00000,
			 0x40000000,

			 /// vpu register f23
			 0x41100000,
			 0x41000000,
			 0x41a80000,
			 0x41d00000,
			 0x41e80000,
			 0x41400000,
			 0x41f80000,
			 0x41e00000,

			 /// vpu register f24
			 0x42000000,
			 0x41200000,
			 0x41880000,
			 0x41d80000,
			 0x40400000,
			 0x41700000,
			 0x41980000,
			 0x41100000,

			 /// vpu register f25
			 0x40400000,
			 0x41300000,
			 0x41100000,
			 0x41f80000,
			 0x41a80000,
			 0x41b00000,
			 0x41700000,
			 0x41e80000,

			 /// vpu register f26
			 0x41e00000,
			 0x41500000,
			 0x41f00000,
			 0x41200000,
			 0x42000000,
			 0x41000000,
			 0x41c00000,
			 0x41a80000,

			 /// vpu register f27
			 0x41f80000,
			 0x41a00000,
			 0x41e00000,
			 0x41f80000,
			 0x41500000,
			 0x40800000,
			 0x41f00000,
			 0x41f80000,

			 /// vpu register f28
			 0x41400000,
			 0x40e00000,
			 0x42000000,
			 0x41b80000,
			 0x41b80000,
			 0x41e80000,
			 0x41a00000,
			 0x41b80000,

			 /// vpu register f29
			 0x40800000,
			 0x41b00000,
			 0x41900000,
			 0x41d00000,
			 0x41b00000,
			 0x41b80000,
			 0x41980000,
			 0x42000000,

			 /// vpu register f30
			 0x3f800000,
			 0x41100000,
			 0x40e00000,
			 0x41f80000,
			 0x41d80000,
			 0x41980000,
			 0x41700000,
			 0x40000000,

			 /// vpu register f31
			 0x40000000,
			 0x40c00000,
			 0x42000000,
			 0x40800000,
			 0x41300000,
			 0x41900000,
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
		"fsrai.pi f29, f28, 11\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f26, f25, 9\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f10, f18, 8\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f12, f25, 2\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f11, f21, 14\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f27, f30, 11\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f9, f25, 9\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f29, f14, 6\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f10, f10, 1\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f26, f14, 1\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f21, f26, 3\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f13, f26, 1\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f4, f8, 9\n"                                ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f24, f24, 5\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f10, f10, 15\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f20, f9, 12\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f23, f5, 15\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f23, f14, 12\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f14, f11, 2\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f26, f18, 13\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f10, f11, 2\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f20, f5, 11\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f28, f29, 10\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f28, f30, 5\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f7, f22, 9\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f17, f18, 7\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f29, f23, 5\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f15, f28, 3\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f12, f23, 15\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f27, f27, 5\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f16, f26, 9\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f14, f15, 2\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f28, f3, 12\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f27, f22, 1\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f29, f12, 8\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f6, f27, 4\n"                               ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f15, f10, 7\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f8, f2, 14\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f25, f3, 15\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f9, f24, 5\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f8, f24, 11\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f24, f12, 9\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f3, f15, 3\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f15, f6, 4\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f7, f8, 2\n"                                ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f27, f10, 8\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f22, f14, 14\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f17, f7, 14\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f9, f19, 2\n"                               ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f17, f24, 12\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f9, f27, 6\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f5, f5, 5\n"                                ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f20, f17, 14\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f8, f20, 6\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f24, f27, 7\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f29, f28, 8\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f2, f29, 6\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f10, f18, 0\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f17, f4, 5\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f14, f26, 8\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f28, f18, 2\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f16, f13, 1\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f24, f26, 9\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f12, f6, 10\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f25, f3, 12\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f30, f9, 5\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f18, f12, 11\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f17, f7, 11\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f17, f6, 0\n"                               ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f6, f23, 11\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f18, f18, 15\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f16, f1, 3\n"                               ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f28, f5, 7\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f5, f5, 6\n"                                ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f5, f18, 15\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f22, f7, 6\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f9, f9, 2\n"                                ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f29, f21, 12\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f3, f19, 6\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f29, f18, 8\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f27, f28, 6\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f19, f0, 7\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f19, f6, 6\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f27, f13, 5\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f22, f13, 13\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f22, f5, 12\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f29, f28, 9\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f28, f1, 5\n"                               ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f28, f13, 4\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f22, f20, 5\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f18, f29, 11\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f22, f0, 3\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f25, f0, 14\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f23, f0, 15\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f6, f0, 11\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f3, f9, 9\n"                                ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f11, f7, 7\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f21, f26, 3\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrai.pi f11, f29, 5\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
