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
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0e000003,                                                  // Trailing 1s:                                /// 00008
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0000c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00010
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00014
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00018
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0001c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00028
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0002c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00030
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00038
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0003c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00040
			 0x0e000007,                                                  // Trailing 1s:                                /// 00044
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x7fc00001,                                                  // signaling NaN                               /// 0004c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00050
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00054
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00058
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0005c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00064
			 0xffc00001,                                                  // -signaling NaN                              /// 00068
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0006c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0007c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00080
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00084
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00090
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00094
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0009c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 000a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000a8
			 0xff800000,                                                  // -inf                                        /// 000ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000c0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000c4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000d8
			 0x4b000000,                                                  // 8388608.0                                   /// 000dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00100
			 0x00000000,                                                  // zero                                        /// 00104
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00108
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00114
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00118
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00120
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00124
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00128
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00134
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00138
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0013c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00140
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00144
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0014c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00150
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00158
			 0x33333333,                                                  // 4 random values                             /// 0015c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00164
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00168
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0016c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00170
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00178
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0017c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00180
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00184
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00188
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0018c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00198
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0019c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001d4
			 0x33333333,                                                  // 4 random values                             /// 001d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 001ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00208
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0020c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00210
			 0x00000000,                                                  // zero                                        /// 00214
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00218
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0021c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00224
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00228
			 0x7f800000,                                                  // inf                                         /// 0022c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00230
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00234
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0023c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00240
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00244
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00248
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0024c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00250
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00278
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00280
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00284
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0028c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0c400000,                                                  // Leading 1s:                                 /// 00294
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00298
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002b0
			 0x0c780000,                                                  // Leading 1s:                                 /// 002b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00300
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0030c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00310
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00314
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00318
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0031c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00320
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00324
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00328
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0032c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00330
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00338
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0033c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00340
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00344
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00348
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0035c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0036c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00370
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00374
			 0x0e000007,                                                  // Trailing 1s:                                /// 00378
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0037c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00380
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00384
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00388
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0038c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00390
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00394
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00398
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003b0
			 0xffc00001,                                                  // -signaling NaN                              /// 003b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 003e4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 003f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 003fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00408
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0040c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00410
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00414
			 0x80000000,                                                  // -zero                                       /// 00418
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0041c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00420
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00424
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00430
			 0x00000000,                                                  // zero                                        /// 00434
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00438
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00440
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00444
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0044c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00450
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00454
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00460
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00464
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0046c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00470
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00478
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00480
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00484
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00488
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00490
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00494
			 0x0c700000,                                                  // Leading 1s:                                 /// 00498
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0049c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004a8
			 0xffc00001,                                                  // -signaling NaN                              /// 004ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00504
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00508
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00510
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00514
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x3f028f5c,                                                  // 0.51                                        /// 0051c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00520
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00524
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0052c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00538
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0053c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00540
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00544
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00548
			 0x0c700000,                                                  // Leading 1s:                                 /// 0054c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x80011111,                                                  // -9.7958E-41                                 /// 00554
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0055c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00560
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00564
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00568
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0056c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00570
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00574
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00578
			 0x0c780000,                                                  // Leading 1s:                                 /// 0057c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x0e000007,                                                  // Trailing 1s:                                /// 00584
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00588
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00590
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00594
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x00000000,                                                  // zero                                        /// 0059c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 005b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005dc
			 0x4b000000,                                                  // 8388608.0                                   /// 005e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00600
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00610
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00618
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0061c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00620
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00628
			 0x0c400000,                                                  // Leading 1s:                                 /// 0062c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00630
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00634
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0063c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x4b000000,                                                  // 8388608.0                                   /// 00650
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00654
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00658
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00664
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00668
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0066c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0e000007,                                                  // Trailing 1s:                                /// 00674
			 0x0e000001,                                                  // Trailing 1s:                                /// 00678
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0067c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00680
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00688
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00698
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0069c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x4b000000,                                                  // 8388608.0                                   /// 006b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00704
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00710
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00714
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00718
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0071c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00724
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0072c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00730
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00734
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00738
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0073c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00740
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00744
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0074c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00754
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00758
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00760
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00770
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00774
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0077c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00784
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0078c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00790
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007ec
			 0xffc00001,                                                  // -signaling NaN                              /// 007f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007fc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x0e000001,                                                  // Trailing 1s:                                /// 00804
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00808
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0080c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00814
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00818
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0081c
			 0x55555555,                                                  // 4 random values                             /// 00820
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00824
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0082c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00830
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00838
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00844
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00848
			 0x0e000007,                                                  // Trailing 1s:                                /// 0084c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00850
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0085c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00860
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00864
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00868
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0086c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00874
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00878
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00884
			 0xbf028f5c,                                                  // -0.51                                       /// 00888
			 0x0c600000,                                                  // Leading 1s:                                 /// 0088c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00890
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00894
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00898
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0089c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008ac
			 0xbf028f5c,                                                  // -0.51                                       /// 008b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008d0
			 0x80000000,                                                  // -zero                                       /// 008d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x7f800000,                                                  // inf                                         /// 008e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00900
			 0xcb000000,                                                  // -8388608.0                                  /// 00904
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00908
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0090c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00910
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0091c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00920
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00924
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00928
			 0x0e000003,                                                  // Trailing 1s:                                /// 0092c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00930
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00934
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00938
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00948
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00958
			 0x0c780000,                                                  // Leading 1s:                                 /// 0095c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00964
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0096c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00970
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00974
			 0x4b000000,                                                  // 8388608.0                                   /// 00978
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00980
			 0x7fc00001,                                                  // signaling NaN                               /// 00984
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00988
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0098c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00990
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00994
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x00011111,                                                  // 9.7958E-41                                  /// 009a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009a4
			 0xff800000,                                                  // -inf                                        /// 009a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x7fc00001,                                                  // signaling NaN                               /// 009e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a30
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a3c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a44
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a48
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a50
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a58
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a74
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a80
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a88
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a8c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a94
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aa8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ab4
			 0xff800000,                                                  // -inf                                        /// 00ab8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00abc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ac0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ac8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ad4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ad8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00adc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ae0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ae4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ae8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00af4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00af8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00afc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b00
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b08
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b0c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b14
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b20
			 0x00000000,                                                  // zero                                        /// 00b24
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b34
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b40
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b60
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b70
			 0x4b000000,                                                  // 8388608.0                                   /// 00b74
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b78
			 0xffc00001,                                                  // -signaling NaN                              /// 00b7c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b8c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b90
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ba0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ba4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ba8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bb0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bb8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bc0
			 0x00000000,                                                  // zero                                        /// 00bc4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bc8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bcc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bd4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bdc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00be4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00be8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bf4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bf8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bfc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c04
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c10
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c18
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x00000000,                                                  // zero                                        /// 00c20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c38
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c40
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c5c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x80000000,                                                  // -zero                                       /// 00c64
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c68
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c74
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x80000000,                                                  // -zero                                       /// 00c8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c90
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c94
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c98
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ca0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ca4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ca8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x00000000,                                                  // zero                                        /// 00cb0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x7f800000,                                                  // inf                                         /// 00cb8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cc4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ccc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cd0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cd4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cd8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cdc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ce0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ce4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ce8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cf0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cf4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cf8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d00
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d04
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x00000000,                                                  // zero                                        /// 00d10
			 0x80000000,                                                  // -zero                                       /// 00d14
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d20
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d28
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d34
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d3c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d54
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d58
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d6c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d74
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d88
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d94
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d98
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00da0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00da4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00db0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00db4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dc8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dcc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dd0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dd4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ddc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00de4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00df0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00df4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00df8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dfc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e00
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e04
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e08
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e14
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e18
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e30
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e34
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e50
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e58
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0xff800000,                                                  // -inf                                        /// 00e60
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e68
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e6c
			 0x55555555,                                                  // 4 random values                             /// 00e70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e7c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e8c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e98
			 0x33333333,                                                  // 4 random values                             /// 00e9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ea8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00eb4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ec0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ec8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ecc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ed0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ed4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00edc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ee8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00efc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f10
			 0x55555555,                                                  // 4 random values                             /// 00f14
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f18
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f1c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f28
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f2c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f48
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f54
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f58
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f6c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f90
			 0x4b000000,                                                  // 8388608.0                                   /// 00f94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fa4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fa8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fb0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fb8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fbc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fc0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fc8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fcc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fd4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fd8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fdc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fe0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fe8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ff4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ff8
			 0x00800000 // min norm                                      // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xb1b98647, /// 0x0
			 0x30dce798, /// 0x4
			 0x2db07f30, /// 0x8
			 0xdafce093, /// 0xc
			 0x5ba3b5a5, /// 0x10
			 0x1ff98cac, /// 0x14
			 0x27f93c8e, /// 0x18
			 0x586d682d, /// 0x1c
			 0xd3e721e5, /// 0x20
			 0x92b3fedd, /// 0x24
			 0x1adc6957, /// 0x28
			 0x321d4a4e, /// 0x2c
			 0x3c94ecfd, /// 0x30
			 0x12cddfcd, /// 0x34
			 0x8cecb25f, /// 0x38
			 0x59539c94, /// 0x3c
			 0xa428d6f3, /// 0x40
			 0xe6054c42, /// 0x44
			 0xcf341370, /// 0x48
			 0x24fdaf46, /// 0x4c
			 0x82328c0c, /// 0x50
			 0x853b3c69, /// 0x54
			 0x402f2ad8, /// 0x58
			 0x0a6fecd7, /// 0x5c
			 0xecb4c6ec, /// 0x60
			 0xeb2ed80b, /// 0x64
			 0x6fe9d503, /// 0x68
			 0xaf090b86, /// 0x6c
			 0x0a8242db, /// 0x70
			 0xb5ce925c, /// 0x74
			 0xaec7355c, /// 0x78
			 0xedeefd2c, /// 0x7c
			 0x9ca5ea35, /// 0x80
			 0xdbf659ef, /// 0x84
			 0xdf27d3ab, /// 0x88
			 0x9d8eb2de, /// 0x8c
			 0x76ebef87, /// 0x90
			 0x2dc408c7, /// 0x94
			 0xb54c21d8, /// 0x98
			 0x0b8db4ad, /// 0x9c
			 0x53c0cc89, /// 0xa0
			 0xf273aa29, /// 0xa4
			 0xbbff9db3, /// 0xa8
			 0xa7d1ef9f, /// 0xac
			 0x458959c5, /// 0xb0
			 0xf296affe, /// 0xb4
			 0xb180bffb, /// 0xb8
			 0xb89dacd4, /// 0xbc
			 0xea50229b, /// 0xc0
			 0xdea9ead7, /// 0xc4
			 0xe75b0e91, /// 0xc8
			 0xb107bf83, /// 0xcc
			 0x8d9a5c4f, /// 0xd0
			 0x06094436, /// 0xd4
			 0xd737653f, /// 0xd8
			 0x7c083075, /// 0xdc
			 0x0eb15b37, /// 0xe0
			 0x250e255e, /// 0xe4
			 0x647feabe, /// 0xe8
			 0xf696a195, /// 0xec
			 0xdcf14fc3, /// 0xf0
			 0x34d25ffb, /// 0xf4
			 0x21729b5d, /// 0xf8
			 0x4319c46a, /// 0xfc
			 0x1dc67ce5, /// 0x100
			 0xc2bf11da, /// 0x104
			 0x7f965e99, /// 0x108
			 0xf6368452, /// 0x10c
			 0xf66f1e4d, /// 0x110
			 0x64b9e633, /// 0x114
			 0xcb0846a4, /// 0x118
			 0x9c4b80d9, /// 0x11c
			 0x7e093965, /// 0x120
			 0xf0a71be1, /// 0x124
			 0x95aa78a4, /// 0x128
			 0xf387db73, /// 0x12c
			 0x285ba135, /// 0x130
			 0x5d3960e6, /// 0x134
			 0xedc55249, /// 0x138
			 0xf1cab7d8, /// 0x13c
			 0x2fdb4da4, /// 0x140
			 0x23d4389a, /// 0x144
			 0xb6a728e1, /// 0x148
			 0xb74f4815, /// 0x14c
			 0xc04308d4, /// 0x150
			 0x10d1e4f8, /// 0x154
			 0x8af0d883, /// 0x158
			 0xb8cbaead, /// 0x15c
			 0x2173ea44, /// 0x160
			 0xb170d299, /// 0x164
			 0x37e620fb, /// 0x168
			 0x00674383, /// 0x16c
			 0xaa0449ec, /// 0x170
			 0xd0db6d9a, /// 0x174
			 0x51eac150, /// 0x178
			 0x075d5ba4, /// 0x17c
			 0xf772d8af, /// 0x180
			 0x87fed388, /// 0x184
			 0x82fc4a7b, /// 0x188
			 0x5187f10e, /// 0x18c
			 0xd547c34c, /// 0x190
			 0x1e19e7b2, /// 0x194
			 0x6ba3ac40, /// 0x198
			 0xd840f110, /// 0x19c
			 0xe2e9cfeb, /// 0x1a0
			 0x4ac60113, /// 0x1a4
			 0x851e2fd7, /// 0x1a8
			 0x27f002b8, /// 0x1ac
			 0xdfe4d21f, /// 0x1b0
			 0x1d432e4b, /// 0x1b4
			 0x3a92ba64, /// 0x1b8
			 0x177712e1, /// 0x1bc
			 0xb12ea6bd, /// 0x1c0
			 0x8970a63e, /// 0x1c4
			 0xa5ec365d, /// 0x1c8
			 0x639c5a45, /// 0x1cc
			 0x62ad95a7, /// 0x1d0
			 0x04dfc552, /// 0x1d4
			 0x05c09e6a, /// 0x1d8
			 0x9e240afc, /// 0x1dc
			 0x711d3f93, /// 0x1e0
			 0x27fe974a, /// 0x1e4
			 0x196d110c, /// 0x1e8
			 0xe16c6bbf, /// 0x1ec
			 0xcbebc1ad, /// 0x1f0
			 0xe89329f6, /// 0x1f4
			 0x8a736c12, /// 0x1f8
			 0x65aa1409, /// 0x1fc
			 0xa7c3430b, /// 0x200
			 0x300e7049, /// 0x204
			 0xd8c6a837, /// 0x208
			 0x810a36dd, /// 0x20c
			 0x3e8ed78d, /// 0x210
			 0xefe23596, /// 0x214
			 0x800a5011, /// 0x218
			 0x229dcd86, /// 0x21c
			 0x58e94e5d, /// 0x220
			 0x736c77a9, /// 0x224
			 0x1d00a22e, /// 0x228
			 0x6f1412b0, /// 0x22c
			 0xe5a498aa, /// 0x230
			 0xfe5b30f2, /// 0x234
			 0x0b5a845a, /// 0x238
			 0x986bd0f7, /// 0x23c
			 0xc31371e8, /// 0x240
			 0xbea58d13, /// 0x244
			 0x3aae7ee5, /// 0x248
			 0xcc5b74df, /// 0x24c
			 0x914f73df, /// 0x250
			 0x1012b754, /// 0x254
			 0xebf30fa7, /// 0x258
			 0xfb3819a5, /// 0x25c
			 0x7725f831, /// 0x260
			 0x736ebac3, /// 0x264
			 0x2e687eae, /// 0x268
			 0x6bdd52b6, /// 0x26c
			 0x2c93bc90, /// 0x270
			 0x02883c88, /// 0x274
			 0xf1817445, /// 0x278
			 0x4865b469, /// 0x27c
			 0x49b9ee5a, /// 0x280
			 0x410d4ce8, /// 0x284
			 0x308ff6b6, /// 0x288
			 0xe6401a58, /// 0x28c
			 0x6e49e184, /// 0x290
			 0x205b371c, /// 0x294
			 0x1ee6ef4c, /// 0x298
			 0x75025d81, /// 0x29c
			 0x9a9dfee8, /// 0x2a0
			 0xacb9b572, /// 0x2a4
			 0x8deb2eef, /// 0x2a8
			 0x2d86dc58, /// 0x2ac
			 0x8a13fef8, /// 0x2b0
			 0x2c353030, /// 0x2b4
			 0xeeaf10ee, /// 0x2b8
			 0x8f047d62, /// 0x2bc
			 0xc31fb3cb, /// 0x2c0
			 0x0b2f7b4e, /// 0x2c4
			 0x13fc3a32, /// 0x2c8
			 0xaf859d2f, /// 0x2cc
			 0xd380ffe2, /// 0x2d0
			 0xb0fe4345, /// 0x2d4
			 0xb70bd11c, /// 0x2d8
			 0x0488630e, /// 0x2dc
			 0x94d86179, /// 0x2e0
			 0xff2b8f29, /// 0x2e4
			 0x94fe08cf, /// 0x2e8
			 0x0e758c68, /// 0x2ec
			 0xdf0bbe3c, /// 0x2f0
			 0xd20508d0, /// 0x2f4
			 0x2c858ce8, /// 0x2f8
			 0x9c24e2fb, /// 0x2fc
			 0xdf00eb0c, /// 0x300
			 0x36e6c093, /// 0x304
			 0xdbbfc4bb, /// 0x308
			 0x811e1e64, /// 0x30c
			 0xb5d11fdf, /// 0x310
			 0xc2fdc692, /// 0x314
			 0x6f332196, /// 0x318
			 0x12123384, /// 0x31c
			 0xe02f36c8, /// 0x320
			 0x8b10b69a, /// 0x324
			 0xf4108f35, /// 0x328
			 0xce534c82, /// 0x32c
			 0x01ffb61a, /// 0x330
			 0x1b1aaad2, /// 0x334
			 0x8b4307db, /// 0x338
			 0xa2f7e751, /// 0x33c
			 0x1bbd3b69, /// 0x340
			 0xa274f612, /// 0x344
			 0xd2c220f7, /// 0x348
			 0xc35e2e04, /// 0x34c
			 0x477bfaf4, /// 0x350
			 0x250ba8ea, /// 0x354
			 0x3c2fc01c, /// 0x358
			 0x27c26843, /// 0x35c
			 0xbf548a10, /// 0x360
			 0xfd400de4, /// 0x364
			 0x8286f402, /// 0x368
			 0x941a1d48, /// 0x36c
			 0x67fabf78, /// 0x370
			 0xc5f5d296, /// 0x374
			 0xe95acf3f, /// 0x378
			 0xe0067ef3, /// 0x37c
			 0xbfa44bd9, /// 0x380
			 0xc1461c08, /// 0x384
			 0xeaf872cd, /// 0x388
			 0x0f8d6c89, /// 0x38c
			 0x10a19192, /// 0x390
			 0x71821702, /// 0x394
			 0xf54f2227, /// 0x398
			 0x9e812381, /// 0x39c
			 0xfa1baee9, /// 0x3a0
			 0x4a0d87e5, /// 0x3a4
			 0xb8ffd994, /// 0x3a8
			 0xb9bc9b8b, /// 0x3ac
			 0xdb471b3e, /// 0x3b0
			 0x28ae7789, /// 0x3b4
			 0xf5aaa4d4, /// 0x3b8
			 0xa60679eb, /// 0x3bc
			 0xe8f98f15, /// 0x3c0
			 0x8a80174a, /// 0x3c4
			 0xc75e37f5, /// 0x3c8
			 0xb8116e32, /// 0x3cc
			 0xaceef469, /// 0x3d0
			 0xe76cbe61, /// 0x3d4
			 0x4b4348de, /// 0x3d8
			 0x1e4545fd, /// 0x3dc
			 0x3b80db81, /// 0x3e0
			 0x8453c86a, /// 0x3e4
			 0xa74f42f7, /// 0x3e8
			 0xd5713d47, /// 0x3ec
			 0xe705dfbd, /// 0x3f0
			 0xa042b98b, /// 0x3f4
			 0x19018326, /// 0x3f8
			 0x49f2e8ab, /// 0x3fc
			 0x4afdc61d, /// 0x400
			 0x928c05cc, /// 0x404
			 0x85e73b63, /// 0x408
			 0x396766a3, /// 0x40c
			 0x1e851145, /// 0x410
			 0x360429ac, /// 0x414
			 0xc0dddc8c, /// 0x418
			 0x2c3403ea, /// 0x41c
			 0x5bf35704, /// 0x420
			 0x626c6fac, /// 0x424
			 0x8e9e4366, /// 0x428
			 0x19ff733e, /// 0x42c
			 0x00b520ec, /// 0x430
			 0xf6cd5486, /// 0x434
			 0x2093c017, /// 0x438
			 0xd8b94b7a, /// 0x43c
			 0xfd2b4967, /// 0x440
			 0xe1f6fcad, /// 0x444
			 0xe78e1f77, /// 0x448
			 0x47d256c0, /// 0x44c
			 0xf08bc97a, /// 0x450
			 0xffb082e5, /// 0x454
			 0xd6040208, /// 0x458
			 0xc29d6a03, /// 0x45c
			 0x2c5c0ef6, /// 0x460
			 0x4344c4ed, /// 0x464
			 0x0bf2fbdf, /// 0x468
			 0xfe308285, /// 0x46c
			 0xc0f164df, /// 0x470
			 0x69c4bf15, /// 0x474
			 0xf75c884d, /// 0x478
			 0xaa8fcb06, /// 0x47c
			 0x92f7c4c7, /// 0x480
			 0x390da137, /// 0x484
			 0x60167000, /// 0x488
			 0xb17ed593, /// 0x48c
			 0xf9f11a75, /// 0x490
			 0x0a11333a, /// 0x494
			 0xf5d1b329, /// 0x498
			 0xfd1c4c13, /// 0x49c
			 0x1d8becfb, /// 0x4a0
			 0xc63f5042, /// 0x4a4
			 0x20c42ae6, /// 0x4a8
			 0xcf827d3e, /// 0x4ac
			 0x1aca554f, /// 0x4b0
			 0x32604bc9, /// 0x4b4
			 0xf3fc6d0a, /// 0x4b8
			 0x922612f3, /// 0x4bc
			 0xeb2ad7c9, /// 0x4c0
			 0x2e9b7004, /// 0x4c4
			 0xd1f0f6e9, /// 0x4c8
			 0xe9f96a32, /// 0x4cc
			 0x524f9386, /// 0x4d0
			 0x23dbbde9, /// 0x4d4
			 0x33b01ab9, /// 0x4d8
			 0xe36abd6f, /// 0x4dc
			 0x64441b99, /// 0x4e0
			 0x9578e5ba, /// 0x4e4
			 0xebbf4810, /// 0x4e8
			 0xf117bd2b, /// 0x4ec
			 0x88dc1ec4, /// 0x4f0
			 0x569f1814, /// 0x4f4
			 0xb0322500, /// 0x4f8
			 0x454ecdfc, /// 0x4fc
			 0xa7bd01b3, /// 0x500
			 0x92a50713, /// 0x504
			 0x8209053b, /// 0x508
			 0xe7cef0b2, /// 0x50c
			 0x2a42d476, /// 0x510
			 0x909b7d09, /// 0x514
			 0x1d5708c7, /// 0x518
			 0x4790dbcb, /// 0x51c
			 0xfb441a20, /// 0x520
			 0x6c6ba987, /// 0x524
			 0xf8fa9d67, /// 0x528
			 0xaaad34a4, /// 0x52c
			 0xdc49f07d, /// 0x530
			 0x97f7e65f, /// 0x534
			 0x00e1e7c8, /// 0x538
			 0xddccdf14, /// 0x53c
			 0x18ff8783, /// 0x540
			 0xcd3709e3, /// 0x544
			 0x8b4e83c1, /// 0x548
			 0x599fae92, /// 0x54c
			 0x56ea749c, /// 0x550
			 0xa8b66250, /// 0x554
			 0xd5ab36ac, /// 0x558
			 0xcd167b12, /// 0x55c
			 0x23dcbd8c, /// 0x560
			 0xa803fcb6, /// 0x564
			 0x99946dc9, /// 0x568
			 0xaf556561, /// 0x56c
			 0x3876d1f3, /// 0x570
			 0xc364ff16, /// 0x574
			 0xf216d6e9, /// 0x578
			 0x02fa35d8, /// 0x57c
			 0xc4581ee5, /// 0x580
			 0x08b8f9c5, /// 0x584
			 0x75e59d49, /// 0x588
			 0x99b40e20, /// 0x58c
			 0xcd3eda81, /// 0x590
			 0x4ea3b45f, /// 0x594
			 0xea8595d5, /// 0x598
			 0x388ba99e, /// 0x59c
			 0x03556eab, /// 0x5a0
			 0xf96616ce, /// 0x5a4
			 0x76c7c514, /// 0x5a8
			 0x18c0b73b, /// 0x5ac
			 0xc22a2823, /// 0x5b0
			 0x02f50d5e, /// 0x5b4
			 0x45f9be2c, /// 0x5b8
			 0x999a6717, /// 0x5bc
			 0x004bdda7, /// 0x5c0
			 0xa045dd8c, /// 0x5c4
			 0x1558fc34, /// 0x5c8
			 0x62d67de0, /// 0x5cc
			 0xb7d65c17, /// 0x5d0
			 0xa3f06225, /// 0x5d4
			 0x0e32885f, /// 0x5d8
			 0x09bf06a5, /// 0x5dc
			 0x58adfcdb, /// 0x5e0
			 0x8a3c401b, /// 0x5e4
			 0x40053b5a, /// 0x5e8
			 0xded05045, /// 0x5ec
			 0xd875f185, /// 0x5f0
			 0x56778b47, /// 0x5f4
			 0xaee9a833, /// 0x5f8
			 0xa6e8ae78, /// 0x5fc
			 0xb307427d, /// 0x600
			 0x172e94d0, /// 0x604
			 0xf5094fc0, /// 0x608
			 0x98e6a1e3, /// 0x60c
			 0x5021ed37, /// 0x610
			 0x275dfc07, /// 0x614
			 0xe3db8fc4, /// 0x618
			 0x085f3b1c, /// 0x61c
			 0x1f0e9e18, /// 0x620
			 0x128ba24e, /// 0x624
			 0xd410ddd4, /// 0x628
			 0xaf47ab90, /// 0x62c
			 0x42c4d8db, /// 0x630
			 0x079cff34, /// 0x634
			 0x37a4c509, /// 0x638
			 0xda4561f0, /// 0x63c
			 0x62c6f7d1, /// 0x640
			 0x521670e9, /// 0x644
			 0xb1c080ff, /// 0x648
			 0x5ced5e9d, /// 0x64c
			 0xc8f1becd, /// 0x650
			 0x130d644b, /// 0x654
			 0x94226198, /// 0x658
			 0x43536da3, /// 0x65c
			 0x4f61a5b5, /// 0x660
			 0xfcec4322, /// 0x664
			 0x020b3aaa, /// 0x668
			 0x1f140d93, /// 0x66c
			 0xa75c8752, /// 0x670
			 0x6a87912a, /// 0x674
			 0x7c9b37e6, /// 0x678
			 0xed670fca, /// 0x67c
			 0x3067275d, /// 0x680
			 0x4e4103bb, /// 0x684
			 0x8ca040c3, /// 0x688
			 0xa62aa92e, /// 0x68c
			 0x860eaec8, /// 0x690
			 0x699919d0, /// 0x694
			 0x1032cab7, /// 0x698
			 0x1c879394, /// 0x69c
			 0x4711a68f, /// 0x6a0
			 0x39c045c9, /// 0x6a4
			 0xc21b0da3, /// 0x6a8
			 0xcdb6fab9, /// 0x6ac
			 0xb3e166bc, /// 0x6b0
			 0x41507814, /// 0x6b4
			 0x3d564f87, /// 0x6b8
			 0xc7f23f2a, /// 0x6bc
			 0x46aceddf, /// 0x6c0
			 0x9b2a0276, /// 0x6c4
			 0x66e949d0, /// 0x6c8
			 0x7c23c331, /// 0x6cc
			 0x6f21621b, /// 0x6d0
			 0xf3d6578b, /// 0x6d4
			 0x49b1c640, /// 0x6d8
			 0xb51b1854, /// 0x6dc
			 0xf7c66d34, /// 0x6e0
			 0x73677262, /// 0x6e4
			 0xcdd95045, /// 0x6e8
			 0x90af12ea, /// 0x6ec
			 0x7efd6035, /// 0x6f0
			 0xfb0a67c3, /// 0x6f4
			 0xaf2b8e58, /// 0x6f8
			 0x8333c474, /// 0x6fc
			 0xc598fe99, /// 0x700
			 0x28165ee4, /// 0x704
			 0x9140ac4d, /// 0x708
			 0xee1269eb, /// 0x70c
			 0xecc7b188, /// 0x710
			 0x45030349, /// 0x714
			 0x2a2ec845, /// 0x718
			 0x235fcc53, /// 0x71c
			 0x146d6b64, /// 0x720
			 0x4a60f201, /// 0x724
			 0x1704ffb8, /// 0x728
			 0x2a79d977, /// 0x72c
			 0x1f554995, /// 0x730
			 0xf6fdc1cb, /// 0x734
			 0x0688ef96, /// 0x738
			 0xc8da4c78, /// 0x73c
			 0x25d33a9d, /// 0x740
			 0x8f5b9275, /// 0x744
			 0xa36a129b, /// 0x748
			 0xe31ce70f, /// 0x74c
			 0xcd247363, /// 0x750
			 0x1fde182c, /// 0x754
			 0x4c9cae81, /// 0x758
			 0xf4709be0, /// 0x75c
			 0x2c39f0ac, /// 0x760
			 0xd3e01516, /// 0x764
			 0x88e1009e, /// 0x768
			 0x61f9a1fc, /// 0x76c
			 0x94681e02, /// 0x770
			 0xf1b53d73, /// 0x774
			 0x91751a6c, /// 0x778
			 0x6663782d, /// 0x77c
			 0xf63c1a16, /// 0x780
			 0x3e5f525a, /// 0x784
			 0x155e79e6, /// 0x788
			 0xe24afbee, /// 0x78c
			 0x3dcb7801, /// 0x790
			 0xceba4479, /// 0x794
			 0x041a74bc, /// 0x798
			 0x7b1c1b89, /// 0x79c
			 0xf4071a4c, /// 0x7a0
			 0x134cbfa9, /// 0x7a4
			 0xf70570f6, /// 0x7a8
			 0x476da3f9, /// 0x7ac
			 0xa88123f4, /// 0x7b0
			 0x7897b27e, /// 0x7b4
			 0xf3b97817, /// 0x7b8
			 0x8919280b, /// 0x7bc
			 0x5d7ffb5a, /// 0x7c0
			 0xbfbc370c, /// 0x7c4
			 0x0087291e, /// 0x7c8
			 0xc4d4eed4, /// 0x7cc
			 0xe9ecfdf0, /// 0x7d0
			 0x1e58fb7b, /// 0x7d4
			 0x7986e154, /// 0x7d8
			 0x815f7f87, /// 0x7dc
			 0x07afb959, /// 0x7e0
			 0x64c0cfc0, /// 0x7e4
			 0xe710d346, /// 0x7e8
			 0xb43049db, /// 0x7ec
			 0x0317b880, /// 0x7f0
			 0x484d38de, /// 0x7f4
			 0xbca15a0f, /// 0x7f8
			 0x32169a62, /// 0x7fc
			 0x5e310ecf, /// 0x800
			 0x3729e6fa, /// 0x804
			 0xa8cd2fcb, /// 0x808
			 0x22901d74, /// 0x80c
			 0x56d42b8b, /// 0x810
			 0x8902a3df, /// 0x814
			 0x8e76c059, /// 0x818
			 0x87a2f519, /// 0x81c
			 0xb845d32c, /// 0x820
			 0xdab71bb8, /// 0x824
			 0x97b818ee, /// 0x828
			 0x81e863f2, /// 0x82c
			 0x8556e4ac, /// 0x830
			 0x7874dd57, /// 0x834
			 0xd6cee090, /// 0x838
			 0xad1b47fd, /// 0x83c
			 0x2e54f5ee, /// 0x840
			 0x456733ef, /// 0x844
			 0x64a497fc, /// 0x848
			 0xe0286630, /// 0x84c
			 0x47c0b090, /// 0x850
			 0xd03f6d2c, /// 0x854
			 0x424b5f4d, /// 0x858
			 0xa3375411, /// 0x85c
			 0xeaa00529, /// 0x860
			 0x3e3e3099, /// 0x864
			 0x2857437f, /// 0x868
			 0x72fcfd4d, /// 0x86c
			 0x985600c1, /// 0x870
			 0x2cadc94d, /// 0x874
			 0x2af30a81, /// 0x878
			 0x5a99719f, /// 0x87c
			 0x6fe1cefd, /// 0x880
			 0xbdf71fe6, /// 0x884
			 0xd96bf6c3, /// 0x888
			 0xf4991268, /// 0x88c
			 0x7b3b8f57, /// 0x890
			 0xdc29f7ad, /// 0x894
			 0x545ff1fe, /// 0x898
			 0x9758c34a, /// 0x89c
			 0xcd1e5328, /// 0x8a0
			 0xcafb276b, /// 0x8a4
			 0xfe01d4cb, /// 0x8a8
			 0xc01e4510, /// 0x8ac
			 0xabb4880d, /// 0x8b0
			 0x4dec1a86, /// 0x8b4
			 0xff1ea25e, /// 0x8b8
			 0xc88c4458, /// 0x8bc
			 0x33d7a355, /// 0x8c0
			 0x75221b4d, /// 0x8c4
			 0xae5642d6, /// 0x8c8
			 0xe7306fe1, /// 0x8cc
			 0xd86a0f57, /// 0x8d0
			 0xf6b6cd41, /// 0x8d4
			 0x59eba505, /// 0x8d8
			 0x397c4929, /// 0x8dc
			 0xd01b6394, /// 0x8e0
			 0x6d70fd36, /// 0x8e4
			 0x519fc12d, /// 0x8e8
			 0x76b91481, /// 0x8ec
			 0x6c5959e1, /// 0x8f0
			 0x77800070, /// 0x8f4
			 0x437fc322, /// 0x8f8
			 0x3abdf1cc, /// 0x8fc
			 0xab339b2e, /// 0x900
			 0xa265bb4d, /// 0x904
			 0x59cf7367, /// 0x908
			 0x285b2566, /// 0x90c
			 0xd6fbc29b, /// 0x910
			 0xc258e474, /// 0x914
			 0xe6e5e768, /// 0x918
			 0xca9a460b, /// 0x91c
			 0x18959cdf, /// 0x920
			 0x77270a21, /// 0x924
			 0xdb150458, /// 0x928
			 0x18e0e1ea, /// 0x92c
			 0x660a6483, /// 0x930
			 0x24fdb258, /// 0x934
			 0x4fb24fd7, /// 0x938
			 0xf6499231, /// 0x93c
			 0xef86be18, /// 0x940
			 0x6f31d3c2, /// 0x944
			 0x927fa7cd, /// 0x948
			 0xb8679645, /// 0x94c
			 0x9e2ab133, /// 0x950
			 0x2f968156, /// 0x954
			 0xff900b4d, /// 0x958
			 0x3e6b2c3e, /// 0x95c
			 0xbedf7615, /// 0x960
			 0xd02d4e83, /// 0x964
			 0x68cfae01, /// 0x968
			 0xb065e7a6, /// 0x96c
			 0xdc906c50, /// 0x970
			 0x741184e7, /// 0x974
			 0xd15bd16e, /// 0x978
			 0xbe909966, /// 0x97c
			 0xe51c8869, /// 0x980
			 0xb8a2312f, /// 0x984
			 0x3b54d7b3, /// 0x988
			 0x529b3468, /// 0x98c
			 0x937b4811, /// 0x990
			 0xc1168994, /// 0x994
			 0xa0a40265, /// 0x998
			 0xd4a33c95, /// 0x99c
			 0x11820b45, /// 0x9a0
			 0x9663e34b, /// 0x9a4
			 0x3f25367e, /// 0x9a8
			 0xab53b77f, /// 0x9ac
			 0x6ed52673, /// 0x9b0
			 0xf9bd2d90, /// 0x9b4
			 0xf1baaaf5, /// 0x9b8
			 0x3b349883, /// 0x9bc
			 0xee718c21, /// 0x9c0
			 0x73d07ca4, /// 0x9c4
			 0xf066494f, /// 0x9c8
			 0x2b7b19b6, /// 0x9cc
			 0xa61bece1, /// 0x9d0
			 0x254b0941, /// 0x9d4
			 0xa415c534, /// 0x9d8
			 0x4555a735, /// 0x9dc
			 0x2de02d29, /// 0x9e0
			 0x7ec7de28, /// 0x9e4
			 0x2fb8fbb2, /// 0x9e8
			 0x8ad6a23e, /// 0x9ec
			 0x53371be8, /// 0x9f0
			 0x22429db7, /// 0x9f4
			 0x13172725, /// 0x9f8
			 0x9b72711b, /// 0x9fc
			 0x1849f096, /// 0xa00
			 0x99c4f797, /// 0xa04
			 0x350ea18d, /// 0xa08
			 0xbc9ff587, /// 0xa0c
			 0x2fb88bcf, /// 0xa10
			 0xb528ca26, /// 0xa14
			 0x9fcf623e, /// 0xa18
			 0x64649362, /// 0xa1c
			 0xc4ea6cc5, /// 0xa20
			 0xd9aa6597, /// 0xa24
			 0x0598a01c, /// 0xa28
			 0xac43777c, /// 0xa2c
			 0x55602be5, /// 0xa30
			 0xcd98c890, /// 0xa34
			 0x01d364a2, /// 0xa38
			 0x08d880ef, /// 0xa3c
			 0xf199081b, /// 0xa40
			 0xec6f9129, /// 0xa44
			 0xa373bb18, /// 0xa48
			 0x162616b7, /// 0xa4c
			 0x7e35c86d, /// 0xa50
			 0x9cd402ab, /// 0xa54
			 0xd4738fc4, /// 0xa58
			 0xfce46009, /// 0xa5c
			 0x7aaeac61, /// 0xa60
			 0xa3a75d9e, /// 0xa64
			 0xe12c446b, /// 0xa68
			 0xd880475a, /// 0xa6c
			 0xfc8e0ce2, /// 0xa70
			 0x7777efeb, /// 0xa74
			 0x01a818b5, /// 0xa78
			 0x56531f3c, /// 0xa7c
			 0x6ebfc97d, /// 0xa80
			 0x943514ef, /// 0xa84
			 0x1980ad8e, /// 0xa88
			 0x54a0babb, /// 0xa8c
			 0x0cafaeb1, /// 0xa90
			 0x99021a75, /// 0xa94
			 0xe819e966, /// 0xa98
			 0x9ff1a07e, /// 0xa9c
			 0xd9d6e6c4, /// 0xaa0
			 0x47ea3b9c, /// 0xaa4
			 0xdcf170f0, /// 0xaa8
			 0x429b2b6e, /// 0xaac
			 0x67c3affb, /// 0xab0
			 0x15965670, /// 0xab4
			 0xcfff2577, /// 0xab8
			 0x6c745dc0, /// 0xabc
			 0xce0e27a6, /// 0xac0
			 0xbeb6e374, /// 0xac4
			 0xaaa9599e, /// 0xac8
			 0xcc2c7b8d, /// 0xacc
			 0x438f4524, /// 0xad0
			 0x7bb52e37, /// 0xad4
			 0x5a1445a0, /// 0xad8
			 0xb0a8c3bd, /// 0xadc
			 0x1166bdca, /// 0xae0
			 0xf7d1d4d0, /// 0xae4
			 0x3392bb2a, /// 0xae8
			 0xa4db5ace, /// 0xaec
			 0xdb60e5df, /// 0xaf0
			 0xb20bca3b, /// 0xaf4
			 0x63f2f94b, /// 0xaf8
			 0x3a486694, /// 0xafc
			 0x43413c6b, /// 0xb00
			 0x86d610c7, /// 0xb04
			 0xbc90487d, /// 0xb08
			 0xf8f61867, /// 0xb0c
			 0x164023d2, /// 0xb10
			 0xf84ba889, /// 0xb14
			 0x91e20dc3, /// 0xb18
			 0x5904687a, /// 0xb1c
			 0xdc769848, /// 0xb20
			 0x2d982eb1, /// 0xb24
			 0x9080f520, /// 0xb28
			 0x9bdf7cb3, /// 0xb2c
			 0x3503aac3, /// 0xb30
			 0xa9f17eef, /// 0xb34
			 0xff5add0c, /// 0xb38
			 0x7273564a, /// 0xb3c
			 0x2764a749, /// 0xb40
			 0xe18ecdbc, /// 0xb44
			 0x2f81dbb8, /// 0xb48
			 0x3a3e49a0, /// 0xb4c
			 0x1080a56e, /// 0xb50
			 0x3ec3b783, /// 0xb54
			 0x28e4dbe4, /// 0xb58
			 0x40a7126c, /// 0xb5c
			 0xd2e9d98b, /// 0xb60
			 0x763d11a5, /// 0xb64
			 0x39cec928, /// 0xb68
			 0x40a30cca, /// 0xb6c
			 0x523af6c5, /// 0xb70
			 0xa099c090, /// 0xb74
			 0x09eceab8, /// 0xb78
			 0x71dedf1f, /// 0xb7c
			 0xb5f547d2, /// 0xb80
			 0x32d1a393, /// 0xb84
			 0xe386c283, /// 0xb88
			 0xaa8d78f7, /// 0xb8c
			 0x8bb51ed9, /// 0xb90
			 0x6417513d, /// 0xb94
			 0x9b8d4ba2, /// 0xb98
			 0xe10bf4a5, /// 0xb9c
			 0xa4486b2d, /// 0xba0
			 0x099b59b1, /// 0xba4
			 0x018767a0, /// 0xba8
			 0xd0c6dea9, /// 0xbac
			 0xde6503ba, /// 0xbb0
			 0x7e803239, /// 0xbb4
			 0x0ab9697d, /// 0xbb8
			 0xfeb6b8a8, /// 0xbbc
			 0x1beea545, /// 0xbc0
			 0x6549b2e0, /// 0xbc4
			 0xba066bf3, /// 0xbc8
			 0xf2f4b36e, /// 0xbcc
			 0x8eb93d6d, /// 0xbd0
			 0xcd60162a, /// 0xbd4
			 0xc4e86aa6, /// 0xbd8
			 0x2b31b918, /// 0xbdc
			 0xcbdb197b, /// 0xbe0
			 0x175fd6c9, /// 0xbe4
			 0x3c26e35c, /// 0xbe8
			 0xf28e5211, /// 0xbec
			 0xc6762c98, /// 0xbf0
			 0x2095fce1, /// 0xbf4
			 0x94a6b30e, /// 0xbf8
			 0xf7a17fa1, /// 0xbfc
			 0x469e8bb8, /// 0xc00
			 0xdd3bdb6c, /// 0xc04
			 0xf9edcf49, /// 0xc08
			 0x8d985f53, /// 0xc0c
			 0x619940a6, /// 0xc10
			 0xbd30844e, /// 0xc14
			 0x6093f8cb, /// 0xc18
			 0x3461025a, /// 0xc1c
			 0x3260528a, /// 0xc20
			 0x1be4e7db, /// 0xc24
			 0xf16e7652, /// 0xc28
			 0xa7b77fb2, /// 0xc2c
			 0x30f75f90, /// 0xc30
			 0xbf123123, /// 0xc34
			 0xf9e00388, /// 0xc38
			 0xdb608041, /// 0xc3c
			 0xb1fe4831, /// 0xc40
			 0x27bbc0d2, /// 0xc44
			 0x93004d8c, /// 0xc48
			 0x2c32661b, /// 0xc4c
			 0x38083404, /// 0xc50
			 0x4d0b2b46, /// 0xc54
			 0x35714482, /// 0xc58
			 0xe8d3219a, /// 0xc5c
			 0x4d6898d9, /// 0xc60
			 0x49e2b01a, /// 0xc64
			 0x64ec5ab6, /// 0xc68
			 0xa2aa9825, /// 0xc6c
			 0xc3882b05, /// 0xc70
			 0xed93f34c, /// 0xc74
			 0x00a3fbc8, /// 0xc78
			 0x35a80edb, /// 0xc7c
			 0x3b82a767, /// 0xc80
			 0xd810c5fd, /// 0xc84
			 0x4e47a019, /// 0xc88
			 0x7cd69aa7, /// 0xc8c
			 0x68562bf5, /// 0xc90
			 0x541e01f8, /// 0xc94
			 0x11d16dcb, /// 0xc98
			 0x520757ff, /// 0xc9c
			 0x22ddbec5, /// 0xca0
			 0x6fe8e6a6, /// 0xca4
			 0x581715bb, /// 0xca8
			 0x23ddc991, /// 0xcac
			 0x30134f84, /// 0xcb0
			 0x8415471b, /// 0xcb4
			 0x7e71bb6d, /// 0xcb8
			 0xa02087d8, /// 0xcbc
			 0xbad6c577, /// 0xcc0
			 0x5c034940, /// 0xcc4
			 0x717e971d, /// 0xcc8
			 0x81368d5c, /// 0xccc
			 0x96717dbd, /// 0xcd0
			 0xa7da1905, /// 0xcd4
			 0xf6f86c5c, /// 0xcd8
			 0x32e583f5, /// 0xcdc
			 0x34bd7ea8, /// 0xce0
			 0x5414df8c, /// 0xce4
			 0x576be4b7, /// 0xce8
			 0xb9668767, /// 0xcec
			 0x22c0d04a, /// 0xcf0
			 0x605cc547, /// 0xcf4
			 0x44642dc1, /// 0xcf8
			 0x28b9ed75, /// 0xcfc
			 0x48608c01, /// 0xd00
			 0xe04f587d, /// 0xd04
			 0xdd739565, /// 0xd08
			 0xee8cf592, /// 0xd0c
			 0x7c83a3c4, /// 0xd10
			 0x253e4ba3, /// 0xd14
			 0x1b96d53d, /// 0xd18
			 0xbc56e31b, /// 0xd1c
			 0x5560210d, /// 0xd20
			 0x32a3c38e, /// 0xd24
			 0x0a86ac98, /// 0xd28
			 0x0328f0f6, /// 0xd2c
			 0xd1e2091d, /// 0xd30
			 0x3d603688, /// 0xd34
			 0x25a1ced2, /// 0xd38
			 0x2df41e56, /// 0xd3c
			 0x6668078f, /// 0xd40
			 0xb46eeefb, /// 0xd44
			 0x28e27637, /// 0xd48
			 0x3fbaba4f, /// 0xd4c
			 0x8aec3b43, /// 0xd50
			 0x47e8cdb7, /// 0xd54
			 0xa6c48afa, /// 0xd58
			 0x6e4dec1a, /// 0xd5c
			 0x1577c68d, /// 0xd60
			 0x80614958, /// 0xd64
			 0x96155dc5, /// 0xd68
			 0xbb51819f, /// 0xd6c
			 0x19d033bc, /// 0xd70
			 0xad4cff10, /// 0xd74
			 0x8953e878, /// 0xd78
			 0xb3ee0a8e, /// 0xd7c
			 0x2a30d339, /// 0xd80
			 0xae57e5e3, /// 0xd84
			 0xc0170f1c, /// 0xd88
			 0x382c0ba1, /// 0xd8c
			 0x7bd48aa7, /// 0xd90
			 0x2edf2762, /// 0xd94
			 0x4229287c, /// 0xd98
			 0x13bf40af, /// 0xd9c
			 0xc4359ec1, /// 0xda0
			 0xa4724987, /// 0xda4
			 0xac531f24, /// 0xda8
			 0xbf75bbe9, /// 0xdac
			 0x44ba7690, /// 0xdb0
			 0x8469d799, /// 0xdb4
			 0x7796bea4, /// 0xdb8
			 0x6ce949d1, /// 0xdbc
			 0xba72de4b, /// 0xdc0
			 0x13df9c1c, /// 0xdc4
			 0x2ddff633, /// 0xdc8
			 0xed38e057, /// 0xdcc
			 0x17fdd980, /// 0xdd0
			 0x31b08d30, /// 0xdd4
			 0x6d5d1bc9, /// 0xdd8
			 0xe67d6f9d, /// 0xddc
			 0xf28ae21f, /// 0xde0
			 0x7d983dec, /// 0xde4
			 0x10265124, /// 0xde8
			 0xbeb5aaa2, /// 0xdec
			 0x97dd41da, /// 0xdf0
			 0x521946b6, /// 0xdf4
			 0xa6349dd2, /// 0xdf8
			 0x5a028f3c, /// 0xdfc
			 0x5ffe690a, /// 0xe00
			 0xf1686ed9, /// 0xe04
			 0x759cbed9, /// 0xe08
			 0x94955e4a, /// 0xe0c
			 0x72b83530, /// 0xe10
			 0x3d6e506f, /// 0xe14
			 0x1d019b9e, /// 0xe18
			 0x07371bd1, /// 0xe1c
			 0xad0565e3, /// 0xe20
			 0x89e7934a, /// 0xe24
			 0x4adb9db1, /// 0xe28
			 0xed03c592, /// 0xe2c
			 0xe9abbf46, /// 0xe30
			 0xea53318f, /// 0xe34
			 0xb0b4f8b4, /// 0xe38
			 0xb0ff5475, /// 0xe3c
			 0x31462759, /// 0xe40
			 0x164eb8e8, /// 0xe44
			 0x120f99d0, /// 0xe48
			 0x42635474, /// 0xe4c
			 0xadc36748, /// 0xe50
			 0x7fe2582b, /// 0xe54
			 0x96af771a, /// 0xe58
			 0x2743effc, /// 0xe5c
			 0x6d0a6ca5, /// 0xe60
			 0xfcbdd9fd, /// 0xe64
			 0xe999799b, /// 0xe68
			 0x93ae63b8, /// 0xe6c
			 0x4ec4a583, /// 0xe70
			 0xb1b32cb1, /// 0xe74
			 0xa0763b03, /// 0xe78
			 0xcb4f7f42, /// 0xe7c
			 0x10f2ae73, /// 0xe80
			 0x507c9f2a, /// 0xe84
			 0x43ec6003, /// 0xe88
			 0x1b0fbe36, /// 0xe8c
			 0x2154d832, /// 0xe90
			 0xe31625b5, /// 0xe94
			 0x98c8133b, /// 0xe98
			 0xfad890f4, /// 0xe9c
			 0x0276670a, /// 0xea0
			 0x0dbcfba5, /// 0xea4
			 0xeb29d187, /// 0xea8
			 0x45a0adb1, /// 0xeac
			 0x8b306df7, /// 0xeb0
			 0xde7afc4d, /// 0xeb4
			 0x6dcf9f35, /// 0xeb8
			 0x3cf7f72f, /// 0xebc
			 0xd59121da, /// 0xec0
			 0x26ff0346, /// 0xec4
			 0x955f7af2, /// 0xec8
			 0xd6c1b244, /// 0xecc
			 0xc1fa5810, /// 0xed0
			 0x060d6434, /// 0xed4
			 0x4927dec3, /// 0xed8
			 0xd1cc4642, /// 0xedc
			 0x9dc5eb9f, /// 0xee0
			 0x8672f88a, /// 0xee4
			 0xab8031f0, /// 0xee8
			 0x8932d05d, /// 0xeec
			 0x6471da3d, /// 0xef0
			 0x314b6e3d, /// 0xef4
			 0x0454d0f2, /// 0xef8
			 0x4b2b8f46, /// 0xefc
			 0x4af62ef9, /// 0xf00
			 0x860e528f, /// 0xf04
			 0x9b348738, /// 0xf08
			 0x710410ad, /// 0xf0c
			 0xbdbb79b2, /// 0xf10
			 0xa82da2d0, /// 0xf14
			 0xe097f16d, /// 0xf18
			 0xc9779528, /// 0xf1c
			 0xa9ade5da, /// 0xf20
			 0xda903a81, /// 0xf24
			 0x46e28e86, /// 0xf28
			 0x90575dce, /// 0xf2c
			 0xd4619781, /// 0xf30
			 0xfd182e3b, /// 0xf34
			 0x4a0fb40d, /// 0xf38
			 0x80dceecb, /// 0xf3c
			 0x15400387, /// 0xf40
			 0xe0f771d6, /// 0xf44
			 0x8e086f24, /// 0xf48
			 0xf35e9ac2, /// 0xf4c
			 0x69f7fc2a, /// 0xf50
			 0xf59a39fc, /// 0xf54
			 0x6ec97885, /// 0xf58
			 0x2135d04a, /// 0xf5c
			 0x8346a5d0, /// 0xf60
			 0x9c398973, /// 0xf64
			 0x4f3cfed1, /// 0xf68
			 0x23963ee0, /// 0xf6c
			 0x6b5b4db2, /// 0xf70
			 0x99f84c74, /// 0xf74
			 0x54dae873, /// 0xf78
			 0xefb33cf4, /// 0xf7c
			 0xe90dcb73, /// 0xf80
			 0x52423ef6, /// 0xf84
			 0x3252f12b, /// 0xf88
			 0x31e94504, /// 0xf8c
			 0xfdb27ed2, /// 0xf90
			 0x5107a33a, /// 0xf94
			 0xaa8464d3, /// 0xf98
			 0xb7058c44, /// 0xf9c
			 0xb068787f, /// 0xfa0
			 0xc2395897, /// 0xfa4
			 0x90c905d1, /// 0xfa8
			 0x07796b54, /// 0xfac
			 0x9b2f26ae, /// 0xfb0
			 0x5f98fa35, /// 0xfb4
			 0x5c7ff4a1, /// 0xfb8
			 0xda337688, /// 0xfbc
			 0x7f6a2fb4, /// 0xfc0
			 0xceb89d17, /// 0xfc4
			 0xe21561e0, /// 0xfc8
			 0xa468a45b, /// 0xfcc
			 0xc52d8097, /// 0xfd0
			 0xa1cd2f91, /// 0xfd4
			 0xe674895a, /// 0xfd8
			 0x6dea2461, /// 0xfdc
			 0x6b200d88, /// 0xfe0
			 0x7cf679f4, /// 0xfe4
			 0x2245bc37, /// 0xfe8
			 0x7f4b1cd6, /// 0xfec
			 0x6e1752bd, /// 0xff0
			 0xc7ca2c8e, /// 0xff4
			 0xd992a5f9, /// 0xff8
			 0x7560c2d6 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00000
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00008
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0000c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00010
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00020
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00024
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00028
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x7fc00001,                                                  // signaling NaN                               /// 00034
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0xff800000,                                                  // -inf                                        /// 0003c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00048
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0004c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00054
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00058
			 0x0c400000,                                                  // Leading 1s:                                 /// 0005c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00060
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00068
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0006c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00078
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0007c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00080
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0x0c780000,                                                  // Leading 1s:                                 /// 00088
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0008c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0009c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000b8
			 0x4b000000,                                                  // 8388608.0                                   /// 000bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000c4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000d0
			 0x7f800000,                                                  // inf                                         /// 000d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 000e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00011111,                                                  // 9.7958E-41                                  /// 00104
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00108
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0010c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00110
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00118
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0011c
			 0x80000000,                                                  // -zero                                       /// 00120
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00124
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0xffc00001,                                                  // -signaling NaN                              /// 0012c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00130
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00134
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00138
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0013c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00140
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00150
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00158
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0015c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00160
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00164
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00168
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00174
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00180
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00184
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00188
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00190
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00194
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00198
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001a0
			 0x4b000000,                                                  // 8388608.0                                   /// 001a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 001c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x4b000000,                                                  // 8388608.0                                   /// 001f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00200
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00208
			 0x7fc00001,                                                  // signaling NaN                               /// 0020c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0021c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00220
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00224
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0022c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0c700000,                                                  // Leading 1s:                                 /// 00234
			 0x00011111,                                                  // 9.7958E-41                                  /// 00238
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0023c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00240
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00248
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0024c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00258
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00260
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00268
			 0x33333333,                                                  // 4 random values                             /// 0026c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00270
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00278
			 0xcb000000,                                                  // -8388608.0                                  /// 0027c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00284
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0028c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00294
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00298
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0029c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 002dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00300
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0030c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00310
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00314
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00320
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00328
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0032c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00330
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00338
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00340
			 0x0c400000,                                                  // Leading 1s:                                 /// 00344
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00348
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0034c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00350
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00368
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00370
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00374
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00378
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0037c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00384
			 0x55555555,                                                  // 4 random values                             /// 00388
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0038c
			 0xbf028f5c,                                                  // -0.51                                       /// 00390
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00394
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00398
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0c400000,                                                  // Leading 1s:                                 /// 003a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b4
			 0x7fc00001,                                                  // signaling NaN                               /// 003b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003dc
			 0x33333333,                                                  // 4 random values                             /// 003e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003f4
			 0xff800000,                                                  // -inf                                        /// 003f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00408
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00410
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00414
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00418
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0041c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00420
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00424
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00428
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0042c
			 0x7f800000,                                                  // inf                                         /// 00430
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00438
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0043c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00448
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0044c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x3f028f5c,                                                  // 0.51                                        /// 00454
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0045c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00460
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x0c400000,                                                  // Leading 1s:                                 /// 00468
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0046c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00474
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00484
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00488
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00494
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00498
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0049c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x7fc00001,                                                  // signaling NaN                               /// 004b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 004c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0xbf028f5c,                                                  // -0.51                                       /// 004e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00500
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x3f028f5c,                                                  // 0.51                                        /// 0050c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00510
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00514
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00518
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0051c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00524
			 0xbf028f5c,                                                  // -0.51                                       /// 00528
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0052c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00530
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00534
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0e000007,                                                  // Trailing 1s:                                /// 0053c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00544
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0054c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00550
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00554
			 0x0c600000,                                                  // Leading 1s:                                 /// 00558
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0055c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x0c780000,                                                  // Leading 1s:                                 /// 00564
			 0x7fc00001,                                                  // signaling NaN                               /// 00568
			 0x3f028f5c,                                                  // 0.51                                        /// 0056c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0e000001,                                                  // Trailing 1s:                                /// 00574
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00578
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0057c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00580
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00584
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00594
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00598
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0059c
			 0x33333333,                                                  // 4 random values                             /// 005a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00600
			 0x0c700000,                                                  // Leading 1s:                                 /// 00604
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00608
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0060c
			 0x7fc00001,                                                  // signaling NaN                               /// 00610
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00614
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0061c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00624
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00628
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00630
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00634
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00638
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00640
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00644
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0064c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00650
			 0xffc00001,                                                  // -signaling NaN                              /// 00654
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00658
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0065c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00664
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00668
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0066c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00670
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0067c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00688
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0068c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00694
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00698
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0069c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0xffc00001,                                                  // -signaling NaN                              /// 006c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x7fc00001,                                                  // signaling NaN                               /// 006e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 006f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x7f800000,                                                  // inf                                         /// 00700
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00708
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00710
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00718
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0071c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00720
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00724
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00728
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00730
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00734
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00738
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0073c
			 0x00000000,                                                  // zero                                        /// 00740
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00744
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00748
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00750
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00754
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00758
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0075c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00760
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00764
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0076c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00774
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00778
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00780
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00790
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00794
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00798
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0079c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0xff800000,                                                  // -inf                                        /// 007dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007f8
			 0x7f800000,                                                  // inf                                         /// 007fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00800
			 0x00000000,                                                  // zero                                        /// 00804
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00808
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0080c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00810
			 0xcb000000,                                                  // -8388608.0                                  /// 00814
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00818
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00820
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00824
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00828
			 0x0e000001,                                                  // Trailing 1s:                                /// 0082c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00834
			 0xff800000,                                                  // -inf                                        /// 00838
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00840
			 0x80011111,                                                  // -9.7958E-41                                 /// 00844
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00848
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0084c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00850
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00858
			 0x00011111,                                                  // 9.7958E-41                                  /// 0085c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0086c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00870
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00874
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00878
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0087c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00880
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00888
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00890
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00894
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00898
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0089c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 008d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x7fc00001,                                                  // signaling NaN                               /// 008d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008e0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00900
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00904
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00908
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00910
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00914
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0c600000,                                                  // Leading 1s:                                 /// 0091c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00920
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00924
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0xbf028f5c,                                                  // -0.51                                       /// 0092c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00930
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00934
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0093c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00940
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00948
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00954
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00958
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0095c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00960
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00964
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0096c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00974
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00978
			 0x00011111,                                                  // 9.7958E-41                                  /// 0097c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00980
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00984
			 0x80000000,                                                  // -zero                                       /// 00988
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0098c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00990
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00994
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0099c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 009a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 009c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a08
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a0c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a18
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a2c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a34
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a38
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a40
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a48
			 0xcb000000,                                                  // -8388608.0                                  /// 00a4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a58
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a7c
			 0xff800000,                                                  // -inf                                        /// 00a80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a8c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00aa0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00aa4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00aa8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ab4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ab8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ac0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ac4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ac8
			 0xff800000,                                                  // -inf                                        /// 00acc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ad0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ad8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00adc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ae0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ae4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00af0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00af4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00af8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b18
			 0x80000000,                                                  // -zero                                       /// 00b1c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b20
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b28
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b30
			 0xcb000000,                                                  // -8388608.0                                  /// 00b34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0xcb000000,                                                  // -8388608.0                                  /// 00b3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b50
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b54
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b58
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b78
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00b90
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b9c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ba4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ba8
			 0xffc00001,                                                  // -signaling NaN                              /// 00bac
			 0x7fc00001,                                                  // signaling NaN                               /// 00bb0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bb4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bb8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bbc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bc4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bc8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bd4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bd8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bdc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00be4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00be8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c00
			 0x3f028f5c,                                                  // 0.51                                        /// 00c04
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x4b000000,                                                  // 8388608.0                                   /// 00c0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c1c
			 0x7f800000,                                                  // inf                                         /// 00c20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c34
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c3c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c44
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c4c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c5c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c68
			 0x7f800000,                                                  // inf                                         /// 00c6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c78
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c84
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c98
			 0x4b000000,                                                  // 8388608.0                                   /// 00c9c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ca4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ca8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cb0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cb4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cb8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x4b000000,                                                  // 8388608.0                                   /// 00cc8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cd8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cdc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ce0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ce4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ce8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cf0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cf4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cf8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cfc
			 0xff800000,                                                  // -inf                                        /// 00d00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0xffc00001,                                                  // -signaling NaN                              /// 00d14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d1c
			 0x00000000,                                                  // zero                                        /// 00d20
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d24
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d28
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d38
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d50
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d54
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d64
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d70
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x80000000,                                                  // -zero                                       /// 00d80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d88
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d90
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d9c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00da0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00da8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00db0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00db4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dd4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ddc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00de0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00df0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00df4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dfc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e10
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e20
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e28
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e2c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e34
			 0x3f028f5c,                                                  // 0.51                                        /// 00e38
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e3c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e44
			 0x33333333,                                                  // 4 random values                             /// 00e48
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e50
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e6c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e88
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e98
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0xff800000,                                                  // -inf                                        /// 00ea0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ea4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ea8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ebc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ec0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ec8
			 0x7fc00001,                                                  // signaling NaN                               /// 00ecc
			 0xcb000000,                                                  // -8388608.0                                  /// 00ed0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00edc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ee0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ef0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ef4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ef8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00efc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f10
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f14
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f28
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f34
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f38
			 0x80000000,                                                  // -zero                                       /// 00f3c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f58
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f88
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f90
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f98
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f9c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fa0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fa8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fb0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x33333333,                                                  // 4 random values                             /// 00fc0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fc4
			 0xbf028f5c,                                                  // -0.51                                       /// 00fc8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fd4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fd8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x33333333,                                                  // 4 random values                             /// 00fe4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ff0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ff8
			 0x00000002 // min subnorm + 1 ulp                           // SP +ve numbers                              /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xbf199fca, /// 0x0
			 0x9eebebb0, /// 0x4
			 0x9d9f8217, /// 0x8
			 0x2cd1b526, /// 0xc
			 0x6c737017, /// 0x10
			 0x7ec6d41b, /// 0x14
			 0x74b23ac0, /// 0x18
			 0x5bf4472c, /// 0x1c
			 0x0ebe3061, /// 0x20
			 0x35b3432a, /// 0x24
			 0x17b7f510, /// 0x28
			 0x307183a3, /// 0x2c
			 0x5cc5bffb, /// 0x30
			 0xc06a61c3, /// 0x34
			 0x5c84126a, /// 0x38
			 0x5a34f20d, /// 0x3c
			 0x37086e98, /// 0x40
			 0xe8ede46e, /// 0x44
			 0x4f245c63, /// 0x48
			 0x926e060f, /// 0x4c
			 0x03d0968b, /// 0x50
			 0x4c6a8798, /// 0x54
			 0x2eaa4693, /// 0x58
			 0xa3d3fcd1, /// 0x5c
			 0x9bf8bf31, /// 0x60
			 0xb6b804e7, /// 0x64
			 0xb8fdb778, /// 0x68
			 0x229d2589, /// 0x6c
			 0xb6f12fc0, /// 0x70
			 0xbc79ff30, /// 0x74
			 0xaa54023a, /// 0x78
			 0xb4b2b6b9, /// 0x7c
			 0xd1a1cfdd, /// 0x80
			 0xd2fcec24, /// 0x84
			 0x1463ea34, /// 0x88
			 0x6d0cb947, /// 0x8c
			 0xa335a3d5, /// 0x90
			 0xee7fc483, /// 0x94
			 0x20e20d67, /// 0x98
			 0x37f47a43, /// 0x9c
			 0x48fe5ece, /// 0xa0
			 0x6ff411f2, /// 0xa4
			 0xf86c956a, /// 0xa8
			 0x0805b835, /// 0xac
			 0x0b630f25, /// 0xb0
			 0x4174bda3, /// 0xb4
			 0x70edbd0d, /// 0xb8
			 0x5fcfe98c, /// 0xbc
			 0x5df3c933, /// 0xc0
			 0xece38a9d, /// 0xc4
			 0x5347f39d, /// 0xc8
			 0x1aa71954, /// 0xcc
			 0xdd8e9a05, /// 0xd0
			 0xac646e43, /// 0xd4
			 0x58a66a00, /// 0xd8
			 0xef3e1536, /// 0xdc
			 0xcbaf4d6c, /// 0xe0
			 0x71bcc477, /// 0xe4
			 0xd5cdfe39, /// 0xe8
			 0x3ff1e23a, /// 0xec
			 0xac0308c7, /// 0xf0
			 0x36a9cd65, /// 0xf4
			 0xbd10ee37, /// 0xf8
			 0x9974c84d, /// 0xfc
			 0x5f6e6522, /// 0x100
			 0x37adecd1, /// 0x104
			 0x16c941b3, /// 0x108
			 0xb4c43bde, /// 0x10c
			 0x67f0f131, /// 0x110
			 0x0b697202, /// 0x114
			 0xc3ceb1eb, /// 0x118
			 0x4f84ab3c, /// 0x11c
			 0xb5d2f762, /// 0x120
			 0x1fde8390, /// 0x124
			 0xd8104d02, /// 0x128
			 0xf7bec3fc, /// 0x12c
			 0xd1eeec91, /// 0x130
			 0xe63de869, /// 0x134
			 0x4c7a9788, /// 0x138
			 0x34e88646, /// 0x13c
			 0xdac524b8, /// 0x140
			 0x40203744, /// 0x144
			 0xd248ed26, /// 0x148
			 0xa4a078a4, /// 0x14c
			 0x05dd40c8, /// 0x150
			 0x6cadebd0, /// 0x154
			 0x1b894cfc, /// 0x158
			 0x514a2017, /// 0x15c
			 0x798b4fab, /// 0x160
			 0x168af605, /// 0x164
			 0xca558f7e, /// 0x168
			 0x46fb91ff, /// 0x16c
			 0xecc55a84, /// 0x170
			 0x289c74f8, /// 0x174
			 0x21b46135, /// 0x178
			 0x4ec9bc84, /// 0x17c
			 0xbb676928, /// 0x180
			 0xf665c05b, /// 0x184
			 0xee01ba87, /// 0x188
			 0xf2026f27, /// 0x18c
			 0x494e4b56, /// 0x190
			 0x59add4a1, /// 0x194
			 0x1cb3b7f0, /// 0x198
			 0x82d21ef8, /// 0x19c
			 0x52f48d01, /// 0x1a0
			 0x726186d1, /// 0x1a4
			 0x0626d79f, /// 0x1a8
			 0x094aa2ae, /// 0x1ac
			 0x51d892ec, /// 0x1b0
			 0x765aaf2e, /// 0x1b4
			 0xd3ecd3dd, /// 0x1b8
			 0x2fd88932, /// 0x1bc
			 0xb51c5230, /// 0x1c0
			 0x7d4b2dd1, /// 0x1c4
			 0xc5ff3a6c, /// 0x1c8
			 0x4c94a771, /// 0x1cc
			 0x82284035, /// 0x1d0
			 0xac91d8f6, /// 0x1d4
			 0x6c3a847d, /// 0x1d8
			 0x6d83ed23, /// 0x1dc
			 0xe0510e4f, /// 0x1e0
			 0x4ed512c9, /// 0x1e4
			 0xecab78eb, /// 0x1e8
			 0xddf5cf38, /// 0x1ec
			 0x1d0ad8ef, /// 0x1f0
			 0x789d1403, /// 0x1f4
			 0xf1c165c1, /// 0x1f8
			 0x1cd5c101, /// 0x1fc
			 0x9a10462b, /// 0x200
			 0xb801efa9, /// 0x204
			 0x8e3707f4, /// 0x208
			 0x8abf5d2d, /// 0x20c
			 0xd14663c5, /// 0x210
			 0x6ace0810, /// 0x214
			 0xd9df7470, /// 0x218
			 0xabb5bac5, /// 0x21c
			 0x786f5199, /// 0x220
			 0xfea4de2c, /// 0x224
			 0x20115be9, /// 0x228
			 0xd4107ac7, /// 0x22c
			 0x9690a839, /// 0x230
			 0xfef76162, /// 0x234
			 0x81ac1c65, /// 0x238
			 0x53e16eda, /// 0x23c
			 0xabb1b6d2, /// 0x240
			 0x73d4a4bf, /// 0x244
			 0x7448906b, /// 0x248
			 0x6b0db16b, /// 0x24c
			 0xee190813, /// 0x250
			 0xd0e364d6, /// 0x254
			 0xb8b73eb2, /// 0x258
			 0xe34019fd, /// 0x25c
			 0xe4b4253b, /// 0x260
			 0x62a47542, /// 0x264
			 0xa34ac473, /// 0x268
			 0x06d24cb1, /// 0x26c
			 0x19821445, /// 0x270
			 0x253c5ea0, /// 0x274
			 0xbd18c68e, /// 0x278
			 0x4a8e9553, /// 0x27c
			 0x62f2434d, /// 0x280
			 0x5d723da4, /// 0x284
			 0xa5e7f68d, /// 0x288
			 0xd77bdf46, /// 0x28c
			 0x34e9f7f4, /// 0x290
			 0x26cd584c, /// 0x294
			 0xdf0c678f, /// 0x298
			 0xb5bef99d, /// 0x29c
			 0x7ebc48e9, /// 0x2a0
			 0x1f28ffa0, /// 0x2a4
			 0xe72e608a, /// 0x2a8
			 0x3679711d, /// 0x2ac
			 0xcb6b9dd9, /// 0x2b0
			 0xd67f60a0, /// 0x2b4
			 0x6c200eb7, /// 0x2b8
			 0x43264780, /// 0x2bc
			 0x833e46f4, /// 0x2c0
			 0x19c22a87, /// 0x2c4
			 0x33bcb719, /// 0x2c8
			 0x8db65f01, /// 0x2cc
			 0x84e7f99a, /// 0x2d0
			 0x28ca74e0, /// 0x2d4
			 0x61223efe, /// 0x2d8
			 0x33590617, /// 0x2dc
			 0xd308e692, /// 0x2e0
			 0x019b3531, /// 0x2e4
			 0xfa47fba6, /// 0x2e8
			 0xb8df00bf, /// 0x2ec
			 0x0421149c, /// 0x2f0
			 0x73f92519, /// 0x2f4
			 0x4cc454b0, /// 0x2f8
			 0x815bc5d2, /// 0x2fc
			 0x70f16592, /// 0x300
			 0x06992e0a, /// 0x304
			 0xfe6f994c, /// 0x308
			 0xff2d50fc, /// 0x30c
			 0xe66959fe, /// 0x310
			 0x8c92be40, /// 0x314
			 0x6c4672c0, /// 0x318
			 0x2cb74e88, /// 0x31c
			 0x1ed8d5a5, /// 0x320
			 0x23098339, /// 0x324
			 0x9c02999c, /// 0x328
			 0xdec3fdfd, /// 0x32c
			 0x6364b8cb, /// 0x330
			 0x9f38ec12, /// 0x334
			 0x2f0c9154, /// 0x338
			 0x52f7b6ae, /// 0x33c
			 0x85bbe658, /// 0x340
			 0x91d0310c, /// 0x344
			 0x35836e65, /// 0x348
			 0x0fa10ddc, /// 0x34c
			 0xb0b78c94, /// 0x350
			 0x3a9ca4c5, /// 0x354
			 0x3804a114, /// 0x358
			 0x1a5a4469, /// 0x35c
			 0xf93039b0, /// 0x360
			 0x8de2a536, /// 0x364
			 0xcb130e7c, /// 0x368
			 0xa95cad54, /// 0x36c
			 0xe0d4c969, /// 0x370
			 0x943e0955, /// 0x374
			 0x57532d11, /// 0x378
			 0x153a6499, /// 0x37c
			 0xc0ec06d4, /// 0x380
			 0xaf672f40, /// 0x384
			 0x0cc237c8, /// 0x388
			 0x46e3ad90, /// 0x38c
			 0x2ced142d, /// 0x390
			 0x030fae3a, /// 0x394
			 0x6271d59c, /// 0x398
			 0x1bc183b6, /// 0x39c
			 0x369518e1, /// 0x3a0
			 0x1599d0ac, /// 0x3a4
			 0xd7c318c1, /// 0x3a8
			 0x963cda34, /// 0x3ac
			 0x8356ba33, /// 0x3b0
			 0x70f1c74a, /// 0x3b4
			 0xc1520dab, /// 0x3b8
			 0x1d320819, /// 0x3bc
			 0x8e5b5491, /// 0x3c0
			 0xc0a16fe9, /// 0x3c4
			 0xa60e64eb, /// 0x3c8
			 0x31a9ca3a, /// 0x3cc
			 0x488d4df2, /// 0x3d0
			 0x93de9d2c, /// 0x3d4
			 0x14bec715, /// 0x3d8
			 0x947c0ef0, /// 0x3dc
			 0x040f13f4, /// 0x3e0
			 0x9faf3180, /// 0x3e4
			 0xe44e9862, /// 0x3e8
			 0xf9d6697a, /// 0x3ec
			 0x8f4603f7, /// 0x3f0
			 0x272a304c, /// 0x3f4
			 0xfde154e0, /// 0x3f8
			 0xa37f51c8, /// 0x3fc
			 0x52ecce85, /// 0x400
			 0xc15ec0f8, /// 0x404
			 0x6052ffee, /// 0x408
			 0xc73182b3, /// 0x40c
			 0x821e4b3e, /// 0x410
			 0xcad5e0a2, /// 0x414
			 0xd3c027a1, /// 0x418
			 0x3d1f5352, /// 0x41c
			 0xaf362db2, /// 0x420
			 0x8010089d, /// 0x424
			 0x960c0426, /// 0x428
			 0x66a6f189, /// 0x42c
			 0x936ef909, /// 0x430
			 0x5f8f9c4c, /// 0x434
			 0x4f7d50ad, /// 0x438
			 0x19049f65, /// 0x43c
			 0xa2924dd4, /// 0x440
			 0xc1f96832, /// 0x444
			 0x3f9beba1, /// 0x448
			 0x3c9a0fa1, /// 0x44c
			 0x52d6d8c4, /// 0x450
			 0x36e702ec, /// 0x454
			 0x24f81b06, /// 0x458
			 0x53f522a4, /// 0x45c
			 0x568dafc3, /// 0x460
			 0x1e0786fe, /// 0x464
			 0xa8e0ee4c, /// 0x468
			 0x3c708a6e, /// 0x46c
			 0x32d3d89a, /// 0x470
			 0xc8e03188, /// 0x474
			 0xe0a2f78e, /// 0x478
			 0x205e66c2, /// 0x47c
			 0xade19764, /// 0x480
			 0xb1c83bc9, /// 0x484
			 0x98c0b2b1, /// 0x488
			 0x661bd98f, /// 0x48c
			 0x52acc1d6, /// 0x490
			 0xfee20268, /// 0x494
			 0x2ac92c6f, /// 0x498
			 0xbc6d6c5d, /// 0x49c
			 0xcfbfc1b6, /// 0x4a0
			 0xa00e086a, /// 0x4a4
			 0x4174cbab, /// 0x4a8
			 0xc8855d4e, /// 0x4ac
			 0x7facfad2, /// 0x4b0
			 0xe7df4940, /// 0x4b4
			 0x5c6d8175, /// 0x4b8
			 0x77a7518a, /// 0x4bc
			 0xcfec3deb, /// 0x4c0
			 0xb3a19efe, /// 0x4c4
			 0x274c1ac4, /// 0x4c8
			 0x145b58da, /// 0x4cc
			 0xd4a4d3b5, /// 0x4d0
			 0xf9176626, /// 0x4d4
			 0xea2ebfb0, /// 0x4d8
			 0x97b3281c, /// 0x4dc
			 0x260c5aa9, /// 0x4e0
			 0x9f1422cc, /// 0x4e4
			 0xbc2a3be2, /// 0x4e8
			 0x53fae187, /// 0x4ec
			 0x7d4720f2, /// 0x4f0
			 0xaa188a6c, /// 0x4f4
			 0xbd850dad, /// 0x4f8
			 0x71f6ef6f, /// 0x4fc
			 0xf3f5eeec, /// 0x500
			 0xcd573da2, /// 0x504
			 0x82bfb84b, /// 0x508
			 0xe796c552, /// 0x50c
			 0x7b885e0e, /// 0x510
			 0x35478f16, /// 0x514
			 0x68ea14cf, /// 0x518
			 0x86e7eddd, /// 0x51c
			 0xf4b48f1e, /// 0x520
			 0xe746b701, /// 0x524
			 0x0d2a88bc, /// 0x528
			 0x3c7f8574, /// 0x52c
			 0xc66bdcba, /// 0x530
			 0xa817172a, /// 0x534
			 0xd3160bc1, /// 0x538
			 0x058f353e, /// 0x53c
			 0x23dce997, /// 0x540
			 0x11b606cb, /// 0x544
			 0x4e5777b9, /// 0x548
			 0x4a915c3b, /// 0x54c
			 0x4b07a528, /// 0x550
			 0xe404b5c5, /// 0x554
			 0x73bc4767, /// 0x558
			 0xac287d74, /// 0x55c
			 0x0a035205, /// 0x560
			 0x4674152b, /// 0x564
			 0x88910b04, /// 0x568
			 0xaff0d183, /// 0x56c
			 0xa21be5bb, /// 0x570
			 0xa7dd0ce4, /// 0x574
			 0x5bfcc7c5, /// 0x578
			 0xf21f74b1, /// 0x57c
			 0x5a33f379, /// 0x580
			 0xbcfd8607, /// 0x584
			 0x4e7858ca, /// 0x588
			 0x0624a20a, /// 0x58c
			 0x06d7298b, /// 0x590
			 0x0dc2b510, /// 0x594
			 0xd86e5009, /// 0x598
			 0x8383be5e, /// 0x59c
			 0xbdcc77e4, /// 0x5a0
			 0x9829e05e, /// 0x5a4
			 0x51ffde1c, /// 0x5a8
			 0x55e411ae, /// 0x5ac
			 0x5cabf2b5, /// 0x5b0
			 0x0a81818e, /// 0x5b4
			 0xd678bd00, /// 0x5b8
			 0x978dfddf, /// 0x5bc
			 0x14b53cd2, /// 0x5c0
			 0xc50b925a, /// 0x5c4
			 0x0dbbfc1c, /// 0x5c8
			 0xfd7b3352, /// 0x5cc
			 0x1e144208, /// 0x5d0
			 0x580d1fa4, /// 0x5d4
			 0xc19f5bad, /// 0x5d8
			 0xecf678bd, /// 0x5dc
			 0x4fa0e09a, /// 0x5e0
			 0x0872e28c, /// 0x5e4
			 0xe649f221, /// 0x5e8
			 0x61020abb, /// 0x5ec
			 0xce7913ac, /// 0x5f0
			 0xb793856f, /// 0x5f4
			 0x5dd0f046, /// 0x5f8
			 0x71d81747, /// 0x5fc
			 0xfed0c32d, /// 0x600
			 0x3dab5f69, /// 0x604
			 0xe2f5bf9e, /// 0x608
			 0x20890791, /// 0x60c
			 0xdadaa956, /// 0x610
			 0x0544a54c, /// 0x614
			 0xc50fe3f4, /// 0x618
			 0xe6769577, /// 0x61c
			 0xc570cdf3, /// 0x620
			 0x7c9b0ed6, /// 0x624
			 0xff561dc4, /// 0x628
			 0x6252d824, /// 0x62c
			 0x3a53c2d7, /// 0x630
			 0x4dd1ad0b, /// 0x634
			 0x74b0484c, /// 0x638
			 0x3bbf2a5a, /// 0x63c
			 0xdf495cf0, /// 0x640
			 0x6adfffad, /// 0x644
			 0xeb9e8f94, /// 0x648
			 0x5336af6a, /// 0x64c
			 0x91377a94, /// 0x650
			 0xf5ab473e, /// 0x654
			 0x38d9d8f5, /// 0x658
			 0x545babf1, /// 0x65c
			 0x956ca363, /// 0x660
			 0xacd235c4, /// 0x664
			 0x9c63ab9a, /// 0x668
			 0xe3a209d7, /// 0x66c
			 0x4a23942a, /// 0x670
			 0xbed23ed5, /// 0x674
			 0x938229c2, /// 0x678
			 0x5ea9ae3b, /// 0x67c
			 0xd2573caa, /// 0x680
			 0xbaf007a0, /// 0x684
			 0x7c183111, /// 0x688
			 0xaa588c9b, /// 0x68c
			 0x766aa04d, /// 0x690
			 0x8c4a6696, /// 0x694
			 0x6705724f, /// 0x698
			 0xba3540d7, /// 0x69c
			 0xbccb09fd, /// 0x6a0
			 0xcbff1234, /// 0x6a4
			 0xbec8e25f, /// 0x6a8
			 0xaaef31c7, /// 0x6ac
			 0x6787800b, /// 0x6b0
			 0xcb8322cf, /// 0x6b4
			 0x8b819a0d, /// 0x6b8
			 0xe104f3ce, /// 0x6bc
			 0x6b4290c9, /// 0x6c0
			 0x99da5298, /// 0x6c4
			 0x1f15cb76, /// 0x6c8
			 0x75de4e8d, /// 0x6cc
			 0xe560fefb, /// 0x6d0
			 0x232a5628, /// 0x6d4
			 0x0bf0aa34, /// 0x6d8
			 0xb7e3bac4, /// 0x6dc
			 0xd268fe9d, /// 0x6e0
			 0x7924b581, /// 0x6e4
			 0x8d1086b6, /// 0x6e8
			 0x39c43917, /// 0x6ec
			 0x4ff6aef2, /// 0x6f0
			 0x0f830faf, /// 0x6f4
			 0x27a94f51, /// 0x6f8
			 0x7ff10e66, /// 0x6fc
			 0xac8a86c6, /// 0x700
			 0x27509ddd, /// 0x704
			 0x2dfe8ae6, /// 0x708
			 0x7298f658, /// 0x70c
			 0x7ca9c86f, /// 0x710
			 0xfd92da72, /// 0x714
			 0x8ce0f553, /// 0x718
			 0x9bd9e89f, /// 0x71c
			 0x1ba88d83, /// 0x720
			 0x9548eb05, /// 0x724
			 0xd8184759, /// 0x728
			 0x60c765f2, /// 0x72c
			 0x35b822d8, /// 0x730
			 0xa4284327, /// 0x734
			 0x7291d0cd, /// 0x738
			 0x0217fd4e, /// 0x73c
			 0x10abbac0, /// 0x740
			 0x1f071e59, /// 0x744
			 0x8d507e1a, /// 0x748
			 0xafe3710a, /// 0x74c
			 0x4e9fc493, /// 0x750
			 0x60bf0c24, /// 0x754
			 0xf40bf7c5, /// 0x758
			 0x77f65bf5, /// 0x75c
			 0xcee1618a, /// 0x760
			 0xd85d5ca6, /// 0x764
			 0x37e1096c, /// 0x768
			 0xb362d1d5, /// 0x76c
			 0x5c70dbe8, /// 0x770
			 0x7e0a1222, /// 0x774
			 0xf0b2f267, /// 0x778
			 0x285b6103, /// 0x77c
			 0xf7f590a7, /// 0x780
			 0x9fcfb77d, /// 0x784
			 0x4634d863, /// 0x788
			 0xe1497580, /// 0x78c
			 0x59b75670, /// 0x790
			 0xecf9f207, /// 0x794
			 0x45045936, /// 0x798
			 0xbd71c582, /// 0x79c
			 0x9737da3c, /// 0x7a0
			 0x6a4d3138, /// 0x7a4
			 0xa085660e, /// 0x7a8
			 0xf84ae297, /// 0x7ac
			 0x1941facb, /// 0x7b0
			 0xfeef5e97, /// 0x7b4
			 0xcb57316c, /// 0x7b8
			 0x977b1221, /// 0x7bc
			 0x753a04ec, /// 0x7c0
			 0x04eae84b, /// 0x7c4
			 0x5a6c2511, /// 0x7c8
			 0x048037bf, /// 0x7cc
			 0x475a55e0, /// 0x7d0
			 0x00d52414, /// 0x7d4
			 0xd8bd6866, /// 0x7d8
			 0xdecce52b, /// 0x7dc
			 0xaa675548, /// 0x7e0
			 0xc138df7c, /// 0x7e4
			 0x15cd3917, /// 0x7e8
			 0xb1d726fe, /// 0x7ec
			 0x32bd280d, /// 0x7f0
			 0x2f168224, /// 0x7f4
			 0x6ea537e5, /// 0x7f8
			 0xe1d6e21b, /// 0x7fc
			 0x931d16ff, /// 0x800
			 0xe502d1c9, /// 0x804
			 0xfb9e33e0, /// 0x808
			 0xbc9a751d, /// 0x80c
			 0x00c1c824, /// 0x810
			 0xb23d831f, /// 0x814
			 0xd6fc2b64, /// 0x818
			 0x3278c6d3, /// 0x81c
			 0xd26e4b10, /// 0x820
			 0x3d034155, /// 0x824
			 0xc47ba098, /// 0x828
			 0xf9232bfd, /// 0x82c
			 0x6f4ce22c, /// 0x830
			 0x4d54f2ee, /// 0x834
			 0x53f6a822, /// 0x838
			 0x0a903597, /// 0x83c
			 0x312fd434, /// 0x840
			 0xf83861ec, /// 0x844
			 0xa26f9908, /// 0x848
			 0x48fb3524, /// 0x84c
			 0xb178a74c, /// 0x850
			 0xb36c9a48, /// 0x854
			 0xe7bffc22, /// 0x858
			 0x1145ba0a, /// 0x85c
			 0xfe34b288, /// 0x860
			 0x0b88abad, /// 0x864
			 0xb2be17d0, /// 0x868
			 0x61be8577, /// 0x86c
			 0x07a2d7d3, /// 0x870
			 0x889207fb, /// 0x874
			 0x6d7fd3b8, /// 0x878
			 0x7f999c5b, /// 0x87c
			 0xb3655183, /// 0x880
			 0x396cde9f, /// 0x884
			 0x468678f1, /// 0x888
			 0x1bdf9c27, /// 0x88c
			 0xfb250595, /// 0x890
			 0xbfdf2fae, /// 0x894
			 0x47394251, /// 0x898
			 0x4c8074a1, /// 0x89c
			 0x811bb7a7, /// 0x8a0
			 0xca7d6ef7, /// 0x8a4
			 0x9d06c416, /// 0x8a8
			 0xeab6474c, /// 0x8ac
			 0xfb7d7913, /// 0x8b0
			 0x0bc955d8, /// 0x8b4
			 0xef22d6ec, /// 0x8b8
			 0xc7983742, /// 0x8bc
			 0x01cb3191, /// 0x8c0
			 0xe9365c9c, /// 0x8c4
			 0xdfbd1363, /// 0x8c8
			 0x270c51f1, /// 0x8cc
			 0xbc6ca74b, /// 0x8d0
			 0x8f58244d, /// 0x8d4
			 0xc1bce2e7, /// 0x8d8
			 0xabc677ae, /// 0x8dc
			 0x1e3342d8, /// 0x8e0
			 0xf60572b8, /// 0x8e4
			 0xee46b0c4, /// 0x8e8
			 0xd4712330, /// 0x8ec
			 0xa0637f79, /// 0x8f0
			 0x3453345a, /// 0x8f4
			 0x66c3dc04, /// 0x8f8
			 0xc4759b64, /// 0x8fc
			 0x2933926c, /// 0x900
			 0x517b0dcd, /// 0x904
			 0xdb193709, /// 0x908
			 0x0f57778a, /// 0x90c
			 0x57024b4b, /// 0x910
			 0xbec1453f, /// 0x914
			 0x08c8d74e, /// 0x918
			 0x606d8b2a, /// 0x91c
			 0xf3443386, /// 0x920
			 0x458f1aba, /// 0x924
			 0x8f8ddefa, /// 0x928
			 0xef120616, /// 0x92c
			 0xbdf5b11d, /// 0x930
			 0xceaa65f6, /// 0x934
			 0xe4f7e09c, /// 0x938
			 0x35771759, /// 0x93c
			 0xb04e2017, /// 0x940
			 0x52c45a98, /// 0x944
			 0x1a74be3c, /// 0x948
			 0xdb755fd7, /// 0x94c
			 0x081eb193, /// 0x950
			 0x6f006aae, /// 0x954
			 0x18a8cfd1, /// 0x958
			 0x1930c1fa, /// 0x95c
			 0xb8383dcb, /// 0x960
			 0xd4a97af6, /// 0x964
			 0xad5e8708, /// 0x968
			 0xd1ae824b, /// 0x96c
			 0x91b85a15, /// 0x970
			 0x3c0632ce, /// 0x974
			 0xded2b3f1, /// 0x978
			 0x7e932990, /// 0x97c
			 0x213af845, /// 0x980
			 0x91e28459, /// 0x984
			 0xb0d019c5, /// 0x988
			 0xdd3c2251, /// 0x98c
			 0x77132039, /// 0x990
			 0x6c0f500b, /// 0x994
			 0x4f57ca65, /// 0x998
			 0xefb911c2, /// 0x99c
			 0x034d0d83, /// 0x9a0
			 0x2d501987, /// 0x9a4
			 0xff7081fa, /// 0x9a8
			 0x1d800145, /// 0x9ac
			 0x4fef4328, /// 0x9b0
			 0x8c766728, /// 0x9b4
			 0x6cb15f68, /// 0x9b8
			 0xe7c85878, /// 0x9bc
			 0x243124ec, /// 0x9c0
			 0xa1b03eed, /// 0x9c4
			 0x1364c67d, /// 0x9c8
			 0x9b73c8ff, /// 0x9cc
			 0x129909a1, /// 0x9d0
			 0x392efb75, /// 0x9d4
			 0x3f04513c, /// 0x9d8
			 0xd00be6d5, /// 0x9dc
			 0x35a7dd1a, /// 0x9e0
			 0x5be79bf4, /// 0x9e4
			 0xe597d786, /// 0x9e8
			 0xf046305e, /// 0x9ec
			 0x18c11757, /// 0x9f0
			 0x8a98029a, /// 0x9f4
			 0xfc19c89e, /// 0x9f8
			 0x2e15f634, /// 0x9fc
			 0x3f403474, /// 0xa00
			 0x74700e15, /// 0xa04
			 0x1c49a0ee, /// 0xa08
			 0xc6105a94, /// 0xa0c
			 0xad77bd16, /// 0xa10
			 0x868f03f0, /// 0xa14
			 0x9cab50f3, /// 0xa18
			 0x282b5bb4, /// 0xa1c
			 0x42a398af, /// 0xa20
			 0x5bee5859, /// 0xa24
			 0xd4700683, /// 0xa28
			 0xacd2c885, /// 0xa2c
			 0xcdd01468, /// 0xa30
			 0x3144dc55, /// 0xa34
			 0x7af74800, /// 0xa38
			 0xe4c38db1, /// 0xa3c
			 0xffd91895, /// 0xa40
			 0x934768cd, /// 0xa44
			 0xa5435031, /// 0xa48
			 0xede5310e, /// 0xa4c
			 0x48fca6e0, /// 0xa50
			 0x1e80224d, /// 0xa54
			 0x29b929a7, /// 0xa58
			 0x1dc3176b, /// 0xa5c
			 0x71a22661, /// 0xa60
			 0x9ef6aa8f, /// 0xa64
			 0xe82953ec, /// 0xa68
			 0x2526b4d5, /// 0xa6c
			 0x18fe7986, /// 0xa70
			 0xe525263a, /// 0xa74
			 0xaaa07a5c, /// 0xa78
			 0xa4e4f435, /// 0xa7c
			 0x7d042ed3, /// 0xa80
			 0xd88790f3, /// 0xa84
			 0x4da92a47, /// 0xa88
			 0xe309be37, /// 0xa8c
			 0x0c5d76ed, /// 0xa90
			 0xc52c92af, /// 0xa94
			 0x7fffde2f, /// 0xa98
			 0xd0d9b82e, /// 0xa9c
			 0x1d1f8afb, /// 0xaa0
			 0x4e098cec, /// 0xaa4
			 0xb54ed796, /// 0xaa8
			 0xd2cf387b, /// 0xaac
			 0x0c946d32, /// 0xab0
			 0xacfc32da, /// 0xab4
			 0x89c0d7eb, /// 0xab8
			 0x997cf254, /// 0xabc
			 0x07c54d7a, /// 0xac0
			 0xdf8b4f36, /// 0xac4
			 0xffbc9dda, /// 0xac8
			 0xa76db910, /// 0xacc
			 0x614ed3bd, /// 0xad0
			 0xffcb82cf, /// 0xad4
			 0xec696888, /// 0xad8
			 0x3ae0ea55, /// 0xadc
			 0xc7c4f1ef, /// 0xae0
			 0xac75ed62, /// 0xae4
			 0xce815897, /// 0xae8
			 0xd5a4af70, /// 0xaec
			 0xb3541364, /// 0xaf0
			 0x9fd3f4f3, /// 0xaf4
			 0x96e16ae1, /// 0xaf8
			 0xe204c75e, /// 0xafc
			 0xbd936c31, /// 0xb00
			 0xf7bbd150, /// 0xb04
			 0x9e0d477c, /// 0xb08
			 0x50c0645d, /// 0xb0c
			 0xc1399507, /// 0xb10
			 0x43af0f78, /// 0xb14
			 0x1ef0f2d8, /// 0xb18
			 0xed307dfe, /// 0xb1c
			 0x139d4e09, /// 0xb20
			 0x7e371729, /// 0xb24
			 0x5eadddf4, /// 0xb28
			 0x5f5beb71, /// 0xb2c
			 0x06f03d46, /// 0xb30
			 0xdb136812, /// 0xb34
			 0x9cb6328c, /// 0xb38
			 0x4674d043, /// 0xb3c
			 0x91e8e9c8, /// 0xb40
			 0x1ce39692, /// 0xb44
			 0xc9b0ff6b, /// 0xb48
			 0x979569af, /// 0xb4c
			 0xfa61b847, /// 0xb50
			 0x776cd6d4, /// 0xb54
			 0x1b847301, /// 0xb58
			 0x4d330689, /// 0xb5c
			 0x8bb1f230, /// 0xb60
			 0x014a65fa, /// 0xb64
			 0x3c7e6051, /// 0xb68
			 0x135d0f88, /// 0xb6c
			 0x4e30b01d, /// 0xb70
			 0x2de27d13, /// 0xb74
			 0x4f0aba63, /// 0xb78
			 0x26c0961d, /// 0xb7c
			 0x3bb95a35, /// 0xb80
			 0xc1665716, /// 0xb84
			 0x17151880, /// 0xb88
			 0x3ee7f79d, /// 0xb8c
			 0xdff62891, /// 0xb90
			 0x95f7cd2a, /// 0xb94
			 0xae9b2648, /// 0xb98
			 0x41948d25, /// 0xb9c
			 0xdb9730cf, /// 0xba0
			 0xbc2c369a, /// 0xba4
			 0xea45e26c, /// 0xba8
			 0x90d84f85, /// 0xbac
			 0x0baff850, /// 0xbb0
			 0x2c438612, /// 0xbb4
			 0x049bf77b, /// 0xbb8
			 0x0fccf75a, /// 0xbbc
			 0xa2e6dbb4, /// 0xbc0
			 0x2318cf4b, /// 0xbc4
			 0x1863546d, /// 0xbc8
			 0xf5a304b4, /// 0xbcc
			 0x950941c4, /// 0xbd0
			 0x2d65e967, /// 0xbd4
			 0x5b2baffa, /// 0xbd8
			 0xf0760200, /// 0xbdc
			 0xcf1296d6, /// 0xbe0
			 0x0ab881af, /// 0xbe4
			 0x5d1bf1c8, /// 0xbe8
			 0x81fc19b8, /// 0xbec
			 0x6d73d0a2, /// 0xbf0
			 0x005633d7, /// 0xbf4
			 0x3f72c2ec, /// 0xbf8
			 0x8822efc7, /// 0xbfc
			 0x9f645c02, /// 0xc00
			 0x7b531beb, /// 0xc04
			 0x5a23c51d, /// 0xc08
			 0x67fb6d67, /// 0xc0c
			 0xb5ecd8c4, /// 0xc10
			 0x9c7d1590, /// 0xc14
			 0xa63ddedd, /// 0xc18
			 0x7e4d49a7, /// 0xc1c
			 0x8fb4da0a, /// 0xc20
			 0x2cb8fa0f, /// 0xc24
			 0x51dae4f9, /// 0xc28
			 0xbb6b4524, /// 0xc2c
			 0xd2105e9d, /// 0xc30
			 0x7998ce22, /// 0xc34
			 0x0747e9f7, /// 0xc38
			 0x7f827309, /// 0xc3c
			 0xb2df2634, /// 0xc40
			 0xa8004938, /// 0xc44
			 0xc0540f32, /// 0xc48
			 0x194e5431, /// 0xc4c
			 0x4bbe3af1, /// 0xc50
			 0x6b55a943, /// 0xc54
			 0x53cbcaef, /// 0xc58
			 0x02596568, /// 0xc5c
			 0x0848700e, /// 0xc60
			 0x0f07295b, /// 0xc64
			 0x83b02f14, /// 0xc68
			 0x1dc16bbe, /// 0xc6c
			 0x33056b9d, /// 0xc70
			 0xbb8566da, /// 0xc74
			 0x1be4bdb5, /// 0xc78
			 0x783fdd80, /// 0xc7c
			 0x8cd034db, /// 0xc80
			 0x5bbdd24f, /// 0xc84
			 0xc02a5111, /// 0xc88
			 0x8fac9476, /// 0xc8c
			 0xfa9f8cbb, /// 0xc90
			 0xf0f12787, /// 0xc94
			 0xe1812432, /// 0xc98
			 0xbbace2b7, /// 0xc9c
			 0xa792e3a1, /// 0xca0
			 0xde259ff9, /// 0xca4
			 0x2cede979, /// 0xca8
			 0x758e7c23, /// 0xcac
			 0x60784b0e, /// 0xcb0
			 0x92b9899e, /// 0xcb4
			 0x6d7f0ba7, /// 0xcb8
			 0x5bad6eea, /// 0xcbc
			 0xbabb3d01, /// 0xcc0
			 0x9992a8d8, /// 0xcc4
			 0x846ebca1, /// 0xcc8
			 0x58ce854e, /// 0xccc
			 0x2ed99067, /// 0xcd0
			 0xf47d339d, /// 0xcd4
			 0x8471a9cf, /// 0xcd8
			 0xcf511478, /// 0xcdc
			 0x61152877, /// 0xce0
			 0xd86fc60c, /// 0xce4
			 0xec5b02cc, /// 0xce8
			 0x44bd6e32, /// 0xcec
			 0x40825e4a, /// 0xcf0
			 0x5eedaf01, /// 0xcf4
			 0xf9804cba, /// 0xcf8
			 0x8436b6ed, /// 0xcfc
			 0x63c9dc8d, /// 0xd00
			 0x3cb973a5, /// 0xd04
			 0xb27fbb2b, /// 0xd08
			 0x2f27fdca, /// 0xd0c
			 0x0969d9e4, /// 0xd10
			 0x2839139a, /// 0xd14
			 0xa00f6660, /// 0xd18
			 0xee23193c, /// 0xd1c
			 0x155de8ee, /// 0xd20
			 0x21bd7145, /// 0xd24
			 0xce020d61, /// 0xd28
			 0xa33aac42, /// 0xd2c
			 0x70a26615, /// 0xd30
			 0x00be8314, /// 0xd34
			 0x97da99f4, /// 0xd38
			 0x47531923, /// 0xd3c
			 0xb88327bc, /// 0xd40
			 0xb8220740, /// 0xd44
			 0x7a524d89, /// 0xd48
			 0x5631721e, /// 0xd4c
			 0xb42fed8c, /// 0xd50
			 0x769d3e97, /// 0xd54
			 0xf4fba3e4, /// 0xd58
			 0xd5f11626, /// 0xd5c
			 0x1bee3e5c, /// 0xd60
			 0xabea308f, /// 0xd64
			 0x1cf82837, /// 0xd68
			 0x8f73e5ed, /// 0xd6c
			 0xcb5c1d57, /// 0xd70
			 0x7bb41f21, /// 0xd74
			 0xf291c8c7, /// 0xd78
			 0x0f7f5cfd, /// 0xd7c
			 0x38996b94, /// 0xd80
			 0x88641e04, /// 0xd84
			 0xf8a4a11b, /// 0xd88
			 0x513c592c, /// 0xd8c
			 0x00b835db, /// 0xd90
			 0xa51b5c6f, /// 0xd94
			 0xaa024b33, /// 0xd98
			 0x64e71f22, /// 0xd9c
			 0x643cc5d5, /// 0xda0
			 0x017d6749, /// 0xda4
			 0x551469d2, /// 0xda8
			 0xc6e4b4e4, /// 0xdac
			 0x1143cc66, /// 0xdb0
			 0xc6973b13, /// 0xdb4
			 0x1d2ef6c6, /// 0xdb8
			 0xcb2d30b5, /// 0xdbc
			 0x6918911a, /// 0xdc0
			 0xda5fbed0, /// 0xdc4
			 0xe83b1c09, /// 0xdc8
			 0xf1752bab, /// 0xdcc
			 0x0ae3aeda, /// 0xdd0
			 0xfcdb194c, /// 0xdd4
			 0x09170015, /// 0xdd8
			 0xe3cbca26, /// 0xddc
			 0xf0cce73a, /// 0xde0
			 0x4fa058a0, /// 0xde4
			 0xee4f5392, /// 0xde8
			 0xe4b34164, /// 0xdec
			 0xf5392fe3, /// 0xdf0
			 0x44476551, /// 0xdf4
			 0xbb989a69, /// 0xdf8
			 0xa2c8543f, /// 0xdfc
			 0x7f9eb567, /// 0xe00
			 0x15aaeae2, /// 0xe04
			 0x9e194a2c, /// 0xe08
			 0xe3c9ca95, /// 0xe0c
			 0xfa65dba4, /// 0xe10
			 0x049efc67, /// 0xe14
			 0xc89adc22, /// 0xe18
			 0x9192b76e, /// 0xe1c
			 0x86901874, /// 0xe20
			 0xfa069101, /// 0xe24
			 0x05e37050, /// 0xe28
			 0xc8b4ae27, /// 0xe2c
			 0x88a5a2f4, /// 0xe30
			 0xbfd6f749, /// 0xe34
			 0x01800456, /// 0xe38
			 0xe0c58711, /// 0xe3c
			 0x018739cc, /// 0xe40
			 0xc0da60ef, /// 0xe44
			 0xe19bd5a9, /// 0xe48
			 0xf82f6657, /// 0xe4c
			 0x1d4a1b2b, /// 0xe50
			 0x37091c50, /// 0xe54
			 0x71b572d9, /// 0xe58
			 0x89a987d9, /// 0xe5c
			 0x8e7d5a0b, /// 0xe60
			 0x69aad570, /// 0xe64
			 0x08db6778, /// 0xe68
			 0x97182474, /// 0xe6c
			 0x66d75e40, /// 0xe70
			 0xccc45839, /// 0xe74
			 0xc336f584, /// 0xe78
			 0xe176bf4b, /// 0xe7c
			 0x2126f4eb, /// 0xe80
			 0x3bf0d82a, /// 0xe84
			 0x43fafb08, /// 0xe88
			 0xae556291, /// 0xe8c
			 0xcf0938e7, /// 0xe90
			 0xae16b25d, /// 0xe94
			 0xc563eb41, /// 0xe98
			 0x1ed54e11, /// 0xe9c
			 0xaa63eb82, /// 0xea0
			 0xeeb52ee4, /// 0xea4
			 0x679c3ba4, /// 0xea8
			 0x7262dc31, /// 0xeac
			 0x9bd1e6b5, /// 0xeb0
			 0x797f67e8, /// 0xeb4
			 0x2266ec40, /// 0xeb8
			 0x64cb54d2, /// 0xebc
			 0xfd3b0825, /// 0xec0
			 0x105113fe, /// 0xec4
			 0x4b3d1f90, /// 0xec8
			 0xed6899a4, /// 0xecc
			 0x79e53b54, /// 0xed0
			 0xcaed87e1, /// 0xed4
			 0x855d10e5, /// 0xed8
			 0x1aeedad9, /// 0xedc
			 0x1f56f304, /// 0xee0
			 0x3401702d, /// 0xee4
			 0xa9f38459, /// 0xee8
			 0xd1e7fbb1, /// 0xeec
			 0x674429ed, /// 0xef0
			 0xce01d2f2, /// 0xef4
			 0x9a5ec204, /// 0xef8
			 0xbfdc86cc, /// 0xefc
			 0x73914781, /// 0xf00
			 0x638b0908, /// 0xf04
			 0x0dc29b27, /// 0xf08
			 0xca2d20de, /// 0xf0c
			 0x7dc62cdd, /// 0xf10
			 0x1db0875e, /// 0xf14
			 0xcb471f58, /// 0xf18
			 0x22af48ca, /// 0xf1c
			 0xf0ab945e, /// 0xf20
			 0x622522d8, /// 0xf24
			 0x4c5735c9, /// 0xf28
			 0x923d5125, /// 0xf2c
			 0x51ae191b, /// 0xf30
			 0x492e4052, /// 0xf34
			 0x34762990, /// 0xf38
			 0x0841fe4b, /// 0xf3c
			 0x8eaddc8f, /// 0xf40
			 0xdaf29e7d, /// 0xf44
			 0x431f6667, /// 0xf48
			 0x2c6d2900, /// 0xf4c
			 0x0abd0837, /// 0xf50
			 0x3fcb90b6, /// 0xf54
			 0xc731cf02, /// 0xf58
			 0x3e801233, /// 0xf5c
			 0xd09fd9ef, /// 0xf60
			 0x26381416, /// 0xf64
			 0x2c223071, /// 0xf68
			 0xa45dabba, /// 0xf6c
			 0x6a66430e, /// 0xf70
			 0xd523dca8, /// 0xf74
			 0x3e5966f3, /// 0xf78
			 0x27f4c6af, /// 0xf7c
			 0x2982e42f, /// 0xf80
			 0x463ef110, /// 0xf84
			 0x6c6626b3, /// 0xf88
			 0xe15aa002, /// 0xf8c
			 0xa1c3fdd7, /// 0xf90
			 0x77102fd6, /// 0xf94
			 0xfe6b60c1, /// 0xf98
			 0x77da5d4d, /// 0xf9c
			 0x307aa585, /// 0xfa0
			 0xf667427a, /// 0xfa4
			 0xb74eab76, /// 0xfa8
			 0x546c7e46, /// 0xfac
			 0x5d00555e, /// 0xfb0
			 0xe04353c9, /// 0xfb4
			 0xfa57c2a1, /// 0xfb8
			 0x6c53c457, /// 0xfbc
			 0xedb33abd, /// 0xfc0
			 0x77ad6b1d, /// 0xfc4
			 0xeb6bff84, /// 0xfc8
			 0xd61357f1, /// 0xfcc
			 0x68c8e2ce, /// 0xfd0
			 0x594f48a6, /// 0xfd4
			 0xa84172e2, /// 0xfd8
			 0xc7c60695, /// 0xfdc
			 0x477fec42, /// 0xfe0
			 0x0f7e7b98, /// 0xfe4
			 0x0ebc44f9, /// 0xfe8
			 0x1e062eff, /// 0xfec
			 0x2cdbb950, /// 0xff0
			 0x8f95ae44, /// 0xff4
			 0xd577baae, /// 0xff8
			 0x630405cb /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x4b396a75, /// 0x0
			 0x297e84d3, /// 0x4
			 0xd0ae1eb7, /// 0x8
			 0x69722089, /// 0xc
			 0x61c97956, /// 0x10
			 0x814965b8, /// 0x14
			 0x150dd0ed, /// 0x18
			 0xe02621e1, /// 0x1c
			 0x832b8ee8, /// 0x20
			 0xf06fc24e, /// 0x24
			 0xcf3fe0ad, /// 0x28
			 0xd1c3f086, /// 0x2c
			 0x1be0454a, /// 0x30
			 0x6795e308, /// 0x34
			 0xf04e4a46, /// 0x38
			 0xae8e4edb, /// 0x3c
			 0xcce648c9, /// 0x40
			 0x939c129c, /// 0x44
			 0x682f7ecf, /// 0x48
			 0x1fc050f9, /// 0x4c
			 0xe9b4ce37, /// 0x50
			 0x6d0dff66, /// 0x54
			 0x16228ba7, /// 0x58
			 0x53787d93, /// 0x5c
			 0x67686753, /// 0x60
			 0x6ca18c09, /// 0x64
			 0x904b9ed7, /// 0x68
			 0x3754b5bd, /// 0x6c
			 0x1b3731a1, /// 0x70
			 0xc593ec78, /// 0x74
			 0xd2151eaf, /// 0x78
			 0xfba54f51, /// 0x7c
			 0xe42b86cc, /// 0x80
			 0x10a90483, /// 0x84
			 0xe75044a7, /// 0x88
			 0x45e3126b, /// 0x8c
			 0xbc1f94a4, /// 0x90
			 0x86592a17, /// 0x94
			 0x95c7ccd4, /// 0x98
			 0xb95d8d49, /// 0x9c
			 0xc11e4357, /// 0xa0
			 0x402d7e65, /// 0xa4
			 0xaf1ff00f, /// 0xa8
			 0x706681fb, /// 0xac
			 0x14e05dbf, /// 0xb0
			 0x0340add5, /// 0xb4
			 0x934d0fd1, /// 0xb8
			 0x758128b6, /// 0xbc
			 0x68fecbfe, /// 0xc0
			 0xa9c8dcd6, /// 0xc4
			 0x9c00cf7e, /// 0xc8
			 0x5ec3eeea, /// 0xcc
			 0xe3649d52, /// 0xd0
			 0x76621eb2, /// 0xd4
			 0xf52702eb, /// 0xd8
			 0xd9c05106, /// 0xdc
			 0xd4becbf2, /// 0xe0
			 0x02698297, /// 0xe4
			 0xbce06a1d, /// 0xe8
			 0x86e21ffc, /// 0xec
			 0x067a6404, /// 0xf0
			 0x4edee7a0, /// 0xf4
			 0x57c494cd, /// 0xf8
			 0x99900053, /// 0xfc
			 0xbe4f4b9d, /// 0x100
			 0xd04b7f6f, /// 0x104
			 0x394d0d89, /// 0x108
			 0x7606d2fe, /// 0x10c
			 0xca6bc29b, /// 0x110
			 0x540b5e16, /// 0x114
			 0x3804faea, /// 0x118
			 0x233a15b9, /// 0x11c
			 0x7e0eab1f, /// 0x120
			 0xba582b2f, /// 0x124
			 0x2bd7cd7b, /// 0x128
			 0x08ccef81, /// 0x12c
			 0xb83aa1da, /// 0x130
			 0x5057ffac, /// 0x134
			 0xa4dd317a, /// 0x138
			 0x0acaf0be, /// 0x13c
			 0x8a2c574e, /// 0x140
			 0x6b580c62, /// 0x144
			 0xe382723f, /// 0x148
			 0x21d96f53, /// 0x14c
			 0x2af60b98, /// 0x150
			 0x1f92cb08, /// 0x154
			 0x01f1b019, /// 0x158
			 0x34df4638, /// 0x15c
			 0x4efca098, /// 0x160
			 0xe844966e, /// 0x164
			 0x5c0b67c2, /// 0x168
			 0xb28de849, /// 0x16c
			 0x96f447c6, /// 0x170
			 0x7a13fee2, /// 0x174
			 0xaf27ccc8, /// 0x178
			 0x0ee0c780, /// 0x17c
			 0x20daf270, /// 0x180
			 0x601858e2, /// 0x184
			 0x79ecb7ce, /// 0x188
			 0xbb943144, /// 0x18c
			 0xee45e045, /// 0x190
			 0x448c8ad6, /// 0x194
			 0x54424495, /// 0x198
			 0x720b6532, /// 0x19c
			 0xb7d4cf1b, /// 0x1a0
			 0xd182fd26, /// 0x1a4
			 0xe2773900, /// 0x1a8
			 0x667cf252, /// 0x1ac
			 0x395aef51, /// 0x1b0
			 0x7dd2ab65, /// 0x1b4
			 0xc8eb5e9f, /// 0x1b8
			 0xe11a375e, /// 0x1bc
			 0x6f3e2dfb, /// 0x1c0
			 0x5068dedf, /// 0x1c4
			 0xcd19073a, /// 0x1c8
			 0xff52fbca, /// 0x1cc
			 0xd17d5c04, /// 0x1d0
			 0x11f89cba, /// 0x1d4
			 0xbe105466, /// 0x1d8
			 0x927c432e, /// 0x1dc
			 0x1b17da87, /// 0x1e0
			 0x6bb6ad46, /// 0x1e4
			 0x8c760e02, /// 0x1e8
			 0xf14b5d7c, /// 0x1ec
			 0xf30f86b2, /// 0x1f0
			 0x318cc5a5, /// 0x1f4
			 0x6ce0658c, /// 0x1f8
			 0xa551d799, /// 0x1fc
			 0x33cc6f58, /// 0x200
			 0x874bd2e8, /// 0x204
			 0xdf50fdce, /// 0x208
			 0xa5b70cc0, /// 0x20c
			 0x3ddffd06, /// 0x210
			 0x8ded718f, /// 0x214
			 0x30e0f806, /// 0x218
			 0x083632b0, /// 0x21c
			 0x5578bc16, /// 0x220
			 0xe21c75ff, /// 0x224
			 0x70e3e97d, /// 0x228
			 0x41c6f0dd, /// 0x22c
			 0xc9493eee, /// 0x230
			 0x988d2ce5, /// 0x234
			 0xb93b3450, /// 0x238
			 0x533a7694, /// 0x23c
			 0x14198902, /// 0x240
			 0x518b6206, /// 0x244
			 0xfe3cccfe, /// 0x248
			 0x60efaa4e, /// 0x24c
			 0x9cb30f0a, /// 0x250
			 0x2f220a6e, /// 0x254
			 0xbd6d005b, /// 0x258
			 0xf8a6c094, /// 0x25c
			 0x24ebdc40, /// 0x260
			 0xd4c48b5c, /// 0x264
			 0xaabb11d8, /// 0x268
			 0x774e1435, /// 0x26c
			 0xc0d3d171, /// 0x270
			 0xde99f5ec, /// 0x274
			 0x96a1af98, /// 0x278
			 0x5cacd44a, /// 0x27c
			 0x57b081e3, /// 0x280
			 0x2e43dfcd, /// 0x284
			 0xbee89497, /// 0x288
			 0x84393d0c, /// 0x28c
			 0xd4985908, /// 0x290
			 0x625fa87a, /// 0x294
			 0x7dac2114, /// 0x298
			 0x7c0e1e7c, /// 0x29c
			 0xdcbaf646, /// 0x2a0
			 0x871b680d, /// 0x2a4
			 0x0d56f90f, /// 0x2a8
			 0xcd68a55e, /// 0x2ac
			 0x86f8d727, /// 0x2b0
			 0xb11f6322, /// 0x2b4
			 0xb0112904, /// 0x2b8
			 0x1c96f387, /// 0x2bc
			 0xb53eef8e, /// 0x2c0
			 0x849f3f5b, /// 0x2c4
			 0x3cd676f5, /// 0x2c8
			 0x9b10c475, /// 0x2cc
			 0x3b5d9c93, /// 0x2d0
			 0x4f5ef73a, /// 0x2d4
			 0xc7bc3643, /// 0x2d8
			 0x919bdf1f, /// 0x2dc
			 0xac0a5b48, /// 0x2e0
			 0xe9d26184, /// 0x2e4
			 0x3e0df2e0, /// 0x2e8
			 0x0167e6e0, /// 0x2ec
			 0xe0180d44, /// 0x2f0
			 0xf24ca2d4, /// 0x2f4
			 0xf1c4a020, /// 0x2f8
			 0x98efc15f, /// 0x2fc
			 0x6fcb18f5, /// 0x300
			 0x027ad048, /// 0x304
			 0x2fda203b, /// 0x308
			 0xc0f04088, /// 0x30c
			 0x40e8700a, /// 0x310
			 0xd2d082e9, /// 0x314
			 0xec649444, /// 0x318
			 0xe3289d8e, /// 0x31c
			 0xa36bddaa, /// 0x320
			 0x40e2d25d, /// 0x324
			 0x387a31f5, /// 0x328
			 0x5a5f060f, /// 0x32c
			 0xb2be5aa9, /// 0x330
			 0x42108cf6, /// 0x334
			 0x3ad7b09c, /// 0x338
			 0x6a036071, /// 0x33c
			 0x8fd9d137, /// 0x340
			 0x8bb67a3e, /// 0x344
			 0x67a11437, /// 0x348
			 0x93cf828b, /// 0x34c
			 0xf6e5de58, /// 0x350
			 0x6aad8f24, /// 0x354
			 0x1d9ec40f, /// 0x358
			 0x8cb14ecd, /// 0x35c
			 0xd3dfb240, /// 0x360
			 0xdb348ed9, /// 0x364
			 0x03d70351, /// 0x368
			 0x70309e51, /// 0x36c
			 0x1956beed, /// 0x370
			 0x4cf32d35, /// 0x374
			 0x04064def, /// 0x378
			 0x7a0d9d68, /// 0x37c
			 0x58f827c8, /// 0x380
			 0xf17cd535, /// 0x384
			 0x6aa62d88, /// 0x388
			 0x34fddc85, /// 0x38c
			 0x160c945d, /// 0x390
			 0x6c1e77be, /// 0x394
			 0xb83498ee, /// 0x398
			 0x2ac2d54d, /// 0x39c
			 0x19a2b588, /// 0x3a0
			 0xcf3d3cc8, /// 0x3a4
			 0x20d475b7, /// 0x3a8
			 0x0b3197f6, /// 0x3ac
			 0x95b1cc12, /// 0x3b0
			 0x9777345c, /// 0x3b4
			 0x799ef3fa, /// 0x3b8
			 0x7030b27e, /// 0x3bc
			 0xb066211c, /// 0x3c0
			 0xb9f206c2, /// 0x3c4
			 0xb3bb14c1, /// 0x3c8
			 0xb2fd2e78, /// 0x3cc
			 0x1bb39f0f, /// 0x3d0
			 0x5b250950, /// 0x3d4
			 0xc19be233, /// 0x3d8
			 0xc8ee54e0, /// 0x3dc
			 0xd4da1b88, /// 0x3e0
			 0x6d22fd19, /// 0x3e4
			 0x83f0d808, /// 0x3e8
			 0x6dc8fd23, /// 0x3ec
			 0x1942cb47, /// 0x3f0
			 0x31928806, /// 0x3f4
			 0x3d3ea236, /// 0x3f8
			 0x3aac61b0, /// 0x3fc
			 0xd455bf4c, /// 0x400
			 0xc82cb53e, /// 0x404
			 0x933a0465, /// 0x408
			 0x6c2dc8e2, /// 0x40c
			 0x4b52b4d9, /// 0x410
			 0x0039439a, /// 0x414
			 0x6175e808, /// 0x418
			 0xe4afeb70, /// 0x41c
			 0x8360b4df, /// 0x420
			 0x8959e338, /// 0x424
			 0x6b29cb10, /// 0x428
			 0x32945d4e, /// 0x42c
			 0xabb95ba0, /// 0x430
			 0x334b67d6, /// 0x434
			 0x8f8c02fe, /// 0x438
			 0x511ccb36, /// 0x43c
			 0xa0a0afa6, /// 0x440
			 0x11f7d30b, /// 0x444
			 0x49c637c9, /// 0x448
			 0xd23504e0, /// 0x44c
			 0xfb82221b, /// 0x450
			 0x530a2095, /// 0x454
			 0xaf5a674a, /// 0x458
			 0x34819230, /// 0x45c
			 0xea7bbcff, /// 0x460
			 0x3087b52d, /// 0x464
			 0xfd410179, /// 0x468
			 0x1f4009a8, /// 0x46c
			 0x27cacf1c, /// 0x470
			 0xcce40285, /// 0x474
			 0x8cc0d089, /// 0x478
			 0xf147ffef, /// 0x47c
			 0x7ec50dea, /// 0x480
			 0xa0d9614e, /// 0x484
			 0xf268463a, /// 0x488
			 0x999d9d94, /// 0x48c
			 0x83e4fb73, /// 0x490
			 0x83b15793, /// 0x494
			 0xc1e5517a, /// 0x498
			 0xef138322, /// 0x49c
			 0x8845c2b1, /// 0x4a0
			 0xb34d18b9, /// 0x4a4
			 0x4a8bf605, /// 0x4a8
			 0x9b7b8cc6, /// 0x4ac
			 0xdb793ee2, /// 0x4b0
			 0x57e50ae9, /// 0x4b4
			 0x0a30038b, /// 0x4b8
			 0x108bb523, /// 0x4bc
			 0x49729f72, /// 0x4c0
			 0x8cc75449, /// 0x4c4
			 0x73ffd6c6, /// 0x4c8
			 0x0ff68a8b, /// 0x4cc
			 0xdd6ceaf9, /// 0x4d0
			 0x961d8ca1, /// 0x4d4
			 0xd899e167, /// 0x4d8
			 0x05097832, /// 0x4dc
			 0x8364fb34, /// 0x4e0
			 0x75e70743, /// 0x4e4
			 0x201dc6da, /// 0x4e8
			 0x6d938ee2, /// 0x4ec
			 0x61d41775, /// 0x4f0
			 0x480250d2, /// 0x4f4
			 0x1c9ef59b, /// 0x4f8
			 0xe657ec3d, /// 0x4fc
			 0x3ea3b349, /// 0x500
			 0x7cb0204b, /// 0x504
			 0x5b5a8ae4, /// 0x508
			 0x27190067, /// 0x50c
			 0x3464a934, /// 0x510
			 0x1f8affb6, /// 0x514
			 0x2a749b15, /// 0x518
			 0xeb52e71f, /// 0x51c
			 0xbebb7ed1, /// 0x520
			 0x99475ceb, /// 0x524
			 0xe292fe1c, /// 0x528
			 0x9e4c9e14, /// 0x52c
			 0xb88a097a, /// 0x530
			 0x7e391222, /// 0x534
			 0x20e5171d, /// 0x538
			 0xdc79dc77, /// 0x53c
			 0x0017617b, /// 0x540
			 0x97af29af, /// 0x544
			 0x57cc6c2f, /// 0x548
			 0x8daff774, /// 0x54c
			 0xaa28dbdb, /// 0x550
			 0xe1150026, /// 0x554
			 0xff4ed5f2, /// 0x558
			 0xc09c0589, /// 0x55c
			 0x14e9ebaf, /// 0x560
			 0x71be8222, /// 0x564
			 0x7a8bec7b, /// 0x568
			 0xdb11e662, /// 0x56c
			 0xa8e12c32, /// 0x570
			 0xbec11883, /// 0x574
			 0x2ebb9f4c, /// 0x578
			 0x9207938a, /// 0x57c
			 0x562a570e, /// 0x580
			 0xf7f7eb3a, /// 0x584
			 0xd753af8b, /// 0x588
			 0x7f0321fa, /// 0x58c
			 0x18635859, /// 0x590
			 0x72de7ab4, /// 0x594
			 0x10902629, /// 0x598
			 0xf8f4aa45, /// 0x59c
			 0xcd7cbe55, /// 0x5a0
			 0x4af06ab5, /// 0x5a4
			 0xb7cb7e56, /// 0x5a8
			 0xdb5afa5c, /// 0x5ac
			 0xb88a6479, /// 0x5b0
			 0xe15af927, /// 0x5b4
			 0x57c6de1c, /// 0x5b8
			 0xa6119e26, /// 0x5bc
			 0x5778adeb, /// 0x5c0
			 0xc1c02186, /// 0x5c4
			 0xbe1b7f53, /// 0x5c8
			 0x31e5c50a, /// 0x5cc
			 0x907cd69d, /// 0x5d0
			 0xe17617af, /// 0x5d4
			 0xfa1e688c, /// 0x5d8
			 0x67645719, /// 0x5dc
			 0x38d32514, /// 0x5e0
			 0xbd080bad, /// 0x5e4
			 0x7dd51acc, /// 0x5e8
			 0x6f930ce4, /// 0x5ec
			 0xa46850fc, /// 0x5f0
			 0xa647c0f2, /// 0x5f4
			 0xbfd72fc3, /// 0x5f8
			 0xfe946046, /// 0x5fc
			 0xf4a500dc, /// 0x600
			 0x2be8fe8a, /// 0x604
			 0xa87e0e76, /// 0x608
			 0x3fa6bd02, /// 0x60c
			 0x1743a623, /// 0x610
			 0x84d58def, /// 0x614
			 0x798171e1, /// 0x618
			 0x33faad0a, /// 0x61c
			 0x376d4ecf, /// 0x620
			 0xbf653e64, /// 0x624
			 0xa285583d, /// 0x628
			 0x0086f504, /// 0x62c
			 0x785bcd34, /// 0x630
			 0xf07672ca, /// 0x634
			 0x6689eb11, /// 0x638
			 0xd233f65f, /// 0x63c
			 0x19e54f06, /// 0x640
			 0x2844589b, /// 0x644
			 0xa5372124, /// 0x648
			 0xd94c805e, /// 0x64c
			 0x038fc0d3, /// 0x650
			 0x872c972f, /// 0x654
			 0x873e2bf9, /// 0x658
			 0x64816dde, /// 0x65c
			 0x01ddac84, /// 0x660
			 0x762c6262, /// 0x664
			 0xabadbe9a, /// 0x668
			 0xddbeea44, /// 0x66c
			 0x42aa7ad5, /// 0x670
			 0x375fc4c4, /// 0x674
			 0x397f343d, /// 0x678
			 0xe39602d6, /// 0x67c
			 0x6dfd8064, /// 0x680
			 0xd3b9cf04, /// 0x684
			 0x8415c921, /// 0x688
			 0xd5f76938, /// 0x68c
			 0x59317225, /// 0x690
			 0x3986a575, /// 0x694
			 0x15759d2f, /// 0x698
			 0x067c28c8, /// 0x69c
			 0xd4d23283, /// 0x6a0
			 0x0e9f88c9, /// 0x6a4
			 0x5330dfd5, /// 0x6a8
			 0x5f059b51, /// 0x6ac
			 0xfde4e3c0, /// 0x6b0
			 0xdd964d9c, /// 0x6b4
			 0xd462f102, /// 0x6b8
			 0x6169ca99, /// 0x6bc
			 0xbdc3cb75, /// 0x6c0
			 0xd4388d17, /// 0x6c4
			 0x00acf673, /// 0x6c8
			 0xc6c6c6a1, /// 0x6cc
			 0x8f2b8b54, /// 0x6d0
			 0x4368f70e, /// 0x6d4
			 0x3c61c8b7, /// 0x6d8
			 0x4b03d05d, /// 0x6dc
			 0x4083ab92, /// 0x6e0
			 0x9c3b57d5, /// 0x6e4
			 0x61d7da51, /// 0x6e8
			 0xad214c34, /// 0x6ec
			 0xaec51867, /// 0x6f0
			 0x3d0efb28, /// 0x6f4
			 0xf096895f, /// 0x6f8
			 0xe96dd710, /// 0x6fc
			 0xa7b2eda4, /// 0x700
			 0x8f454c58, /// 0x704
			 0x31412984, /// 0x708
			 0xa074e6f5, /// 0x70c
			 0x048a8759, /// 0x710
			 0x67cc101f, /// 0x714
			 0xb04e2814, /// 0x718
			 0xd6b16845, /// 0x71c
			 0x91abdd92, /// 0x720
			 0xd5ac1f95, /// 0x724
			 0xf9eedaa7, /// 0x728
			 0xd13ba18f, /// 0x72c
			 0x4229fe3f, /// 0x730
			 0x28611a8d, /// 0x734
			 0x9b73e9c2, /// 0x738
			 0x95574d58, /// 0x73c
			 0xb5a9b86e, /// 0x740
			 0xd0a50b0c, /// 0x744
			 0xf670c2d2, /// 0x748
			 0x43ab8cec, /// 0x74c
			 0x31f56749, /// 0x750
			 0x0d1fc239, /// 0x754
			 0xeefbdde4, /// 0x758
			 0x5a52a7e8, /// 0x75c
			 0x320cb46c, /// 0x760
			 0x06a98798, /// 0x764
			 0x47a04585, /// 0x768
			 0x9c6fb025, /// 0x76c
			 0xcf2067a5, /// 0x770
			 0x1a3be4b6, /// 0x774
			 0xf2e5cd9c, /// 0x778
			 0x781618ae, /// 0x77c
			 0x4dc6c675, /// 0x780
			 0x20918f13, /// 0x784
			 0x48e503be, /// 0x788
			 0xb76d63d2, /// 0x78c
			 0xd5c542ad, /// 0x790
			 0x07a37bf1, /// 0x794
			 0xfe63ca29, /// 0x798
			 0xd452f767, /// 0x79c
			 0x8659328a, /// 0x7a0
			 0x929c9d4b, /// 0x7a4
			 0x1abd0b95, /// 0x7a8
			 0xc7f0d9f1, /// 0x7ac
			 0x7893a180, /// 0x7b0
			 0xdd945c31, /// 0x7b4
			 0x50ada7db, /// 0x7b8
			 0x79b6290e, /// 0x7bc
			 0xf953d149, /// 0x7c0
			 0x1d66b130, /// 0x7c4
			 0xbb19a88b, /// 0x7c8
			 0x295374e7, /// 0x7cc
			 0xf0078d60, /// 0x7d0
			 0x7991297c, /// 0x7d4
			 0xd605823f, /// 0x7d8
			 0x20beaadb, /// 0x7dc
			 0x3bcd6e5d, /// 0x7e0
			 0x224bf7c8, /// 0x7e4
			 0x22df4de0, /// 0x7e8
			 0x8948ef49, /// 0x7ec
			 0x83155679, /// 0x7f0
			 0x280de452, /// 0x7f4
			 0xfd2d2c1f, /// 0x7f8
			 0xa38d06db, /// 0x7fc
			 0x60317350, /// 0x800
			 0xb0eee565, /// 0x804
			 0x5555c01e, /// 0x808
			 0x4cf2bfe9, /// 0x80c
			 0x6f4e9f7a, /// 0x810
			 0x2fc0cf22, /// 0x814
			 0x323eba73, /// 0x818
			 0x1d2e196d, /// 0x81c
			 0x0170336b, /// 0x820
			 0x970fd243, /// 0x824
			 0xa1649558, /// 0x828
			 0xdc767183, /// 0x82c
			 0x1562ea63, /// 0x830
			 0x296505fd, /// 0x834
			 0xfca5fa98, /// 0x838
			 0xcbc07728, /// 0x83c
			 0x38c6c3fc, /// 0x840
			 0xcd6450ad, /// 0x844
			 0x32201a9c, /// 0x848
			 0x7ba2f73c, /// 0x84c
			 0x4b558d0e, /// 0x850
			 0x1b10cdb8, /// 0x854
			 0xb39910e9, /// 0x858
			 0xee70ab24, /// 0x85c
			 0xbdcfced8, /// 0x860
			 0x333d970d, /// 0x864
			 0x2415064c, /// 0x868
			 0x62aef6d6, /// 0x86c
			 0x07f1cc54, /// 0x870
			 0x60f05179, /// 0x874
			 0x5fe51df4, /// 0x878
			 0xe4300a61, /// 0x87c
			 0x88b94170, /// 0x880
			 0xcd774dc7, /// 0x884
			 0x37b2c22c, /// 0x888
			 0x7544a22e, /// 0x88c
			 0x293675a8, /// 0x890
			 0xe3d2174b, /// 0x894
			 0x281cb2db, /// 0x898
			 0x5c1d548a, /// 0x89c
			 0x10baf502, /// 0x8a0
			 0x3dcba3aa, /// 0x8a4
			 0x7248397d, /// 0x8a8
			 0xbbe56539, /// 0x8ac
			 0xe6beb694, /// 0x8b0
			 0xd9ff8a59, /// 0x8b4
			 0xb6bf3895, /// 0x8b8
			 0x6faccccb, /// 0x8bc
			 0xe8192120, /// 0x8c0
			 0xaae2596c, /// 0x8c4
			 0xff29c3c1, /// 0x8c8
			 0xba3c0e48, /// 0x8cc
			 0xa8b5f245, /// 0x8d0
			 0x30948d61, /// 0x8d4
			 0xc91a16b1, /// 0x8d8
			 0xa6c32304, /// 0x8dc
			 0x1620d071, /// 0x8e0
			 0x55460ec9, /// 0x8e4
			 0x991ca0a7, /// 0x8e8
			 0x0bcfaa94, /// 0x8ec
			 0xca364312, /// 0x8f0
			 0x8fe78076, /// 0x8f4
			 0xe2bdf6ae, /// 0x8f8
			 0xd5a08795, /// 0x8fc
			 0x15b153e5, /// 0x900
			 0x1dfb287c, /// 0x904
			 0xe03568f1, /// 0x908
			 0xc813eb4c, /// 0x90c
			 0xdf037b36, /// 0x910
			 0x31a39924, /// 0x914
			 0xd53ee40a, /// 0x918
			 0x40228eb0, /// 0x91c
			 0x5961708e, /// 0x920
			 0xf6e42986, /// 0x924
			 0xd7cc1e27, /// 0x928
			 0xca9289f1, /// 0x92c
			 0x54ac2bc0, /// 0x930
			 0xe5cc9249, /// 0x934
			 0xcc187f05, /// 0x938
			 0xdc02d5d8, /// 0x93c
			 0x8a551374, /// 0x940
			 0x3ba3f324, /// 0x944
			 0xb01fc5b9, /// 0x948
			 0x405856ff, /// 0x94c
			 0xc4a69db3, /// 0x950
			 0x48d75895, /// 0x954
			 0x6362c9e3, /// 0x958
			 0xd49a411a, /// 0x95c
			 0xcf0d17fa, /// 0x960
			 0x1bda7a70, /// 0x964
			 0x34dd045b, /// 0x968
			 0x83f5382d, /// 0x96c
			 0x1219c1b2, /// 0x970
			 0xe04255f4, /// 0x974
			 0x2548ab9c, /// 0x978
			 0x819c5a83, /// 0x97c
			 0x1cb0c39a, /// 0x980
			 0xf31ac14b, /// 0x984
			 0xe3d4d2b2, /// 0x988
			 0xdc7e81e9, /// 0x98c
			 0xcaaddee8, /// 0x990
			 0x3e19829c, /// 0x994
			 0x56e8b04d, /// 0x998
			 0x246a1f36, /// 0x99c
			 0x17bc7ef5, /// 0x9a0
			 0x6a44355c, /// 0x9a4
			 0x066b119d, /// 0x9a8
			 0xe652dc93, /// 0x9ac
			 0xd2106b59, /// 0x9b0
			 0xf6f2a003, /// 0x9b4
			 0xbbbf4ba4, /// 0x9b8
			 0x38c9cf05, /// 0x9bc
			 0xb8396396, /// 0x9c0
			 0xdc42fe1b, /// 0x9c4
			 0x38a48bdb, /// 0x9c8
			 0x59dc38d2, /// 0x9cc
			 0x9237c75f, /// 0x9d0
			 0x071aa595, /// 0x9d4
			 0x3827b12c, /// 0x9d8
			 0x047f74ef, /// 0x9dc
			 0x01c4088a, /// 0x9e0
			 0x4f6ed82d, /// 0x9e4
			 0x7c687d55, /// 0x9e8
			 0xf20dce55, /// 0x9ec
			 0x8a7b6b03, /// 0x9f0
			 0xecfb83f6, /// 0x9f4
			 0x47128e72, /// 0x9f8
			 0x46da16df, /// 0x9fc
			 0x42e40c8d, /// 0xa00
			 0x8760dd83, /// 0xa04
			 0x111f50d7, /// 0xa08
			 0xe9eeaa60, /// 0xa0c
			 0xeed47a6f, /// 0xa10
			 0xd1b98e16, /// 0xa14
			 0x0b382fa7, /// 0xa18
			 0xd4d169ed, /// 0xa1c
			 0x02c3740c, /// 0xa20
			 0xfb66b746, /// 0xa24
			 0xe729884e, /// 0xa28
			 0xf8b0da7b, /// 0xa2c
			 0x6692a229, /// 0xa30
			 0x0702f330, /// 0xa34
			 0xb1e25b9a, /// 0xa38
			 0x8f6575b9, /// 0xa3c
			 0x41557c06, /// 0xa40
			 0x8858574c, /// 0xa44
			 0xa702663d, /// 0xa48
			 0xd01b2c9c, /// 0xa4c
			 0xe8be9a49, /// 0xa50
			 0x1aeb7a65, /// 0xa54
			 0x8103c15d, /// 0xa58
			 0x319aa2fe, /// 0xa5c
			 0xc00e020e, /// 0xa60
			 0x417766ba, /// 0xa64
			 0x8d9755d2, /// 0xa68
			 0xd9bd38fa, /// 0xa6c
			 0x9a12096e, /// 0xa70
			 0x76ddd560, /// 0xa74
			 0x50a363fa, /// 0xa78
			 0xca19e4ba, /// 0xa7c
			 0x16f89028, /// 0xa80
			 0x27e3ce78, /// 0xa84
			 0x8b216de1, /// 0xa88
			 0x39163da7, /// 0xa8c
			 0x90feba8b, /// 0xa90
			 0x36a8b95e, /// 0xa94
			 0x97d2b5b7, /// 0xa98
			 0x41c23107, /// 0xa9c
			 0x087cce9a, /// 0xaa0
			 0x33b64035, /// 0xaa4
			 0x73652b19, /// 0xaa8
			 0xc72abf75, /// 0xaac
			 0xb5d8cea9, /// 0xab0
			 0xb66ad0a1, /// 0xab4
			 0x1db5a7f7, /// 0xab8
			 0x10e5117f, /// 0xabc
			 0x78cdc2fd, /// 0xac0
			 0x8a8e0c65, /// 0xac4
			 0x3585cf59, /// 0xac8
			 0x1084266f, /// 0xacc
			 0x9b021de8, /// 0xad0
			 0xf2bdbd53, /// 0xad4
			 0xa1c8ceb6, /// 0xad8
			 0xf50a3e82, /// 0xadc
			 0xce9572f3, /// 0xae0
			 0x9a76fb9f, /// 0xae4
			 0x466d87b9, /// 0xae8
			 0x1aaa1f30, /// 0xaec
			 0x9ab78170, /// 0xaf0
			 0xd1977376, /// 0xaf4
			 0x82063a09, /// 0xaf8
			 0x80f501fd, /// 0xafc
			 0xe3b98202, /// 0xb00
			 0xb68dbba2, /// 0xb04
			 0x7b854624, /// 0xb08
			 0x897880a2, /// 0xb0c
			 0xbca56836, /// 0xb10
			 0xc4c62de2, /// 0xb14
			 0x61de17e5, /// 0xb18
			 0xad994f40, /// 0xb1c
			 0x5753b287, /// 0xb20
			 0xe293526a, /// 0xb24
			 0x5db8ee88, /// 0xb28
			 0xa5ce6975, /// 0xb2c
			 0xcc5e670b, /// 0xb30
			 0x611ddeb6, /// 0xb34
			 0xd3f295b9, /// 0xb38
			 0x3acf4993, /// 0xb3c
			 0x35e74485, /// 0xb40
			 0xbff26809, /// 0xb44
			 0xf3b926f9, /// 0xb48
			 0xe8324e67, /// 0xb4c
			 0x5dfb289e, /// 0xb50
			 0x7d369d99, /// 0xb54
			 0x0c69e89c, /// 0xb58
			 0x25f5079c, /// 0xb5c
			 0x0d75ff9d, /// 0xb60
			 0x652caefd, /// 0xb64
			 0x854853f0, /// 0xb68
			 0x724ceda3, /// 0xb6c
			 0x6b9a9043, /// 0xb70
			 0x616de623, /// 0xb74
			 0x5ff2a12f, /// 0xb78
			 0x97ba4bb2, /// 0xb7c
			 0xe47398d8, /// 0xb80
			 0xfd242c4b, /// 0xb84
			 0xedfc353a, /// 0xb88
			 0x9ef6e0fc, /// 0xb8c
			 0x982abb07, /// 0xb90
			 0x9074cd73, /// 0xb94
			 0xe6617411, /// 0xb98
			 0x5510d1b4, /// 0xb9c
			 0xcd28f510, /// 0xba0
			 0xf14139c8, /// 0xba4
			 0x5651588e, /// 0xba8
			 0xc5847a95, /// 0xbac
			 0x9a64b149, /// 0xbb0
			 0x76c85f6e, /// 0xbb4
			 0x49877852, /// 0xbb8
			 0x1ffa998e, /// 0xbbc
			 0x0ba1d08c, /// 0xbc0
			 0x756d7be1, /// 0xbc4
			 0x5b71adf2, /// 0xbc8
			 0x05ece725, /// 0xbcc
			 0xbaa6eb4e, /// 0xbd0
			 0x418058c6, /// 0xbd4
			 0x71eaca84, /// 0xbd8
			 0x16e6c3b0, /// 0xbdc
			 0xab64da9e, /// 0xbe0
			 0x4c976e47, /// 0xbe4
			 0xfb3049fe, /// 0xbe8
			 0x5676007d, /// 0xbec
			 0x3155eb3e, /// 0xbf0
			 0x8a4e550a, /// 0xbf4
			 0xcebe3896, /// 0xbf8
			 0x054afbef, /// 0xbfc
			 0x6d334d06, /// 0xc00
			 0xf06db4dd, /// 0xc04
			 0x63b6dbd4, /// 0xc08
			 0x0d987f85, /// 0xc0c
			 0x1fc797a7, /// 0xc10
			 0xfe9b8af5, /// 0xc14
			 0x0ee495d5, /// 0xc18
			 0x2f570e5b, /// 0xc1c
			 0x1b14a666, /// 0xc20
			 0xdff59d7f, /// 0xc24
			 0xe7551c9a, /// 0xc28
			 0x4ba0318f, /// 0xc2c
			 0xcea4a7b0, /// 0xc30
			 0x63c60d92, /// 0xc34
			 0xd75eada4, /// 0xc38
			 0x5b288cfa, /// 0xc3c
			 0xc07b395c, /// 0xc40
			 0xc0916ae3, /// 0xc44
			 0x5df543e6, /// 0xc48
			 0x14bb13cc, /// 0xc4c
			 0xc4b17ece, /// 0xc50
			 0xde1d45d2, /// 0xc54
			 0xc862ba56, /// 0xc58
			 0xf6d18da8, /// 0xc5c
			 0xf689d11b, /// 0xc60
			 0xbd965b12, /// 0xc64
			 0x68ba383d, /// 0xc68
			 0xfa8ccc56, /// 0xc6c
			 0x678c9a53, /// 0xc70
			 0x27a51aff, /// 0xc74
			 0xb8cce978, /// 0xc78
			 0x3df31a14, /// 0xc7c
			 0xaab981f1, /// 0xc80
			 0x5e7bdc47, /// 0xc84
			 0xd96bcbf8, /// 0xc88
			 0x3b76a21e, /// 0xc8c
			 0xb2912e5d, /// 0xc90
			 0x8085f1ac, /// 0xc94
			 0xa80d8328, /// 0xc98
			 0xa183c81d, /// 0xc9c
			 0xbbf962b3, /// 0xca0
			 0xd955d0b0, /// 0xca4
			 0xb98f5e7e, /// 0xca8
			 0xd2c5429a, /// 0xcac
			 0x21a89fa1, /// 0xcb0
			 0x028b973b, /// 0xcb4
			 0x5366ce23, /// 0xcb8
			 0xae9b3931, /// 0xcbc
			 0xfd1b1089, /// 0xcc0
			 0xfc8c243a, /// 0xcc4
			 0x631010a0, /// 0xcc8
			 0x4d585325, /// 0xccc
			 0x4c997d28, /// 0xcd0
			 0xc150e7e2, /// 0xcd4
			 0x70e59db7, /// 0xcd8
			 0x45116e72, /// 0xcdc
			 0x246c9356, /// 0xce0
			 0x0a850706, /// 0xce4
			 0xbdfe1764, /// 0xce8
			 0xf0e78630, /// 0xcec
			 0xa2f13ea4, /// 0xcf0
			 0x0e983ff3, /// 0xcf4
			 0x09a1c458, /// 0xcf8
			 0x859ad841, /// 0xcfc
			 0x5a7de373, /// 0xd00
			 0x7ceb1f0d, /// 0xd04
			 0x9b293024, /// 0xd08
			 0x532baec4, /// 0xd0c
			 0xf75387fc, /// 0xd10
			 0xcdce68ec, /// 0xd14
			 0x08621828, /// 0xd18
			 0xf0367698, /// 0xd1c
			 0xc849fc96, /// 0xd20
			 0x07f6dd5b, /// 0xd24
			 0xe3532be7, /// 0xd28
			 0xc9451c2a, /// 0xd2c
			 0x57dcfb8d, /// 0xd30
			 0x01e383ff, /// 0xd34
			 0xea7a0998, /// 0xd38
			 0x98641034, /// 0xd3c
			 0x1622797d, /// 0xd40
			 0xc2a8b47f, /// 0xd44
			 0x3e95072b, /// 0xd48
			 0xbafb3016, /// 0xd4c
			 0xbeb13397, /// 0xd50
			 0x464859e8, /// 0xd54
			 0x483f53e2, /// 0xd58
			 0x0b30cc6c, /// 0xd5c
			 0x1e9a57cd, /// 0xd60
			 0x8d51fc60, /// 0xd64
			 0xb2b96eaa, /// 0xd68
			 0xff497a44, /// 0xd6c
			 0xb7733075, /// 0xd70
			 0xf77e9450, /// 0xd74
			 0xb88d2c48, /// 0xd78
			 0x508b82ea, /// 0xd7c
			 0xccf015d5, /// 0xd80
			 0x23543d82, /// 0xd84
			 0x3386dcd5, /// 0xd88
			 0x04882ddd, /// 0xd8c
			 0x48b86a97, /// 0xd90
			 0x40193fcb, /// 0xd94
			 0xa5e71ef4, /// 0xd98
			 0xf32aa031, /// 0xd9c
			 0x514082f3, /// 0xda0
			 0xff6edf77, /// 0xda4
			 0xad813c7a, /// 0xda8
			 0xa1be41d2, /// 0xdac
			 0x794b2d16, /// 0xdb0
			 0x1d573037, /// 0xdb4
			 0x3f72daab, /// 0xdb8
			 0x00491508, /// 0xdbc
			 0x6cfabbd0, /// 0xdc0
			 0xf4f9422b, /// 0xdc4
			 0xd4863804, /// 0xdc8
			 0xf74a736f, /// 0xdcc
			 0xc93e1d5f, /// 0xdd0
			 0x11416d16, /// 0xdd4
			 0x28af6720, /// 0xdd8
			 0xac0c823c, /// 0xddc
			 0xcb0ad9a2, /// 0xde0
			 0xaa4eee9d, /// 0xde4
			 0x792a50a3, /// 0xde8
			 0x0e904030, /// 0xdec
			 0xa7d69991, /// 0xdf0
			 0xf7323c73, /// 0xdf4
			 0x6cea36d5, /// 0xdf8
			 0xb69c6b6c, /// 0xdfc
			 0xeca7f5d9, /// 0xe00
			 0xb6840ce8, /// 0xe04
			 0xb0569bbf, /// 0xe08
			 0x71531cfe, /// 0xe0c
			 0xbbdbd762, /// 0xe10
			 0x5de2c40a, /// 0xe14
			 0x2e328234, /// 0xe18
			 0xfc1bda09, /// 0xe1c
			 0x6bf9e506, /// 0xe20
			 0x3bef4ec5, /// 0xe24
			 0x5cb40ae5, /// 0xe28
			 0xa23be76f, /// 0xe2c
			 0x007ad391, /// 0xe30
			 0x08667e35, /// 0xe34
			 0xcffb5bb3, /// 0xe38
			 0x67c11798, /// 0xe3c
			 0xc862ea38, /// 0xe40
			 0xe711065c, /// 0xe44
			 0xff7ba5aa, /// 0xe48
			 0x3f056a82, /// 0xe4c
			 0xc2f054f0, /// 0xe50
			 0xa53457a5, /// 0xe54
			 0x04958048, /// 0xe58
			 0xd1f22604, /// 0xe5c
			 0xf37d554c, /// 0xe60
			 0x14833800, /// 0xe64
			 0x50eeb532, /// 0xe68
			 0xe3e179ba, /// 0xe6c
			 0x77338958, /// 0xe70
			 0xb7a39957, /// 0xe74
			 0x3e592ed3, /// 0xe78
			 0x3124e3d2, /// 0xe7c
			 0x8dbd71de, /// 0xe80
			 0xb0ccd572, /// 0xe84
			 0xcb58f80e, /// 0xe88
			 0x615b3ccd, /// 0xe8c
			 0x1798bbca, /// 0xe90
			 0xc55a0c88, /// 0xe94
			 0x7347aa4b, /// 0xe98
			 0x0217577e, /// 0xe9c
			 0xf75575e2, /// 0xea0
			 0x3629555d, /// 0xea4
			 0x002039d6, /// 0xea8
			 0xebf567aa, /// 0xeac
			 0xf4a0571d, /// 0xeb0
			 0xee5b654a, /// 0xeb4
			 0xc63bddb6, /// 0xeb8
			 0x00d5e6a1, /// 0xebc
			 0x7ddda147, /// 0xec0
			 0x2b0b610d, /// 0xec4
			 0x21b0c9d8, /// 0xec8
			 0x6fc017ee, /// 0xecc
			 0x646befaa, /// 0xed0
			 0x9dc44638, /// 0xed4
			 0xab35e949, /// 0xed8
			 0xbf55a80f, /// 0xedc
			 0x4a2ec41f, /// 0xee0
			 0xaf1246f9, /// 0xee4
			 0x16a56054, /// 0xee8
			 0x1930c6f8, /// 0xeec
			 0x592d99fd, /// 0xef0
			 0x590e3dbf, /// 0xef4
			 0xd3c14e4f, /// 0xef8
			 0x9cde5628, /// 0xefc
			 0x1dcd926c, /// 0xf00
			 0x1815927c, /// 0xf04
			 0x215ec5d0, /// 0xf08
			 0xc42d6f17, /// 0xf0c
			 0xe66dbd0d, /// 0xf10
			 0x5abc58d3, /// 0xf14
			 0x045f7b80, /// 0xf18
			 0xe9dc7bf5, /// 0xf1c
			 0x097e86cd, /// 0xf20
			 0x9420f233, /// 0xf24
			 0xd3967f59, /// 0xf28
			 0x88bf5328, /// 0xf2c
			 0xaba8ef0c, /// 0xf30
			 0x5a9cfa74, /// 0xf34
			 0x7360ea51, /// 0xf38
			 0xb748e651, /// 0xf3c
			 0x6f27d6d3, /// 0xf40
			 0xee1f8ff8, /// 0xf44
			 0x37312eab, /// 0xf48
			 0xd13a6635, /// 0xf4c
			 0xf3462445, /// 0xf50
			 0xd2ce94ff, /// 0xf54
			 0xa2531a3d, /// 0xf58
			 0xdbc31b94, /// 0xf5c
			 0x080d42c7, /// 0xf60
			 0xb898b3d1, /// 0xf64
			 0xc1977af8, /// 0xf68
			 0x2284ce87, /// 0xf6c
			 0x1271ea85, /// 0xf70
			 0x327d3ce0, /// 0xf74
			 0xe70fb0cf, /// 0xf78
			 0x492eb30b, /// 0xf7c
			 0x1bbdfa9d, /// 0xf80
			 0x730b575b, /// 0xf84
			 0x2dd03786, /// 0xf88
			 0x69447958, /// 0xf8c
			 0x0a252c98, /// 0xf90
			 0x6c167b13, /// 0xf94
			 0x957f72d6, /// 0xf98
			 0x832e9ffe, /// 0xf9c
			 0xe1cf8cda, /// 0xfa0
			 0xb8fb301f, /// 0xfa4
			 0xf40b7eed, /// 0xfa8
			 0xfc558782, /// 0xfac
			 0xc834fa69, /// 0xfb0
			 0x500427da, /// 0xfb4
			 0xa4798b1b, /// 0xfb8
			 0x271a5ce7, /// 0xfbc
			 0xee164f9d, /// 0xfc0
			 0x8f973aee, /// 0xfc4
			 0x1c1739b9, /// 0xfc8
			 0x8372909d, /// 0xfcc
			 0x1418f89d, /// 0xfd0
			 0x637834bf, /// 0xfd4
			 0x3973de85, /// 0xfd8
			 0x4b2ba424, /// 0xfdc
			 0xae88d4c4, /// 0xfe0
			 0x84bf4054, /// 0xfe4
			 0x2d12f346, /// 0xfe8
			 0xa416a0ac, /// 0xfec
			 0xf4786830, /// 0xff0
			 0x05ba0433, /// 0xff4
			 0x32db0c43, /// 0xff8
			 0x552f0c06 /// last
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
			 0x000006e4,
			 0x00000604,
			 0x000000dc,
			 0x000007a8,
			 0x00000624,
			 0x00000574,
			 0x0000073c,
			 0x00000244,

			 /// vpu register f2
			 0x41f00000,
			 0x41d80000,
			 0x41a00000,
			 0x40800000,
			 0x40000000,
			 0x3f800000,
			 0x41100000,
			 0x41c80000,

			 /// vpu register f3
			 0x41100000,
			 0x40e00000,
			 0x42000000,
			 0x40400000,
			 0x41d80000,
			 0x41e00000,
			 0x40c00000,
			 0x41980000,

			 /// vpu register f4
			 0x41300000,
			 0x41f00000,
			 0x41200000,
			 0x41300000,
			 0x40c00000,
			 0x41600000,
			 0x40a00000,
			 0x41c80000,

			 /// vpu register f5
			 0x41900000,
			 0x41e00000,
			 0x41c80000,
			 0x41700000,
			 0x42000000,
			 0x41e00000,
			 0x41b80000,
			 0x41c00000,

			 /// vpu register f6
			 0x41400000,
			 0x41300000,
			 0x41400000,
			 0x41c80000,
			 0x41400000,
			 0x41980000,
			 0x41a00000,
			 0x41200000,

			 /// vpu register f7
			 0x41700000,
			 0x40000000,
			 0x41d00000,
			 0x40e00000,
			 0x41000000,
			 0x41a80000,
			 0x41e00000,
			 0x41000000,

			 /// vpu register f8
			 0x41500000,
			 0x41b80000,
			 0x40800000,
			 0x40800000,
			 0x41b00000,
			 0x41f80000,
			 0x41c80000,
			 0x41e80000,

			 /// vpu register f9
			 0x40800000,
			 0x42000000,
			 0x42000000,
			 0x41800000,
			 0x41c00000,
			 0x41e80000,
			 0x40e00000,
			 0x41f00000,

			 /// vpu register f10
			 0x41000000,
			 0x41d00000,
			 0x41f00000,
			 0x42000000,
			 0x40e00000,
			 0x41980000,
			 0x41000000,
			 0x41900000,

			 /// vpu register f11
			 0x41e80000,
			 0x41e00000,
			 0x40800000,
			 0x41a80000,
			 0x41300000,
			 0x41600000,
			 0x42000000,
			 0x41e00000,

			 /// vpu register f12
			 0x41b00000,
			 0x40e00000,
			 0x40000000,
			 0x41200000,
			 0x40800000,
			 0x41500000,
			 0x40400000,
			 0x40c00000,

			 /// vpu register f13
			 0x41c00000,
			 0x41000000,
			 0x41f00000,
			 0x41f80000,
			 0x41f80000,
			 0x41400000,
			 0x41000000,
			 0x40c00000,

			 /// vpu register f14
			 0x41200000,
			 0x41700000,
			 0x40000000,
			 0x41f00000,
			 0x41100000,
			 0x41f00000,
			 0x41e80000,
			 0x41980000,

			 /// vpu register f15
			 0x41800000,
			 0x40800000,
			 0x41800000,
			 0x41b80000,
			 0x41f80000,
			 0x41a80000,
			 0x41100000,
			 0x41e80000,

			 /// vpu register f16
			 0x40c00000,
			 0x41300000,
			 0x41600000,
			 0x41900000,
			 0x41e80000,
			 0x40800000,
			 0x42000000,
			 0x41500000,

			 /// vpu register f17
			 0x41700000,
			 0x41200000,
			 0x41d80000,
			 0x40800000,
			 0x41f80000,
			 0x41100000,
			 0x40800000,
			 0x41f80000,

			 /// vpu register f18
			 0x41b00000,
			 0x40c00000,
			 0x40800000,
			 0x41700000,
			 0x41100000,
			 0x40800000,
			 0x41e00000,
			 0x41c80000,

			 /// vpu register f19
			 0x41980000,
			 0x41980000,
			 0x41d00000,
			 0x40e00000,
			 0x41f80000,
			 0x41500000,
			 0x41200000,
			 0x41f00000,

			 /// vpu register f20
			 0x41300000,
			 0x41d80000,
			 0x41a80000,
			 0x41880000,
			 0x41a80000,
			 0x41d00000,
			 0x41a80000,
			 0x40000000,

			 /// vpu register f21
			 0x41d00000,
			 0x41400000,
			 0x41d80000,
			 0x41b80000,
			 0x41c80000,
			 0x41300000,
			 0x41000000,
			 0x41a80000,

			 /// vpu register f22
			 0x41b00000,
			 0x41900000,
			 0x41d80000,
			 0x41f00000,
			 0x41d00000,
			 0x40000000,
			 0x41800000,
			 0x40800000,

			 /// vpu register f23
			 0x41200000,
			 0x41b00000,
			 0x41d00000,
			 0x40e00000,
			 0x40c00000,
			 0x41700000,
			 0x41b00000,
			 0x41980000,

			 /// vpu register f24
			 0x41b80000,
			 0x41800000,
			 0x41000000,
			 0x41300000,
			 0x40a00000,
			 0x40a00000,
			 0x41a80000,
			 0x40800000,

			 /// vpu register f25
			 0x40c00000,
			 0x41b00000,
			 0x41600000,
			 0x41500000,
			 0x41e80000,
			 0x41d00000,
			 0x41a00000,
			 0x41e00000,

			 /// vpu register f26
			 0x41900000,
			 0x41f00000,
			 0x41900000,
			 0x40c00000,
			 0x40e00000,
			 0x41e80000,
			 0x40400000,
			 0x40400000,

			 /// vpu register f27
			 0x41500000,
			 0x41f00000,
			 0x41f80000,
			 0x41c00000,
			 0x42000000,
			 0x41a00000,
			 0x41b80000,
			 0x41200000,

			 /// vpu register f28
			 0x41d00000,
			 0x41e80000,
			 0x41e00000,
			 0x40a00000,
			 0x40c00000,
			 0x41200000,
			 0x40e00000,
			 0x41200000,

			 /// vpu register f29
			 0x42000000,
			 0x40a00000,
			 0x41c00000,
			 0x41b80000,
			 0x41f80000,
			 0x41b80000,
			 0x3f800000,
			 0x3f800000,

			 /// vpu register f30
			 0x41800000,
			 0x41e00000,
			 0x41400000,
			 0x41980000,
			 0x41300000,
			 0x41200000,
			 0x41b80000,
			 0x41e00000,

			 /// vpu register f31
			 0x41880000,
			 0x41880000,
			 0x41a80000,
			 0x41a80000,
			 0x41e80000,
			 0x41c00000,
			 0x40c00000,
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
		"famoxorl.pi f29, f1 (x11)\n"                         ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f23, f1 (x12)\n"                         ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f17, f1 (x14)\n"                         ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f15, f1 (x11)\n"                         ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f4, f1 (x13)\n"                          ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f6, f1 (x11)\n"                          ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f20, f1 (x15)\n"                         ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f18, f1 (x13)\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f8, f1 (x14)\n"                          ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f28, f1 (x14)\n"                         ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f8, f1 (x15)\n"                          ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f13, f1 (x15)\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f20, f1 (x11)\n"                         ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f18, f1 (x14)\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f24, f1 (x14)\n"                         ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f23, f1 (x13)\n"                         ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f12, f1 (x13)\n"                         ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f26, f1 (x14)\n"                         ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f9, f1 (x11)\n"                          ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f8, f1 (x13)\n"                          ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f20, f1 (x11)\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f28, f1 (x15)\n"                         ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f18, f1 (x15)\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f2, f1 (x15)\n"                          ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f30, f1 (x11)\n"                         ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f10, f1 (x15)\n"                         ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f17, f1 (x14)\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f5, f1 (x11)\n"                          ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f27, f1 (x14)\n"                         ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f5, f1 (x12)\n"                          ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f13, f1 (x12)\n"                         ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f15, f1 (x11)\n"                         ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f26, f1 (x14)\n"                         ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f12, f1 (x15)\n"                         ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f15, f1 (x15)\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f28, f1 (x12)\n"                         ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f13, f1 (x15)\n"                         ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f8, f1 (x11)\n"                          ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f18, f1 (x15)\n"                         ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f24, f1 (x11)\n"                         ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f5, f1 (x11)\n"                          ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f3, f1 (x11)\n"                          ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f8, f1 (x11)\n"                          ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f23, f1 (x12)\n"                         ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f10, f1 (x15)\n"                         ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f18, f1 (x13)\n"                         ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f13, f1 (x14)\n"                         ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f8, f1 (x12)\n"                          ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f20, f1 (x12)\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f22, f1 (x13)\n"                         ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f16, f1 (x12)\n"                         ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f5, f1 (x15)\n"                          ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f13, f1 (x13)\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f24, f1 (x13)\n"                         ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f6, f1 (x15)\n"                          ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f5, f1 (x12)\n"                          ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f26, f1 (x11)\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f6, f1 (x12)\n"                          ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f18, f1 (x14)\n"                         ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f3, f1 (x12)\n"                          ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f4, f1 (x14)\n"                          ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f24, f1 (x13)\n"                         ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f25, f1 (x11)\n"                         ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f25, f1 (x15)\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f2, f1 (x14)\n"                          ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f11, f1 (x12)\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f16, f1 (x13)\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f12, f1 (x11)\n"                         ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f7, f1 (x12)\n"                          ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f15, f1 (x14)\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f29, f1 (x15)\n"                         ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f14, f1 (x14)\n"                         ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f30, f1 (x12)\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f2, f1 (x15)\n"                          ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f9, f1 (x13)\n"                          ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f12, f1 (x11)\n"                         ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f27, f1 (x11)\n"                         ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f22, f1 (x13)\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f25, f1 (x12)\n"                         ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f2, f1 (x11)\n"                          ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f6, f1 (x11)\n"                          ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f3, f1 (x15)\n"                          ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f12, f1 (x13)\n"                         ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f12, f1 (x12)\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f23, f1 (x15)\n"                         ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f19, f1 (x15)\n"                         ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f8, f1 (x15)\n"                          ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f11, f1 (x13)\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f23, f1 (x11)\n"                         ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f20, f1 (x15)\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f27, f1 (x12)\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f2, f1 (x11)\n"                          ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f29, f1 (x12)\n"                         ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f25, f1 (x15)\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f18, f1 (x14)\n"                         ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f13, f1 (x11)\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f6, f1 (x11)\n"                          ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f14, f1 (x14)\n"                         ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famoxorl.pi f28, f1 (x13)\n"                         ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
