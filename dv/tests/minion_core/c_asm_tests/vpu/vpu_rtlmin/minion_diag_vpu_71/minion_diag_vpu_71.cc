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
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00004
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0000c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00014
			 0x0e000007,                                                  // Trailing 1s:                                /// 00018
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00020
			 0x7fc00001,                                                  // signaling NaN                               /// 00024
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00028
			 0x0c700000,                                                  // Leading 1s:                                 /// 0002c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00030
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00034
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00038
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0003c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00040
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00044
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00048
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x33333333,                                                  // 4 random values                             /// 00050
			 0x80011111,                                                  // -9.7958E-41                                 /// 00054
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00064
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00068
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0006c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00070
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00078
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0007c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x7f800000,                                                  // inf                                         /// 00084
			 0x7fc00001,                                                  // signaling NaN                               /// 00088
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00090
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00094
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x0c780000,                                                  // Leading 1s:                                 /// 000a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000ac
			 0xffc00001,                                                  // -signaling NaN                              /// 000b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 000b4
			 0x7f800000,                                                  // inf                                         /// 000b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00100
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00104
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00108
			 0x0c400000,                                                  // Leading 1s:                                 /// 0010c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00110
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00114
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x80000000,                                                  // -zero                                       /// 0011c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00120
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00128
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0012c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00130
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00134
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00138
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00148
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0014c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00150
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00154
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00160
			 0x0c780000,                                                  // Leading 1s:                                 /// 00164
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x0e000007,                                                  // Trailing 1s:                                /// 0016c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00170
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00178
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0017c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00180
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00184
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00188
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00190
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00194
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00198
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0019c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001a4
			 0x33333333,                                                  // 4 random values                             /// 001a8
			 0xffc00001,                                                  // -signaling NaN                              /// 001ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x3f028f5c,                                                  // 0.51                                        /// 001c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0xcb000000,                                                  // -8388608.0                                  /// 001dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00208
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0020c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00218
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0021c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00220
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00228
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0022c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00230
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00234
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00238
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x00000000,                                                  // zero                                        /// 00240
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00244
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00248
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0024c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00250
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00254
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00258
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0025c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00260
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00264
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00268
			 0x3f028f5c,                                                  // 0.51                                        /// 0026c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00270
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00274
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00278
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0027c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00284
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x0c700000,                                                  // Leading 1s:                                 /// 0028c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00294
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0029c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00300
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00308
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0030c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00310
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0031c
			 0x80000000,                                                  // -zero                                       /// 00320
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00328
			 0x4b000000,                                                  // 8388608.0                                   /// 0032c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0e000003,                                                  // Trailing 1s:                                /// 00334
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00338
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x7fc00001,                                                  // signaling NaN                               /// 00340
			 0x80011111,                                                  // -9.7958E-41                                 /// 00344
			 0x7fc00001,                                                  // signaling NaN                               /// 00348
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0034c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00350
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0035c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00360
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00364
			 0x80000000,                                                  // -zero                                       /// 00368
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00370
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00374
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0037c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00380
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00384
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00388
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0038c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00390
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00394
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0039c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003b4
			 0x33333333,                                                  // 4 random values                             /// 003b8
			 0x7f800000,                                                  // inf                                         /// 003bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 003c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00404
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0040c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00410
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00414
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0041c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00420
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00424
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00428
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0042c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00430
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00434
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0043c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00440
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00444
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00448
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0044c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0045c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0046c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00474
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00478
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00480
			 0x0e000001,                                                  // Trailing 1s:                                /// 00484
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00488
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0048c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00494
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0049c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00500
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00504
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00514
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0051c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x0e000003,                                                  // Trailing 1s:                                /// 0052c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00530
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00534
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00538
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0053c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00540
			 0xcb000000,                                                  // -8388608.0                                  /// 00544
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0054c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00554
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x0c780000,                                                  // Leading 1s:                                 /// 0055c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00568
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0056c
			 0x4b000000,                                                  // 8388608.0                                   /// 00570
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0e000007,                                                  // Trailing 1s:                                /// 0057c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00580
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00584
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0058c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00590
			 0x0c700000,                                                  // Leading 1s:                                 /// 00594
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00598
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0059c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0xbf028f5c,                                                  // -0.51                                       /// 005e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 005e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00600
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0060c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00610
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00614
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00618
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0061c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00620
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00624
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00628
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0062c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00630
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00640
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00644
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00648
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0064c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00650
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00654
			 0x0c700000,                                                  // Leading 1s:                                 /// 00658
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00664
			 0x0c600000,                                                  // Leading 1s:                                 /// 00668
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0066c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0e000003,                                                  // Trailing 1s:                                /// 00674
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00678
			 0x0e000001,                                                  // Trailing 1s:                                /// 0067c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00690
			 0xcb000000,                                                  // -8388608.0                                  /// 00694
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0069c
			 0xff800000,                                                  // -inf                                        /// 006a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x4b000000,                                                  // 8388608.0                                   /// 006a8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 006c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006dc
			 0xff800000,                                                  // -inf                                        /// 006e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00704
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00708
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0070c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00710
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00714
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00718
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00720
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00724
			 0x7fc00001,                                                  // signaling NaN                               /// 00728
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0072c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00730
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00734
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00738
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0073c
			 0x33333333,                                                  // 4 random values                             /// 00740
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00744
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00748
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0074c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00750
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00754
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00758
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0075c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00764
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0076c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00770
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00774
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x00000000,                                                  // zero                                        /// 0077c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x3f028f5c,                                                  // 0.51                                        /// 00784
			 0xffc00001,                                                  // -signaling NaN                              /// 00788
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00794
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00798
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0079c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 007d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 007e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00800
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00804
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00808
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00814
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x0c600000,                                                  // Leading 1s:                                 /// 0081c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00820
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x7fc00001,                                                  // signaling NaN                               /// 00830
			 0xffc00001,                                                  // -signaling NaN                              /// 00834
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00838
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00840
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00844
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0084c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00850
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00858
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0085c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00864
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00868
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0086c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00870
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00880
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00888
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0088c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00890
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0089c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 008c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0xbf028f5c,                                                  // -0.51                                       /// 008f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00900
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00904
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00908
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0090c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00914
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00918
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0091c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00920
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0xcb000000,                                                  // -8388608.0                                  /// 0092c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0xffc00001,                                                  // -signaling NaN                              /// 00934
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00938
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0093c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00940
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00948
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00950
			 0x33333333,                                                  // 4 random values                             /// 00954
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00974
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00978
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0097c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0xffc00001,                                                  // -signaling NaN                              /// 00988
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0098c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00994
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00998
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009f4
			 0x55555555,                                                  // 4 random values                             /// 009f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a04
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a08
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a0c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a10
			 0x80000000,                                                  // -zero                                       /// 00a14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a18
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a1c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a24
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a2c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a38
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a40
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a44
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a48
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a4c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a68
			 0x33333333,                                                  // 4 random values                             /// 00a6c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a70
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a74
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a78
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a7c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a88
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a90
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a98
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a9c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00aa0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00aa4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00aac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ab0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ab4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ab8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00abc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ac0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ac4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00acc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ad8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00adc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ae0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00aec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00af0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00af4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00af8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b18
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b1c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b20
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b24
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b38
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b3c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b40
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b4c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b50
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b54
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b5c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b60
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b64
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b68
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b6c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b74
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b7c
			 0x00000000,                                                  // zero                                        /// 00b80
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b90
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b94
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ba4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ba8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bb8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bbc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bc4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bc8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bcc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bd0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bd8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00be4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00be8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bf4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bf8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bfc
			 0x7fc00001,                                                  // signaling NaN                               /// 00c00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c18
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c1c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c24
			 0x7fc00001,                                                  // signaling NaN                               /// 00c28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c38
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c3c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c48
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c50
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c54
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c58
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c68
			 0xcb000000,                                                  // -8388608.0                                  /// 00c6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c84
			 0xff800000,                                                  // -inf                                        /// 00c88
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c8c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c98
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ca4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cb4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cc0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ccc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cd0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cd8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ce8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cf0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cf4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cf8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cfc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d04
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d0c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d28
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d4c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d54
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d68
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d6c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d88
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d98
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00da0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00da4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00db0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00db4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00db8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dbc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dc0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dc4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0xff800000,                                                  // -inf                                        /// 00dcc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dd0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dd4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dd8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ddc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00de8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00df8
			 0x7fc00001,                                                  // signaling NaN                               /// 00dfc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e10
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e34
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e38
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0xffc00001,                                                  // -signaling NaN                              /// 00e4c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e50
			 0xcb000000,                                                  // -8388608.0                                  /// 00e54
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e64
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e6c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e78
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e90
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e9c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ea0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ea8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00eac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00eb0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00eb8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ebc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ec8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ecc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ed0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ed4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00edc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ee4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ef0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ef8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f00
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f04
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f08
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f14
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f18
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f1c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f44
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f50
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f54
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f60
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f64
			 0xbf028f5c,                                                  // -0.51                                       /// 00f68
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f84
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f88
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f8c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fa4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fac
			 0x80000000,                                                  // -zero                                       /// 00fb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fb4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fb8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fbc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fc4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fc8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fcc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fd0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fd4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fd8
			 0xcb000000,                                                  // -8388608.0                                  /// 00fdc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fe0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fe8
			 0x3f028f5c,                                                  // 0.51                                        /// 00fec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ff0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ff8
			 0x00800001 // min norm + 1ulp                               // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00000
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0xcb000000,                                                  // -8388608.0                                  /// 00010
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00018
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0001c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0002c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00030
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00034
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0c400000,                                                  // Leading 1s:                                 /// 0003c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00040
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00044
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0004c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00050
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00054
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00058
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x80000000,                                                  // -zero                                       /// 00060
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00064
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00068
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0006c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00070
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00078
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0007c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00080
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00084
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00088
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00090
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00094
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000b0
			 0x4b000000,                                                  // 8388608.0                                   /// 000b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000b8
			 0x4b000000,                                                  // 8388608.0                                   /// 000bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00100
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00104
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0010c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00110
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00118
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0012c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00130
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00134
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0013c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00140
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00148
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0014c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00154
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00158
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0015c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00164
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0016c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00170
			 0x0e000001,                                                  // Trailing 1s:                                /// 00174
			 0x0e000003,                                                  // Trailing 1s:                                /// 00178
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0017c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00184
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00188
			 0x0c400000,                                                  // Leading 1s:                                 /// 0018c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00190
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00194
			 0x80000000,                                                  // -zero                                       /// 00198
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0019c
			 0x55555555,                                                  // 4 random values                             /// 001a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001d8
			 0x7f800000,                                                  // inf                                         /// 001dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00200
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00204
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00208
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0020c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00210
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00228
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00234
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00238
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0c700000,                                                  // Leading 1s:                                 /// 00248
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0024c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0025c
			 0x55555555,                                                  // 4 random values                             /// 00260
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00264
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00268
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0026c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00278
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0027c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00280
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00284
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0028c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00290
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00294
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00298
			 0x80011111,                                                  // -9.7958E-41                                 /// 0029c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002c8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0xcb000000,                                                  // -8388608.0                                  /// 002e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00304
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00314
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0032c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00330
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00344
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00348
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00350
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00358
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0035c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00360
			 0x0c780000,                                                  // Leading 1s:                                 /// 00364
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00368
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0036c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00374
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0037c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00380
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00384
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00388
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00390
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00394
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00398
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003c4
			 0x00000000,                                                  // zero                                        /// 003c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80000000,                                                  // -zero                                       /// 00400
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00404
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00410
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00414
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00418
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0041c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00420
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00428
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0042c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00430
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00438
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0043c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00440
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00444
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00448
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0044c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00450
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00454
			 0x0c780000,                                                  // Leading 1s:                                 /// 00458
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0045c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00468
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0046c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00470
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00474
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00478
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0047c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00488
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0xbf028f5c,                                                  // -0.51                                       /// 00490
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00494
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0049c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x3f028f5c,                                                  // 0.51                                        /// 004c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004d0
			 0x7f800000,                                                  // inf                                         /// 004d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00500
			 0xbf028f5c,                                                  // -0.51                                       /// 00504
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00508
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0050c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00514
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00518
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0051c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00520
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00524
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0052c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00530
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00538
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0053c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00540
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00544
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00548
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0e000003,                                                  // Trailing 1s:                                /// 00554
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0055c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00560
			 0x3f028f5c,                                                  // 0.51                                        /// 00564
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00574
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0057c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00580
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00588
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0058c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00590
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00598
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0059c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005b0
			 0x4b000000,                                                  // 8388608.0                                   /// 005b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00600
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00604
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0061c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00624
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00634
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00638
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00640
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0064c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00650
			 0x00000000,                                                  // zero                                        /// 00654
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0065c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00660
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00664
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0066c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00670
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0067c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00680
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00684
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00688
			 0x00000000,                                                  // zero                                        /// 0068c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00690
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00694
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0069c
			 0x7f800000,                                                  // inf                                         /// 006a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006bc
			 0xbf028f5c,                                                  // -0.51                                       /// 006c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006e0
			 0x55555555,                                                  // 4 random values                             /// 006e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006e8
			 0xcb000000,                                                  // -8388608.0                                  /// 006ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00700
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00704
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00708
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0070c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00710
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00714
			 0x0e000003,                                                  // Trailing 1s:                                /// 00718
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00724
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00728
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0072c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00730
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00738
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0073c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00740
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00744
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00748
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0074c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00750
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00754
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00758
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x4b000000,                                                  // 8388608.0                                   /// 00760
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00764
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00768
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0076c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00770
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00778
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00780
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00784
			 0x0e000007,                                                  // Trailing 1s:                                /// 00788
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0078c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00790
			 0x3f028f5c,                                                  // 0.51                                        /// 00794
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00798
			 0x0e000001,                                                  // Trailing 1s:                                /// 0079c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00800
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00808
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0080c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00810
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00814
			 0x55555555,                                                  // 4 random values                             /// 00818
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0081c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00820
			 0x55555555,                                                  // 4 random values                             /// 00824
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00828
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00830
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00838
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00844
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0084c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00850
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00854
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00858
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0085c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00860
			 0x00000000,                                                  // zero                                        /// 00864
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00870
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00878
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0087c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00880
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00884
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00888
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0088c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00890
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00898
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 008c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 008cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008e8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00900
			 0xff800000,                                                  // -inf                                        /// 00904
			 0x7fc00001,                                                  // signaling NaN                               /// 00908
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0090c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00910
			 0xff800000,                                                  // -inf                                        /// 00914
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00920
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00924
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00928
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0092c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00938
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0093c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00940
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00954
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00958
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0095c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00960
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00964
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00968
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0096c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00970
			 0x7fc00001,                                                  // signaling NaN                               /// 00974
			 0x33333333,                                                  // 4 random values                             /// 00978
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0097c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00980
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00990
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0099c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009bc
			 0x7fc00001,                                                  // signaling NaN                               /// 009c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009ec
			 0x33333333,                                                  // 4 random values                             /// 009f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a10
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a14
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a18
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a34
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a3c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a40
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a64
			 0xcb000000,                                                  // -8388608.0                                  /// 00a68
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a78
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a98
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00aa0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aa4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ab4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ab8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00abc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ac0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ac4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00acc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ad0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ad8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00adc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ae0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00aec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00af0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00af4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00af8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00afc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b00
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b04
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b10
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b14
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b18
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b1c
			 0x7f800000,                                                  // inf                                         /// 00b20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b24
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b34
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b38
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b40
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x55555555,                                                  // 4 random values                             /// 00b48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b50
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b54
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b58
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b5c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b68
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b78
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b80
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x7f800000,                                                  // inf                                         /// 00b90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ba0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ba4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bb8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bbc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bc0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bc4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bcc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bd0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bd4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bdc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00be4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bf0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bf4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bf8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bfc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c00
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80000000,                                                  // -zero                                       /// 00c0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c28
			 0xbf028f5c,                                                  // -0.51                                       /// 00c2c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c30
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c34
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x3f028f5c,                                                  // 0.51                                        /// 00c54
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c5c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c68
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c7c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c88
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c94
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ca4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ca8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cb0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cb8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cc4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cc8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x7f800000,                                                  // inf                                         /// 00cd0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x7f800000,                                                  // inf                                         /// 00cd8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cdc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ce4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ce8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cf0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cf4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cf8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cfc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d18
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d34
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d44
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d54
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d5c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d60
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d64
			 0x80000000,                                                  // -zero                                       /// 00d68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d78
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d7c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d80
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d84
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d88
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d98
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d9c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00da0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00da4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00da8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00db0
			 0xcb000000,                                                  // -8388608.0                                  /// 00db4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00db8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dc4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dc8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dcc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dd0
			 0x80000000,                                                  // -zero                                       /// 00dd4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dd8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ddc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00de4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00de8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00df0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00df4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00df8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dfc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e04
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e10
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e14
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e20
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e24
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e34
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e38
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e4c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e54
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e58
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e68
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e6c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e84
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e88
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e94
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e98
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ea0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ea8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00eac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00eb4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00eb8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ec4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ec8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ed8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ee4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ef0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ef4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f00
			 0x80000000,                                                  // -zero                                       /// 00f04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x80000000,                                                  // -zero                                       /// 00f0c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f24
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f68
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f78
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f98
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fa0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fa4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fa8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fb0
			 0x00000000,                                                  // zero                                        /// 00fb4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fbc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fc0
			 0xbf028f5c,                                                  // -0.51                                       /// 00fc4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fc8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fd0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fd4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fe0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fe4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fe8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ff4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ff8
			 0x00000000                                                  // zero                                        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00000
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00004
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00008
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0000c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00010
			 0x00000000,                                                  // zero                                        /// 00014
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00018
			 0x7fc00001,                                                  // signaling NaN                               /// 0001c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00020
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00028
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0002c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00034
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00038
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00048
			 0x55555555,                                                  // 4 random values                             /// 0004c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00050
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00058
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0005c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00064
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0006c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00070
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00074
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00078
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00080
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00088
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0008c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00090
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00094
			 0x0e000007,                                                  // Trailing 1s:                                /// 00098
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0009c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 000c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 000d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000d8
			 0x7fc00001,                                                  // signaling NaN                               /// 000dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 000e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 000ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00100
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00104
			 0x0c600000,                                                  // Leading 1s:                                 /// 00108
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0010c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00110
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0011c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00124
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00134
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00138
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0013c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00140
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x00000000,                                                  // zero                                        /// 00150
			 0x33333333,                                                  // 4 random values                             /// 00154
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0015c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00160
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00164
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00168
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0016c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00170
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00174
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00178
			 0x3f028f5c,                                                  // 0.51                                        /// 0017c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00180
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00184
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0018c
			 0xffc00001,                                                  // -signaling NaN                              /// 00190
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00194
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001a0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001ec
			 0x33333333,                                                  // 4 random values                             /// 001f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00200
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0020c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00210
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00214
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00218
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00224
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x3f028f5c,                                                  // 0.51                                        /// 0022c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00230
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00234
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00238
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00248
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00250
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00260
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00268
			 0x0c700000,                                                  // Leading 1s:                                 /// 0026c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00270
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00274
			 0x80000000,                                                  // -zero                                       /// 00278
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0027c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00280
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00288
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0028c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00294
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0029c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002b4
			 0x80000000,                                                  // -zero                                       /// 002b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0xbf028f5c,                                                  // -0.51                                       /// 002c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002dc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002e0
			 0x80000000,                                                  // -zero                                       /// 002e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x80000000,                                                  // -zero                                       /// 002f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00300
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00308
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0030c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00318
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0031c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00320
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00324
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00328
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00330
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00338
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0033c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00340
			 0x0e000007,                                                  // Trailing 1s:                                /// 00344
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00348
			 0x7f800000,                                                  // inf                                         /// 0034c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00354
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00358
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0035c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x4b000000,                                                  // 8388608.0                                   /// 00364
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00370
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0037c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00380
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0038c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00394
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 003c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003e4
			 0x3f028f5c,                                                  // 0.51                                        /// 003e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 003ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 003f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00400
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00404
			 0x0c400000,                                                  // Leading 1s:                                 /// 00408
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00414
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0041c
			 0xcb000000,                                                  // -8388608.0                                  /// 00420
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00424
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0042c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00430
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00434
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00438
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0043c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00440
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00444
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00448
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0044c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00450
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00458
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0045c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00460
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00464
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00468
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0046c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00470
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00474
			 0x0c700000,                                                  // Leading 1s:                                 /// 00478
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0047c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00484
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0048c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00490
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00494
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x7fc00001,                                                  // signaling NaN                               /// 004a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004ac
			 0x33333333,                                                  // 4 random values                             /// 004b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004d0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004f8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004fc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00500
			 0xff800000,                                                  // -inf                                        /// 00504
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00508
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00518
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x0c600000,                                                  // Leading 1s:                                 /// 00524
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0052c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00530
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00538
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00540
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00544
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0054c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00550
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00554
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00560
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0056c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00570
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00574
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0057c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00584
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0058c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00594
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00598
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0059c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005cc
			 0x3f028f5c,                                                  // 0.51                                        /// 005d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005f0
			 0xbf028f5c,                                                  // -0.51                                       /// 005f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005f8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00604
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00608
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00614
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0061c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00620
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00624
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00630
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00634
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00638
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0063c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00640
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00644
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00648
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0064c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x55555555,                                                  // 4 random values                             /// 00654
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00658
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0065c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00660
			 0x33333333,                                                  // 4 random values                             /// 00664
			 0xff800000,                                                  // -inf                                        /// 00668
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0066c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00674
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00678
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00688
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0068c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00698
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0069c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 006a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006f8
			 0x55555555,                                                  // 4 random values                             /// 006fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00700
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00704
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0070c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00710
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00714
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0e000003,                                                  // Trailing 1s:                                /// 0071c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00724
			 0x0c600000,                                                  // Leading 1s:                                 /// 00728
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0072c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00738
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00744
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00748
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0074c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00750
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00754
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00758
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0075c
			 0x00000000,                                                  // zero                                        /// 00760
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00764
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00770
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00774
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00778
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0077c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00780
			 0x7fc00001,                                                  // signaling NaN                               /// 00784
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0e000001,                                                  // Trailing 1s:                                /// 00794
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00798
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0079c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007b8
			 0xbf028f5c,                                                  // -0.51                                       /// 007bc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x0e000001,                                                  // Trailing 1s:                                /// 0080c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00810
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00814
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00818
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x0c600000,                                                  // Leading 1s:                                 /// 00824
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00828
			 0x80011111,                                                  // -9.7958E-41                                 /// 0082c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00834
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0083c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00840
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00848
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0084c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00858
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0085c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00860
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00864
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00868
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0086c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00870
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0087c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00880
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0xffc00001,                                                  // -signaling NaN                              /// 00888
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00890
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x0c600000,                                                  // Leading 1s:                                 /// 00898
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0089c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 008c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x00000000,                                                  // zero                                        /// 008d8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00900
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00904
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0090c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00910
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00918
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0091c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00920
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00928
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0092c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00930
			 0xff800000,                                                  // -inf                                        /// 00934
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00938
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0093c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00940
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00948
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00954
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00958
			 0x55555555,                                                  // 4 random values                             /// 0095c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00970
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00978
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x00011111,                                                  // 9.7958E-41                                  /// 00984
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00988
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0098c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00994
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00998
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a00
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a08
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a10
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a20
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a24
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a28
			 0x55555555,                                                  // 4 random values                             /// 00a2c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a30
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a34
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a68
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a6c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a78
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a84
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a90
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a94
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00aa0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00aa4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00aa8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ab4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00abc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ac4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ac8
			 0xcb000000,                                                  // -8388608.0                                  /// 00acc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ad0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00adc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ae0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00aec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00af0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00af4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00afc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b00
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b04
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b08
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b0c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b14
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b30
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b34
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b38
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b5c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b60
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b6c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b7c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b8c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b90
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b98
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ba0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ba8
			 0x80000000,                                                  // -zero                                       /// 00bac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bb4
			 0xffc00001,                                                  // -signaling NaN                              /// 00bb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bc0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bc4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bc8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bcc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bd0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bd4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bd8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00be0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00be4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00be8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bf0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bf8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bfc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c00
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c04
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c08
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c0c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c10
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c14
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c20
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c44
			 0xcb000000,                                                  // -8388608.0                                  /// 00c48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c50
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c54
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c60
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c64
			 0x7f800000,                                                  // inf                                         /// 00c68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c80
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c84
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c90
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c94
			 0x00000000,                                                  // zero                                        /// 00c98
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ca0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ca4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ca8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cb0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cb4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x3f028f5c,                                                  // 0.51                                        /// 00cbc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cc0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cc4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cd0
			 0x55555555,                                                  // 4 random values                             /// 00cd4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cd8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cdc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ce8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cf4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x55555555,                                                  // 4 random values                             /// 00d08
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d2c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d34
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d44
			 0x55555555,                                                  // 4 random values                             /// 00d48
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0xcb000000,                                                  // -8388608.0                                  /// 00d58
			 0xbf028f5c,                                                  // -0.51                                       /// 00d5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d68
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0xff800000,                                                  // -inf                                        /// 00d74
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d7c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d80
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d88
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d98
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00da4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00db0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00db4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00db8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dc0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x3f028f5c,                                                  // 0.51                                        /// 00dcc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dd4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dd8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ddc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00de0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00de4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00de8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00df4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dfc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e08
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e0c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e20
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e2c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e30
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e34
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e44
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x80000000,                                                  // -zero                                       /// 00e58
			 0x00000000,                                                  // zero                                        /// 00e5c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e68
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e78
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e7c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e80
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e88
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e90
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x80000000,                                                  // -zero                                       /// 00e98
			 0x80000000,                                                  // -zero                                       /// 00e9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ea0
			 0x80000000,                                                  // -zero                                       /// 00ea4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ea8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00eb0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00eb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ebc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ec0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ec4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ec8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ecc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80000000,                                                  // -zero                                       /// 00ed4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ed8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00edc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ee4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ee8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00eec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ef0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ef4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ef8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00efc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f28
			 0x00000000,                                                  // zero                                        /// 00f2c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f34
			 0x7fc00001,                                                  // signaling NaN                               /// 00f38
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f58
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f64
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f68
			 0xffc00001,                                                  // -signaling NaN                              /// 00f6c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f74
			 0x33333333,                                                  // 4 random values                             /// 00f78
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f7c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fa0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fb0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fb8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fbc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fc8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fd0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fd4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fd8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fe0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fe4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fe8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ff0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ff8
			 0x7f7ffffe // max norm - 1ulp                               // max norm +ve                                /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x2d42b260, /// 0x0
			 0x93491aa2, /// 0x4
			 0x2a934cba, /// 0x8
			 0x3af48617, /// 0xc
			 0x2b42d727, /// 0x10
			 0xb6f0d392, /// 0x14
			 0xb5d66054, /// 0x18
			 0x83983c74, /// 0x1c
			 0xa76fc12d, /// 0x20
			 0x75eb1d98, /// 0x24
			 0x70536932, /// 0x28
			 0x4285e913, /// 0x2c
			 0xa7d53ee5, /// 0x30
			 0x6a4ae034, /// 0x34
			 0x22aa61a5, /// 0x38
			 0x41be4da3, /// 0x3c
			 0xb8603699, /// 0x40
			 0x35594cd6, /// 0x44
			 0x756d442f, /// 0x48
			 0x355af7cd, /// 0x4c
			 0xae336d5a, /// 0x50
			 0xb2fed19e, /// 0x54
			 0xd16273b7, /// 0x58
			 0x04af340c, /// 0x5c
			 0xc03cda4a, /// 0x60
			 0x4dbfd549, /// 0x64
			 0x59827ac9, /// 0x68
			 0x34defd7b, /// 0x6c
			 0x945e4aa5, /// 0x70
			 0x33ffa577, /// 0x74
			 0xf83d6998, /// 0x78
			 0xa442bff4, /// 0x7c
			 0x9ab7b09e, /// 0x80
			 0x551787d2, /// 0x84
			 0x95e98712, /// 0x88
			 0x6e6f62f6, /// 0x8c
			 0xde1320dc, /// 0x90
			 0x6ffa3d25, /// 0x94
			 0x9fef8d8c, /// 0x98
			 0x838992ea, /// 0x9c
			 0xcef80eb4, /// 0xa0
			 0x282fc5a7, /// 0xa4
			 0x4e0ba369, /// 0xa8
			 0xee013ce6, /// 0xac
			 0x4ab3f062, /// 0xb0
			 0xfae0e841, /// 0xb4
			 0x4d8b974f, /// 0xb8
			 0xc33036dd, /// 0xbc
			 0x1666de6b, /// 0xc0
			 0x798f2ef3, /// 0xc4
			 0xa5f2382d, /// 0xc8
			 0x936fd2af, /// 0xcc
			 0x4b587401, /// 0xd0
			 0x35c69bda, /// 0xd4
			 0x14b3197a, /// 0xd8
			 0xed82523c, /// 0xdc
			 0xf522a706, /// 0xe0
			 0xa3a262b3, /// 0xe4
			 0xfd8183d0, /// 0xe8
			 0x7f2047a1, /// 0xec
			 0xa302dbf4, /// 0xf0
			 0x3db1ee05, /// 0xf4
			 0xbdf96490, /// 0xf8
			 0xcd8d3954, /// 0xfc
			 0x7fa9cf47, /// 0x100
			 0xdf6d1c10, /// 0x104
			 0xffd1eceb, /// 0x108
			 0xd3133f00, /// 0x10c
			 0x3a5bd735, /// 0x110
			 0x16aa38d9, /// 0x114
			 0x38d45efe, /// 0x118
			 0x42c33e49, /// 0x11c
			 0x9c53d22b, /// 0x120
			 0xdbeebc93, /// 0x124
			 0x839a118b, /// 0x128
			 0x5a523fa9, /// 0x12c
			 0xeb704b15, /// 0x130
			 0xf68cfe4c, /// 0x134
			 0xa43aa951, /// 0x138
			 0x231d174b, /// 0x13c
			 0x918da0fa, /// 0x140
			 0xa77916cd, /// 0x144
			 0xcf3575ea, /// 0x148
			 0x44311b4a, /// 0x14c
			 0x188048a5, /// 0x150
			 0x2d87c9e3, /// 0x154
			 0x118336ca, /// 0x158
			 0x5560e39a, /// 0x15c
			 0x8f004aaa, /// 0x160
			 0x996c342e, /// 0x164
			 0xea60433c, /// 0x168
			 0xce9c3714, /// 0x16c
			 0xa71ac118, /// 0x170
			 0xcc6a3cb2, /// 0x174
			 0x579a60ba, /// 0x178
			 0xa927f173, /// 0x17c
			 0x8fea6fd3, /// 0x180
			 0xf8f97ed4, /// 0x184
			 0xbdfdab76, /// 0x188
			 0x646667c0, /// 0x18c
			 0xb968a28e, /// 0x190
			 0xeaa6c0a1, /// 0x194
			 0x6b5a5778, /// 0x198
			 0xdcd7b7a1, /// 0x19c
			 0xb4a59332, /// 0x1a0
			 0x2a270cdf, /// 0x1a4
			 0x47c39d3b, /// 0x1a8
			 0x3a8ce031, /// 0x1ac
			 0xa321b592, /// 0x1b0
			 0xe4da0962, /// 0x1b4
			 0xbdd40bb1, /// 0x1b8
			 0x76ad225c, /// 0x1bc
			 0x2c45f809, /// 0x1c0
			 0x185928ae, /// 0x1c4
			 0x0f62d3a5, /// 0x1c8
			 0x3f62bcf9, /// 0x1cc
			 0x81321e15, /// 0x1d0
			 0x0bb0e637, /// 0x1d4
			 0x30b546ba, /// 0x1d8
			 0xe85224ed, /// 0x1dc
			 0x2ce38405, /// 0x1e0
			 0x27a54aae, /// 0x1e4
			 0x1b10516f, /// 0x1e8
			 0x17eb33f0, /// 0x1ec
			 0xb5f30f9d, /// 0x1f0
			 0x557518df, /// 0x1f4
			 0xeb149461, /// 0x1f8
			 0x2c5ee856, /// 0x1fc
			 0x9ab2d924, /// 0x200
			 0xb8cddb64, /// 0x204
			 0x6072ec49, /// 0x208
			 0xda1fe845, /// 0x20c
			 0xd670237d, /// 0x210
			 0xc4430141, /// 0x214
			 0xdb22ebab, /// 0x218
			 0x4a7e214b, /// 0x21c
			 0x021f51d1, /// 0x220
			 0xda37a97b, /// 0x224
			 0x12de3a33, /// 0x228
			 0xa67b749e, /// 0x22c
			 0xc59ce807, /// 0x230
			 0xf6f1895c, /// 0x234
			 0xc6c73f60, /// 0x238
			 0x7b6982de, /// 0x23c
			 0xacbafbe2, /// 0x240
			 0x9ebf16bf, /// 0x244
			 0x5af8858f, /// 0x248
			 0x3aafdd44, /// 0x24c
			 0xe9363ba0, /// 0x250
			 0xcb6705c5, /// 0x254
			 0xfc5d5980, /// 0x258
			 0x9782cf79, /// 0x25c
			 0x6d31cff6, /// 0x260
			 0x4648c2dc, /// 0x264
			 0x7a44aa52, /// 0x268
			 0x41577cca, /// 0x26c
			 0xfc7f5f91, /// 0x270
			 0x12a51fd1, /// 0x274
			 0x979cfa54, /// 0x278
			 0xe7c7833d, /// 0x27c
			 0x5b896152, /// 0x280
			 0xd8f49845, /// 0x284
			 0xb8d1f3be, /// 0x288
			 0xe09ec423, /// 0x28c
			 0x38f814a4, /// 0x290
			 0xd5a12b95, /// 0x294
			 0xf7fd2519, /// 0x298
			 0xcb1bc455, /// 0x29c
			 0x763b5da5, /// 0x2a0
			 0xba91f49a, /// 0x2a4
			 0x6eebae4d, /// 0x2a8
			 0xdb409655, /// 0x2ac
			 0x5cc68f6f, /// 0x2b0
			 0x5548362d, /// 0x2b4
			 0x87cdbd01, /// 0x2b8
			 0x297603a5, /// 0x2bc
			 0x5a09816e, /// 0x2c0
			 0x9b5ba340, /// 0x2c4
			 0xdf8059b2, /// 0x2c8
			 0x8325f906, /// 0x2cc
			 0xdf281597, /// 0x2d0
			 0x53f28dd8, /// 0x2d4
			 0x45670651, /// 0x2d8
			 0xd55e2d70, /// 0x2dc
			 0x56b9faa7, /// 0x2e0
			 0xa9fb08ed, /// 0x2e4
			 0x3ba61177, /// 0x2e8
			 0x2d5ea4be, /// 0x2ec
			 0x3a19cce2, /// 0x2f0
			 0xa6b18552, /// 0x2f4
			 0x8b88572e, /// 0x2f8
			 0x17ea6000, /// 0x2fc
			 0x06c7c5ba, /// 0x300
			 0x48687390, /// 0x304
			 0x72df5b2a, /// 0x308
			 0xe5dd3f33, /// 0x30c
			 0x75511b24, /// 0x310
			 0x3c855abd, /// 0x314
			 0xa84ae41b, /// 0x318
			 0x36bdde01, /// 0x31c
			 0xa40d5edc, /// 0x320
			 0xfc9d0b8f, /// 0x324
			 0x53a23f89, /// 0x328
			 0xb70bda3d, /// 0x32c
			 0x023798c7, /// 0x330
			 0x8626e204, /// 0x334
			 0x5763faef, /// 0x338
			 0xad319b54, /// 0x33c
			 0xdf99d649, /// 0x340
			 0xa1f85d0d, /// 0x344
			 0xec20b382, /// 0x348
			 0x3aa6cb3e, /// 0x34c
			 0x33ac6fac, /// 0x350
			 0x723fcaf1, /// 0x354
			 0x86df0ab1, /// 0x358
			 0x589a6802, /// 0x35c
			 0xd6f28f7c, /// 0x360
			 0xde47954b, /// 0x364
			 0x66fe1ccc, /// 0x368
			 0x974e0271, /// 0x36c
			 0x0e55fa9e, /// 0x370
			 0x3d58908c, /// 0x374
			 0x118143c8, /// 0x378
			 0xa37292ed, /// 0x37c
			 0x873f3de5, /// 0x380
			 0x0a305499, /// 0x384
			 0x494279b2, /// 0x388
			 0x34b635ee, /// 0x38c
			 0x95a0fac0, /// 0x390
			 0xd7d32fa5, /// 0x394
			 0xe83be4f2, /// 0x398
			 0x8c6fa850, /// 0x39c
			 0x29c207e6, /// 0x3a0
			 0x7bf67b51, /// 0x3a4
			 0x1d9c2f63, /// 0x3a8
			 0x82520136, /// 0x3ac
			 0xddc0c0ff, /// 0x3b0
			 0xe1d2e034, /// 0x3b4
			 0x0b464834, /// 0x3b8
			 0xf5613bee, /// 0x3bc
			 0x8cc1259c, /// 0x3c0
			 0x09d66cbd, /// 0x3c4
			 0x453a8712, /// 0x3c8
			 0x927bfb86, /// 0x3cc
			 0x3c3499df, /// 0x3d0
			 0x8579e9ed, /// 0x3d4
			 0x81eab54f, /// 0x3d8
			 0x14836fb0, /// 0x3dc
			 0x11e865b1, /// 0x3e0
			 0xf7fdf922, /// 0x3e4
			 0xe8b8595f, /// 0x3e8
			 0x58cd0ad0, /// 0x3ec
			 0x984f2b10, /// 0x3f0
			 0xfd64d8cd, /// 0x3f4
			 0x0c3841bf, /// 0x3f8
			 0x98f9f72e, /// 0x3fc
			 0xf25eb37a, /// 0x400
			 0x5d6f5154, /// 0x404
			 0x9688f5c8, /// 0x408
			 0x07190e29, /// 0x40c
			 0x5858a17f, /// 0x410
			 0x3122cdf1, /// 0x414
			 0xc939ff97, /// 0x418
			 0x6046c6a7, /// 0x41c
			 0x904a5450, /// 0x420
			 0x1be1bb29, /// 0x424
			 0xee14e929, /// 0x428
			 0x62b0d59d, /// 0x42c
			 0x5048b615, /// 0x430
			 0x0c5fc125, /// 0x434
			 0x7cc8086d, /// 0x438
			 0x57ef70ff, /// 0x43c
			 0x04faed3f, /// 0x440
			 0xc6c3d564, /// 0x444
			 0x9ea4fd25, /// 0x448
			 0x54eaccd3, /// 0x44c
			 0x43790748, /// 0x450
			 0x70cc60c6, /// 0x454
			 0x8ff1b8e6, /// 0x458
			 0x83f9d7d7, /// 0x45c
			 0xd3725ea1, /// 0x460
			 0x612ab9bf, /// 0x464
			 0x28161217, /// 0x468
			 0xca263afb, /// 0x46c
			 0x1c9b20d5, /// 0x470
			 0x6ad8bfb4, /// 0x474
			 0x0cc87d37, /// 0x478
			 0xd00d54e2, /// 0x47c
			 0xae2262b9, /// 0x480
			 0xfce3022a, /// 0x484
			 0x1e286735, /// 0x488
			 0xf04c0694, /// 0x48c
			 0x76907132, /// 0x490
			 0x00a738e4, /// 0x494
			 0xb631480a, /// 0x498
			 0xd626428e, /// 0x49c
			 0xb991127c, /// 0x4a0
			 0x1f090aec, /// 0x4a4
			 0xbf707771, /// 0x4a8
			 0xa1ff85aa, /// 0x4ac
			 0xc4a252c6, /// 0x4b0
			 0xe7264f5b, /// 0x4b4
			 0x8422b1b2, /// 0x4b8
			 0xc5d1a5ef, /// 0x4bc
			 0x25b065e8, /// 0x4c0
			 0x1f62cfa4, /// 0x4c4
			 0x909de36f, /// 0x4c8
			 0x594186d4, /// 0x4cc
			 0x73d6cdb1, /// 0x4d0
			 0x663cc9a9, /// 0x4d4
			 0x92745993, /// 0x4d8
			 0xe3bcd0d6, /// 0x4dc
			 0xca59cea8, /// 0x4e0
			 0xd794ddbb, /// 0x4e4
			 0xbc9730b7, /// 0x4e8
			 0x405848fc, /// 0x4ec
			 0xe89ff76e, /// 0x4f0
			 0xa76b8141, /// 0x4f4
			 0x8a65e9af, /// 0x4f8
			 0xaae01c91, /// 0x4fc
			 0x96891aad, /// 0x500
			 0x048c305a, /// 0x504
			 0x6ef98d23, /// 0x508
			 0x5884d15c, /// 0x50c
			 0x102e4811, /// 0x510
			 0x4347c20a, /// 0x514
			 0x09d75f45, /// 0x518
			 0x929171c2, /// 0x51c
			 0x46d04704, /// 0x520
			 0x2433ef74, /// 0x524
			 0xea3edcd6, /// 0x528
			 0x321d6f57, /// 0x52c
			 0x0cc5580d, /// 0x530
			 0x9a0b45fa, /// 0x534
			 0x1a0d07d5, /// 0x538
			 0x42d93a14, /// 0x53c
			 0x82f7103e, /// 0x540
			 0xc8b5a4cc, /// 0x544
			 0xb8751556, /// 0x548
			 0xd6f2c2b1, /// 0x54c
			 0x0f1cd1f9, /// 0x550
			 0x4a99f092, /// 0x554
			 0x9b8a34bc, /// 0x558
			 0xeb3913c4, /// 0x55c
			 0x51af8c32, /// 0x560
			 0x09566ef1, /// 0x564
			 0x05e88e11, /// 0x568
			 0xd7c043c7, /// 0x56c
			 0x745d9e3c, /// 0x570
			 0xc223e97b, /// 0x574
			 0x9d0abbc5, /// 0x578
			 0x4cfbd8d5, /// 0x57c
			 0x36dd73c3, /// 0x580
			 0x607f0663, /// 0x584
			 0x840da92b, /// 0x588
			 0x75722a2a, /// 0x58c
			 0xe8f1ab10, /// 0x590
			 0x6df04f0d, /// 0x594
			 0x1510e2c9, /// 0x598
			 0x2049f7a6, /// 0x59c
			 0x73cf9915, /// 0x5a0
			 0xd6bd589d, /// 0x5a4
			 0x3c1bd0e3, /// 0x5a8
			 0xbef60131, /// 0x5ac
			 0x2e8da989, /// 0x5b0
			 0x207e4b68, /// 0x5b4
			 0xd06f72ea, /// 0x5b8
			 0x78dc063d, /// 0x5bc
			 0x81302bdc, /// 0x5c0
			 0x53d572ee, /// 0x5c4
			 0x3a658e15, /// 0x5c8
			 0x48c9c49a, /// 0x5cc
			 0xf315afbd, /// 0x5d0
			 0x8f09513f, /// 0x5d4
			 0xb636da35, /// 0x5d8
			 0x48a81cbb, /// 0x5dc
			 0x6ab0b283, /// 0x5e0
			 0xc7ebd3f4, /// 0x5e4
			 0x153a5a75, /// 0x5e8
			 0xcd279724, /// 0x5ec
			 0xd96cd20e, /// 0x5f0
			 0x041bfbc0, /// 0x5f4
			 0x12420c0c, /// 0x5f8
			 0x7b42a486, /// 0x5fc
			 0x5cd2ecd4, /// 0x600
			 0xa9d16214, /// 0x604
			 0x69116408, /// 0x608
			 0x1f907c80, /// 0x60c
			 0x5d7e4ce5, /// 0x610
			 0x2b8f1d30, /// 0x614
			 0xd35341ff, /// 0x618
			 0xe5cb1a1f, /// 0x61c
			 0x225a2f01, /// 0x620
			 0x65802b4b, /// 0x624
			 0xe3040e01, /// 0x628
			 0x5132cc58, /// 0x62c
			 0xc17b6dde, /// 0x630
			 0x4080180e, /// 0x634
			 0xf09ff3d9, /// 0x638
			 0x8cf1da20, /// 0x63c
			 0xa3f096ca, /// 0x640
			 0xbcb5270a, /// 0x644
			 0xd23a4621, /// 0x648
			 0x98fc62d7, /// 0x64c
			 0x90860ec8, /// 0x650
			 0xec81df26, /// 0x654
			 0x3ce3c9a1, /// 0x658
			 0x79ef2bf5, /// 0x65c
			 0xcab20785, /// 0x660
			 0x3bb0f63c, /// 0x664
			 0xe7163cfa, /// 0x668
			 0xf243779b, /// 0x66c
			 0x8570b02c, /// 0x670
			 0xa55d1420, /// 0x674
			 0x39bf4d56, /// 0x678
			 0x51b96493, /// 0x67c
			 0x0ac691c1, /// 0x680
			 0x47a4278a, /// 0x684
			 0x0998a42e, /// 0x688
			 0x80dd4fea, /// 0x68c
			 0x5f8a9040, /// 0x690
			 0xfad161d2, /// 0x694
			 0xc63bc9b9, /// 0x698
			 0x50b18b2b, /// 0x69c
			 0xb0220ac5, /// 0x6a0
			 0xcd031da9, /// 0x6a4
			 0x3f2f211c, /// 0x6a8
			 0x591497e3, /// 0x6ac
			 0xc0d8e0d5, /// 0x6b0
			 0x1c608d67, /// 0x6b4
			 0xeccb4a92, /// 0x6b8
			 0xcdd4531e, /// 0x6bc
			 0x41b5bb8b, /// 0x6c0
			 0x044c8d86, /// 0x6c4
			 0xb500f9b1, /// 0x6c8
			 0xbadf52bc, /// 0x6cc
			 0x9d2732ec, /// 0x6d0
			 0xee0c8ba7, /// 0x6d4
			 0xfe579688, /// 0x6d8
			 0xe5f08b73, /// 0x6dc
			 0x2e7db28f, /// 0x6e0
			 0x1a9b8599, /// 0x6e4
			 0x5cc2d92c, /// 0x6e8
			 0xdb40dbc6, /// 0x6ec
			 0x935638d9, /// 0x6f0
			 0x69d504b4, /// 0x6f4
			 0x4c50236e, /// 0x6f8
			 0x54efe8f1, /// 0x6fc
			 0x17741b16, /// 0x700
			 0x82a77042, /// 0x704
			 0x3224c268, /// 0x708
			 0x1e8ef1d0, /// 0x70c
			 0xc7048e01, /// 0x710
			 0x5d6c27ee, /// 0x714
			 0xb046f54e, /// 0x718
			 0x159a1e22, /// 0x71c
			 0x5c9ef112, /// 0x720
			 0x4947b0d9, /// 0x724
			 0x36c61184, /// 0x728
			 0x3444c355, /// 0x72c
			 0xeb7b38e9, /// 0x730
			 0xdb4199c4, /// 0x734
			 0x34b7485a, /// 0x738
			 0x928d1260, /// 0x73c
			 0x1e29362b, /// 0x740
			 0xd193c163, /// 0x744
			 0x632183ac, /// 0x748
			 0x9dbdb088, /// 0x74c
			 0x6be944c6, /// 0x750
			 0xc22456a5, /// 0x754
			 0x7862ec90, /// 0x758
			 0xcc49b1d2, /// 0x75c
			 0x789dff65, /// 0x760
			 0xaabad349, /// 0x764
			 0x5b668d02, /// 0x768
			 0xdd51cee5, /// 0x76c
			 0x277f7f7e, /// 0x770
			 0xa67405b1, /// 0x774
			 0x44a16bc0, /// 0x778
			 0x730a58fc, /// 0x77c
			 0x514759c1, /// 0x780
			 0x4f0e2c98, /// 0x784
			 0xb673edc7, /// 0x788
			 0x35304a73, /// 0x78c
			 0xe67444dc, /// 0x790
			 0x631ade34, /// 0x794
			 0xf2c8fa78, /// 0x798
			 0xf5207100, /// 0x79c
			 0x86f200c6, /// 0x7a0
			 0xaac5bde1, /// 0x7a4
			 0x875fc8ed, /// 0x7a8
			 0xeb6d3200, /// 0x7ac
			 0xef5f0597, /// 0x7b0
			 0x5c967cda, /// 0x7b4
			 0xafc48e50, /// 0x7b8
			 0xae8664a3, /// 0x7bc
			 0x4e23cd6d, /// 0x7c0
			 0x3b016330, /// 0x7c4
			 0x6dc0c037, /// 0x7c8
			 0x8cf1d899, /// 0x7cc
			 0x7cce1df6, /// 0x7d0
			 0x2c31e834, /// 0x7d4
			 0x2f30495d, /// 0x7d8
			 0xa5e7d97d, /// 0x7dc
			 0x45e411bd, /// 0x7e0
			 0x5c4f6aa0, /// 0x7e4
			 0x015bfeeb, /// 0x7e8
			 0xdf291d4b, /// 0x7ec
			 0xebbfdeb5, /// 0x7f0
			 0x4d03d733, /// 0x7f4
			 0xc2c62571, /// 0x7f8
			 0x3ae3d0a8, /// 0x7fc
			 0x3621aa6d, /// 0x800
			 0x805760c2, /// 0x804
			 0xefb13cbe, /// 0x808
			 0xd9c99186, /// 0x80c
			 0xc52888c0, /// 0x810
			 0xcc13a8f1, /// 0x814
			 0xaf47f6de, /// 0x818
			 0x41e3b73f, /// 0x81c
			 0x52d56077, /// 0x820
			 0x03ef58e1, /// 0x824
			 0xe5e6b0e4, /// 0x828
			 0x0e241089, /// 0x82c
			 0x45118301, /// 0x830
			 0x8a15b165, /// 0x834
			 0xd902c73e, /// 0x838
			 0x7372f446, /// 0x83c
			 0x65aa7074, /// 0x840
			 0xdf424694, /// 0x844
			 0xcaf69bf8, /// 0x848
			 0x48a49c71, /// 0x84c
			 0x5e183808, /// 0x850
			 0x4bf323af, /// 0x854
			 0x6cb7c080, /// 0x858
			 0xf75eeb5c, /// 0x85c
			 0x348a52bd, /// 0x860
			 0xe30aed3f, /// 0x864
			 0xb405f88e, /// 0x868
			 0xdb7dbedd, /// 0x86c
			 0xe086531c, /// 0x870
			 0x54ca559c, /// 0x874
			 0xb36899a1, /// 0x878
			 0x1867dc80, /// 0x87c
			 0xf9664d3a, /// 0x880
			 0xcbbecc9f, /// 0x884
			 0x2cfb9919, /// 0x888
			 0xf4ab7aa4, /// 0x88c
			 0xd08b7129, /// 0x890
			 0xc1f9bdd0, /// 0x894
			 0xc2591e95, /// 0x898
			 0x88575a67, /// 0x89c
			 0x81744e35, /// 0x8a0
			 0xb706dc86, /// 0x8a4
			 0x510dbb60, /// 0x8a8
			 0x03fa5b22, /// 0x8ac
			 0xe3c61a36, /// 0x8b0
			 0x0985e079, /// 0x8b4
			 0x40b078e9, /// 0x8b8
			 0x43f4fbb9, /// 0x8bc
			 0x31b08b40, /// 0x8c0
			 0xfe8f481a, /// 0x8c4
			 0xeb584006, /// 0x8c8
			 0xc74512f4, /// 0x8cc
			 0xf89fc98a, /// 0x8d0
			 0x71c591a1, /// 0x8d4
			 0xaf6b4fe7, /// 0x8d8
			 0x17edd99b, /// 0x8dc
			 0x8ddbdf05, /// 0x8e0
			 0x7011eb7e, /// 0x8e4
			 0xbb8d4057, /// 0x8e8
			 0x0c85179d, /// 0x8ec
			 0xa5f67460, /// 0x8f0
			 0x05fb193b, /// 0x8f4
			 0x45ab7bda, /// 0x8f8
			 0xc96bb2fb, /// 0x8fc
			 0xd477d36c, /// 0x900
			 0xb214ac54, /// 0x904
			 0xdf0ac7e1, /// 0x908
			 0x1d7a9fdd, /// 0x90c
			 0xc8383010, /// 0x910
			 0x54a4886e, /// 0x914
			 0xcd9dde32, /// 0x918
			 0xf6353f6a, /// 0x91c
			 0x95a0b9d5, /// 0x920
			 0xbf70c27b, /// 0x924
			 0x47c8cedc, /// 0x928
			 0xca2439e8, /// 0x92c
			 0x511ed156, /// 0x930
			 0x27e3347e, /// 0x934
			 0xb85404ae, /// 0x938
			 0xddd4b6bf, /// 0x93c
			 0xeb08bc25, /// 0x940
			 0x21f0bb10, /// 0x944
			 0xa1c07d31, /// 0x948
			 0xd5020a0b, /// 0x94c
			 0x145dc895, /// 0x950
			 0x256f5c06, /// 0x954
			 0x37963ac4, /// 0x958
			 0xf10aacfd, /// 0x95c
			 0x1f04068c, /// 0x960
			 0x2884e531, /// 0x964
			 0x6bf6d666, /// 0x968
			 0xa154d564, /// 0x96c
			 0xd869a470, /// 0x970
			 0xd6a52660, /// 0x974
			 0xce130ea3, /// 0x978
			 0x79b97d93, /// 0x97c
			 0xde7735f8, /// 0x980
			 0x21c996f4, /// 0x984
			 0x871dee35, /// 0x988
			 0x3f7f9000, /// 0x98c
			 0xa99b7b73, /// 0x990
			 0x124a1ce7, /// 0x994
			 0x8d437de8, /// 0x998
			 0xdb926301, /// 0x99c
			 0x6f1e643e, /// 0x9a0
			 0x7bdaa980, /// 0x9a4
			 0xdc72ee8a, /// 0x9a8
			 0x0ff3dcac, /// 0x9ac
			 0x07e5e2e1, /// 0x9b0
			 0x8ed494e7, /// 0x9b4
			 0x46a71fdb, /// 0x9b8
			 0x9526024f, /// 0x9bc
			 0xe717c7f3, /// 0x9c0
			 0x175ab56f, /// 0x9c4
			 0x60e511ab, /// 0x9c8
			 0x342c148d, /// 0x9cc
			 0x8ef40377, /// 0x9d0
			 0xdf32d27d, /// 0x9d4
			 0xf227bfb4, /// 0x9d8
			 0x5e149f4a, /// 0x9dc
			 0x3eec55fe, /// 0x9e0
			 0x5a12bb0e, /// 0x9e4
			 0x5418d763, /// 0x9e8
			 0xb5c3d144, /// 0x9ec
			 0x3582d395, /// 0x9f0
			 0xc4431215, /// 0x9f4
			 0x53bbdd5f, /// 0x9f8
			 0x803e91c3, /// 0x9fc
			 0x9a26936c, /// 0xa00
			 0xe1c517e4, /// 0xa04
			 0xbc5af2f2, /// 0xa08
			 0xe38dcacb, /// 0xa0c
			 0xbb9d58a5, /// 0xa10
			 0x22eaf319, /// 0xa14
			 0x27fa09cc, /// 0xa18
			 0x19a35a04, /// 0xa1c
			 0x9001daa7, /// 0xa20
			 0xea624097, /// 0xa24
			 0x86100107, /// 0xa28
			 0x999eef64, /// 0xa2c
			 0xa3c3e2cc, /// 0xa30
			 0xf41c4f85, /// 0xa34
			 0x5127ad13, /// 0xa38
			 0x9455a236, /// 0xa3c
			 0xcc63c984, /// 0xa40
			 0xb750000a, /// 0xa44
			 0x789bf9a4, /// 0xa48
			 0x391e9fc0, /// 0xa4c
			 0x90d4aacd, /// 0xa50
			 0xddf569c9, /// 0xa54
			 0x51145513, /// 0xa58
			 0x81f584b5, /// 0xa5c
			 0xae69f6df, /// 0xa60
			 0x757bd884, /// 0xa64
			 0x0ef8155e, /// 0xa68
			 0xca9cf8f8, /// 0xa6c
			 0x954f1f60, /// 0xa70
			 0xf6ee8f56, /// 0xa74
			 0x6912d241, /// 0xa78
			 0x5636ecd1, /// 0xa7c
			 0x5edf166a, /// 0xa80
			 0x6c093be0, /// 0xa84
			 0x68ebb8cc, /// 0xa88
			 0xc92a3426, /// 0xa8c
			 0xe2b6f9ab, /// 0xa90
			 0x2308ece2, /// 0xa94
			 0x8e92ec33, /// 0xa98
			 0x3d3fdccc, /// 0xa9c
			 0x50f89a89, /// 0xaa0
			 0x8a1b13fe, /// 0xaa4
			 0x0a93875d, /// 0xaa8
			 0x70a36760, /// 0xaac
			 0x1bbc9469, /// 0xab0
			 0x05f8d14f, /// 0xab4
			 0x530729dd, /// 0xab8
			 0x4c087622, /// 0xabc
			 0x0cf76f17, /// 0xac0
			 0x2809624c, /// 0xac4
			 0x2d0b0d18, /// 0xac8
			 0x9c5b724f, /// 0xacc
			 0x9dbe6bf2, /// 0xad0
			 0xe5807b3d, /// 0xad4
			 0x5fae5f40, /// 0xad8
			 0x12123431, /// 0xadc
			 0xd0e019a7, /// 0xae0
			 0x874ea25b, /// 0xae4
			 0xe56704c5, /// 0xae8
			 0xdff1a539, /// 0xaec
			 0xda6b0ec6, /// 0xaf0
			 0x66e7eb84, /// 0xaf4
			 0x25b8848e, /// 0xaf8
			 0x7abf73eb, /// 0xafc
			 0xc42a13ab, /// 0xb00
			 0xf3135be6, /// 0xb04
			 0x037507c7, /// 0xb08
			 0xdc71af47, /// 0xb0c
			 0x7d52f3ab, /// 0xb10
			 0x4c5a13fb, /// 0xb14
			 0x759eac68, /// 0xb18
			 0x4ac60124, /// 0xb1c
			 0xa6dcd0a7, /// 0xb20
			 0x2f5cff91, /// 0xb24
			 0x944b2c68, /// 0xb28
			 0x4571b233, /// 0xb2c
			 0xc52bb4a2, /// 0xb30
			 0xf81bb356, /// 0xb34
			 0x3c86e487, /// 0xb38
			 0xe125bce4, /// 0xb3c
			 0x2124c02a, /// 0xb40
			 0xc08b8ab2, /// 0xb44
			 0xf1fc0877, /// 0xb48
			 0x69db7002, /// 0xb4c
			 0x0c3f752f, /// 0xb50
			 0xa862b36d, /// 0xb54
			 0x7002aa6f, /// 0xb58
			 0x5fe57235, /// 0xb5c
			 0xd4516c88, /// 0xb60
			 0xeee917a6, /// 0xb64
			 0x97ab5783, /// 0xb68
			 0x74e37524, /// 0xb6c
			 0x49dfae5a, /// 0xb70
			 0xfd3a7dfb, /// 0xb74
			 0xc629d9e2, /// 0xb78
			 0xcced095e, /// 0xb7c
			 0x10625f0b, /// 0xb80
			 0x6fe854f4, /// 0xb84
			 0x539dd05f, /// 0xb88
			 0xfbdaf009, /// 0xb8c
			 0x127291c5, /// 0xb90
			 0x18d50040, /// 0xb94
			 0xfe6053b7, /// 0xb98
			 0x8af65fc6, /// 0xb9c
			 0x55f77d1b, /// 0xba0
			 0xd7dd18e5, /// 0xba4
			 0xda9d7107, /// 0xba8
			 0x982c61d2, /// 0xbac
			 0xd432fcc7, /// 0xbb0
			 0xae92482b, /// 0xbb4
			 0xc04b6cf3, /// 0xbb8
			 0xe61addef, /// 0xbbc
			 0x1882f858, /// 0xbc0
			 0xd04a4a6a, /// 0xbc4
			 0x79627200, /// 0xbc8
			 0xcd7a5873, /// 0xbcc
			 0x1f57d96d, /// 0xbd0
			 0x8283008b, /// 0xbd4
			 0xc160536f, /// 0xbd8
			 0x892f7c5a, /// 0xbdc
			 0xfc397590, /// 0xbe0
			 0x2faeb00b, /// 0xbe4
			 0x23e3171b, /// 0xbe8
			 0x317b70fb, /// 0xbec
			 0xa8def608, /// 0xbf0
			 0xf41dd549, /// 0xbf4
			 0xf4b2b076, /// 0xbf8
			 0x42d92149, /// 0xbfc
			 0xebcaa5d0, /// 0xc00
			 0x3d9092e0, /// 0xc04
			 0x8e435e31, /// 0xc08
			 0x3b81b233, /// 0xc0c
			 0xebd60f58, /// 0xc10
			 0xe80de95a, /// 0xc14
			 0x2c51d776, /// 0xc18
			 0xc1046281, /// 0xc1c
			 0x3e2a583d, /// 0xc20
			 0x17f32307, /// 0xc24
			 0xf4d90197, /// 0xc28
			 0xbf9e3aef, /// 0xc2c
			 0xec6a142c, /// 0xc30
			 0x3e0bca86, /// 0xc34
			 0x20182e72, /// 0xc38
			 0x0ae2bf16, /// 0xc3c
			 0x9fe17883, /// 0xc40
			 0x21c61d20, /// 0xc44
			 0xfb340f67, /// 0xc48
			 0xe283412f, /// 0xc4c
			 0x3c512f76, /// 0xc50
			 0xb9497964, /// 0xc54
			 0x949790f2, /// 0xc58
			 0x0611ee44, /// 0xc5c
			 0x20edf36b, /// 0xc60
			 0xe862d30e, /// 0xc64
			 0xfb18523d, /// 0xc68
			 0x5ae38729, /// 0xc6c
			 0xed902447, /// 0xc70
			 0xb4340211, /// 0xc74
			 0x225f246e, /// 0xc78
			 0xd5733202, /// 0xc7c
			 0xffc3a3ab, /// 0xc80
			 0xaf2868c0, /// 0xc84
			 0xf8ea533c, /// 0xc88
			 0xabb7235c, /// 0xc8c
			 0xe9a19464, /// 0xc90
			 0xe62baef7, /// 0xc94
			 0xe1b92fcd, /// 0xc98
			 0xc7a4ff72, /// 0xc9c
			 0xa730161f, /// 0xca0
			 0xa48e2cb1, /// 0xca4
			 0x8cfb0002, /// 0xca8
			 0x10ac38ca, /// 0xcac
			 0x0129d5e7, /// 0xcb0
			 0xa0f47921, /// 0xcb4
			 0x59aa4899, /// 0xcb8
			 0x70ba5b24, /// 0xcbc
			 0xd8890693, /// 0xcc0
			 0xcf9cb9e1, /// 0xcc4
			 0x6af8abc0, /// 0xcc8
			 0x13fc043c, /// 0xccc
			 0xad2b054b, /// 0xcd0
			 0xf4f5329a, /// 0xcd4
			 0x4127aacd, /// 0xcd8
			 0xa011969b, /// 0xcdc
			 0xbedfac9b, /// 0xce0
			 0x016c7a3a, /// 0xce4
			 0x45fc04e8, /// 0xce8
			 0x0082c9d2, /// 0xcec
			 0xcd6592b2, /// 0xcf0
			 0x6b7c4478, /// 0xcf4
			 0xbdd80ef6, /// 0xcf8
			 0xe15dce51, /// 0xcfc
			 0xb2a036b3, /// 0xd00
			 0xde193b86, /// 0xd04
			 0x4d4d7a85, /// 0xd08
			 0xd96a8126, /// 0xd0c
			 0x687d426f, /// 0xd10
			 0xb0b3629e, /// 0xd14
			 0xf0915920, /// 0xd18
			 0x42a05f4e, /// 0xd1c
			 0x45d8d194, /// 0xd20
			 0x1b0b36c8, /// 0xd24
			 0x3e64ab4f, /// 0xd28
			 0x4f25d3fd, /// 0xd2c
			 0xd5c9f8ad, /// 0xd30
			 0xf0892a0e, /// 0xd34
			 0x8417f5fc, /// 0xd38
			 0xd5051dcb, /// 0xd3c
			 0x1906ca56, /// 0xd40
			 0xece46aee, /// 0xd44
			 0x7d8cb67c, /// 0xd48
			 0x79d136a8, /// 0xd4c
			 0x3b8de1fa, /// 0xd50
			 0x514d2343, /// 0xd54
			 0xe663ddcd, /// 0xd58
			 0xf28900f7, /// 0xd5c
			 0x96221ce5, /// 0xd60
			 0x1f49de4c, /// 0xd64
			 0xb2a4b738, /// 0xd68
			 0xf0f4e948, /// 0xd6c
			 0x738abb7b, /// 0xd70
			 0x3f88a981, /// 0xd74
			 0x8eaf7c89, /// 0xd78
			 0x67ec5d67, /// 0xd7c
			 0x1b994ee2, /// 0xd80
			 0xdd4aee05, /// 0xd84
			 0x8d60559f, /// 0xd88
			 0x8f7bf9e7, /// 0xd8c
			 0xfe036ce2, /// 0xd90
			 0xebfd3eb0, /// 0xd94
			 0xde0b721d, /// 0xd98
			 0x31008550, /// 0xd9c
			 0x64a0c3d4, /// 0xda0
			 0xe81f314a, /// 0xda4
			 0x591ed2be, /// 0xda8
			 0x0fd5af3d, /// 0xdac
			 0x535d7d15, /// 0xdb0
			 0x31acab6f, /// 0xdb4
			 0x622ec9c1, /// 0xdb8
			 0x3bc98d36, /// 0xdbc
			 0xa0986c99, /// 0xdc0
			 0x70b7815d, /// 0xdc4
			 0x54216dd4, /// 0xdc8
			 0x30e9d22d, /// 0xdcc
			 0xa19c82cf, /// 0xdd0
			 0x97e7ed70, /// 0xdd4
			 0x54077b47, /// 0xdd8
			 0xf8dbdbc8, /// 0xddc
			 0xa14a7c2f, /// 0xde0
			 0x21512980, /// 0xde4
			 0x2a295bc4, /// 0xde8
			 0xad6ddb38, /// 0xdec
			 0x643d5c14, /// 0xdf0
			 0x73da7b03, /// 0xdf4
			 0x6b9542bb, /// 0xdf8
			 0x05c2aa72, /// 0xdfc
			 0xe60c68be, /// 0xe00
			 0x141b6880, /// 0xe04
			 0xb6d8a5c2, /// 0xe08
			 0x7cc07d3f, /// 0xe0c
			 0x3f62c77c, /// 0xe10
			 0xe4fb0da2, /// 0xe14
			 0x6d5a0a34, /// 0xe18
			 0x207d4819, /// 0xe1c
			 0x3a133847, /// 0xe20
			 0x414a99f0, /// 0xe24
			 0x26d3013d, /// 0xe28
			 0x2b257930, /// 0xe2c
			 0x283f0fe0, /// 0xe30
			 0x80d0f258, /// 0xe34
			 0xfde00a19, /// 0xe38
			 0x6bad05c9, /// 0xe3c
			 0xf018e061, /// 0xe40
			 0x820681d6, /// 0xe44
			 0x78fcebcc, /// 0xe48
			 0x54d35b73, /// 0xe4c
			 0x43038e3b, /// 0xe50
			 0x7146470e, /// 0xe54
			 0xe6b637f1, /// 0xe58
			 0xa1dca5ad, /// 0xe5c
			 0xb01af294, /// 0xe60
			 0xaf57dea0, /// 0xe64
			 0x2738ea90, /// 0xe68
			 0x4fb0c4cc, /// 0xe6c
			 0xec6ae643, /// 0xe70
			 0xffa7d292, /// 0xe74
			 0x793b27ee, /// 0xe78
			 0x573c0ca4, /// 0xe7c
			 0x9f51cc19, /// 0xe80
			 0x535e67c5, /// 0xe84
			 0xeeeaa3c5, /// 0xe88
			 0xa7541b5c, /// 0xe8c
			 0xffd777cb, /// 0xe90
			 0x424f0d1a, /// 0xe94
			 0x7e20610b, /// 0xe98
			 0x936b4d01, /// 0xe9c
			 0xd31d54a1, /// 0xea0
			 0xc324d975, /// 0xea4
			 0xd89bea16, /// 0xea8
			 0xd318607d, /// 0xeac
			 0x15f7940d, /// 0xeb0
			 0x921af843, /// 0xeb4
			 0x1c09422d, /// 0xeb8
			 0x5423afdc, /// 0xebc
			 0x799388f3, /// 0xec0
			 0x265e325f, /// 0xec4
			 0x9b8e278b, /// 0xec8
			 0x1c19680a, /// 0xecc
			 0x2055c868, /// 0xed0
			 0x192e3ea5, /// 0xed4
			 0x78464fca, /// 0xed8
			 0x569b5c07, /// 0xedc
			 0x8c4b1b0d, /// 0xee0
			 0xeb8064c9, /// 0xee4
			 0x947e17c2, /// 0xee8
			 0xde4e1561, /// 0xeec
			 0xfd6cfb7b, /// 0xef0
			 0xa072dfcc, /// 0xef4
			 0xfac01e52, /// 0xef8
			 0xbd997be0, /// 0xefc
			 0xb163eaff, /// 0xf00
			 0x24a18524, /// 0xf04
			 0x16312acc, /// 0xf08
			 0x4a2d258b, /// 0xf0c
			 0x8a4f002c, /// 0xf10
			 0xc277008c, /// 0xf14
			 0xb8a3d3dc, /// 0xf18
			 0xe78c8577, /// 0xf1c
			 0xb62c2344, /// 0xf20
			 0xbaa1234f, /// 0xf24
			 0xbba1ee53, /// 0xf28
			 0x792dd148, /// 0xf2c
			 0x55ce8647, /// 0xf30
			 0x0af7d787, /// 0xf34
			 0x3aaf912f, /// 0xf38
			 0x7bd52730, /// 0xf3c
			 0xd9c4243b, /// 0xf40
			 0x519a7be2, /// 0xf44
			 0xf8c07005, /// 0xf48
			 0x6c8ec1cb, /// 0xf4c
			 0x2d04084c, /// 0xf50
			 0xaa1f79ab, /// 0xf54
			 0x4afd344d, /// 0xf58
			 0xe9fa5615, /// 0xf5c
			 0xd997ac78, /// 0xf60
			 0x97a900e1, /// 0xf64
			 0x1ddee963, /// 0xf68
			 0x4e03c23b, /// 0xf6c
			 0x538d5d73, /// 0xf70
			 0x5a290c15, /// 0xf74
			 0x7bd1e690, /// 0xf78
			 0x9e7c2edc, /// 0xf7c
			 0xa812786d, /// 0xf80
			 0x29d2e6a2, /// 0xf84
			 0x08c0d33e, /// 0xf88
			 0xb6b3ee04, /// 0xf8c
			 0xaa31738f, /// 0xf90
			 0x57b0f700, /// 0xf94
			 0xd4242223, /// 0xf98
			 0x621a5d1e, /// 0xf9c
			 0x3fdc4c89, /// 0xfa0
			 0xe78624cd, /// 0xfa4
			 0xc477743b, /// 0xfa8
			 0x6b6adb78, /// 0xfac
			 0x2323df04, /// 0xfb0
			 0xfdab58dd, /// 0xfb4
			 0x80054f40, /// 0xfb8
			 0x27f90b53, /// 0xfbc
			 0x65de4136, /// 0xfc0
			 0x71630238, /// 0xfc4
			 0x3184ac1e, /// 0xfc8
			 0x701721c6, /// 0xfcc
			 0xd42a2ee8, /// 0xfd0
			 0x18d120e0, /// 0xfd4
			 0x6f2072d8, /// 0xfd8
			 0x0481b7be, /// 0xfdc
			 0x94a74b8a, /// 0xfe0
			 0x42425947, /// 0xfe4
			 0x1378e8c6, /// 0xfe8
			 0xd3691bdc, /// 0xfec
			 0x89ab2759, /// 0xff0
			 0x218f43d0, /// 0xff4
			 0x04952dc4, /// 0xff8
			 0x3d7e70bd /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00000
			 0x0e000007,                                                  // Trailing 1s:                                /// 00004
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00008
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0000c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x7f800000,                                                  // inf                                         /// 0001c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00020
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00028
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00034
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00038
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0e000003,                                                  // Trailing 1s:                                /// 00044
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00048
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0004c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00058
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00060
			 0xffc00001,                                                  // -signaling NaN                              /// 00064
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00068
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0006c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00070
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00078
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0007c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00080
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00084
			 0x0c780000,                                                  // Leading 1s:                                 /// 00088
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0008c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00090
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0xff800000,                                                  // -inf                                        /// 00098
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x0e000001,                                                  // Trailing 1s:                                /// 000a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 000d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000f4
			 0xffc00001,                                                  // -signaling NaN                              /// 000f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00100
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00104
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00108
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0010c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00110
			 0xffc00001,                                                  // -signaling NaN                              /// 00114
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00120
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00124
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00128
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0012c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00130
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00134
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00138
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00140
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00148
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0014c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00158
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0015c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00160
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00168
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0016c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00170
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00174
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0xcb000000,                                                  // -8388608.0                                  /// 00180
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00184
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00188
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00190
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 001ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001ec
			 0xbf028f5c,                                                  // -0.51                                       /// 001f0
			 0xbf028f5c,                                                  // -0.51                                       /// 001f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00200
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00204
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0020c
			 0x55555555,                                                  // 4 random values                             /// 00210
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00218
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00228
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0022c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00230
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00234
			 0x00011111,                                                  // 9.7958E-41                                  /// 00238
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0023c
			 0x00000000,                                                  // zero                                        /// 00240
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x55555555,                                                  // 4 random values                             /// 00248
			 0x0e000001,                                                  // Trailing 1s:                                /// 0024c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00250
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00264
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00268
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x7fc00001,                                                  // signaling NaN                               /// 00270
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00274
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00278
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0027c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00280
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00284
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0028c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00290
			 0xbf028f5c,                                                  // -0.51                                       /// 00294
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00298
			 0x80000000,                                                  // -zero                                       /// 0029c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 002d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002d8
			 0xbf028f5c,                                                  // -0.51                                       /// 002dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 002e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002e8
			 0x33333333,                                                  // 4 random values                             /// 002ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002f0
			 0xbf028f5c,                                                  // -0.51                                       /// 002f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00300
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00308
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0030c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00314
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00318
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0031c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00320
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0032c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00330
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00334
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00338
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00340
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0034c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00350
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00354
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00358
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00360
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00364
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00368
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0036c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00374
			 0x7f800000,                                                  // inf                                         /// 00378
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0037c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00380
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00388
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00390
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00398
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003b4
			 0xbf028f5c,                                                  // -0.51                                       /// 003b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 003d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003e0
			 0xcb000000,                                                  // -8388608.0                                  /// 003e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 003f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00400
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00404
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00408
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0040c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00410
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00414
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00418
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0041c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00420
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00424
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00428
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0042c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00430
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00434
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00438
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0043c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00444
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00448
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0044c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00450
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00458
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00468
			 0x3f028f5c,                                                  // 0.51                                        /// 0046c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00470
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00474
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00478
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00480
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00484
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0048c
			 0x00000000,                                                  // zero                                        /// 00490
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00498
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0049c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 004c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 004d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00504
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x0e000007,                                                  // Trailing 1s:                                /// 0050c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00510
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0051c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0xcb000000,                                                  // -8388608.0                                  /// 00524
			 0x00011111,                                                  // 9.7958E-41                                  /// 00528
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0052c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00538
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0xcb000000,                                                  // -8388608.0                                  /// 00540
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00548
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0054c
			 0x80000000,                                                  // -zero                                       /// 00550
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00554
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00558
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0055c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00564
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00568
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00570
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00574
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00578
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0057c
			 0x7f800000,                                                  // inf                                         /// 00580
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00588
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0058c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00590
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00594
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00598
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0xbf028f5c,                                                  // -0.51                                       /// 005b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 005c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 005dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00604
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00608
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0060c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00610
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00618
			 0x0e000007,                                                  // Trailing 1s:                                /// 0061c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00620
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00624
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00628
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00630
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00638
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0063c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00644
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00648
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00650
			 0x33333333,                                                  // 4 random values                             /// 00654
			 0x0c600000,                                                  // Leading 1s:                                 /// 00658
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0065c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00660
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00664
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00668
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0066c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00670
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x7fc00001,                                                  // signaling NaN                               /// 00678
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0067c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00694
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0069c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006f0
			 0xff800000,                                                  // -inf                                        /// 006f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00700
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00704
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00708
			 0x00011111,                                                  // 9.7958E-41                                  /// 0070c
			 0xff800000,                                                  // -inf                                        /// 00710
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00714
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00718
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0071c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00720
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0x0c700000,                                                  // Leading 1s:                                 /// 0072c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00730
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00734
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0073c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00744
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0074c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00750
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00754
			 0x3f028f5c,                                                  // 0.51                                        /// 00758
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0075c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00760
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00764
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00768
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00770
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00774
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00778
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00784
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0078c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00790
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00794
			 0xffc00001,                                                  // -signaling NaN                              /// 00798
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0079c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007b0
			 0xcb000000,                                                  // -8388608.0                                  /// 007b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 007d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007e4
			 0x3f028f5c,                                                  // 0.51                                        /// 007e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00804
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0080c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00810
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00814
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00818
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0081c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00820
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00830
			 0x0e000001,                                                  // Trailing 1s:                                /// 00834
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0083c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x00000000,                                                  // zero                                        /// 00848
			 0xff800000,                                                  // -inf                                        /// 0084c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00850
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00858
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0085c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00868
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0086c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00870
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00874
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00878
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0087c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00888
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0088c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00894
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00898
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0089c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 008b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008ec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008f8
			 0xcb000000,                                                  // -8388608.0                                  /// 008fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00900
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00904
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00908
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0090c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00910
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0x55555555,                                                  // 4 random values                             /// 00918
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0091c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00924
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00928
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0092c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00940
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00944
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00948
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0094c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00950
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00954
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00958
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0095c
			 0x4b000000,                                                  // 8388608.0                                   /// 00960
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00968
			 0x4b000000,                                                  // 8388608.0                                   /// 0096c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00970
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00974
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x7f800000,                                                  // inf                                         /// 00980
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00984
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00988
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0098c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00990
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00994
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00998
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009b0
			 0xffc00001,                                                  // -signaling NaN                              /// 009b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009c4
			 0x33333333,                                                  // 4 random values                             /// 009c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 009d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a08
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a14
			 0x3f028f5c,                                                  // 0.51                                        /// 00a18
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a20
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a40
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a5c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a64
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a74
			 0x7f800000,                                                  // inf                                         /// 00a78
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a88
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a94
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a98
			 0xcb000000,                                                  // -8388608.0                                  /// 00a9c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0xffc00001,                                                  // -signaling NaN                              /// 00aa4
			 0x3f028f5c,                                                  // 0.51                                        /// 00aa8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ab0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ab4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ac0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ac4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ac8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00acc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ad0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ae0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00aec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00af4
			 0xff800000,                                                  // -inf                                        /// 00af8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b14
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b18
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b28
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b34
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b38
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b3c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b54
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b68
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b70
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b74
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b78
			 0xffc00001,                                                  // -signaling NaN                              /// 00b7c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b80
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b88
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b98
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ba0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bb0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bb4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bc0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bc4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bd8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bdc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00be4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bf4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bf8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bfc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c04
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c0c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c10
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c1c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c28
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c2c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c48
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c4c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c50
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c5c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c6c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c70
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c84
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ca8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cc0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cc4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ccc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cd8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cdc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ce0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ce8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cf0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cf8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cfc
			 0xbf028f5c,                                                  // -0.51                                       /// 00d00
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d08
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d10
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d30
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d3c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d40
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d44
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d48
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d50
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d58
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d60
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d64
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d74
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d78
			 0xcb000000,                                                  // -8388608.0                                  /// 00d7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d88
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d90
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d94
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00da4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00db0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00db8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dbc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dc4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dcc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dd4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dd8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ddc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00de4
			 0x33333333,                                                  // 4 random values                             /// 00de8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00df0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00df8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dfc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e00
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e04
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e14
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e2c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e34
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e3c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e40
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e50
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e6c
			 0x55555555,                                                  // 4 random values                             /// 00e70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e78
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e80
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e84
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xbf028f5c,                                                  // -0.51                                       /// 00e98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e9c
			 0x7f800000,                                                  // inf                                         /// 00ea0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00eb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00eb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00ebc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ec0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ec4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ed0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00edc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ee4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ee8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ef0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00efc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f08
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f0c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f1c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f28
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f34
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f3c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f44
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f54
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f58
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f68
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00000000,                                                  // zero                                        /// 00f74
			 0xcb000000,                                                  // -8388608.0                                  /// 00f78
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f7c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f84
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f88
			 0xbf028f5c,                                                  // -0.51                                       /// 00f8c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f98
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f9c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fa4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fb8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fbc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fc0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fc8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fcc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fd0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fd8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fe0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fe4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ff0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ff8
			 0x0c700000                                                  // Leading 1s:                                 /// last
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
			 0x000006c0,
			 0x000006d4,
			 0x00000508,
			 0x000007b4,
			 0x0000031c,
			 0x0000003c,
			 0x00000714,
			 0x00000194,

			 /// vpu register f2
			 0x41a80000,
			 0x40c00000,
			 0x41980000,
			 0x41000000,
			 0x41e00000,
			 0x41f80000,
			 0x42000000,
			 0x3f800000,

			 /// vpu register f3
			 0x40400000,
			 0x40400000,
			 0x41b00000,
			 0x41980000,
			 0x41400000,
			 0x40a00000,
			 0x41c80000,
			 0x40400000,

			 /// vpu register f4
			 0x41e80000,
			 0x41400000,
			 0x41e80000,
			 0x40000000,
			 0x42000000,
			 0x40800000,
			 0x41900000,
			 0x41900000,

			 /// vpu register f5
			 0x41a00000,
			 0x41b80000,
			 0x41c80000,
			 0x41700000,
			 0x41900000,
			 0x41880000,
			 0x40400000,
			 0x41600000,

			 /// vpu register f6
			 0x40000000,
			 0x41c00000,
			 0x41e00000,
			 0x41f80000,
			 0x41b80000,
			 0x40a00000,
			 0x41900000,
			 0x41d80000,

			 /// vpu register f7
			 0x41d80000,
			 0x41a80000,
			 0x40800000,
			 0x41800000,
			 0x41400000,
			 0x41400000,
			 0x41d80000,
			 0x41f00000,

			 /// vpu register f8
			 0x41800000,
			 0x41700000,
			 0x41800000,
			 0x41b00000,
			 0x41d80000,
			 0x41100000,
			 0x3f800000,
			 0x41200000,

			 /// vpu register f9
			 0x40c00000,
			 0x41200000,
			 0x41600000,
			 0x40a00000,
			 0x41a80000,
			 0x41a00000,
			 0x41300000,
			 0x41f00000,

			 /// vpu register f10
			 0x41600000,
			 0x41e80000,
			 0x41d00000,
			 0x41c00000,
			 0x41f80000,
			 0x40400000,
			 0x40c00000,
			 0x40800000,

			 /// vpu register f11
			 0x40000000,
			 0x41a80000,
			 0x40c00000,
			 0x41c80000,
			 0x41c80000,
			 0x41980000,
			 0x41e80000,
			 0x41980000,

			 /// vpu register f12
			 0x40c00000,
			 0x41b00000,
			 0x41000000,
			 0x41100000,
			 0x40000000,
			 0x41a80000,
			 0x41c00000,
			 0x41200000,

			 /// vpu register f13
			 0x41a00000,
			 0x40c00000,
			 0x41900000,
			 0x41300000,
			 0x41b00000,
			 0x41e80000,
			 0x41a80000,
			 0x41e00000,

			 /// vpu register f14
			 0x42000000,
			 0x41700000,
			 0x40e00000,
			 0x40000000,
			 0x41f00000,
			 0x41b80000,
			 0x41300000,
			 0x3f800000,

			 /// vpu register f15
			 0x41a00000,
			 0x41e00000,
			 0x41100000,
			 0x41c80000,
			 0x41e80000,
			 0x41600000,
			 0x3f800000,
			 0x41000000,

			 /// vpu register f16
			 0x41880000,
			 0x41000000,
			 0x41200000,
			 0x41b80000,
			 0x40e00000,
			 0x40000000,
			 0x41d00000,
			 0x41200000,

			 /// vpu register f17
			 0x40c00000,
			 0x3f800000,
			 0x40c00000,
			 0x41500000,
			 0x41e00000,
			 0x40800000,
			 0x41a00000,
			 0x41700000,

			 /// vpu register f18
			 0x41300000,
			 0x41a00000,
			 0x41900000,
			 0x41200000,
			 0x40800000,
			 0x40000000,
			 0x41700000,
			 0x41f80000,

			 /// vpu register f19
			 0x40800000,
			 0x40800000,
			 0x41e80000,
			 0x42000000,
			 0x40800000,
			 0x41d00000,
			 0x41e80000,
			 0x41c80000,

			 /// vpu register f20
			 0x41500000,
			 0x41c80000,
			 0x40000000,
			 0x41e00000,
			 0x40a00000,
			 0x41b80000,
			 0x40400000,
			 0x41a00000,

			 /// vpu register f21
			 0x41b00000,
			 0x41600000,
			 0x41000000,
			 0x41500000,
			 0x41900000,
			 0x41d80000,
			 0x41a00000,
			 0x41c00000,

			 /// vpu register f22
			 0x40000000,
			 0x41f80000,
			 0x40e00000,
			 0x41a00000,
			 0x3f800000,
			 0x41400000,
			 0x40c00000,
			 0x41880000,

			 /// vpu register f23
			 0x41e80000,
			 0x41d80000,
			 0x40c00000,
			 0x41f00000,
			 0x41000000,
			 0x42000000,
			 0x40e00000,
			 0x41e00000,

			 /// vpu register f24
			 0x40a00000,
			 0x41e80000,
			 0x41300000,
			 0x41b00000,
			 0x41900000,
			 0x41b80000,
			 0x41c00000,
			 0x41a80000,

			 /// vpu register f25
			 0x40a00000,
			 0x40c00000,
			 0x41d80000,
			 0x41980000,
			 0x41b80000,
			 0x41800000,
			 0x41200000,
			 0x40800000,

			 /// vpu register f26
			 0x41d80000,
			 0x41800000,
			 0x41400000,
			 0x42000000,
			 0x40a00000,
			 0x41e00000,
			 0x40c00000,
			 0x41a80000,

			 /// vpu register f27
			 0x40000000,
			 0x41d80000,
			 0x40400000,
			 0x41a80000,
			 0x41b80000,
			 0x41400000,
			 0x41a00000,
			 0x41700000,

			 /// vpu register f28
			 0x41c00000,
			 0x41c80000,
			 0x41e00000,
			 0x41c00000,
			 0x41300000,
			 0x41b00000,
			 0x40400000,
			 0x41d00000,

			 /// vpu register f29
			 0x41e00000,
			 0x41f00000,
			 0x41800000,
			 0x41f00000,
			 0x41b00000,
			 0x41400000,
			 0x40a00000,
			 0x40a00000,

			 /// vpu register f30
			 0x41a00000,
			 0x41f80000,
			 0x40a00000,
			 0x41a00000,
			 0x41f80000,
			 0x42000000,
			 0x41900000,
			 0x41700000,

			 /// vpu register f31
			 0x3f800000,
			 0x3f800000,
			 0x40e00000,
			 0x41100000,
			 0x41980000,
			 0x40400000,
			 0x41980000,
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
		"flt.ps f25, f1, f23\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f25, f6, f14\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f4, f12, f14\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f19, f28, f26\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f27, f12, f14\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f27, f1, f15\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f28, f11, f27\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f6, f20, f12\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f3, f11, f25\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f8, f2, f15\n"                                ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f19, f2, f20\n"                               ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f16, f18, f16\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f18, f14, f7\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f18, f7, f14\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f9, f30, f3\n"                                ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f13, f25, f29\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f5, f26, f4\n"                                ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f20, f30, f28\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f30, f7, f19\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f17, f1, f9\n"                                ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f8, f19, f0\n"                                ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f22, f2, f11\n"                               ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f28, f1, f25\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f13, f10, f19\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f18, f21, f17\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f28, f10, f21\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f15, f16, f8\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f9, f19, f15\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f20, f8, f20\n"                               ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f28, f11, f0\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f2, f5, f22\n"                                ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f28, f1, f26\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f26, f10, f13\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f21, f1, f9\n"                                ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f16, f30, f23\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f9, f18, f8\n"                                ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f18, f21, f30\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f8, f28, f1\n"                                ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f23, f19, f15\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f4, f5, f8\n"                                 ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f22, f12, f15\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f4, f16, f12\n"                               ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f6, f12, f14\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f18, f21, f5\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f23, f15, f8\n"                               ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f7, f11, f18\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f3, f10, f17\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f3, f1, f28\n"                                ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f19, f11, f24\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f11, f10, f5\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f22, f0, f6\n"                                ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f11, f10, f25\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f25, f22, f1\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f7, f1, f24\n"                                ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f19, f22, f8\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f12, f11, f19\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f16, f7, f21\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f27, f17, f7\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f18, f28, f21\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f26, f30, f17\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f11, f26, f24\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f10, f18, f6\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f11, f16, f0\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f20, f6, f30\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f10, f11, f19\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f23, f8, f4\n"                                ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f28, f26, f20\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f22, f5, f9\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f25, f5, f0\n"                                ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f24, f7, f12\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f27, f27, f8\n"                               ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f16, f18, f20\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f12, f2, f11\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f4, f20, f25\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f3, f23, f3\n"                                ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f5, f21, f17\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f12, f9, f11\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f30, f17, f2\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f10, f2, f3\n"                                ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f19, f26, f0\n"                               ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f22, f0, f0\n"                                ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f12, f8, f7\n"                                ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f10, f29, f15\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f20, f18, f19\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f21, f22, f9\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f15, f22, f1\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f17, f17, f12\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f5, f4, f24\n"                                ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f24, f7, f2\n"                                ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f22, f19, f28\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f30, f28, f23\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f18, f24, f17\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f20, f10, f29\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f14, f7, f26\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f15, f9, f3\n"                                ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f14, f3, f16\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f29, f9, f13\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f4, f24, f18\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.ps f28, f21, f17\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
