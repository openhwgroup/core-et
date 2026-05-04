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
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00000
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00004
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00008
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0000c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00018
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0001c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00020
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00024
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0002c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00030
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00038
			 0x0c400000,                                                  // Leading 1s:                                 /// 0003c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00040
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00044
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00048
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0004c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00050
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x0c600000,                                                  // Leading 1s:                                 /// 00058
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0005c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00068
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00070
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00074
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0007c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00084
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00088
			 0x7f800000,                                                  // inf                                         /// 0008c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00090
			 0xcb000000,                                                  // -8388608.0                                  /// 00094
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00098
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 000b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 000cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 000f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0010c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00110
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00114
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0011c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00120
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00128
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0012c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00130
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00134
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00138
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0013c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00140
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00148
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0014c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0xff800000,                                                  // -inf                                        /// 00154
			 0x7fc00001,                                                  // signaling NaN                               /// 00158
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0015c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00160
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00168
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00170
			 0x0e000001,                                                  // Trailing 1s:                                /// 00174
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00178
			 0x4b000000,                                                  // 8388608.0                                   /// 0017c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00180
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00184
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00188
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x33333333,                                                  // 4 random values                             /// 00190
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00194
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0019c
			 0xffc00001,                                                  // -signaling NaN                              /// 001a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001b8
			 0x00000000,                                                  // zero                                        /// 001bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001f0
			 0x80000000,                                                  // -zero                                       /// 001f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 001f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00200
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00204
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00208
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0021c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00220
			 0x00000000,                                                  // zero                                        /// 00224
			 0x0c700000,                                                  // Leading 1s:                                 /// 00228
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0022c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00230
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00234
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0023c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00240
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00244
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00250
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00254
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00258
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0025c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00260
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0026c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00274
			 0x0e000003,                                                  // Trailing 1s:                                /// 00278
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0027c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00284
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00288
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0028c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00290
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00298
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0029c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 002d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 002e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 002ec
			 0xff800000,                                                  // -inf                                        /// 002f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00308
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0030c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00310
			 0x0c600000,                                                  // Leading 1s:                                 /// 00314
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00318
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00320
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00324
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00328
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00330
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00334
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0033c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00344
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00348
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0034c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00354
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00358
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0035c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00360
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0036c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00370
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0037c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00380
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0c700000,                                                  // Leading 1s:                                 /// 0038c
			 0x4b000000,                                                  // 8388608.0                                   /// 00390
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00394
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0039c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003c8
			 0x4b000000,                                                  // 8388608.0                                   /// 003cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00400
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00408
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00410
			 0x0c400000,                                                  // Leading 1s:                                 /// 00414
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00418
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0042c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00430
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00434
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00438
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0043c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00444
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0044c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00454
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0045c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00460
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00464
			 0x0e000003,                                                  // Trailing 1s:                                /// 00468
			 0x33333333,                                                  // 4 random values                             /// 0046c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00474
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00478
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x55555555,                                                  // 4 random values                             /// 00480
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00488
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0048c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00494
			 0xbf028f5c,                                                  // -0.51                                       /// 00498
			 0xbf028f5c,                                                  // -0.51                                       /// 0049c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x55555555,                                                  // 4 random values                             /// 004b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004e0
			 0x55555555,                                                  // 4 random values                             /// 004e4
			 0x00000000,                                                  // zero                                        /// 004e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0xffc00001,                                                  // -signaling NaN                              /// 00504
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x0e000001,                                                  // Trailing 1s:                                /// 0050c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00510
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00518
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0051c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00520
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00524
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00528
			 0x7fc00001,                                                  // signaling NaN                               /// 0052c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00530
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00534
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00538
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0053c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00540
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00544
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00548
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0c400000,                                                  // Leading 1s:                                 /// 00554
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00558
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00560
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0xcb000000,                                                  // -8388608.0                                  /// 0056c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00570
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0xff800000,                                                  // -inf                                        /// 00578
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0057c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00580
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00584
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00588
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00590
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00598
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0059c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0xff800000,                                                  // -inf                                        /// 005d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 005d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00600
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00604
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0060c
			 0x7f800000,                                                  // inf                                         /// 00610
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00620
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0062c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00630
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00638
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0063c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00640
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00644
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00648
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0064c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00650
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00658
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0065c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00660
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00664
			 0xffc00001,                                                  // -signaling NaN                              /// 00668
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0066c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00670
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00674
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00678
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x00000000,                                                  // zero                                        /// 0068c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00690
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00694
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00698
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0069c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00700
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00704
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00708
			 0x80000000,                                                  // -zero                                       /// 0070c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00710
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00714
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00718
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0071c
			 0xcb000000,                                                  // -8388608.0                                  /// 00720
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00724
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00728
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00730
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00738
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00740
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00744
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00748
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0074c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0xcb000000,                                                  // -8388608.0                                  /// 00754
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0075c
			 0x33333333,                                                  // 4 random values                             /// 00760
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00764
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00768
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0076c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00770
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00778
			 0xff800000,                                                  // -inf                                        /// 0077c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00780
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00788
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00790
			 0x00000000,                                                  // zero                                        /// 00794
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0079c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007a4
			 0x3f028f5c,                                                  // 0.51                                        /// 007a8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007b0
			 0x3f028f5c,                                                  // 0.51                                        /// 007b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x7fc00001,                                                  // signaling NaN                               /// 007f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00800
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00804
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00808
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00814
			 0x7f800000,                                                  // inf                                         /// 00818
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0081c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00820
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0082c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00830
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00834
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00838
			 0xffc00001,                                                  // -signaling NaN                              /// 0083c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00844
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x3f028f5c,                                                  // 0.51                                        /// 0084c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00850
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0085c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00864
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00868
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0086c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00870
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00874
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00878
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0087c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00880
			 0x80000000,                                                  // -zero                                       /// 00884
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00894
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x7f800000,                                                  // inf                                         /// 0089c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 008b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 008d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00900
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00904
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00908
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00918
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0091c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00924
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00928
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00930
			 0x7f800000,                                                  // inf                                         /// 00934
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00938
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00940
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00950
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00954
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0095c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00964
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00970
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0097c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00980
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00984
			 0x0e000003,                                                  // Trailing 1s:                                /// 00988
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00994
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00998
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0099c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009dc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a0c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a18
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a20
			 0x4b000000,                                                  // 8388608.0                                   /// 00a24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a34
			 0xffc00001,                                                  // -signaling NaN                              /// 00a38
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a48
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a54
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a5c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a68
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a70
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a74
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a78
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a7c
			 0x33333333,                                                  // 4 random values                             /// 00a80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a88
			 0xffc00001,                                                  // -signaling NaN                              /// 00a8c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a94
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a98
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00aa8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00aac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ab8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00abc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ac0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ac4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ad4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ae4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00af0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00afc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b04
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b10
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b14
			 0x55555555,                                                  // 4 random values                             /// 00b18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80000000,                                                  // -zero                                       /// 00b2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b30
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b38
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b3c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b40
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b44
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b4c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x00000000,                                                  // zero                                        /// 00b58
			 0xcb000000,                                                  // -8388608.0                                  /// 00b5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b78
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b7c
			 0x55555555,                                                  // 4 random values                             /// 00b80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b8c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b90
			 0x7fc00001,                                                  // signaling NaN                               /// 00b94
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0xff800000,                                                  // -inf                                        /// 00ba4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bb0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bb4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bb8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bbc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bc0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bc4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bc8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bcc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bd0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bd8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bdc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00be0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00be8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bf8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bfc
			 0x7f800000,                                                  // inf                                         /// 00c00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c08
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c14
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c2c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c40
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c60
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c70
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c7c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c88
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c90
			 0xcb000000,                                                  // -8388608.0                                  /// 00c94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c9c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cb0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cb4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cb8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cc0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cc4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ccc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cd4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cf0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cfc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d0c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d18
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d1c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d20
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d28
			 0xcb000000,                                                  // -8388608.0                                  /// 00d2c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d30
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d34
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d38
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d40
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d4c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d54
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d58
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d70
			 0x33333333,                                                  // 4 random values                             /// 00d74
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d7c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d94
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00da0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00da4
			 0x80000000,                                                  // -zero                                       /// 00da8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00db0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00db4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00db8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dbc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dc0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dc8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dd0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dd8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00de0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00de8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00df0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00df4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00df8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dfc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e00
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e08
			 0x3f028f5c,                                                  // 0.51                                        /// 00e0c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e10
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0xff800000,                                                  // -inf                                        /// 00e18
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e20
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e2c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e34
			 0x55555555,                                                  // 4 random values                             /// 00e38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e40
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e4c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e54
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e68
			 0x33333333,                                                  // 4 random values                             /// 00e6c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e74
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e78
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e88
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e90
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e98
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ea4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00eac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eb0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00eb4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eb8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ebc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ecc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ed4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ee0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ee8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00eec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ef0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ef4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f08
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f0c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f10
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f14
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f1c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f20
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f28
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f34
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f38
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f3c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f64
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f6c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f84
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f90
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fa0
			 0x00000000,                                                  // zero                                        /// 00fa4
			 0x7fc00001,                                                  // signaling NaN                               /// 00fa8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fbc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fc4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fc8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fd0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fd4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fe0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fe8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ff4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ff8
			 0x00000001                                                  // 1.4E-45 (+denorm)                           /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80011111,                                                  // -9.7958E-41                                 /// 00000
			 0x80011111,                                                  // -9.7958E-41                                 /// 00004
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00008
			 0x0e000003,                                                  // Trailing 1s:                                /// 0000c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00014
			 0xff800000,                                                  // -inf                                        /// 00018
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00020
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00028
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0002c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00030
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00038
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00040
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00048
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0004c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00050
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00060
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x0e000001,                                                  // Trailing 1s:                                /// 00068
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0006c
			 0x7f800000,                                                  // inf                                         /// 00070
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00074
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00078
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0007c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00088
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0008c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00090
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0009c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000a0
			 0x7f800000,                                                  // inf                                         /// 000a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 000b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000d0
			 0xbf028f5c,                                                  // -0.51                                       /// 000d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0xcb000000,                                                  // -8388608.0                                  /// 000e8
			 0xff800000,                                                  // -inf                                        /// 000ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00104
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0010c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00110
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00114
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00118
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00120
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00124
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00128
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0012c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00130
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00134
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00138
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x0c400000,                                                  // Leading 1s:                                 /// 00148
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00150
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00154
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00158
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00168
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0016c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00170
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00174
			 0x0c780000,                                                  // Leading 1s:                                 /// 00178
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0017c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00184
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00194
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00198
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 001a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001d4
			 0x4b000000,                                                  // 8388608.0                                   /// 001d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00200
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00204
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00208
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00210
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00224
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00228
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0022c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00230
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00234
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00238
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0023c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00244
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00248
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0024c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00250
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0025c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00268
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0026c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00270
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00274
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00278
			 0xbf028f5c,                                                  // -0.51                                       /// 0027c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00280
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00284
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00288
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0028c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00294
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00298
			 0x7f800000,                                                  // inf                                         /// 0029c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002f8
			 0x7fc00001,                                                  // signaling NaN                               /// 002fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00304
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00308
			 0x80000000,                                                  // -zero                                       /// 0030c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00314
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00318
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00320
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00324
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0033c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00340
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00344
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00348
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0034c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00350
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00358
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0035c
			 0x3f028f5c,                                                  // 0.51                                        /// 00360
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0xcb000000,                                                  // -8388608.0                                  /// 0036c
			 0xffc00001,                                                  // -signaling NaN                              /// 00370
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00374
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0e000007,                                                  // Trailing 1s:                                /// 0037c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0038c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00390
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00398
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 003b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003e8
			 0x7f800000,                                                  // inf                                         /// 003ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00400
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00404
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00408
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0040c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00410
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00414
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00418
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0041c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00420
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00424
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00428
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00430
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00440
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00448
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00450
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00454
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00460
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00464
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00468
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00474
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00478
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0047c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00480
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00484
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0048c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00490
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x80000000,                                                  // -zero                                       /// 00498
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0049c
			 0x0c600000,                                                  // Leading 1s:                                 /// 004a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00500
			 0x00011111,                                                  // 9.7958E-41                                  /// 00504
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00508
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0050c
			 0x4b000000,                                                  // 8388608.0                                   /// 00510
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00514
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00518
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00528
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00534
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00538
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00540
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00544
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00550
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00554
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0055c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0056c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00570
			 0x0e000007,                                                  // Trailing 1s:                                /// 00574
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00578
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00580
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00584
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00588
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00590
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0059c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005c4
			 0x0e000001,                                                  // Trailing 1s:                                /// 005c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005cc
			 0x00000000,                                                  // zero                                        /// 005d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 005e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00600
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0060c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00610
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00618
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0061c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00620
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00624
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0062c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00634
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00638
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00640
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00644
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0064c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00650
			 0x0c400000,                                                  // Leading 1s:                                 /// 00654
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0065c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00660
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0066c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00674
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x55555555,                                                  // 4 random values                             /// 00684
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00688
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0068c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00690
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x00000000,                                                  // zero                                        /// 00698
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0069c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006c0
			 0x00000000,                                                  // zero                                        /// 006c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 006d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006dc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00708
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x0e000001,                                                  // Trailing 1s:                                /// 00714
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0xcb000000,                                                  // -8388608.0                                  /// 0071c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00720
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00724
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0073c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00740
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00744
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00748
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0074c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00754
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0075c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00764
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0076c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00770
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00778
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0077c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00780
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00784
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0078c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00794
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00798
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 007b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0xffc00001,                                                  // -signaling NaN                              /// 007cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 007e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0xcb000000,                                                  // -8388608.0                                  /// 007f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007f8
			 0xffc00001,                                                  // -signaling NaN                              /// 007fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00808
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0080c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00810
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00814
			 0x00000000,                                                  // zero                                        /// 00818
			 0x0c400000,                                                  // Leading 1s:                                 /// 0081c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00820
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00824
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00828
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00830
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00834
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00840
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00844
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0xbf028f5c,                                                  // -0.51                                       /// 00850
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00858
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0085c
			 0xbf028f5c,                                                  // -0.51                                       /// 00860
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00868
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00870
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00874
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0087c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00880
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00884
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0e000001,                                                  // Trailing 1s:                                /// 0088c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00894
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00898
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x33333333,                                                  // 4 random values                             /// 008c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00900
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00908
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00910
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00914
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0091c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00924
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00928
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0092c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00934
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00938
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00940
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00944
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00948
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0094c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00954
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00958
			 0x00011111,                                                  // 9.7958E-41                                  /// 0095c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00960
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00968
			 0x00011111,                                                  // 9.7958E-41                                  /// 0096c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00970
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00974
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00978
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0097c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00984
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0098c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00990
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 009d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 009e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x3f028f5c,                                                  // 0.51                                        /// 009f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a04
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a0c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a1c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a24
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a28
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a38
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a40
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a44
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a50
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a54
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a60
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a90
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a94
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a98
			 0x7fc00001,                                                  // signaling NaN                               /// 00a9c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00aa0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00aa4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00aa8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ab0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ab4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ab8
			 0xbf028f5c,                                                  // -0.51                                       /// 00abc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ac8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00acc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ad0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ad4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x7f800000,                                                  // inf                                         /// 00ae0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ae4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ae8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00aec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00af0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00af4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00afc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b10
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b1c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0xcb000000,                                                  // -8388608.0                                  /// 00b24
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b34
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b38
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b40
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b48
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b64
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x7f800000,                                                  // inf                                         /// 00b6c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b90
			 0xff800000,                                                  // -inf                                        /// 00b94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bb0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bb8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bbc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bc4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bc8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x55555555,                                                  // 4 random values                             /// 00bd0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bd8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bdc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00be0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00be4
			 0xbf028f5c,                                                  // -0.51                                       /// 00be8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bf0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bf8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bfc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c14
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c1c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c2c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x3f028f5c,                                                  // 0.51                                        /// 00c38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c40
			 0xffc00001,                                                  // -signaling NaN                              /// 00c44
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c4c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c50
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c60
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c80
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c88
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c94
			 0x00000000,                                                  // zero                                        /// 00c98
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ca0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ca4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cb0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cb8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cbc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cc0
			 0x55555555,                                                  // 4 random values                             /// 00cc4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cc8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ccc
			 0xffc00001,                                                  // -signaling NaN                              /// 00cd0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cd4
			 0x00000000,                                                  // zero                                        /// 00cd8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ce0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ce4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ce8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cf0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cf4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cf8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d04
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d28
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d30
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d3c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d44
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d54
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d6c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d7c
			 0x55555555,                                                  // 4 random values                             /// 00d80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d84
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d88
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d8c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00da0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00da8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dc0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dcc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dd4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00de0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00de4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00df0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00df8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dfc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e04
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x80000000,                                                  // -zero                                       /// 00e1c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e34
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e38
			 0x00000000,                                                  // zero                                        /// 00e3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e40
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e48
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e4c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e58
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e70
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e80
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e88
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e8c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e90
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e98
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x33333333,                                                  // 4 random values                             /// 00ea4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00eac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eb4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ebc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ec4
			 0x00000000,                                                  // zero                                        /// 00ec8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ecc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00edc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ee0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ee4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ee8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ef4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00efc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f04
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f1c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f24
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f28
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f38
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f48
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f54
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f58
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f5c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f60
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f68
			 0x4b000000,                                                  // 8388608.0                                   /// 00f6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f7c
			 0x33333333,                                                  // 4 random values                             /// 00f80
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f84
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f98
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f9c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fa0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fa4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fa8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fb0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fb4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fb8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fbc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fc0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fd0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fd8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fe8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ff4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0xff800001 // SNaN                                          // SP - ve numbers                             /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00004
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0000c
			 0x7fc00001,                                                  // signaling NaN                               /// 00010
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00014
			 0x0c700000,                                                  // Leading 1s:                                 /// 00018
			 0x0c600000,                                                  // Leading 1s:                                 /// 0001c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00020
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00024
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00028
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0002c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00030
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0003c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00040
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x80011111,                                                  // -9.7958E-41                                 /// 00048
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0004c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00050
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00054
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00058
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00064
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00068
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00074
			 0x80011111,                                                  // -9.7958E-41                                 /// 00078
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00080
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0e000007,                                                  // Trailing 1s:                                /// 0008c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00090
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000ec
			 0x7fc00001,                                                  // signaling NaN                               /// 000f0
			 0x80000000,                                                  // -zero                                       /// 000f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00100
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00104
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00108
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0010c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00110
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00118
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0011c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00120
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00124
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00128
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00130
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00138
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00140
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00144
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00148
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00150
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00154
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0015c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00168
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00170
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00174
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0017c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00180
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x7f800000,                                                  // inf                                         /// 00188
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00194
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00198
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0019c
			 0x0c700000,                                                  // Leading 1s:                                 /// 001a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 001e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0xcb000000,                                                  // -8388608.0                                  /// 001f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 001f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001fc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00200
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00204
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00208
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0020c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00210
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00214
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0021c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00220
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00224
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00228
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0022c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00230
			 0x0e000003,                                                  // Trailing 1s:                                /// 00234
			 0x0e000001,                                                  // Trailing 1s:                                /// 00238
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0023c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00240
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0x55555555,                                                  // 4 random values                             /// 00248
			 0x0e000001,                                                  // Trailing 1s:                                /// 0024c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00250
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00254
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0026c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00270
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00278
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00280
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80000000,                                                  // -zero                                       /// 00298
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002b0
			 0x00000000,                                                  // zero                                        /// 002b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002bc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002c8
			 0x55555555,                                                  // 4 random values                             /// 002cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002ec
			 0x00000000,                                                  // zero                                        /// 002f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00304
			 0x4b000000,                                                  // 8388608.0                                   /// 00308
			 0x0c700000,                                                  // Leading 1s:                                 /// 0030c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0xcb000000,                                                  // -8388608.0                                  /// 00318
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0031c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00320
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00328
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0032c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00330
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00344
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00348
			 0xcb000000,                                                  // -8388608.0                                  /// 0034c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00354
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00358
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00360
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00364
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00370
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00378
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00384
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0c400000,                                                  // Leading 1s:                                 /// 0038c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00394
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00398
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0039c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003ac
			 0x00000000,                                                  // zero                                        /// 003b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00400
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00404
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00408
			 0x7f800000,                                                  // inf                                         /// 0040c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00414
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0041c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00420
			 0x7fc00001,                                                  // signaling NaN                               /// 00424
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00430
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00440
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00444
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0044c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x55555555,                                                  // 4 random values                             /// 00454
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00458
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00460
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00464
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0046c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00474
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00478
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00480
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00484
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00488
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00490
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00494
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00498
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0049c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004a4
			 0x55555555,                                                  // 4 random values                             /// 004a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00500
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00011111,                                                  // 9.7958E-41                                  /// 00508
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x55555555,                                                  // 4 random values                             /// 00518
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0051c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00520
			 0x0c700000,                                                  // Leading 1s:                                 /// 00524
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00528
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0052c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00530
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00534
			 0x7fc00001,                                                  // signaling NaN                               /// 00538
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00544
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00548
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00550
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00554
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0055c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00560
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00568
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0xbf028f5c,                                                  // -0.51                                       /// 00570
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0057c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x3f028f5c,                                                  // 0.51                                        /// 00584
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0058c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00590
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x3f028f5c,                                                  // 0.51                                        /// 0059c
			 0x0c780000,                                                  // Leading 1s:                                 /// 005a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005a8
			 0xffc00001,                                                  // -signaling NaN                              /// 005ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 005e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00600
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00604
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00608
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0060c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00610
			 0x55555555,                                                  // 4 random values                             /// 00614
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00618
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0061c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00620
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00624
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00628
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0062c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00630
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00634
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00638
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0063c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00644
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00648
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0064c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00650
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00654
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00658
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0065c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00660
			 0xbf028f5c,                                                  // -0.51                                       /// 00664
			 0x7fc00001,                                                  // signaling NaN                               /// 00668
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00670
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0067c
			 0xff800000,                                                  // -inf                                        /// 00680
			 0x0c400000,                                                  // Leading 1s:                                 /// 00684
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0c400000,                                                  // Leading 1s:                                 /// 0068c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00690
			 0x55555555,                                                  // 4 random values                             /// 00694
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0069c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0xbf028f5c,                                                  // -0.51                                       /// 006b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 006c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006f8
			 0xffc00001,                                                  // -signaling NaN                              /// 006fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00704
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00708
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0070c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x7f800000,                                                  // inf                                         /// 00714
			 0x00011111,                                                  // 9.7958E-41                                  /// 00718
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0071c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00720
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00728
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0072c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00730
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00734
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00738
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0073c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00740
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00748
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0074c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00750
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00754
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00758
			 0x3f028f5c,                                                  // 0.51                                        /// 0075c
			 0xbf028f5c,                                                  // -0.51                                       /// 00760
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00764
			 0x0c600000,                                                  // Leading 1s:                                 /// 00768
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00774
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00778
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00780
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00784
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00788
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0078c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00798
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0079c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007b4
			 0x80000000,                                                  // -zero                                       /// 007b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00800
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00808
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0080c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00810
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0c600000,                                                  // Leading 1s:                                 /// 00818
			 0xffc00001,                                                  // -signaling NaN                              /// 0081c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00820
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x0c700000,                                                  // Leading 1s:                                 /// 0082c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x00000000,                                                  // zero                                        /// 00834
			 0xcb000000,                                                  // -8388608.0                                  /// 00838
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0083c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00844
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00848
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00850
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x4b000000,                                                  // 8388608.0                                   /// 0085c
			 0x33333333,                                                  // 4 random values                             /// 00860
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00868
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0086c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00870
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00874
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00880
			 0x4b000000,                                                  // 8388608.0                                   /// 00884
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00890
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0xff800000,                                                  // -inf                                        /// 00898
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0089c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008cc
			 0x7fc00001,                                                  // signaling NaN                               /// 008d0
			 0x4b000000,                                                  // 8388608.0                                   /// 008d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008f0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00904
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0090c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00910
			 0x80000000,                                                  // -zero                                       /// 00914
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00918
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00920
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00924
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00928
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0092c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00930
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00940
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00944
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0094c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00960
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00964
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00984
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0098c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00990
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00994
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0099c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a0c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a24
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a38
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a44
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a54
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a58
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a64
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a68
			 0x00000000,                                                  // zero                                        /// 00a6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a70
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a80
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a84
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a90
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a94
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00aa8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ab4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ac4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ac8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00adc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ae0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ae8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00af0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00afc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b00
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b08
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b1c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b2c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b34
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80000000,                                                  // -zero                                       /// 00b3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b48
			 0xcb000000,                                                  // -8388608.0                                  /// 00b4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b58
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b70
			 0xbf028f5c,                                                  // -0.51                                       /// 00b74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b78
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b94
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ba0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ba8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bb0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bb8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bbc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bc8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bcc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bd0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bd4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bd8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bdc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00be0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00be4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00be8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bf0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bf4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bf8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bfc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c04
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c28
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c30
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x80000000,                                                  // -zero                                       /// 00c48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c4c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c54
			 0x33333333,                                                  // 4 random values                             /// 00c58
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c60
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c64
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c6c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c70
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c98
			 0xff800000,                                                  // -inf                                        /// 00c9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ca0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cb0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cb4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cb8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cbc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cc4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cc8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ccc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00cd8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cdc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ce0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ce4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ce8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cf0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cf4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cf8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d14
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d20
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d34
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d50
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d54
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d58
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d64
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d68
			 0xbf028f5c,                                                  // -0.51                                       /// 00d6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d70
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d7c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d80
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d8c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d98
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00da0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00da4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00db0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00db8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dbc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dc4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dcc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dd8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00de0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00de4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00df0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00df4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e10
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e1c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e2c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e50
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e54
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e60
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e64
			 0x7f800000,                                                  // inf                                         /// 00e68
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e74
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e78
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e7c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e88
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e8c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x80000000,                                                  // -zero                                       /// 00ea0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ea4
			 0x7f800000,                                                  // inf                                         /// 00ea8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00eac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00eb4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eb8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ebc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ec0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ec8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ed4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ed8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00edc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ee4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ee8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ef4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ef8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0xcb000000,                                                  // -8388608.0                                  /// 00f00
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f14
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f18
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f2c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f38
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f48
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f4c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f74
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f78
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f8c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f90
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f94
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f9c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fa0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fa4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fa8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fb0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fb4
			 0x4b000000,                                                  // 8388608.0                                   /// 00fb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fd0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fd4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fd8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fdc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fe0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ff0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ff4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ff8
			 0x0c7ffff0                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x9def7b29, /// 0x0
			 0xc852abd4, /// 0x4
			 0xd7a4fec6, /// 0x8
			 0x7a41782e, /// 0xc
			 0xd33dbd3a, /// 0x10
			 0xb2738156, /// 0x14
			 0xb4660592, /// 0x18
			 0x4925c142, /// 0x1c
			 0x7638ded6, /// 0x20
			 0x29e02f62, /// 0x24
			 0xdbe30d3f, /// 0x28
			 0x9ad94499, /// 0x2c
			 0xec6381d2, /// 0x30
			 0xe351cffb, /// 0x34
			 0x385efc3f, /// 0x38
			 0xc1e1f0d1, /// 0x3c
			 0xfa3189d8, /// 0x40
			 0x33ff71e9, /// 0x44
			 0xad06c951, /// 0x48
			 0xb995e978, /// 0x4c
			 0xf52ae9f8, /// 0x50
			 0x6555003b, /// 0x54
			 0x1950d060, /// 0x58
			 0x27181b4c, /// 0x5c
			 0x9584555c, /// 0x60
			 0x4a661421, /// 0x64
			 0x1a20d253, /// 0x68
			 0x9988e820, /// 0x6c
			 0xa1bc1730, /// 0x70
			 0x9732f8c4, /// 0x74
			 0x41087dcc, /// 0x78
			 0x12316578, /// 0x7c
			 0x6880abff, /// 0x80
			 0x3d2ff266, /// 0x84
			 0xe3c3dabf, /// 0x88
			 0x2fd659e0, /// 0x8c
			 0xd2108bdf, /// 0x90
			 0x435d7706, /// 0x94
			 0x3d597e53, /// 0x98
			 0x52a46a5f, /// 0x9c
			 0xaddd3004, /// 0xa0
			 0x36269f0f, /// 0xa4
			 0x316cd106, /// 0xa8
			 0x267150f8, /// 0xac
			 0xb4485884, /// 0xb0
			 0xe2a7801f, /// 0xb4
			 0x894b92b0, /// 0xb8
			 0x3e44ba81, /// 0xbc
			 0x5b58dd46, /// 0xc0
			 0x0b447ca6, /// 0xc4
			 0x135ac64f, /// 0xc8
			 0xfa7e0a2a, /// 0xcc
			 0x533250b7, /// 0xd0
			 0x94d6dafc, /// 0xd4
			 0xdb7a781d, /// 0xd8
			 0xdf1b0233, /// 0xdc
			 0xb83a5375, /// 0xe0
			 0x65abb9be, /// 0xe4
			 0xb370c0a0, /// 0xe8
			 0x0ebad7e6, /// 0xec
			 0x597e1240, /// 0xf0
			 0x17369c65, /// 0xf4
			 0xeb025ca3, /// 0xf8
			 0x0c1a6c81, /// 0xfc
			 0xaa9a7d83, /// 0x100
			 0xcce74112, /// 0x104
			 0xfc26672b, /// 0x108
			 0x655cc74f, /// 0x10c
			 0xcbab7179, /// 0x110
			 0x7ee66f4f, /// 0x114
			 0x19642c19, /// 0x118
			 0x40719d4d, /// 0x11c
			 0x2a877332, /// 0x120
			 0x475a4c57, /// 0x124
			 0x6e69fa7c, /// 0x128
			 0xf308df58, /// 0x12c
			 0x16b67abe, /// 0x130
			 0x4c45a8f9, /// 0x134
			 0x6cd68e73, /// 0x138
			 0x3b42e586, /// 0x13c
			 0xe1377d8c, /// 0x140
			 0x639a296e, /// 0x144
			 0x136b2f27, /// 0x148
			 0xb4e1b579, /// 0x14c
			 0xa2728277, /// 0x150
			 0x9e4f7326, /// 0x154
			 0xae36c18a, /// 0x158
			 0x51aa9fb3, /// 0x15c
			 0xb99333eb, /// 0x160
			 0x6f5329c6, /// 0x164
			 0x08dd1d77, /// 0x168
			 0x56d4428c, /// 0x16c
			 0x7b1eacdf, /// 0x170
			 0xff757cd7, /// 0x174
			 0x5d7a4512, /// 0x178
			 0xa1734ba3, /// 0x17c
			 0x5d6e24a6, /// 0x180
			 0x46ef7bb9, /// 0x184
			 0xfc277184, /// 0x188
			 0x2f2e8a1b, /// 0x18c
			 0x856b9889, /// 0x190
			 0x0dde38bc, /// 0x194
			 0xbb1004eb, /// 0x198
			 0xe2dc1134, /// 0x19c
			 0x5af36585, /// 0x1a0
			 0xbfd3f079, /// 0x1a4
			 0x5602ccda, /// 0x1a8
			 0x897619e5, /// 0x1ac
			 0x643e3f67, /// 0x1b0
			 0xecac71b4, /// 0x1b4
			 0xff5bd4a8, /// 0x1b8
			 0xef671ba7, /// 0x1bc
			 0x8b4244dd, /// 0x1c0
			 0xf52b30ef, /// 0x1c4
			 0xa5b7fdd8, /// 0x1c8
			 0xba49c414, /// 0x1cc
			 0xcee35fdf, /// 0x1d0
			 0xcaee8d36, /// 0x1d4
			 0x2023e825, /// 0x1d8
			 0x620a106d, /// 0x1dc
			 0xdb3ed499, /// 0x1e0
			 0xecaf00d5, /// 0x1e4
			 0x598737da, /// 0x1e8
			 0xf27b8385, /// 0x1ec
			 0x04ad44a1, /// 0x1f0
			 0x1498fc2f, /// 0x1f4
			 0xd60f8341, /// 0x1f8
			 0x4046bef7, /// 0x1fc
			 0xf19ea3a1, /// 0x200
			 0x868ce07c, /// 0x204
			 0x18ff5710, /// 0x208
			 0xe796874d, /// 0x20c
			 0xc7ed87b9, /// 0x210
			 0xd9ae4600, /// 0x214
			 0xa72a1861, /// 0x218
			 0x86d9a10a, /// 0x21c
			 0xf25b163d, /// 0x220
			 0xd0e53894, /// 0x224
			 0xd4330af3, /// 0x228
			 0xefc9c9b0, /// 0x22c
			 0x3e6c3802, /// 0x230
			 0x265013a7, /// 0x234
			 0x6f64831f, /// 0x238
			 0xa827643e, /// 0x23c
			 0x9ed6bbcc, /// 0x240
			 0xbbaa7154, /// 0x244
			 0xa8ae849c, /// 0x248
			 0x384f3a50, /// 0x24c
			 0x3721577d, /// 0x250
			 0xa4f27eca, /// 0x254
			 0xf3495e41, /// 0x258
			 0x27011099, /// 0x25c
			 0x92f29e57, /// 0x260
			 0xa361c502, /// 0x264
			 0xdd84e0c5, /// 0x268
			 0x45b65778, /// 0x26c
			 0xf1a35d6e, /// 0x270
			 0xb929cc27, /// 0x274
			 0xccf32d74, /// 0x278
			 0xd66426c6, /// 0x27c
			 0x3b0d9618, /// 0x280
			 0x97929837, /// 0x284
			 0x99cb22f1, /// 0x288
			 0x6a81ba91, /// 0x28c
			 0x2d656a93, /// 0x290
			 0x5d733870, /// 0x294
			 0xad966c5c, /// 0x298
			 0x8a51ac55, /// 0x29c
			 0xb29fd76e, /// 0x2a0
			 0x8a4581e6, /// 0x2a4
			 0xbd371336, /// 0x2a8
			 0x6f4083b3, /// 0x2ac
			 0x666a9fed, /// 0x2b0
			 0x50397fb7, /// 0x2b4
			 0x9b3c7df3, /// 0x2b8
			 0x9b698b1a, /// 0x2bc
			 0x9b949673, /// 0x2c0
			 0x03e02570, /// 0x2c4
			 0xd4ff635b, /// 0x2c8
			 0x9d9cdfdd, /// 0x2cc
			 0x4686dc64, /// 0x2d0
			 0x833db052, /// 0x2d4
			 0xc782e472, /// 0x2d8
			 0xb54c94aa, /// 0x2dc
			 0x083389b2, /// 0x2e0
			 0xf86f8920, /// 0x2e4
			 0xcd4d3680, /// 0x2e8
			 0x4e5c9ed6, /// 0x2ec
			 0x86b1886a, /// 0x2f0
			 0x511d22f5, /// 0x2f4
			 0xebdbacd0, /// 0x2f8
			 0x48a389d4, /// 0x2fc
			 0x4bd55f64, /// 0x300
			 0x650c49af, /// 0x304
			 0x465a861c, /// 0x308
			 0x47976eb4, /// 0x30c
			 0xd89551cb, /// 0x310
			 0x440e830a, /// 0x314
			 0xa7e843c7, /// 0x318
			 0x7bffc267, /// 0x31c
			 0x364d3179, /// 0x320
			 0xedfd8ae1, /// 0x324
			 0x75c2c03a, /// 0x328
			 0xb00e8403, /// 0x32c
			 0xdad5e21b, /// 0x330
			 0x956ba09a, /// 0x334
			 0x838b952d, /// 0x338
			 0xdb8136e4, /// 0x33c
			 0x138dad9c, /// 0x340
			 0xbe3f60fb, /// 0x344
			 0x9283c6b3, /// 0x348
			 0xdcd58769, /// 0x34c
			 0x507bce7f, /// 0x350
			 0x0b87dc3e, /// 0x354
			 0x555833a1, /// 0x358
			 0xc46b98a5, /// 0x35c
			 0xa83a7a69, /// 0x360
			 0x48ed9efc, /// 0x364
			 0x93914a23, /// 0x368
			 0xc7ac4821, /// 0x36c
			 0xb1f5bb9d, /// 0x370
			 0xd04c7fbd, /// 0x374
			 0x60917ba7, /// 0x378
			 0x9649223f, /// 0x37c
			 0xc93145ea, /// 0x380
			 0xdb62613c, /// 0x384
			 0xa742a1bc, /// 0x388
			 0x85609483, /// 0x38c
			 0xb197a41a, /// 0x390
			 0x4885f1a3, /// 0x394
			 0x9869ab35, /// 0x398
			 0x4e6e27c6, /// 0x39c
			 0xac736484, /// 0x3a0
			 0x12e319b8, /// 0x3a4
			 0x1782583e, /// 0x3a8
			 0x4f66a2c8, /// 0x3ac
			 0x9f665c5f, /// 0x3b0
			 0x32225629, /// 0x3b4
			 0xa42c9186, /// 0x3b8
			 0x9a442be3, /// 0x3bc
			 0x23d34b62, /// 0x3c0
			 0x0cd99fcb, /// 0x3c4
			 0x2d4a19dc, /// 0x3c8
			 0xefbc1e9d, /// 0x3cc
			 0x4227e40d, /// 0x3d0
			 0x8174fd31, /// 0x3d4
			 0xd4e578ce, /// 0x3d8
			 0xa2fc00ff, /// 0x3dc
			 0x4876f70d, /// 0x3e0
			 0xf1f6d9f2, /// 0x3e4
			 0x8c23abb4, /// 0x3e8
			 0x64e67baa, /// 0x3ec
			 0x43d537f4, /// 0x3f0
			 0xbd9d1b07, /// 0x3f4
			 0xcdd4c154, /// 0x3f8
			 0xded788fc, /// 0x3fc
			 0xf828eaca, /// 0x400
			 0xc159cf98, /// 0x404
			 0xa0dc68f9, /// 0x408
			 0x12c8c44e, /// 0x40c
			 0x9cbf46f9, /// 0x410
			 0xbe520184, /// 0x414
			 0x45200ffa, /// 0x418
			 0xbc675760, /// 0x41c
			 0x0c3046d5, /// 0x420
			 0xfe5aef7d, /// 0x424
			 0xf03ad6a0, /// 0x428
			 0xfd7549e7, /// 0x42c
			 0x183c1944, /// 0x430
			 0x70c94e9b, /// 0x434
			 0xa3e5bf0b, /// 0x438
			 0xf03e008e, /// 0x43c
			 0x94d50b24, /// 0x440
			 0xb29a4828, /// 0x444
			 0x0ce7ff10, /// 0x448
			 0x48daced7, /// 0x44c
			 0x152ae393, /// 0x450
			 0xa66425f1, /// 0x454
			 0xb04a7316, /// 0x458
			 0x80c305df, /// 0x45c
			 0xb21c348c, /// 0x460
			 0xbb083e45, /// 0x464
			 0xdd63d630, /// 0x468
			 0x9d6b0312, /// 0x46c
			 0xc476dfed, /// 0x470
			 0x4ba6c30d, /// 0x474
			 0x2bac8c0d, /// 0x478
			 0x0faf46a4, /// 0x47c
			 0x71ea0fa5, /// 0x480
			 0xcbc02bf9, /// 0x484
			 0xb96b745c, /// 0x488
			 0x5e695c5b, /// 0x48c
			 0x57e995e1, /// 0x490
			 0x08ff3819, /// 0x494
			 0x9ebe2d5a, /// 0x498
			 0xa5817eac, /// 0x49c
			 0xcabb0432, /// 0x4a0
			 0x1aa6738e, /// 0x4a4
			 0xce292b15, /// 0x4a8
			 0xe67682f9, /// 0x4ac
			 0xc3c21578, /// 0x4b0
			 0xcdbd2fab, /// 0x4b4
			 0x5b758faf, /// 0x4b8
			 0xc44570cb, /// 0x4bc
			 0xb2086901, /// 0x4c0
			 0xc349cd82, /// 0x4c4
			 0x2ff9c470, /// 0x4c8
			 0xdfd0358e, /// 0x4cc
			 0x1392abaf, /// 0x4d0
			 0x095ffef4, /// 0x4d4
			 0x12509c25, /// 0x4d8
			 0x2abbcfd9, /// 0x4dc
			 0x04027185, /// 0x4e0
			 0x3e8a704d, /// 0x4e4
			 0x7d4231c9, /// 0x4e8
			 0xa1062f87, /// 0x4ec
			 0xbb1e79ca, /// 0x4f0
			 0x9922c834, /// 0x4f4
			 0xf95ab293, /// 0x4f8
			 0x629a057f, /// 0x4fc
			 0xb621f7fb, /// 0x500
			 0x40d27d39, /// 0x504
			 0xde0ec8fd, /// 0x508
			 0x23f55c00, /// 0x50c
			 0xd1e8cab2, /// 0x510
			 0x55be5ebe, /// 0x514
			 0xe67e49c0, /// 0x518
			 0xfcdce0f8, /// 0x51c
			 0x165c94d8, /// 0x520
			 0xa68b6529, /// 0x524
			 0x7d0b7e83, /// 0x528
			 0x5250e287, /// 0x52c
			 0x58a7d45b, /// 0x530
			 0x612a87b7, /// 0x534
			 0x2beea8d7, /// 0x538
			 0x20f30103, /// 0x53c
			 0x97352377, /// 0x540
			 0x13a8f2c7, /// 0x544
			 0xf8568f69, /// 0x548
			 0xc243fc18, /// 0x54c
			 0x3d9d3d99, /// 0x550
			 0xe4ade304, /// 0x554
			 0xad14d060, /// 0x558
			 0x916883dd, /// 0x55c
			 0x41f4f9bd, /// 0x560
			 0x73467b78, /// 0x564
			 0x3dff89ac, /// 0x568
			 0xb08052fc, /// 0x56c
			 0x1357a881, /// 0x570
			 0xc271a284, /// 0x574
			 0x8a99f17c, /// 0x578
			 0x1440bb18, /// 0x57c
			 0x6c627e27, /// 0x580
			 0xc9288868, /// 0x584
			 0x7036e8dd, /// 0x588
			 0xdd0fecd7, /// 0x58c
			 0xb5f8effd, /// 0x590
			 0xd8394f38, /// 0x594
			 0x80012933, /// 0x598
			 0xf54c3206, /// 0x59c
			 0x2fc7605a, /// 0x5a0
			 0x1e92e7a9, /// 0x5a4
			 0x16f76c35, /// 0x5a8
			 0x57173fb8, /// 0x5ac
			 0x56bcf40e, /// 0x5b0
			 0x46c9c2b0, /// 0x5b4
			 0x564f69d4, /// 0x5b8
			 0xe57c2abb, /// 0x5bc
			 0x0405d945, /// 0x5c0
			 0xa57ed0d3, /// 0x5c4
			 0x6aa28a76, /// 0x5c8
			 0x5cd5b21d, /// 0x5cc
			 0xc459da9f, /// 0x5d0
			 0xc6de6f10, /// 0x5d4
			 0xbd40015f, /// 0x5d8
			 0x718d7d6d, /// 0x5dc
			 0x05050d93, /// 0x5e0
			 0x7d4d8549, /// 0x5e4
			 0x752f3be2, /// 0x5e8
			 0x45ef14a6, /// 0x5ec
			 0x811cbbf8, /// 0x5f0
			 0x1d20632a, /// 0x5f4
			 0x3fee4a66, /// 0x5f8
			 0x54763069, /// 0x5fc
			 0x0656ca51, /// 0x600
			 0xdb22233c, /// 0x604
			 0x5a8a01bc, /// 0x608
			 0x4c3952ef, /// 0x60c
			 0xd3c90154, /// 0x610
			 0x97885606, /// 0x614
			 0xdad2eec5, /// 0x618
			 0x9faba275, /// 0x61c
			 0x16c9e8c0, /// 0x620
			 0x393e4950, /// 0x624
			 0xb8e24d09, /// 0x628
			 0x50c9262a, /// 0x62c
			 0x946f7f65, /// 0x630
			 0xff95bed4, /// 0x634
			 0x5973109c, /// 0x638
			 0xcaab66cb, /// 0x63c
			 0xcd477183, /// 0x640
			 0xbde6f074, /// 0x644
			 0x304278f0, /// 0x648
			 0x54323db0, /// 0x64c
			 0xf1699ab7, /// 0x650
			 0x1d4a1df0, /// 0x654
			 0x639bbb10, /// 0x658
			 0xae1dda99, /// 0x65c
			 0xcec7f903, /// 0x660
			 0xd95a49d2, /// 0x664
			 0xd6a7542e, /// 0x668
			 0xe262781f, /// 0x66c
			 0xb363a660, /// 0x670
			 0xc913d570, /// 0x674
			 0x3df71778, /// 0x678
			 0xfe67e1cf, /// 0x67c
			 0xa293018a, /// 0x680
			 0x80c932fa, /// 0x684
			 0xaa452d75, /// 0x688
			 0xc9dd153e, /// 0x68c
			 0xa14d09c1, /// 0x690
			 0xe985f054, /// 0x694
			 0x44184ba3, /// 0x698
			 0x8e2c04b4, /// 0x69c
			 0x30870502, /// 0x6a0
			 0x3a754ec1, /// 0x6a4
			 0x4cc7442d, /// 0x6a8
			 0x4a393a07, /// 0x6ac
			 0x57350a16, /// 0x6b0
			 0xff382031, /// 0x6b4
			 0x30a16411, /// 0x6b8
			 0x94cf6516, /// 0x6bc
			 0x609368b7, /// 0x6c0
			 0xcfa02395, /// 0x6c4
			 0x4b0d8c0b, /// 0x6c8
			 0xfd27ea90, /// 0x6cc
			 0xcf96d1a2, /// 0x6d0
			 0x8154bc1e, /// 0x6d4
			 0xf7d4a984, /// 0x6d8
			 0x782fdd31, /// 0x6dc
			 0x23b82566, /// 0x6e0
			 0x958b4e0e, /// 0x6e4
			 0x4843e597, /// 0x6e8
			 0xe4926392, /// 0x6ec
			 0x1597cd4f, /// 0x6f0
			 0x9e3c3853, /// 0x6f4
			 0xcaf77167, /// 0x6f8
			 0x0503ec89, /// 0x6fc
			 0x4bb1c9f6, /// 0x700
			 0xbf4db279, /// 0x704
			 0xc79faf02, /// 0x708
			 0x08ffedfd, /// 0x70c
			 0x3f52e470, /// 0x710
			 0xa666bbe4, /// 0x714
			 0xcbdce832, /// 0x718
			 0x2801f2bf, /// 0x71c
			 0x8fbca378, /// 0x720
			 0x0b060380, /// 0x724
			 0x53e6a043, /// 0x728
			 0x434371db, /// 0x72c
			 0x4206b89a, /// 0x730
			 0xc4c522de, /// 0x734
			 0xadf1e255, /// 0x738
			 0xc0eb21fd, /// 0x73c
			 0xea16a813, /// 0x740
			 0xd1fd8990, /// 0x744
			 0x4b23aec0, /// 0x748
			 0x4796f0ed, /// 0x74c
			 0xc1b63042, /// 0x750
			 0x4d72b6ab, /// 0x754
			 0xf577b983, /// 0x758
			 0x337bcd8d, /// 0x75c
			 0xb0eeb333, /// 0x760
			 0x3d6fec8b, /// 0x764
			 0x040dc1c4, /// 0x768
			 0x89bdccb4, /// 0x76c
			 0x694ccde6, /// 0x770
			 0x7b22286a, /// 0x774
			 0x3229362f, /// 0x778
			 0x3101672e, /// 0x77c
			 0x4baa8fef, /// 0x780
			 0x356b62af, /// 0x784
			 0xffff4db4, /// 0x788
			 0x283aef2d, /// 0x78c
			 0x13ef1307, /// 0x790
			 0xebbe4c17, /// 0x794
			 0xcc14abb5, /// 0x798
			 0x06983c3c, /// 0x79c
			 0x22ee642b, /// 0x7a0
			 0x3d7b6e04, /// 0x7a4
			 0x9d206626, /// 0x7a8
			 0x5665a28d, /// 0x7ac
			 0xd3465f46, /// 0x7b0
			 0x461409d2, /// 0x7b4
			 0xa0e04c51, /// 0x7b8
			 0xa11264dc, /// 0x7bc
			 0x0521d7b4, /// 0x7c0
			 0xf4c18774, /// 0x7c4
			 0x2ec6197b, /// 0x7c8
			 0x1e6b2b03, /// 0x7cc
			 0x3ab1045e, /// 0x7d0
			 0xea53ac31, /// 0x7d4
			 0xa1d591a5, /// 0x7d8
			 0x5ae7c801, /// 0x7dc
			 0xd0f6db13, /// 0x7e0
			 0x85a9de31, /// 0x7e4
			 0x341aeee3, /// 0x7e8
			 0x69a30353, /// 0x7ec
			 0x731a1905, /// 0x7f0
			 0x38da99cf, /// 0x7f4
			 0xace1a8af, /// 0x7f8
			 0xd79a0d43, /// 0x7fc
			 0xed967406, /// 0x800
			 0xb91c0e51, /// 0x804
			 0x387d7995, /// 0x808
			 0xc47d5e17, /// 0x80c
			 0x3855db6a, /// 0x810
			 0x73b34f5d, /// 0x814
			 0x92cbe529, /// 0x818
			 0x86a4f980, /// 0x81c
			 0xd7264be3, /// 0x820
			 0xfb422b15, /// 0x824
			 0xb9c4a496, /// 0x828
			 0xad657f04, /// 0x82c
			 0xa725c28f, /// 0x830
			 0xb8234457, /// 0x834
			 0x454ae5f0, /// 0x838
			 0x431273bd, /// 0x83c
			 0x39cade2c, /// 0x840
			 0x990a8cb4, /// 0x844
			 0x6ccc7754, /// 0x848
			 0x3b64e382, /// 0x84c
			 0xe7c4d584, /// 0x850
			 0xaa308f03, /// 0x854
			 0x1a6a107f, /// 0x858
			 0x4ecc3a27, /// 0x85c
			 0x411c4b82, /// 0x860
			 0xeaa04cf4, /// 0x864
			 0x74a33d2e, /// 0x868
			 0xb2eda99f, /// 0x86c
			 0xc5175eeb, /// 0x870
			 0x9c58cf04, /// 0x874
			 0x2a3b7dae, /// 0x878
			 0xe6534a14, /// 0x87c
			 0x12d60866, /// 0x880
			 0x03d3cb37, /// 0x884
			 0xdf616673, /// 0x888
			 0x6b9fe88f, /// 0x88c
			 0xa0e3cd01, /// 0x890
			 0x5c53a90e, /// 0x894
			 0x85dbcf25, /// 0x898
			 0xc84da0ed, /// 0x89c
			 0xfe35ac4d, /// 0x8a0
			 0xf3f8ee00, /// 0x8a4
			 0xdd882d2f, /// 0x8a8
			 0x4b3d2228, /// 0x8ac
			 0x2a5eb0a1, /// 0x8b0
			 0xb8c25e60, /// 0x8b4
			 0x5e2b15ac, /// 0x8b8
			 0xe9556c14, /// 0x8bc
			 0x44878b9b, /// 0x8c0
			 0x0f95569a, /// 0x8c4
			 0x9a29069e, /// 0x8c8
			 0x432d0cfe, /// 0x8cc
			 0x3aec9830, /// 0x8d0
			 0x82ff94d0, /// 0x8d4
			 0xa7c3be53, /// 0x8d8
			 0xb5d12731, /// 0x8dc
			 0x09f73db6, /// 0x8e0
			 0x1cdea062, /// 0x8e4
			 0x27bc8555, /// 0x8e8
			 0x382f490d, /// 0x8ec
			 0x06fa60e5, /// 0x8f0
			 0x50f68eba, /// 0x8f4
			 0x284528ca, /// 0x8f8
			 0xaa9513e9, /// 0x8fc
			 0x60f02d34, /// 0x900
			 0x0725cfa7, /// 0x904
			 0x2ed3d617, /// 0x908
			 0x6d6f6ab8, /// 0x90c
			 0xf61e77d0, /// 0x910
			 0x9ed2ebbd, /// 0x914
			 0xe40331ae, /// 0x918
			 0x6f36b0ea, /// 0x91c
			 0x2f089add, /// 0x920
			 0x77ff7eb6, /// 0x924
			 0x41db7655, /// 0x928
			 0xca4b2513, /// 0x92c
			 0x19601c62, /// 0x930
			 0x54edea37, /// 0x934
			 0x34c3d556, /// 0x938
			 0xe4785505, /// 0x93c
			 0x1a8cb0f0, /// 0x940
			 0xb364a9cb, /// 0x944
			 0xbb8c2c27, /// 0x948
			 0x14c85b30, /// 0x94c
			 0x7ec8fcd1, /// 0x950
			 0x9128c3db, /// 0x954
			 0x6c29dac1, /// 0x958
			 0xa9beeeb8, /// 0x95c
			 0x366fd101, /// 0x960
			 0x507695c2, /// 0x964
			 0x00566295, /// 0x968
			 0xee1dc17c, /// 0x96c
			 0xf22ee372, /// 0x970
			 0x659c2518, /// 0x974
			 0x03329bad, /// 0x978
			 0x91ca7b88, /// 0x97c
			 0x979d87c2, /// 0x980
			 0xec15490b, /// 0x984
			 0xfa84c029, /// 0x988
			 0xa0f8e5f3, /// 0x98c
			 0xf1e172af, /// 0x990
			 0xdb636940, /// 0x994
			 0x5ff80466, /// 0x998
			 0x3f1ddc03, /// 0x99c
			 0xf316b0d2, /// 0x9a0
			 0x00dbd6c5, /// 0x9a4
			 0x4dc70376, /// 0x9a8
			 0x8680af08, /// 0x9ac
			 0xa9788e62, /// 0x9b0
			 0xfb6a0a57, /// 0x9b4
			 0x95d282ce, /// 0x9b8
			 0x031d8b7c, /// 0x9bc
			 0x6845a721, /// 0x9c0
			 0x269052f5, /// 0x9c4
			 0x2625b4bb, /// 0x9c8
			 0xdf851893, /// 0x9cc
			 0xba1a3b00, /// 0x9d0
			 0xa7737eab, /// 0x9d4
			 0x5d176a5d, /// 0x9d8
			 0x29f5f7b4, /// 0x9dc
			 0x1f016499, /// 0x9e0
			 0x2cea89af, /// 0x9e4
			 0x3d20df2f, /// 0x9e8
			 0xd0d392ea, /// 0x9ec
			 0xefb024a8, /// 0x9f0
			 0xa0da60b5, /// 0x9f4
			 0x60e13121, /// 0x9f8
			 0x3c9f83e5, /// 0x9fc
			 0xd8956225, /// 0xa00
			 0x8e7e4bff, /// 0xa04
			 0xaf091d65, /// 0xa08
			 0x150d0d2f, /// 0xa0c
			 0x2879f31f, /// 0xa10
			 0x3bcbdee4, /// 0xa14
			 0xe19c7a3c, /// 0xa18
			 0xd33aef20, /// 0xa1c
			 0xe8198279, /// 0xa20
			 0x2af6afee, /// 0xa24
			 0xf8006832, /// 0xa28
			 0x299b8fe1, /// 0xa2c
			 0x69e64429, /// 0xa30
			 0x22ed01dd, /// 0xa34
			 0x7238480d, /// 0xa38
			 0xb454f073, /// 0xa3c
			 0xb5b3211b, /// 0xa40
			 0x7385721e, /// 0xa44
			 0xa2658b07, /// 0xa48
			 0xebfcaaea, /// 0xa4c
			 0x0b0a3017, /// 0xa50
			 0x197e4e2e, /// 0xa54
			 0x7e8a6126, /// 0xa58
			 0x09eb1453, /// 0xa5c
			 0xee99a3aa, /// 0xa60
			 0x16236504, /// 0xa64
			 0x6ee18fe3, /// 0xa68
			 0x1cba29c5, /// 0xa6c
			 0x1480af81, /// 0xa70
			 0x11f28878, /// 0xa74
			 0x64bc5724, /// 0xa78
			 0x92206526, /// 0xa7c
			 0xd966edbd, /// 0xa80
			 0x560346e2, /// 0xa84
			 0x014ba534, /// 0xa88
			 0x309e1023, /// 0xa8c
			 0xcd3587b4, /// 0xa90
			 0x66225746, /// 0xa94
			 0x24fedc77, /// 0xa98
			 0x8e625c80, /// 0xa9c
			 0x924187ce, /// 0xaa0
			 0x25db8777, /// 0xaa4
			 0x2ff858b0, /// 0xaa8
			 0xd5e47fae, /// 0xaac
			 0x8e874e61, /// 0xab0
			 0x676f35d8, /// 0xab4
			 0xd9a9abb8, /// 0xab8
			 0xab0e67cd, /// 0xabc
			 0xb8d532df, /// 0xac0
			 0x71748199, /// 0xac4
			 0xee66d967, /// 0xac8
			 0x8c193395, /// 0xacc
			 0x252a617f, /// 0xad0
			 0xe324a10a, /// 0xad4
			 0xbc633d5c, /// 0xad8
			 0xf6df2452, /// 0xadc
			 0x3a1b29b4, /// 0xae0
			 0x98ca26c5, /// 0xae4
			 0x0a1297ad, /// 0xae8
			 0x57978bd1, /// 0xaec
			 0x32ce77fe, /// 0xaf0
			 0xf763de7a, /// 0xaf4
			 0x80c0cb46, /// 0xaf8
			 0x2b43429d, /// 0xafc
			 0x780a09c7, /// 0xb00
			 0x86bd05ab, /// 0xb04
			 0x485c2b5b, /// 0xb08
			 0x336bf82b, /// 0xb0c
			 0xe2d9559c, /// 0xb10
			 0x6aa4f995, /// 0xb14
			 0x42c99625, /// 0xb18
			 0x9494a7eb, /// 0xb1c
			 0x444db080, /// 0xb20
			 0x1cf37ef2, /// 0xb24
			 0x52b332bb, /// 0xb28
			 0x3343ff84, /// 0xb2c
			 0xdc74af90, /// 0xb30
			 0xfdc4965d, /// 0xb34
			 0x1667837d, /// 0xb38
			 0xd062afad, /// 0xb3c
			 0x164b36ed, /// 0xb40
			 0x76d25d9a, /// 0xb44
			 0x13a9924b, /// 0xb48
			 0x1edffa9c, /// 0xb4c
			 0xd0778d48, /// 0xb50
			 0x991b280d, /// 0xb54
			 0x471b89ad, /// 0xb58
			 0x25f3f67e, /// 0xb5c
			 0xb2998e69, /// 0xb60
			 0xdbe1deb0, /// 0xb64
			 0x411ec160, /// 0xb68
			 0xcbade1fc, /// 0xb6c
			 0xe77b3282, /// 0xb70
			 0x0fa9cd79, /// 0xb74
			 0x6c5787e7, /// 0xb78
			 0xca195cf1, /// 0xb7c
			 0x22000bbb, /// 0xb80
			 0xaf270c23, /// 0xb84
			 0x2b18ede3, /// 0xb88
			 0x1553faf9, /// 0xb8c
			 0x265d550c, /// 0xb90
			 0x9b7865d5, /// 0xb94
			 0x640e1cf1, /// 0xb98
			 0xf5d18420, /// 0xb9c
			 0x26aeee5a, /// 0xba0
			 0x1edb78f5, /// 0xba4
			 0xd3f7873b, /// 0xba8
			 0x8af00754, /// 0xbac
			 0x53d24a30, /// 0xbb0
			 0x9670c226, /// 0xbb4
			 0x92f6abcf, /// 0xbb8
			 0xfaf37d62, /// 0xbbc
			 0x40a608bc, /// 0xbc0
			 0xcdfaf5e3, /// 0xbc4
			 0x89906eb6, /// 0xbc8
			 0x9806d7b0, /// 0xbcc
			 0xa271ec0d, /// 0xbd0
			 0xd245893c, /// 0xbd4
			 0xb0e51e69, /// 0xbd8
			 0xc21b403f, /// 0xbdc
			 0x3e4cd267, /// 0xbe0
			 0x17b0aa52, /// 0xbe4
			 0x18ff4231, /// 0xbe8
			 0x6b6e47be, /// 0xbec
			 0xe185eba2, /// 0xbf0
			 0x75f67aed, /// 0xbf4
			 0xed2a728e, /// 0xbf8
			 0x892b04cd, /// 0xbfc
			 0x3f9b625f, /// 0xc00
			 0xab7f15a0, /// 0xc04
			 0xe0f7db53, /// 0xc08
			 0xba3da247, /// 0xc0c
			 0x97f7d6b5, /// 0xc10
			 0xc21408f5, /// 0xc14
			 0xca615e3b, /// 0xc18
			 0xf9731f87, /// 0xc1c
			 0x7a64c51d, /// 0xc20
			 0x0e7c74a0, /// 0xc24
			 0xb8c0e98f, /// 0xc28
			 0xe08a1677, /// 0xc2c
			 0xe9de41f4, /// 0xc30
			 0x8f863e81, /// 0xc34
			 0x8272bfbc, /// 0xc38
			 0x0c54270e, /// 0xc3c
			 0x7f8a8458, /// 0xc40
			 0x3ca89b1c, /// 0xc44
			 0x824d14cd, /// 0xc48
			 0x7213dfdb, /// 0xc4c
			 0x94129971, /// 0xc50
			 0xac4c8770, /// 0xc54
			 0x39dffc28, /// 0xc58
			 0x31da26f8, /// 0xc5c
			 0x1e25c444, /// 0xc60
			 0x25660205, /// 0xc64
			 0xfd1d7f8f, /// 0xc68
			 0xba3730ea, /// 0xc6c
			 0x5f6ad3a9, /// 0xc70
			 0xd5350588, /// 0xc74
			 0x7d8b6afc, /// 0xc78
			 0x9af3a140, /// 0xc7c
			 0x142ae27e, /// 0xc80
			 0x6c0794d2, /// 0xc84
			 0x920cd388, /// 0xc88
			 0x9d31f2ef, /// 0xc8c
			 0x15364559, /// 0xc90
			 0xf1fdb4ac, /// 0xc94
			 0x6523928f, /// 0xc98
			 0x04150a3f, /// 0xc9c
			 0x553730e7, /// 0xca0
			 0x28ee80a6, /// 0xca4
			 0x42aba178, /// 0xca8
			 0xec8ac190, /// 0xcac
			 0x700393d5, /// 0xcb0
			 0x4434d038, /// 0xcb4
			 0x7b1d0217, /// 0xcb8
			 0xfc5cf3c0, /// 0xcbc
			 0xef18d46a, /// 0xcc0
			 0xaa13fb2a, /// 0xcc4
			 0x673d3155, /// 0xcc8
			 0x113fde03, /// 0xccc
			 0xb086a954, /// 0xcd0
			 0x804be181, /// 0xcd4
			 0x72efc1cd, /// 0xcd8
			 0x5f76c4e4, /// 0xcdc
			 0x8f8eefd5, /// 0xce0
			 0x1eeb1a99, /// 0xce4
			 0xe73ee4c7, /// 0xce8
			 0xfba2d09f, /// 0xcec
			 0x7e8e4a40, /// 0xcf0
			 0x5b4353a5, /// 0xcf4
			 0x36105847, /// 0xcf8
			 0xee3945ed, /// 0xcfc
			 0xb1e8fadb, /// 0xd00
			 0x8896314e, /// 0xd04
			 0xefa8750a, /// 0xd08
			 0x34971460, /// 0xd0c
			 0x69b6147d, /// 0xd10
			 0x59425512, /// 0xd14
			 0x28517ebb, /// 0xd18
			 0xba6174da, /// 0xd1c
			 0x523b95f9, /// 0xd20
			 0xe9aa8c77, /// 0xd24
			 0x632c7ae8, /// 0xd28
			 0x1e9a9b71, /// 0xd2c
			 0x8287e442, /// 0xd30
			 0x2a2bc763, /// 0xd34
			 0xfe0dca8a, /// 0xd38
			 0x97e656a5, /// 0xd3c
			 0xd2456c76, /// 0xd40
			 0xa01774a6, /// 0xd44
			 0x07c78cf1, /// 0xd48
			 0xc7e1df10, /// 0xd4c
			 0x65e1df85, /// 0xd50
			 0xb0c486ac, /// 0xd54
			 0xe4e9abfe, /// 0xd58
			 0xd65ec51d, /// 0xd5c
			 0xfc5202ed, /// 0xd60
			 0x4ead705f, /// 0xd64
			 0x3f32dac7, /// 0xd68
			 0xc787bd38, /// 0xd6c
			 0x4a2513f7, /// 0xd70
			 0x0af8ad40, /// 0xd74
			 0x3c0d2a6b, /// 0xd78
			 0xc341c75f, /// 0xd7c
			 0x8be230a6, /// 0xd80
			 0xf6a44003, /// 0xd84
			 0xbae29c84, /// 0xd88
			 0xcdad3456, /// 0xd8c
			 0x226b572b, /// 0xd90
			 0x6bee0b88, /// 0xd94
			 0x9ed2d0d1, /// 0xd98
			 0x404081ce, /// 0xd9c
			 0xbf52f747, /// 0xda0
			 0x2acd7a88, /// 0xda4
			 0x8849db63, /// 0xda8
			 0xb9efff15, /// 0xdac
			 0x5d387f3d, /// 0xdb0
			 0xa7ea52f8, /// 0xdb4
			 0x74c3a5fa, /// 0xdb8
			 0x39a56680, /// 0xdbc
			 0x6888789f, /// 0xdc0
			 0xdc66069f, /// 0xdc4
			 0xaf932aa4, /// 0xdc8
			 0x8c26ddbc, /// 0xdcc
			 0xfa12dd2f, /// 0xdd0
			 0x32690078, /// 0xdd4
			 0x37f401c2, /// 0xdd8
			 0xf5eb233a, /// 0xddc
			 0xe79fbf47, /// 0xde0
			 0xeaa44a05, /// 0xde4
			 0x40d81f76, /// 0xde8
			 0xf3a31c06, /// 0xdec
			 0x2408cbf9, /// 0xdf0
			 0xaa7a7715, /// 0xdf4
			 0xea661083, /// 0xdf8
			 0x71717c8a, /// 0xdfc
			 0x4b0186c5, /// 0xe00
			 0xa283134a, /// 0xe04
			 0x1614aff8, /// 0xe08
			 0xd3de7342, /// 0xe0c
			 0xe8df50ff, /// 0xe10
			 0x20fc1bd0, /// 0xe14
			 0x077871e8, /// 0xe18
			 0xccc46108, /// 0xe1c
			 0xc5d60b80, /// 0xe20
			 0xfc01f2c8, /// 0xe24
			 0x4d64e4a4, /// 0xe28
			 0x3418bf23, /// 0xe2c
			 0xc662fe18, /// 0xe30
			 0x6bf97c7c, /// 0xe34
			 0x8d146734, /// 0xe38
			 0x3a9542a3, /// 0xe3c
			 0x76549971, /// 0xe40
			 0xce61fab7, /// 0xe44
			 0x09413160, /// 0xe48
			 0xd5e41f3c, /// 0xe4c
			 0x07f9af08, /// 0xe50
			 0x871977e5, /// 0xe54
			 0xc1e6cb61, /// 0xe58
			 0xd71b4765, /// 0xe5c
			 0xaaf1bfe3, /// 0xe60
			 0xfa195255, /// 0xe64
			 0xe959a86b, /// 0xe68
			 0x814e04eb, /// 0xe6c
			 0x61a3cf92, /// 0xe70
			 0x4b89ae05, /// 0xe74
			 0x14b12e30, /// 0xe78
			 0x515b9e5e, /// 0xe7c
			 0xd6ce8d77, /// 0xe80
			 0x14bb1ec0, /// 0xe84
			 0x378016b5, /// 0xe88
			 0x15a094be, /// 0xe8c
			 0xcac223cc, /// 0xe90
			 0xcec4e620, /// 0xe94
			 0xbbbdee9a, /// 0xe98
			 0x975d92ba, /// 0xe9c
			 0x59d3346b, /// 0xea0
			 0x87aa93fd, /// 0xea4
			 0xa31b6d04, /// 0xea8
			 0x6876369c, /// 0xeac
			 0xbff0718f, /// 0xeb0
			 0xa3f3c6b9, /// 0xeb4
			 0x0d319d63, /// 0xeb8
			 0x3840d2a2, /// 0xebc
			 0x9a24b0d3, /// 0xec0
			 0xb2966d68, /// 0xec4
			 0x5ada1d71, /// 0xec8
			 0xc8fd9076, /// 0xecc
			 0x751b5428, /// 0xed0
			 0xb0925bd7, /// 0xed4
			 0x45b3fe83, /// 0xed8
			 0xfa30a026, /// 0xedc
			 0x512785e2, /// 0xee0
			 0x7e0fd2fa, /// 0xee4
			 0x8676f811, /// 0xee8
			 0x1fe30278, /// 0xeec
			 0x43488bd7, /// 0xef0
			 0x910100c8, /// 0xef4
			 0xa1a48b18, /// 0xef8
			 0x9f819469, /// 0xefc
			 0xf484f523, /// 0xf00
			 0x46ebfdef, /// 0xf04
			 0x77f1b40e, /// 0xf08
			 0x87e40d42, /// 0xf0c
			 0x67c85334, /// 0xf10
			 0x9146b6eb, /// 0xf14
			 0xf6b3aeb5, /// 0xf18
			 0x9bdab83b, /// 0xf1c
			 0xb474b880, /// 0xf20
			 0x5f8413aa, /// 0xf24
			 0x18a303e5, /// 0xf28
			 0x00313206, /// 0xf2c
			 0x821861b3, /// 0xf30
			 0x8fa502d7, /// 0xf34
			 0xd3599eb0, /// 0xf38
			 0x216597e2, /// 0xf3c
			 0x988be80a, /// 0xf40
			 0x2592fb12, /// 0xf44
			 0x057ef004, /// 0xf48
			 0x7db8d9f1, /// 0xf4c
			 0x14c4f7f7, /// 0xf50
			 0x85e41c13, /// 0xf54
			 0xae0c8677, /// 0xf58
			 0xf107867d, /// 0xf5c
			 0x30511874, /// 0xf60
			 0x3673fa56, /// 0xf64
			 0x4da951de, /// 0xf68
			 0x300b7ff1, /// 0xf6c
			 0x7ebf11f1, /// 0xf70
			 0x868d7b6d, /// 0xf74
			 0xd968fa61, /// 0xf78
			 0xbf0644e2, /// 0xf7c
			 0xfda73da3, /// 0xf80
			 0x136a55ec, /// 0xf84
			 0x028f241e, /// 0xf88
			 0xced6b7ad, /// 0xf8c
			 0x6403c03f, /// 0xf90
			 0x17524f6e, /// 0xf94
			 0xb6513afe, /// 0xf98
			 0xecb656d4, /// 0xf9c
			 0xac593e4d, /// 0xfa0
			 0x4cb603e8, /// 0xfa4
			 0xf9dc10ba, /// 0xfa8
			 0x822ce838, /// 0xfac
			 0x1fe4422b, /// 0xfb0
			 0xca71851c, /// 0xfb4
			 0xcfb0a64d, /// 0xfb8
			 0x57b9c7ae, /// 0xfbc
			 0x093c587e, /// 0xfc0
			 0xd818822d, /// 0xfc4
			 0xb7e82122, /// 0xfc8
			 0x1408209a, /// 0xfcc
			 0xa0835195, /// 0xfd0
			 0xc38a359f, /// 0xfd4
			 0x52cf6383, /// 0xfd8
			 0x93439e33, /// 0xfdc
			 0xc810206f, /// 0xfe0
			 0xc4ba86d8, /// 0xfe4
			 0xef298b7d, /// 0xfe8
			 0x60dcc8eb, /// 0xfec
			 0x30bb67c0, /// 0xff0
			 0x9b6ca9a7, /// 0xff4
			 0xcd06f228, /// 0xff8
			 0xdbe4f75f /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00008
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0000c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00010
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00014
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00018
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0001c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00020
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00024
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0002c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00030
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00034
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0xbf028f5c,                                                  // -0.51                                       /// 0003c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0c600000,                                                  // Leading 1s:                                 /// 00044
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00048
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0004c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00050
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0005c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x00000000,                                                  // zero                                        /// 00064
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00068
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0x7fc00001,                                                  // signaling NaN                               /// 00070
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00074
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00078
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00080
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00084
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0x0c780000,                                                  // Leading 1s:                                 /// 0008c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00098
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0009c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 000c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00100
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00104
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00108
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0010c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00110
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00114
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00118
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0011c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00120
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00124
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0012c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00130
			 0x0c700000,                                                  // Leading 1s:                                 /// 00134
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00138
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00148
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00154
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00158
			 0x00000000,                                                  // zero                                        /// 0015c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00164
			 0x7f800000,                                                  // inf                                         /// 00168
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x7fc00001,                                                  // signaling NaN                               /// 00170
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00178
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00180
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00184
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x0e000003,                                                  // Trailing 1s:                                /// 0018c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00190
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00194
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00198
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 001e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001f0
			 0x55555555,                                                  // 4 random values                             /// 001f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00200
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00204
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00208
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0020c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0xffc00001,                                                  // -signaling NaN                              /// 00214
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0021c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00220
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00224
			 0x00000000,                                                  // zero                                        /// 00228
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00230
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00234
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0023c
			 0xffc00001,                                                  // -signaling NaN                              /// 00240
			 0x55555555,                                                  // 4 random values                             /// 00244
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00250
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00258
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00260
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00264
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x80000000,                                                  // -zero                                       /// 00274
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00278
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00280
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00288
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0028c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00294
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0029c
			 0x80000000,                                                  // -zero                                       /// 002a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002d0
			 0x7f800000,                                                  // inf                                         /// 002d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 002e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0xff800000,                                                  // -inf                                        /// 002f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00300
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00308
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00320
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00324
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00328
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0032c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00338
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0033c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00344
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0034c
			 0x3f028f5c,                                                  // 0.51                                        /// 00350
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00354
			 0x80000000,                                                  // -zero                                       /// 00358
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0035c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00360
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00368
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0036c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00370
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00374
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00378
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0037c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00380
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00388
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0038c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00398
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0039c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 003b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 003d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003e8
			 0x3f028f5c,                                                  // 0.51                                        /// 003ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x00000000,                                                  // zero                                        /// 00400
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00408
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0040c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00414
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0041c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x00011111,                                                  // 9.7958E-41                                  /// 00424
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00428
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00434
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00438
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0043c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00440
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00444
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00448
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0045c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00460
			 0x00011111,                                                  // 9.7958E-41                                  /// 00464
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00468
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00470
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00474
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00478
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0047c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00480
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00484
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00490
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00494
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00498
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x0c400000,                                                  // Leading 1s:                                 /// 004a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0xbf028f5c,                                                  // -0.51                                       /// 004c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x7fc00001,                                                  // signaling NaN                               /// 004cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x7fc00001,                                                  // signaling NaN                               /// 004f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00500
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00504
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0050c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00518
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00520
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00524
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00528
			 0x55555555,                                                  // 4 random values                             /// 0052c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00530
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00534
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0053c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x0c600000,                                                  // Leading 1s:                                 /// 00544
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00548
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0054c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00550
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00554
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00558
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0055c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00560
			 0x00000000,                                                  // zero                                        /// 00564
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00568
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0056c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0057c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00580
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00588
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00590
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0059c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 005ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005d0
			 0x7fc00001,                                                  // signaling NaN                               /// 005d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 005e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 005f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 005fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00604
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00608
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00610
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00614
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0061c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00620
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00624
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00628
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0062c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00630
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00638
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00640
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00644
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0064c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00650
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00654
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00658
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0065c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00660
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00664
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00668
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0066c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00670
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00674
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0067c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00680
			 0x80000000,                                                  // -zero                                       /// 00684
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0068c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00690
			 0x33333333,                                                  // 4 random values                             /// 00694
			 0x80011111,                                                  // -9.7958E-41                                 /// 00698
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006c4
			 0x33333333,                                                  // 4 random values                             /// 006c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0070c
			 0x33333333,                                                  // 4 random values                             /// 00710
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0e000007,                                                  // Trailing 1s:                                /// 00718
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0071c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00720
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00730
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00734
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00740
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00744
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00748
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0074c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00750
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00754
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00758
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0075c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00760
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00764
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x00011111,                                                  // 9.7958E-41                                  /// 0076c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0c780000,                                                  // Leading 1s:                                 /// 00774
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00778
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0077c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00788
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0078c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00794
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00798
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0079c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007b0
			 0xbf028f5c,                                                  // -0.51                                       /// 007b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 007c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 007f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00800
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00804
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0080c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00810
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00814
			 0x0c400000,                                                  // Leading 1s:                                 /// 00818
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x7f800000,                                                  // inf                                         /// 00820
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00824
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00828
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0082c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00830
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00834
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0083c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0084c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00854
			 0x0c600000,                                                  // Leading 1s:                                 /// 00858
			 0x4b000000,                                                  // 8388608.0                                   /// 0085c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00860
			 0x3f028f5c,                                                  // 0.51                                        /// 00864
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0086c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00870
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0087c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00880
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x7fc00001,                                                  // signaling NaN                               /// 00888
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00898
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x7f800000,                                                  // inf                                         /// 008c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008cc
			 0xcb000000,                                                  // -8388608.0                                  /// 008d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 008e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008e8
			 0x00000000,                                                  // zero                                        /// 008ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00900
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00908
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00910
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00914
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00918
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0091c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00920
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00924
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00928
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0092c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00930
			 0x7f800000,                                                  // inf                                         /// 00934
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00938
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00940
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00944
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00948
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00950
			 0x7f800000,                                                  // inf                                         /// 00954
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00958
			 0x4b000000,                                                  // 8388608.0                                   /// 0095c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00960
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00964
			 0x80011111,                                                  // -9.7958E-41                                 /// 00968
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00970
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00974
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00978
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0097c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00980
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00984
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00988
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0098c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00990
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00994
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x7f800000,                                                  // inf                                         /// 009a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0xcb000000,                                                  // -8388608.0                                  /// 009e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009f4
			 0x33333333,                                                  // 4 random values                             /// 009f8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a00
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a04
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a14
			 0x7fc00001,                                                  // signaling NaN                               /// 00a18
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a4c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a50
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a68
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0xff800000,                                                  // -inf                                        /// 00a70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a80
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a8c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a9c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00aac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ab4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ac0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ac4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ac8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00acc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ad0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ad4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ad8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00adc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ae4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ae8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00af8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b20
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x7fc00001,                                                  // signaling NaN                               /// 00b2c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b38
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b3c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b44
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b84
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b8c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ba0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ba8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x3f028f5c,                                                  // 0.51                                        /// 00bb0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bbc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bc0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bc8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bcc
			 0xcb000000,                                                  // -8388608.0                                  /// 00bd0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bd4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0xffc00001,                                                  // -signaling NaN                              /// 00be4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00be8
			 0x7f800000,                                                  // inf                                         /// 00bec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bf0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bf4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c00
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c10
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c14
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c24
			 0xcb000000,                                                  // -8388608.0                                  /// 00c28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c38
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c4c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c54
			 0x4b000000,                                                  // 8388608.0                                   /// 00c58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c5c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c60
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c64
			 0x4b000000,                                                  // 8388608.0                                   /// 00c68
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x00000000,                                                  // zero                                        /// 00c80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c88
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ca4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ca8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cb0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cb4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cbc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cc0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cc4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ccc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cd0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cd4
			 0x7f800000,                                                  // inf                                         /// 00cd8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cdc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ce0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ce4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ce8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cf8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cfc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d00
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d04
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d30
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x7f800000,                                                  // inf                                         /// 00d38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d48
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d58
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d5c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d68
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d6c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d98
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00da0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00da8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00db0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00db4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00db8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dc4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dc8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dcc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dd4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ddc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00de0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00de4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00de8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00df4
			 0x33333333,                                                  // 4 random values                             /// 00df8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dfc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e30
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e38
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e40
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e50
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e90
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e98
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e9c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ea4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ea8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00eb0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00eb4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00eb8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ec0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ec4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ed0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ed4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ed8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00edc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ee4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ee8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00eec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ef4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00efc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f04
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f08
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f0c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f18
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f1c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f20
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f24
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f30
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f38
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f50
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f6c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f8c
			 0xff800000,                                                  // -inf                                        /// 00f90
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f94
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fa8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fb0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fb4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fc0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fc8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fcc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fd0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fd4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fd8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fe0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fe4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fe8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x33333333,                                                  // 4 random values                             /// 00ff4
			 0x00000000,                                                  // zero                                        /// 00ff8
			 0x80000004                                                  // SP - 1 bit alone set in mantissa -ve        /// last
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
			 0x000005a8,
			 0x0000033c,
			 0x0000027c,
			 0x0000068c,
			 0x00000304,
			 0x00000428,
			 0x00000218,
			 0x00000364,

			 /// vpu register f2
			 0x41e80000,
			 0x41100000,
			 0x40c00000,
			 0x41500000,
			 0x41000000,
			 0x41a80000,
			 0x41100000,
			 0x40c00000,

			 /// vpu register f3
			 0x41f80000,
			 0x41700000,
			 0x41b80000,
			 0x41500000,
			 0x41e80000,
			 0x41700000,
			 0x41300000,
			 0x41f80000,

			 /// vpu register f4
			 0x41500000,
			 0x41880000,
			 0x41f00000,
			 0x3f800000,
			 0x3f800000,
			 0x41a00000,
			 0x41d00000,
			 0x41d00000,

			 /// vpu register f5
			 0x41200000,
			 0x41200000,
			 0x41700000,
			 0x41300000,
			 0x40000000,
			 0x41400000,
			 0x41f00000,
			 0x41a80000,

			 /// vpu register f6
			 0x41e80000,
			 0x41e80000,
			 0x3f800000,
			 0x41700000,
			 0x41880000,
			 0x41a80000,
			 0x41600000,
			 0x41980000,

			 /// vpu register f7
			 0x41100000,
			 0x41200000,
			 0x41200000,
			 0x40c00000,
			 0x41980000,
			 0x41f00000,
			 0x40a00000,
			 0x41500000,

			 /// vpu register f8
			 0x41800000,
			 0x40a00000,
			 0x3f800000,
			 0x40000000,
			 0x41e00000,
			 0x41980000,
			 0x41f80000,
			 0x41500000,

			 /// vpu register f9
			 0x41800000,
			 0x41980000,
			 0x40400000,
			 0x41b80000,
			 0x41b00000,
			 0x41900000,
			 0x41e80000,
			 0x41b80000,

			 /// vpu register f10
			 0x41a80000,
			 0x42000000,
			 0x41500000,
			 0x41300000,
			 0x41f80000,
			 0x40400000,
			 0x41a00000,
			 0x40c00000,

			 /// vpu register f11
			 0x41600000,
			 0x42000000,
			 0x41600000,
			 0x41f00000,
			 0x40e00000,
			 0x41b80000,
			 0x41e00000,
			 0x41e00000,

			 /// vpu register f12
			 0x41300000,
			 0x41980000,
			 0x41800000,
			 0x42000000,
			 0x41980000,
			 0x41b80000,
			 0x41600000,
			 0x40e00000,

			 /// vpu register f13
			 0x41880000,
			 0x41d80000,
			 0x41a00000,
			 0x41400000,
			 0x41700000,
			 0x40e00000,
			 0x42000000,
			 0x41c00000,

			 /// vpu register f14
			 0x41200000,
			 0x42000000,
			 0x41800000,
			 0x41500000,
			 0x41600000,
			 0x41400000,
			 0x41a00000,
			 0x41e80000,

			 /// vpu register f15
			 0x41900000,
			 0x41f00000,
			 0x41a80000,
			 0x41100000,
			 0x40c00000,
			 0x41b00000,
			 0x41600000,
			 0x41880000,

			 /// vpu register f16
			 0x41000000,
			 0x41400000,
			 0x40c00000,
			 0x40000000,
			 0x41400000,
			 0x41300000,
			 0x41d80000,
			 0x40c00000,

			 /// vpu register f17
			 0x41c00000,
			 0x41d80000,
			 0x41500000,
			 0x41b80000,
			 0x40800000,
			 0x40400000,
			 0x41a80000,
			 0x41980000,

			 /// vpu register f18
			 0x41000000,
			 0x41200000,
			 0x40400000,
			 0x40e00000,
			 0x42000000,
			 0x41c00000,
			 0x41900000,
			 0x41400000,

			 /// vpu register f19
			 0x41e00000,
			 0x42000000,
			 0x41c80000,
			 0x41d00000,
			 0x41a80000,
			 0x41880000,
			 0x41f80000,
			 0x41200000,

			 /// vpu register f20
			 0x41800000,
			 0x41000000,
			 0x41700000,
			 0x40800000,
			 0x41200000,
			 0x41f80000,
			 0x41980000,
			 0x41b00000,

			 /// vpu register f21
			 0x3f800000,
			 0x41b00000,
			 0x40e00000,
			 0x41800000,
			 0x3f800000,
			 0x40400000,
			 0x41a00000,
			 0x41c00000,

			 /// vpu register f22
			 0x41200000,
			 0x41d00000,
			 0x41a00000,
			 0x42000000,
			 0x41980000,
			 0x41900000,
			 0x41980000,
			 0x41e00000,

			 /// vpu register f23
			 0x41f00000,
			 0x41f80000,
			 0x41000000,
			 0x41c80000,
			 0x41000000,
			 0x41800000,
			 0x40a00000,
			 0x41000000,

			 /// vpu register f24
			 0x41b80000,
			 0x41300000,
			 0x41e80000,
			 0x41b80000,
			 0x41f00000,
			 0x41980000,
			 0x41e00000,
			 0x41b80000,

			 /// vpu register f25
			 0x41100000,
			 0x40000000,
			 0x41b00000,
			 0x41980000,
			 0x42000000,
			 0x41100000,
			 0x41f00000,
			 0x40c00000,

			 /// vpu register f26
			 0x41300000,
			 0x41700000,
			 0x41b00000,
			 0x41a00000,
			 0x3f800000,
			 0x3f800000,
			 0x41600000,
			 0x41e80000,

			 /// vpu register f27
			 0x40400000,
			 0x3f800000,
			 0x41f00000,
			 0x41800000,
			 0x41c80000,
			 0x41c00000,
			 0x42000000,
			 0x41c80000,

			 /// vpu register f28
			 0x40e00000,
			 0x41200000,
			 0x41a80000,
			 0x42000000,
			 0x3f800000,
			 0x40e00000,
			 0x41f00000,
			 0x41a80000,

			 /// vpu register f29
			 0x41e80000,
			 0x40e00000,
			 0x41400000,
			 0x41700000,
			 0x41e00000,
			 0x41980000,
			 0x41c80000,
			 0x41f80000,

			 /// vpu register f30
			 0x41d80000,
			 0x41e80000,
			 0x41a80000,
			 0x41f80000,
			 0x40400000,
			 0x41200000,
			 0x41d00000,
			 0x41f00000,

			 /// vpu register f31
			 0x40e00000,
			 0x41300000,
			 0x40a00000,
			 0x41600000,
			 0x41b00000,
			 0x41400000,
			 0x41200000,
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
		"fcvt.un2.ps f10, f17\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f3, f26\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f7, f0\n"                                ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f17, f16\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f7, f16\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f15, f21\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f26, f18\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f4, f3\n"                                ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f4, f14\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f28, f22\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f8, f8\n"                                ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f19, f2\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f23, f6\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f25, f7\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f3, f8\n"                                ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f12, f17\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f4, f18\n"                               ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f20, f30\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f28, f5\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f25, f22\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f17, f4\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f27, f17\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f9, f16\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f10, f23\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f25, f11\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f2, f19\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f18, f6\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f4, f23\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f4, f10\n"                               ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f22, f13\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f30, f8\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f21, f20\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f26, f24\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f15, f24\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f10, f11\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f15, f21\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f29, f21\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f5, f26\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f8, f9\n"                                ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f11, f13\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f4, f21\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f22, f13\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f2, f8\n"                                ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f6, f21\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f10, f11\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f8, f26\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f3, f21\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f24, f5\n"                               ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f3, f18\n"                               ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f3, f16\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f16, f28\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f20, f0\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f22, f14\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f5, f25\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f16, f7\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f23, f17\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f27, f24\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f12, f15\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f24, f25\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f2, f4\n"                                ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f17, f24\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f29, f3\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f13, f22\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f23, f10\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f13, f6\n"                               ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f11, f14\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f21, f5\n"                               ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f2, f27\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f28, f10\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f19, f22\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f3, f9\n"                                ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f3, f3\n"                                ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f25, f26\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f13, f11\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f11, f6\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f14, f30\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f17, f14\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f14, f1\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f24, f27\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f30, f2\n"                               ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f26, f24\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f27, f16\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f26, f2\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f23, f12\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f20, f16\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f27, f14\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f7, f0\n"                                ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f28, f5\n"                               ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f29, f10\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f30, f1\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f27, f28\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f11, f11\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f10, f11\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f4, f18\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f4, f25\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f10, f19\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f15, f18\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f26, f15\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.un2.ps f18, f22\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
