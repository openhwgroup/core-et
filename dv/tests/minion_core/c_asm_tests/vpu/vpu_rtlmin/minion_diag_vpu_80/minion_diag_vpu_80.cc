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
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00000
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00004
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00008
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0000c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00010
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00014
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00018
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0001c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00020
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x4b000000,                                                  // 8388608.0                                   /// 0002c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00030
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00040
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00044
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00048
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0004c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00058
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0005c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00060
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00064
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00068
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0006c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00074
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00078
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0007c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00080
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00084
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0008c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00090
			 0x0e000003,                                                  // Trailing 1s:                                /// 00094
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00098
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0009c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000d8
			 0x7fc00001,                                                  // signaling NaN                               /// 000dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00100
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00104
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00108
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0010c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00114
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0011c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00120
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00128
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0012c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00130
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00134
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00138
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0013c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00140
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x55555555,                                                  // 4 random values                             /// 00148
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0014c
			 0xff800000,                                                  // -inf                                        /// 00150
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00154
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00158
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0015c
			 0x4b000000,                                                  // 8388608.0                                   /// 00160
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00164
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00170
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00174
			 0x55555555,                                                  // 4 random values                             /// 00178
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0017c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00184
			 0xbf028f5c,                                                  // -0.51                                       /// 00188
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0018c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00190
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00194
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00198
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0019c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001a4
			 0x55555555,                                                  // 4 random values                             /// 001a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 001ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 001b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00200
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00204
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00208
			 0x4b000000,                                                  // 8388608.0                                   /// 0020c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00218
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0021c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00228
			 0x7f800000,                                                  // inf                                         /// 0022c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00234
			 0x4b000000,                                                  // 8388608.0                                   /// 00238
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0023c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00240
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00244
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00250
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00254
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00260
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00264
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00268
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0026c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00270
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00274
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0027c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00284
			 0x00011111,                                                  // 9.7958E-41                                  /// 00288
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0028c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00290
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00294
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0029c
			 0x0e000001,                                                  // Trailing 1s:                                /// 002a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002d0
			 0x7fc00001,                                                  // signaling NaN                               /// 002d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 002e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002fc
			 0x33333333,                                                  // 4 random values                             /// 00300
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00304
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00308
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0030c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00314
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00318
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0xff800000,                                                  // -inf                                        /// 00320
			 0x0c700000,                                                  // Leading 1s:                                 /// 00324
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00328
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0032c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00334
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00338
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0033c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00340
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00344
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00348
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00350
			 0x0e000001,                                                  // Trailing 1s:                                /// 00354
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0e000007,                                                  // Trailing 1s:                                /// 0035c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00360
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00364
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00368
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0036c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00380
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00388
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00390
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00394
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00398
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003e8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003ec
			 0x00000000,                                                  // zero                                        /// 003f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00400
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00404
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00408
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0040c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00420
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00424
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0042c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00430
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00434
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00440
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00448
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0044c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00454
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0045c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00460
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00468
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0046c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00470
			 0x7f800000,                                                  // inf                                         /// 00474
			 0xffc00001,                                                  // -signaling NaN                              /// 00478
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0047c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00480
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00484
			 0x80000000,                                                  // -zero                                       /// 00488
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0048c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00490
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00494
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 004a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004a8
			 0x3f028f5c,                                                  // 0.51                                        /// 004ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 004c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004e0
			 0xff800000,                                                  // -inf                                        /// 004e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0xbf028f5c,                                                  // -0.51                                       /// 004f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00500
			 0x0e000001,                                                  // Trailing 1s:                                /// 00504
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00508
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0050c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00510
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00518
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00520
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0052c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00538
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0053c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00548
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0054c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00550
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00558
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00568
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0056c
			 0xbf028f5c,                                                  // -0.51                                       /// 00570
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00574
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00578
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0057c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00580
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00590
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00594
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0059c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 005c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0xcb000000,                                                  // -8388608.0                                  /// 005d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 005f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 005fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00600
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00608
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0060c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00610
			 0x0c700000,                                                  // Leading 1s:                                 /// 00614
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0061c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00620
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00624
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00628
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0062c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00630
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00634
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00638
			 0x80000000,                                                  // -zero                                       /// 0063c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00640
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00650
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00654
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0xffc00001,                                                  // -signaling NaN                              /// 00660
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00664
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00668
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0066c
			 0x3f028f5c,                                                  // 0.51                                        /// 00670
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00678
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0067c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00684
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00688
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0068c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00690
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00698
			 0x0c600000,                                                  // Leading 1s:                                 /// 0069c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x3f028f5c,                                                  // 0.51                                        /// 006d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x0c400000,                                                  // Leading 1s:                                 /// 00704
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00708
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0070c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00710
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x33333333,                                                  // 4 random values                             /// 00718
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00724
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00728
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00730
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00738
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0073c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00744
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00748
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0074c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00750
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00758
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0075c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00760
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00764
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00768
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0076c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00770
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00774
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00778
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0077c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00780
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00784
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00788
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0078c
			 0x4b000000,                                                  // 8388608.0                                   /// 00790
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00798
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0079c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 007c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0xcb000000,                                                  // -8388608.0                                  /// 007f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00808
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0080c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00810
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00818
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0081c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00824
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0082c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00830
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00834
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00848
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00850
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00854
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00858
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0085c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00864
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00868
			 0xcb000000,                                                  // -8388608.0                                  /// 0086c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00874
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00878
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0087c
			 0xffc00001,                                                  // -signaling NaN                              /// 00880
			 0xbf028f5c,                                                  // -0.51                                       /// 00884
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00888
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00890
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00898
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0089c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008e8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008f8
			 0x55555555,                                                  // 4 random values                             /// 008fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00904
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0090c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00910
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00914
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0091c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00920
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00924
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00928
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00934
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00938
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0093c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00940
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00944
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0094c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00950
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x00011111,                                                  // 9.7958E-41                                  /// 00958
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0095c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00960
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00964
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00968
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00970
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00974
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00978
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0097c
			 0x7f800000,                                                  // inf                                         /// 00980
			 0xcb000000,                                                  // -8388608.0                                  /// 00984
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00988
			 0xcb000000,                                                  // -8388608.0                                  /// 0098c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00990
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00994
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009f0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0xffc00001,                                                  // -signaling NaN                              /// 009f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a04
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a08
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a1c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a20
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a30
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a34
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a4c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a50
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a54
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a60
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a64
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a68
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a6c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a78
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a84
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a90
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a94
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a9c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00aa0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00aa4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00aac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ab4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ab8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ac4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ad0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ad8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00adc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ae0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ae4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ae8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00aec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00af0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00af4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00afc
			 0x4b000000,                                                  // 8388608.0                                   /// 00b00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b04
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b10
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b1c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b24
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b30
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b40
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b44
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b48
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b4c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b60
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b68
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x7f800000,                                                  // inf                                         /// 00b74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b78
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b88
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b90
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b98
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ba8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bc0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bd0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bd4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00be0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00be4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bf0
			 0x3f028f5c,                                                  // 0.51                                        /// 00bf4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bf8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c00
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c0c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c10
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0xbf028f5c,                                                  // -0.51                                       /// 00c18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c20
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0xff800000,                                                  // -inf                                        /// 00c30
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c48
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c58
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c60
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c6c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c78
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c7c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c8c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c98
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c9c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ca0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ca8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cac
			 0x55555555,                                                  // 4 random values                             /// 00cb0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cb4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cbc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cc4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ccc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cd0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cd4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cdc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ce0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x55555555,                                                  // 4 random values                             /// 00ce8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cec
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cf0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cf4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cf8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cfc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d08
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d10
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d24
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d28
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d34
			 0x00000000,                                                  // zero                                        /// 00d38
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d3c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d48
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d60
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d78
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d80
			 0xbf028f5c,                                                  // -0.51                                       /// 00d84
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d98
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00da4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00db0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00db8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dbc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dc4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dcc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dd0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dd4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ddc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00de0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00de8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dec
			 0xbf028f5c,                                                  // -0.51                                       /// 00df0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00df8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dfc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e00
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e10
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e18
			 0xcb000000,                                                  // -8388608.0                                  /// 00e1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e20
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e28
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e2c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e34
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e38
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e40
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x4b000000,                                                  // 8388608.0                                   /// 00e48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e54
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e64
			 0xffc00001,                                                  // -signaling NaN                              /// 00e68
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e94
			 0x80000000,                                                  // -zero                                       /// 00e98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e9c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ea0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ec0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ec4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ec8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ecc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ed0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ed4
			 0xff800000,                                                  // -inf                                        /// 00ed8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00edc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ee4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ef4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f10
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f1c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f24
			 0xff800000,                                                  // -inf                                        /// 00f28
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f2c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f3c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f4c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f58
			 0x3f028f5c,                                                  // 0.51                                        /// 00f5c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f64
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f6c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f74
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f88
			 0xbf028f5c,                                                  // -0.51                                       /// 00f8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fa0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fa4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fa8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fac
			 0x7f800000,                                                  // inf                                         /// 00fb0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fb4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fb8
			 0x33333333,                                                  // 4 random values                             /// 00fbc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fc0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fc8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fd4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fdc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fe0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fe8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ff4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x80000001 // min subnorm                                   // SP - ve numbers                             /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x8d18b23a, /// 0x0
			 0xe300ce54, /// 0x4
			 0x3c50ccc2, /// 0x8
			 0x1316882c, /// 0xc
			 0x4dd57424, /// 0x10
			 0xa40026cb, /// 0x14
			 0x9f3bb833, /// 0x18
			 0x962dc0bf, /// 0x1c
			 0x5a3cc038, /// 0x20
			 0xb6f30ce9, /// 0x24
			 0xaf799df9, /// 0x28
			 0x9af6f8d3, /// 0x2c
			 0xe4ca5c51, /// 0x30
			 0xc9d4cbf0, /// 0x34
			 0x9916dacf, /// 0x38
			 0x044f9a9a, /// 0x3c
			 0x123c62e7, /// 0x40
			 0xc4bd6f64, /// 0x44
			 0xcda84e03, /// 0x48
			 0xd0c11377, /// 0x4c
			 0xe9c9b9ac, /// 0x50
			 0x2a17162f, /// 0x54
			 0x55642767, /// 0x58
			 0xb62e6755, /// 0x5c
			 0x25935d14, /// 0x60
			 0xa7bdaf6f, /// 0x64
			 0xf67529c4, /// 0x68
			 0x3a10b448, /// 0x6c
			 0xea1c879e, /// 0x70
			 0x63900b73, /// 0x74
			 0x5e9e8e7c, /// 0x78
			 0x251aebc3, /// 0x7c
			 0x7171b367, /// 0x80
			 0x4c8186be, /// 0x84
			 0xc3b169c1, /// 0x88
			 0x6ba304ec, /// 0x8c
			 0xf2635c0f, /// 0x90
			 0x8c3ee0b4, /// 0x94
			 0x168747fc, /// 0x98
			 0x88078592, /// 0x9c
			 0x17a6f974, /// 0xa0
			 0x688af600, /// 0xa4
			 0x2cb57092, /// 0xa8
			 0x07408e2a, /// 0xac
			 0x7f9b7ccb, /// 0xb0
			 0xbe117095, /// 0xb4
			 0xa28908df, /// 0xb8
			 0x3c42dddb, /// 0xbc
			 0x81fe1900, /// 0xc0
			 0x2e7e29af, /// 0xc4
			 0xcd4aba63, /// 0xc8
			 0xf6f88e15, /// 0xcc
			 0x9a2e5fca, /// 0xd0
			 0xc98a2a76, /// 0xd4
			 0xdb23537d, /// 0xd8
			 0x9f08f592, /// 0xdc
			 0x157412a9, /// 0xe0
			 0xce9ae1f8, /// 0xe4
			 0xfa8af6af, /// 0xe8
			 0x34d88ab6, /// 0xec
			 0x9e959581, /// 0xf0
			 0xc79b3524, /// 0xf4
			 0x980b8183, /// 0xf8
			 0x16efff16, /// 0xfc
			 0x36219c7a, /// 0x100
			 0x254703be, /// 0x104
			 0x182b6ec5, /// 0x108
			 0x6f546b01, /// 0x10c
			 0xc1a5082f, /// 0x110
			 0x2cce195d, /// 0x114
			 0xe1a26877, /// 0x118
			 0x59548ad0, /// 0x11c
			 0xaf9a7a30, /// 0x120
			 0xd006b9f8, /// 0x124
			 0x6427f3ae, /// 0x128
			 0x96e7f6c4, /// 0x12c
			 0x3c32de6c, /// 0x130
			 0x2bdad569, /// 0x134
			 0x610a5cc4, /// 0x138
			 0x06b0ebd9, /// 0x13c
			 0x368c6cb6, /// 0x140
			 0x2886a34b, /// 0x144
			 0xecd35bb3, /// 0x148
			 0x62646186, /// 0x14c
			 0xe8d59628, /// 0x150
			 0xca983537, /// 0x154
			 0xb91d2dd5, /// 0x158
			 0x18ab30e6, /// 0x15c
			 0x81773b39, /// 0x160
			 0x1b31b6bb, /// 0x164
			 0x96e0698d, /// 0x168
			 0xa9864bc7, /// 0x16c
			 0x82330aea, /// 0x170
			 0x81a8c143, /// 0x174
			 0xdb043221, /// 0x178
			 0xf2a3d4c3, /// 0x17c
			 0x9fbf27cc, /// 0x180
			 0x01cf77fb, /// 0x184
			 0xd1a8018a, /// 0x188
			 0xac4f13e4, /// 0x18c
			 0xc9078957, /// 0x190
			 0xa1ac59d0, /// 0x194
			 0xbe9a238d, /// 0x198
			 0xb9331137, /// 0x19c
			 0x5deaac76, /// 0x1a0
			 0x543138ab, /// 0x1a4
			 0x16e73e0b, /// 0x1a8
			 0x926cd2e2, /// 0x1ac
			 0xe249ddb4, /// 0x1b0
			 0xfde7546d, /// 0x1b4
			 0xbdbdabee, /// 0x1b8
			 0x7db55ee0, /// 0x1bc
			 0x2d883c71, /// 0x1c0
			 0x3a7db4b7, /// 0x1c4
			 0x831ec0e7, /// 0x1c8
			 0x3edb8681, /// 0x1cc
			 0xebf5db56, /// 0x1d0
			 0x44e272b5, /// 0x1d4
			 0x5986dcae, /// 0x1d8
			 0xd45a4e37, /// 0x1dc
			 0x3322fdaf, /// 0x1e0
			 0xe0d0f518, /// 0x1e4
			 0x3657ccb9, /// 0x1e8
			 0x99b47262, /// 0x1ec
			 0x09ad5573, /// 0x1f0
			 0x8ac4482e, /// 0x1f4
			 0x063e6ce2, /// 0x1f8
			 0xd4db1e8b, /// 0x1fc
			 0x79e148e2, /// 0x200
			 0x535ff710, /// 0x204
			 0xfc90e0a1, /// 0x208
			 0x50f9b919, /// 0x20c
			 0x7c092c92, /// 0x210
			 0x7d9ac9f4, /// 0x214
			 0xee773265, /// 0x218
			 0xe7e86c10, /// 0x21c
			 0xffeec245, /// 0x220
			 0x094afc3a, /// 0x224
			 0xaa1367aa, /// 0x228
			 0xa7609f74, /// 0x22c
			 0x708d499f, /// 0x230
			 0xfe5f3569, /// 0x234
			 0xbee548b0, /// 0x238
			 0x4c6e5c71, /// 0x23c
			 0x1b24bae6, /// 0x240
			 0xa74f31bc, /// 0x244
			 0xa6e85f77, /// 0x248
			 0xff67c8a0, /// 0x24c
			 0x795f7c5d, /// 0x250
			 0xb1a15adb, /// 0x254
			 0x4ea0b303, /// 0x258
			 0x36dd73c5, /// 0x25c
			 0xe3d9463d, /// 0x260
			 0x1a815700, /// 0x264
			 0x9a07298f, /// 0x268
			 0xbb7d4828, /// 0x26c
			 0x2b9d0ed8, /// 0x270
			 0xd22a0b0a, /// 0x274
			 0xa07518a6, /// 0x278
			 0x47c0e37a, /// 0x27c
			 0x126044fb, /// 0x280
			 0xa28c203d, /// 0x284
			 0x180dc6df, /// 0x288
			 0xe8d6d263, /// 0x28c
			 0x00f98262, /// 0x290
			 0x68fb44b7, /// 0x294
			 0xc762db13, /// 0x298
			 0xe2594f2e, /// 0x29c
			 0x9763a0c1, /// 0x2a0
			 0x0561087a, /// 0x2a4
			 0xe1cbc3b0, /// 0x2a8
			 0x3603e842, /// 0x2ac
			 0x383ef3dc, /// 0x2b0
			 0x0221d24c, /// 0x2b4
			 0x75c082a6, /// 0x2b8
			 0xa6ff90d9, /// 0x2bc
			 0xdf13e028, /// 0x2c0
			 0x73e5a56f, /// 0x2c4
			 0x134ac95a, /// 0x2c8
			 0x944f0427, /// 0x2cc
			 0xbd9b017f, /// 0x2d0
			 0x4cfbdad8, /// 0x2d4
			 0x9d8fa859, /// 0x2d8
			 0x565c3d6c, /// 0x2dc
			 0x005fcd9f, /// 0x2e0
			 0x85ecfe15, /// 0x2e4
			 0xe63983b0, /// 0x2e8
			 0x79ead925, /// 0x2ec
			 0x0e7e2331, /// 0x2f0
			 0x5a32fb56, /// 0x2f4
			 0xf7fa842f, /// 0x2f8
			 0xa604e08a, /// 0x2fc
			 0x2adb4cc3, /// 0x300
			 0x20e7e937, /// 0x304
			 0x370e0fcb, /// 0x308
			 0x95ee9d95, /// 0x30c
			 0x1f5b6793, /// 0x310
			 0xca086429, /// 0x314
			 0xe3c00c2c, /// 0x318
			 0xab5b5c20, /// 0x31c
			 0x612b0368, /// 0x320
			 0x1b6a130b, /// 0x324
			 0x6872d943, /// 0x328
			 0x195a0a80, /// 0x32c
			 0x09f37d8f, /// 0x330
			 0xe3728901, /// 0x334
			 0xb9fe8c11, /// 0x338
			 0x3996e0d3, /// 0x33c
			 0x33a3ef9a, /// 0x340
			 0x1f06614c, /// 0x344
			 0xfc01d3bc, /// 0x348
			 0x32c2b139, /// 0x34c
			 0x153ac605, /// 0x350
			 0x4c800803, /// 0x354
			 0xe3301c81, /// 0x358
			 0xab66a4d3, /// 0x35c
			 0xb380a0e3, /// 0x360
			 0x598c72c7, /// 0x364
			 0xe27339d1, /// 0x368
			 0xe0904091, /// 0x36c
			 0x7d6f9a60, /// 0x370
			 0x3eed0727, /// 0x374
			 0x2c99a2a6, /// 0x378
			 0x10d620b9, /// 0x37c
			 0xfb645326, /// 0x380
			 0x5be77e59, /// 0x384
			 0x28be519c, /// 0x388
			 0x5cee057e, /// 0x38c
			 0x8b0acb54, /// 0x390
			 0x6b1fcd28, /// 0x394
			 0x67d36ec2, /// 0x398
			 0x670b079f, /// 0x39c
			 0xd4082445, /// 0x3a0
			 0xc0737583, /// 0x3a4
			 0x7b85e52d, /// 0x3a8
			 0x40277a05, /// 0x3ac
			 0xbd618185, /// 0x3b0
			 0x6cf403bb, /// 0x3b4
			 0xeec60ee9, /// 0x3b8
			 0x61d2ff3e, /// 0x3bc
			 0x59c1c8c8, /// 0x3c0
			 0x72ba54da, /// 0x3c4
			 0x887b323b, /// 0x3c8
			 0xe46bc20a, /// 0x3cc
			 0x7fa2d6e2, /// 0x3d0
			 0x6a6372be, /// 0x3d4
			 0xe779c2dc, /// 0x3d8
			 0xa6aa5f84, /// 0x3dc
			 0x7e2395fd, /// 0x3e0
			 0xee1603a8, /// 0x3e4
			 0x3d1b6fbc, /// 0x3e8
			 0x19e125e7, /// 0x3ec
			 0xba6d8c26, /// 0x3f0
			 0xc5725f7d, /// 0x3f4
			 0x8136e1b5, /// 0x3f8
			 0xcf3a3730, /// 0x3fc
			 0x5ca98e93, /// 0x400
			 0x547f0ac7, /// 0x404
			 0x69081495, /// 0x408
			 0xf8b00ac9, /// 0x40c
			 0xace7a738, /// 0x410
			 0x9989233c, /// 0x414
			 0x658b104d, /// 0x418
			 0x83b5bca0, /// 0x41c
			 0x893f9e7e, /// 0x420
			 0xe0eabf36, /// 0x424
			 0x52cb0439, /// 0x428
			 0x3d6ca128, /// 0x42c
			 0xeb954f11, /// 0x430
			 0xe2322063, /// 0x434
			 0x55432082, /// 0x438
			 0x17b3546e, /// 0x43c
			 0xe508576e, /// 0x440
			 0xd01a4e8e, /// 0x444
			 0xb55724f0, /// 0x448
			 0x2d1bacd2, /// 0x44c
			 0xc42d6e08, /// 0x450
			 0xd043ef64, /// 0x454
			 0x6081019a, /// 0x458
			 0x7a021b9b, /// 0x45c
			 0xd4d73c83, /// 0x460
			 0xe12fc482, /// 0x464
			 0xad056d40, /// 0x468
			 0xd167b90b, /// 0x46c
			 0xfe5507bc, /// 0x470
			 0x6c813a18, /// 0x474
			 0xf167c5fe, /// 0x478
			 0x33cc3be1, /// 0x47c
			 0xd2b3cda8, /// 0x480
			 0x2fbe5495, /// 0x484
			 0xf304015b, /// 0x488
			 0x1c43ef76, /// 0x48c
			 0xbbcc8841, /// 0x490
			 0xf0470f17, /// 0x494
			 0xc4303c00, /// 0x498
			 0x370e6694, /// 0x49c
			 0xdc4052ae, /// 0x4a0
			 0xf84f6675, /// 0x4a4
			 0x668953e4, /// 0x4a8
			 0x4694fab6, /// 0x4ac
			 0xd55ab9a7, /// 0x4b0
			 0xf56f4360, /// 0x4b4
			 0xb369112b, /// 0x4b8
			 0x75371148, /// 0x4bc
			 0x79148062, /// 0x4c0
			 0xb1054a62, /// 0x4c4
			 0x701d0528, /// 0x4c8
			 0x1d383747, /// 0x4cc
			 0xafd89e20, /// 0x4d0
			 0x2c5dc933, /// 0x4d4
			 0x7ab52848, /// 0x4d8
			 0x9f6b5639, /// 0x4dc
			 0x195147c6, /// 0x4e0
			 0xe5ce1353, /// 0x4e4
			 0x030a1a3e, /// 0x4e8
			 0x60dae6d4, /// 0x4ec
			 0xc346ceb0, /// 0x4f0
			 0xd85fa099, /// 0x4f4
			 0x5ebc3a2f, /// 0x4f8
			 0x79a46a13, /// 0x4fc
			 0xedc73359, /// 0x500
			 0x196860c0, /// 0x504
			 0xeef48954, /// 0x508
			 0x0c72cb5e, /// 0x50c
			 0xd75eb66e, /// 0x510
			 0xdc9b7816, /// 0x514
			 0x38b693ec, /// 0x518
			 0x204200f6, /// 0x51c
			 0xc8f93b5d, /// 0x520
			 0x30d0984c, /// 0x524
			 0x195d3ecd, /// 0x528
			 0x763de666, /// 0x52c
			 0x9c97ef59, /// 0x530
			 0xdf680782, /// 0x534
			 0xf8aa3a0c, /// 0x538
			 0x323e32f4, /// 0x53c
			 0xb101410e, /// 0x540
			 0xb7ea3d42, /// 0x544
			 0xc3524ecc, /// 0x548
			 0xc51259c7, /// 0x54c
			 0x8ddf7db2, /// 0x550
			 0xf99a8df8, /// 0x554
			 0x2642199f, /// 0x558
			 0x911013ec, /// 0x55c
			 0xad4b3335, /// 0x560
			 0x1e3d750e, /// 0x564
			 0xff045848, /// 0x568
			 0xd8656a02, /// 0x56c
			 0x301e69b7, /// 0x570
			 0x7601aa23, /// 0x574
			 0x522045bc, /// 0x578
			 0xb0eb3dea, /// 0x57c
			 0xc86ba078, /// 0x580
			 0x35a5a648, /// 0x584
			 0x0ef01af0, /// 0x588
			 0xb7a6751e, /// 0x58c
			 0x3545b9a5, /// 0x590
			 0x81241adb, /// 0x594
			 0x8bde037d, /// 0x598
			 0x691255b1, /// 0x59c
			 0x7b3169ad, /// 0x5a0
			 0x5578a027, /// 0x5a4
			 0xd582e4a2, /// 0x5a8
			 0x00da81f8, /// 0x5ac
			 0x3f47bcd9, /// 0x5b0
			 0x26fa32d2, /// 0x5b4
			 0xf3f61864, /// 0x5b8
			 0xe35c5578, /// 0x5bc
			 0xd5003452, /// 0x5c0
			 0x72b23de9, /// 0x5c4
			 0x13da20d7, /// 0x5c8
			 0x0f43c81e, /// 0x5cc
			 0x7946aab1, /// 0x5d0
			 0xe403ef10, /// 0x5d4
			 0x00b5430e, /// 0x5d8
			 0x92abbb0d, /// 0x5dc
			 0x427f1f4c, /// 0x5e0
			 0x2ab50955, /// 0x5e4
			 0x26f71e9b, /// 0x5e8
			 0x2b0c93c6, /// 0x5ec
			 0x228dd3f7, /// 0x5f0
			 0x73d3a914, /// 0x5f4
			 0xcd49b119, /// 0x5f8
			 0x509b8e01, /// 0x5fc
			 0x13fbadc3, /// 0x600
			 0x8c9fcd5c, /// 0x604
			 0xb8313e74, /// 0x608
			 0xca6b1549, /// 0x60c
			 0xd4e15e2a, /// 0x610
			 0x1bb65c9a, /// 0x614
			 0xbef618e6, /// 0x618
			 0x2e29bf4e, /// 0x61c
			 0xdd7e9419, /// 0x620
			 0x39676ef7, /// 0x624
			 0xd7059277, /// 0x628
			 0xe3720495, /// 0x62c
			 0x1900bba8, /// 0x630
			 0x4b64cd3a, /// 0x634
			 0x82ec9cb3, /// 0x638
			 0x202c523d, /// 0x63c
			 0xb88400be, /// 0x640
			 0x023226ed, /// 0x644
			 0xdc67a198, /// 0x648
			 0x12b9b9f3, /// 0x64c
			 0x063392e1, /// 0x650
			 0xfb92844c, /// 0x654
			 0x49493127, /// 0x658
			 0xc9e17696, /// 0x65c
			 0xfd9ecefe, /// 0x660
			 0x15a4d27c, /// 0x664
			 0x027a5fa5, /// 0x668
			 0x52a84280, /// 0x66c
			 0x2dd02e65, /// 0x670
			 0xd37b6542, /// 0x674
			 0x61016498, /// 0x678
			 0x636b45ee, /// 0x67c
			 0x5a05e34f, /// 0x680
			 0x86a01b0c, /// 0x684
			 0x36011570, /// 0x688
			 0x16cdae56, /// 0x68c
			 0xf5489089, /// 0x690
			 0x4adb85a5, /// 0x694
			 0x058a4196, /// 0x698
			 0x65d03355, /// 0x69c
			 0x5f63dab9, /// 0x6a0
			 0xd4bdfc3d, /// 0x6a4
			 0x10f3de98, /// 0x6a8
			 0xd41b45c8, /// 0x6ac
			 0x90aac787, /// 0x6b0
			 0x5a957683, /// 0x6b4
			 0x82ec4a33, /// 0x6b8
			 0x9ca9f23f, /// 0x6bc
			 0x3b11228e, /// 0x6c0
			 0x50d6ea6b, /// 0x6c4
			 0x9c2f748f, /// 0x6c8
			 0xe49eb2c6, /// 0x6cc
			 0x234b15f7, /// 0x6d0
			 0x645b3fa3, /// 0x6d4
			 0xad79d180, /// 0x6d8
			 0x175deddc, /// 0x6dc
			 0x2c75be1c, /// 0x6e0
			 0x566fd8d7, /// 0x6e4
			 0x95950367, /// 0x6e8
			 0x85cdb27e, /// 0x6ec
			 0x8b4dde29, /// 0x6f0
			 0x43c9bf1f, /// 0x6f4
			 0x266f3ba1, /// 0x6f8
			 0x12c4af42, /// 0x6fc
			 0xb2ba7045, /// 0x700
			 0x1e304cfe, /// 0x704
			 0x93827f4c, /// 0x708
			 0x09891486, /// 0x70c
			 0xeb4d7f61, /// 0x710
			 0x8cc32332, /// 0x714
			 0x2d934ec8, /// 0x718
			 0x68fcbfdb, /// 0x71c
			 0xe2cc1440, /// 0x720
			 0x2bb96708, /// 0x724
			 0xc8f472cc, /// 0x728
			 0x9ef33599, /// 0x72c
			 0x4d004fed, /// 0x730
			 0x0aad4e84, /// 0x734
			 0x41dce01c, /// 0x738
			 0x3f693306, /// 0x73c
			 0x45426336, /// 0x740
			 0xf432741a, /// 0x744
			 0xf0a53a52, /// 0x748
			 0x43d06d08, /// 0x74c
			 0xe993056a, /// 0x750
			 0xde29aeb3, /// 0x754
			 0x4902bd90, /// 0x758
			 0x21703a73, /// 0x75c
			 0x0e8bc92d, /// 0x760
			 0x39fe063e, /// 0x764
			 0x18b7f8ae, /// 0x768
			 0x6c6b2968, /// 0x76c
			 0xbb6a8da2, /// 0x770
			 0x50ae25b0, /// 0x774
			 0xfa9edf6d, /// 0x778
			 0x2502f7ce, /// 0x77c
			 0x8d9077a6, /// 0x780
			 0x5614cf2b, /// 0x784
			 0x191e992b, /// 0x788
			 0x48ff30c9, /// 0x78c
			 0x7c67d409, /// 0x790
			 0x17be8681, /// 0x794
			 0x2fce7b58, /// 0x798
			 0xc251eb56, /// 0x79c
			 0x9a43acfb, /// 0x7a0
			 0x03a9b145, /// 0x7a4
			 0x321ea198, /// 0x7a8
			 0x9e9bcb93, /// 0x7ac
			 0x27eba97d, /// 0x7b0
			 0x5fb8c7fe, /// 0x7b4
			 0xcb676976, /// 0x7b8
			 0x31165045, /// 0x7bc
			 0x71227756, /// 0x7c0
			 0x05ed7b31, /// 0x7c4
			 0xe77e62d1, /// 0x7c8
			 0x51dd0e05, /// 0x7cc
			 0x47e41051, /// 0x7d0
			 0xac7a9dfc, /// 0x7d4
			 0x2c856249, /// 0x7d8
			 0x4a30e29a, /// 0x7dc
			 0x1acf0bc4, /// 0x7e0
			 0xcf1c6d09, /// 0x7e4
			 0x247f1a1e, /// 0x7e8
			 0x9a063eab, /// 0x7ec
			 0x427f5b60, /// 0x7f0
			 0x6efd1dc6, /// 0x7f4
			 0x934821b0, /// 0x7f8
			 0xad71c58f, /// 0x7fc
			 0x766a2425, /// 0x800
			 0x55174cc1, /// 0x804
			 0x0dda84c4, /// 0x808
			 0x23468807, /// 0x80c
			 0x892faf8c, /// 0x810
			 0x62346ce9, /// 0x814
			 0xa0864130, /// 0x818
			 0xddea67fa, /// 0x81c
			 0x47c6f458, /// 0x820
			 0x50521228, /// 0x824
			 0xc5cf278e, /// 0x828
			 0x0bdc85fb, /// 0x82c
			 0xb438de79, /// 0x830
			 0xb1fb5b3b, /// 0x834
			 0xfeed0c58, /// 0x838
			 0x21b3e29e, /// 0x83c
			 0x218efde6, /// 0x840
			 0x27c8ed95, /// 0x844
			 0x93094503, /// 0x848
			 0x2bee8226, /// 0x84c
			 0x2e5e1a6c, /// 0x850
			 0x1889f099, /// 0x854
			 0x4542e8ce, /// 0x858
			 0xaaa4f622, /// 0x85c
			 0x8c736d44, /// 0x860
			 0xec0e9c3b, /// 0x864
			 0xc712e404, /// 0x868
			 0x89328e5b, /// 0x86c
			 0x77c226bd, /// 0x870
			 0xaecca07e, /// 0x874
			 0xff4033d3, /// 0x878
			 0x7d117178, /// 0x87c
			 0x6fb38c6e, /// 0x880
			 0xd2ef1d22, /// 0x884
			 0x88713b5c, /// 0x888
			 0xa5f87401, /// 0x88c
			 0x591f5b1f, /// 0x890
			 0x1bb95d05, /// 0x894
			 0x4170e3e6, /// 0x898
			 0x29268420, /// 0x89c
			 0xddb4afd1, /// 0x8a0
			 0xc4b8e772, /// 0x8a4
			 0x023dda56, /// 0x8a8
			 0x8386dc12, /// 0x8ac
			 0xbb10b0b0, /// 0x8b0
			 0xe28b8320, /// 0x8b4
			 0xbef3fffb, /// 0x8b8
			 0xb689b0d0, /// 0x8bc
			 0x5e9cff1c, /// 0x8c0
			 0xa7879949, /// 0x8c4
			 0xd2d82a93, /// 0x8c8
			 0x61d70e90, /// 0x8cc
			 0x89ab7520, /// 0x8d0
			 0x3ea96886, /// 0x8d4
			 0xee4ad934, /// 0x8d8
			 0x453b6ec6, /// 0x8dc
			 0xd8893f9c, /// 0x8e0
			 0xd2560aae, /// 0x8e4
			 0xa2161570, /// 0x8e8
			 0xa378343b, /// 0x8ec
			 0x839661f4, /// 0x8f0
			 0x011d239b, /// 0x8f4
			 0xbe8dd503, /// 0x8f8
			 0x9d46a3a1, /// 0x8fc
			 0xabda7e68, /// 0x900
			 0xd4a9ac48, /// 0x904
			 0x49c3cf56, /// 0x908
			 0xdb269ac2, /// 0x90c
			 0x569d2f42, /// 0x910
			 0x9381c3eb, /// 0x914
			 0x521125f6, /// 0x918
			 0x33fadf48, /// 0x91c
			 0xcc83851a, /// 0x920
			 0x1ac253b2, /// 0x924
			 0xa6f79c67, /// 0x928
			 0x9ee6c01d, /// 0x92c
			 0x5e552c9f, /// 0x930
			 0x57e8f27a, /// 0x934
			 0xeb568ff2, /// 0x938
			 0x6a8487ca, /// 0x93c
			 0x62068d75, /// 0x940
			 0xb2a5c817, /// 0x944
			 0x3d260b7c, /// 0x948
			 0x7eec8ea6, /// 0x94c
			 0x9fdd9d3d, /// 0x950
			 0x5fb59ebc, /// 0x954
			 0xe0eac23b, /// 0x958
			 0x3724ed2e, /// 0x95c
			 0x3526a630, /// 0x960
			 0xa652dae3, /// 0x964
			 0x0873a023, /// 0x968
			 0x721fcc0d, /// 0x96c
			 0x7fd806a6, /// 0x970
			 0x7ca381f9, /// 0x974
			 0x9b68424c, /// 0x978
			 0x141817e7, /// 0x97c
			 0x691108df, /// 0x980
			 0xc4cea940, /// 0x984
			 0x6f5797fb, /// 0x988
			 0xc9eff272, /// 0x98c
			 0xb49bb1a3, /// 0x990
			 0xa294072a, /// 0x994
			 0xa4c33603, /// 0x998
			 0x3babf97c, /// 0x99c
			 0x5d924091, /// 0x9a0
			 0x79ae4fc2, /// 0x9a4
			 0x4396fcb9, /// 0x9a8
			 0x50ab88eb, /// 0x9ac
			 0x517ac83d, /// 0x9b0
			 0x17757bcb, /// 0x9b4
			 0xbc7578a5, /// 0x9b8
			 0x651c7f9a, /// 0x9bc
			 0xfbe3caa2, /// 0x9c0
			 0x4b8ec1ad, /// 0x9c4
			 0x901928b7, /// 0x9c8
			 0xd840265e, /// 0x9cc
			 0xe2eb0842, /// 0x9d0
			 0x6d3a058d, /// 0x9d4
			 0x431ad136, /// 0x9d8
			 0x8e2c8aab, /// 0x9dc
			 0x7d3494d1, /// 0x9e0
			 0x0668721c, /// 0x9e4
			 0x01895118, /// 0x9e8
			 0xcb723999, /// 0x9ec
			 0x7de4d4d5, /// 0x9f0
			 0x03219173, /// 0x9f4
			 0xfd4291fb, /// 0x9f8
			 0x87d0de03, /// 0x9fc
			 0xb44a2caa, /// 0xa00
			 0x12739c8c, /// 0xa04
			 0x9f2edf78, /// 0xa08
			 0x8588f968, /// 0xa0c
			 0xd2556c6f, /// 0xa10
			 0xe326fa84, /// 0xa14
			 0x47994436, /// 0xa18
			 0x26e47422, /// 0xa1c
			 0x509f7ce4, /// 0xa20
			 0x96c28217, /// 0xa24
			 0x1e32c939, /// 0xa28
			 0x2edc147d, /// 0xa2c
			 0x774fcb01, /// 0xa30
			 0x430e5b98, /// 0xa34
			 0xb547dd2a, /// 0xa38
			 0xa69455db, /// 0xa3c
			 0x56febfb4, /// 0xa40
			 0x63936cd2, /// 0xa44
			 0x48ca6e62, /// 0xa48
			 0xa0357834, /// 0xa4c
			 0xee7d52c1, /// 0xa50
			 0x42267f3b, /// 0xa54
			 0x2387b840, /// 0xa58
			 0x1c59f423, /// 0xa5c
			 0x3805da80, /// 0xa60
			 0x8e95f50a, /// 0xa64
			 0xdbac5b66, /// 0xa68
			 0x87f8721a, /// 0xa6c
			 0x120c6cee, /// 0xa70
			 0x436835e1, /// 0xa74
			 0x460de7eb, /// 0xa78
			 0xa576857c, /// 0xa7c
			 0x5c708de2, /// 0xa80
			 0x79dda69a, /// 0xa84
			 0x78d0d71d, /// 0xa88
			 0x239a9257, /// 0xa8c
			 0x205d737d, /// 0xa90
			 0xbd8e7d9a, /// 0xa94
			 0x619c4169, /// 0xa98
			 0x7fc2081e, /// 0xa9c
			 0x36a594b1, /// 0xaa0
			 0xa60c5d24, /// 0xaa4
			 0xa3fc6604, /// 0xaa8
			 0x0a95d34d, /// 0xaac
			 0xbed3e24f, /// 0xab0
			 0x5c151549, /// 0xab4
			 0x2abc2058, /// 0xab8
			 0x67dab58d, /// 0xabc
			 0x3cd82a5f, /// 0xac0
			 0xff987fe0, /// 0xac4
			 0xaf429571, /// 0xac8
			 0x0faaf1c1, /// 0xacc
			 0x4e76fc1e, /// 0xad0
			 0xbeb6fd80, /// 0xad4
			 0x5a4c7403, /// 0xad8
			 0xd5c5035d, /// 0xadc
			 0x126d2371, /// 0xae0
			 0x16fce668, /// 0xae4
			 0xbf6e839a, /// 0xae8
			 0x7e8878a6, /// 0xaec
			 0x2e073b05, /// 0xaf0
			 0xa2d2a8e9, /// 0xaf4
			 0x12bbe261, /// 0xaf8
			 0xb98215e1, /// 0xafc
			 0xda72e03e, /// 0xb00
			 0x70cbdce9, /// 0xb04
			 0xb4447b6a, /// 0xb08
			 0x6227fde6, /// 0xb0c
			 0xed9b5d09, /// 0xb10
			 0x0681848e, /// 0xb14
			 0x3c3d0eda, /// 0xb18
			 0xdc9a18ad, /// 0xb1c
			 0x1745392d, /// 0xb20
			 0x8a9fdb0e, /// 0xb24
			 0xb3a133a5, /// 0xb28
			 0xe3483f16, /// 0xb2c
			 0x86c1bdad, /// 0xb30
			 0x8a0a3422, /// 0xb34
			 0x4b608c25, /// 0xb38
			 0x78247c4c, /// 0xb3c
			 0xe0506d62, /// 0xb40
			 0x9ca7ca00, /// 0xb44
			 0xa92cf89f, /// 0xb48
			 0xc8b1cdc7, /// 0xb4c
			 0x0610f671, /// 0xb50
			 0x75b62a66, /// 0xb54
			 0x1ae14623, /// 0xb58
			 0x92a2ee1d, /// 0xb5c
			 0xb669b8e9, /// 0xb60
			 0x6cfe4209, /// 0xb64
			 0xbf8bc029, /// 0xb68
			 0x37e17196, /// 0xb6c
			 0xf5c01d8b, /// 0xb70
			 0x5d28bfde, /// 0xb74
			 0x5c7e0ecf, /// 0xb78
			 0x3405d32f, /// 0xb7c
			 0xa2b3a432, /// 0xb80
			 0x50ed8ac0, /// 0xb84
			 0xa25abe82, /// 0xb88
			 0xb0eddaf6, /// 0xb8c
			 0xcdcda7d9, /// 0xb90
			 0xd3da8a3e, /// 0xb94
			 0xa26d6c17, /// 0xb98
			 0x0364d062, /// 0xb9c
			 0xabd8ce76, /// 0xba0
			 0x442bb9bd, /// 0xba4
			 0x7a3ad5f8, /// 0xba8
			 0xf1beaf10, /// 0xbac
			 0x647f5051, /// 0xbb0
			 0x6b20c957, /// 0xbb4
			 0xd2582f97, /// 0xbb8
			 0xbc562598, /// 0xbbc
			 0x1e69c9ae, /// 0xbc0
			 0x7f22f1ca, /// 0xbc4
			 0x6c95197b, /// 0xbc8
			 0x9012668d, /// 0xbcc
			 0x61e0c6c7, /// 0xbd0
			 0x613c4f2c, /// 0xbd4
			 0x38a89597, /// 0xbd8
			 0x6ab4f521, /// 0xbdc
			 0x5661d088, /// 0xbe0
			 0x5beec989, /// 0xbe4
			 0xc66d7217, /// 0xbe8
			 0xf00198d9, /// 0xbec
			 0xb52a5ade, /// 0xbf0
			 0xd2d14268, /// 0xbf4
			 0xb2b13d20, /// 0xbf8
			 0x7c2fafcf, /// 0xbfc
			 0x3d7d6243, /// 0xc00
			 0xc686a378, /// 0xc04
			 0x4dfae8bd, /// 0xc08
			 0x67c727e0, /// 0xc0c
			 0x80c17efd, /// 0xc10
			 0xf03a4f16, /// 0xc14
			 0x94463c7b, /// 0xc18
			 0x49a05644, /// 0xc1c
			 0x46115de0, /// 0xc20
			 0xccef0044, /// 0xc24
			 0x857ab8a6, /// 0xc28
			 0xea02946f, /// 0xc2c
			 0x0abe0817, /// 0xc30
			 0x701fa79d, /// 0xc34
			 0x27d7d635, /// 0xc38
			 0xc5a8b039, /// 0xc3c
			 0xa12a8f4b, /// 0xc40
			 0xaa23f94d, /// 0xc44
			 0x66deb1ff, /// 0xc48
			 0x372fd120, /// 0xc4c
			 0xb4bf9d1f, /// 0xc50
			 0x32e7ab57, /// 0xc54
			 0x35e21d3c, /// 0xc58
			 0x69783930, /// 0xc5c
			 0xb5f4316e, /// 0xc60
			 0x43efda76, /// 0xc64
			 0x81c22cf7, /// 0xc68
			 0xf20f37f6, /// 0xc6c
			 0xc372acc6, /// 0xc70
			 0x5b43f3c7, /// 0xc74
			 0x8235c3af, /// 0xc78
			 0xaa1339b8, /// 0xc7c
			 0x04a0d4b6, /// 0xc80
			 0x66f01603, /// 0xc84
			 0x36b40edb, /// 0xc88
			 0x774edd34, /// 0xc8c
			 0x452e0e02, /// 0xc90
			 0xb80733c2, /// 0xc94
			 0xb73cfea4, /// 0xc98
			 0x5d8c9b06, /// 0xc9c
			 0x8322ddc1, /// 0xca0
			 0x87c1110b, /// 0xca4
			 0xc951c597, /// 0xca8
			 0x04dbb6e6, /// 0xcac
			 0x7e5beefa, /// 0xcb0
			 0xce06a02e, /// 0xcb4
			 0x35dd1d20, /// 0xcb8
			 0xcf2e683b, /// 0xcbc
			 0xade77442, /// 0xcc0
			 0xc63270f7, /// 0xcc4
			 0x4a3ebe86, /// 0xcc8
			 0x0b97f089, /// 0xccc
			 0x594581c4, /// 0xcd0
			 0xe23199d7, /// 0xcd4
			 0x146f276c, /// 0xcd8
			 0xb8ebea54, /// 0xcdc
			 0x36e8a5b6, /// 0xce0
			 0x95c28458, /// 0xce4
			 0x1e1b6d6a, /// 0xce8
			 0x02b98a15, /// 0xcec
			 0xf649a21e, /// 0xcf0
			 0x69d9c8b9, /// 0xcf4
			 0x8eacfe15, /// 0xcf8
			 0xd5ec43f2, /// 0xcfc
			 0x26c238e2, /// 0xd00
			 0xde7fe34e, /// 0xd04
			 0x8ba08464, /// 0xd08
			 0x495334af, /// 0xd0c
			 0xfb1ca657, /// 0xd10
			 0x1440c964, /// 0xd14
			 0xf2141a8d, /// 0xd18
			 0xf14b672f, /// 0xd1c
			 0x533f6091, /// 0xd20
			 0xb6a304d3, /// 0xd24
			 0xbe40e209, /// 0xd28
			 0x7c9d590f, /// 0xd2c
			 0x6c7de77f, /// 0xd30
			 0x0c629ebe, /// 0xd34
			 0xd7af62c7, /// 0xd38
			 0x2319dc9c, /// 0xd3c
			 0xbd190588, /// 0xd40
			 0xcac5dfea, /// 0xd44
			 0x0fc359bf, /// 0xd48
			 0xff8b62bb, /// 0xd4c
			 0x366c1df1, /// 0xd50
			 0x3cdcf35c, /// 0xd54
			 0xa569fc72, /// 0xd58
			 0xb2f3553f, /// 0xd5c
			 0xedddabae, /// 0xd60
			 0x66059fdd, /// 0xd64
			 0x350ff3db, /// 0xd68
			 0xce375f1a, /// 0xd6c
			 0xd81280ea, /// 0xd70
			 0xe9164d4f, /// 0xd74
			 0x689f68dc, /// 0xd78
			 0x09f3ce82, /// 0xd7c
			 0xf1e95b3c, /// 0xd80
			 0x913234cf, /// 0xd84
			 0x32c02067, /// 0xd88
			 0x4e2096b4, /// 0xd8c
			 0x1e029c29, /// 0xd90
			 0xe88772b2, /// 0xd94
			 0x5c7b0466, /// 0xd98
			 0x7e017e37, /// 0xd9c
			 0xf1f96fa3, /// 0xda0
			 0xb868d10d, /// 0xda4
			 0x1cf43004, /// 0xda8
			 0x4e5db292, /// 0xdac
			 0x69d10790, /// 0xdb0
			 0x2b7d80eb, /// 0xdb4
			 0x16a9b9bf, /// 0xdb8
			 0x466245e0, /// 0xdbc
			 0x388d424d, /// 0xdc0
			 0x48455fa8, /// 0xdc4
			 0x91b7998b, /// 0xdc8
			 0x34223312, /// 0xdcc
			 0x562b00c5, /// 0xdd0
			 0xe1f7bd79, /// 0xdd4
			 0x2f7b300e, /// 0xdd8
			 0x5bcdc394, /// 0xddc
			 0x38502b2b, /// 0xde0
			 0x58853781, /// 0xde4
			 0x5b8b256e, /// 0xde8
			 0xa8233b1f, /// 0xdec
			 0x745fc059, /// 0xdf0
			 0x6d2677e7, /// 0xdf4
			 0x7edbdacb, /// 0xdf8
			 0xe95377a3, /// 0xdfc
			 0xe521e627, /// 0xe00
			 0x54d053f0, /// 0xe04
			 0x8f405346, /// 0xe08
			 0x8813639c, /// 0xe0c
			 0xb9d0cc2b, /// 0xe10
			 0x288c8e5f, /// 0xe14
			 0x9c8cf279, /// 0xe18
			 0x567f140d, /// 0xe1c
			 0xbe7ddec0, /// 0xe20
			 0xfae61faa, /// 0xe24
			 0xf6fc3421, /// 0xe28
			 0x4f161989, /// 0xe2c
			 0xf78a44da, /// 0xe30
			 0x12ccbf8a, /// 0xe34
			 0x1a9a9ba1, /// 0xe38
			 0x7c7183c5, /// 0xe3c
			 0xfef6692c, /// 0xe40
			 0x4eabc771, /// 0xe44
			 0xbd2f4dd8, /// 0xe48
			 0x2a2b9a01, /// 0xe4c
			 0xdfe69cc7, /// 0xe50
			 0xf8f065ed, /// 0xe54
			 0x0d12d766, /// 0xe58
			 0x4618e5ae, /// 0xe5c
			 0x739e0dd8, /// 0xe60
			 0x44a7eadd, /// 0xe64
			 0x7c13b5e9, /// 0xe68
			 0xa3596243, /// 0xe6c
			 0xbec23e27, /// 0xe70
			 0x7fe87275, /// 0xe74
			 0xf97d0324, /// 0xe78
			 0x09e0597b, /// 0xe7c
			 0x40a5c3d7, /// 0xe80
			 0x9246ef5c, /// 0xe84
			 0x3a096a51, /// 0xe88
			 0x2e5c2244, /// 0xe8c
			 0x48f6d946, /// 0xe90
			 0x4e08db36, /// 0xe94
			 0x93fc46ec, /// 0xe98
			 0xbf025fce, /// 0xe9c
			 0x55f98558, /// 0xea0
			 0xc4d068f5, /// 0xea4
			 0x37c5953c, /// 0xea8
			 0x729e0476, /// 0xeac
			 0xe5048357, /// 0xeb0
			 0x34af1b66, /// 0xeb4
			 0x00511a80, /// 0xeb8
			 0xaef6264d, /// 0xebc
			 0x004db50a, /// 0xec0
			 0x3845504a, /// 0xec4
			 0xd2f9c31a, /// 0xec8
			 0xb70e339e, /// 0xecc
			 0x04f7e206, /// 0xed0
			 0xa0c37a0d, /// 0xed4
			 0x0aef03e7, /// 0xed8
			 0x7f7f7db9, /// 0xedc
			 0x96cd08f4, /// 0xee0
			 0xda75b519, /// 0xee4
			 0x027a653c, /// 0xee8
			 0x1476b75d, /// 0xeec
			 0x1a6e589a, /// 0xef0
			 0x363bebcc, /// 0xef4
			 0xea346abc, /// 0xef8
			 0xa7763850, /// 0xefc
			 0xb4882b4e, /// 0xf00
			 0xa059f032, /// 0xf04
			 0x01f8261d, /// 0xf08
			 0xb29da694, /// 0xf0c
			 0x0b969c68, /// 0xf10
			 0xe0656d03, /// 0xf14
			 0x748d8db0, /// 0xf18
			 0xb03d6494, /// 0xf1c
			 0x83924530, /// 0xf20
			 0x1146fce8, /// 0xf24
			 0xefa23cee, /// 0xf28
			 0x452cb7e4, /// 0xf2c
			 0x7b4dde31, /// 0xf30
			 0xc649250e, /// 0xf34
			 0xa97cf164, /// 0xf38
			 0xee2d9f23, /// 0xf3c
			 0xe4261c78, /// 0xf40
			 0xca199b14, /// 0xf44
			 0x9c892967, /// 0xf48
			 0x633cc971, /// 0xf4c
			 0x29d2fb32, /// 0xf50
			 0x6405093c, /// 0xf54
			 0xa828f4fe, /// 0xf58
			 0x57278c3b, /// 0xf5c
			 0x6c6d3ca1, /// 0xf60
			 0xad2ecf4c, /// 0xf64
			 0xe0efe38c, /// 0xf68
			 0x226d6a3b, /// 0xf6c
			 0x9861918c, /// 0xf70
			 0x10dca6e0, /// 0xf74
			 0xd62f4a5c, /// 0xf78
			 0xb7d2e18c, /// 0xf7c
			 0xab376153, /// 0xf80
			 0x1741d332, /// 0xf84
			 0x56e22562, /// 0xf88
			 0xc438ccff, /// 0xf8c
			 0xa5cb0421, /// 0xf90
			 0xb9bb1514, /// 0xf94
			 0xfc19105c, /// 0xf98
			 0x9bd4d77b, /// 0xf9c
			 0xdd819045, /// 0xfa0
			 0x8bd24a4d, /// 0xfa4
			 0x2c55f8d9, /// 0xfa8
			 0xcddd81d0, /// 0xfac
			 0xf26db59b, /// 0xfb0
			 0xfdd9b3ee, /// 0xfb4
			 0x6b05c43b, /// 0xfb8
			 0x4c7121fe, /// 0xfbc
			 0xc24f0a57, /// 0xfc0
			 0xd48badfa, /// 0xfc4
			 0x76113bc6, /// 0xfc8
			 0xe5841200, /// 0xfcc
			 0x86a08170, /// 0xfd0
			 0xf4e0d8df, /// 0xfd4
			 0xebdbf388, /// 0xfd8
			 0xd62ebd8a, /// 0xfdc
			 0x5a34cdd7, /// 0xfe0
			 0x3aa721f2, /// 0xfe4
			 0x01793af1, /// 0xfe8
			 0xce9f627c, /// 0xfec
			 0xc21be554, /// 0xff0
			 0x35d92a70, /// 0xff4
			 0x13fb9bc9, /// 0xff8
			 0x07e057f4 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00000
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00004
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00008
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00010
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00018
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00020
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00024
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00028
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0002c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00034
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00038
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00044
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0004c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00050
			 0x0e000001,                                                  // Trailing 1s:                                /// 00054
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00058
			 0x0e000007,                                                  // Trailing 1s:                                /// 0005c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0006c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00070
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0007c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00080
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00084
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00088
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0008c
			 0xff800000,                                                  // -inf                                        /// 00090
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0009c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x00000000,                                                  // zero                                        /// 000b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 000f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00100
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00104
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00108
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0010c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00110
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0011c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00120
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00124
			 0x0c600000,                                                  // Leading 1s:                                 /// 00128
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0012c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00134
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00138
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0013c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0c700000,                                                  // Leading 1s:                                 /// 00144
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x0c700000,                                                  // Leading 1s:                                 /// 0014c
			 0x33333333,                                                  // 4 random values                             /// 00150
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00154
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0015c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00160
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00164
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00168
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00174
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0017c
			 0xbf028f5c,                                                  // -0.51                                       /// 00180
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00184
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x4b000000,                                                  // 8388608.0                                   /// 0018c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00190
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00194
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00198
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0019c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001a4
			 0x4b000000,                                                  // 8388608.0                                   /// 001a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001b8
			 0x3f028f5c,                                                  // 0.51                                        /// 001bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 001c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 001e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 001ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 001fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00208
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00210
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00214
			 0x80000000,                                                  // -zero                                       /// 00218
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0021c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x80011111,                                                  // -9.7958E-41                                 /// 00224
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0022c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00230
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00234
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00238
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00240
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00244
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00258
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0025c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00260
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00264
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00268
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00270
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00278
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00280
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0028c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00290
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00294
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0029c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00304
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00308
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00310
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00314
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00318
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00324
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00328
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00330
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00334
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0033c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00344
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00348
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0034c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00350
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00354
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x3f028f5c,                                                  // 0.51                                        /// 00360
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00364
			 0x33333333,                                                  // 4 random values                             /// 00368
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0036c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00370
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00374
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00378
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0037c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00384
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00388
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0038c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00390
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00394
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003bc
			 0x55555555,                                                  // 4 random values                             /// 003c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0xff800000,                                                  // -inf                                        /// 003d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0xffc00001,                                                  // -signaling NaN                              /// 003f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00400
			 0xbf028f5c,                                                  // -0.51                                       /// 00404
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00414
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0041c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00420
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00424
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0042c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00430
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00434
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00438
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0043c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00444
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00448
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0044c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0045c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00464
			 0x33333333,                                                  // 4 random values                             /// 00468
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0046c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00470
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0047c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00484
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00488
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0048c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00498
			 0x00000000,                                                  // zero                                        /// 0049c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004a0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004b8
			 0xbf028f5c,                                                  // -0.51                                       /// 004bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x7fc00001,                                                  // signaling NaN                               /// 004c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004e4
			 0x7f800000,                                                  // inf                                         /// 004e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004f4
			 0x4b000000,                                                  // 8388608.0                                   /// 004f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00500
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00510
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00514
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00518
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0051c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00528
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0052c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00534
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00538
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00540
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00544
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00548
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00554
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00558
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00560
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00564
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00568
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0056c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00570
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x55555555,                                                  // 4 random values                             /// 00578
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x0c600000,                                                  // Leading 1s:                                 /// 00584
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00588
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00590
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00594
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00598
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0059c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005d4
			 0x7fc00001,                                                  // signaling NaN                               /// 005d8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005f0
			 0xffc00001,                                                  // -signaling NaN                              /// 005f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00604
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00608
			 0x0c700000,                                                  // Leading 1s:                                 /// 0060c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00610
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0061c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00628
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0062c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00630
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00638
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0063c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00644
			 0x4b000000,                                                  // 8388608.0                                   /// 00648
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0064c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00650
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0065c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00660
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00668
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0066c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00670
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00678
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0067c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00680
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00688
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0068c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00690
			 0x80011111,                                                  // -9.7958E-41                                 /// 00694
			 0x4b000000,                                                  // 8388608.0                                   /// 00698
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00700
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00708
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0070c
			 0x80000000,                                                  // -zero                                       /// 00710
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0xffc00001,                                                  // -signaling NaN                              /// 00718
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0071c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00728
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0072c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00738
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00740
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00744
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0074c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0075c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00760
			 0x33333333,                                                  // 4 random values                             /// 00764
			 0xcb000000,                                                  // -8388608.0                                  /// 00768
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0076c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00770
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0c780000,                                                  // Leading 1s:                                 /// 00778
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0077c
			 0x33333333,                                                  // 4 random values                             /// 00780
			 0x33333333,                                                  // 4 random values                             /// 00784
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00790
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00794
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0xffc00001,                                                  // -signaling NaN                              /// 0079c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007a4
			 0xff800000,                                                  // -inf                                        /// 007a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x00000000,                                                  // zero                                        /// 007b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00808
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00810
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00818
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0081c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00820
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0e000001,                                                  // Trailing 1s:                                /// 0082c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00838
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00848
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0084c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00850
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00854
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0xff800000,                                                  // -inf                                        /// 0085c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00864
			 0xcb000000,                                                  // -8388608.0                                  /// 00868
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0086c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00870
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0087c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00880
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00884
			 0x0e000007,                                                  // Trailing 1s:                                /// 00888
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0088c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00890
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00894
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00898
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0089c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00900
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00904
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0e000001,                                                  // Trailing 1s:                                /// 0090c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00918
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00928
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0092c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00938
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0093c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00940
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00944
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0094c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00950
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00954
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00958
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0095c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00960
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00964
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0096c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00970
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00978
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0097c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00980
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00984
			 0x55555555,                                                  // 4 random values                             /// 00988
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0098c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00990
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0xff800000,                                                  // -inf                                        /// 009c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x3f028f5c,                                                  // 0.51                                        /// 009d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a04
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a08
			 0x7fc00001,                                                  // signaling NaN                               /// 00a0c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a18
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x3f028f5c,                                                  // 0.51                                        /// 00a24
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a28
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a30
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a3c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a40
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a4c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a50
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a58
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a64
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a6c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a78
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a7c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a80
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a84
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a94
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a98
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00aa0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00aa4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ab8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00abc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ac4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ac8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00acc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0xff800000,                                                  // -inf                                        /// 00ad4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ad8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00adc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ae0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00af8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00afc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b10
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b24
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b34
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b38
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b3c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b40
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b44
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b4c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0xff800000,                                                  // -inf                                        /// 00b54
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b58
			 0xcb000000,                                                  // -8388608.0                                  /// 00b5c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b60
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b64
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b68
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b6c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b70
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b74
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b7c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b84
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b88
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b8c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b9c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ba0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ba8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bb0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bb4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bb8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bc4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x7f800000,                                                  // inf                                         /// 00bd0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bd4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bd8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00be0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00be4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00be8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bf4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bf8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bfc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c08
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c14
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c1c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c28
			 0x55555555,                                                  // 4 random values                             /// 00c2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c3c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c40
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c44
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c4c
			 0x00000000,                                                  // zero                                        /// 00c50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c54
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c58
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c5c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c60
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c64
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c78
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c80
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c84
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c8c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c98
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ca0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cb0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cb4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cc0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cc4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cc8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ccc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cd0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cd8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ce0
			 0x80000000,                                                  // -zero                                       /// 00ce4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ce8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cf0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cf4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cf8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d08
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d0c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d2c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d3c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d4c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d5c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d60
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d70
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d8c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d98
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00da0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00da4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00da8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00db4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00db8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dc8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dcc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0xffc00001,                                                  // -signaling NaN                              /// 00dd4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dd8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00de0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00de4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00de8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00df0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00df4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00df8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dfc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e14
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e24
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e28
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e30
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e38
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e40
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e54
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e60
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e7c
			 0x55555555,                                                  // 4 random values                             /// 00e80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e84
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e88
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ea0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ea8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00eac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00eb0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eb4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00eb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ec4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ed0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ed8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ee0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ee4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ee8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00eec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ef0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ef8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00efc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f00
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f0c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f10
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f24
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0xffc00001,                                                  // -signaling NaN                              /// 00f2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f48
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f6c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f84
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f8c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fa0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fa8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fb0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fb4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fb8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fc4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fc8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fcc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fd0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fd4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0xbf028f5c,                                                  // -0.51                                       /// 00fe0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fe4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fe8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ff0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ff4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ff8
			 0x80000010                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x9779d191, /// 0x0
			 0x709a3a30, /// 0x4
			 0xa443b6bd, /// 0x8
			 0x916511bc, /// 0xc
			 0xd8d93c9f, /// 0x10
			 0x654faf98, /// 0x14
			 0x4249dbe7, /// 0x18
			 0x6f409e3b, /// 0x1c
			 0x3daa1e8b, /// 0x20
			 0x36bea279, /// 0x24
			 0x47f3bd11, /// 0x28
			 0x30157b9d, /// 0x2c
			 0x6274a5f8, /// 0x30
			 0xe1d94835, /// 0x34
			 0x2273ff1a, /// 0x38
			 0x01e7ae3c, /// 0x3c
			 0xfd327690, /// 0x40
			 0x091e87e5, /// 0x44
			 0xec57c5f0, /// 0x48
			 0xdcf3ae1f, /// 0x4c
			 0x1011a4ac, /// 0x50
			 0xdc62a782, /// 0x54
			 0x1a027eac, /// 0x58
			 0x30d6032e, /// 0x5c
			 0xde070bf7, /// 0x60
			 0xfddb9bdd, /// 0x64
			 0xa3169ecd, /// 0x68
			 0x52672f31, /// 0x6c
			 0xdc31d90e, /// 0x70
			 0x134412f9, /// 0x74
			 0x00ebe9b6, /// 0x78
			 0x8447638e, /// 0x7c
			 0xfb352de7, /// 0x80
			 0xb27fb0d7, /// 0x84
			 0xfc28f8e3, /// 0x88
			 0xd6b4f053, /// 0x8c
			 0xdbb1e5f3, /// 0x90
			 0x771eff51, /// 0x94
			 0x2aaf46b1, /// 0x98
			 0x98883a85, /// 0x9c
			 0x887a7188, /// 0xa0
			 0xebc7d42b, /// 0xa4
			 0x66a41aa4, /// 0xa8
			 0xd4bed874, /// 0xac
			 0x7b68ae04, /// 0xb0
			 0x74f32e2f, /// 0xb4
			 0xf84505d1, /// 0xb8
			 0x894d027c, /// 0xbc
			 0xf74509ce, /// 0xc0
			 0x057ec6d5, /// 0xc4
			 0x00b867ba, /// 0xc8
			 0x5fab6ac7, /// 0xcc
			 0x19a392ff, /// 0xd0
			 0xee998f11, /// 0xd4
			 0x9eaca89e, /// 0xd8
			 0x41d94731, /// 0xdc
			 0x75f0204e, /// 0xe0
			 0x832bce7a, /// 0xe4
			 0xc5f64bcf, /// 0xe8
			 0xb1969230, /// 0xec
			 0xd147210b, /// 0xf0
			 0xbb947556, /// 0xf4
			 0x10d22c33, /// 0xf8
			 0x607f332e, /// 0xfc
			 0x68aace43, /// 0x100
			 0xf191d1c9, /// 0x104
			 0xf2a5d00a, /// 0x108
			 0xdf631977, /// 0x10c
			 0x5590260b, /// 0x110
			 0xa3ec22da, /// 0x114
			 0xe8462060, /// 0x118
			 0xa065522b, /// 0x11c
			 0x39617531, /// 0x120
			 0x959db4fc, /// 0x124
			 0xba8e937e, /// 0x128
			 0xa244a506, /// 0x12c
			 0x0d3df76e, /// 0x130
			 0x9ec1e309, /// 0x134
			 0xaac93265, /// 0x138
			 0xcbdb01ec, /// 0x13c
			 0x50fd8e51, /// 0x140
			 0x8650e5f0, /// 0x144
			 0xed774ba8, /// 0x148
			 0xdb4b74e6, /// 0x14c
			 0x7fb87505, /// 0x150
			 0xee3d24f4, /// 0x154
			 0xcbdaf35f, /// 0x158
			 0x35e2d01e, /// 0x15c
			 0x91731fad, /// 0x160
			 0xb39600bc, /// 0x164
			 0x449967b6, /// 0x168
			 0x9efaa4d4, /// 0x16c
			 0xfe4a6b90, /// 0x170
			 0xc7621c31, /// 0x174
			 0x9c2e848c, /// 0x178
			 0xf477e827, /// 0x17c
			 0x8904017d, /// 0x180
			 0x51bf57c2, /// 0x184
			 0xa9c92bd3, /// 0x188
			 0x29adfd26, /// 0x18c
			 0x71ceb36f, /// 0x190
			 0x6ce70df2, /// 0x194
			 0x541a26fd, /// 0x198
			 0x7aec506b, /// 0x19c
			 0xd81b7032, /// 0x1a0
			 0x0ee1e70e, /// 0x1a4
			 0xe5f2c906, /// 0x1a8
			 0x7a8366a5, /// 0x1ac
			 0x4f01bc26, /// 0x1b0
			 0x49418820, /// 0x1b4
			 0x58e3bf7a, /// 0x1b8
			 0x07a0bc30, /// 0x1bc
			 0xca7fec21, /// 0x1c0
			 0x496c36b8, /// 0x1c4
			 0x691366fd, /// 0x1c8
			 0x12490d1b, /// 0x1cc
			 0x770f0eb0, /// 0x1d0
			 0x35cdc109, /// 0x1d4
			 0xe86e4a75, /// 0x1d8
			 0xfe2a6517, /// 0x1dc
			 0x9a887b9d, /// 0x1e0
			 0x784d71dc, /// 0x1e4
			 0x12adbb00, /// 0x1e8
			 0x0f9227e4, /// 0x1ec
			 0x5dc7626d, /// 0x1f0
			 0xa52c5c28, /// 0x1f4
			 0xa7534fb5, /// 0x1f8
			 0xbb0617ed, /// 0x1fc
			 0xf6f4229c, /// 0x200
			 0x2ac29029, /// 0x204
			 0x1e4abef1, /// 0x208
			 0x3fd6e5ee, /// 0x20c
			 0xb62b7998, /// 0x210
			 0x25988f15, /// 0x214
			 0xca6271a8, /// 0x218
			 0x02fb447a, /// 0x21c
			 0xa87066f7, /// 0x220
			 0xc59c8f81, /// 0x224
			 0x06a8056f, /// 0x228
			 0x640d6326, /// 0x22c
			 0x3a29254b, /// 0x230
			 0x28610b01, /// 0x234
			 0x0db0d043, /// 0x238
			 0xc52bd863, /// 0x23c
			 0xfcad840e, /// 0x240
			 0x04cab00c, /// 0x244
			 0xa7ce5d2b, /// 0x248
			 0x60dfac90, /// 0x24c
			 0x6171afae, /// 0x250
			 0xda171888, /// 0x254
			 0xfb00a178, /// 0x258
			 0xf0bc4e9d, /// 0x25c
			 0x5cb860b1, /// 0x260
			 0x905b6ae7, /// 0x264
			 0xf3a529fa, /// 0x268
			 0x4b28d247, /// 0x26c
			 0x776ce4d3, /// 0x270
			 0x31f3f826, /// 0x274
			 0x718ae525, /// 0x278
			 0xa52ec721, /// 0x27c
			 0xd03fef8a, /// 0x280
			 0xf1094ad4, /// 0x284
			 0x4b0fe6a6, /// 0x288
			 0x163e8bed, /// 0x28c
			 0x536dff1f, /// 0x290
			 0x09d7c3e8, /// 0x294
			 0xb5a3b429, /// 0x298
			 0x62091e8f, /// 0x29c
			 0xd360e18a, /// 0x2a0
			 0xe2c9cf1c, /// 0x2a4
			 0x00f4309c, /// 0x2a8
			 0xa29c4598, /// 0x2ac
			 0x7ae6cd47, /// 0x2b0
			 0x2c6914cd, /// 0x2b4
			 0xee7224d9, /// 0x2b8
			 0xc3d10b7a, /// 0x2bc
			 0xb12838ac, /// 0x2c0
			 0x55b0fc98, /// 0x2c4
			 0x6f0b7229, /// 0x2c8
			 0x96be53d8, /// 0x2cc
			 0x4d85f120, /// 0x2d0
			 0x8d438fcb, /// 0x2d4
			 0x70e521e1, /// 0x2d8
			 0x9edcde17, /// 0x2dc
			 0x6214e313, /// 0x2e0
			 0x60004691, /// 0x2e4
			 0x80bb4699, /// 0x2e8
			 0xba5f7e59, /// 0x2ec
			 0x987a56d0, /// 0x2f0
			 0x602a4ea1, /// 0x2f4
			 0x2db084ca, /// 0x2f8
			 0x344d27fc, /// 0x2fc
			 0x4399ece7, /// 0x300
			 0x0c79b99c, /// 0x304
			 0xb6022fae, /// 0x308
			 0x4b11086c, /// 0x30c
			 0x58dca6ce, /// 0x310
			 0x42d5ca04, /// 0x314
			 0xea25b24a, /// 0x318
			 0xe4c75553, /// 0x31c
			 0x40e8b55c, /// 0x320
			 0x5012a98f, /// 0x324
			 0xb718f3a7, /// 0x328
			 0xceb0d871, /// 0x32c
			 0xbab61e38, /// 0x330
			 0x05bcb415, /// 0x334
			 0xad948001, /// 0x338
			 0x6394110c, /// 0x33c
			 0x5c46be8d, /// 0x340
			 0x6070d02b, /// 0x344
			 0x7f8704cf, /// 0x348
			 0x059b61f5, /// 0x34c
			 0x8fefc55d, /// 0x350
			 0x742be24b, /// 0x354
			 0xb6a2a644, /// 0x358
			 0x1af1636a, /// 0x35c
			 0x0b16f5c0, /// 0x360
			 0x3c68b809, /// 0x364
			 0xfcf4d3d5, /// 0x368
			 0x0749a065, /// 0x36c
			 0xb075b22d, /// 0x370
			 0x3588347d, /// 0x374
			 0x4d7d2ca4, /// 0x378
			 0x73a9b251, /// 0x37c
			 0x4c2cbcbb, /// 0x380
			 0x1d853833, /// 0x384
			 0x6d12ffb0, /// 0x388
			 0x0e99fe20, /// 0x38c
			 0xf9b9e678, /// 0x390
			 0x25ccec87, /// 0x394
			 0x3bb47aa8, /// 0x398
			 0xf9a804d8, /// 0x39c
			 0x7a77f4c2, /// 0x3a0
			 0x626a915c, /// 0x3a4
			 0x6efb7959, /// 0x3a8
			 0x65e347fd, /// 0x3ac
			 0x87a7c37b, /// 0x3b0
			 0xc722f0d2, /// 0x3b4
			 0x0f54ec4f, /// 0x3b8
			 0x304d6af0, /// 0x3bc
			 0x35e24b16, /// 0x3c0
			 0x48180cd9, /// 0x3c4
			 0xc765f2f5, /// 0x3c8
			 0x0d5a142c, /// 0x3cc
			 0xac4fbb91, /// 0x3d0
			 0x7734bef7, /// 0x3d4
			 0x4cf79ab2, /// 0x3d8
			 0xdf866ea8, /// 0x3dc
			 0xa89f4e5b, /// 0x3e0
			 0xfaf57250, /// 0x3e4
			 0xb4288993, /// 0x3e8
			 0x718d7bb6, /// 0x3ec
			 0x706a59b5, /// 0x3f0
			 0x927df13c, /// 0x3f4
			 0x1e78e8a0, /// 0x3f8
			 0xba852843, /// 0x3fc
			 0x6ac97381, /// 0x400
			 0x9f1d0670, /// 0x404
			 0xa0d5e81e, /// 0x408
			 0x42264998, /// 0x40c
			 0x8fc51068, /// 0x410
			 0x23b30402, /// 0x414
			 0x68775bc0, /// 0x418
			 0x97fb4f9f, /// 0x41c
			 0xb0cb651e, /// 0x420
			 0x634e3e02, /// 0x424
			 0x08da21d8, /// 0x428
			 0xdd046dd3, /// 0x42c
			 0xd6a7bd38, /// 0x430
			 0x363bec76, /// 0x434
			 0x439ddd01, /// 0x438
			 0x1fdb1829, /// 0x43c
			 0xfb8209ad, /// 0x440
			 0xc2b9f721, /// 0x444
			 0x44329609, /// 0x448
			 0x58f8ba0a, /// 0x44c
			 0x7cadf778, /// 0x450
			 0x7fabc562, /// 0x454
			 0x3828cadd, /// 0x458
			 0x1301116c, /// 0x45c
			 0xc4e21518, /// 0x460
			 0x7a43c755, /// 0x464
			 0xc0caa79c, /// 0x468
			 0xb0e3eed9, /// 0x46c
			 0xd52530f6, /// 0x470
			 0x6e2d00be, /// 0x474
			 0xe4f3c6e4, /// 0x478
			 0x2c89228e, /// 0x47c
			 0xc023eda9, /// 0x480
			 0xd6404c6b, /// 0x484
			 0xa84d45ca, /// 0x488
			 0x7244fc9b, /// 0x48c
			 0x6151c88f, /// 0x490
			 0x48f196fe, /// 0x494
			 0x475dca0d, /// 0x498
			 0x78e61c2d, /// 0x49c
			 0x15560241, /// 0x4a0
			 0x85089c6c, /// 0x4a4
			 0x381951f6, /// 0x4a8
			 0xaf12dcc3, /// 0x4ac
			 0xb941cb91, /// 0x4b0
			 0x10c18dc4, /// 0x4b4
			 0x83e56ea4, /// 0x4b8
			 0x4c3cd312, /// 0x4bc
			 0xf1d3579b, /// 0x4c0
			 0xd166d481, /// 0x4c4
			 0x307cd2f5, /// 0x4c8
			 0x9ec937f0, /// 0x4cc
			 0x63d82780, /// 0x4d0
			 0xd0c02c7d, /// 0x4d4
			 0x70bc1e0b, /// 0x4d8
			 0x7985b59c, /// 0x4dc
			 0x4f6f5ae3, /// 0x4e0
			 0xb3d0e6f1, /// 0x4e4
			 0x9092040f, /// 0x4e8
			 0xb4764bcb, /// 0x4ec
			 0x35d5dbe8, /// 0x4f0
			 0x9091856d, /// 0x4f4
			 0x85a3e014, /// 0x4f8
			 0x8e88d34a, /// 0x4fc
			 0x1fa6ae77, /// 0x500
			 0x8e06000e, /// 0x504
			 0x03ee5184, /// 0x508
			 0x909ef6ea, /// 0x50c
			 0x57bc3301, /// 0x510
			 0x75450744, /// 0x514
			 0xd4b918cf, /// 0x518
			 0x442c81e5, /// 0x51c
			 0x94194943, /// 0x520
			 0x5255d5a3, /// 0x524
			 0xec8641bc, /// 0x528
			 0x50e554e3, /// 0x52c
			 0xd1e57427, /// 0x530
			 0x5e08a245, /// 0x534
			 0xac06ea1f, /// 0x538
			 0xd8f2b3b5, /// 0x53c
			 0x6991ee09, /// 0x540
			 0xb2d62765, /// 0x544
			 0x5601788a, /// 0x548
			 0xa61d0905, /// 0x54c
			 0x740074c1, /// 0x550
			 0x65776b4a, /// 0x554
			 0xfdda7330, /// 0x558
			 0x310ac667, /// 0x55c
			 0xddbb1642, /// 0x560
			 0xd2c810c0, /// 0x564
			 0x773aeb6e, /// 0x568
			 0x6bc390b1, /// 0x56c
			 0x6f2268cb, /// 0x570
			 0x11df93bb, /// 0x574
			 0xb0eb0db3, /// 0x578
			 0x7f568153, /// 0x57c
			 0x2b0fa384, /// 0x580
			 0x02e4a976, /// 0x584
			 0x9d302326, /// 0x588
			 0x0dd7bed1, /// 0x58c
			 0x3f8eb861, /// 0x590
			 0x33e40a92, /// 0x594
			 0xedba80b4, /// 0x598
			 0xec9d747f, /// 0x59c
			 0xc1658f21, /// 0x5a0
			 0x93540ade, /// 0x5a4
			 0x84342e30, /// 0x5a8
			 0x5c1e60c4, /// 0x5ac
			 0x9b2713bb, /// 0x5b0
			 0x1129191c, /// 0x5b4
			 0x8b75907b, /// 0x5b8
			 0xbc9e5bfc, /// 0x5bc
			 0x9aa33959, /// 0x5c0
			 0xfb5d8765, /// 0x5c4
			 0xdde39cc4, /// 0x5c8
			 0x59e23bd5, /// 0x5cc
			 0xebfd5936, /// 0x5d0
			 0xed61ae8a, /// 0x5d4
			 0x4e8e2d2f, /// 0x5d8
			 0x2fd21765, /// 0x5dc
			 0x4fecd3b0, /// 0x5e0
			 0xe1dffe53, /// 0x5e4
			 0x4c7262a5, /// 0x5e8
			 0x20740e44, /// 0x5ec
			 0xca8a1bca, /// 0x5f0
			 0x0dffb7b0, /// 0x5f4
			 0xf5b8f86c, /// 0x5f8
			 0x4b76bc2e, /// 0x5fc
			 0xd228c4d0, /// 0x600
			 0x4ff77091, /// 0x604
			 0x7585f498, /// 0x608
			 0x618508c5, /// 0x60c
			 0x29b864f1, /// 0x610
			 0x2c17f659, /// 0x614
			 0x475c4b4c, /// 0x618
			 0x7f1467a7, /// 0x61c
			 0x89bc31bb, /// 0x620
			 0x09db06bd, /// 0x624
			 0xa15c843f, /// 0x628
			 0xcc03d722, /// 0x62c
			 0x8dc51684, /// 0x630
			 0xb15b8f01, /// 0x634
			 0xf229a67f, /// 0x638
			 0x7a5f9a3a, /// 0x63c
			 0xc1dba06e, /// 0x640
			 0xeaf8351e, /// 0x644
			 0x4514c21d, /// 0x648
			 0xc32d5db1, /// 0x64c
			 0xe36104a5, /// 0x650
			 0x6db95194, /// 0x654
			 0x843b41fe, /// 0x658
			 0xc89793ae, /// 0x65c
			 0x0792266e, /// 0x660
			 0xf4b9cf2c, /// 0x664
			 0x432a0610, /// 0x668
			 0x355b2586, /// 0x66c
			 0xb98d8e35, /// 0x670
			 0xb1b5bbc3, /// 0x674
			 0x7fb3a2a8, /// 0x678
			 0x44bf2d36, /// 0x67c
			 0x6743577c, /// 0x680
			 0x411fc6e7, /// 0x684
			 0x027ccb98, /// 0x688
			 0x818548a9, /// 0x68c
			 0x00eeff54, /// 0x690
			 0xb6dc6ca4, /// 0x694
			 0xd56c2fd0, /// 0x698
			 0x1c38cd92, /// 0x69c
			 0x589a245c, /// 0x6a0
			 0x5d332e24, /// 0x6a4
			 0xa9a74b42, /// 0x6a8
			 0x6c82ebc1, /// 0x6ac
			 0x2925b21d, /// 0x6b0
			 0x5b46cd06, /// 0x6b4
			 0xb98d278e, /// 0x6b8
			 0x24fe4712, /// 0x6bc
			 0xfd53c8b1, /// 0x6c0
			 0x34409ac0, /// 0x6c4
			 0xde2dddf9, /// 0x6c8
			 0x5edc30e4, /// 0x6cc
			 0x8b008d24, /// 0x6d0
			 0x02084674, /// 0x6d4
			 0xfea39a0e, /// 0x6d8
			 0x9da20cf9, /// 0x6dc
			 0xcf626b7e, /// 0x6e0
			 0x7ca08fb1, /// 0x6e4
			 0x6db69849, /// 0x6e8
			 0x48b907ab, /// 0x6ec
			 0x79c00f29, /// 0x6f0
			 0x8148c1cd, /// 0x6f4
			 0x4a8ec0bd, /// 0x6f8
			 0xb9a48da6, /// 0x6fc
			 0x007b4669, /// 0x700
			 0x1a750b9b, /// 0x704
			 0xb25ec738, /// 0x708
			 0x2fd4c640, /// 0x70c
			 0xce2dc5f4, /// 0x710
			 0xfb8c3467, /// 0x714
			 0xe9786549, /// 0x718
			 0x6680aa78, /// 0x71c
			 0x6ea6f28e, /// 0x720
			 0x6a6ec7c7, /// 0x724
			 0x052f702a, /// 0x728
			 0x9819de6c, /// 0x72c
			 0x5b34be83, /// 0x730
			 0x6ff73948, /// 0x734
			 0x8ebe1e5f, /// 0x738
			 0x825745db, /// 0x73c
			 0xb96cb8eb, /// 0x740
			 0xf511582d, /// 0x744
			 0x67ccfa38, /// 0x748
			 0xb5f57c24, /// 0x74c
			 0xea18c7c5, /// 0x750
			 0xe8280e87, /// 0x754
			 0x8e2ea3d8, /// 0x758
			 0xc57dff1e, /// 0x75c
			 0x7372f601, /// 0x760
			 0x4c08f2d2, /// 0x764
			 0xb78c1b0e, /// 0x768
			 0x1fc9522c, /// 0x76c
			 0xdfc3cb35, /// 0x770
			 0x87cb3503, /// 0x774
			 0x1010cc72, /// 0x778
			 0x3b060d95, /// 0x77c
			 0x839fb72f, /// 0x780
			 0x9a6289b6, /// 0x784
			 0x52abf91e, /// 0x788
			 0x5fd80245, /// 0x78c
			 0x34727400, /// 0x790
			 0x2f357933, /// 0x794
			 0x4157e4d1, /// 0x798
			 0x871b3961, /// 0x79c
			 0x0833fe34, /// 0x7a0
			 0x2132342f, /// 0x7a4
			 0xbfd9b5ab, /// 0x7a8
			 0xbb36d22b, /// 0x7ac
			 0xfce145d1, /// 0x7b0
			 0x9ce6543a, /// 0x7b4
			 0x25f8c6cf, /// 0x7b8
			 0x773b6f78, /// 0x7bc
			 0xc93bc638, /// 0x7c0
			 0x4efa56e1, /// 0x7c4
			 0x58d6a22c, /// 0x7c8
			 0xc30d6f6a, /// 0x7cc
			 0xfc94bb47, /// 0x7d0
			 0x8eb2d66f, /// 0x7d4
			 0x0b70c368, /// 0x7d8
			 0x82aa9c4b, /// 0x7dc
			 0xa813e5d5, /// 0x7e0
			 0xaa2d1b3e, /// 0x7e4
			 0xed8d2bb1, /// 0x7e8
			 0xadefa079, /// 0x7ec
			 0xefba0ddd, /// 0x7f0
			 0xc2f1340d, /// 0x7f4
			 0xdab37be5, /// 0x7f8
			 0xea8efe4f, /// 0x7fc
			 0x3d993f43, /// 0x800
			 0xfebafa92, /// 0x804
			 0x20bd25c8, /// 0x808
			 0xbc4b355b, /// 0x80c
			 0xd6c1a965, /// 0x810
			 0x0f686bee, /// 0x814
			 0x12a60592, /// 0x818
			 0x8d94510d, /// 0x81c
			 0xd9bb29c0, /// 0x820
			 0x0225d7a1, /// 0x824
			 0x68b59508, /// 0x828
			 0x82eeb7c4, /// 0x82c
			 0x499b3a65, /// 0x830
			 0xeb8ce676, /// 0x834
			 0xaab6c507, /// 0x838
			 0xe63893b0, /// 0x83c
			 0xc5e99808, /// 0x840
			 0xe904de40, /// 0x844
			 0xc2517238, /// 0x848
			 0xc0ac4335, /// 0x84c
			 0xeefee5cd, /// 0x850
			 0xe1a980f3, /// 0x854
			 0x40266971, /// 0x858
			 0x66670235, /// 0x85c
			 0xd1c672aa, /// 0x860
			 0x0bdf311b, /// 0x864
			 0x3687cf44, /// 0x868
			 0x76c9c669, /// 0x86c
			 0xe2acd27f, /// 0x870
			 0x4c0d035a, /// 0x874
			 0x782032e9, /// 0x878
			 0x0a830c8d, /// 0x87c
			 0xf814443a, /// 0x880
			 0x79c70ca0, /// 0x884
			 0x981439a5, /// 0x888
			 0x207966ff, /// 0x88c
			 0x02b08356, /// 0x890
			 0x6e0d104f, /// 0x894
			 0x760d2d72, /// 0x898
			 0xac51ad9a, /// 0x89c
			 0x5bc9da9c, /// 0x8a0
			 0xbf151437, /// 0x8a4
			 0x6dd2f846, /// 0x8a8
			 0xa115d6ed, /// 0x8ac
			 0x4760778d, /// 0x8b0
			 0x822a80ff, /// 0x8b4
			 0x92ad59f9, /// 0x8b8
			 0xd70f3e47, /// 0x8bc
			 0xd6ebf7b7, /// 0x8c0
			 0xa0714a13, /// 0x8c4
			 0x1e1c2a15, /// 0x8c8
			 0x736ca1fd, /// 0x8cc
			 0x51c91d47, /// 0x8d0
			 0xa82d512d, /// 0x8d4
			 0x2526e5dc, /// 0x8d8
			 0xb66105d0, /// 0x8dc
			 0x49517a35, /// 0x8e0
			 0x2391f8c9, /// 0x8e4
			 0x1ecd698f, /// 0x8e8
			 0x06f9a5e8, /// 0x8ec
			 0x4abcd929, /// 0x8f0
			 0x74322778, /// 0x8f4
			 0xdf1d68ad, /// 0x8f8
			 0xfe5e3e02, /// 0x8fc
			 0x2a4e179b, /// 0x900
			 0x511cf7ce, /// 0x904
			 0xdddd18bb, /// 0x908
			 0xc9ec45c9, /// 0x90c
			 0x42aaa07a, /// 0x910
			 0x1084c70d, /// 0x914
			 0x278aa859, /// 0x918
			 0x3d3b2018, /// 0x91c
			 0xc924545e, /// 0x920
			 0xc857e8a8, /// 0x924
			 0xb96f5918, /// 0x928
			 0x38fbb7ae, /// 0x92c
			 0xd168fc70, /// 0x930
			 0xbce68a5e, /// 0x934
			 0x45c3e9f4, /// 0x938
			 0xbf8a8800, /// 0x93c
			 0xd221a600, /// 0x940
			 0xfe092bff, /// 0x944
			 0x455121fc, /// 0x948
			 0x6e9d256c, /// 0x94c
			 0x532f7527, /// 0x950
			 0x3465ae1f, /// 0x954
			 0xbfe81797, /// 0x958
			 0x78e5ee28, /// 0x95c
			 0xf3e5bdf7, /// 0x960
			 0xca1f4e02, /// 0x964
			 0x96496318, /// 0x968
			 0x582960e0, /// 0x96c
			 0xf790a61b, /// 0x970
			 0x851bfbbf, /// 0x974
			 0xff139ba8, /// 0x978
			 0x79d2a3b8, /// 0x97c
			 0x32baeb0d, /// 0x980
			 0xaece26c9, /// 0x984
			 0xefe14668, /// 0x988
			 0xc626a48b, /// 0x98c
			 0xceefefea, /// 0x990
			 0x5309576e, /// 0x994
			 0x4c1c37d7, /// 0x998
			 0xbf040529, /// 0x99c
			 0x49f05f49, /// 0x9a0
			 0xe7da5b54, /// 0x9a4
			 0x6c4edc5e, /// 0x9a8
			 0x1f4bc54e, /// 0x9ac
			 0x65f7b831, /// 0x9b0
			 0x6f349c6b, /// 0x9b4
			 0xd72ec938, /// 0x9b8
			 0x981c8ce6, /// 0x9bc
			 0x324d2538, /// 0x9c0
			 0x5621d12d, /// 0x9c4
			 0x29e6821f, /// 0x9c8
			 0x149df24b, /// 0x9cc
			 0x69a54aa6, /// 0x9d0
			 0x6d295ebf, /// 0x9d4
			 0xe55d1081, /// 0x9d8
			 0x94ad16fd, /// 0x9dc
			 0xcfb66cf5, /// 0x9e0
			 0x1763d814, /// 0x9e4
			 0x945b01ba, /// 0x9e8
			 0x723d11ab, /// 0x9ec
			 0x7ac21aa4, /// 0x9f0
			 0x1c9a2e6b, /// 0x9f4
			 0x8f922491, /// 0x9f8
			 0x5d9d9c66, /// 0x9fc
			 0x17bb8b2d, /// 0xa00
			 0xf78190d4, /// 0xa04
			 0x3cce9be1, /// 0xa08
			 0x77b84802, /// 0xa0c
			 0xf6a890ff, /// 0xa10
			 0xc482d1e2, /// 0xa14
			 0xbbfccc41, /// 0xa18
			 0xff74302a, /// 0xa1c
			 0x102a7bb0, /// 0xa20
			 0x9765ae32, /// 0xa24
			 0x01097a2d, /// 0xa28
			 0xd3219e21, /// 0xa2c
			 0xbeb977b1, /// 0xa30
			 0xac77839c, /// 0xa34
			 0x5f0c3d53, /// 0xa38
			 0xd7608017, /// 0xa3c
			 0x9607c53b, /// 0xa40
			 0xad657887, /// 0xa44
			 0x83a750e0, /// 0xa48
			 0x41c1fa47, /// 0xa4c
			 0x14958270, /// 0xa50
			 0x5e659da6, /// 0xa54
			 0x90b700f3, /// 0xa58
			 0xb1cd2f59, /// 0xa5c
			 0x0fba3b09, /// 0xa60
			 0xbe13a0f4, /// 0xa64
			 0x449692e4, /// 0xa68
			 0xb103ff1b, /// 0xa6c
			 0x3e1a5f3b, /// 0xa70
			 0x7a8d3df1, /// 0xa74
			 0x65d504ba, /// 0xa78
			 0xec3220ac, /// 0xa7c
			 0x449a6809, /// 0xa80
			 0x3143ca78, /// 0xa84
			 0xa02cebf4, /// 0xa88
			 0x3ad73bd0, /// 0xa8c
			 0xeb322e2e, /// 0xa90
			 0xe9e3c3c8, /// 0xa94
			 0x24da5f54, /// 0xa98
			 0x04e4d310, /// 0xa9c
			 0x31dddb85, /// 0xaa0
			 0x6519da12, /// 0xaa4
			 0x6b7cc971, /// 0xaa8
			 0xc9c10f12, /// 0xaac
			 0x8d35405e, /// 0xab0
			 0x37097249, /// 0xab4
			 0x6ef798e0, /// 0xab8
			 0x377b34ee, /// 0xabc
			 0x738b2e98, /// 0xac0
			 0x854cfebb, /// 0xac4
			 0xa4318f3d, /// 0xac8
			 0xa1053651, /// 0xacc
			 0x28f0004e, /// 0xad0
			 0xd3376183, /// 0xad4
			 0x4a39eab5, /// 0xad8
			 0xdc46a057, /// 0xadc
			 0x551f6717, /// 0xae0
			 0x367dbc98, /// 0xae4
			 0xec26057e, /// 0xae8
			 0xcb8ba03b, /// 0xaec
			 0xdf657d98, /// 0xaf0
			 0xd8005241, /// 0xaf4
			 0x16221fd9, /// 0xaf8
			 0x2fede309, /// 0xafc
			 0x8d8f1538, /// 0xb00
			 0x59fab7eb, /// 0xb04
			 0xc1923bc7, /// 0xb08
			 0x62c8ed41, /// 0xb0c
			 0x18af3d09, /// 0xb10
			 0x008a18b1, /// 0xb14
			 0xd2b4bff5, /// 0xb18
			 0x52fb8518, /// 0xb1c
			 0x82a015b2, /// 0xb20
			 0x530b7f5a, /// 0xb24
			 0x2901fb03, /// 0xb28
			 0x573f3482, /// 0xb2c
			 0xe645d27a, /// 0xb30
			 0x4c5c0647, /// 0xb34
			 0x9528107c, /// 0xb38
			 0x46b7edef, /// 0xb3c
			 0x7df18fa2, /// 0xb40
			 0x340102a1, /// 0xb44
			 0x7af54473, /// 0xb48
			 0xd005311d, /// 0xb4c
			 0xc9bc5b01, /// 0xb50
			 0x17eb388f, /// 0xb54
			 0xa73118cb, /// 0xb58
			 0x9aa49d96, /// 0xb5c
			 0xb7c5712e, /// 0xb60
			 0x4d3e05c1, /// 0xb64
			 0x26032985, /// 0xb68
			 0xe96ef6f0, /// 0xb6c
			 0xe44dce89, /// 0xb70
			 0x12150d4f, /// 0xb74
			 0xd419f4c0, /// 0xb78
			 0xef38041e, /// 0xb7c
			 0x4d7663a1, /// 0xb80
			 0x0be83d77, /// 0xb84
			 0xc5452f7a, /// 0xb88
			 0xa1c46110, /// 0xb8c
			 0x391c39f8, /// 0xb90
			 0x77b926c8, /// 0xb94
			 0xd094f19b, /// 0xb98
			 0x347b7beb, /// 0xb9c
			 0xb3f02e37, /// 0xba0
			 0xfabe1845, /// 0xba4
			 0x6ac8725b, /// 0xba8
			 0x5212851f, /// 0xbac
			 0xe61a44e7, /// 0xbb0
			 0x31bf3862, /// 0xbb4
			 0x93930e97, /// 0xbb8
			 0x88e46899, /// 0xbbc
			 0x4af2eb0b, /// 0xbc0
			 0xa0b6ab53, /// 0xbc4
			 0xfccdde0c, /// 0xbc8
			 0x3b288f68, /// 0xbcc
			 0x03fd8f62, /// 0xbd0
			 0x14beae4c, /// 0xbd4
			 0x17f1c7d6, /// 0xbd8
			 0x38196e77, /// 0xbdc
			 0x55519bb2, /// 0xbe0
			 0x172afed2, /// 0xbe4
			 0x451089b1, /// 0xbe8
			 0xe77ddd43, /// 0xbec
			 0x79ef9ca9, /// 0xbf0
			 0xb5d15634, /// 0xbf4
			 0x1312ce16, /// 0xbf8
			 0xf7a5f771, /// 0xbfc
			 0x518d8512, /// 0xc00
			 0xd489abf2, /// 0xc04
			 0xcb0b2335, /// 0xc08
			 0x398da0cc, /// 0xc0c
			 0x60808fd7, /// 0xc10
			 0xe97915ab, /// 0xc14
			 0x743c63e3, /// 0xc18
			 0xaae19bcf, /// 0xc1c
			 0xd8228134, /// 0xc20
			 0x23aec276, /// 0xc24
			 0x19812c08, /// 0xc28
			 0xc7f30b83, /// 0xc2c
			 0x79533afe, /// 0xc30
			 0xda0ef1a6, /// 0xc34
			 0x7b667e2b, /// 0xc38
			 0xcbdae86a, /// 0xc3c
			 0x120e235e, /// 0xc40
			 0xaf32b950, /// 0xc44
			 0x3104df0b, /// 0xc48
			 0x509e036c, /// 0xc4c
			 0x95832d38, /// 0xc50
			 0x9ec72b49, /// 0xc54
			 0x9354e7e4, /// 0xc58
			 0x0927be1a, /// 0xc5c
			 0xd1cd45c5, /// 0xc60
			 0x2cf4ee5e, /// 0xc64
			 0xcb3b6ef7, /// 0xc68
			 0xdc92c0dc, /// 0xc6c
			 0x9e8a26ce, /// 0xc70
			 0x102e8653, /// 0xc74
			 0x082a3ecd, /// 0xc78
			 0xf11c9bad, /// 0xc7c
			 0xbd1f501c, /// 0xc80
			 0x231db93e, /// 0xc84
			 0xea006a23, /// 0xc88
			 0xd956c1da, /// 0xc8c
			 0xcb8d6d8b, /// 0xc90
			 0x0f480993, /// 0xc94
			 0xd271ef83, /// 0xc98
			 0x5af909d8, /// 0xc9c
			 0x2131ffc7, /// 0xca0
			 0xe65b79a7, /// 0xca4
			 0x90b50d9b, /// 0xca8
			 0xeed00ee5, /// 0xcac
			 0x6d33cf3b, /// 0xcb0
			 0xe93d7944, /// 0xcb4
			 0x2e0fdf69, /// 0xcb8
			 0x29b0fdff, /// 0xcbc
			 0xcaeccc0d, /// 0xcc0
			 0x5e3fb5ff, /// 0xcc4
			 0x860d0e8b, /// 0xcc8
			 0x59f2aef9, /// 0xccc
			 0x3d43cae5, /// 0xcd0
			 0xd455e441, /// 0xcd4
			 0x732d7a4e, /// 0xcd8
			 0xf88f29ab, /// 0xcdc
			 0xd88de17b, /// 0xce0
			 0xcce20efd, /// 0xce4
			 0xd1c4fdef, /// 0xce8
			 0x87260fa1, /// 0xcec
			 0xff4af2d6, /// 0xcf0
			 0x5bc4c775, /// 0xcf4
			 0x1485396a, /// 0xcf8
			 0xfaeae305, /// 0xcfc
			 0x8cc57313, /// 0xd00
			 0x8e71352f, /// 0xd04
			 0x184b7758, /// 0xd08
			 0xcef5fded, /// 0xd0c
			 0xfa7ccec1, /// 0xd10
			 0xd43e03e5, /// 0xd14
			 0x9e58e858, /// 0xd18
			 0x93718884, /// 0xd1c
			 0xc7fc40c0, /// 0xd20
			 0x202a99bd, /// 0xd24
			 0x41f01305, /// 0xd28
			 0x279acda0, /// 0xd2c
			 0x39463da3, /// 0xd30
			 0xa092674b, /// 0xd34
			 0xfd2ae950, /// 0xd38
			 0x47c5de90, /// 0xd3c
			 0x7ae7a41f, /// 0xd40
			 0x1d9ce23c, /// 0xd44
			 0xf128cc5c, /// 0xd48
			 0x06dfb9fa, /// 0xd4c
			 0x1f0c53ef, /// 0xd50
			 0xe3b0f148, /// 0xd54
			 0x32ec7497, /// 0xd58
			 0xb978eb0b, /// 0xd5c
			 0x01e38033, /// 0xd60
			 0x85bbca67, /// 0xd64
			 0x7344de34, /// 0xd68
			 0x55d8d0e0, /// 0xd6c
			 0x1a1d61ea, /// 0xd70
			 0xbb716346, /// 0xd74
			 0x4cf887ae, /// 0xd78
			 0xb1a527de, /// 0xd7c
			 0xf5b2bf81, /// 0xd80
			 0x3cf4d7f1, /// 0xd84
			 0xe3032642, /// 0xd88
			 0x9685269f, /// 0xd8c
			 0xf786d41a, /// 0xd90
			 0x3bb6cc1c, /// 0xd94
			 0x00cc21d4, /// 0xd98
			 0x4f6db9db, /// 0xd9c
			 0xadc49415, /// 0xda0
			 0xa7c6a68c, /// 0xda4
			 0xa3fb5183, /// 0xda8
			 0xd1a31636, /// 0xdac
			 0xbff69ee5, /// 0xdb0
			 0x6bb5397a, /// 0xdb4
			 0xd2d907e3, /// 0xdb8
			 0xc4b0e9cc, /// 0xdbc
			 0xdeee5e74, /// 0xdc0
			 0x24c8758b, /// 0xdc4
			 0x0b8452ba, /// 0xdc8
			 0x5f2ca38e, /// 0xdcc
			 0x9a5cff46, /// 0xdd0
			 0x22b4f8be, /// 0xdd4
			 0xcd48c89f, /// 0xdd8
			 0xdfcdedb6, /// 0xddc
			 0x81d980b8, /// 0xde0
			 0x758a72ab, /// 0xde4
			 0x02944efd, /// 0xde8
			 0x43995643, /// 0xdec
			 0x17ce6e28, /// 0xdf0
			 0xc783a8a2, /// 0xdf4
			 0x4cb28848, /// 0xdf8
			 0xb728f1b9, /// 0xdfc
			 0xaad6e069, /// 0xe00
			 0x0a72526a, /// 0xe04
			 0x4f474ea5, /// 0xe08
			 0xdc6e68f0, /// 0xe0c
			 0x0f1cd54a, /// 0xe10
			 0xb31ce066, /// 0xe14
			 0x80a5f7d9, /// 0xe18
			 0x806e43d5, /// 0xe1c
			 0xb652ace3, /// 0xe20
			 0x98454954, /// 0xe24
			 0x49fb1bad, /// 0xe28
			 0xaec18380, /// 0xe2c
			 0xa182c8f2, /// 0xe30
			 0x05d04aff, /// 0xe34
			 0x374eb30e, /// 0xe38
			 0x23c30936, /// 0xe3c
			 0x1d055b0a, /// 0xe40
			 0x5e2b9722, /// 0xe44
			 0x3687dd9c, /// 0xe48
			 0x052a5ffe, /// 0xe4c
			 0x4082b253, /// 0xe50
			 0xc522928d, /// 0xe54
			 0xb603f137, /// 0xe58
			 0x9f1714a9, /// 0xe5c
			 0xd6b3a302, /// 0xe60
			 0xaab7556f, /// 0xe64
			 0x67189e23, /// 0xe68
			 0xd45c4811, /// 0xe6c
			 0xbb00a039, /// 0xe70
			 0x227a6a6f, /// 0xe74
			 0xb90c2357, /// 0xe78
			 0x561943dd, /// 0xe7c
			 0xedfd3a44, /// 0xe80
			 0xf438d2ab, /// 0xe84
			 0x318c58c5, /// 0xe88
			 0x23d58efa, /// 0xe8c
			 0xdbf266ef, /// 0xe90
			 0xf5ef7fbd, /// 0xe94
			 0xdbc76107, /// 0xe98
			 0x92663c99, /// 0xe9c
			 0xa17b562f, /// 0xea0
			 0xd95ae34a, /// 0xea4
			 0x9cba8251, /// 0xea8
			 0xa8b50c09, /// 0xeac
			 0xd62277be, /// 0xeb0
			 0xa243bc67, /// 0xeb4
			 0x3fff4034, /// 0xeb8
			 0x17a8984c, /// 0xebc
			 0xcabb6f1e, /// 0xec0
			 0xd255b7ba, /// 0xec4
			 0xfe72b00f, /// 0xec8
			 0x5ad0d210, /// 0xecc
			 0x1aace351, /// 0xed0
			 0xae434f09, /// 0xed4
			 0x03d9d001, /// 0xed8
			 0xf324880c, /// 0xedc
			 0xc8dbb339, /// 0xee0
			 0x4bdf9dd2, /// 0xee4
			 0xb1d22a74, /// 0xee8
			 0x24e5564e, /// 0xeec
			 0x4c3403bd, /// 0xef0
			 0x6bfffaf0, /// 0xef4
			 0x9e499b74, /// 0xef8
			 0xff78be50, /// 0xefc
			 0xd5c7c935, /// 0xf00
			 0x4d122f32, /// 0xf04
			 0xd08a48d7, /// 0xf08
			 0xdd1c1eb8, /// 0xf0c
			 0xf3c3422e, /// 0xf10
			 0x65076d97, /// 0xf14
			 0x9e1dc961, /// 0xf18
			 0x4bf91706, /// 0xf1c
			 0x063d801c, /// 0xf20
			 0x04c49ece, /// 0xf24
			 0x7f2d751c, /// 0xf28
			 0x7943af4d, /// 0xf2c
			 0xa6940498, /// 0xf30
			 0xaa3e6816, /// 0xf34
			 0xbb7bc7d4, /// 0xf38
			 0x662ea8f1, /// 0xf3c
			 0x83e2c3b5, /// 0xf40
			 0x515c9f08, /// 0xf44
			 0x7a059a75, /// 0xf48
			 0x905cb09d, /// 0xf4c
			 0xd35ae894, /// 0xf50
			 0x65493d9d, /// 0xf54
			 0xa3d04248, /// 0xf58
			 0x71e5ac6f, /// 0xf5c
			 0xc00576f3, /// 0xf60
			 0x13c529db, /// 0xf64
			 0xe1396e8e, /// 0xf68
			 0xf1a8975f, /// 0xf6c
			 0x663ef6cf, /// 0xf70
			 0xc79c9076, /// 0xf74
			 0x01b6877a, /// 0xf78
			 0x96a28226, /// 0xf7c
			 0x405d9ddf, /// 0xf80
			 0x91bcc537, /// 0xf84
			 0x61af890f, /// 0xf88
			 0x24d7eaee, /// 0xf8c
			 0x65bdad63, /// 0xf90
			 0x054bc3b9, /// 0xf94
			 0x29507d58, /// 0xf98
			 0xe555266b, /// 0xf9c
			 0x445ce93f, /// 0xfa0
			 0xa5ba16db, /// 0xfa4
			 0xb9bdad40, /// 0xfa8
			 0x32665787, /// 0xfac
			 0xbefc9bc6, /// 0xfb0
			 0x916d1756, /// 0xfb4
			 0xba4eba08, /// 0xfb8
			 0x4127f25b, /// 0xfbc
			 0x52492c72, /// 0xfc0
			 0x604a722d, /// 0xfc4
			 0x9f09f291, /// 0xfc8
			 0x0500b9d2, /// 0xfcc
			 0x07e63239, /// 0xfd0
			 0x20438d18, /// 0xfd4
			 0xcbe50105, /// 0xfd8
			 0x8c7c1371, /// 0xfdc
			 0x07ed46ed, /// 0xfe0
			 0xb6cbcab8, /// 0xfe4
			 0x54cd1f90, /// 0xfe8
			 0xcad6e102, /// 0xfec
			 0x4a828d7b, /// 0xff0
			 0x7846283b, /// 0xff4
			 0xe52b0cc2, /// 0xff8
			 0x50eed7db /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x44a577b0, /// 0x0
			 0xac00e2f2, /// 0x4
			 0xf6a7ecee, /// 0x8
			 0xfcfbfa2c, /// 0xc
			 0xab8a20c2, /// 0x10
			 0x3df6e858, /// 0x14
			 0xb641a45d, /// 0x18
			 0xdb84d220, /// 0x1c
			 0xc5d78a4d, /// 0x20
			 0x31480b85, /// 0x24
			 0xc769ac7a, /// 0x28
			 0x6250b905, /// 0x2c
			 0x55e19341, /// 0x30
			 0xddf68d5b, /// 0x34
			 0xd53dc10e, /// 0x38
			 0x684b7379, /// 0x3c
			 0xb89b4a5c, /// 0x40
			 0xe509b264, /// 0x44
			 0x48c7b417, /// 0x48
			 0xa501adfb, /// 0x4c
			 0x78480e8e, /// 0x50
			 0xbddf86d8, /// 0x54
			 0x16d062c2, /// 0x58
			 0x00eb96f1, /// 0x5c
			 0x3842d566, /// 0x60
			 0xa18a6f87, /// 0x64
			 0xbaf1dcef, /// 0x68
			 0x57dc73cb, /// 0x6c
			 0x3c268281, /// 0x70
			 0x0ba96bec, /// 0x74
			 0xbabaaf4f, /// 0x78
			 0xb6e0a21d, /// 0x7c
			 0x631974d8, /// 0x80
			 0x868d5127, /// 0x84
			 0x04943202, /// 0x88
			 0x510f3015, /// 0x8c
			 0x1fd4b5df, /// 0x90
			 0x61af2760, /// 0x94
			 0x01977285, /// 0x98
			 0xe47c5629, /// 0x9c
			 0xd539b782, /// 0xa0
			 0xdba14ad9, /// 0xa4
			 0x8add0ff6, /// 0xa8
			 0xc8b2a0e9, /// 0xac
			 0x14dca7ab, /// 0xb0
			 0x0b9251ff, /// 0xb4
			 0xa9db7b89, /// 0xb8
			 0x17cd9d40, /// 0xbc
			 0xe2400894, /// 0xc0
			 0xed04e359, /// 0xc4
			 0x3e3c2e71, /// 0xc8
			 0x37450539, /// 0xcc
			 0x4b131662, /// 0xd0
			 0xc988292f, /// 0xd4
			 0x5df9cd83, /// 0xd8
			 0x875b0b5b, /// 0xdc
			 0x3c9a4d90, /// 0xe0
			 0x8feab203, /// 0xe4
			 0x343fb62c, /// 0xe8
			 0x9bcd5bf6, /// 0xec
			 0x9aeee788, /// 0xf0
			 0x56eac5bf, /// 0xf4
			 0x9eb4c169, /// 0xf8
			 0x936d73ef, /// 0xfc
			 0xa1b5b372, /// 0x100
			 0x43a41303, /// 0x104
			 0x28a8d78c, /// 0x108
			 0x2314ca95, /// 0x10c
			 0x443c11af, /// 0x110
			 0x8f12a150, /// 0x114
			 0xd5e0693e, /// 0x118
			 0xda3a33db, /// 0x11c
			 0x3f208f4a, /// 0x120
			 0x66ade1a3, /// 0x124
			 0x550eb491, /// 0x128
			 0xff1dcb21, /// 0x12c
			 0x5a4cfd17, /// 0x130
			 0x21dbd1c9, /// 0x134
			 0xf247837c, /// 0x138
			 0x2cfc2512, /// 0x13c
			 0xb345c6b7, /// 0x140
			 0x7e69d714, /// 0x144
			 0x622e4f8f, /// 0x148
			 0xe02e27a3, /// 0x14c
			 0x378f6c7d, /// 0x150
			 0x7d2d4b1f, /// 0x154
			 0x58e2533c, /// 0x158
			 0xdcf78fc0, /// 0x15c
			 0x6638974a, /// 0x160
			 0xf4469d95, /// 0x164
			 0x6302178e, /// 0x168
			 0x9b82634f, /// 0x16c
			 0x1b071bd8, /// 0x170
			 0x60e4af57, /// 0x174
			 0x75a99992, /// 0x178
			 0xc9a9fd0f, /// 0x17c
			 0x3052d8f8, /// 0x180
			 0xa539e22a, /// 0x184
			 0xa42a27fb, /// 0x188
			 0xe957bcaa, /// 0x18c
			 0x18e6486d, /// 0x190
			 0x2706c3e4, /// 0x194
			 0x420ea6f0, /// 0x198
			 0x9c47ec45, /// 0x19c
			 0x2fc5e73b, /// 0x1a0
			 0x0655e32a, /// 0x1a4
			 0xe7543ec1, /// 0x1a8
			 0x1a2ce834, /// 0x1ac
			 0x678bf075, /// 0x1b0
			 0x3d5ec669, /// 0x1b4
			 0x99a45189, /// 0x1b8
			 0x0ed48ecc, /// 0x1bc
			 0x3fc980bb, /// 0x1c0
			 0x841a5f17, /// 0x1c4
			 0x746b5a9b, /// 0x1c8
			 0xd787beb2, /// 0x1cc
			 0xf780bf73, /// 0x1d0
			 0x385def5d, /// 0x1d4
			 0x643cb2aa, /// 0x1d8
			 0x5f6865ef, /// 0x1dc
			 0x6757c489, /// 0x1e0
			 0x4de5c962, /// 0x1e4
			 0x4c285de5, /// 0x1e8
			 0x8e819567, /// 0x1ec
			 0xd48b6eb6, /// 0x1f0
			 0x7de0bf90, /// 0x1f4
			 0x3f82b2a0, /// 0x1f8
			 0x3b172798, /// 0x1fc
			 0xace5ccb2, /// 0x200
			 0x07b72127, /// 0x204
			 0xbbe672bb, /// 0x208
			 0x6f473c7c, /// 0x20c
			 0xe2acb255, /// 0x210
			 0x2b659873, /// 0x214
			 0x27c49f89, /// 0x218
			 0xe5d5cf76, /// 0x21c
			 0x3d516400, /// 0x220
			 0x695406e8, /// 0x224
			 0x23062192, /// 0x228
			 0x33595b42, /// 0x22c
			 0x10c733e3, /// 0x230
			 0xa30a0335, /// 0x234
			 0x4f98ae6d, /// 0x238
			 0x570b3145, /// 0x23c
			 0xb1e883c2, /// 0x240
			 0xf57d2f72, /// 0x244
			 0x2f1ba79a, /// 0x248
			 0x50ee2ad9, /// 0x24c
			 0x7fdda74b, /// 0x250
			 0x2e7130fe, /// 0x254
			 0x42bd15dd, /// 0x258
			 0x832a8178, /// 0x25c
			 0x482ec960, /// 0x260
			 0xf6bb33b7, /// 0x264
			 0xe029297e, /// 0x268
			 0xe3433238, /// 0x26c
			 0x074ac850, /// 0x270
			 0x75b4cfd2, /// 0x274
			 0x8c35a397, /// 0x278
			 0xbf391628, /// 0x27c
			 0x40f79acd, /// 0x280
			 0x1f9ec166, /// 0x284
			 0x45a9683e, /// 0x288
			 0x38e9cae8, /// 0x28c
			 0x26fe53d7, /// 0x290
			 0xee86314b, /// 0x294
			 0x0b879770, /// 0x298
			 0xbec9195f, /// 0x29c
			 0x17ced867, /// 0x2a0
			 0xb21d4fae, /// 0x2a4
			 0x6d634a40, /// 0x2a8
			 0xaebd4130, /// 0x2ac
			 0xd9a23e75, /// 0x2b0
			 0xff81c936, /// 0x2b4
			 0x95dd8e0a, /// 0x2b8
			 0xe5635755, /// 0x2bc
			 0x8712adac, /// 0x2c0
			 0x3774f5d8, /// 0x2c4
			 0xc7e6c80b, /// 0x2c8
			 0x06c2e978, /// 0x2cc
			 0xf90b104d, /// 0x2d0
			 0xf2f7cf59, /// 0x2d4
			 0x1c9ba515, /// 0x2d8
			 0x1dbba2f3, /// 0x2dc
			 0xf64bab3c, /// 0x2e0
			 0xd9e4acc5, /// 0x2e4
			 0x6c6bf641, /// 0x2e8
			 0x6525f733, /// 0x2ec
			 0xf0bd54b0, /// 0x2f0
			 0x2a63fe85, /// 0x2f4
			 0x17c35613, /// 0x2f8
			 0x905210bd, /// 0x2fc
			 0xb1756b31, /// 0x300
			 0x0ff5d040, /// 0x304
			 0x4262e066, /// 0x308
			 0x29a88086, /// 0x30c
			 0xfde2483f, /// 0x310
			 0xd9e0397d, /// 0x314
			 0x364b4457, /// 0x318
			 0x3735be0b, /// 0x31c
			 0xc1a236df, /// 0x320
			 0x3824ef6c, /// 0x324
			 0x8f3bc80a, /// 0x328
			 0x9b6fd170, /// 0x32c
			 0x6b129234, /// 0x330
			 0xb1e2ea2c, /// 0x334
			 0x383f9cd8, /// 0x338
			 0x2adaaef1, /// 0x33c
			 0x6856805f, /// 0x340
			 0x554e43fb, /// 0x344
			 0x7027d1ae, /// 0x348
			 0xec65b894, /// 0x34c
			 0xce0c6ae3, /// 0x350
			 0x806652e2, /// 0x354
			 0x6722e3a0, /// 0x358
			 0x2080c3d5, /// 0x35c
			 0x8c75e7bf, /// 0x360
			 0xe1b42c85, /// 0x364
			 0xecf250a5, /// 0x368
			 0x231fe98d, /// 0x36c
			 0x7c99beba, /// 0x370
			 0xa4ea65b0, /// 0x374
			 0x40c0465e, /// 0x378
			 0x1a08a7a9, /// 0x37c
			 0xd6aef078, /// 0x380
			 0x7f64765e, /// 0x384
			 0x669fab34, /// 0x388
			 0xe822d85d, /// 0x38c
			 0x6cd94a4e, /// 0x390
			 0xb83ec6ad, /// 0x394
			 0x58ab97b1, /// 0x398
			 0x1b9979bd, /// 0x39c
			 0xb54d1845, /// 0x3a0
			 0xbd4136ce, /// 0x3a4
			 0xb4173ac3, /// 0x3a8
			 0xe903ae3b, /// 0x3ac
			 0xda15904d, /// 0x3b0
			 0x3ec48f49, /// 0x3b4
			 0x6ac9b47d, /// 0x3b8
			 0x2886a4bd, /// 0x3bc
			 0xa976f0fb, /// 0x3c0
			 0x07882ecc, /// 0x3c4
			 0xad40148c, /// 0x3c8
			 0x5540ddb8, /// 0x3cc
			 0x008e3d47, /// 0x3d0
			 0x809e1e7b, /// 0x3d4
			 0xe0591f03, /// 0x3d8
			 0xab19b65d, /// 0x3dc
			 0xbdda1d2e, /// 0x3e0
			 0xd95b45cf, /// 0x3e4
			 0x758cea19, /// 0x3e8
			 0xf00470d8, /// 0x3ec
			 0x5c7e3abe, /// 0x3f0
			 0x34e1ca62, /// 0x3f4
			 0x0be1df96, /// 0x3f8
			 0x4d80430c, /// 0x3fc
			 0x6b872ab0, /// 0x400
			 0x463266df, /// 0x404
			 0xb3f6cb96, /// 0x408
			 0x28d9b21b, /// 0x40c
			 0x50065f28, /// 0x410
			 0xb606f017, /// 0x414
			 0x5eb2976f, /// 0x418
			 0x1355d0dd, /// 0x41c
			 0x9941c34c, /// 0x420
			 0x0d32f8d8, /// 0x424
			 0x9229216d, /// 0x428
			 0x28a2721c, /// 0x42c
			 0x415b0b51, /// 0x430
			 0x2b47d0c7, /// 0x434
			 0x7ce128a5, /// 0x438
			 0xc261cc04, /// 0x43c
			 0xea19d035, /// 0x440
			 0xd6c945aa, /// 0x444
			 0x385d09a3, /// 0x448
			 0x44ea263c, /// 0x44c
			 0xdd0f4c1d, /// 0x450
			 0x616595f4, /// 0x454
			 0x4a41ca0d, /// 0x458
			 0xacdf17f3, /// 0x45c
			 0x7595d183, /// 0x460
			 0xd1ea15a0, /// 0x464
			 0x0f106111, /// 0x468
			 0xfd327f19, /// 0x46c
			 0x71a455b0, /// 0x470
			 0x3b538996, /// 0x474
			 0x25ef2c1c, /// 0x478
			 0xb3b90d9a, /// 0x47c
			 0x7c24f788, /// 0x480
			 0x9053aed2, /// 0x484
			 0xff8447fa, /// 0x488
			 0x836115a0, /// 0x48c
			 0x6053f7fd, /// 0x490
			 0xc6e4c5ff, /// 0x494
			 0x19d52632, /// 0x498
			 0x60ac4f89, /// 0x49c
			 0xc89d61eb, /// 0x4a0
			 0xd5921963, /// 0x4a4
			 0x2c3a31e9, /// 0x4a8
			 0xdbdaa7ee, /// 0x4ac
			 0x0bd1173f, /// 0x4b0
			 0x4d5c0d95, /// 0x4b4
			 0xd0edcd9c, /// 0x4b8
			 0xf0421110, /// 0x4bc
			 0x4230e5fb, /// 0x4c0
			 0xf6c5e5d2, /// 0x4c4
			 0x66655b86, /// 0x4c8
			 0xdd7f1fee, /// 0x4cc
			 0xdab66eda, /// 0x4d0
			 0xc2aecf25, /// 0x4d4
			 0xe8862194, /// 0x4d8
			 0x1ff53e31, /// 0x4dc
			 0xf0b8d595, /// 0x4e0
			 0xffc63289, /// 0x4e4
			 0x62e873e9, /// 0x4e8
			 0x978d514b, /// 0x4ec
			 0x49be1c8c, /// 0x4f0
			 0xd85bf359, /// 0x4f4
			 0x5b43eb86, /// 0x4f8
			 0xb6e740f5, /// 0x4fc
			 0xb0af3dde, /// 0x500
			 0x6ec6084b, /// 0x504
			 0x497a66f8, /// 0x508
			 0xa08cbe3f, /// 0x50c
			 0x251f2506, /// 0x510
			 0x14924728, /// 0x514
			 0x2112f9bc, /// 0x518
			 0x25ff727a, /// 0x51c
			 0x295b5c3b, /// 0x520
			 0x19d18012, /// 0x524
			 0x21ec8bb1, /// 0x528
			 0xb8072a55, /// 0x52c
			 0xe5589f90, /// 0x530
			 0x8fe1227c, /// 0x534
			 0xefda2e8c, /// 0x538
			 0x6bd08e30, /// 0x53c
			 0xeb7d7470, /// 0x540
			 0xc9a2802b, /// 0x544
			 0xe9c545b6, /// 0x548
			 0x49071027, /// 0x54c
			 0xb0397623, /// 0x550
			 0xb14a9a97, /// 0x554
			 0xb6d037a5, /// 0x558
			 0xda6243e7, /// 0x55c
			 0xed15f030, /// 0x560
			 0xde244387, /// 0x564
			 0xde2e5375, /// 0x568
			 0xcc138727, /// 0x56c
			 0x2e5369b6, /// 0x570
			 0x531ec427, /// 0x574
			 0x9173bee4, /// 0x578
			 0x7da22f3e, /// 0x57c
			 0xac981e3a, /// 0x580
			 0xf924a43c, /// 0x584
			 0x4cef437d, /// 0x588
			 0xde5f4874, /// 0x58c
			 0xc6d935f6, /// 0x590
			 0x3a5a6135, /// 0x594
			 0xc041ce6c, /// 0x598
			 0x14a11133, /// 0x59c
			 0xa848afdd, /// 0x5a0
			 0xe706043e, /// 0x5a4
			 0x914b032e, /// 0x5a8
			 0x87360ee6, /// 0x5ac
			 0x0e7548b2, /// 0x5b0
			 0x75274d0b, /// 0x5b4
			 0x72960f66, /// 0x5b8
			 0x45f07fd2, /// 0x5bc
			 0x29b66339, /// 0x5c0
			 0xef20e8b1, /// 0x5c4
			 0xfbe38db3, /// 0x5c8
			 0x5fcc8d03, /// 0x5cc
			 0x2a2295c4, /// 0x5d0
			 0xd502b60a, /// 0x5d4
			 0x9551a92b, /// 0x5d8
			 0xd9c87ddc, /// 0x5dc
			 0x7147e24c, /// 0x5e0
			 0x53fbca1e, /// 0x5e4
			 0x0f042785, /// 0x5e8
			 0xb7bd62f2, /// 0x5ec
			 0xcbd14420, /// 0x5f0
			 0x3a8db2ae, /// 0x5f4
			 0xbf5eeeaf, /// 0x5f8
			 0xcbb0acb9, /// 0x5fc
			 0xa266f792, /// 0x600
			 0x99407dba, /// 0x604
			 0x872f48d4, /// 0x608
			 0x866216da, /// 0x60c
			 0x0a91d282, /// 0x610
			 0x1f82e483, /// 0x614
			 0x8cff3dba, /// 0x618
			 0xc9fe19b1, /// 0x61c
			 0x0b23adcb, /// 0x620
			 0xf0b8b5b6, /// 0x624
			 0x65a407a7, /// 0x628
			 0xc68f3935, /// 0x62c
			 0x2c235c95, /// 0x630
			 0xf9dd457b, /// 0x634
			 0xfb9b8d82, /// 0x638
			 0x87c0cf5f, /// 0x63c
			 0xae0e5220, /// 0x640
			 0x8543a32b, /// 0x644
			 0xc0203ba1, /// 0x648
			 0xd21f25ab, /// 0x64c
			 0x337f1798, /// 0x650
			 0xc010a883, /// 0x654
			 0xc27983d3, /// 0x658
			 0xfaa8d401, /// 0x65c
			 0x4fec37b4, /// 0x660
			 0x754402b1, /// 0x664
			 0x9e7bcfa6, /// 0x668
			 0x6258f3fc, /// 0x66c
			 0x4a88d9be, /// 0x670
			 0x939786fa, /// 0x674
			 0x2b95ee66, /// 0x678
			 0x1e296501, /// 0x67c
			 0x5fbad3be, /// 0x680
			 0x8e7d7886, /// 0x684
			 0xa88b725b, /// 0x688
			 0xc3a48d60, /// 0x68c
			 0x4bb4959c, /// 0x690
			 0xebe715a8, /// 0x694
			 0xf820857d, /// 0x698
			 0xe3890dab, /// 0x69c
			 0xe42d8983, /// 0x6a0
			 0x2257ff9e, /// 0x6a4
			 0xc1021d84, /// 0x6a8
			 0xb461d807, /// 0x6ac
			 0x70c69370, /// 0x6b0
			 0xd285b64b, /// 0x6b4
			 0x79bd6381, /// 0x6b8
			 0xe6318272, /// 0x6bc
			 0x08d6cf6f, /// 0x6c0
			 0xcd500966, /// 0x6c4
			 0x32467438, /// 0x6c8
			 0x9d57423d, /// 0x6cc
			 0x3ccfe9b3, /// 0x6d0
			 0x0651cb69, /// 0x6d4
			 0x4c34ea29, /// 0x6d8
			 0x4f50ed7a, /// 0x6dc
			 0xbf6ad0aa, /// 0x6e0
			 0x6c7c4855, /// 0x6e4
			 0xd0189c9e, /// 0x6e8
			 0x8fa52c0a, /// 0x6ec
			 0x237e5d15, /// 0x6f0
			 0xe7469dde, /// 0x6f4
			 0x1a295d38, /// 0x6f8
			 0x959b881c, /// 0x6fc
			 0xc759cd34, /// 0x700
			 0x01eff604, /// 0x704
			 0xcab3cfb0, /// 0x708
			 0x35d1ed7b, /// 0x70c
			 0xd1f442db, /// 0x710
			 0x647af24b, /// 0x714
			 0x4804df53, /// 0x718
			 0x96e48394, /// 0x71c
			 0xe151e52e, /// 0x720
			 0xc7cadb3d, /// 0x724
			 0x4ec1be21, /// 0x728
			 0xd360c81a, /// 0x72c
			 0x8e6c1211, /// 0x730
			 0x9d4e11a2, /// 0x734
			 0xf1fb237d, /// 0x738
			 0xb0152db6, /// 0x73c
			 0xda75d279, /// 0x740
			 0x1fd902c1, /// 0x744
			 0xf136a1eb, /// 0x748
			 0xec7759d1, /// 0x74c
			 0x4fdb13a0, /// 0x750
			 0x06a2e150, /// 0x754
			 0x70304a75, /// 0x758
			 0x9e3ba88a, /// 0x75c
			 0x94b9375b, /// 0x760
			 0xa4d30d5e, /// 0x764
			 0x43d4f2c6, /// 0x768
			 0x82ca27f3, /// 0x76c
			 0x3cafbca2, /// 0x770
			 0x15d01010, /// 0x774
			 0x1486adf3, /// 0x778
			 0xcda63e35, /// 0x77c
			 0x7c8b5e01, /// 0x780
			 0xa632e1f1, /// 0x784
			 0x5172e070, /// 0x788
			 0xa791994d, /// 0x78c
			 0x4f9bc518, /// 0x790
			 0x8db1539d, /// 0x794
			 0xa1052afc, /// 0x798
			 0x6105e48d, /// 0x79c
			 0xe809d5ef, /// 0x7a0
			 0xb821b136, /// 0x7a4
			 0x923b8da9, /// 0x7a8
			 0x6a7755a0, /// 0x7ac
			 0x01ba3f3b, /// 0x7b0
			 0xb675d732, /// 0x7b4
			 0xcc59a382, /// 0x7b8
			 0x32eda0db, /// 0x7bc
			 0x2c697dca, /// 0x7c0
			 0xd85fe87c, /// 0x7c4
			 0x56110b77, /// 0x7c8
			 0x2967f0ae, /// 0x7cc
			 0x2a63162b, /// 0x7d0
			 0xf4420849, /// 0x7d4
			 0x6ca60485, /// 0x7d8
			 0x9530fe37, /// 0x7dc
			 0xd2eac9cc, /// 0x7e0
			 0x0c3068db, /// 0x7e4
			 0x0a203f77, /// 0x7e8
			 0xe0f408c9, /// 0x7ec
			 0x963709e2, /// 0x7f0
			 0xe6b1776c, /// 0x7f4
			 0x9b98effc, /// 0x7f8
			 0x08a1eb85, /// 0x7fc
			 0xa572d2e9, /// 0x800
			 0x7b03705b, /// 0x804
			 0xe0c932be, /// 0x808
			 0x44a2eb4e, /// 0x80c
			 0xd3ab630f, /// 0x810
			 0x02d5a72b, /// 0x814
			 0xe53bdccb, /// 0x818
			 0xb08e226a, /// 0x81c
			 0x88e044d3, /// 0x820
			 0x814bc0be, /// 0x824
			 0xc295d51f, /// 0x828
			 0x10e473c0, /// 0x82c
			 0x34235052, /// 0x830
			 0x4f21fc11, /// 0x834
			 0xc4adc7bc, /// 0x838
			 0x5a897c51, /// 0x83c
			 0x8f2e334b, /// 0x840
			 0x12fe399b, /// 0x844
			 0xa60f9171, /// 0x848
			 0x7a3f88c1, /// 0x84c
			 0x1d23fba3, /// 0x850
			 0xa1b49028, /// 0x854
			 0x928601cb, /// 0x858
			 0x706c44e7, /// 0x85c
			 0x78f1a3fb, /// 0x860
			 0x1f7ce8c6, /// 0x864
			 0x115d22c4, /// 0x868
			 0x25c56ba7, /// 0x86c
			 0xc3f6b497, /// 0x870
			 0xcb25fd7c, /// 0x874
			 0xc03a3af1, /// 0x878
			 0x6ccf69c8, /// 0x87c
			 0x8f34a073, /// 0x880
			 0x27dc1001, /// 0x884
			 0x0b8b50e7, /// 0x888
			 0x1ad517d5, /// 0x88c
			 0x7dfa6956, /// 0x890
			 0x75ab108a, /// 0x894
			 0x65a54f8a, /// 0x898
			 0x6f01fc5c, /// 0x89c
			 0x9893dbd5, /// 0x8a0
			 0x5b7c3752, /// 0x8a4
			 0x01d6e42c, /// 0x8a8
			 0x32058527, /// 0x8ac
			 0xf901cd4a, /// 0x8b0
			 0x8b09ad55, /// 0x8b4
			 0xb4a4b052, /// 0x8b8
			 0x12914f50, /// 0x8bc
			 0x08f8f9f9, /// 0x8c0
			 0x9ec32929, /// 0x8c4
			 0xcaa3a26e, /// 0x8c8
			 0xb78a9167, /// 0x8cc
			 0x7dcf2fe4, /// 0x8d0
			 0x9306856c, /// 0x8d4
			 0x7558b6a3, /// 0x8d8
			 0x3daa047b, /// 0x8dc
			 0x4ea4f9e2, /// 0x8e0
			 0xf8a11bb6, /// 0x8e4
			 0x368c4874, /// 0x8e8
			 0x90c79c3e, /// 0x8ec
			 0xc6a54949, /// 0x8f0
			 0x09911342, /// 0x8f4
			 0x255d0017, /// 0x8f8
			 0xee8363b3, /// 0x8fc
			 0x14694d00, /// 0x900
			 0xdf69cc5a, /// 0x904
			 0xc63771b1, /// 0x908
			 0x5fdb806a, /// 0x90c
			 0x41ee3557, /// 0x910
			 0xd0b4dfbf, /// 0x914
			 0xc78ca7d9, /// 0x918
			 0xab35ab32, /// 0x91c
			 0x7fa6bd4b, /// 0x920
			 0x921236d4, /// 0x924
			 0xcdd4c336, /// 0x928
			 0xf80a87ea, /// 0x92c
			 0xe353cba9, /// 0x930
			 0x46df980e, /// 0x934
			 0x41b16ae8, /// 0x938
			 0x90c7a101, /// 0x93c
			 0xaab966ed, /// 0x940
			 0xec084f0a, /// 0x944
			 0x59352e44, /// 0x948
			 0xc7d2c4de, /// 0x94c
			 0xaa3e690c, /// 0x950
			 0x1ab293e3, /// 0x954
			 0x211bdaae, /// 0x958
			 0x0e3258e1, /// 0x95c
			 0xe78e1907, /// 0x960
			 0x662739b6, /// 0x964
			 0xcc76f337, /// 0x968
			 0x53c3abf7, /// 0x96c
			 0xc464d987, /// 0x970
			 0xc82b528b, /// 0x974
			 0x0af7d7d2, /// 0x978
			 0xf3cab16b, /// 0x97c
			 0xd0beadfe, /// 0x980
			 0x868963bd, /// 0x984
			 0x78c3f817, /// 0x988
			 0x84fe98cd, /// 0x98c
			 0x42edfa68, /// 0x990
			 0x1bab1c3f, /// 0x994
			 0x22b12a8a, /// 0x998
			 0xe6f41852, /// 0x99c
			 0x3844b800, /// 0x9a0
			 0x060b8242, /// 0x9a4
			 0x8ff6397e, /// 0x9a8
			 0x47657500, /// 0x9ac
			 0x9cbf65fb, /// 0x9b0
			 0x2f75d999, /// 0x9b4
			 0x016fcdb7, /// 0x9b8
			 0x7f812e76, /// 0x9bc
			 0xb2219857, /// 0x9c0
			 0x685524f5, /// 0x9c4
			 0x7cce3b2d, /// 0x9c8
			 0x050e703d, /// 0x9cc
			 0x2db20351, /// 0x9d0
			 0x6f59144e, /// 0x9d4
			 0x5c7145f5, /// 0x9d8
			 0x0f94529c, /// 0x9dc
			 0x540ed66c, /// 0x9e0
			 0x7bbf6013, /// 0x9e4
			 0x70dbb143, /// 0x9e8
			 0x7bd51a80, /// 0x9ec
			 0xbec24f9d, /// 0x9f0
			 0x3db16d3d, /// 0x9f4
			 0x1f76e9c6, /// 0x9f8
			 0x3d018eb8, /// 0x9fc
			 0x7c13e347, /// 0xa00
			 0xd7ed7009, /// 0xa04
			 0x79cee1d3, /// 0xa08
			 0x2dea9513, /// 0xa0c
			 0xf78148c3, /// 0xa10
			 0x826579a9, /// 0xa14
			 0x873e33da, /// 0xa18
			 0xd3226114, /// 0xa1c
			 0x06690a26, /// 0xa20
			 0x5fb3b96b, /// 0xa24
			 0xceca4cb9, /// 0xa28
			 0x15031bee, /// 0xa2c
			 0xf88181a1, /// 0xa30
			 0x9f392ed8, /// 0xa34
			 0x02569944, /// 0xa38
			 0xd4cc8682, /// 0xa3c
			 0xd032f255, /// 0xa40
			 0x8140a65a, /// 0xa44
			 0xc7517e6f, /// 0xa48
			 0x5622abd0, /// 0xa4c
			 0x70caf465, /// 0xa50
			 0x552ac439, /// 0xa54
			 0xd5567cc8, /// 0xa58
			 0x2798197e, /// 0xa5c
			 0x1930ced8, /// 0xa60
			 0x9b0b5cbc, /// 0xa64
			 0x7637062f, /// 0xa68
			 0xa9a784f7, /// 0xa6c
			 0xcce6b474, /// 0xa70
			 0xad20f99a, /// 0xa74
			 0x77acc983, /// 0xa78
			 0x10a1e9d4, /// 0xa7c
			 0x4f2b4856, /// 0xa80
			 0x47f856a7, /// 0xa84
			 0xc3a58369, /// 0xa88
			 0x0c88397f, /// 0xa8c
			 0x4b2fbaf4, /// 0xa90
			 0xc4a0a46d, /// 0xa94
			 0x0c2d9375, /// 0xa98
			 0xd44ebbd3, /// 0xa9c
			 0x72159de9, /// 0xaa0
			 0x58549eab, /// 0xaa4
			 0x26369ead, /// 0xaa8
			 0x1f0eba63, /// 0xaac
			 0x4e3b99b2, /// 0xab0
			 0x1c99102f, /// 0xab4
			 0x53bca4c6, /// 0xab8
			 0xd385563e, /// 0xabc
			 0x18bf9a6c, /// 0xac0
			 0xd1455bfa, /// 0xac4
			 0x46aa620d, /// 0xac8
			 0xe3e45773, /// 0xacc
			 0x80a32376, /// 0xad0
			 0xdeaa3df3, /// 0xad4
			 0xe4f81fa9, /// 0xad8
			 0x02936e92, /// 0xadc
			 0x2402f87c, /// 0xae0
			 0x0a8155c2, /// 0xae4
			 0x4e3c8c64, /// 0xae8
			 0xde876e5d, /// 0xaec
			 0xa6ebb9f8, /// 0xaf0
			 0x9e0d42d1, /// 0xaf4
			 0x12638b75, /// 0xaf8
			 0x396e9141, /// 0xafc
			 0x9fec7dba, /// 0xb00
			 0xb85d5186, /// 0xb04
			 0xb0e4106c, /// 0xb08
			 0x7c1b3b6d, /// 0xb0c
			 0x2dcb9bcf, /// 0xb10
			 0x99db8d73, /// 0xb14
			 0xf0237f2d, /// 0xb18
			 0xc57e0a86, /// 0xb1c
			 0x70c3448c, /// 0xb20
			 0x7ee743b4, /// 0xb24
			 0x81c02bd7, /// 0xb28
			 0xc4735e0a, /// 0xb2c
			 0xba454049, /// 0xb30
			 0x19d3a287, /// 0xb34
			 0xca55589f, /// 0xb38
			 0x45868916, /// 0xb3c
			 0x3dfcd6dc, /// 0xb40
			 0xb28538d0, /// 0xb44
			 0x1830185f, /// 0xb48
			 0xd3b74e42, /// 0xb4c
			 0x01323ccc, /// 0xb50
			 0x343fb3d3, /// 0xb54
			 0xe667fc51, /// 0xb58
			 0x47fe7012, /// 0xb5c
			 0xf38f2929, /// 0xb60
			 0xa0b8d6e2, /// 0xb64
			 0xf719c249, /// 0xb68
			 0x020bffb9, /// 0xb6c
			 0xfd4decff, /// 0xb70
			 0x7d6be875, /// 0xb74
			 0xcde38550, /// 0xb78
			 0x041430c5, /// 0xb7c
			 0x6b2d74aa, /// 0xb80
			 0x1ae4c7a4, /// 0xb84
			 0x64be2da9, /// 0xb88
			 0xd7481995, /// 0xb8c
			 0x893963fc, /// 0xb90
			 0x42185ee0, /// 0xb94
			 0x76b518fd, /// 0xb98
			 0x90e36364, /// 0xb9c
			 0xc6b3866b, /// 0xba0
			 0xdc242462, /// 0xba4
			 0xd019ad70, /// 0xba8
			 0x762c885d, /// 0xbac
			 0xd5a8cf89, /// 0xbb0
			 0xd419bf80, /// 0xbb4
			 0x3b404aee, /// 0xbb8
			 0xe618e0d9, /// 0xbbc
			 0x65ac0660, /// 0xbc0
			 0x20f0eaa3, /// 0xbc4
			 0xe44f4ddd, /// 0xbc8
			 0x3288bac2, /// 0xbcc
			 0x215727d4, /// 0xbd0
			 0x7277b39b, /// 0xbd4
			 0x494cab05, /// 0xbd8
			 0xea8ec182, /// 0xbdc
			 0xfbec65f4, /// 0xbe0
			 0x76d43966, /// 0xbe4
			 0x1e7dffcf, /// 0xbe8
			 0xd7f8ed28, /// 0xbec
			 0xde750083, /// 0xbf0
			 0x02477536, /// 0xbf4
			 0x1995eaaa, /// 0xbf8
			 0xb4061757, /// 0xbfc
			 0xd6a7220a, /// 0xc00
			 0xd90a3d59, /// 0xc04
			 0xb17db287, /// 0xc08
			 0x135a3975, /// 0xc0c
			 0xbade67a1, /// 0xc10
			 0xb34a01b7, /// 0xc14
			 0xcc0acdf6, /// 0xc18
			 0xe8a88064, /// 0xc1c
			 0x45a2e02f, /// 0xc20
			 0x48396f69, /// 0xc24
			 0xf8d02e94, /// 0xc28
			 0x185312fa, /// 0xc2c
			 0xcb9737f7, /// 0xc30
			 0xdc7ad3bc, /// 0xc34
			 0x9a9f1d66, /// 0xc38
			 0x00448822, /// 0xc3c
			 0x0f100a00, /// 0xc40
			 0x8f45eefb, /// 0xc44
			 0x998e6a9e, /// 0xc48
			 0x02d8c5f1, /// 0xc4c
			 0x2255687a, /// 0xc50
			 0xda2af257, /// 0xc54
			 0xc7e0c295, /// 0xc58
			 0x36288a9b, /// 0xc5c
			 0x1bcef0c2, /// 0xc60
			 0x9fd2ba1f, /// 0xc64
			 0x0c4abd6a, /// 0xc68
			 0xf74665ec, /// 0xc6c
			 0xab7e8939, /// 0xc70
			 0xefcc03a8, /// 0xc74
			 0x82f09e5e, /// 0xc78
			 0xbcd58f79, /// 0xc7c
			 0x20381f6d, /// 0xc80
			 0x62f231c9, /// 0xc84
			 0x895c61e9, /// 0xc88
			 0x5acd1d74, /// 0xc8c
			 0x7738b765, /// 0xc90
			 0xe36d9d93, /// 0xc94
			 0x21775756, /// 0xc98
			 0x2de5fb7b, /// 0xc9c
			 0xe07a9839, /// 0xca0
			 0x27f898d3, /// 0xca4
			 0x76a71081, /// 0xca8
			 0xbebdb683, /// 0xcac
			 0x6e86dfda, /// 0xcb0
			 0x29365036, /// 0xcb4
			 0x257d1fff, /// 0xcb8
			 0x4ae31c9c, /// 0xcbc
			 0x45fcf3ee, /// 0xcc0
			 0x8a5e441b, /// 0xcc4
			 0x8c58fabf, /// 0xcc8
			 0xadda66a5, /// 0xccc
			 0x233729da, /// 0xcd0
			 0x9f92c218, /// 0xcd4
			 0x7bde03fa, /// 0xcd8
			 0x3033ee22, /// 0xcdc
			 0x99183e83, /// 0xce0
			 0xd5212a7a, /// 0xce4
			 0x47c25ad3, /// 0xce8
			 0x4c0c7e28, /// 0xcec
			 0x05e708c5, /// 0xcf0
			 0xcd03c32f, /// 0xcf4
			 0x62a4ed9b, /// 0xcf8
			 0x67594ec5, /// 0xcfc
			 0x713419e7, /// 0xd00
			 0x75833c8e, /// 0xd04
			 0x34c6493d, /// 0xd08
			 0x18b7a097, /// 0xd0c
			 0x8711fd68, /// 0xd10
			 0x8124bf7c, /// 0xd14
			 0x25b3614d, /// 0xd18
			 0xc72ed135, /// 0xd1c
			 0x3fea5190, /// 0xd20
			 0x7cf144f3, /// 0xd24
			 0xfefcb2af, /// 0xd28
			 0xaff1e1c7, /// 0xd2c
			 0x4bad624a, /// 0xd30
			 0x64f68ca8, /// 0xd34
			 0x068b2d3b, /// 0xd38
			 0xedeb835f, /// 0xd3c
			 0x9c70af6d, /// 0xd40
			 0xe9e1e9cb, /// 0xd44
			 0xa3766597, /// 0xd48
			 0x19a2d599, /// 0xd4c
			 0x580768fa, /// 0xd50
			 0xef0c4747, /// 0xd54
			 0xa9a21e0d, /// 0xd58
			 0x86c99981, /// 0xd5c
			 0x97d88861, /// 0xd60
			 0x2a690c89, /// 0xd64
			 0x4e1eb696, /// 0xd68
			 0x82c4339e, /// 0xd6c
			 0x241705ad, /// 0xd70
			 0x414d5b04, /// 0xd74
			 0x4d09e757, /// 0xd78
			 0x8a257a32, /// 0xd7c
			 0xb7c1444a, /// 0xd80
			 0x1f74987c, /// 0xd84
			 0xfb4ef647, /// 0xd88
			 0xa2d8af24, /// 0xd8c
			 0x31c1aa73, /// 0xd90
			 0x6992459a, /// 0xd94
			 0x0f01fac4, /// 0xd98
			 0xb1f25418, /// 0xd9c
			 0x60143929, /// 0xda0
			 0x756e1e37, /// 0xda4
			 0x044ea4fe, /// 0xda8
			 0xa9c355ae, /// 0xdac
			 0xe7182caf, /// 0xdb0
			 0x6000d2d3, /// 0xdb4
			 0x1ff780bc, /// 0xdb8
			 0xdb1c298e, /// 0xdbc
			 0xead5a4a7, /// 0xdc0
			 0x2627057f, /// 0xdc4
			 0xf16ca259, /// 0xdc8
			 0xe43d5b7a, /// 0xdcc
			 0xa58b1cbb, /// 0xdd0
			 0xb93bc441, /// 0xdd4
			 0xc8e5475b, /// 0xdd8
			 0x9c6e9f0f, /// 0xddc
			 0x53cfe022, /// 0xde0
			 0x71b9d976, /// 0xde4
			 0x9701310c, /// 0xde8
			 0x89e057e9, /// 0xdec
			 0x88f03e8a, /// 0xdf0
			 0x80c0223f, /// 0xdf4
			 0x92178434, /// 0xdf8
			 0xdeb06d18, /// 0xdfc
			 0x5e8706fa, /// 0xe00
			 0xa99b56f6, /// 0xe04
			 0xdbbbe9e2, /// 0xe08
			 0xa8c961b9, /// 0xe0c
			 0xe5b78f34, /// 0xe10
			 0xcbca96f4, /// 0xe14
			 0x25b190d5, /// 0xe18
			 0x8f9ad2a5, /// 0xe1c
			 0x286086e5, /// 0xe20
			 0xdab82930, /// 0xe24
			 0x84836959, /// 0xe28
			 0xa11cfdca, /// 0xe2c
			 0xaedcd3e7, /// 0xe30
			 0x561aaeb9, /// 0xe34
			 0xb7728cbf, /// 0xe38
			 0x401a3599, /// 0xe3c
			 0xd1d096b0, /// 0xe40
			 0x0ec05ec8, /// 0xe44
			 0xf8c3a905, /// 0xe48
			 0x722c9922, /// 0xe4c
			 0xf07e6f48, /// 0xe50
			 0xca994f05, /// 0xe54
			 0xc19afa9f, /// 0xe58
			 0xebfec446, /// 0xe5c
			 0x6a79cede, /// 0xe60
			 0x850179d3, /// 0xe64
			 0xbc4d1245, /// 0xe68
			 0x482ad9e8, /// 0xe6c
			 0x997efec5, /// 0xe70
			 0xd6bcfc94, /// 0xe74
			 0xe84eb1ee, /// 0xe78
			 0x734055cb, /// 0xe7c
			 0x7cd8f891, /// 0xe80
			 0x0ed26b34, /// 0xe84
			 0xd3363ebf, /// 0xe88
			 0x1feea120, /// 0xe8c
			 0xed19f0be, /// 0xe90
			 0x78eb4191, /// 0xe94
			 0xc2503b3d, /// 0xe98
			 0xb74a0850, /// 0xe9c
			 0xaeabb85b, /// 0xea0
			 0xec6ba306, /// 0xea4
			 0xf164d1c7, /// 0xea8
			 0xaa07f5ad, /// 0xeac
			 0xad2dce86, /// 0xeb0
			 0xb63c79a8, /// 0xeb4
			 0x494cb355, /// 0xeb8
			 0x8e794a21, /// 0xebc
			 0x107b719f, /// 0xec0
			 0x9c49344a, /// 0xec4
			 0xbacd6fca, /// 0xec8
			 0x5eb8eb69, /// 0xecc
			 0x3cfdfe21, /// 0xed0
			 0xcb85c846, /// 0xed4
			 0xe1d1bd41, /// 0xed8
			 0x0583fb1b, /// 0xedc
			 0x2a45c3eb, /// 0xee0
			 0xb24ba0d1, /// 0xee4
			 0xdd6b4e46, /// 0xee8
			 0xe0f0c07d, /// 0xeec
			 0x5289b188, /// 0xef0
			 0xbfa00acf, /// 0xef4
			 0xa9c50879, /// 0xef8
			 0x57589428, /// 0xefc
			 0x78aa7f39, /// 0xf00
			 0x9bba7aa1, /// 0xf04
			 0xcc1ad7d2, /// 0xf08
			 0x193002a1, /// 0xf0c
			 0x57953e3c, /// 0xf10
			 0x29a7f09b, /// 0xf14
			 0x1ddefe9b, /// 0xf18
			 0x62e33db7, /// 0xf1c
			 0x4707dcc8, /// 0xf20
			 0xd8fae658, /// 0xf24
			 0x729b8175, /// 0xf28
			 0x98e94c08, /// 0xf2c
			 0x1a67d3e0, /// 0xf30
			 0x28d129a4, /// 0xf34
			 0xeb4eade7, /// 0xf38
			 0xa6aade66, /// 0xf3c
			 0x9e2f5046, /// 0xf40
			 0xd01a1516, /// 0xf44
			 0xd2a9207e, /// 0xf48
			 0x2986cd66, /// 0xf4c
			 0xff610718, /// 0xf50
			 0x2cc61cab, /// 0xf54
			 0x1618ae89, /// 0xf58
			 0x8f062d4c, /// 0xf5c
			 0x4f0567c4, /// 0xf60
			 0x72ed63d6, /// 0xf64
			 0x04270ebb, /// 0xf68
			 0x691dd0ed, /// 0xf6c
			 0x8df31060, /// 0xf70
			 0x643f6e59, /// 0xf74
			 0x706da1d7, /// 0xf78
			 0xf6818199, /// 0xf7c
			 0x5708e5ff, /// 0xf80
			 0x50ca78db, /// 0xf84
			 0x225afacb, /// 0xf88
			 0xa1ef771e, /// 0xf8c
			 0x06153f79, /// 0xf90
			 0x83f8ab9f, /// 0xf94
			 0x621d4c2a, /// 0xf98
			 0xf6b74e61, /// 0xf9c
			 0x43fac6a7, /// 0xfa0
			 0x6bb80a83, /// 0xfa4
			 0x464d16a4, /// 0xfa8
			 0xf060a806, /// 0xfac
			 0x66200fa7, /// 0xfb0
			 0x5dd21691, /// 0xfb4
			 0x99f93304, /// 0xfb8
			 0x412bedec, /// 0xfbc
			 0x57dfaa9b, /// 0xfc0
			 0xca8eed02, /// 0xfc4
			 0x12e04c44, /// 0xfc8
			 0x0ef00559, /// 0xfcc
			 0x44f7e628, /// 0xfd0
			 0xbcbac773, /// 0xfd4
			 0xd53dd19c, /// 0xfd8
			 0x4d888060, /// 0xfdc
			 0xb8517f80, /// 0xfe0
			 0x7cc0ce3a, /// 0xfe4
			 0x469457d5, /// 0xfe8
			 0xfaf3c76a, /// 0xfec
			 0xdbd3cb12, /// 0xff0
			 0x5c706888, /// 0xff4
			 0x340e150a, /// 0xff8
			 0x86f7d20d /// last
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
			 0x00000404,
			 0x00000384,
			 0x000006f0,
			 0x000003c4,
			 0x000005f0,
			 0x00000154,
			 0x0000066c,
			 0x000001cc,

			 /// vpu register f2
			 0x41200000,
			 0x41900000,
			 0x41a00000,
			 0x40000000,
			 0x41a00000,
			 0x41d00000,
			 0x41200000,
			 0x41d00000,

			 /// vpu register f3
			 0x41c00000,
			 0x41e80000,
			 0x41980000,
			 0x41700000,
			 0x41100000,
			 0x40800000,
			 0x41600000,
			 0x41900000,

			 /// vpu register f4
			 0x41980000,
			 0x41300000,
			 0x41d00000,
			 0x41d00000,
			 0x41d80000,
			 0x41b00000,
			 0x41b00000,
			 0x41e80000,

			 /// vpu register f5
			 0x40a00000,
			 0x41300000,
			 0x41600000,
			 0x41600000,
			 0x41400000,
			 0x41e80000,
			 0x41500000,
			 0x3f800000,

			 /// vpu register f6
			 0x41800000,
			 0x41e00000,
			 0x41000000,
			 0x40a00000,
			 0x40e00000,
			 0x41880000,
			 0x41c80000,
			 0x40c00000,

			 /// vpu register f7
			 0x41300000,
			 0x41880000,
			 0x40a00000,
			 0x41500000,
			 0x41b80000,
			 0x41880000,
			 0x40c00000,
			 0x41900000,

			 /// vpu register f8
			 0x41880000,
			 0x41880000,
			 0x40800000,
			 0x41e00000,
			 0x41d80000,
			 0x41000000,
			 0x41800000,
			 0x41700000,

			 /// vpu register f9
			 0x41900000,
			 0x41f80000,
			 0x3f800000,
			 0x3f800000,
			 0x41c80000,
			 0x40a00000,
			 0x40e00000,
			 0x41a80000,

			 /// vpu register f10
			 0x41400000,
			 0x40c00000,
			 0x41200000,
			 0x41b00000,
			 0x41980000,
			 0x41500000,
			 0x41f00000,
			 0x41b80000,

			 /// vpu register f11
			 0x41d00000,
			 0x41f80000,
			 0x41300000,
			 0x41a80000,
			 0x3f800000,
			 0x40400000,
			 0x41700000,
			 0x40000000,

			 /// vpu register f12
			 0x40c00000,
			 0x41a00000,
			 0x40a00000,
			 0x41d80000,
			 0x41600000,
			 0x41b80000,
			 0x41e00000,
			 0x42000000,

			 /// vpu register f13
			 0x40800000,
			 0x40a00000,
			 0x41f80000,
			 0x41c80000,
			 0x41f00000,
			 0x41880000,
			 0x41980000,
			 0x41f00000,

			 /// vpu register f14
			 0x41800000,
			 0x41f00000,
			 0x41d80000,
			 0x41500000,
			 0x41e00000,
			 0x41900000,
			 0x41600000,
			 0x40e00000,

			 /// vpu register f15
			 0x41100000,
			 0x41b00000,
			 0x41000000,
			 0x3f800000,
			 0x41c00000,
			 0x41200000,
			 0x41980000,
			 0x41f80000,

			 /// vpu register f16
			 0x40400000,
			 0x41c80000,
			 0x42000000,
			 0x41b00000,
			 0x40400000,
			 0x40400000,
			 0x41e80000,
			 0x41300000,

			 /// vpu register f17
			 0x41c80000,
			 0x41000000,
			 0x41b80000,
			 0x41500000,
			 0x41900000,
			 0x41600000,
			 0x41100000,
			 0x41500000,

			 /// vpu register f18
			 0x41800000,
			 0x40a00000,
			 0x41880000,
			 0x41c00000,
			 0x41200000,
			 0x41400000,
			 0x40a00000,
			 0x40a00000,

			 /// vpu register f19
			 0x41300000,
			 0x41300000,
			 0x41900000,
			 0x41000000,
			 0x41d00000,
			 0x40c00000,
			 0x41400000,
			 0x40c00000,

			 /// vpu register f20
			 0x41500000,
			 0x41b00000,
			 0x41b00000,
			 0x41200000,
			 0x40a00000,
			 0x41c00000,
			 0x41c00000,
			 0x41c00000,

			 /// vpu register f21
			 0x41300000,
			 0x41600000,
			 0x41400000,
			 0x42000000,
			 0x41900000,
			 0x41c00000,
			 0x41d80000,
			 0x40a00000,

			 /// vpu register f22
			 0x41a80000,
			 0x40c00000,
			 0x41e00000,
			 0x41b80000,
			 0x3f800000,
			 0x41f00000,
			 0x3f800000,
			 0x41b80000,

			 /// vpu register f23
			 0x41c80000,
			 0x40000000,
			 0x41c00000,
			 0x41f00000,
			 0x3f800000,
			 0x41c00000,
			 0x41d80000,
			 0x41000000,

			 /// vpu register f24
			 0x41b80000,
			 0x41400000,
			 0x3f800000,
			 0x41000000,
			 0x41980000,
			 0x41a80000,
			 0x41d80000,
			 0x40c00000,

			 /// vpu register f25
			 0x40a00000,
			 0x41100000,
			 0x40e00000,
			 0x40a00000,
			 0x41e00000,
			 0x41c00000,
			 0x41500000,
			 0x41000000,

			 /// vpu register f26
			 0x41a80000,
			 0x42000000,
			 0x3f800000,
			 0x40800000,
			 0x41800000,
			 0x41c80000,
			 0x41f80000,
			 0x41d00000,

			 /// vpu register f27
			 0x41880000,
			 0x41800000,
			 0x41880000,
			 0x41e00000,
			 0x41200000,
			 0x41400000,
			 0x41d80000,
			 0x41a00000,

			 /// vpu register f28
			 0x41f80000,
			 0x41c00000,
			 0x41200000,
			 0x41000000,
			 0x40000000,
			 0x41e00000,
			 0x41b80000,
			 0x41000000,

			 /// vpu register f29
			 0x41c00000,
			 0x41c00000,
			 0x41f00000,
			 0x41f80000,
			 0x41200000,
			 0x41900000,
			 0x41500000,
			 0x41700000,

			 /// vpu register f30
			 0x41000000,
			 0x41d80000,
			 0x40a00000,
			 0x41100000,
			 0x41500000,
			 0x41f80000,
			 0x3f800000,
			 0x41800000,

			 /// vpu register f31
			 0x41300000,
			 0x41500000,
			 0x40800000,
			 0x41700000,
			 0x3f800000,
			 0x40c00000,
			 0x41880000,
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
		"fcvt.ps.un8 f13, f10\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f6, f1\n"                                ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f24, f1\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f26, f30\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f8, f0\n"                                ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f19, f12\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f26, f3\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f15, f4\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f27, f15\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f27, f29\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f10, f18\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f24, f20\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f26, f22\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f8, f12\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f14, f2\n"                               ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f9, f11\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f23, f9\n"                               ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f15, f30\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f4, f25\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f8, f6\n"                                ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f27, f4\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f12, f0\n"                               ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f17, f14\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f26, f5\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f11, f30\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f2, f15\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f7, f11\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f30, f14\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f23, f17\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f26, f4\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f3, f12\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f21, f8\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f13, f10\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f28, f1\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f2, f6\n"                                ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f22, f15\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f8, f21\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f6, f9\n"                                ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f26, f29\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f26, f25\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f2, f22\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f26, f30\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f27, f7\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f26, f4\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f28, f25\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f9, f16\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f5, f18\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f17, f30\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f16, f14\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f6, f21\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f6, f16\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f22, f8\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f25, f12\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f21, f30\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f18, f13\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f19, f2\n"                               ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f8, f14\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f20, f1\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f28, f28\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f28, f20\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f10, f2\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f16, f26\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f2, f16\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f23, f16\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f20, f25\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f23, f5\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f25, f17\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f28, f7\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f20, f9\n"                               ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f25, f22\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f26, f23\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f9, f25\n"                               ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f5, f15\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f26, f7\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f21, f16\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f26, f21\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f3, f21\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f18, f3\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f27, f4\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f19, f0\n"                               ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f30, f27\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f5, f14\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f5, f22\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f2, f1\n"                                ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f24, f4\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f10, f23\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f20, f25\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f19, f12\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f29, f30\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f10, f3\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f23, f0\n"                               ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f29, f0\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f11, f8\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f22, f3\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f8, f25\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f23, f2\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f7, f6\n"                                ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f5, f28\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.ps.un8 f3, f21\n"                               ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
