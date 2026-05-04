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
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x80011111,                                                  // -9.7958E-41                                 /// 00004
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00008
			 0xbf028f5c,                                                  // -0.51                                       /// 0000c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x0c600000,                                                  // Leading 1s:                                 /// 00014
			 0x00011111,                                                  // 9.7958E-41                                  /// 00018
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00020
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00024
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00028
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00034
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00038
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0003c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00040
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00044
			 0x7fc00001,                                                  // signaling NaN                               /// 00048
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0004c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00054
			 0x0e000001,                                                  // Trailing 1s:                                /// 00058
			 0x33333333,                                                  // 4 random values                             /// 0005c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00060
			 0x0e000007,                                                  // Trailing 1s:                                /// 00064
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00080
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00084
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00088
			 0x0c780000,                                                  // Leading 1s:                                 /// 0008c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00094
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0009c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000b8
			 0x3f028f5c,                                                  // 0.51                                        /// 000bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x00000000,                                                  // zero                                        /// 000e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00104
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0010c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00110
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00114
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00118
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0011c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00120
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00124
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00128
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0012c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00130
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00138
			 0x3f028f5c,                                                  // 0.51                                        /// 0013c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00140
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00144
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00148
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0014c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00150
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00158
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0015c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00160
			 0x55555555,                                                  // 4 random values                             /// 00164
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00168
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0016c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00178
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00180
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00184
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00188
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0018c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00190
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00194
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001cc
			 0x80011111,                                                  // -9.7958E-41                                 /// 001d0
			 0x55555555,                                                  // 4 random values                             /// 001d4
			 0x80000000,                                                  // -zero                                       /// 001d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001dc
			 0x33333333,                                                  // 4 random values                             /// 001e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00200
			 0x80011111,                                                  // -9.7958E-41                                 /// 00204
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00210
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00218
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0021c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00220
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00228
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0022c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00234
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00238
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0023c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00240
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00244
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00248
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00250
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00258
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0xffc00001,                                                  // -signaling NaN                              /// 00260
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00264
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00268
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0027c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00280
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00284
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00288
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00294
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002c8
			 0x4b000000,                                                  // 8388608.0                                   /// 002cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x3f028f5c,                                                  // 0.51                                        /// 002e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00300
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00308
			 0x0c700000,                                                  // Leading 1s:                                 /// 0030c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00310
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00318
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0031c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x4b000000,                                                  // 8388608.0                                   /// 00328
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0032c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00330
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0033c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00340
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x80011111,                                                  // -9.7958E-41                                 /// 00348
			 0x0e000007,                                                  // Trailing 1s:                                /// 0034c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00350
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00354
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0035c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00360
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00364
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00368
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00370
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00374
			 0x00011111,                                                  // 9.7958E-41                                  /// 00378
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0037c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0xff800000,                                                  // -inf                                        /// 00384
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00388
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0038c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00390
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00394
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00398
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0039c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003a4
			 0x3f028f5c,                                                  // 0.51                                        /// 003a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003cc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003e0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00400
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00408
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0040c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00414
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0041c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00420
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00424
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00428
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x0e000001,                                                  // Trailing 1s:                                /// 00434
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00438
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00448
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0044c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00454
			 0x0c700000,                                                  // Leading 1s:                                 /// 00458
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0045c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00464
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00468
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00474
			 0x0c400000,                                                  // Leading 1s:                                 /// 00478
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0047c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00480
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00488
			 0xbf028f5c,                                                  // -0.51                                       /// 0048c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00490
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00494
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00000000,                                                  // zero                                        /// 0049c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00500
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00504
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00508
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0050c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00510
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00514
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00518
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00524
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00528
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0052c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00530
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00534
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00538
			 0xbf028f5c,                                                  // -0.51                                       /// 0053c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00540
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00544
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00548
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00550
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00554
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00558
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00560
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00570
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00578
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0057c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00580
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00588
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00590
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00594
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0x00000000,                                                  // zero                                        /// 00604
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00608
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0060c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00610
			 0xcb000000,                                                  // -8388608.0                                  /// 00614
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00620
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00624
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00630
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00634
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00644
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0064c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x80000000,                                                  // -zero                                       /// 00654
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00658
			 0x00011111,                                                  // 9.7958E-41                                  /// 0065c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00660
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00664
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00668
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0066c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00678
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0067c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00680
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00684
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00688
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0068c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00690
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00698
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 006b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006bc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006c8
			 0x7f800000,                                                  // inf                                         /// 006cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 006e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00700
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00708
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0070c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00710
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00714
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00718
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0071c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00720
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00728
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0072c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00738
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0073c
			 0x3f028f5c,                                                  // 0.51                                        /// 00740
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00744
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00748
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0074c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00750
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00754
			 0x55555555,                                                  // 4 random values                             /// 00758
			 0x00000000,                                                  // zero                                        /// 0075c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00760
			 0x0c700000,                                                  // Leading 1s:                                 /// 00764
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00768
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0076c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00778
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0077c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00780
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00788
			 0x3f028f5c,                                                  // 0.51                                        /// 0078c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00790
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00794
			 0x0c780000,                                                  // Leading 1s:                                 /// 00798
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0079c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x4b000000,                                                  // 8388608.0                                   /// 007b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007e8
			 0x3f028f5c,                                                  // 0.51                                        /// 007ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x0c780000,                                                  // Leading 1s:                                 /// 00804
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00808
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0080c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00818
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0081c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00828
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0082c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00830
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00834
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00840
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00848
			 0x7f800000,                                                  // inf                                         /// 0084c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00850
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00854
			 0x0e000003,                                                  // Trailing 1s:                                /// 00858
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0085c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00860
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00864
			 0x7fc00001,                                                  // signaling NaN                               /// 00868
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00870
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0xcb000000,                                                  // -8388608.0                                  /// 00878
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0087c
			 0x7fc00001,                                                  // signaling NaN                               /// 00880
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00888
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0088c
			 0x7fc00001,                                                  // signaling NaN                               /// 00890
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00894
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0089c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 008b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 008e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00900
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00904
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0xbf028f5c,                                                  // -0.51                                       /// 0090c
			 0xbf028f5c,                                                  // -0.51                                       /// 00910
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0091c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00924
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x55555555,                                                  // 4 random values                             /// 0092c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00930
			 0x4b000000,                                                  // 8388608.0                                   /// 00934
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00938
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0093c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00944
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00950
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0095c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00968
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0096c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00970
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00980
			 0x0e000001,                                                  // Trailing 1s:                                /// 00984
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00988
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00990
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00994
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00998
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 009a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009c0
			 0x4b000000,                                                  // 8388608.0                                   /// 009c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009cc
			 0x33333333,                                                  // 4 random values                             /// 009d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009f0
			 0xcb000000,                                                  // -8388608.0                                  /// 009f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009f8
			 0xbf028f5c,                                                  // -0.51                                       /// 009fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a00
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a04
			 0x7f800000,                                                  // inf                                         /// 00a08
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a2c
			 0x33333333,                                                  // 4 random values                             /// 00a30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a38
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a3c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a48
			 0x7fc00001,                                                  // signaling NaN                               /// 00a4c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a68
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a78
			 0x80000000,                                                  // -zero                                       /// 00a7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a88
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a98
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00aa4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00aa8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ab4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ab8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00abc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ac0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ac4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ad4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ad8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00adc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ae0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ae8
			 0x3f028f5c,                                                  // 0.51                                        /// 00aec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00af0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00afc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b08
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b0c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b10
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b30
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x7fc00001,                                                  // signaling NaN                               /// 00b38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b44
			 0x33333333,                                                  // 4 random values                             /// 00b48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b4c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b5c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b68
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b74
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b90
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x4b000000,                                                  // 8388608.0                                   /// 00b9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ba0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ba4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ba8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bb0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bb4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bb8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bbc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bc0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bc4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bc8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bd0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bd4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bd8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bdc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00be0
			 0x55555555,                                                  // 4 random values                             /// 00be4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bf0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c00
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c0c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c1c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c24
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c2c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c38
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c58
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c60
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c68
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c6c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0xffc00001,                                                  // -signaling NaN                              /// 00c78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c90
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c94
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ca0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ca4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ca8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cb0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cb4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cc0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cc8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ccc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cd0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cd8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ce8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cf4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cf8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d00
			 0x00000000,                                                  // zero                                        /// 00d04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d08
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d0c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d20
			 0x3f028f5c,                                                  // 0.51                                        /// 00d24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d30
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d38
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d44
			 0x55555555,                                                  // 4 random values                             /// 00d48
			 0x4b000000,                                                  // 8388608.0                                   /// 00d4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d64
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d70
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d74
			 0xcb000000,                                                  // -8388608.0                                  /// 00d78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d90
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d94
			 0x4b000000,                                                  // 8388608.0                                   /// 00d98
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00da0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00da4
			 0x7f800000,                                                  // inf                                         /// 00da8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00db0
			 0x00000000,                                                  // zero                                        /// 00db4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00db8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dd4
			 0x00000000,                                                  // zero                                        /// 00dd8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ddc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00de0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00de8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00df4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00df8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dfc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e00
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e08
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e0c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e14
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e18
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e24
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e2c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e38
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e40
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e44
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e48
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e4c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e58
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e68
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e70
			 0x00000000,                                                  // zero                                        /// 00e74
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e78
			 0x4b000000,                                                  // 8388608.0                                   /// 00e7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e80
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e84
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e88
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e94
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ea0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ea4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ea8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eb0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00eb4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00eb8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ebc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ec0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ed8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ee8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ef0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ef8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00efc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f0c
			 0x7f800000,                                                  // inf                                         /// 00f10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f18
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f24
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f28
			 0x3f028f5c,                                                  // 0.51                                        /// 00f2c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f38
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f3c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f40
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f44
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f48
			 0xbf028f5c,                                                  // -0.51                                       /// 00f4c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f58
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f88
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f9c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x55555555,                                                  // 4 random values                             /// 00fa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fb4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fc4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fd0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fe0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fe4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fe8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ff4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x80011111                                                  // -9.7958E-41                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00000
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00004
			 0x0c780000,                                                  // Leading 1s:                                 /// 00008
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00010
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00014
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00018
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00020
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00024
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00028
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0002c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00034
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00038
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0003c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00044
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x80000000,                                                  // -zero                                       /// 00054
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00058
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0005c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00060
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0006c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00074
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00078
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0007c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00080
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00084
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00088
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00094
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00098
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000b0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000cc
			 0xcb000000,                                                  // -8388608.0                                  /// 000d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00100
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00104
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00110
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00114
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00124
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00128
			 0x80000000,                                                  // -zero                                       /// 0012c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00130
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00134
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00140
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00144
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00148
			 0x0c700000,                                                  // Leading 1s:                                 /// 0014c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00150
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00164
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00168
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0016c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00170
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00174
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0017c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00180
			 0x0c600000,                                                  // Leading 1s:                                 /// 00184
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00188
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0018c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00190
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00194
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00198
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x55555555,                                                  // 4 random values                             /// 001a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001b0
			 0xff800000,                                                  // -inf                                        /// 001b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001bc
			 0x55555555,                                                  // 4 random values                             /// 001c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00200
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00204
			 0x0e000007,                                                  // Trailing 1s:                                /// 00208
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x00000000,                                                  // zero                                        /// 00210
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00218
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0021c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00220
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00224
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00228
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00230
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00234
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00238
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0023c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0xffc00001,                                                  // -signaling NaN                              /// 0024c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00254
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00260
			 0x80000000,                                                  // -zero                                       /// 00264
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00268
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00278
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0027c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00284
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00290
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00294
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00298
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0029c
			 0x0c600000,                                                  // Leading 1s:                                 /// 002a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002b0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 002f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00300
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0030c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00310
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00314
			 0x0e000003,                                                  // Trailing 1s:                                /// 00318
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0031c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00320
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00324
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0032c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00330
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00334
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00338
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0033c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00340
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0034c
			 0x7fc00001,                                                  // signaling NaN                               /// 00350
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0035c
			 0x33333333,                                                  // 4 random values                             /// 00360
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00364
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00370
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00374
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x7f800000,                                                  // inf                                         /// 0037c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00380
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00011111,                                                  // 9.7958E-41                                  /// 00388
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0038c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0xff800000,                                                  // -inf                                        /// 00398
			 0x0c400000,                                                  // Leading 1s:                                 /// 0039c
			 0x0c700000,                                                  // Leading 1s:                                 /// 003a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003a4
			 0x00011111,                                                  // 9.7958E-41                                  /// 003a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x7fc00001,                                                  // signaling NaN                               /// 003ec
			 0x4b000000,                                                  // 8388608.0                                   /// 003f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00400
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00404
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00408
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00418
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0041c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00420
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0e000003,                                                  // Trailing 1s:                                /// 00428
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0042c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00434
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00444
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00448
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0044c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00450
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00458
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0045c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00460
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00464
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0046c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00470
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00474
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00478
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0047c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0xbf028f5c,                                                  // -0.51                                       /// 0048c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00490
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00498
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0049c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x55555555,                                                  // 4 random values                             /// 004c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004e0
			 0x55555555,                                                  // 4 random values                             /// 004e4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00504
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x4b000000,                                                  // 8388608.0                                   /// 0050c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00514
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00518
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0051c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00520
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00528
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0052c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00530
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00544
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00548
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0054c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00558
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0055c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00564
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x55555555,                                                  // 4 random values                             /// 00574
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x0e000007,                                                  // Trailing 1s:                                /// 0057c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00580
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00584
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00588
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0058c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00594
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0xffc00001,                                                  // -signaling NaN                              /// 005b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xcb000000,                                                  // -8388608.0                                  /// 005b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0c400000,                                                  // Leading 1s:                                 /// 00604
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0060c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0061c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00620
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00624
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x4b000000,                                                  // 8388608.0                                   /// 0062c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00634
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00638
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00640
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00650
			 0x7fc00001,                                                  // signaling NaN                               /// 00654
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00658
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0065c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00660
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00664
			 0x80011111,                                                  // -9.7958E-41                                 /// 00668
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00670
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00674
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0067c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00688
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0068c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00694
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0069c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006a4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 006b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x33333333,                                                  // 4 random values                             /// 006d0
			 0xffc00001,                                                  // -signaling NaN                              /// 006d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00700
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00704
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00708
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0070c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00718
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x7fc00001,                                                  // signaling NaN                               /// 00720
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00728
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0072c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00734
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00738
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0073c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00748
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0074c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00750
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00754
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00758
			 0x00000000,                                                  // zero                                        /// 0075c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0e000001,                                                  // Trailing 1s:                                /// 00764
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00768
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00770
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x00011111,                                                  // 9.7958E-41                                  /// 00778
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0077c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00780
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0078c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00794
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0079c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0xffc00001,                                                  // -signaling NaN                              /// 007a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0xff800000,                                                  // -inf                                        /// 007d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007fc
			 0x00000000,                                                  // zero                                        /// 00800
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00804
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00808
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0080c
			 0x33333333,                                                  // 4 random values                             /// 00810
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00814
			 0x7f800000,                                                  // inf                                         /// 00818
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0081c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00820
			 0x0e000003,                                                  // Trailing 1s:                                /// 00824
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00828
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0082c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00830
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00834
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00838
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0083c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00840
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0084c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00850
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00858
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00860
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00864
			 0x0e000007,                                                  // Trailing 1s:                                /// 00868
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0086c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00870
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00878
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00880
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x7fc00001,                                                  // signaling NaN                               /// 00888
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0088c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00890
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00898
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0089c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008d4
			 0x33333333,                                                  // 4 random values                             /// 008d8
			 0x55555555,                                                  // 4 random values                             /// 008dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008e0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00904
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00908
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00918
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00920
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00924
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00928
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0092c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00930
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00934
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0093c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00940
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00944
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00950
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00954
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0095c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00960
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00964
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0096c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00970
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00974
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00978
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x55555555,                                                  // 4 random values                             /// 00990
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00994
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00998
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0099c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009a0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009fc
			 0x00000000,                                                  // zero                                        /// 00a00
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a10
			 0x00000000,                                                  // zero                                        /// 00a14
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a18
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a1c
			 0x00000000,                                                  // zero                                        /// 00a20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a24
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a28
			 0x4b000000,                                                  // 8388608.0                                   /// 00a2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a30
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a3c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a44
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a50
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a54
			 0x7f800000,                                                  // inf                                         /// 00a58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a5c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a60
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a6c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a70
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a78
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a80
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a94
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aa0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00aa4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00aa8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ab4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ab8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ac8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ad4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ad8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00adc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ae0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ae4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00aec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00af0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00af8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00afc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b20
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b24
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0xffc00001,                                                  // -signaling NaN                              /// 00b3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b50
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x33333333,                                                  // 4 random values                             /// 00b58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b60
			 0x7f800000,                                                  // inf                                         /// 00b64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b74
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b90
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b98
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b9c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ba0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ba4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0xff800000,                                                  // -inf                                        /// 00bac
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bb0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bb4
			 0xff800000,                                                  // -inf                                        /// 00bb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bbc
			 0xbf028f5c,                                                  // -0.51                                       /// 00bc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bc4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bc8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bcc
			 0x80000000,                                                  // -zero                                       /// 00bd0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bd4
			 0x7fc00001,                                                  // signaling NaN                               /// 00bd8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bdc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x7fc00001,                                                  // signaling NaN                               /// 00be4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bf0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bf4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bfc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x3f028f5c,                                                  // 0.51                                        /// 00c04
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c08
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c0c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c20
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c28
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c30
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c34
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c38
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c40
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c50
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c5c
			 0x33333333,                                                  // 4 random values                             /// 00c60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c74
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c78
			 0x4b000000,                                                  // 8388608.0                                   /// 00c7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c88
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x55555555,                                                  // 4 random values                             /// 00c98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c9c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ca0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0xffc00001,                                                  // -signaling NaN                              /// 00cb4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cb8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cbc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cc0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cc8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ccc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cdc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ce0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ce4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cf0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cf8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d04
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d0c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d18
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x3f028f5c,                                                  // 0.51                                        /// 00d38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d3c
			 0x80000000,                                                  // -zero                                       /// 00d40
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d44
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d48
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d50
			 0xbf028f5c,                                                  // -0.51                                       /// 00d54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d5c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d64
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d68
			 0x3f028f5c,                                                  // 0.51                                        /// 00d6c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d80
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d84
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d8c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d90
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d94
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d98
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d9c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00da0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00da8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00db0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00db4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x33333333,                                                  // 4 random values                             /// 00dc0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dc8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dd8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00de0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00de8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00df0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0xffc00001,                                                  // -signaling NaN                              /// 00df8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dfc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e04
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e10
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e18
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e30
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e3c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e40
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e4c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e54
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e64
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e88
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e8c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e9c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x7f800000,                                                  // inf                                         /// 00ea4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ea8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00eac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00eb0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00000000,                                                  // zero                                        /// 00eb8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ebc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ec4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ec8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ecc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ed0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ed8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ee0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ee4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00eec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ef0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ef4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00efc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f08
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f10
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0xffc00001,                                                  // -signaling NaN                              /// 00f18
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f24
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f38
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f5c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f60
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f6c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f70
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f78
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f88
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f9c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fa0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fa4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fa8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fb0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fc4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fd0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fd4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fdc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fe4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fe8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ff4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ff8
			 0x0c600000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00000
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00004
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0000c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00014
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00018
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00020
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00024
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00028
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0002c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00030
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00034
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00038
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0003c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00048
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00054
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00058
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00064
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00068
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00070
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00074
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00078
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0007c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00080
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00084
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00088
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00098
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0009c
			 0x0c700000,                                                  // Leading 1s:                                 /// 000a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00100
			 0x4b000000,                                                  // 8388608.0                                   /// 00104
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00108
			 0x4b000000,                                                  // 8388608.0                                   /// 0010c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00110
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x7f800000,                                                  // inf                                         /// 0011c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00120
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00124
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00128
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0012c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00130
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00134
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00138
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0013c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00144
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0014c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00150
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00158
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00164
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00170
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00174
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0017c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00180
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00184
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00188
			 0xcb000000,                                                  // -8388608.0                                  /// 0018c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00190
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00194
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0019c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 001ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 001b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00204
			 0x0e000001,                                                  // Trailing 1s:                                /// 00208
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0020c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00214
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0021c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00228
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0022c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00234
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0023c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00240
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00248
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0024c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00250
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00254
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00258
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0025c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00264
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0026c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00270
			 0x0e000007,                                                  // Trailing 1s:                                /// 00274
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00278
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00280
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00288
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00290
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00294
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00298
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0029c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x4b000000,                                                  // 8388608.0                                   /// 002ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002b0
			 0x3f028f5c,                                                  // 0.51                                        /// 002b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002c8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002e8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00300
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00304
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00308
			 0x3f028f5c,                                                  // 0.51                                        /// 0030c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00310
			 0x33333333,                                                  // 4 random values                             /// 00314
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00320
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00324
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00328
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0032c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00330
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00334
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00338
			 0x55555555,                                                  // 4 random values                             /// 0033c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00348
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0034c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00350
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00354
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0035c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00360
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00364
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00370
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00378
			 0x0e000007,                                                  // Trailing 1s:                                /// 0037c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00380
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00384
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00388
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00390
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0xffc00001,                                                  // -signaling NaN                              /// 00398
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0xcb000000,                                                  // -8388608.0                                  /// 003a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00400
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00408
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0040c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00410
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00414
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00418
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0041c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00420
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x80011111,                                                  // -9.7958E-41                                 /// 00428
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0042c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00438
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00440
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00448
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0044c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00454
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0045c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00460
			 0x3f028f5c,                                                  // 0.51                                        /// 00464
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00468
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0046c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00470
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00474
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00478
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0047c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00480
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00484
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0048c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0049c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004ac
			 0xff800000,                                                  // -inf                                        /// 004b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00500
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00504
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00508
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00510
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00514
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00518
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00524
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00528
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0052c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00530
			 0x80011111,                                                  // -9.7958E-41                                 /// 00534
			 0x0c780000,                                                  // Leading 1s:                                 /// 00538
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0053c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00540
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00548
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0054c
			 0xbf028f5c,                                                  // -0.51                                       /// 00550
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00554
			 0x0c400000,                                                  // Leading 1s:                                 /// 00558
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x55555555,                                                  // 4 random values                             /// 00560
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00564
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00568
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0056c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00570
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00578
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0057c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00584
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00588
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0058c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00590
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00594
			 0x00000000,                                                  // zero                                        /// 00598
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0059c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 005b0
			 0x80000000,                                                  // -zero                                       /// 005b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 005f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x55555555,                                                  // 4 random values                             /// 00604
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00608
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0060c
			 0xff800000,                                                  // -inf                                        /// 00610
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00614
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0061c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00620
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00624
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00628
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0062c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00630
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00634
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00640
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00648
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00650
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00654
			 0x3f028f5c,                                                  // 0.51                                        /// 00658
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0065c
			 0x3f028f5c,                                                  // 0.51                                        /// 00660
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00664
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00668
			 0xffc00001,                                                  // -signaling NaN                              /// 0066c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00670
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0067c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00680
			 0x0c780000,                                                  // Leading 1s:                                 /// 00684
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00690
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00694
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00698
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x55555555,                                                  // 4 random values                             /// 006cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006d8
			 0xffc00001,                                                  // -signaling NaN                              /// 006dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00704
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00708
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00710
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00720
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00724
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00728
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0072c
			 0xff800000,                                                  // -inf                                        /// 00730
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0073c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00748
			 0x55555555,                                                  // 4 random values                             /// 0074c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00754
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00758
			 0x0c600000,                                                  // Leading 1s:                                 /// 0075c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00764
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00768
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0076c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00770
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00774
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00778
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0077c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00788
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00790
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00798
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0079c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007ac
			 0x00000000,                                                  // zero                                        /// 007b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 007b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 007ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00808
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0080c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00810
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00818
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0081c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00820
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00824
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00828
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00830
			 0x33333333,                                                  // 4 random values                             /// 00834
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00838
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0083c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00840
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00844
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00848
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0084c
			 0x7f800000,                                                  // inf                                         /// 00850
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00854
			 0x33333333,                                                  // 4 random values                             /// 00858
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0085c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00864
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0086c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00870
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x80000000,                                                  // -zero                                       /// 00878
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00884
			 0x0c400000,                                                  // Leading 1s:                                 /// 00888
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00894
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x4b000000,                                                  // 8388608.0                                   /// 008a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00904
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00908
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0090c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00910
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00918
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0091c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00920
			 0x0e000007,                                                  // Trailing 1s:                                /// 00924
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0092c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00930
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00938
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0093c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00950
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00954
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00958
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0095c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00960
			 0x80000000,                                                  // -zero                                       /// 00964
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00968
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00970
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00974
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0097c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00980
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00984
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00988
			 0x55555555,                                                  // 4 random values                             /// 0098c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00990
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00994
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0xcb000000,                                                  // -8388608.0                                  /// 0099c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009ac
			 0x80000000,                                                  // -zero                                       /// 009b0
			 0xffc00001,                                                  // -signaling NaN                              /// 009b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x55555555,                                                  // 4 random values                             /// 009dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 009f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009f4
			 0x3f028f5c,                                                  // 0.51                                        /// 009f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a00
			 0xcb000000,                                                  // -8388608.0                                  /// 00a04
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a14
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a18
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a28
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a34
			 0x55555555,                                                  // 4 random values                             /// 00a38
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a44
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a48
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a54
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a5c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a68
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a78
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a7c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a88
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a90
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aa0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ab4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ab8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00abc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ac0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ad0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ad4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00af0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00af4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00af8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b08
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b30
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b34
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b40
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b44
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b50
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b54
			 0x7f800000,                                                  // inf                                         /// 00b58
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b84
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b9c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ba0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ba4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ba8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bb4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bbc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bcc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bd0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bd4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00be0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00be4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bf4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bf8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bfc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c08
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c0c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c20
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c24
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c34
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c40
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c44
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c48
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c4c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0xff800000,                                                  // -inf                                        /// 00c54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c80
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c84
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ca4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ca8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cb0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cb4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cb8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cbc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cc0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0xffc00001,                                                  // -signaling NaN                              /// 00ccc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cd0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cd4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cdc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ce0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ce4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cf0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cf4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cfc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d18
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d20
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d24
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d28
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d30
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d3c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d48
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d5c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d60
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x7f800000,                                                  // inf                                         /// 00d6c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d98
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d9c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00da0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00da4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00db0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0xcb000000,                                                  // -8388608.0                                  /// 00dc4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dd4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dd8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00de4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00de8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00df4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00df8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dfc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e08
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e14
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e1c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e24
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e28
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e34
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e40
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e50
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e54
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e58
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e64
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e74
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e7c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e84
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e90
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00ea0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ea4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ea8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00eac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00eb8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ebc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ec0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ec8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ecc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ed0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ed4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ed8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ee8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00eec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ef0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00efc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f0c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f68
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f74
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fa0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fa4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fb0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fb4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fbc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fc0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fc4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fc8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fcc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fd4
			 0xcb000000,                                                  // -8388608.0                                  /// 00fd8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fdc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fe0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fe4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fe8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ff0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ff8
			 0x80000040                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00000
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00008
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00010
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00018
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x4b000000,                                                  // 8388608.0                                   /// 00024
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0002c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00030
			 0x4b000000,                                                  // 8388608.0                                   /// 00034
			 0x55555555,                                                  // 4 random values                             /// 00038
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0003c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00044
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00050
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00054
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00058
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00060
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x0c400000,                                                  // Leading 1s:                                 /// 00068
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0006c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00070
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00074
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0007c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0008c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00090
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00094
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00098
			 0x7f800000,                                                  // inf                                         /// 0009c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000a8
			 0x33333333,                                                  // 4 random values                             /// 000ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000b8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0xff800000,                                                  // -inf                                        /// 000d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0e000003,                                                  // Trailing 1s:                                /// 00108
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0010c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00110
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0011c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00120
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00124
			 0x0c780000,                                                  // Leading 1s:                                 /// 00128
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0012c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00134
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0013c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00140
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00144
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0014c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00150
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00154
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00158
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0015c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00160
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00168
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00170
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00178
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0017c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00180
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00184
			 0x0c600000,                                                  // Leading 1s:                                 /// 00188
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0018c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00190
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00194
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00198
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0019c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0xcb000000,                                                  // -8388608.0                                  /// 001bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00204
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0c400000,                                                  // Leading 1s:                                 /// 0020c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00214
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0021c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00228
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0022c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00230
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00240
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00244
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00248
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0024c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00254
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x4b000000,                                                  // 8388608.0                                   /// 0025c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00264
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0026c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00270
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00274
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0027c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00280
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00284
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00288
			 0x0e000007,                                                  // Trailing 1s:                                /// 0028c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00290
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00294
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00298
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0c600000,                                                  // Leading 1s:                                 /// 002a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00300
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00304
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00308
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00310
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0031c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00320
			 0xffc00001,                                                  // -signaling NaN                              /// 00324
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00328
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0032c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00334
			 0x00000000,                                                  // zero                                        /// 00338
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x7fc00001,                                                  // signaling NaN                               /// 00340
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00344
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00348
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00350
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00358
			 0x7fc00001,                                                  // signaling NaN                               /// 0035c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x0c600000,                                                  // Leading 1s:                                 /// 00364
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00368
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00370
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x7fc00001,                                                  // signaling NaN                               /// 00378
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0037c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00380
			 0x0c700000,                                                  // Leading 1s:                                 /// 00384
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00388
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0038c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00390
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x7fc00001,                                                  // signaling NaN                               /// 00398
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x4b000000,                                                  // 8388608.0                                   /// 003a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003a8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 003b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003d4
			 0x55555555,                                                  // 4 random values                             /// 003d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00408
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00410
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00414
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00418
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0041c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00424
			 0x33333333,                                                  // 4 random values                             /// 00428
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00430
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00440
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00444
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00448
			 0xcb000000,                                                  // -8388608.0                                  /// 0044c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00450
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00454
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00458
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0045c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00460
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00468
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0046c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00470
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00474
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0047c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00490
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00498
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0049c
			 0x0c600000,                                                  // Leading 1s:                                 /// 004a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 004c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00500
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00504
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00508
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0050c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00514
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0051c
			 0x80000000,                                                  // -zero                                       /// 00520
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00524
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00528
			 0x7f800000,                                                  // inf                                         /// 0052c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00534
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00538
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0053c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00540
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00544
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00548
			 0xffc00001,                                                  // -signaling NaN                              /// 0054c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00550
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00554
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0055c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0c600000,                                                  // Leading 1s:                                 /// 0056c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00570
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00574
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00578
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0057c
			 0x33333333,                                                  // 4 random values                             /// 00580
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0x0c700000,                                                  // Leading 1s:                                 /// 00588
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00594
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00598
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0059c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005a4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00600
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00608
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0060c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00610
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00614
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00618
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0061c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00620
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x3f028f5c,                                                  // 0.51                                        /// 00628
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00634
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0063c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00640
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00644
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00648
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0064c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00650
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00654
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00658
			 0x7f800000,                                                  // inf                                         /// 0065c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00664
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00668
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0066c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00670
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00674
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00678
			 0xffc00001,                                                  // -signaling NaN                              /// 0067c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00680
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00684
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00688
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0068c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00694
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00698
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 006a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 006cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00700
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0070c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00714
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00718
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00720
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00724
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00738
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0073c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00740
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00748
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00754
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00758
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0075c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00764
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00770
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00778
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00780
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0078c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00790
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00794
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0e000001,                                                  // Trailing 1s:                                /// 0079c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007c4
			 0x4b000000,                                                  // 8388608.0                                   /// 007c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007cc
			 0x33333333,                                                  // 4 random values                             /// 007d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007dc
			 0x00000000,                                                  // zero                                        /// 007e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x00000000,                                                  // zero                                        /// 007f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00804
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00808
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0080c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00814
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00818
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0081c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00828
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0082c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00830
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00834
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00838
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0083c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00840
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00844
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00848
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0084c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00850
			 0xcb000000,                                                  // -8388608.0                                  /// 00854
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00858
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0085c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00860
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0086c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00874
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00878
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0087c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00880
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00884
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00888
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0088c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00894
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0089c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 008d0
			 0x33333333,                                                  // 4 random values                             /// 008d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0xcb000000,                                                  // -8388608.0                                  /// 008dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008e8
			 0xcb000000,                                                  // -8388608.0                                  /// 008ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00900
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00910
			 0x00011111,                                                  // 9.7958E-41                                  /// 00914
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00918
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00924
			 0x3f028f5c,                                                  // 0.51                                        /// 00928
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0092c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00930
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00938
			 0x0c600000,                                                  // Leading 1s:                                 /// 0093c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00940
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00944
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00948
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0094c
			 0x7f800000,                                                  // inf                                         /// 00950
			 0x0c400000,                                                  // Leading 1s:                                 /// 00954
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00958
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00960
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00964
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00968
			 0x80000000,                                                  // -zero                                       /// 0096c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00970
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00974
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00978
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0097c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00980
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00984
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00990
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00994
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0099c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 009a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 009e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a00
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a08
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a1c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a20
			 0xcb000000,                                                  // -8388608.0                                  /// 00a24
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a28
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a30
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a3c
			 0x80000000,                                                  // -zero                                       /// 00a40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a50
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a58
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a5c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a60
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a74
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a78
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x80000000,                                                  // -zero                                       /// 00a84
			 0x3f028f5c,                                                  // 0.51                                        /// 00a88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a8c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a90
			 0x00000000,                                                  // zero                                        /// 00a94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a98
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00aa4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aa8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ab4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ab8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00abc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ac0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ac4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ac8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00acc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ad0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ad4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00adc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ae0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ae4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ae8
			 0x7fc00001,                                                  // signaling NaN                               /// 00aec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00af0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00af8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b00
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b08
			 0x4b000000,                                                  // 8388608.0                                   /// 00b0c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b10
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b14
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b20
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b2c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b40
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b44
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b4c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b54
			 0x7f800000,                                                  // inf                                         /// 00b58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b5c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b80
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b90
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x80000000,                                                  // -zero                                       /// 00b9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ba4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bac
			 0x4b000000,                                                  // 8388608.0                                   /// 00bb0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bb4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bb8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bc0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bc4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bc8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bcc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bd0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bd8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00be0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00be4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00be8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bf0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bf4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bfc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c00
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c18
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x00000000,                                                  // zero                                        /// 00c20
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c34
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c40
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c48
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c50
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c54
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c70
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c78
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c7c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c80
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c84
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c94
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c9c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x80000000,                                                  // -zero                                       /// 00ca4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ca8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cac
			 0x80000000,                                                  // -zero                                       /// 00cb0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cb4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cc0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cc4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cc8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00000000,                                                  // zero                                        /// 00cd4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ce0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ce4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ce8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cf0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cf8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cfc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d00
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x7f800000,                                                  // inf                                         /// 00d0c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d14
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d18
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d2c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d3c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d40
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d50
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d54
			 0x4b000000,                                                  // 8388608.0                                   /// 00d58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d6c
			 0xff800000,                                                  // -inf                                        /// 00d70
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d84
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d88
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d8c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d90
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00da4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00da8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00db4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00db8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x00000000,                                                  // zero                                        /// 00dc8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dcc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0x3f028f5c,                                                  // 0.51                                        /// 00dd4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dd8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ddc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00de0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00de4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00de8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00df8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e10
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e14
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e18
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0xffc00001,                                                  // -signaling NaN                              /// 00e24
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e2c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e30
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e4c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e60
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e64
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e70
			 0x4b000000,                                                  // 8388608.0                                   /// 00e74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e8c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e98
			 0xbf028f5c,                                                  // -0.51                                       /// 00e9c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ea0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ea8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00eac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00eb0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ec0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ec4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ecc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ed4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x80000000,                                                  // -zero                                       /// 00edc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ee0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ee4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00eec
			 0xffc00001,                                                  // -signaling NaN                              /// 00ef0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ef8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00efc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f00
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f04
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x80000000,                                                  // -zero                                       /// 00f1c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f20
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f24
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f28
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f2c
			 0x55555555,                                                  // 4 random values                             /// 00f30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f4c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f50
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f58
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f68
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f6c
			 0x55555555,                                                  // 4 random values                             /// 00f70
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f74
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f7c
			 0xff800000,                                                  // -inf                                        /// 00f80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f84
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f98
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f9c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fa0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fa4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fb0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fb4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fbc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fc8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fcc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fd0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fd4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fdc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fe4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fe8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ff0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x7f7ffffe // max norm - 1ulp                               // SP +ve numbers                              /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x15c2a5e2, /// 0x0
			 0x87f37981, /// 0x4
			 0x32c33564, /// 0x8
			 0x654d2994, /// 0xc
			 0x49ea2172, /// 0x10
			 0x897c880a, /// 0x14
			 0x6c6e5e8a, /// 0x18
			 0x26808117, /// 0x1c
			 0xc3dd527d, /// 0x20
			 0xd031106a, /// 0x24
			 0xca7bb4ec, /// 0x28
			 0xa2eedff5, /// 0x2c
			 0x545c2615, /// 0x30
			 0x50a51747, /// 0x34
			 0xf40e0840, /// 0x38
			 0x53f66f48, /// 0x3c
			 0x45ccbb16, /// 0x40
			 0x0c31a402, /// 0x44
			 0x3c331633, /// 0x48
			 0x50d47dce, /// 0x4c
			 0x3f050940, /// 0x50
			 0x18db1f70, /// 0x54
			 0xe35ddb39, /// 0x58
			 0x2b457cbc, /// 0x5c
			 0x95f6b15d, /// 0x60
			 0x13343581, /// 0x64
			 0xa7a006e8, /// 0x68
			 0xbff47fc5, /// 0x6c
			 0xf7c2d3da, /// 0x70
			 0x7a176e5d, /// 0x74
			 0x3e3f22bb, /// 0x78
			 0x8a3c8613, /// 0x7c
			 0xa4dd937e, /// 0x80
			 0x3fcb475c, /// 0x84
			 0x5e9b401a, /// 0x88
			 0xd8065c37, /// 0x8c
			 0x6b9cd78a, /// 0x90
			 0x078bd627, /// 0x94
			 0x515fbb21, /// 0x98
			 0x42848db8, /// 0x9c
			 0x44d93611, /// 0xa0
			 0xb194eab3, /// 0xa4
			 0x0510b8fd, /// 0xa8
			 0xcfced9bd, /// 0xac
			 0xeb17b823, /// 0xb0
			 0x17d734ae, /// 0xb4
			 0x50c2d0ae, /// 0xb8
			 0x278699de, /// 0xbc
			 0xd77e5374, /// 0xc0
			 0xeb16b335, /// 0xc4
			 0x92c88e06, /// 0xc8
			 0x41930a84, /// 0xcc
			 0x8e74aaa6, /// 0xd0
			 0x57065943, /// 0xd4
			 0x30f18649, /// 0xd8
			 0xafbf5ea6, /// 0xdc
			 0xe55f47cd, /// 0xe0
			 0x52d72e1b, /// 0xe4
			 0x249839ad, /// 0xe8
			 0x619de336, /// 0xec
			 0xa809f5df, /// 0xf0
			 0x4729375b, /// 0xf4
			 0x962849d6, /// 0xf8
			 0x390e7acc, /// 0xfc
			 0x5348bf5d, /// 0x100
			 0xa2d2ec9e, /// 0x104
			 0x741c15b4, /// 0x108
			 0xaa5a62d1, /// 0x10c
			 0x9255078a, /// 0x110
			 0x8b274d4a, /// 0x114
			 0x41d4f38b, /// 0x118
			 0x4ac547bd, /// 0x11c
			 0x1c47f07d, /// 0x120
			 0x1d006679, /// 0x124
			 0x413a4835, /// 0x128
			 0x4633882e, /// 0x12c
			 0x278cfc0f, /// 0x130
			 0x5000fc2d, /// 0x134
			 0xd6eda2be, /// 0x138
			 0x215279b8, /// 0x13c
			 0xe8e6daad, /// 0x140
			 0x346db3d9, /// 0x144
			 0x59f470d0, /// 0x148
			 0xdcda22e1, /// 0x14c
			 0xe1e4e4e0, /// 0x150
			 0x70cc6578, /// 0x154
			 0xb9c86ab1, /// 0x158
			 0xc7234fed, /// 0x15c
			 0x7ca8901b, /// 0x160
			 0xd3474b85, /// 0x164
			 0x0ba02929, /// 0x168
			 0x20e65a9f, /// 0x16c
			 0x58080355, /// 0x170
			 0x3d423512, /// 0x174
			 0x812d88ed, /// 0x178
			 0x02b2a476, /// 0x17c
			 0xc44d0d31, /// 0x180
			 0xd3f73ad9, /// 0x184
			 0x32a968f1, /// 0x188
			 0xc88ab0cf, /// 0x18c
			 0xe6590ab2, /// 0x190
			 0xb2d87f96, /// 0x194
			 0x7dbd0484, /// 0x198
			 0x09e671c0, /// 0x19c
			 0x3de832f6, /// 0x1a0
			 0x75ff94f2, /// 0x1a4
			 0x6ede3aff, /// 0x1a8
			 0xdd19b875, /// 0x1ac
			 0x9b05b250, /// 0x1b0
			 0xc1acb5f9, /// 0x1b4
			 0x763ae1f9, /// 0x1b8
			 0xa525a80a, /// 0x1bc
			 0x12cd0720, /// 0x1c0
			 0x6d8df3a2, /// 0x1c4
			 0x7c97e3b0, /// 0x1c8
			 0xe37bcbbf, /// 0x1cc
			 0xf60ae7c2, /// 0x1d0
			 0x5b0a8d82, /// 0x1d4
			 0x23cea94d, /// 0x1d8
			 0xb8e04c44, /// 0x1dc
			 0x617d2bb6, /// 0x1e0
			 0xe1b24af5, /// 0x1e4
			 0x56d4b9ba, /// 0x1e8
			 0xec12ffbf, /// 0x1ec
			 0xd56199c4, /// 0x1f0
			 0xa39599ba, /// 0x1f4
			 0x30e50994, /// 0x1f8
			 0x93691b3d, /// 0x1fc
			 0x0f3a4ffc, /// 0x200
			 0x55c55ee5, /// 0x204
			 0x40f727c2, /// 0x208
			 0x7b15a5d9, /// 0x20c
			 0xc47cc832, /// 0x210
			 0x92a77391, /// 0x214
			 0x80c1b13e, /// 0x218
			 0xaa38ee4b, /// 0x21c
			 0x61cbf281, /// 0x220
			 0xe4169b15, /// 0x224
			 0x0df25dfe, /// 0x228
			 0x184ece1b, /// 0x22c
			 0x4bb980e5, /// 0x230
			 0xc50efb03, /// 0x234
			 0xab4afdfb, /// 0x238
			 0x166aaeb3, /// 0x23c
			 0xc461db1b, /// 0x240
			 0x54df8325, /// 0x244
			 0x134e0acf, /// 0x248
			 0x06493422, /// 0x24c
			 0xc8e87f3c, /// 0x250
			 0x0a0b34d1, /// 0x254
			 0x579fb658, /// 0x258
			 0xff9d3e82, /// 0x25c
			 0xb2758e15, /// 0x260
			 0xc98d5d2c, /// 0x264
			 0x1ad23f10, /// 0x268
			 0x3203d6c9, /// 0x26c
			 0x638def25, /// 0x270
			 0xf12ae792, /// 0x274
			 0xfe303be1, /// 0x278
			 0xbab7f6a7, /// 0x27c
			 0xff85a134, /// 0x280
			 0xa315b221, /// 0x284
			 0x2e501005, /// 0x288
			 0x1bb09c33, /// 0x28c
			 0x19e97bf4, /// 0x290
			 0x09d816c9, /// 0x294
			 0x87920255, /// 0x298
			 0x02c2158d, /// 0x29c
			 0x590c9f98, /// 0x2a0
			 0x4a086aa8, /// 0x2a4
			 0x8c626d77, /// 0x2a8
			 0xa24e33eb, /// 0x2ac
			 0x6aa8fa2c, /// 0x2b0
			 0x2ea995f8, /// 0x2b4
			 0x64453ba0, /// 0x2b8
			 0x353a60f0, /// 0x2bc
			 0x4e79d1cf, /// 0x2c0
			 0xa941a473, /// 0x2c4
			 0xb7c6ecb7, /// 0x2c8
			 0x11b2429f, /// 0x2cc
			 0x7e3819bd, /// 0x2d0
			 0x20eaf742, /// 0x2d4
			 0x6a81cd21, /// 0x2d8
			 0xe2187b10, /// 0x2dc
			 0x25d6a5c3, /// 0x2e0
			 0xdaea1481, /// 0x2e4
			 0x46ed4f3b, /// 0x2e8
			 0x57f4f441, /// 0x2ec
			 0x2dfc200c, /// 0x2f0
			 0x0bd88e7c, /// 0x2f4
			 0x9bdfb455, /// 0x2f8
			 0x9388f529, /// 0x2fc
			 0xc0146e8d, /// 0x300
			 0x8da2284f, /// 0x304
			 0xf450aef0, /// 0x308
			 0xd6816ee1, /// 0x30c
			 0x3e0e9871, /// 0x310
			 0x8df5ac55, /// 0x314
			 0xb24dd579, /// 0x318
			 0xb684f5ff, /// 0x31c
			 0xb138a40d, /// 0x320
			 0x0fedbb7d, /// 0x324
			 0xd942d82d, /// 0x328
			 0xfd1bdf77, /// 0x32c
			 0x284cde62, /// 0x330
			 0x4b1b0613, /// 0x334
			 0xcfd12641, /// 0x338
			 0x2ea44301, /// 0x33c
			 0xc3cbe86c, /// 0x340
			 0x4a315c1e, /// 0x344
			 0xacdf1785, /// 0x348
			 0xabdf87e1, /// 0x34c
			 0xcb359c22, /// 0x350
			 0xbd0cfdb1, /// 0x354
			 0x5a35e2b0, /// 0x358
			 0x95a23d03, /// 0x35c
			 0x8c21764c, /// 0x360
			 0x2aec4ed9, /// 0x364
			 0xf8779581, /// 0x368
			 0x8cfc7289, /// 0x36c
			 0xf388b75c, /// 0x370
			 0x7ef93218, /// 0x374
			 0x24409c88, /// 0x378
			 0xd531e34e, /// 0x37c
			 0x84a66cf8, /// 0x380
			 0x7111376c, /// 0x384
			 0x81ceebaa, /// 0x388
			 0x12cd9316, /// 0x38c
			 0xc42f5eec, /// 0x390
			 0x092f6c4f, /// 0x394
			 0xf9e9e77e, /// 0x398
			 0x862fad14, /// 0x39c
			 0x670e31fa, /// 0x3a0
			 0x92aa45da, /// 0x3a4
			 0x1532e048, /// 0x3a8
			 0xfe786d65, /// 0x3ac
			 0xe91bfbe5, /// 0x3b0
			 0x88bbcb97, /// 0x3b4
			 0x47d5fae3, /// 0x3b8
			 0x0b4ea9f0, /// 0x3bc
			 0x70bb3bb8, /// 0x3c0
			 0x3a0d7b3a, /// 0x3c4
			 0x605949d7, /// 0x3c8
			 0x347ca3aa, /// 0x3cc
			 0xed220728, /// 0x3d0
			 0x2b7d1cc1, /// 0x3d4
			 0x1875400d, /// 0x3d8
			 0x02ee5eda, /// 0x3dc
			 0x6b4eb615, /// 0x3e0
			 0x5110f3cc, /// 0x3e4
			 0x0efd6056, /// 0x3e8
			 0x7a062c84, /// 0x3ec
			 0x3cdf5068, /// 0x3f0
			 0x10effc33, /// 0x3f4
			 0x5f650190, /// 0x3f8
			 0x7b833f94, /// 0x3fc
			 0x487ed99e, /// 0x400
			 0x81bd0df9, /// 0x404
			 0x53315101, /// 0x408
			 0xcdf4da98, /// 0x40c
			 0x0dfd8217, /// 0x410
			 0x9dec9896, /// 0x414
			 0x8b2db025, /// 0x418
			 0xde796c32, /// 0x41c
			 0x0939a022, /// 0x420
			 0x6249b8b9, /// 0x424
			 0x4b233365, /// 0x428
			 0x7b4d6589, /// 0x42c
			 0x58c45acb, /// 0x430
			 0x20e33cc6, /// 0x434
			 0x20d711ba, /// 0x438
			 0x0803c79d, /// 0x43c
			 0x7ab15508, /// 0x440
			 0x5add2315, /// 0x444
			 0x44c5ee8e, /// 0x448
			 0x5d3dde87, /// 0x44c
			 0xc349b417, /// 0x450
			 0xfcefc56d, /// 0x454
			 0x55b9e7b8, /// 0x458
			 0x7a9e1d2d, /// 0x45c
			 0x44b35b9e, /// 0x460
			 0x4571735c, /// 0x464
			 0x5fcd2bd3, /// 0x468
			 0x02ad1b50, /// 0x46c
			 0xf5bcba7e, /// 0x470
			 0x37601db3, /// 0x474
			 0xeecbd510, /// 0x478
			 0x66a8e365, /// 0x47c
			 0x06b05c88, /// 0x480
			 0xc7cd55a1, /// 0x484
			 0xe3b98f42, /// 0x488
			 0xeac6c87e, /// 0x48c
			 0xc6dfb047, /// 0x490
			 0xa47a9b0c, /// 0x494
			 0x1b69f96b, /// 0x498
			 0x68d4c6b7, /// 0x49c
			 0x0b48b00a, /// 0x4a0
			 0x6f4b2b3d, /// 0x4a4
			 0xfc329b4a, /// 0x4a8
			 0xbdb4c183, /// 0x4ac
			 0x39b77047, /// 0x4b0
			 0xdba65253, /// 0x4b4
			 0xccb90cef, /// 0x4b8
			 0xff0c118a, /// 0x4bc
			 0x9fe93c10, /// 0x4c0
			 0xebb91530, /// 0x4c4
			 0xad30e0ed, /// 0x4c8
			 0x3dc885cf, /// 0x4cc
			 0x2a1144d5, /// 0x4d0
			 0x40e43799, /// 0x4d4
			 0xb1c0cb4d, /// 0x4d8
			 0x93a9c784, /// 0x4dc
			 0x41ae5007, /// 0x4e0
			 0x6bdb927f, /// 0x4e4
			 0xb6a05813, /// 0x4e8
			 0x4cd16523, /// 0x4ec
			 0xed691951, /// 0x4f0
			 0xaeeec1e9, /// 0x4f4
			 0x3a50fca3, /// 0x4f8
			 0x9c0d7416, /// 0x4fc
			 0xd6b30cb0, /// 0x500
			 0x95163c30, /// 0x504
			 0xd81eca61, /// 0x508
			 0xb68af32a, /// 0x50c
			 0x7be5fb42, /// 0x510
			 0x1c4399dc, /// 0x514
			 0xa53b89dd, /// 0x518
			 0x378d242a, /// 0x51c
			 0xc8aaab56, /// 0x520
			 0x8793a71a, /// 0x524
			 0xc7ace248, /// 0x528
			 0x2a036ed4, /// 0x52c
			 0x0250151f, /// 0x530
			 0xda346a8d, /// 0x534
			 0x19843170, /// 0x538
			 0xca3847de, /// 0x53c
			 0xa8c49e7a, /// 0x540
			 0x2da80bf3, /// 0x544
			 0xf9313dd5, /// 0x548
			 0x7ac1bbd5, /// 0x54c
			 0xaeff13eb, /// 0x550
			 0x9f16dc55, /// 0x554
			 0x2e7abf1a, /// 0x558
			 0x109cc96a, /// 0x55c
			 0x60d0e42d, /// 0x560
			 0x85f06f69, /// 0x564
			 0x2d579c97, /// 0x568
			 0xe2a9a880, /// 0x56c
			 0x7d3ea19a, /// 0x570
			 0x82af4228, /// 0x574
			 0x02981478, /// 0x578
			 0x62323eeb, /// 0x57c
			 0x984797dc, /// 0x580
			 0x698e70de, /// 0x584
			 0xef4ef6f7, /// 0x588
			 0xb26d1073, /// 0x58c
			 0x98e55bb8, /// 0x590
			 0xe6e24883, /// 0x594
			 0xaad430d4, /// 0x598
			 0x66d5129d, /// 0x59c
			 0x1432f148, /// 0x5a0
			 0xd79ef63a, /// 0x5a4
			 0xf67edadf, /// 0x5a8
			 0x24583b63, /// 0x5ac
			 0x87eab589, /// 0x5b0
			 0xaf7caf42, /// 0x5b4
			 0xa1e46ecc, /// 0x5b8
			 0xcdda2691, /// 0x5bc
			 0x49057c88, /// 0x5c0
			 0x4541cb7b, /// 0x5c4
			 0x3f316334, /// 0x5c8
			 0x0c00676d, /// 0x5cc
			 0x3248ec02, /// 0x5d0
			 0x1a087331, /// 0x5d4
			 0x36132647, /// 0x5d8
			 0x56108f7e, /// 0x5dc
			 0x1096cb15, /// 0x5e0
			 0xcedfd8f2, /// 0x5e4
			 0x4ad5974f, /// 0x5e8
			 0x4d53dcb0, /// 0x5ec
			 0xd857b121, /// 0x5f0
			 0x7f7503a1, /// 0x5f4
			 0x758b4629, /// 0x5f8
			 0x46312020, /// 0x5fc
			 0x7ffef14b, /// 0x600
			 0xb58f74cf, /// 0x604
			 0xa3b569a3, /// 0x608
			 0xe9eb84ce, /// 0x60c
			 0xc90fcd03, /// 0x610
			 0x1c79aa3a, /// 0x614
			 0x407a7d92, /// 0x618
			 0xf98bae03, /// 0x61c
			 0x98bedfd7, /// 0x620
			 0xc8c89730, /// 0x624
			 0xa4644750, /// 0x628
			 0x5982288d, /// 0x62c
			 0x2a013780, /// 0x630
			 0x8e6a6b20, /// 0x634
			 0x46f8544d, /// 0x638
			 0xadd79cb9, /// 0x63c
			 0x07d92194, /// 0x640
			 0x8eaa9953, /// 0x644
			 0x54cdb777, /// 0x648
			 0x5b00c246, /// 0x64c
			 0x7a947d81, /// 0x650
			 0x9b2c030f, /// 0x654
			 0x9276eace, /// 0x658
			 0x395573d8, /// 0x65c
			 0x884a6f7c, /// 0x660
			 0xeda1456f, /// 0x664
			 0x774d790b, /// 0x668
			 0x75ce8597, /// 0x66c
			 0x6244cc02, /// 0x670
			 0x1994573d, /// 0x674
			 0x76b53202, /// 0x678
			 0x6cda97dc, /// 0x67c
			 0xf7864216, /// 0x680
			 0x7184c958, /// 0x684
			 0xefb3fe06, /// 0x688
			 0x42114aac, /// 0x68c
			 0x49304a32, /// 0x690
			 0xd0f21f6b, /// 0x694
			 0x40195d32, /// 0x698
			 0xc3a354e2, /// 0x69c
			 0xe3d20c03, /// 0x6a0
			 0xd27a8b15, /// 0x6a4
			 0x41abf894, /// 0x6a8
			 0x0375c1eb, /// 0x6ac
			 0x9dcc8cd8, /// 0x6b0
			 0x8096bba3, /// 0x6b4
			 0x51a7a708, /// 0x6b8
			 0x69d47d60, /// 0x6bc
			 0x02669309, /// 0x6c0
			 0x40bda872, /// 0x6c4
			 0x9da8b625, /// 0x6c8
			 0xfb089ea4, /// 0x6cc
			 0x0e196f2b, /// 0x6d0
			 0x8f75bd09, /// 0x6d4
			 0x35c1618c, /// 0x6d8
			 0xa927d02f, /// 0x6dc
			 0x11d444fd, /// 0x6e0
			 0xf576cdfb, /// 0x6e4
			 0x060e16e4, /// 0x6e8
			 0x6fd33443, /// 0x6ec
			 0x6e98039b, /// 0x6f0
			 0xb209dfb4, /// 0x6f4
			 0xfb3a31dc, /// 0x6f8
			 0xb317eb3b, /// 0x6fc
			 0x60dce89c, /// 0x700
			 0x48523e34, /// 0x704
			 0xd26f4934, /// 0x708
			 0xea7eba8d, /// 0x70c
			 0x8de102fe, /// 0x710
			 0xdd14a03c, /// 0x714
			 0x82831bf2, /// 0x718
			 0x53965a85, /// 0x71c
			 0x7e0dec41, /// 0x720
			 0x0b06789b, /// 0x724
			 0xafd574c6, /// 0x728
			 0xb3dda07c, /// 0x72c
			 0x7d7cff6d, /// 0x730
			 0x71e482e2, /// 0x734
			 0xed479a57, /// 0x738
			 0xecbe343d, /// 0x73c
			 0x1c5d8a97, /// 0x740
			 0xfcd844c0, /// 0x744
			 0x62eb3fe6, /// 0x748
			 0x29a9ffcd, /// 0x74c
			 0xdaa698e0, /// 0x750
			 0x9d1e5776, /// 0x754
			 0x4b7abd5c, /// 0x758
			 0xd8ce7820, /// 0x75c
			 0xab6a8b90, /// 0x760
			 0x163da6d7, /// 0x764
			 0x3358e29b, /// 0x768
			 0x0208ad96, /// 0x76c
			 0x59c4a63d, /// 0x770
			 0xdc970e4c, /// 0x774
			 0x20149cff, /// 0x778
			 0x148286ee, /// 0x77c
			 0x4b9b6dca, /// 0x780
			 0x325ee62d, /// 0x784
			 0xe18059dc, /// 0x788
			 0x1804489c, /// 0x78c
			 0x9bfa8a78, /// 0x790
			 0x61b04da2, /// 0x794
			 0x18d1e63d, /// 0x798
			 0x25394edc, /// 0x79c
			 0x4ceb10a5, /// 0x7a0
			 0x9eb07ada, /// 0x7a4
			 0xbf487915, /// 0x7a8
			 0x771db087, /// 0x7ac
			 0x7174d902, /// 0x7b0
			 0x518e9f49, /// 0x7b4
			 0x950032ef, /// 0x7b8
			 0x97d5d7ad, /// 0x7bc
			 0xabf7c523, /// 0x7c0
			 0x885e79ba, /// 0x7c4
			 0xcc3320b1, /// 0x7c8
			 0xb2f78211, /// 0x7cc
			 0x5f6dce89, /// 0x7d0
			 0xca4ac34c, /// 0x7d4
			 0x8527d13a, /// 0x7d8
			 0x571696a0, /// 0x7dc
			 0x0a2e961d, /// 0x7e0
			 0x21db0403, /// 0x7e4
			 0x7e859fc3, /// 0x7e8
			 0x55e9927b, /// 0x7ec
			 0x5454d486, /// 0x7f0
			 0x162ea126, /// 0x7f4
			 0x20fef422, /// 0x7f8
			 0x49d83278, /// 0x7fc
			 0x29987904, /// 0x800
			 0xa54b89b2, /// 0x804
			 0x471af81b, /// 0x808
			 0x5a514f44, /// 0x80c
			 0xab3b9f68, /// 0x810
			 0xd3338706, /// 0x814
			 0x3d45d0ad, /// 0x818
			 0xa8057100, /// 0x81c
			 0x28941afd, /// 0x820
			 0xa75188c8, /// 0x824
			 0x58230d8d, /// 0x828
			 0x21fc531c, /// 0x82c
			 0x4bec383c, /// 0x830
			 0xc1c7adf1, /// 0x834
			 0x4ba9983e, /// 0x838
			 0xfc21ce21, /// 0x83c
			 0x69b45ad8, /// 0x840
			 0x3200d693, /// 0x844
			 0x1f3ded9a, /// 0x848
			 0x0e0dd8b2, /// 0x84c
			 0xf13c3481, /// 0x850
			 0x402fb92d, /// 0x854
			 0xc77dec0f, /// 0x858
			 0xaf2e8ed1, /// 0x85c
			 0x4eb4ba33, /// 0x860
			 0x391d49b6, /// 0x864
			 0x9cabf180, /// 0x868
			 0xf716f36b, /// 0x86c
			 0x9ee43896, /// 0x870
			 0x01d88198, /// 0x874
			 0x99f3c44c, /// 0x878
			 0x478c3737, /// 0x87c
			 0x5979c7eb, /// 0x880
			 0x3a232696, /// 0x884
			 0x2139deae, /// 0x888
			 0x33da65c5, /// 0x88c
			 0xac17e50f, /// 0x890
			 0x568bba5e, /// 0x894
			 0x5e4d0e4b, /// 0x898
			 0xb1bdf1d9, /// 0x89c
			 0x140c5447, /// 0x8a0
			 0x4410c224, /// 0x8a4
			 0xb5a8f7b6, /// 0x8a8
			 0x2579cc7d, /// 0x8ac
			 0xe406178b, /// 0x8b0
			 0x030ef25c, /// 0x8b4
			 0x6c5c8377, /// 0x8b8
			 0xc11e90b9, /// 0x8bc
			 0xf2516418, /// 0x8c0
			 0x5742b577, /// 0x8c4
			 0xb75e8edf, /// 0x8c8
			 0x81530167, /// 0x8cc
			 0x922d316f, /// 0x8d0
			 0x61816838, /// 0x8d4
			 0x2c87530c, /// 0x8d8
			 0x2df938fa, /// 0x8dc
			 0xae478ee2, /// 0x8e0
			 0x37896526, /// 0x8e4
			 0x10505768, /// 0x8e8
			 0x2d458e23, /// 0x8ec
			 0x552ab437, /// 0x8f0
			 0xabf08338, /// 0x8f4
			 0x56f47bc3, /// 0x8f8
			 0x7f7ef3ce, /// 0x8fc
			 0x0a43c6be, /// 0x900
			 0x0b4e6013, /// 0x904
			 0xdf735fd8, /// 0x908
			 0xda248447, /// 0x90c
			 0x109aca19, /// 0x910
			 0x1363ea81, /// 0x914
			 0x842ade3d, /// 0x918
			 0x350af75b, /// 0x91c
			 0x624c972c, /// 0x920
			 0x4a30e87d, /// 0x924
			 0x3721621d, /// 0x928
			 0x4bbee68b, /// 0x92c
			 0x65e5aa13, /// 0x930
			 0xdcd6fa92, /// 0x934
			 0x92f933a6, /// 0x938
			 0xae22ebad, /// 0x93c
			 0x58123967, /// 0x940
			 0x6aea722d, /// 0x944
			 0x339b0378, /// 0x948
			 0x3c85ddd5, /// 0x94c
			 0x3f6928f4, /// 0x950
			 0x02e66a13, /// 0x954
			 0x260b8f72, /// 0x958
			 0xbc071581, /// 0x95c
			 0x24319d28, /// 0x960
			 0xdb0b12a5, /// 0x964
			 0x4357141c, /// 0x968
			 0x39861ed6, /// 0x96c
			 0xbc8a7377, /// 0x970
			 0xcc16e2d3, /// 0x974
			 0x545f4ca9, /// 0x978
			 0xb598f7a0, /// 0x97c
			 0x498db2ad, /// 0x980
			 0xbdaf8a40, /// 0x984
			 0x778b6d61, /// 0x988
			 0x81b6b260, /// 0x98c
			 0x5299af23, /// 0x990
			 0x825accae, /// 0x994
			 0x97a9891c, /// 0x998
			 0x24050a70, /// 0x99c
			 0x20a499f4, /// 0x9a0
			 0x4e60b307, /// 0x9a4
			 0xa2d20794, /// 0x9a8
			 0x2bb774c7, /// 0x9ac
			 0x3014946f, /// 0x9b0
			 0x9e5ffaf1, /// 0x9b4
			 0xb5526219, /// 0x9b8
			 0x5fa1ad98, /// 0x9bc
			 0xf57e1d3b, /// 0x9c0
			 0xa687e553, /// 0x9c4
			 0xff9a056f, /// 0x9c8
			 0xa08de279, /// 0x9cc
			 0x2ae1ff55, /// 0x9d0
			 0x775fc298, /// 0x9d4
			 0x8aae516f, /// 0x9d8
			 0xf3ef81fb, /// 0x9dc
			 0x1d8a1857, /// 0x9e0
			 0x3522d463, /// 0x9e4
			 0x015653e8, /// 0x9e8
			 0x371cb530, /// 0x9ec
			 0xcf901b1f, /// 0x9f0
			 0xc5cf674d, /// 0x9f4
			 0x5209454f, /// 0x9f8
			 0x6704521b, /// 0x9fc
			 0x4710bdaf, /// 0xa00
			 0x3ecdae81, /// 0xa04
			 0xc5cebc97, /// 0xa08
			 0xed236c59, /// 0xa0c
			 0xe241f998, /// 0xa10
			 0x61e67762, /// 0xa14
			 0x9b13a9c4, /// 0xa18
			 0x79f10a50, /// 0xa1c
			 0x50bd9b5c, /// 0xa20
			 0xb2cdf126, /// 0xa24
			 0x8125baa6, /// 0xa28
			 0x09a174d6, /// 0xa2c
			 0x2fadfe4a, /// 0xa30
			 0x8bdc560c, /// 0xa34
			 0xa4a756e0, /// 0xa38
			 0xa9cd74d0, /// 0xa3c
			 0x2cfbc65a, /// 0xa40
			 0x815dc75a, /// 0xa44
			 0x69f6bc8d, /// 0xa48
			 0x5eea6a74, /// 0xa4c
			 0x38187e71, /// 0xa50
			 0xfbcf528f, /// 0xa54
			 0xe3126dd1, /// 0xa58
			 0x61692bb1, /// 0xa5c
			 0xe56fc123, /// 0xa60
			 0x7b51b33c, /// 0xa64
			 0x2fe7d8c3, /// 0xa68
			 0xf5c47e69, /// 0xa6c
			 0xce67173c, /// 0xa70
			 0x94c220bb, /// 0xa74
			 0x85f81454, /// 0xa78
			 0xc3002ae8, /// 0xa7c
			 0xef77784b, /// 0xa80
			 0xd3387f56, /// 0xa84
			 0x1f11024f, /// 0xa88
			 0x9820925c, /// 0xa8c
			 0x0e7bbaa8, /// 0xa90
			 0x9b061ef7, /// 0xa94
			 0x9e667c5f, /// 0xa98
			 0x00325345, /// 0xa9c
			 0x4af35be0, /// 0xaa0
			 0x1ac35527, /// 0xaa4
			 0x1dd08f6f, /// 0xaa8
			 0x41555885, /// 0xaac
			 0x31f3f443, /// 0xab0
			 0x23894aef, /// 0xab4
			 0x97a98f79, /// 0xab8
			 0x6e55e30d, /// 0xabc
			 0x13c54d85, /// 0xac0
			 0x07d1399f, /// 0xac4
			 0x05ff033e, /// 0xac8
			 0x92b6e604, /// 0xacc
			 0x385d47e0, /// 0xad0
			 0x347b8046, /// 0xad4
			 0xb825d53c, /// 0xad8
			 0x4b7ce3e7, /// 0xadc
			 0xe3767d0f, /// 0xae0
			 0xa4966a93, /// 0xae4
			 0x7ceaf397, /// 0xae8
			 0x46905f1a, /// 0xaec
			 0x4957b9e7, /// 0xaf0
			 0xc5571aaf, /// 0xaf4
			 0x81a7a230, /// 0xaf8
			 0xe643a8ae, /// 0xafc
			 0xda06f2a8, /// 0xb00
			 0xeb9ab6d8, /// 0xb04
			 0x73445588, /// 0xb08
			 0x95043519, /// 0xb0c
			 0x640ec6d5, /// 0xb10
			 0xe434b8e9, /// 0xb14
			 0xa7039ce6, /// 0xb18
			 0xc4dfad6c, /// 0xb1c
			 0x4b918d1a, /// 0xb20
			 0x782d10c4, /// 0xb24
			 0x326151a1, /// 0xb28
			 0xe03014fb, /// 0xb2c
			 0x37c65f71, /// 0xb30
			 0x1898b16e, /// 0xb34
			 0xe0f3532b, /// 0xb38
			 0x1c00303b, /// 0xb3c
			 0xd32fad76, /// 0xb40
			 0xb501b1bd, /// 0xb44
			 0xcf7486b8, /// 0xb48
			 0xaff52ef7, /// 0xb4c
			 0xf8781786, /// 0xb50
			 0x2bc7cfbd, /// 0xb54
			 0xf298d702, /// 0xb58
			 0x8b748528, /// 0xb5c
			 0x05a296af, /// 0xb60
			 0x46091288, /// 0xb64
			 0xd18d660b, /// 0xb68
			 0x2e782f25, /// 0xb6c
			 0x614a5e6e, /// 0xb70
			 0xe29aa636, /// 0xb74
			 0x5d0106ee, /// 0xb78
			 0x5a7f12af, /// 0xb7c
			 0xa4badd07, /// 0xb80
			 0x34663227, /// 0xb84
			 0x488197e3, /// 0xb88
			 0x7c644d6e, /// 0xb8c
			 0x1f1d8af1, /// 0xb90
			 0x53c7e2aa, /// 0xb94
			 0xf1f7b836, /// 0xb98
			 0x65526223, /// 0xb9c
			 0x2142e50a, /// 0xba0
			 0x936ea5ba, /// 0xba4
			 0x7b5cff26, /// 0xba8
			 0x799c4564, /// 0xbac
			 0x67b4044a, /// 0xbb0
			 0x445de178, /// 0xbb4
			 0x29546742, /// 0xbb8
			 0x61eab0c7, /// 0xbbc
			 0xcd927b83, /// 0xbc0
			 0x2ad9ef8a, /// 0xbc4
			 0xd722663b, /// 0xbc8
			 0xfc3f8eb4, /// 0xbcc
			 0x824a9f6e, /// 0xbd0
			 0x9ec650ce, /// 0xbd4
			 0xeb1161d4, /// 0xbd8
			 0x11d79c1b, /// 0xbdc
			 0xf8c8e580, /// 0xbe0
			 0xd037ac50, /// 0xbe4
			 0x709b34dd, /// 0xbe8
			 0x939b623a, /// 0xbec
			 0x4f90bab3, /// 0xbf0
			 0x2552f2dd, /// 0xbf4
			 0x1a53cbda, /// 0xbf8
			 0x206f850d, /// 0xbfc
			 0x1b132360, /// 0xc00
			 0xf9f29ea9, /// 0xc04
			 0xcd98ab67, /// 0xc08
			 0x571bbc39, /// 0xc0c
			 0xfcc94d5d, /// 0xc10
			 0x5704a3a3, /// 0xc14
			 0x5aae1bed, /// 0xc18
			 0xf25cba1c, /// 0xc1c
			 0x42311683, /// 0xc20
			 0x0e7f151b, /// 0xc24
			 0x9d047e8a, /// 0xc28
			 0xc0be9e49, /// 0xc2c
			 0xe470c9f0, /// 0xc30
			 0xee9ea9bc, /// 0xc34
			 0xad2d8dc7, /// 0xc38
			 0x58f87d28, /// 0xc3c
			 0x822d8687, /// 0xc40
			 0xbc4de7bf, /// 0xc44
			 0xa8cbdba4, /// 0xc48
			 0x4273762f, /// 0xc4c
			 0x86b17cd0, /// 0xc50
			 0xbfe89c94, /// 0xc54
			 0x44e0cccc, /// 0xc58
			 0x6fe76d31, /// 0xc5c
			 0xf54c1fc1, /// 0xc60
			 0x03561824, /// 0xc64
			 0xf8c7329d, /// 0xc68
			 0x9851a71f, /// 0xc6c
			 0x393b8346, /// 0xc70
			 0xecad85b4, /// 0xc74
			 0x388b044d, /// 0xc78
			 0x39408f5d, /// 0xc7c
			 0x55e43751, /// 0xc80
			 0x25ee661f, /// 0xc84
			 0x6c540a90, /// 0xc88
			 0xb89c37dd, /// 0xc8c
			 0x3c407c87, /// 0xc90
			 0x60453473, /// 0xc94
			 0x30bd46f3, /// 0xc98
			 0x61131880, /// 0xc9c
			 0xdc4e6944, /// 0xca0
			 0x4b97512e, /// 0xca4
			 0xf2a35a13, /// 0xca8
			 0x41377768, /// 0xcac
			 0x7b55ab04, /// 0xcb0
			 0x6279f602, /// 0xcb4
			 0x8fb5ac45, /// 0xcb8
			 0x839220d2, /// 0xcbc
			 0xd93b212c, /// 0xcc0
			 0xe195100c, /// 0xcc4
			 0xb28f774a, /// 0xcc8
			 0x20706096, /// 0xccc
			 0xdf017a3e, /// 0xcd0
			 0xa9c73292, /// 0xcd4
			 0xa616ad8a, /// 0xcd8
			 0x6c056813, /// 0xcdc
			 0xbcfe920d, /// 0xce0
			 0x93a45171, /// 0xce4
			 0xf996121f, /// 0xce8
			 0x465bd278, /// 0xcec
			 0xf3dae101, /// 0xcf0
			 0x20799382, /// 0xcf4
			 0xb7e7cd8e, /// 0xcf8
			 0x919e3af5, /// 0xcfc
			 0x054bf802, /// 0xd00
			 0xa4d59510, /// 0xd04
			 0x0ac5b8cb, /// 0xd08
			 0x04793113, /// 0xd0c
			 0x396e7f23, /// 0xd10
			 0xaf642648, /// 0xd14
			 0x2bc69b5a, /// 0xd18
			 0x5842e6f7, /// 0xd1c
			 0x18005e45, /// 0xd20
			 0xd6a921dd, /// 0xd24
			 0xd43619d7, /// 0xd28
			 0xcd56ac83, /// 0xd2c
			 0x28c5d591, /// 0xd30
			 0x7017c7d0, /// 0xd34
			 0x177be372, /// 0xd38
			 0xf0177286, /// 0xd3c
			 0x4cf64208, /// 0xd40
			 0xdabdcde9, /// 0xd44
			 0x8b4093d7, /// 0xd48
			 0x1d4c19de, /// 0xd4c
			 0x3fa32f0c, /// 0xd50
			 0xccb6426e, /// 0xd54
			 0x1e20ea7b, /// 0xd58
			 0xa6d7c9bf, /// 0xd5c
			 0x47af0f4e, /// 0xd60
			 0xe4df6506, /// 0xd64
			 0x26258f91, /// 0xd68
			 0x861037ac, /// 0xd6c
			 0x86312f47, /// 0xd70
			 0x6bf8e9d2, /// 0xd74
			 0x87779dc7, /// 0xd78
			 0x58b40c2c, /// 0xd7c
			 0x406a889d, /// 0xd80
			 0x0938a126, /// 0xd84
			 0x3e3a343e, /// 0xd88
			 0x49c8247b, /// 0xd8c
			 0x13aaaed3, /// 0xd90
			 0x0f82b9b2, /// 0xd94
			 0xb3798051, /// 0xd98
			 0xa84a29f9, /// 0xd9c
			 0x6a687e69, /// 0xda0
			 0xfab10b7d, /// 0xda4
			 0xe3c44b07, /// 0xda8
			 0x4d5da95c, /// 0xdac
			 0x54a627e1, /// 0xdb0
			 0x26b317b1, /// 0xdb4
			 0x65421eb8, /// 0xdb8
			 0xce3982db, /// 0xdbc
			 0xc3779db4, /// 0xdc0
			 0x2f7638d2, /// 0xdc4
			 0x3da3b73f, /// 0xdc8
			 0xf9491b41, /// 0xdcc
			 0xc6379526, /// 0xdd0
			 0x59aee8c8, /// 0xdd4
			 0x16ccfde1, /// 0xdd8
			 0x7b3956a1, /// 0xddc
			 0x81c783ab, /// 0xde0
			 0xd076cd9d, /// 0xde4
			 0x84d9512c, /// 0xde8
			 0x23de6248, /// 0xdec
			 0x5e68b938, /// 0xdf0
			 0x2ead0938, /// 0xdf4
			 0x8eed42c0, /// 0xdf8
			 0xff8358a1, /// 0xdfc
			 0xace822ed, /// 0xe00
			 0x76751539, /// 0xe04
			 0x67564fad, /// 0xe08
			 0xcbef0f78, /// 0xe0c
			 0x1ed7eae2, /// 0xe10
			 0x86f91091, /// 0xe14
			 0x7df6f845, /// 0xe18
			 0x124f3e79, /// 0xe1c
			 0xcee72603, /// 0xe20
			 0x9f2f9b61, /// 0xe24
			 0x8aae8f6b, /// 0xe28
			 0x488794eb, /// 0xe2c
			 0xd4ae66d0, /// 0xe30
			 0x1d2b7e41, /// 0xe34
			 0xd3a00f9f, /// 0xe38
			 0x968cc726, /// 0xe3c
			 0xcdd2773a, /// 0xe40
			 0xe49c31c8, /// 0xe44
			 0xe9b396dd, /// 0xe48
			 0xc46f3c38, /// 0xe4c
			 0x0a8d3129, /// 0xe50
			 0x8e1eb327, /// 0xe54
			 0x6f146dd5, /// 0xe58
			 0x44f632c8, /// 0xe5c
			 0xef7e006b, /// 0xe60
			 0x8604528e, /// 0xe64
			 0xcfd53bbb, /// 0xe68
			 0x5b212cf9, /// 0xe6c
			 0x23f06f08, /// 0xe70
			 0xba99c956, /// 0xe74
			 0x41b1edee, /// 0xe78
			 0x4bcc3715, /// 0xe7c
			 0x7d7f0372, /// 0xe80
			 0xc3fe53c9, /// 0xe84
			 0x8b7bfb7e, /// 0xe88
			 0x6dcedbda, /// 0xe8c
			 0xc575eb49, /// 0xe90
			 0x16044e23, /// 0xe94
			 0xbcde3d54, /// 0xe98
			 0x1184e7bf, /// 0xe9c
			 0x2efd72f5, /// 0xea0
			 0x84f166e6, /// 0xea4
			 0x7dc82457, /// 0xea8
			 0x6d82ed92, /// 0xeac
			 0x871290e1, /// 0xeb0
			 0xea4e5d17, /// 0xeb4
			 0x488a8839, /// 0xeb8
			 0xd24e5092, /// 0xebc
			 0xbe6f7d2b, /// 0xec0
			 0x4e925778, /// 0xec4
			 0x2427856d, /// 0xec8
			 0x6abf5518, /// 0xecc
			 0x4fe81f4b, /// 0xed0
			 0xb504ca79, /// 0xed4
			 0xb508be53, /// 0xed8
			 0xd14dac99, /// 0xedc
			 0x7f405f58, /// 0xee0
			 0x08b13068, /// 0xee4
			 0xb395539d, /// 0xee8
			 0x4ce3e3e1, /// 0xeec
			 0x51eda815, /// 0xef0
			 0xbeea2a78, /// 0xef4
			 0x48de3212, /// 0xef8
			 0x051d0b91, /// 0xefc
			 0x6d3d8557, /// 0xf00
			 0x3e9535aa, /// 0xf04
			 0x2aef39d4, /// 0xf08
			 0x1f3fa931, /// 0xf0c
			 0x37081422, /// 0xf10
			 0x6b74a601, /// 0xf14
			 0x80890a25, /// 0xf18
			 0x1d94185a, /// 0xf1c
			 0x45855bbe, /// 0xf20
			 0x0dd3e216, /// 0xf24
			 0x1466287d, /// 0xf28
			 0x5f2249ce, /// 0xf2c
			 0x69007a26, /// 0xf30
			 0x28a641d8, /// 0xf34
			 0x8d3ed66a, /// 0xf38
			 0x7e2e9125, /// 0xf3c
			 0xffd34738, /// 0xf40
			 0xe6959d38, /// 0xf44
			 0x855bc8f7, /// 0xf48
			 0x52f7465a, /// 0xf4c
			 0x5fdf159f, /// 0xf50
			 0x808e7534, /// 0xf54
			 0x754ba064, /// 0xf58
			 0xfcff8e27, /// 0xf5c
			 0x2594c8f8, /// 0xf60
			 0x54dbbb51, /// 0xf64
			 0x92c987b3, /// 0xf68
			 0x7be75885, /// 0xf6c
			 0xe63c7b3c, /// 0xf70
			 0x5f5fefb0, /// 0xf74
			 0x01d77d96, /// 0xf78
			 0x097ab05b, /// 0xf7c
			 0x8a6ca88e, /// 0xf80
			 0xde0772a4, /// 0xf84
			 0xffa9b1cc, /// 0xf88
			 0xc125bee8, /// 0xf8c
			 0x19912fb6, /// 0xf90
			 0xc4afa03d, /// 0xf94
			 0xd914e4af, /// 0xf98
			 0xa56466ce, /// 0xf9c
			 0x51b9d8f6, /// 0xfa0
			 0xc3dbda50, /// 0xfa4
			 0x24e69081, /// 0xfa8
			 0xb60fe950, /// 0xfac
			 0x813dbd8e, /// 0xfb0
			 0x234d945d, /// 0xfb4
			 0xd5a302a4, /// 0xfb8
			 0xac2a55cb, /// 0xfbc
			 0xcc83c9c5, /// 0xfc0
			 0x5c766c46, /// 0xfc4
			 0x8c5a51d1, /// 0xfc8
			 0xf7b5b07a, /// 0xfcc
			 0x373a7904, /// 0xfd0
			 0xd4867b22, /// 0xfd4
			 0x5cc332d6, /// 0xfd8
			 0x16f134c2, /// 0xfdc
			 0xb69fcf4d, /// 0xfe0
			 0x06700047, /// 0xfe4
			 0x82756caf, /// 0xfe8
			 0x75b12303, /// 0xfec
			 0x537f20e4, /// 0xff0
			 0x60a0cc76, /// 0xff4
			 0x8d818e5f, /// 0xff8
			 0xb2170501 /// last
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
			 0x00000534,
			 0x000001f0,
			 0x00000078,
			 0x00000784,
			 0x00000208,
			 0x000006d4,
			 0x000001b4,
			 0x00000360,

			 /// vpu register f2
			 0x41200000,
			 0x40000000,
			 0x41a00000,
			 0x41800000,
			 0x41d80000,
			 0x41200000,
			 0x41900000,
			 0x41900000,

			 /// vpu register f3
			 0x41d00000,
			 0x41b00000,
			 0x41a80000,
			 0x42000000,
			 0x41100000,
			 0x41700000,
			 0x41f80000,
			 0x41b00000,

			 /// vpu register f4
			 0x41a80000,
			 0x40800000,
			 0x40000000,
			 0x41600000,
			 0x40000000,
			 0x41d80000,
			 0x41f80000,
			 0x41600000,

			 /// vpu register f5
			 0x41700000,
			 0x40400000,
			 0x40a00000,
			 0x41700000,
			 0x41500000,
			 0x41600000,
			 0x40800000,
			 0x41c00000,

			 /// vpu register f6
			 0x41900000,
			 0x41500000,
			 0x41700000,
			 0x41b80000,
			 0x40000000,
			 0x41800000,
			 0x41400000,
			 0x41a80000,

			 /// vpu register f7
			 0x41500000,
			 0x41f00000,
			 0x3f800000,
			 0x41980000,
			 0x41880000,
			 0x40c00000,
			 0x41a80000,
			 0x41c00000,

			 /// vpu register f8
			 0x41980000,
			 0x41200000,
			 0x40e00000,
			 0x41f80000,
			 0x41c80000,
			 0x41c00000,
			 0x41980000,
			 0x41800000,

			 /// vpu register f9
			 0x41300000,
			 0x3f800000,
			 0x41c80000,
			 0x40c00000,
			 0x41600000,
			 0x41e80000,
			 0x40000000,
			 0x40000000,

			 /// vpu register f10
			 0x40000000,
			 0x41a00000,
			 0x41980000,
			 0x41700000,
			 0x41700000,
			 0x41e80000,
			 0x41400000,
			 0x41980000,

			 /// vpu register f11
			 0x41e00000,
			 0x41b80000,
			 0x41a80000,
			 0x41f80000,
			 0x40c00000,
			 0x41700000,
			 0x41e00000,
			 0x41300000,

			 /// vpu register f12
			 0x41d00000,
			 0x41800000,
			 0x40000000,
			 0x40800000,
			 0x41980000,
			 0x41000000,
			 0x40c00000,
			 0x41600000,

			 /// vpu register f13
			 0x41980000,
			 0x41880000,
			 0x40400000,
			 0x40a00000,
			 0x41e80000,
			 0x41700000,
			 0x40a00000,
			 0x41e00000,

			 /// vpu register f14
			 0x41a00000,
			 0x40a00000,
			 0x41c00000,
			 0x41000000,
			 0x41700000,
			 0x41800000,
			 0x41e80000,
			 0x41500000,

			 /// vpu register f15
			 0x41d00000,
			 0x3f800000,
			 0x41200000,
			 0x41a80000,
			 0x41300000,
			 0x41800000,
			 0x41d00000,
			 0x41f00000,

			 /// vpu register f16
			 0x42000000,
			 0x40000000,
			 0x40e00000,
			 0x41700000,
			 0x3f800000,
			 0x41c00000,
			 0x41100000,
			 0x40400000,

			 /// vpu register f17
			 0x41980000,
			 0x41500000,
			 0x41500000,
			 0x41100000,
			 0x40a00000,
			 0x41f00000,
			 0x41880000,
			 0x41500000,

			 /// vpu register f18
			 0x41a80000,
			 0x41000000,
			 0x3f800000,
			 0x41e80000,
			 0x40000000,
			 0x40c00000,
			 0x41c80000,
			 0x41400000,

			 /// vpu register f19
			 0x40000000,
			 0x41e80000,
			 0x41b80000,
			 0x41e80000,
			 0x40c00000,
			 0x41500000,
			 0x41900000,
			 0x41900000,

			 /// vpu register f20
			 0x41d00000,
			 0x3f800000,
			 0x40000000,
			 0x40c00000,
			 0x41a00000,
			 0x41200000,
			 0x41a00000,
			 0x40400000,

			 /// vpu register f21
			 0x41c80000,
			 0x41f00000,
			 0x41600000,
			 0x40800000,
			 0x41a00000,
			 0x41b00000,
			 0x40000000,
			 0x41980000,

			 /// vpu register f22
			 0x40c00000,
			 0x41b00000,
			 0x41200000,
			 0x41300000,
			 0x41f80000,
			 0x3f800000,
			 0x41800000,
			 0x41900000,

			 /// vpu register f23
			 0x41f80000,
			 0x41300000,
			 0x41200000,
			 0x41b00000,
			 0x41600000,
			 0x41600000,
			 0x42000000,
			 0x41c00000,

			 /// vpu register f24
			 0x42000000,
			 0x41e80000,
			 0x41880000,
			 0x41b80000,
			 0x41300000,
			 0x40400000,
			 0x41e80000,
			 0x41c80000,

			 /// vpu register f25
			 0x41d80000,
			 0x40a00000,
			 0x41f80000,
			 0x41f80000,
			 0x3f800000,
			 0x41700000,
			 0x40400000,
			 0x40800000,

			 /// vpu register f26
			 0x41000000,
			 0x41300000,
			 0x41e00000,
			 0x41f00000,
			 0x41880000,
			 0x41a80000,
			 0x41f00000,
			 0x3f800000,

			 /// vpu register f27
			 0x41b80000,
			 0x41700000,
			 0x41600000,
			 0x41200000,
			 0x41300000,
			 0x41500000,
			 0x41980000,
			 0x41300000,

			 /// vpu register f28
			 0x41600000,
			 0x41d80000,
			 0x41900000,
			 0x41000000,
			 0x41600000,
			 0x40e00000,
			 0x41880000,
			 0x41980000,

			 /// vpu register f29
			 0x41300000,
			 0x41f80000,
			 0x42000000,
			 0x41e00000,
			 0x41400000,
			 0x41980000,
			 0x40e00000,
			 0x41200000,

			 /// vpu register f30
			 0x41d80000,
			 0x41900000,
			 0x41700000,
			 0x41200000,
			 0x41200000,
			 0x41700000,
			 0x3f800000,
			 0x3f800000,

			 /// vpu register f31
			 0x41d00000,
			 0x41d00000,
			 0x41a80000,
			 0x41100000,
			 0x41d00000,
			 0x41f00000,
			 0x41100000,
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
		"fand.pi f10, f17, f21\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f30, f14, f29\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f12, f30, f8\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f10, f23, f19\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f30, f9, f6\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f12, f26, f20\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f11, f0, f26\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f12, f18, f1\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f29, f2, f0\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f25, f23, f3\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f19, f28, f26\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f17, f1, f16\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f9, f19, f2\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f3, f4, f3\n"                                ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f8, f11, f29\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f18, f4, f21\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f3, f8, f14\n"                               ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f11, f0, f21\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f14, f27, f24\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f3, f19, f9\n"                               ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f25, f14, f0\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f16, f22, f5\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f23, f22, f14\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f8, f15, f28\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f14, f18, f4\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f14, f13, f18\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f7, f4, f16\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f16, f4, f11\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f10, f1, f19\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f27, f12, f29\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f10, f1, f13\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f24, f29, f24\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f15, f19, f21\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f16, f4, f29\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f6, f24, f9\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f23, f4, f25\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f20, f2, f21\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f27, f11, f29\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f18, f14, f23\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f8, f23, f16\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f21, f20, f9\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f10, f28, f7\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f26, f24, f27\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f25, f16, f11\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f30, f12, f0\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f4, f24, f16\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f4, f6, f8\n"                                ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f17, f19, f15\n"                             ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f20, f8, f28\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f26, f6, f0\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f24, f20, f4\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f5, f2, f3\n"                                ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f25, f9, f21\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f9, f3, f25\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f28, f22, f11\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f21, f6, f18\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f23, f9, f30\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f23, f1, f10\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f23, f18, f14\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f18, f8, f24\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f17, f8, f21\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f19, f5, f18\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f27, f7, f5\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f11, f20, f28\n"                             ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f12, f25, f27\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f6, f4, f4\n"                                ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f6, f18, f30\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f22, f29, f28\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f25, f11, f25\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f5, f17, f23\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f4, f24, f26\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f29, f24, f22\n"                             ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f23, f8, f12\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f6, f14, f17\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f26, f24, f21\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f18, f16, f15\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f30, f19, f8\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f19, f8, f2\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f14, f10, f23\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f28, f12, f1\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f29, f25, f22\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f28, f25, f26\n"                             ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f16, f20, f6\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f19, f21, f22\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f8, f25, f21\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f26, f0, f5\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f11, f13, f12\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f15, f15, f11\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f20, f20, f24\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f19, f18, f29\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f22, f10, f13\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f17, f13, f17\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f16, f29, f2\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f23, f24, f28\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f2, f26, f2\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f4, f20, f10\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f3, f6, f30\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f22, f9, f1\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fand.pi f23, f7, f12\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
