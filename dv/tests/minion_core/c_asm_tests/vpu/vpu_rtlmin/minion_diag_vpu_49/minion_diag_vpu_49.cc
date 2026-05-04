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
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00000
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00004
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0000c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00010
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00014
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00018
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00020
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00024
			 0x0c600000,                                                  // Leading 1s:                                 /// 00028
			 0x0e000003,                                                  // Trailing 1s:                                /// 0002c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00030
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00034
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0003c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00044
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00048
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0004c
			 0xffc00001,                                                  // -signaling NaN                              /// 00050
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00058
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00060
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x3f028f5c,                                                  // 0.51                                        /// 00074
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00078
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00080
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00088
			 0x0e000001,                                                  // Trailing 1s:                                /// 0008c
			 0x80000000,                                                  // -zero                                       /// 00090
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00094
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00098
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x80000000,                                                  // -zero                                       /// 000a0
			 0xffc00001,                                                  // -signaling NaN                              /// 000a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000b4
			 0xff800000,                                                  // -inf                                        /// 000b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 000d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000e0
			 0xff800000,                                                  // -inf                                        /// 000e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00100
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00104
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0010c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00114
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00118
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x3f028f5c,                                                  // 0.51                                        /// 00120
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00124
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00128
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0012c
			 0xbf028f5c,                                                  // -0.51                                       /// 00130
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00138
			 0x4b000000,                                                  // 8388608.0                                   /// 0013c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00140
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00144
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x80000000,                                                  // -zero                                       /// 0014c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00150
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0xff800000,                                                  // -inf                                        /// 00158
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0015c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00160
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00168
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0016c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00174
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00178
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0017c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00180
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00188
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0018c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00198
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001c4
			 0xffc00001,                                                  // -signaling NaN                              /// 001c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 001fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00200
			 0x0c400000,                                                  // Leading 1s:                                 /// 00204
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00208
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0020c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0xffc00001,                                                  // -signaling NaN                              /// 00214
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00218
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0021c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00220
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x7f800000,                                                  // inf                                         /// 00228
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0022c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00230
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00234
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00238
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0023c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00240
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00244
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0024c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00250
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00254
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00258
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0025c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00264
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0026c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00270
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00274
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00278
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0027c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0028c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00290
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00298
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0029c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002e4
			 0x4b000000,                                                  // 8388608.0                                   /// 002e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00300
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00308
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00320
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00324
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00328
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0032c
			 0x80000000,                                                  // -zero                                       /// 00330
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00334
			 0xff800000,                                                  // -inf                                        /// 00338
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0033c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00340
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00344
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00348
			 0xcb000000,                                                  // -8388608.0                                  /// 0034c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00354
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00364
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0036c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00374
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00378
			 0xff800000,                                                  // -inf                                        /// 0037c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00380
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00384
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00388
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x7fc00001,                                                  // signaling NaN                               /// 00390
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00398
			 0x0e000003,                                                  // Trailing 1s:                                /// 0039c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 003a4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 003b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003cc
			 0x33333333,                                                  // 4 random values                             /// 003d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003e8
			 0x80000000,                                                  // -zero                                       /// 003ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 003f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00400
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00408
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00410
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00414
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0041c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0xcb000000,                                                  // -8388608.0                                  /// 00424
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0042c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0043c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00440
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00444
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00448
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0044c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00454
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00458
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0045c
			 0xbf028f5c,                                                  // -0.51                                       /// 00460
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00464
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00468
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00470
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00474
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00484
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00490
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00494
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0049c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0xbf028f5c,                                                  // -0.51                                       /// 004c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004dc
			 0xcb000000,                                                  // -8388608.0                                  /// 004e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00504
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00508
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0050c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00514
			 0x0c700000,                                                  // Leading 1s:                                 /// 00518
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0051c
			 0x80000000,                                                  // -zero                                       /// 00520
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00524
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00528
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x00000000,                                                  // zero                                        /// 00530
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00538
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0053c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00548
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0054c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00554
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0055c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00560
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00564
			 0x0e000007,                                                  // Trailing 1s:                                /// 00568
			 0x0e000003,                                                  // Trailing 1s:                                /// 0056c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00570
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00578
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0057c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00584
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00588
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0058c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00590
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00598
			 0x0c700000,                                                  // Leading 1s:                                 /// 0059c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 005c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00600
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00604
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00608
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0060c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00610
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00614
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00618
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00624
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00628
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0062c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00630
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00638
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0063c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00640
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00644
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00648
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0064c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00650
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00654
			 0x80011111,                                                  // -9.7958E-41                                 /// 00658
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00660
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00668
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0066c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00670
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00678
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00680
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00684
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0xcb000000,                                                  // -8388608.0                                  /// 0068c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00690
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00694
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00698
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0069c
			 0x0c600000,                                                  // Leading 1s:                                 /// 006a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006ac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x33333333,                                                  // 4 random values                             /// 006d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006dc
			 0x7fc00001,                                                  // signaling NaN                               /// 006e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006ec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00700
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00704
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00710
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00714
			 0x00000000,                                                  // zero                                        /// 00718
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0071c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00720
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00724
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00728
			 0x0c780000,                                                  // Leading 1s:                                 /// 0072c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00730
			 0x0c600000,                                                  // Leading 1s:                                 /// 00734
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0073c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00740
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80000000,                                                  // -zero                                       /// 00748
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00750
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0075c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0076c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00770
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00778
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00784
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00788
			 0x80000000,                                                  // -zero                                       /// 0078c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00790
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00794
			 0x0e000001,                                                  // Trailing 1s:                                /// 00798
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x00000000,                                                  // zero                                        /// 007a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00800
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00808
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0080c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00810
			 0x7fc00001,                                                  // signaling NaN                               /// 00814
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00818
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0082c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00830
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00834
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0083c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00840
			 0x7fc00001,                                                  // signaling NaN                               /// 00844
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00848
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0084c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00850
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00860
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00868
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0086c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00870
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00874
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00878
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0087c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00880
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00884
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0088c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00890
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00894
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00898
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0089c
			 0x00000000,                                                  // zero                                        /// 008a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008a8
			 0x00000000,                                                  // zero                                        /// 008ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x3f028f5c,                                                  // 0.51                                        /// 008cc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00900
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00904
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00908
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0090c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00910
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00914
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00918
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00924
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00928
			 0xffc00001,                                                  // -signaling NaN                              /// 0092c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00930
			 0x80011111,                                                  // -9.7958E-41                                 /// 00934
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00938
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0093c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00940
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00944
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00950
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00954
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00958
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0095c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00960
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0096c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00970
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00974
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00978
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00980
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0c600000,                                                  // Leading 1s:                                 /// 00988
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00998
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0099c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80000000,                                                  // -zero                                       /// 009b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a08
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a10
			 0xcb000000,                                                  // -8388608.0                                  /// 00a14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x3f028f5c,                                                  // 0.51                                        /// 00a24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a28
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a2c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a34
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a58
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a78
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a80
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a88
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a90
			 0x55555555,                                                  // 4 random values                             /// 00a94
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00aa0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00aa4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00aac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ab0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ab4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ab8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00abc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ac4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ad0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ad4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00adc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ae4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ae8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0xffc00001,                                                  // -signaling NaN                              /// 00af0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00af8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00afc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b04
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b0c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b18
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b2c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b34
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b38
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b3c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b40
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b54
			 0x4b000000,                                                  // 8388608.0                                   /// 00b58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b5c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b60
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b78
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b88
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b90
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b94
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b9c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ba4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ba8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x7fc00001,                                                  // signaling NaN                               /// 00bb0
			 0x7fc00001,                                                  // signaling NaN                               /// 00bb4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bbc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x3f028f5c,                                                  // 0.51                                        /// 00bc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bc8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bcc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bd0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bd4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bdc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00be4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00be8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bf0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bf4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bf8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bfc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0xbf028f5c,                                                  // -0.51                                       /// 00c04
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c0c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x55555555,                                                  // 4 random values                             /// 00c20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x4b000000,                                                  // 8388608.0                                   /// 00c28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c30
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c3c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c48
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c68
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c74
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c98
			 0x7f800000,                                                  // inf                                         /// 00c9c
			 0xbf028f5c,                                                  // -0.51                                       /// 00ca0
			 0x00000000,                                                  // zero                                        /// 00ca4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ca8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cb0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cb8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cbc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x33333333,                                                  // 4 random values                             /// 00cd0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cdc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ce0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ce4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cf0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cf4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cfc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d10
			 0x00000000,                                                  // zero                                        /// 00d14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d1c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d48
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d4c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d50
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d60
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d64
			 0x00000000,                                                  // zero                                        /// 00d68
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d74
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d7c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x33333333,                                                  // 4 random values                             /// 00d94
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00da0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00da8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00db0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00db8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dbc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x7fc00001,                                                  // signaling NaN                               /// 00dc4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dc8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dd0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dd4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dd8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00de0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00de4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00df8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e00
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e14
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e2c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x7f800000,                                                  // inf                                         /// 00e3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e4c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e60
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e64
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e70
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e80
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e88
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e90
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e98
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ea0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ea8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00eac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00eb8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ebc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ec4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ed4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ed8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00edc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ee0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ee4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x7fc00001,                                                  // signaling NaN                               /// 00efc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f1c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f20
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f24
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f28
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f34
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f38
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f40
			 0xcb000000,                                                  // -8388608.0                                  /// 00f44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f54
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0xffc00001,                                                  // -signaling NaN                              /// 00f5c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0xffc00001,                                                  // -signaling NaN                              /// 00f68
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f70
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f74
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f94
			 0x00000000,                                                  // zero                                        /// 00f98
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f9c
			 0xbf028f5c,                                                  // -0.51                                       /// 00fa0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fa8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fb0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fb8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fbc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fd4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fd8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fe4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ff0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ff4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0c7ff800                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x90ed65b2, /// 0x0
			 0xebe4db6e, /// 0x4
			 0x109d33fa, /// 0x8
			 0xf5072d40, /// 0xc
			 0xc05bbb93, /// 0x10
			 0xf481a11c, /// 0x14
			 0x9af2bca7, /// 0x18
			 0x9b0854d6, /// 0x1c
			 0xa5e4bf5b, /// 0x20
			 0x04e45b51, /// 0x24
			 0x6c9368f9, /// 0x28
			 0xdb2f7569, /// 0x2c
			 0x7d4cc2df, /// 0x30
			 0xf6ab5dd1, /// 0x34
			 0x0a47f24d, /// 0x38
			 0x412f5036, /// 0x3c
			 0x7a13a89b, /// 0x40
			 0x5a80d8ab, /// 0x44
			 0x36f59c36, /// 0x48
			 0x95cb5ac7, /// 0x4c
			 0x80fd6b3f, /// 0x50
			 0x0a11ede4, /// 0x54
			 0x2a47124e, /// 0x58
			 0x06f179c1, /// 0x5c
			 0x67e13a81, /// 0x60
			 0x8b3ff718, /// 0x64
			 0x9ad45af1, /// 0x68
			 0x516c1650, /// 0x6c
			 0x62eb2ebe, /// 0x70
			 0x5c002719, /// 0x74
			 0x5b9faedd, /// 0x78
			 0xd3f6fd3c, /// 0x7c
			 0xbcff0cc5, /// 0x80
			 0x5813ec8b, /// 0x84
			 0x05c1aabb, /// 0x88
			 0x7350fb46, /// 0x8c
			 0x85308573, /// 0x90
			 0x06dd2296, /// 0x94
			 0x3a55e28c, /// 0x98
			 0xc9080eae, /// 0x9c
			 0x8190f1c8, /// 0xa0
			 0xacec1246, /// 0xa4
			 0x4fc56e06, /// 0xa8
			 0xb4a3bb7d, /// 0xac
			 0x9d8c61e5, /// 0xb0
			 0xb36e491b, /// 0xb4
			 0x2eaad726, /// 0xb8
			 0xad6bf981, /// 0xbc
			 0x49e001b8, /// 0xc0
			 0xc4238262, /// 0xc4
			 0x9ff91968, /// 0xc8
			 0xc404241a, /// 0xcc
			 0x6a789c0d, /// 0xd0
			 0x46e92c91, /// 0xd4
			 0x94695f9d, /// 0xd8
			 0x3776be89, /// 0xdc
			 0xb0df8b2f, /// 0xe0
			 0xdbd3556e, /// 0xe4
			 0x2b8ce139, /// 0xe8
			 0x3a600765, /// 0xec
			 0x1e0b69d2, /// 0xf0
			 0x56d8eb10, /// 0xf4
			 0x29ce4613, /// 0xf8
			 0xbea834f6, /// 0xfc
			 0xdd4b953f, /// 0x100
			 0xbb0ab2fc, /// 0x104
			 0x841054c2, /// 0x108
			 0xf57af560, /// 0x10c
			 0xa6c251c8, /// 0x110
			 0xf1cbb96c, /// 0x114
			 0x778374d7, /// 0x118
			 0x5d252762, /// 0x11c
			 0x7d98715d, /// 0x120
			 0xbb510bb3, /// 0x124
			 0x26c8d8a3, /// 0x128
			 0x05280b3c, /// 0x12c
			 0xd50fcc56, /// 0x130
			 0xf216463c, /// 0x134
			 0x8a180570, /// 0x138
			 0x3ffbd9d0, /// 0x13c
			 0xa192aacf, /// 0x140
			 0xe87a985b, /// 0x144
			 0x5d79d2b4, /// 0x148
			 0x4ed97db2, /// 0x14c
			 0x907b8414, /// 0x150
			 0x830c7133, /// 0x154
			 0xe835b505, /// 0x158
			 0x19a7fc34, /// 0x15c
			 0xaec3e6b5, /// 0x160
			 0x84855b74, /// 0x164
			 0x95bcde6f, /// 0x168
			 0xd1e6eb2a, /// 0x16c
			 0x83226efa, /// 0x170
			 0x1d7b287e, /// 0x174
			 0x011de311, /// 0x178
			 0xf34e34d2, /// 0x17c
			 0x4f823ed2, /// 0x180
			 0x5155e3e9, /// 0x184
			 0x560f335f, /// 0x188
			 0x0fa84c97, /// 0x18c
			 0xc70c6e29, /// 0x190
			 0x761cf616, /// 0x194
			 0x10f5e4b6, /// 0x198
			 0x21d2e847, /// 0x19c
			 0x8942fa19, /// 0x1a0
			 0xe4c4d640, /// 0x1a4
			 0x8c0a036c, /// 0x1a8
			 0x5291efa6, /// 0x1ac
			 0x86cdd302, /// 0x1b0
			 0x0e442cc4, /// 0x1b4
			 0xe7f4950b, /// 0x1b8
			 0x493eb76a, /// 0x1bc
			 0xe0109c9c, /// 0x1c0
			 0xf7ef20c4, /// 0x1c4
			 0x7fa84cd9, /// 0x1c8
			 0x9587b822, /// 0x1cc
			 0x2819a803, /// 0x1d0
			 0xa7cb2381, /// 0x1d4
			 0x5ed3761f, /// 0x1d8
			 0x0b8c8868, /// 0x1dc
			 0xcba177ca, /// 0x1e0
			 0xfe469cdf, /// 0x1e4
			 0xabc970ee, /// 0x1e8
			 0x84115751, /// 0x1ec
			 0x74d79b9e, /// 0x1f0
			 0x39336acc, /// 0x1f4
			 0x429485a0, /// 0x1f8
			 0x37059ef3, /// 0x1fc
			 0xa055e813, /// 0x200
			 0xd3abee46, /// 0x204
			 0xb35e05be, /// 0x208
			 0x0d5afd86, /// 0x20c
			 0x47912dc6, /// 0x210
			 0x92f25646, /// 0x214
			 0x1afc27aa, /// 0x218
			 0xfef7b703, /// 0x21c
			 0x3ef69373, /// 0x220
			 0x2cf90e0b, /// 0x224
			 0x9a02595f, /// 0x228
			 0x92c65464, /// 0x22c
			 0xade83ccd, /// 0x230
			 0x7f40da40, /// 0x234
			 0x1954a3e5, /// 0x238
			 0xf841dcde, /// 0x23c
			 0x446a54e2, /// 0x240
			 0x1b35599d, /// 0x244
			 0x84a9c648, /// 0x248
			 0xf69439a7, /// 0x24c
			 0x081418c2, /// 0x250
			 0x8a702798, /// 0x254
			 0xc9381e92, /// 0x258
			 0xe5ebc642, /// 0x25c
			 0x55f15afc, /// 0x260
			 0xf52f31e4, /// 0x264
			 0xed9e3a75, /// 0x268
			 0xc4657db6, /// 0x26c
			 0xc4641134, /// 0x270
			 0x00a8cd2c, /// 0x274
			 0xa7b3471f, /// 0x278
			 0x69fd13ab, /// 0x27c
			 0xde144766, /// 0x280
			 0xe0fdc6d9, /// 0x284
			 0x5aa921d4, /// 0x288
			 0x30babb2a, /// 0x28c
			 0x2a2f37f9, /// 0x290
			 0xb0d989f2, /// 0x294
			 0x47ad2e13, /// 0x298
			 0xe6fecacf, /// 0x29c
			 0x259cba15, /// 0x2a0
			 0x3b6b9f9f, /// 0x2a4
			 0x84b20872, /// 0x2a8
			 0x9aa0393b, /// 0x2ac
			 0x13ecf1e7, /// 0x2b0
			 0xbd7a98f6, /// 0x2b4
			 0xa4e83f79, /// 0x2b8
			 0x7d841ba2, /// 0x2bc
			 0x1ce26d80, /// 0x2c0
			 0x8ca7b22a, /// 0x2c4
			 0xd683367a, /// 0x2c8
			 0x4a107bde, /// 0x2cc
			 0x62ef2a6e, /// 0x2d0
			 0xe226766d, /// 0x2d4
			 0x23f7ce8c, /// 0x2d8
			 0x389f97ea, /// 0x2dc
			 0xcea889e1, /// 0x2e0
			 0xd72b2580, /// 0x2e4
			 0xb73c64c6, /// 0x2e8
			 0x35f38473, /// 0x2ec
			 0x0315322b, /// 0x2f0
			 0xde502913, /// 0x2f4
			 0xb3c864b7, /// 0x2f8
			 0x11fc9d8f, /// 0x2fc
			 0xcfd66787, /// 0x300
			 0xee28a24a, /// 0x304
			 0x6bf1fd58, /// 0x308
			 0x3a37860f, /// 0x30c
			 0xad40b286, /// 0x310
			 0x232c31c5, /// 0x314
			 0x959aee6d, /// 0x318
			 0x05a0cacf, /// 0x31c
			 0x6e6bdd3d, /// 0x320
			 0x1e14106a, /// 0x324
			 0xcadc6940, /// 0x328
			 0xdcdc6676, /// 0x32c
			 0xfc19cb2a, /// 0x330
			 0x78e70ac2, /// 0x334
			 0x26d62890, /// 0x338
			 0xdf58940c, /// 0x33c
			 0x45b004c1, /// 0x340
			 0xb7da150c, /// 0x344
			 0x7f1c0a87, /// 0x348
			 0xd875656d, /// 0x34c
			 0x990fa179, /// 0x350
			 0xc5232329, /// 0x354
			 0x76df57b6, /// 0x358
			 0x777ca8fc, /// 0x35c
			 0x7dfd6420, /// 0x360
			 0x3dad65e2, /// 0x364
			 0x36d4b408, /// 0x368
			 0x5023a446, /// 0x36c
			 0x517c536c, /// 0x370
			 0xf3bc7465, /// 0x374
			 0xec33190e, /// 0x378
			 0x631580fa, /// 0x37c
			 0xac7bca63, /// 0x380
			 0x3985436f, /// 0x384
			 0xa1c93bdd, /// 0x388
			 0xcd11a553, /// 0x38c
			 0x5be74350, /// 0x390
			 0x38da55a5, /// 0x394
			 0xc79c5fe9, /// 0x398
			 0x862c75c0, /// 0x39c
			 0xd3f0ca26, /// 0x3a0
			 0x8e876366, /// 0x3a4
			 0xd7df908f, /// 0x3a8
			 0x3b647be5, /// 0x3ac
			 0x85d79e94, /// 0x3b0
			 0x9f3e5f94, /// 0x3b4
			 0x71a1fb7d, /// 0x3b8
			 0x08fea7db, /// 0x3bc
			 0xd298dfa1, /// 0x3c0
			 0x9ddefaca, /// 0x3c4
			 0xd82a1aa5, /// 0x3c8
			 0x97da9790, /// 0x3cc
			 0x237aafab, /// 0x3d0
			 0xc126dd22, /// 0x3d4
			 0x568335dd, /// 0x3d8
			 0x78552ef1, /// 0x3dc
			 0x01f1ae65, /// 0x3e0
			 0x423f89a8, /// 0x3e4
			 0xd316074e, /// 0x3e8
			 0x236052d0, /// 0x3ec
			 0xf570f395, /// 0x3f0
			 0xcb910962, /// 0x3f4
			 0x976c91b7, /// 0x3f8
			 0xc07a9e85, /// 0x3fc
			 0xa615b1c1, /// 0x400
			 0x144b6639, /// 0x404
			 0xcd3a4f5a, /// 0x408
			 0x7ceabe06, /// 0x40c
			 0xcee4441a, /// 0x410
			 0x5301655d, /// 0x414
			 0xd2012a3d, /// 0x418
			 0x920bcd0f, /// 0x41c
			 0xaa4b806b, /// 0x420
			 0xa81a3723, /// 0x424
			 0x500ba71a, /// 0x428
			 0xcbffc7fa, /// 0x42c
			 0x32b66bfb, /// 0x430
			 0x229e56da, /// 0x434
			 0x0896a9d6, /// 0x438
			 0xb3c777bb, /// 0x43c
			 0xa87a3ea1, /// 0x440
			 0x67ed76dd, /// 0x444
			 0x7d2196f9, /// 0x448
			 0x0f85f3dd, /// 0x44c
			 0xd2cf833b, /// 0x450
			 0x197c41bf, /// 0x454
			 0xc634b0b8, /// 0x458
			 0xc4fcb375, /// 0x45c
			 0x024637ba, /// 0x460
			 0x691200e7, /// 0x464
			 0x5ed10a56, /// 0x468
			 0xd68a4b83, /// 0x46c
			 0xcbae0232, /// 0x470
			 0x48ca9906, /// 0x474
			 0x2e9ed2d4, /// 0x478
			 0xfbcca2cb, /// 0x47c
			 0x10e59193, /// 0x480
			 0xc96071ad, /// 0x484
			 0xbe2923b2, /// 0x488
			 0xc9adbb2f, /// 0x48c
			 0xc28393f5, /// 0x490
			 0x73191337, /// 0x494
			 0x4d096724, /// 0x498
			 0xb6a9b1ac, /// 0x49c
			 0x1b433121, /// 0x4a0
			 0xbd9769b3, /// 0x4a4
			 0x8285feb4, /// 0x4a8
			 0xb7032695, /// 0x4ac
			 0xb9a73a2e, /// 0x4b0
			 0xeb5a5e27, /// 0x4b4
			 0x26db998b, /// 0x4b8
			 0x9d6e2f00, /// 0x4bc
			 0x03b07e47, /// 0x4c0
			 0xc53e1db4, /// 0x4c4
			 0x150908e0, /// 0x4c8
			 0xcf95e199, /// 0x4cc
			 0xb1242209, /// 0x4d0
			 0x900650e3, /// 0x4d4
			 0xf8b247bf, /// 0x4d8
			 0xa9e373ae, /// 0x4dc
			 0xc1867cce, /// 0x4e0
			 0xdd761841, /// 0x4e4
			 0x1d8a670f, /// 0x4e8
			 0x1bb143ba, /// 0x4ec
			 0x7a7505fe, /// 0x4f0
			 0xc3cdf5e7, /// 0x4f4
			 0xc914a6b7, /// 0x4f8
			 0xa09e716b, /// 0x4fc
			 0xfb501ff4, /// 0x500
			 0x3a03b85e, /// 0x504
			 0x3d508adf, /// 0x508
			 0x86b61b91, /// 0x50c
			 0x4062280b, /// 0x510
			 0x14992237, /// 0x514
			 0xcc6cb345, /// 0x518
			 0x82125b62, /// 0x51c
			 0xad44a1aa, /// 0x520
			 0x54415d14, /// 0x524
			 0xf515c33b, /// 0x528
			 0xc36e4dba, /// 0x52c
			 0xfc95106d, /// 0x530
			 0x0db03693, /// 0x534
			 0x66879cc0, /// 0x538
			 0x7b25a58f, /// 0x53c
			 0x9756fb17, /// 0x540
			 0xc5864058, /// 0x544
			 0xe01e1fb2, /// 0x548
			 0x04f1ebbf, /// 0x54c
			 0xe0ef66fc, /// 0x550
			 0xb14a485a, /// 0x554
			 0x09db5dbd, /// 0x558
			 0x5d9b8d5c, /// 0x55c
			 0xc8234f80, /// 0x560
			 0x10ab669e, /// 0x564
			 0x61152fbc, /// 0x568
			 0x08ea545f, /// 0x56c
			 0x0b1d7521, /// 0x570
			 0x184ce2bb, /// 0x574
			 0x35c91b75, /// 0x578
			 0xaddbb446, /// 0x57c
			 0xe9938ece, /// 0x580
			 0x478ae8aa, /// 0x584
			 0x3671a65e, /// 0x588
			 0x630607f2, /// 0x58c
			 0xbff69107, /// 0x590
			 0xc05b6c9e, /// 0x594
			 0x201f82c3, /// 0x598
			 0x27bf6b13, /// 0x59c
			 0x053bb3d7, /// 0x5a0
			 0x43e8926a, /// 0x5a4
			 0x8918e46a, /// 0x5a8
			 0x24a74772, /// 0x5ac
			 0xad2096b0, /// 0x5b0
			 0x06b44fcf, /// 0x5b4
			 0x417c192e, /// 0x5b8
			 0xfa65a67d, /// 0x5bc
			 0x6fea8f5e, /// 0x5c0
			 0x0f400324, /// 0x5c4
			 0xecad6478, /// 0x5c8
			 0xa35c3dac, /// 0x5cc
			 0xe286123c, /// 0x5d0
			 0x1360aa38, /// 0x5d4
			 0x2d661782, /// 0x5d8
			 0xe619f76d, /// 0x5dc
			 0x918fb0fd, /// 0x5e0
			 0x5e70f192, /// 0x5e4
			 0xb838684d, /// 0x5e8
			 0x63f106cc, /// 0x5ec
			 0x45c6c3ed, /// 0x5f0
			 0x3182b48c, /// 0x5f4
			 0xee5f2c12, /// 0x5f8
			 0x7d4c8768, /// 0x5fc
			 0xa2377304, /// 0x600
			 0xe17d0589, /// 0x604
			 0xc39bf89d, /// 0x608
			 0x0b4890d5, /// 0x60c
			 0x2c00ff56, /// 0x610
			 0x6a8eb595, /// 0x614
			 0x22d09826, /// 0x618
			 0xb9f1fc0a, /// 0x61c
			 0x2b05da35, /// 0x620
			 0x74b1d3a5, /// 0x624
			 0x986ced3d, /// 0x628
			 0x96d062e2, /// 0x62c
			 0xe7f93053, /// 0x630
			 0x25413470, /// 0x634
			 0xcfb6baa3, /// 0x638
			 0x98110e66, /// 0x63c
			 0xa61e4265, /// 0x640
			 0x3b7f1bbe, /// 0x644
			 0x0d2aea50, /// 0x648
			 0x3479de3f, /// 0x64c
			 0x63bb9f9e, /// 0x650
			 0x5e121fff, /// 0x654
			 0x2b8929a7, /// 0x658
			 0x22c575d0, /// 0x65c
			 0x5a88f834, /// 0x660
			 0x3ce10a81, /// 0x664
			 0x55420a49, /// 0x668
			 0x6ed6b597, /// 0x66c
			 0x470456da, /// 0x670
			 0xfc904be8, /// 0x674
			 0xa59b881d, /// 0x678
			 0x1eff6086, /// 0x67c
			 0x995f1c9a, /// 0x680
			 0x810c6df6, /// 0x684
			 0x03c5e196, /// 0x688
			 0xe46985bb, /// 0x68c
			 0xffa084d1, /// 0x690
			 0x8522c129, /// 0x694
			 0x314f88b1, /// 0x698
			 0x7c5539c8, /// 0x69c
			 0xdc102358, /// 0x6a0
			 0xade6d806, /// 0x6a4
			 0x747a7077, /// 0x6a8
			 0x6e7048f3, /// 0x6ac
			 0x7d82afd0, /// 0x6b0
			 0xf3bab353, /// 0x6b4
			 0x7a3c6c19, /// 0x6b8
			 0x7e3aea92, /// 0x6bc
			 0xbdb488d8, /// 0x6c0
			 0x8becbd57, /// 0x6c4
			 0xdf17c8fa, /// 0x6c8
			 0x77e8cdce, /// 0x6cc
			 0xf491ec2d, /// 0x6d0
			 0xedf6abef, /// 0x6d4
			 0x903ab400, /// 0x6d8
			 0xec06a921, /// 0x6dc
			 0x959b6876, /// 0x6e0
			 0xbee7599c, /// 0x6e4
			 0xdb931bd4, /// 0x6e8
			 0x9d3a0786, /// 0x6ec
			 0x2d81a98c, /// 0x6f0
			 0xe80e2bdb, /// 0x6f4
			 0x31a84906, /// 0x6f8
			 0x19e65af6, /// 0x6fc
			 0x3da38df8, /// 0x700
			 0xd2e7ba93, /// 0x704
			 0xfcfd2530, /// 0x708
			 0x4aa1b1d0, /// 0x70c
			 0x8536e5dd, /// 0x710
			 0xa1457ee2, /// 0x714
			 0x7eb0233e, /// 0x718
			 0x5f76330d, /// 0x71c
			 0xb3b3ba79, /// 0x720
			 0x1c3f9048, /// 0x724
			 0x2964419c, /// 0x728
			 0xc28864b8, /// 0x72c
			 0x9d01aa30, /// 0x730
			 0x8e4f7b1b, /// 0x734
			 0xc9f9a010, /// 0x738
			 0xb423cfae, /// 0x73c
			 0x9b9a30e6, /// 0x740
			 0x5b9dfa12, /// 0x744
			 0x8ff5b8d9, /// 0x748
			 0x17c2e90c, /// 0x74c
			 0x69595740, /// 0x750
			 0xb5809d98, /// 0x754
			 0x18657965, /// 0x758
			 0xe135c678, /// 0x75c
			 0x3207584d, /// 0x760
			 0xcf65023a, /// 0x764
			 0xaf3740e9, /// 0x768
			 0x1f0676fc, /// 0x76c
			 0xc58e9765, /// 0x770
			 0x88dc3a21, /// 0x774
			 0x3abab49b, /// 0x778
			 0xa80e9c14, /// 0x77c
			 0x6c96ca0b, /// 0x780
			 0xeca0ce4a, /// 0x784
			 0x382ce5cf, /// 0x788
			 0x7bdec15d, /// 0x78c
			 0x73a2c77c, /// 0x790
			 0x54e082f8, /// 0x794
			 0xc3486b35, /// 0x798
			 0xcfd18418, /// 0x79c
			 0x829dac56, /// 0x7a0
			 0xacc86c8d, /// 0x7a4
			 0xb8018435, /// 0x7a8
			 0xb7d56c78, /// 0x7ac
			 0x826e6a42, /// 0x7b0
			 0x03eb8ec4, /// 0x7b4
			 0x0c5a7ed5, /// 0x7b8
			 0xe1afff36, /// 0x7bc
			 0xfa199c40, /// 0x7c0
			 0xdc9f277f, /// 0x7c4
			 0xd305d464, /// 0x7c8
			 0x781cb309, /// 0x7cc
			 0xdc12a81c, /// 0x7d0
			 0xc66b98d8, /// 0x7d4
			 0x1d22bd7d, /// 0x7d8
			 0x72522c61, /// 0x7dc
			 0x89ec8af8, /// 0x7e0
			 0x8e3de8e8, /// 0x7e4
			 0x64f22290, /// 0x7e8
			 0xc13885e7, /// 0x7ec
			 0x68d48a18, /// 0x7f0
			 0xcf26f8e9, /// 0x7f4
			 0xad78e36e, /// 0x7f8
			 0xbec97fa3, /// 0x7fc
			 0xb82543ac, /// 0x800
			 0x28c55110, /// 0x804
			 0xa6c14737, /// 0x808
			 0x718e4bbf, /// 0x80c
			 0xc0901329, /// 0x810
			 0x5e25aa04, /// 0x814
			 0x4ddf190e, /// 0x818
			 0x4ca3b742, /// 0x81c
			 0xab50d5eb, /// 0x820
			 0xaf94c2b4, /// 0x824
			 0x4f621fb8, /// 0x828
			 0x913e8c70, /// 0x82c
			 0x80516b10, /// 0x830
			 0x46f63d4a, /// 0x834
			 0x0a4679a5, /// 0x838
			 0x520109dc, /// 0x83c
			 0xc252cd00, /// 0x840
			 0x3b9679c8, /// 0x844
			 0x7947c924, /// 0x848
			 0x9f6cadb2, /// 0x84c
			 0x5d0c1e4d, /// 0x850
			 0x20c589cc, /// 0x854
			 0x15f72439, /// 0x858
			 0x146b8d6b, /// 0x85c
			 0xf504e8b8, /// 0x860
			 0xebc53d71, /// 0x864
			 0x2862cdee, /// 0x868
			 0xe73aaf05, /// 0x86c
			 0x71875833, /// 0x870
			 0x3e7a5ba3, /// 0x874
			 0x020a168a, /// 0x878
			 0x95a0aa4c, /// 0x87c
			 0x5e59a8af, /// 0x880
			 0xb6d0780c, /// 0x884
			 0x04b903e1, /// 0x888
			 0xf486783e, /// 0x88c
			 0xebaa9c11, /// 0x890
			 0xefe710a8, /// 0x894
			 0x84ef6c3b, /// 0x898
			 0x840096bc, /// 0x89c
			 0x1ade89ab, /// 0x8a0
			 0x86de34ff, /// 0x8a4
			 0x1555bd26, /// 0x8a8
			 0x740771a6, /// 0x8ac
			 0x1a5f6b14, /// 0x8b0
			 0x1253cdb4, /// 0x8b4
			 0x58c9dabf, /// 0x8b8
			 0x54585a56, /// 0x8bc
			 0x1ab27cc6, /// 0x8c0
			 0x0b891b70, /// 0x8c4
			 0x261785ec, /// 0x8c8
			 0x6ba503f8, /// 0x8cc
			 0x02f99b1f, /// 0x8d0
			 0x4a0c2460, /// 0x8d4
			 0xe9b4617d, /// 0x8d8
			 0xd86670f5, /// 0x8dc
			 0x36461c38, /// 0x8e0
			 0x08211033, /// 0x8e4
			 0x01a92fa0, /// 0x8e8
			 0x19f8be48, /// 0x8ec
			 0x12441a89, /// 0x8f0
			 0x60cf3c7f, /// 0x8f4
			 0x5ac0172f, /// 0x8f8
			 0x35ea0435, /// 0x8fc
			 0xfbbca87c, /// 0x900
			 0xdeebbeb1, /// 0x904
			 0xc0084ef9, /// 0x908
			 0x72817b9f, /// 0x90c
			 0xa87dae31, /// 0x910
			 0x5df673af, /// 0x914
			 0x053e7697, /// 0x918
			 0xf8ebf258, /// 0x91c
			 0xe455de4a, /// 0x920
			 0x61a7f10a, /// 0x924
			 0xa1d8072c, /// 0x928
			 0x54e4a6cf, /// 0x92c
			 0x0e945d25, /// 0x930
			 0x5275abd2, /// 0x934
			 0x10e46c60, /// 0x938
			 0xef7a26ce, /// 0x93c
			 0x4329c568, /// 0x940
			 0x34476bd7, /// 0x944
			 0x31eb4ea9, /// 0x948
			 0x85184eaa, /// 0x94c
			 0xe41751e0, /// 0x950
			 0xf43a091e, /// 0x954
			 0xddedbea3, /// 0x958
			 0x814bf272, /// 0x95c
			 0x1074e27f, /// 0x960
			 0x49eada78, /// 0x964
			 0x1134b7ab, /// 0x968
			 0xabf40711, /// 0x96c
			 0xaf9d1e4e, /// 0x970
			 0x2eb0c0ae, /// 0x974
			 0xd74a3218, /// 0x978
			 0xaff27cf5, /// 0x97c
			 0xa1129019, /// 0x980
			 0x28c1019f, /// 0x984
			 0xb75ad380, /// 0x988
			 0xcb73b88f, /// 0x98c
			 0xab5bd295, /// 0x990
			 0x6f323147, /// 0x994
			 0x4c6dbb54, /// 0x998
			 0x55057c8d, /// 0x99c
			 0x08840aea, /// 0x9a0
			 0x4fddd7cf, /// 0x9a4
			 0x5758853c, /// 0x9a8
			 0xfce5b967, /// 0x9ac
			 0xda9b1e53, /// 0x9b0
			 0xa4319af3, /// 0x9b4
			 0xbddfe54c, /// 0x9b8
			 0x5b2fa5f7, /// 0x9bc
			 0x2b56c4c6, /// 0x9c0
			 0x0a3e177a, /// 0x9c4
			 0x2af125f3, /// 0x9c8
			 0x61eb6627, /// 0x9cc
			 0x083c7abc, /// 0x9d0
			 0x08d2ac9d, /// 0x9d4
			 0xae5037bf, /// 0x9d8
			 0xeea841a1, /// 0x9dc
			 0x5d75aeca, /// 0x9e0
			 0x9b535467, /// 0x9e4
			 0xb12af888, /// 0x9e8
			 0x9b12575f, /// 0x9ec
			 0xb07c9e71, /// 0x9f0
			 0xf087eb7c, /// 0x9f4
			 0x2d390554, /// 0x9f8
			 0xabc26653, /// 0x9fc
			 0x5819b23c, /// 0xa00
			 0xd126460a, /// 0xa04
			 0x3f2a7ae2, /// 0xa08
			 0x8a04a40a, /// 0xa0c
			 0x96165106, /// 0xa10
			 0x90736549, /// 0xa14
			 0xdeccd00a, /// 0xa18
			 0xcbfa2abc, /// 0xa1c
			 0x5ea1cf31, /// 0xa20
			 0xfe4025da, /// 0xa24
			 0x8b92bbc6, /// 0xa28
			 0x692e98ef, /// 0xa2c
			 0xa98f58cc, /// 0xa30
			 0x2529ae08, /// 0xa34
			 0x45ac23aa, /// 0xa38
			 0x02ec99ae, /// 0xa3c
			 0xf1c0f366, /// 0xa40
			 0xbe6ccb42, /// 0xa44
			 0xc6cc3ff4, /// 0xa48
			 0x64610c1e, /// 0xa4c
			 0xcf6c486f, /// 0xa50
			 0x2c5cca8b, /// 0xa54
			 0xf5eb0e65, /// 0xa58
			 0x67d4d7ce, /// 0xa5c
			 0x59c5d611, /// 0xa60
			 0x422a224f, /// 0xa64
			 0xa253d12b, /// 0xa68
			 0xed3b6525, /// 0xa6c
			 0x4a2b9a3e, /// 0xa70
			 0x6d184a38, /// 0xa74
			 0x1fbc256d, /// 0xa78
			 0xa42d668d, /// 0xa7c
			 0xfcad6c6f, /// 0xa80
			 0xecb9e741, /// 0xa84
			 0xaeb6a3c3, /// 0xa88
			 0xd26edbac, /// 0xa8c
			 0x95922e0e, /// 0xa90
			 0xaa53bed2, /// 0xa94
			 0x6d1dd077, /// 0xa98
			 0x6f4020dd, /// 0xa9c
			 0x5871aa5e, /// 0xaa0
			 0x9345a89d, /// 0xaa4
			 0xcd415ac8, /// 0xaa8
			 0xaf11ead2, /// 0xaac
			 0xd75b1b9a, /// 0xab0
			 0x1ab58ef5, /// 0xab4
			 0x0e05dd71, /// 0xab8
			 0x8ae6a11f, /// 0xabc
			 0x64873483, /// 0xac0
			 0xc8af30f5, /// 0xac4
			 0x9b4fd375, /// 0xac8
			 0x284e0a3c, /// 0xacc
			 0xd99abee3, /// 0xad0
			 0x24ed8dc4, /// 0xad4
			 0x8faeec22, /// 0xad8
			 0x9b124244, /// 0xadc
			 0x615844e0, /// 0xae0
			 0xb373df38, /// 0xae4
			 0x9d00d108, /// 0xae8
			 0x55b05607, /// 0xaec
			 0xa8e0137d, /// 0xaf0
			 0x37f969ec, /// 0xaf4
			 0x93d53c75, /// 0xaf8
			 0x4cf515f9, /// 0xafc
			 0x408dae6e, /// 0xb00
			 0x47316830, /// 0xb04
			 0xb1b23e0b, /// 0xb08
			 0x60c30b2a, /// 0xb0c
			 0x60a0c6a6, /// 0xb10
			 0x8d11f928, /// 0xb14
			 0x07b80fa3, /// 0xb18
			 0x6a12b139, /// 0xb1c
			 0x5a387391, /// 0xb20
			 0xde488906, /// 0xb24
			 0xd0313b88, /// 0xb28
			 0xf93a2b34, /// 0xb2c
			 0xb2d34dc9, /// 0xb30
			 0xde86dbe7, /// 0xb34
			 0xd963cf3c, /// 0xb38
			 0x00639412, /// 0xb3c
			 0xeead6ec4, /// 0xb40
			 0xe3b298cc, /// 0xb44
			 0x2dc42b41, /// 0xb48
			 0xece2c2cd, /// 0xb4c
			 0xb4af7b5d, /// 0xb50
			 0xacc339b5, /// 0xb54
			 0xf0c8dc66, /// 0xb58
			 0xb0264a24, /// 0xb5c
			 0x55155bfd, /// 0xb60
			 0xd08e5c46, /// 0xb64
			 0x544ab6a7, /// 0xb68
			 0x277d7df0, /// 0xb6c
			 0xcc95df19, /// 0xb70
			 0x7ad0469e, /// 0xb74
			 0x81aa62a9, /// 0xb78
			 0x3f36bf6e, /// 0xb7c
			 0x0d74fce5, /// 0xb80
			 0x6d883b13, /// 0xb84
			 0x1ff40fe9, /// 0xb88
			 0xf804f49a, /// 0xb8c
			 0x719820b8, /// 0xb90
			 0xe0f89b18, /// 0xb94
			 0xcdc05f35, /// 0xb98
			 0xbed9168e, /// 0xb9c
			 0xd08455ef, /// 0xba0
			 0xeeef483c, /// 0xba4
			 0xd82d8f6e, /// 0xba8
			 0x09b07db7, /// 0xbac
			 0xb3719eab, /// 0xbb0
			 0xc3f34ab0, /// 0xbb4
			 0x30d30363, /// 0xbb8
			 0x027ad527, /// 0xbbc
			 0xcd206f16, /// 0xbc0
			 0x5ca1006a, /// 0xbc4
			 0x81407346, /// 0xbc8
			 0x56d9665a, /// 0xbcc
			 0x15a3d178, /// 0xbd0
			 0x47aaa583, /// 0xbd4
			 0xf0696987, /// 0xbd8
			 0x44db939f, /// 0xbdc
			 0x63d09a88, /// 0xbe0
			 0x5c08ece0, /// 0xbe4
			 0x38fb7ec6, /// 0xbe8
			 0x456ba83c, /// 0xbec
			 0x044ca01f, /// 0xbf0
			 0xbf65da3b, /// 0xbf4
			 0x20bff36e, /// 0xbf8
			 0xd7f9f8a4, /// 0xbfc
			 0x59ae7417, /// 0xc00
			 0x376e5134, /// 0xc04
			 0xfdd456b8, /// 0xc08
			 0x232760f4, /// 0xc0c
			 0x240226de, /// 0xc10
			 0xa1908a4b, /// 0xc14
			 0x824dfd1a, /// 0xc18
			 0x71acaae1, /// 0xc1c
			 0xbbc3358a, /// 0xc20
			 0x801c0d02, /// 0xc24
			 0x7959ca52, /// 0xc28
			 0xb606ba9b, /// 0xc2c
			 0x2fc3fb4a, /// 0xc30
			 0x735e3bb7, /// 0xc34
			 0x30dfb68d, /// 0xc38
			 0x7697b2ea, /// 0xc3c
			 0x84ad120c, /// 0xc40
			 0x5468e273, /// 0xc44
			 0x19accf28, /// 0xc48
			 0x0128c7e4, /// 0xc4c
			 0x7f3977a2, /// 0xc50
			 0xabe7afb9, /// 0xc54
			 0x482b89c2, /// 0xc58
			 0xb7d0f81d, /// 0xc5c
			 0xb6e71841, /// 0xc60
			 0xc73a807f, /// 0xc64
			 0x4e6ed98d, /// 0xc68
			 0x2d4f6e48, /// 0xc6c
			 0x0335765c, /// 0xc70
			 0xbf5677f7, /// 0xc74
			 0xcb0b415a, /// 0xc78
			 0xae78b541, /// 0xc7c
			 0xcebd1b9e, /// 0xc80
			 0xd978b777, /// 0xc84
			 0x554cc029, /// 0xc88
			 0xe229eee3, /// 0xc8c
			 0x5efa794b, /// 0xc90
			 0xffe9c8cf, /// 0xc94
			 0x6aa216ed, /// 0xc98
			 0x953dc720, /// 0xc9c
			 0x13dc26e8, /// 0xca0
			 0x59dde33f, /// 0xca4
			 0x54e13b64, /// 0xca8
			 0x160cdfd5, /// 0xcac
			 0xca00f5d7, /// 0xcb0
			 0x9a299317, /// 0xcb4
			 0x954d9b1f, /// 0xcb8
			 0x8e592383, /// 0xcbc
			 0x4b0570e0, /// 0xcc0
			 0xd87cab6d, /// 0xcc4
			 0x889848da, /// 0xcc8
			 0x2fb5dafc, /// 0xccc
			 0x4c10698a, /// 0xcd0
			 0x3b50de6e, /// 0xcd4
			 0x25ced0e2, /// 0xcd8
			 0x253cfa20, /// 0xcdc
			 0x599a4818, /// 0xce0
			 0x829f42aa, /// 0xce4
			 0xa1b22d62, /// 0xce8
			 0xa22ea5ec, /// 0xcec
			 0x5f345212, /// 0xcf0
			 0xcd4a424a, /// 0xcf4
			 0x70463c7b, /// 0xcf8
			 0x17f0ae7f, /// 0xcfc
			 0xaa8070be, /// 0xd00
			 0xda1cdd84, /// 0xd04
			 0x0bfe1216, /// 0xd08
			 0x6f200d50, /// 0xd0c
			 0x18008079, /// 0xd10
			 0xb386180a, /// 0xd14
			 0x011fa95e, /// 0xd18
			 0x86a318ca, /// 0xd1c
			 0xcc81f080, /// 0xd20
			 0xc0bcc4e7, /// 0xd24
			 0x58646ee7, /// 0xd28
			 0xccaff0f1, /// 0xd2c
			 0x21238f83, /// 0xd30
			 0x14e8c8de, /// 0xd34
			 0x95c954ca, /// 0xd38
			 0x3bc1ffd2, /// 0xd3c
			 0x917dafa3, /// 0xd40
			 0xe65bba46, /// 0xd44
			 0xb2530d9d, /// 0xd48
			 0x73fbf250, /// 0xd4c
			 0x7aeb0310, /// 0xd50
			 0x5c3c9a4d, /// 0xd54
			 0x437f21ab, /// 0xd58
			 0x1d38d86f, /// 0xd5c
			 0xe2446243, /// 0xd60
			 0x67301ce4, /// 0xd64
			 0xe24fadf9, /// 0xd68
			 0x5441f331, /// 0xd6c
			 0xa63cdf29, /// 0xd70
			 0xcbe5cdf5, /// 0xd74
			 0xb0fdb71e, /// 0xd78
			 0xfd9b03c7, /// 0xd7c
			 0x1f6c028a, /// 0xd80
			 0x652b15b8, /// 0xd84
			 0xb9082543, /// 0xd88
			 0xe5ec0a4b, /// 0xd8c
			 0x55cd1fcc, /// 0xd90
			 0xb9f8cb38, /// 0xd94
			 0x4830e1df, /// 0xd98
			 0x2775e0bc, /// 0xd9c
			 0xda9f5f07, /// 0xda0
			 0x1eee22c0, /// 0xda4
			 0x547efa73, /// 0xda8
			 0xa8973855, /// 0xdac
			 0x0f017a52, /// 0xdb0
			 0xd32ef77a, /// 0xdb4
			 0xa7391f97, /// 0xdb8
			 0xc2b05366, /// 0xdbc
			 0xdcbb4f4c, /// 0xdc0
			 0x2e116290, /// 0xdc4
			 0xea7a2fe0, /// 0xdc8
			 0x76b3b51b, /// 0xdcc
			 0x74a653ec, /// 0xdd0
			 0xbd99c516, /// 0xdd4
			 0x10886f3b, /// 0xdd8
			 0x8ec8f491, /// 0xddc
			 0xb37ac5ab, /// 0xde0
			 0xe8f18a18, /// 0xde4
			 0x3c8f97dd, /// 0xde8
			 0x0cf6f6c6, /// 0xdec
			 0x86ae6845, /// 0xdf0
			 0xae59fbc7, /// 0xdf4
			 0x4058278d, /// 0xdf8
			 0x434bc9a8, /// 0xdfc
			 0x7a37dc59, /// 0xe00
			 0x009c1bef, /// 0xe04
			 0x9664b0cb, /// 0xe08
			 0xf6fd116f, /// 0xe0c
			 0x17e1301f, /// 0xe10
			 0xa55d2302, /// 0xe14
			 0x1db24b59, /// 0xe18
			 0x4bc44a2d, /// 0xe1c
			 0x63843667, /// 0xe20
			 0xe14bed8e, /// 0xe24
			 0x78c01259, /// 0xe28
			 0xf373865e, /// 0xe2c
			 0x0510d26c, /// 0xe30
			 0xe5c5657b, /// 0xe34
			 0x96400c5a, /// 0xe38
			 0xdca8a286, /// 0xe3c
			 0x2b9a10ba, /// 0xe40
			 0x89690bda, /// 0xe44
			 0x5d97fbba, /// 0xe48
			 0x42b1ad42, /// 0xe4c
			 0xe3e31561, /// 0xe50
			 0xccd5d148, /// 0xe54
			 0xb227bd48, /// 0xe58
			 0x0efafa42, /// 0xe5c
			 0xccab7a4e, /// 0xe60
			 0xc511ef27, /// 0xe64
			 0x97116bbe, /// 0xe68
			 0x22da2369, /// 0xe6c
			 0x5a0d64f4, /// 0xe70
			 0x1ea99a97, /// 0xe74
			 0x78f32b38, /// 0xe78
			 0x500cb50d, /// 0xe7c
			 0x622d1f98, /// 0xe80
			 0xd3215a7b, /// 0xe84
			 0x87cebe4d, /// 0xe88
			 0xd60d4574, /// 0xe8c
			 0xab2eb546, /// 0xe90
			 0x16d82772, /// 0xe94
			 0xdbdd8138, /// 0xe98
			 0x0b482b0d, /// 0xe9c
			 0x3a0d8b02, /// 0xea0
			 0xd43515df, /// 0xea4
			 0x33d9d1f9, /// 0xea8
			 0xa55f87dc, /// 0xeac
			 0xcdc8d37e, /// 0xeb0
			 0xc3300292, /// 0xeb4
			 0xd9b14a47, /// 0xeb8
			 0x9dca2b56, /// 0xebc
			 0x4ddddf8f, /// 0xec0
			 0xb715fad0, /// 0xec4
			 0x6ae3ce03, /// 0xec8
			 0x456b4132, /// 0xecc
			 0xcaa049ff, /// 0xed0
			 0x8055038d, /// 0xed4
			 0x4bde2530, /// 0xed8
			 0x6984d5d9, /// 0xedc
			 0x9e9934b3, /// 0xee0
			 0x17b34f45, /// 0xee4
			 0x24900d2a, /// 0xee8
			 0xcc6f0135, /// 0xeec
			 0xe2640851, /// 0xef0
			 0x5663fd77, /// 0xef4
			 0x672e9db5, /// 0xef8
			 0xb7ff122d, /// 0xefc
			 0x1b5bd18b, /// 0xf00
			 0x6682f6ab, /// 0xf04
			 0x6cef2ffb, /// 0xf08
			 0x9049b687, /// 0xf0c
			 0x655c6997, /// 0xf10
			 0x2912b365, /// 0xf14
			 0x1fc0535e, /// 0xf18
			 0x7a477fe1, /// 0xf1c
			 0x613457af, /// 0xf20
			 0x24d1a83f, /// 0xf24
			 0xe24bf4ae, /// 0xf28
			 0xaf01b641, /// 0xf2c
			 0x26ffc9c3, /// 0xf30
			 0x68c178f8, /// 0xf34
			 0x2d58b67d, /// 0xf38
			 0xd2bff0af, /// 0xf3c
			 0xf04b6788, /// 0xf40
			 0xc4e3ae83, /// 0xf44
			 0x6a78c404, /// 0xf48
			 0x45fa9279, /// 0xf4c
			 0xaf3bd599, /// 0xf50
			 0x3ade2b5e, /// 0xf54
			 0xabf5fe9f, /// 0xf58
			 0xdf292f9a, /// 0xf5c
			 0xfc976f8e, /// 0xf60
			 0x335f7e1e, /// 0xf64
			 0xc8ded5ca, /// 0xf68
			 0x3a46acb9, /// 0xf6c
			 0x3601d0c3, /// 0xf70
			 0xc2b93b2b, /// 0xf74
			 0x70b2c901, /// 0xf78
			 0xceb6c9ea, /// 0xf7c
			 0x02c73c4c, /// 0xf80
			 0xdd2d6fd1, /// 0xf84
			 0xd156968f, /// 0xf88
			 0xe2ef78b1, /// 0xf8c
			 0xc877cc2d, /// 0xf90
			 0xee53df74, /// 0xf94
			 0x6bbdc0d9, /// 0xf98
			 0xff3bb620, /// 0xf9c
			 0x4e8abe03, /// 0xfa0
			 0x34901025, /// 0xfa4
			 0x2631f82f, /// 0xfa8
			 0x8f9d9782, /// 0xfac
			 0x3591be83, /// 0xfb0
			 0xb67b7552, /// 0xfb4
			 0x4da63238, /// 0xfb8
			 0x96e28cb3, /// 0xfbc
			 0x40105e14, /// 0xfc0
			 0x3dc231ab, /// 0xfc4
			 0xf3b6fee8, /// 0xfc8
			 0x3c342d02, /// 0xfcc
			 0xfcff24b8, /// 0xfd0
			 0x2afee785, /// 0xfd4
			 0x7560bfcd, /// 0xfd8
			 0xacc9d945, /// 0xfdc
			 0x847419e8, /// 0xfe0
			 0xffa019c3, /// 0xfe4
			 0x810e27db, /// 0xfe8
			 0x39500f89, /// 0xfec
			 0xb7dd62d4, /// 0xff0
			 0xc96ae784, /// 0xff4
			 0x783ecf22, /// 0xff8
			 0x7bad779f /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00000
			 0x0e000007,                                                  // Trailing 1s:                                /// 00004
			 0x0e000001,                                                  // Trailing 1s:                                /// 00008
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x7fc00001,                                                  // signaling NaN                               /// 00010
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00018
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0001c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00024
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00028
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00040
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00044
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00048
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0004c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00050
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00058
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0005c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00060
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00064
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0006c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00070
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00074
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00078
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0007c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00080
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00088
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0008c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00090
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0xcb000000,                                                  // -8388608.0                                  /// 000c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000cc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000d0
			 0x80000000,                                                  // -zero                                       /// 000d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 000f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00100
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00108
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0010c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0011c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00120
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00124
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0012c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00130
			 0x0e000001,                                                  // Trailing 1s:                                /// 00134
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0013c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00144
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00148
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0014c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00150
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00154
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0015c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00160
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00164
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00168
			 0x0e000007,                                                  // Trailing 1s:                                /// 0016c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0xcb000000,                                                  // -8388608.0                                  /// 00174
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00178
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0017c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00180
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00184
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00188
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00190
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00194
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00198
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0019c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001ac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0xcb000000,                                                  // -8388608.0                                  /// 001d8
			 0x80000000,                                                  // -zero                                       /// 001dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 001e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 001f0
			 0xcb000000,                                                  // -8388608.0                                  /// 001f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x7f800000,                                                  // inf                                         /// 001fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00200
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00204
			 0x0c780000,                                                  // Leading 1s:                                 /// 00208
			 0xff800000,                                                  // -inf                                        /// 0020c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00210
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00218
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x7f800000,                                                  // inf                                         /// 00220
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0c780000,                                                  // Leading 1s:                                 /// 00228
			 0x7f800000,                                                  // inf                                         /// 0022c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00230
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00234
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00238
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0023c
			 0x4b000000,                                                  // 8388608.0                                   /// 00240
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00244
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00248
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0024c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00250
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00254
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00258
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0025c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00260
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00264
			 0x55555555,                                                  // 4 random values                             /// 00268
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0026c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00270
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00274
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00278
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0027c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x80000000,                                                  // -zero                                       /// 00284
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00288
			 0x55555555,                                                  // 4 random values                             /// 0028c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00290
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00294
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00298
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0029c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002b4
			 0x00000000,                                                  // zero                                        /// 002b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00300
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00304
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00308
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0030c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0x33333333,                                                  // 4 random values                             /// 00314
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0031c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00320
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00328
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0032c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00330
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00338
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0033c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00340
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0xcb000000,                                                  // -8388608.0                                  /// 00354
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0035c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00360
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00364
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00368
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0036c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00370
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00378
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x00000000,                                                  // zero                                        /// 00384
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00388
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0038c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00390
			 0x7f800000,                                                  // inf                                         /// 00394
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00398
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003b8
			 0x00000000,                                                  // zero                                        /// 003bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x7f800000,                                                  // inf                                         /// 003c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00400
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00404
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00408
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0040c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00414
			 0x3f028f5c,                                                  // 0.51                                        /// 00418
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00424
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00428
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0042c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00430
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0e000003,                                                  // Trailing 1s:                                /// 00438
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00440
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00444
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00448
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0044c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00458
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00464
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00468
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0046c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00470
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00474
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0047c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00480
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00484
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0048c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00490
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00494
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0049c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 004bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004c4
			 0xcb000000,                                                  // -8388608.0                                  /// 004c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004dc
			 0xbf028f5c,                                                  // -0.51                                       /// 004e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00504
			 0x0c400000,                                                  // Leading 1s:                                 /// 00508
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0050c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00510
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0051c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0xffc00001,                                                  // -signaling NaN                              /// 00528
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0052c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00530
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00534
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00540
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00548
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0054c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00550
			 0x55555555,                                                  // 4 random values                             /// 00554
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00558
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0055c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00560
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00564
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0056c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00570
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00574
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0057c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00588
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0058c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00594
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00598
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0059c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005c8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 005d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005ec
			 0x55555555,                                                  // 4 random values                             /// 005f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00600
			 0x00011111,                                                  // 9.7958E-41                                  /// 00604
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00608
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0060c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00610
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00614
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00618
			 0x80000000,                                                  // -zero                                       /// 0061c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00620
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00624
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00628
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0062c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00630
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00634
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00638
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0063c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00640
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0064c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0065c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00660
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0066c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00670
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00678
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0067c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00680
			 0x7f800000,                                                  // inf                                         /// 00684
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0068c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00690
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00698
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0069c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 006c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 006f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00700
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00708
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x0c780000,                                                  // Leading 1s:                                 /// 00714
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0071c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0x7f800000,                                                  // inf                                         /// 00724
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0072c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00730
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00734
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00738
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0073c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00740
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00744
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0074c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00754
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00758
			 0x0c400000,                                                  // Leading 1s:                                 /// 0075c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x7f800000,                                                  // inf                                         /// 00770
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00778
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00780
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00788
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00790
			 0x0c400000,                                                  // Leading 1s:                                 /// 00794
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x55555555,                                                  // 4 random values                             /// 0079c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x4b000000,                                                  // 8388608.0                                   /// 007a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 007ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00804
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00808
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0080c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00814
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00818
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00820
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00824
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0082c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00834
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00838
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0083c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00850
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00858
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0085c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00868
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00870
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00874
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00878
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00884
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00890
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0089c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00904
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00908
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0090c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00910
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00918
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00924
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00928
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00930
			 0x0c400000,                                                  // Leading 1s:                                 /// 00934
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00938
			 0x80000000,                                                  // -zero                                       /// 0093c
			 0x3f028f5c,                                                  // 0.51                                        /// 00940
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00948
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0094c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x0c400000,                                                  // Leading 1s:                                 /// 00958
			 0x0e000003,                                                  // Trailing 1s:                                /// 0095c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00960
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0096c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00970
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00974
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00978
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00984
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0098c
			 0x3f028f5c,                                                  // 0.51                                        /// 00990
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00998
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0099c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a4
			 0x4b000000,                                                  // 8388608.0                                   /// 009a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009d8
			 0x55555555,                                                  // 4 random values                             /// 009dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009f8
			 0xffc00001,                                                  // -signaling NaN                              /// 009fc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0xff800000,                                                  // -inf                                        /// 00a0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a1c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a30
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a58
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a78
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a80
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a90
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00aa4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00aa8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ab8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00abc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ad0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ad8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00adc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ae0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ae8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aec
			 0x33333333,                                                  // 4 random values                             /// 00af0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00af8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00afc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b0c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b10
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b18
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b20
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b24
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x33333333,                                                  // 4 random values                             /// 00b34
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b44
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b48
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b4c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b54
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b64
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b68
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b6c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b74
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b80
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x55555555,                                                  // 4 random values                             /// 00b88
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b98
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ba8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bb0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bbc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bc0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bc4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bc8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bd0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x33333333,                                                  // 4 random values                             /// 00bd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bdc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00be0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00be4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00be8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bf0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bf4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bfc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c04
			 0xffc00001,                                                  // -signaling NaN                              /// 00c08
			 0x3f028f5c,                                                  // 0.51                                        /// 00c0c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c10
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c14
			 0x00000000,                                                  // zero                                        /// 00c18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c1c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c40
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c48
			 0xbf028f5c,                                                  // -0.51                                       /// 00c4c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c54
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c64
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c80
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c88
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c8c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c94
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c9c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ca0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ca4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ca8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cb0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cb4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ccc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cd0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cd4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cdc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ce4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ce8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cf0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cf4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cf8
			 0x3f028f5c,                                                  // 0.51                                        /// 00cfc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d00
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d04
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0xff800000,                                                  // -inf                                        /// 00d0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d14
			 0x80000000,                                                  // -zero                                       /// 00d18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d30
			 0xcb000000,                                                  // -8388608.0                                  /// 00d34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d44
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d4c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d54
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d58
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d5c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x80000000,                                                  // -zero                                       /// 00d70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d74
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d78
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d94
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00da8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00db4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00db8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dbc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dc8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dcc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dd4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ddc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00de0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00df4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e04
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e28
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e30
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e38
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e40
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e54
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e58
			 0x80000000,                                                  // -zero                                       /// 00e5c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e64
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e68
			 0x55555555,                                                  // 4 random values                             /// 00e6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x55555555,                                                  // 4 random values                             /// 00e84
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e8c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e94
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e98
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ea4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ea8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0e000001,                                                  // Trailing 1s:                                /// 00eb0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00eb8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ebc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ec0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ec4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ec8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ecc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ed4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ee0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ee8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00eec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ef0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ef4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00efc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f18
			 0xff800000,                                                  // -inf                                        /// 00f1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f30
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f38
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f40
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f48
			 0x7f800000,                                                  // inf                                         /// 00f4c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f50
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f6c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f70
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0xcb000000,                                                  // -8388608.0                                  /// 00f78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f7c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f80
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f84
			 0x3f028f5c,                                                  // 0.51                                        /// 00f88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f90
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fa4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fa8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fb4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fb8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fbc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fc0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fc4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fc8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fd4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fd8
			 0x33333333,                                                  // 4 random values                             /// 00fdc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fe0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fe8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x7fc00001,                                                  // signaling NaN                               /// 00ff0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ff4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ff8
			 0x8f7ffffe                                                  // all bit of mantissa set upto -3ulp          /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00004
			 0x4b000000,                                                  // 8388608.0                                   /// 00008
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0000c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00014
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00018
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0001c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00020
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00028
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00030
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00034
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00038
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0003c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00040
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00044
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00048
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0004c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00050
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00054
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0005c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00060
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00064
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00068
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0006c
			 0x7f800000,                                                  // inf                                         /// 00070
			 0xcb000000,                                                  // -8388608.0                                  /// 00074
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00078
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80000000,                                                  // -zero                                       /// 00080
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00084
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00088
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00090
			 0x00011111,                                                  // 9.7958E-41                                  /// 00094
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00098
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0009c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00100
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00104
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0xffc00001,                                                  // -signaling NaN                              /// 0010c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00110
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00114
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00118
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0011c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00124
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00128
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0012c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00130
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00138
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0013c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00140
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00144
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80011111,                                                  // -9.7958E-41                                 /// 0014c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00154
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00158
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0015c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00160
			 0x7f800000,                                                  // inf                                         /// 00164
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00168
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00170
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00174
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0xcb000000,                                                  // -8388608.0                                  /// 0017c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00180
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00184
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0018c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00194
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00198
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0019c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 001f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 001f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00200
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00204
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00210
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00220
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x0e000007,                                                  // Trailing 1s:                                /// 00228
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0022c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00230
			 0xffc00001,                                                  // -signaling NaN                              /// 00234
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0023c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00240
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00244
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00248
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x0e000001,                                                  // Trailing 1s:                                /// 00254
			 0xbf028f5c,                                                  // -0.51                                       /// 00258
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0025c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00264
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00268
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0026c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00274
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00278
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0xbf028f5c,                                                  // -0.51                                       /// 00280
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00288
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0028c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00298
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0029c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 002d8
			 0x3f028f5c,                                                  // 0.51                                        /// 002dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00304
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00308
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00310
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00318
			 0xcb000000,                                                  // -8388608.0                                  /// 0031c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00320
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00324
			 0xff800000,                                                  // -inf                                        /// 00328
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00330
			 0xbf028f5c,                                                  // -0.51                                       /// 00334
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00338
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0xffc00001,                                                  // -signaling NaN                              /// 00340
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00344
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0034c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00350
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00354
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00364
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0c400000,                                                  // Leading 1s:                                 /// 0036c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00370
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0037c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00384
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00388
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00390
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00394
			 0xffc00001,                                                  // -signaling NaN                              /// 00398
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0039c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x80000000,                                                  // -zero                                       /// 003e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00400
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00404
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00408
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0040c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00410
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00414
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00418
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00420
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0042c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00434
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00438
			 0x7f800000,                                                  // inf                                         /// 0043c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00440
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00444
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00448
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0044c
			 0x33333333,                                                  // 4 random values                             /// 00450
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00458
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00460
			 0x0c400000,                                                  // Leading 1s:                                 /// 00464
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00468
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0046c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00470
			 0x80000000,                                                  // -zero                                       /// 00474
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0047c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00480
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00488
			 0xff800000,                                                  // -inf                                        /// 0048c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00490
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00498
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x4b000000,                                                  // 8388608.0                                   /// 00508
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00510
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00518
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0051c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00520
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00524
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0052c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00530
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00534
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00538
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0053c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00540
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00548
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00550
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00554
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00558
			 0x7fc00001,                                                  // signaling NaN                               /// 0055c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00560
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00568
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00570
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00574
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00578
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00584
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00588
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0058c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00594
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00598
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0059c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 005b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x7f800000,                                                  // inf                                         /// 005bc
			 0x00000000,                                                  // zero                                        /// 005c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 005c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 005ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00600
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00608
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0060c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00610
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00614
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0061c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00620
			 0xffc00001,                                                  // -signaling NaN                              /// 00624
			 0x0e000001,                                                  // Trailing 1s:                                /// 00628
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x33333333,                                                  // 4 random values                             /// 00634
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0063c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00640
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00644
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00648
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00650
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00658
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0065c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00670
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00674
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0067c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00680
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00688
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0068c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00694
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00698
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x0e000003,                                                  // Trailing 1s:                                /// 006a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 006b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 006e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006f8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00700
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00704
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00708
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x33333333,                                                  // 4 random values                             /// 00710
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00714
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00718
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00720
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00724
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00728
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0072c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00734
			 0x7fc00001,                                                  // signaling NaN                               /// 00738
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00740
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00744
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00748
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00750
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00758
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0075c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00760
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0076c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00770
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00780
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00784
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00788
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0078c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00790
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00794
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00798
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0079c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007c8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007cc
			 0xcb000000,                                                  // -8388608.0                                  /// 007d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007f0
			 0x7f800000,                                                  // inf                                         /// 007f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00800
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00808
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0080c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00810
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00818
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0081c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00820
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00824
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00828
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00830
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0083c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x33333333,                                                  // 4 random values                             /// 00844
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00848
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0084c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00850
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00854
			 0x0c700000,                                                  // Leading 1s:                                 /// 00858
			 0xbf028f5c,                                                  // -0.51                                       /// 0085c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00860
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00864
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00868
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0086c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00870
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00878
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0087c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00880
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00884
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00888
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0088c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00890
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00894
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00898
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008d4
			 0xcb000000,                                                  // -8388608.0                                  /// 008d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x7f800000,                                                  // inf                                         /// 008e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00900
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00904
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0090c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00910
			 0x4b000000,                                                  // 8388608.0                                   /// 00914
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00918
			 0xffc00001,                                                  // -signaling NaN                              /// 0091c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00920
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00924
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00928
			 0x0c700000,                                                  // Leading 1s:                                 /// 0092c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00934
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00938
			 0x3f028f5c,                                                  // 0.51                                        /// 0093c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00944
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00948
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00950
			 0x00000000,                                                  // zero                                        /// 00954
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00958
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0095c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00960
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00964
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00968
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0097c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00980
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00984
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0098c
			 0x7f800000,                                                  // inf                                         /// 00990
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00994
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0099c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 009a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 009cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 009e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x3f028f5c,                                                  // 0.51                                        /// 009f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009f4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a10
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a1c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a28
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a34
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x7f800000,                                                  // inf                                         /// 00a40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a48
			 0xffc00001,                                                  // -signaling NaN                              /// 00a4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a54
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a58
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a60
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a70
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a80
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a90
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a94
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x7f800000,                                                  // inf                                         /// 00a9c
			 0xbf028f5c,                                                  // -0.51                                       /// 00aa0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00aa4
			 0x4b000000,                                                  // 8388608.0                                   /// 00aa8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00aac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ab0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00abc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00acc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ad4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00adc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ae0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ae8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00aec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b04
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b0c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b1c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b20
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b2c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b30
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b40
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b48
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b4c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b60
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b64
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b68
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b78
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b7c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b98
			 0xbf028f5c,                                                  // -0.51                                       /// 00b9c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ba0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ba4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ba8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bb4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bb8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bbc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bc0
			 0x3f028f5c,                                                  // 0.51                                        /// 00bc4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bcc
			 0x4b000000,                                                  // 8388608.0                                   /// 00bd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bd8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bdc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00be4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bf4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bf8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bfc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0xbf028f5c,                                                  // -0.51                                       /// 00c14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c18
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c20
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c34
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c38
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c40
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c4c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0xff800000,                                                  // -inf                                        /// 00c54
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c58
			 0x00000000,                                                  // zero                                        /// 00c5c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c78
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c90
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c98
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c9c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ca0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ca4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ca8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0xffc00001,                                                  // -signaling NaN                              /// 00cb8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cbc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cc0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cc4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ccc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cd0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cd4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cdc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ce0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cf4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cf8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cfc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d00
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d08
			 0x00000000,                                                  // zero                                        /// 00d0c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d18
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d20
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d30
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d38
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d40
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0xffc00001,                                                  // -signaling NaN                              /// 00d58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x7f800000,                                                  // inf                                         /// 00d64
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d70
			 0x33333333,                                                  // 4 random values                             /// 00d74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d98
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00da0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00da4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00db8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00de4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00df0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00df8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e00
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e04
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e10
			 0x7fc00001,                                                  // signaling NaN                               /// 00e14
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e30
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e34
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e40
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e58
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e64
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e6c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e90
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e94
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ea0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ea4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ea8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00eac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00eb4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ebc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ec8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ecc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ed4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ed8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00edc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ee0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ee4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ee8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ef0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ef4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ef8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f00
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f04
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f18
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f30
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f58
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f60
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f64
			 0x4b000000,                                                  // 8388608.0                                   /// 00f68
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f70
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f78
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f80
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f84
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f88
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f90
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fa0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fa4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fb8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fc8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fcc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fd4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fe0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fe4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fe8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ff0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ff8
			 0x0c780000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x16fc5c84, /// 0x0
			 0xa712dd14, /// 0x4
			 0x10986637, /// 0x8
			 0x8d9898f7, /// 0xc
			 0x5e2f09d0, /// 0x10
			 0xb036f4cb, /// 0x14
			 0x207e09d8, /// 0x18
			 0xe2a56fb5, /// 0x1c
			 0x63dc2382, /// 0x20
			 0x796e1d07, /// 0x24
			 0xba3fdca7, /// 0x28
			 0xae2b03bf, /// 0x2c
			 0x0e53cec7, /// 0x30
			 0xf2965db9, /// 0x34
			 0xb38f3b13, /// 0x38
			 0x1ee7860e, /// 0x3c
			 0x5ea33107, /// 0x40
			 0x63aaacbb, /// 0x44
			 0x4c3840ca, /// 0x48
			 0x872e4896, /// 0x4c
			 0x1e28b336, /// 0x50
			 0xcb9f5880, /// 0x54
			 0x2d52a793, /// 0x58
			 0x8270499f, /// 0x5c
			 0x4d0f9649, /// 0x60
			 0x88318475, /// 0x64
			 0x1399a189, /// 0x68
			 0x7b821602, /// 0x6c
			 0xfc5c6c89, /// 0x70
			 0x09becff4, /// 0x74
			 0xff4b5771, /// 0x78
			 0xf01c163f, /// 0x7c
			 0x235e0826, /// 0x80
			 0x6328ab01, /// 0x84
			 0x6c7e3798, /// 0x88
			 0xe83f1e5f, /// 0x8c
			 0x89145e58, /// 0x90
			 0x2ae7484d, /// 0x94
			 0xa9870016, /// 0x98
			 0x812fbfa8, /// 0x9c
			 0x487590bf, /// 0xa0
			 0xc82f1418, /// 0xa4
			 0xa214d1f2, /// 0xa8
			 0x26daef3c, /// 0xac
			 0xf7aa5d42, /// 0xb0
			 0x7be5be77, /// 0xb4
			 0x0b2f844d, /// 0xb8
			 0xd0dd8488, /// 0xbc
			 0x76820803, /// 0xc0
			 0x7104da08, /// 0xc4
			 0xfbcb69e6, /// 0xc8
			 0xbe245e43, /// 0xcc
			 0xc289bf1f, /// 0xd0
			 0x365dc6f5, /// 0xd4
			 0x435897d8, /// 0xd8
			 0x831cf001, /// 0xdc
			 0x37860b07, /// 0xe0
			 0xa1e2737e, /// 0xe4
			 0x39b530b8, /// 0xe8
			 0x8e2275ab, /// 0xec
			 0xe25ddd7c, /// 0xf0
			 0xc03572d2, /// 0xf4
			 0x6fd62c09, /// 0xf8
			 0xf3ae7006, /// 0xfc
			 0x7a811b5b, /// 0x100
			 0x0f418d0a, /// 0x104
			 0xf99cff1c, /// 0x108
			 0x0bc1e087, /// 0x10c
			 0x75fe5f72, /// 0x110
			 0xb4d8d714, /// 0x114
			 0x79206840, /// 0x118
			 0x5fdcc46a, /// 0x11c
			 0xc4b24f24, /// 0x120
			 0x46f3678e, /// 0x124
			 0x6605f303, /// 0x128
			 0x06898e6b, /// 0x12c
			 0xe0d82f6f, /// 0x130
			 0xe861b349, /// 0x134
			 0x4269225a, /// 0x138
			 0x4dc297e8, /// 0x13c
			 0xa0d3a2a8, /// 0x140
			 0xbb5073ce, /// 0x144
			 0x27d65c29, /// 0x148
			 0xd89d5726, /// 0x14c
			 0xe8ad3640, /// 0x150
			 0xb1089adf, /// 0x154
			 0x73b440a5, /// 0x158
			 0x1f021fda, /// 0x15c
			 0xed99f1d6, /// 0x160
			 0x02dc3ffe, /// 0x164
			 0x362cfbfb, /// 0x168
			 0x61495065, /// 0x16c
			 0xeefa847d, /// 0x170
			 0xc7ae8e9d, /// 0x174
			 0x899e7122, /// 0x178
			 0x158454a7, /// 0x17c
			 0xbb9ce536, /// 0x180
			 0x85bf28fa, /// 0x184
			 0x9bb95403, /// 0x188
			 0xf37e98b6, /// 0x18c
			 0xed7685e6, /// 0x190
			 0xbff559e3, /// 0x194
			 0xc97d129e, /// 0x198
			 0x2ff767c7, /// 0x19c
			 0x1d30185c, /// 0x1a0
			 0x341d0ca2, /// 0x1a4
			 0xd32508d9, /// 0x1a8
			 0x4baa2a8b, /// 0x1ac
			 0x47db7666, /// 0x1b0
			 0x54a4042c, /// 0x1b4
			 0x005fb5c2, /// 0x1b8
			 0xab2290f0, /// 0x1bc
			 0xa6a51114, /// 0x1c0
			 0x7e6f6332, /// 0x1c4
			 0xe1e6b37c, /// 0x1c8
			 0x49916025, /// 0x1cc
			 0x248d17e7, /// 0x1d0
			 0x15b01322, /// 0x1d4
			 0x3f3eaaa2, /// 0x1d8
			 0x0beb4029, /// 0x1dc
			 0xc93a1c70, /// 0x1e0
			 0xccd7893f, /// 0x1e4
			 0xe1f35498, /// 0x1e8
			 0x89aeb0d9, /// 0x1ec
			 0x5123795e, /// 0x1f0
			 0x6d45b3a6, /// 0x1f4
			 0x0cc91dd0, /// 0x1f8
			 0xf1d0f44f, /// 0x1fc
			 0xb3d4aa1f, /// 0x200
			 0xed29c6ef, /// 0x204
			 0xf114ddc5, /// 0x208
			 0xcf1b2db6, /// 0x20c
			 0x48376a52, /// 0x210
			 0xcc391206, /// 0x214
			 0x0d1439dc, /// 0x218
			 0x7b13ca77, /// 0x21c
			 0x46d240ef, /// 0x220
			 0x4bbbfc98, /// 0x224
			 0x473eb3d3, /// 0x228
			 0x61146042, /// 0x22c
			 0x1066d04f, /// 0x230
			 0x7c0debfd, /// 0x234
			 0x02c5108e, /// 0x238
			 0x40435262, /// 0x23c
			 0xb8a78050, /// 0x240
			 0x6024ce49, /// 0x244
			 0x4f45e383, /// 0x248
			 0x5d5589bc, /// 0x24c
			 0x22cc8511, /// 0x250
			 0x23e090da, /// 0x254
			 0x65004cc3, /// 0x258
			 0x079f47f1, /// 0x25c
			 0x4fb3f70d, /// 0x260
			 0xa696cd10, /// 0x264
			 0x73cfc760, /// 0x268
			 0x6d028efb, /// 0x26c
			 0x1556619d, /// 0x270
			 0xe0fd287a, /// 0x274
			 0xd7fa8872, /// 0x278
			 0xe9bc7d15, /// 0x27c
			 0xdb264795, /// 0x280
			 0x9f9848b3, /// 0x284
			 0x5321e3b3, /// 0x288
			 0x7cc76f6a, /// 0x28c
			 0xfd0aa43d, /// 0x290
			 0xcd34b962, /// 0x294
			 0xc707d6a1, /// 0x298
			 0xa6bc48e4, /// 0x29c
			 0x7b8c1fce, /// 0x2a0
			 0xc7f788ea, /// 0x2a4
			 0x14738955, /// 0x2a8
			 0xd34e5bc4, /// 0x2ac
			 0x5178669e, /// 0x2b0
			 0xba6e3fd9, /// 0x2b4
			 0x5001f743, /// 0x2b8
			 0xeaf4f213, /// 0x2bc
			 0x19ffc5ac, /// 0x2c0
			 0x111a5a8b, /// 0x2c4
			 0x012bd306, /// 0x2c8
			 0xd71d0e60, /// 0x2cc
			 0x5ad659fe, /// 0x2d0
			 0x684fb531, /// 0x2d4
			 0x5cc69799, /// 0x2d8
			 0x2456f555, /// 0x2dc
			 0xf4539299, /// 0x2e0
			 0x9cfeefae, /// 0x2e4
			 0x9e53fbc2, /// 0x2e8
			 0x8b1c42fc, /// 0x2ec
			 0x5dfdd153, /// 0x2f0
			 0xc41e1e3f, /// 0x2f4
			 0xa1f95da6, /// 0x2f8
			 0x890b81da, /// 0x2fc
			 0xc567908b, /// 0x300
			 0x57ecabc9, /// 0x304
			 0x799b055e, /// 0x308
			 0xfcb54e90, /// 0x30c
			 0xec3a5f16, /// 0x310
			 0xb3a7d931, /// 0x314
			 0x0792fb64, /// 0x318
			 0x5c76490b, /// 0x31c
			 0xdb4696f8, /// 0x320
			 0xe8198c98, /// 0x324
			 0x7797617f, /// 0x328
			 0x2aee6af2, /// 0x32c
			 0xbb1b15f0, /// 0x330
			 0x65f9304d, /// 0x334
			 0xc1f047bf, /// 0x338
			 0xf476e2d6, /// 0x33c
			 0x5871d5d7, /// 0x340
			 0x8d9051d2, /// 0x344
			 0xf84ff4f5, /// 0x348
			 0xf4dd4dd5, /// 0x34c
			 0xf3d9b84e, /// 0x350
			 0xd7f88087, /// 0x354
			 0x163e211e, /// 0x358
			 0xf4cc3af0, /// 0x35c
			 0x75ec57cc, /// 0x360
			 0x5f6500dd, /// 0x364
			 0xcecb2c06, /// 0x368
			 0x537f3ccb, /// 0x36c
			 0x71e20b14, /// 0x370
			 0x8a7045ed, /// 0x374
			 0xe818925b, /// 0x378
			 0x4a30c4db, /// 0x37c
			 0xa5e57ca3, /// 0x380
			 0x9f28edc6, /// 0x384
			 0x0733fe87, /// 0x388
			 0xf0d2e744, /// 0x38c
			 0x5999dc2d, /// 0x390
			 0x88591a23, /// 0x394
			 0xa5e269ff, /// 0x398
			 0x1c567f9c, /// 0x39c
			 0x9341a51e, /// 0x3a0
			 0xc9473ad6, /// 0x3a4
			 0xf4b3b252, /// 0x3a8
			 0x60d83cfe, /// 0x3ac
			 0xd802470f, /// 0x3b0
			 0x4e986704, /// 0x3b4
			 0x51195b6f, /// 0x3b8
			 0x50d9a6a7, /// 0x3bc
			 0xd9c83e53, /// 0x3c0
			 0xb163a372, /// 0x3c4
			 0xc0ad9540, /// 0x3c8
			 0xd261b6b3, /// 0x3cc
			 0xd791d3e9, /// 0x3d0
			 0x03372baa, /// 0x3d4
			 0x154189db, /// 0x3d8
			 0xd09d732f, /// 0x3dc
			 0x855e8ae3, /// 0x3e0
			 0x54574e68, /// 0x3e4
			 0xbfdbb1f4, /// 0x3e8
			 0x1b3b1ff6, /// 0x3ec
			 0xd6c9a78b, /// 0x3f0
			 0x80d638d3, /// 0x3f4
			 0xf8c1a27e, /// 0x3f8
			 0xa01d62a5, /// 0x3fc
			 0xc5c2c81c, /// 0x400
			 0x871d0428, /// 0x404
			 0x025f5b6c, /// 0x408
			 0x2cc33389, /// 0x40c
			 0xa6cdfaa8, /// 0x410
			 0x676cbb14, /// 0x414
			 0x554ae7e2, /// 0x418
			 0xf4e870a9, /// 0x41c
			 0x0e067a59, /// 0x420
			 0xe2f6a92c, /// 0x424
			 0xde3668ef, /// 0x428
			 0x48e1605f, /// 0x42c
			 0x4082ae75, /// 0x430
			 0xfdebf88c, /// 0x434
			 0xad48a5f5, /// 0x438
			 0x0d5d038f, /// 0x43c
			 0xbcbfccec, /// 0x440
			 0x0525ae80, /// 0x444
			 0x871a1b3b, /// 0x448
			 0xf44556dc, /// 0x44c
			 0x5aab818c, /// 0x450
			 0x137816b3, /// 0x454
			 0x03f40748, /// 0x458
			 0xcadf6951, /// 0x45c
			 0xf2f46d0c, /// 0x460
			 0x3a20c506, /// 0x464
			 0xdc110d5f, /// 0x468
			 0x5214dc02, /// 0x46c
			 0xd65e7a1f, /// 0x470
			 0x033a3cd4, /// 0x474
			 0x54bf03a4, /// 0x478
			 0xce0661e8, /// 0x47c
			 0xec7ec2ad, /// 0x480
			 0x3441c2b8, /// 0x484
			 0xaf8e610e, /// 0x488
			 0x364364c2, /// 0x48c
			 0xbe359057, /// 0x490
			 0x4634e150, /// 0x494
			 0xb049d38a, /// 0x498
			 0x33afbac1, /// 0x49c
			 0x60e54957, /// 0x4a0
			 0x4893a9e4, /// 0x4a4
			 0xf5d095d3, /// 0x4a8
			 0x712d6c8f, /// 0x4ac
			 0xd4a860d8, /// 0x4b0
			 0x5130071c, /// 0x4b4
			 0x08995133, /// 0x4b8
			 0x296d567e, /// 0x4bc
			 0x54fc8d33, /// 0x4c0
			 0x11f3cd62, /// 0x4c4
			 0x2f2b4450, /// 0x4c8
			 0x8ff82ff8, /// 0x4cc
			 0xdbdceb08, /// 0x4d0
			 0x6f60e016, /// 0x4d4
			 0x5f9bfd09, /// 0x4d8
			 0x0d24b999, /// 0x4dc
			 0xe40bbda3, /// 0x4e0
			 0xcf65e53c, /// 0x4e4
			 0x3354993b, /// 0x4e8
			 0x4a470046, /// 0x4ec
			 0x3c9a5441, /// 0x4f0
			 0x92740820, /// 0x4f4
			 0xf75ad516, /// 0x4f8
			 0xb3ece221, /// 0x4fc
			 0x4b3fb9df, /// 0x500
			 0x4e3ccb02, /// 0x504
			 0x9bce1769, /// 0x508
			 0x06c95244, /// 0x50c
			 0x722b007d, /// 0x510
			 0x1e7ba721, /// 0x514
			 0x5424f3a6, /// 0x518
			 0xbcc12023, /// 0x51c
			 0x5dd367d3, /// 0x520
			 0x6c4a6e93, /// 0x524
			 0xd7d6f7b7, /// 0x528
			 0x3fa08e3c, /// 0x52c
			 0x88bbf7bf, /// 0x530
			 0xdb91bd57, /// 0x534
			 0x7ec5ec69, /// 0x538
			 0x20e1e005, /// 0x53c
			 0x1e0961fc, /// 0x540
			 0x1be280e9, /// 0x544
			 0x4d75dc96, /// 0x548
			 0xdbdc8b6e, /// 0x54c
			 0x083152fd, /// 0x550
			 0x8e81c908, /// 0x554
			 0x8130ea9f, /// 0x558
			 0xe908122b, /// 0x55c
			 0x0ebe2aa0, /// 0x560
			 0xe6efc07e, /// 0x564
			 0x331e3936, /// 0x568
			 0xc4f4bb52, /// 0x56c
			 0xe891499e, /// 0x570
			 0x12fb1bdf, /// 0x574
			 0x9006818d, /// 0x578
			 0xc1928519, /// 0x57c
			 0x23317590, /// 0x580
			 0xdc34f9a7, /// 0x584
			 0xaa6fbfd1, /// 0x588
			 0x8f48d32f, /// 0x58c
			 0x3d24998e, /// 0x590
			 0xb3ce0cb1, /// 0x594
			 0xbd4c310b, /// 0x598
			 0x95006c48, /// 0x59c
			 0xa79a1eb9, /// 0x5a0
			 0xf031ba74, /// 0x5a4
			 0x7970d2c4, /// 0x5a8
			 0x7c2727ec, /// 0x5ac
			 0x8fcf3896, /// 0x5b0
			 0x69a728f4, /// 0x5b4
			 0xed6bfea6, /// 0x5b8
			 0x296152f5, /// 0x5bc
			 0x98eaa17e, /// 0x5c0
			 0x97475df5, /// 0x5c4
			 0xea35fd41, /// 0x5c8
			 0x233b01b4, /// 0x5cc
			 0xe4be1c96, /// 0x5d0
			 0xe9d91901, /// 0x5d4
			 0xcb76bcb1, /// 0x5d8
			 0x9e3f7d42, /// 0x5dc
			 0x2c6eb812, /// 0x5e0
			 0xc19c7fc2, /// 0x5e4
			 0x93ec0438, /// 0x5e8
			 0x53677d0b, /// 0x5ec
			 0x63242115, /// 0x5f0
			 0xf810cdaa, /// 0x5f4
			 0x20b2096b, /// 0x5f8
			 0x2a64be79, /// 0x5fc
			 0xc17e432a, /// 0x600
			 0x52773154, /// 0x604
			 0xb5e3b009, /// 0x608
			 0x5b8ee839, /// 0x60c
			 0x8695accc, /// 0x610
			 0x79aca058, /// 0x614
			 0x4d29c997, /// 0x618
			 0x4f05ad1f, /// 0x61c
			 0x9c1541f9, /// 0x620
			 0x954bfa67, /// 0x624
			 0x0dff3be7, /// 0x628
			 0xe6bc0a1d, /// 0x62c
			 0xde1b7287, /// 0x630
			 0x7939ae79, /// 0x634
			 0xfb8db5fd, /// 0x638
			 0xa3c222d9, /// 0x63c
			 0x35caa34f, /// 0x640
			 0x0510b8f0, /// 0x644
			 0xf7289a58, /// 0x648
			 0x60a927bd, /// 0x64c
			 0x41583e96, /// 0x650
			 0x4fe401e4, /// 0x654
			 0x5d1ddbe3, /// 0x658
			 0xf9859419, /// 0x65c
			 0x7017b132, /// 0x660
			 0x4a1b96b5, /// 0x664
			 0x536d52fe, /// 0x668
			 0x872499ea, /// 0x66c
			 0xdf9a0c39, /// 0x670
			 0x0d51ab4b, /// 0x674
			 0x648f8de5, /// 0x678
			 0xd0e2fabc, /// 0x67c
			 0xe695e796, /// 0x680
			 0x81e243a8, /// 0x684
			 0xc5884131, /// 0x688
			 0x6d2fea1d, /// 0x68c
			 0x7785842c, /// 0x690
			 0xe411e701, /// 0x694
			 0x71b2c383, /// 0x698
			 0xc79a3845, /// 0x69c
			 0xb97d5f08, /// 0x6a0
			 0x5b724c7d, /// 0x6a4
			 0x28f291bd, /// 0x6a8
			 0x3e4ec408, /// 0x6ac
			 0xc86df6a8, /// 0x6b0
			 0x69cde519, /// 0x6b4
			 0xefbd7725, /// 0x6b8
			 0x1e2c58ac, /// 0x6bc
			 0x892276fe, /// 0x6c0
			 0xa661a2aa, /// 0x6c4
			 0x9038fa15, /// 0x6c8
			 0xbb500368, /// 0x6cc
			 0x508cc1aa, /// 0x6d0
			 0xef71851e, /// 0x6d4
			 0x530d6321, /// 0x6d8
			 0xa5034aac, /// 0x6dc
			 0xfa321493, /// 0x6e0
			 0x9b6324e4, /// 0x6e4
			 0xfde7090b, /// 0x6e8
			 0x48a64036, /// 0x6ec
			 0x859823e8, /// 0x6f0
			 0xc8be0a64, /// 0x6f4
			 0x5259fcba, /// 0x6f8
			 0xefd2c3e3, /// 0x6fc
			 0x6aa9b47a, /// 0x700
			 0x636c65de, /// 0x704
			 0xe4eff71c, /// 0x708
			 0x8d16fb69, /// 0x70c
			 0xddfa1d9c, /// 0x710
			 0x7705a962, /// 0x714
			 0xd94c7822, /// 0x718
			 0xdc3b6d8e, /// 0x71c
			 0x2855b7f1, /// 0x720
			 0x53652559, /// 0x724
			 0xebf2ae21, /// 0x728
			 0xa8de73f2, /// 0x72c
			 0xdef4ec4e, /// 0x730
			 0x9071860a, /// 0x734
			 0xcd80b2f5, /// 0x738
			 0xde657f27, /// 0x73c
			 0xc198e008, /// 0x740
			 0x3428df4d, /// 0x744
			 0x6bea73b8, /// 0x748
			 0xfbd04acc, /// 0x74c
			 0x11dcba14, /// 0x750
			 0x3d67a7f8, /// 0x754
			 0x4bbfbd95, /// 0x758
			 0xd40823c6, /// 0x75c
			 0x6c33565e, /// 0x760
			 0x35cb6db0, /// 0x764
			 0x9aa75e13, /// 0x768
			 0x86943675, /// 0x76c
			 0xe5de204e, /// 0x770
			 0xb7ab48a1, /// 0x774
			 0xda772626, /// 0x778
			 0x27939544, /// 0x77c
			 0x1cb2b221, /// 0x780
			 0x484fd034, /// 0x784
			 0xa3b03602, /// 0x788
			 0xd1c9fb94, /// 0x78c
			 0xc9ca7ef7, /// 0x790
			 0x04bb18b6, /// 0x794
			 0xa27a5ce7, /// 0x798
			 0x13d5b02b, /// 0x79c
			 0xc8f00ded, /// 0x7a0
			 0xe5a5d970, /// 0x7a4
			 0x04ebe69a, /// 0x7a8
			 0x37d247c2, /// 0x7ac
			 0x6fe910b8, /// 0x7b0
			 0xac002acf, /// 0x7b4
			 0x247e057f, /// 0x7b8
			 0x6ae345ec, /// 0x7bc
			 0x763fe376, /// 0x7c0
			 0x5596de1a, /// 0x7c4
			 0x0817a7f2, /// 0x7c8
			 0xfd16262b, /// 0x7cc
			 0xc87125d8, /// 0x7d0
			 0xfb496f7f, /// 0x7d4
			 0xfbc75182, /// 0x7d8
			 0x8c93bff2, /// 0x7dc
			 0x960a915f, /// 0x7e0
			 0x7e0a3edd, /// 0x7e4
			 0xdbdc20f1, /// 0x7e8
			 0x1d723d0d, /// 0x7ec
			 0xaa60d9b8, /// 0x7f0
			 0x095c3b28, /// 0x7f4
			 0xd1b79860, /// 0x7f8
			 0x298acfdf, /// 0x7fc
			 0xe2a207db, /// 0x800
			 0xd27737d6, /// 0x804
			 0xc715d139, /// 0x808
			 0x4df0e727, /// 0x80c
			 0x38975948, /// 0x810
			 0xd93ccd81, /// 0x814
			 0xc693e962, /// 0x818
			 0xc13f1b6e, /// 0x81c
			 0xc93b4c6e, /// 0x820
			 0xff7c1d1b, /// 0x824
			 0x774c1433, /// 0x828
			 0x77ba005a, /// 0x82c
			 0xbf5856ff, /// 0x830
			 0x65e4a447, /// 0x834
			 0xe58770a0, /// 0x838
			 0x46e72092, /// 0x83c
			 0xcf340a83, /// 0x840
			 0xfb2a01e2, /// 0x844
			 0xa390e1f7, /// 0x848
			 0x8d00b7de, /// 0x84c
			 0xdfd5ea84, /// 0x850
			 0xb3d26988, /// 0x854
			 0x68496258, /// 0x858
			 0x8f1bd23a, /// 0x85c
			 0x944ebaf1, /// 0x860
			 0xff639932, /// 0x864
			 0xb5a31c3c, /// 0x868
			 0x5caa0b33, /// 0x86c
			 0xbffde3d5, /// 0x870
			 0xa5a6c145, /// 0x874
			 0x8ae4abd7, /// 0x878
			 0x3af88999, /// 0x87c
			 0x328a2819, /// 0x880
			 0x2ec66dfd, /// 0x884
			 0xdf25f8ef, /// 0x888
			 0x3bb1d652, /// 0x88c
			 0x822f426f, /// 0x890
			 0x766f0df2, /// 0x894
			 0x92cc5c3c, /// 0x898
			 0x094f28a8, /// 0x89c
			 0xd7880940, /// 0x8a0
			 0x840d4c44, /// 0x8a4
			 0x77edf4ba, /// 0x8a8
			 0x96ffc5f4, /// 0x8ac
			 0x089b93dc, /// 0x8b0
			 0x4835f722, /// 0x8b4
			 0x94860471, /// 0x8b8
			 0x5c8214d1, /// 0x8bc
			 0x82381b20, /// 0x8c0
			 0xfc1d48ce, /// 0x8c4
			 0x10159c6b, /// 0x8c8
			 0x62d31e9d, /// 0x8cc
			 0x678b865d, /// 0x8d0
			 0xace559bb, /// 0x8d4
			 0xe345e90d, /// 0x8d8
			 0x314049b8, /// 0x8dc
			 0x7453d9a2, /// 0x8e0
			 0x7ae1136e, /// 0x8e4
			 0x12bf10f2, /// 0x8e8
			 0xd6a692ad, /// 0x8ec
			 0xc545588a, /// 0x8f0
			 0x875c3b69, /// 0x8f4
			 0xfc8162f2, /// 0x8f8
			 0x090b7acb, /// 0x8fc
			 0x3f07ef98, /// 0x900
			 0xdefdf57c, /// 0x904
			 0x429248e0, /// 0x908
			 0xe9ab753c, /// 0x90c
			 0x89cc3466, /// 0x910
			 0x12866586, /// 0x914
			 0x50cd0a0d, /// 0x918
			 0x128314de, /// 0x91c
			 0xae8bc29a, /// 0x920
			 0xca37da69, /// 0x924
			 0x2c048b37, /// 0x928
			 0x046a273d, /// 0x92c
			 0x3445eddb, /// 0x930
			 0x30359bdf, /// 0x934
			 0xfe150bc7, /// 0x938
			 0xd0753cef, /// 0x93c
			 0x659c5fe1, /// 0x940
			 0xc46d65a6, /// 0x944
			 0xccaa1e5b, /// 0x948
			 0xdf1416fe, /// 0x94c
			 0xc4375bdf, /// 0x950
			 0xc18067c6, /// 0x954
			 0xaf735636, /// 0x958
			 0x551cfdec, /// 0x95c
			 0x56709405, /// 0x960
			 0xc2bc07d0, /// 0x964
			 0x7c0cc859, /// 0x968
			 0x15059ea8, /// 0x96c
			 0x390b699c, /// 0x970
			 0x16fcf7b3, /// 0x974
			 0xa1aa47a2, /// 0x978
			 0x3de6787a, /// 0x97c
			 0x820cc3fd, /// 0x980
			 0x8041dd8f, /// 0x984
			 0x7c7c8adf, /// 0x988
			 0x4f49d1f7, /// 0x98c
			 0x85268f0e, /// 0x990
			 0xc81df152, /// 0x994
			 0xf0578bab, /// 0x998
			 0xe0080e24, /// 0x99c
			 0xbea5b09b, /// 0x9a0
			 0xcbf873cd, /// 0x9a4
			 0xb7527ea3, /// 0x9a8
			 0xfaf91537, /// 0x9ac
			 0xc7b535aa, /// 0x9b0
			 0xffc2861a, /// 0x9b4
			 0xf0dcfc01, /// 0x9b8
			 0xc5a68cab, /// 0x9bc
			 0xdb43ca09, /// 0x9c0
			 0x17f3089e, /// 0x9c4
			 0x9521e5dd, /// 0x9c8
			 0x011b8536, /// 0x9cc
			 0x3abaae1e, /// 0x9d0
			 0xd41d314a, /// 0x9d4
			 0x49a76464, /// 0x9d8
			 0x40e7aa36, /// 0x9dc
			 0x627ca6d5, /// 0x9e0
			 0xaf46737e, /// 0x9e4
			 0x751aff41, /// 0x9e8
			 0x7fae7cc0, /// 0x9ec
			 0x49b7f17a, /// 0x9f0
			 0x760c3f1f, /// 0x9f4
			 0x38946013, /// 0x9f8
			 0x2c6bc51c, /// 0x9fc
			 0x162dd8f7, /// 0xa00
			 0x1e4329b9, /// 0xa04
			 0x4f6ff0ce, /// 0xa08
			 0x5a520af0, /// 0xa0c
			 0x84aafad4, /// 0xa10
			 0x98a7cbb3, /// 0xa14
			 0xb930f402, /// 0xa18
			 0xa8f195e7, /// 0xa1c
			 0xf64e1130, /// 0xa20
			 0xb079fd9d, /// 0xa24
			 0xc0efbea7, /// 0xa28
			 0x354319e1, /// 0xa2c
			 0xc4eeb4ee, /// 0xa30
			 0xdc63947b, /// 0xa34
			 0xd601d920, /// 0xa38
			 0x08a4336c, /// 0xa3c
			 0x8edf22a9, /// 0xa40
			 0xcd22b568, /// 0xa44
			 0x6df07842, /// 0xa48
			 0x7e1fb91f, /// 0xa4c
			 0x1c043c6f, /// 0xa50
			 0x8bf1274d, /// 0xa54
			 0x5bc0fade, /// 0xa58
			 0xabe888b2, /// 0xa5c
			 0x3a77d050, /// 0xa60
			 0x63dd9cb6, /// 0xa64
			 0x65790051, /// 0xa68
			 0x5056f335, /// 0xa6c
			 0x2889a591, /// 0xa70
			 0x20265ea2, /// 0xa74
			 0x23d6c590, /// 0xa78
			 0xd75ff5e3, /// 0xa7c
			 0x05e2870b, /// 0xa80
			 0xa3e5ddbe, /// 0xa84
			 0xcac1649f, /// 0xa88
			 0xca7bfa34, /// 0xa8c
			 0x11ae1579, /// 0xa90
			 0x2c79092d, /// 0xa94
			 0x8d694580, /// 0xa98
			 0x46667b7c, /// 0xa9c
			 0xb1f75498, /// 0xaa0
			 0x7061614d, /// 0xaa4
			 0x5343f142, /// 0xaa8
			 0x4285b34e, /// 0xaac
			 0x9c928bc2, /// 0xab0
			 0x3bd7b32a, /// 0xab4
			 0x8d5379d7, /// 0xab8
			 0x23866624, /// 0xabc
			 0x02a7616e, /// 0xac0
			 0xda047571, /// 0xac4
			 0x6d88737e, /// 0xac8
			 0xf88c8570, /// 0xacc
			 0x83d9e1a6, /// 0xad0
			 0x272785ab, /// 0xad4
			 0xda2f5459, /// 0xad8
			 0x393de19a, /// 0xadc
			 0x15dcb14c, /// 0xae0
			 0x576ef499, /// 0xae4
			 0xadc0c13a, /// 0xae8
			 0x17f5a38f, /// 0xaec
			 0x221ffaf5, /// 0xaf0
			 0xd1dd4e52, /// 0xaf4
			 0x3d763f2e, /// 0xaf8
			 0x8c08d70e, /// 0xafc
			 0x0bd0dd5d, /// 0xb00
			 0x1a5ee84d, /// 0xb04
			 0x128703e0, /// 0xb08
			 0x6b977a78, /// 0xb0c
			 0xe42a4a36, /// 0xb10
			 0x8c859fec, /// 0xb14
			 0x28959c3b, /// 0xb18
			 0x6f58cc4e, /// 0xb1c
			 0x83f11bc7, /// 0xb20
			 0x0ee41c93, /// 0xb24
			 0x77287e6c, /// 0xb28
			 0x2f635a84, /// 0xb2c
			 0xa5882c11, /// 0xb30
			 0x189c8dee, /// 0xb34
			 0x40dd3587, /// 0xb38
			 0x074b15d5, /// 0xb3c
			 0x55878d03, /// 0xb40
			 0xb0902ded, /// 0xb44
			 0xb6cd90f4, /// 0xb48
			 0x5e07f433, /// 0xb4c
			 0xb5f20e57, /// 0xb50
			 0xac3def12, /// 0xb54
			 0xc897f9a5, /// 0xb58
			 0xc4ab9c44, /// 0xb5c
			 0x8f560b21, /// 0xb60
			 0x80ed1d3d, /// 0xb64
			 0xee958969, /// 0xb68
			 0xd6cd366c, /// 0xb6c
			 0x14a09550, /// 0xb70
			 0xbf81ea99, /// 0xb74
			 0x72a4879f, /// 0xb78
			 0xc1caef96, /// 0xb7c
			 0x5ce338ae, /// 0xb80
			 0xff461dd0, /// 0xb84
			 0xb245ca12, /// 0xb88
			 0x6a128f1d, /// 0xb8c
			 0x36784301, /// 0xb90
			 0xee5a9ff9, /// 0xb94
			 0x05dafcf3, /// 0xb98
			 0xb2e4bd05, /// 0xb9c
			 0xb46c4c92, /// 0xba0
			 0xed87306c, /// 0xba4
			 0x785b5c6f, /// 0xba8
			 0x5f4ba1a2, /// 0xbac
			 0xd29d6140, /// 0xbb0
			 0x258f84c5, /// 0xbb4
			 0x6953c0e8, /// 0xbb8
			 0xb32ffac3, /// 0xbbc
			 0x6df2422f, /// 0xbc0
			 0x5c88744c, /// 0xbc4
			 0xbe89bffd, /// 0xbc8
			 0x3336c02c, /// 0xbcc
			 0x3096b892, /// 0xbd0
			 0x60331015, /// 0xbd4
			 0x42dc5326, /// 0xbd8
			 0x2998aac0, /// 0xbdc
			 0x11ee8f5b, /// 0xbe0
			 0x22d9ac3d, /// 0xbe4
			 0xa81557df, /// 0xbe8
			 0x18bbdd74, /// 0xbec
			 0x35ed2799, /// 0xbf0
			 0x173b6a1b, /// 0xbf4
			 0x13a9c24f, /// 0xbf8
			 0x55b5fe8f, /// 0xbfc
			 0x006e1025, /// 0xc00
			 0x6aececc6, /// 0xc04
			 0xcce75fcb, /// 0xc08
			 0xcfb861e3, /// 0xc0c
			 0x25578c92, /// 0xc10
			 0xf4f58129, /// 0xc14
			 0x19e7197e, /// 0xc18
			 0x7457c91f, /// 0xc1c
			 0xe61eb3c2, /// 0xc20
			 0xdc3147a7, /// 0xc24
			 0x285a3028, /// 0xc28
			 0x1aec1a19, /// 0xc2c
			 0x0b59f5e7, /// 0xc30
			 0x6ccd669b, /// 0xc34
			 0x40265fdf, /// 0xc38
			 0xb6027364, /// 0xc3c
			 0xd54bc175, /// 0xc40
			 0xa230ee71, /// 0xc44
			 0xb201c0c3, /// 0xc48
			 0xd0b918a8, /// 0xc4c
			 0x2424a68c, /// 0xc50
			 0xe145c71c, /// 0xc54
			 0x79c4c688, /// 0xc58
			 0xd1e1a0ef, /// 0xc5c
			 0x6c66b04b, /// 0xc60
			 0xb79f362d, /// 0xc64
			 0xd7cd3d02, /// 0xc68
			 0x69a7c1ad, /// 0xc6c
			 0xe50accb8, /// 0xc70
			 0xfccc1156, /// 0xc74
			 0x457558ea, /// 0xc78
			 0x1054e209, /// 0xc7c
			 0xa62a6c07, /// 0xc80
			 0xb26a17b2, /// 0xc84
			 0x51437c88, /// 0xc88
			 0x109f9d14, /// 0xc8c
			 0x98a19cbd, /// 0xc90
			 0x559423bf, /// 0xc94
			 0xca25b097, /// 0xc98
			 0xb968ecb8, /// 0xc9c
			 0xbe7ced6d, /// 0xca0
			 0x1c922b69, /// 0xca4
			 0x3d0b4c5a, /// 0xca8
			 0x19189218, /// 0xcac
			 0xa124e50b, /// 0xcb0
			 0xea12ecf1, /// 0xcb4
			 0xc24a58bf, /// 0xcb8
			 0xfe622a95, /// 0xcbc
			 0xa8d44ab8, /// 0xcc0
			 0xc6b1db02, /// 0xcc4
			 0x84227ef9, /// 0xcc8
			 0xbcbbf338, /// 0xccc
			 0x9696b250, /// 0xcd0
			 0x7646cc1d, /// 0xcd4
			 0xd450f2d1, /// 0xcd8
			 0xd09c5c1c, /// 0xcdc
			 0xa9ce5552, /// 0xce0
			 0xccfb8c2d, /// 0xce4
			 0xbae656c8, /// 0xce8
			 0xf68d2112, /// 0xcec
			 0x0e062ec9, /// 0xcf0
			 0x683718e0, /// 0xcf4
			 0x683ccef4, /// 0xcf8
			 0x62cb94cd, /// 0xcfc
			 0xac470ea8, /// 0xd00
			 0x34b07ce1, /// 0xd04
			 0x6d475cf8, /// 0xd08
			 0x8a6cf32b, /// 0xd0c
			 0x6097d7ab, /// 0xd10
			 0x8a827843, /// 0xd14
			 0x3dd787e5, /// 0xd18
			 0x25175372, /// 0xd1c
			 0x2e32418d, /// 0xd20
			 0x826a5c64, /// 0xd24
			 0xfe697c47, /// 0xd28
			 0x2aeca75a, /// 0xd2c
			 0x6a449686, /// 0xd30
			 0xfb61c089, /// 0xd34
			 0x68430511, /// 0xd38
			 0x1e447de7, /// 0xd3c
			 0xd3d884bd, /// 0xd40
			 0xcd13e6df, /// 0xd44
			 0x94a8d1ab, /// 0xd48
			 0x3041d777, /// 0xd4c
			 0x3ff087ba, /// 0xd50
			 0x832fb63b, /// 0xd54
			 0x9f07cfab, /// 0xd58
			 0xf0f3247b, /// 0xd5c
			 0x342130d5, /// 0xd60
			 0x3bc38ab9, /// 0xd64
			 0x813cd95f, /// 0xd68
			 0xc403ad22, /// 0xd6c
			 0x9d050d9d, /// 0xd70
			 0x0f739e82, /// 0xd74
			 0xe0a2af53, /// 0xd78
			 0xd8613350, /// 0xd7c
			 0x2c9d9105, /// 0xd80
			 0xe6c44566, /// 0xd84
			 0x0541b3ae, /// 0xd88
			 0x58f007b9, /// 0xd8c
			 0x76acf36f, /// 0xd90
			 0xf6c58b0e, /// 0xd94
			 0x9c8a06af, /// 0xd98
			 0x39ee35a8, /// 0xd9c
			 0x5ad30e2f, /// 0xda0
			 0x8411e236, /// 0xda4
			 0xee22d4d4, /// 0xda8
			 0x77cc4e83, /// 0xdac
			 0x88de3e93, /// 0xdb0
			 0x760914dd, /// 0xdb4
			 0x6e971a32, /// 0xdb8
			 0x6503d490, /// 0xdbc
			 0x682b8d40, /// 0xdc0
			 0x7a62727a, /// 0xdc4
			 0x4a84a8e5, /// 0xdc8
			 0x919e9b83, /// 0xdcc
			 0xe21c20b1, /// 0xdd0
			 0x0fe839b0, /// 0xdd4
			 0x731f1c9c, /// 0xdd8
			 0xcd32ebc8, /// 0xddc
			 0xe7bb608d, /// 0xde0
			 0xcb9c506e, /// 0xde4
			 0xcd83207e, /// 0xde8
			 0xbd5af14a, /// 0xdec
			 0xf37a4c4a, /// 0xdf0
			 0x2eb8fc9f, /// 0xdf4
			 0xb3a4eb3a, /// 0xdf8
			 0x4ae9a173, /// 0xdfc
			 0xa418ea41, /// 0xe00
			 0x700a9ed7, /// 0xe04
			 0x0242c5eb, /// 0xe08
			 0xc004fb70, /// 0xe0c
			 0x88b85f11, /// 0xe10
			 0x85abebf9, /// 0xe14
			 0x7eb38da4, /// 0xe18
			 0x632b0749, /// 0xe1c
			 0x9106b8c2, /// 0xe20
			 0x23d3ef36, /// 0xe24
			 0x56c0eecd, /// 0xe28
			 0x5882e8b4, /// 0xe2c
			 0x6f6d772c, /// 0xe30
			 0xc5290168, /// 0xe34
			 0xbe22dd78, /// 0xe38
			 0x5cae8cad, /// 0xe3c
			 0xcc8476a5, /// 0xe40
			 0xa5334cce, /// 0xe44
			 0x280ddd19, /// 0xe48
			 0xd31c8333, /// 0xe4c
			 0x8d00fc4c, /// 0xe50
			 0x4eb028a4, /// 0xe54
			 0xa16118e2, /// 0xe58
			 0x6d77373e, /// 0xe5c
			 0x6dfedd8f, /// 0xe60
			 0xcdf07f1c, /// 0xe64
			 0xdb541532, /// 0xe68
			 0x7ed71471, /// 0xe6c
			 0x123eaec4, /// 0xe70
			 0xb1654b57, /// 0xe74
			 0x481a5e95, /// 0xe78
			 0xcffbc613, /// 0xe7c
			 0x3a14a55d, /// 0xe80
			 0xd2471a3c, /// 0xe84
			 0xee2b4829, /// 0xe88
			 0x41ae9649, /// 0xe8c
			 0xb3c70308, /// 0xe90
			 0xbb72d0bb, /// 0xe94
			 0x3a55489b, /// 0xe98
			 0xde504eb4, /// 0xe9c
			 0xe32d78d2, /// 0xea0
			 0x55c9f65e, /// 0xea4
			 0xb8bb106d, /// 0xea8
			 0xa5bbddb3, /// 0xeac
			 0xf03cad75, /// 0xeb0
			 0x4d906191, /// 0xeb4
			 0xfc810525, /// 0xeb8
			 0x84bc24e3, /// 0xebc
			 0x92f3dcbd, /// 0xec0
			 0x4a104aff, /// 0xec4
			 0x03c8542c, /// 0xec8
			 0x842ae991, /// 0xecc
			 0xd2eca71f, /// 0xed0
			 0x867424e9, /// 0xed4
			 0xc1918de7, /// 0xed8
			 0x7180dda4, /// 0xedc
			 0x9ad46d7c, /// 0xee0
			 0xc82ab8ec, /// 0xee4
			 0x0a90cd1c, /// 0xee8
			 0xd54b206c, /// 0xeec
			 0xe496d03e, /// 0xef0
			 0x5c0be134, /// 0xef4
			 0x56459bd7, /// 0xef8
			 0x564bef29, /// 0xefc
			 0xa0a416d7, /// 0xf00
			 0xe36a97c5, /// 0xf04
			 0xdfcd06f4, /// 0xf08
			 0xfdd1180b, /// 0xf0c
			 0x814d6070, /// 0xf10
			 0x161581d4, /// 0xf14
			 0x883649d9, /// 0xf18
			 0xc11f20ba, /// 0xf1c
			 0xf2d43813, /// 0xf20
			 0x0de2cf12, /// 0xf24
			 0x28f8eba9, /// 0xf28
			 0x3557419f, /// 0xf2c
			 0xbc05c421, /// 0xf30
			 0x139e779b, /// 0xf34
			 0xad61abec, /// 0xf38
			 0x4852176d, /// 0xf3c
			 0xf61c00b8, /// 0xf40
			 0xc35725bb, /// 0xf44
			 0x9595fe29, /// 0xf48
			 0xdb6ec3a9, /// 0xf4c
			 0x4648f157, /// 0xf50
			 0x5cf835b0, /// 0xf54
			 0x62c961f0, /// 0xf58
			 0x3cda0237, /// 0xf5c
			 0xb014555c, /// 0xf60
			 0xf3de1b42, /// 0xf64
			 0xb0891ed9, /// 0xf68
			 0x0cdf05fc, /// 0xf6c
			 0x11c680b8, /// 0xf70
			 0x772a463a, /// 0xf74
			 0x28810870, /// 0xf78
			 0x73891f63, /// 0xf7c
			 0xf655ad38, /// 0xf80
			 0x59a79385, /// 0xf84
			 0x6d61f014, /// 0xf88
			 0x2f0984b2, /// 0xf8c
			 0xb84c0afa, /// 0xf90
			 0xb36bc5da, /// 0xf94
			 0x120b9be9, /// 0xf98
			 0x097577b4, /// 0xf9c
			 0xc8ff6237, /// 0xfa0
			 0x9cf58b07, /// 0xfa4
			 0x86a843e6, /// 0xfa8
			 0x98230209, /// 0xfac
			 0x86cfce5e, /// 0xfb0
			 0x36ba72ba, /// 0xfb4
			 0xd6ec34a7, /// 0xfb8
			 0x467dfe7f, /// 0xfbc
			 0xeda642f8, /// 0xfc0
			 0x46278bf0, /// 0xfc4
			 0x71047ec0, /// 0xfc8
			 0x101010a2, /// 0xfcc
			 0xd1de74ba, /// 0xfd0
			 0x6e6febd8, /// 0xfd4
			 0x4941c480, /// 0xfd8
			 0xc55d03f3, /// 0xfdc
			 0x67176cc1, /// 0xfe0
			 0xb824b4a0, /// 0xfe4
			 0x3b0156b7, /// 0xfe8
			 0x78d6e0bd, /// 0xfec
			 0x09d9902f, /// 0xff0
			 0x3549405d, /// 0xff4
			 0xaa77dffa, /// 0xff8
			 0x7b81623c /// last
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
			 0x0000079c,
			 0x000001ec,
			 0x000007a8,
			 0x00000248,
			 0x00000074,
			 0x00000298,
			 0x00000284,
			 0x0000061c,

			 /// vpu register f2
			 0x41d80000,
			 0x41c00000,
			 0x41e80000,
			 0x40e00000,
			 0x41a00000,
			 0x40800000,
			 0x41800000,
			 0x41c80000,

			 /// vpu register f3
			 0x41e00000,
			 0x40c00000,
			 0x40800000,
			 0x41a80000,
			 0x41f80000,
			 0x40a00000,
			 0x41300000,
			 0x41c00000,

			 /// vpu register f4
			 0x41500000,
			 0x41300000,
			 0x40a00000,
			 0x41c80000,
			 0x3f800000,
			 0x40400000,
			 0x41980000,
			 0x41000000,

			 /// vpu register f5
			 0x41c80000,
			 0x40800000,
			 0x41b80000,
			 0x41a80000,
			 0x41c00000,
			 0x41400000,
			 0x41700000,
			 0x40e00000,

			 /// vpu register f6
			 0x41700000,
			 0x41000000,
			 0x40c00000,
			 0x41880000,
			 0x41c80000,
			 0x41700000,
			 0x41e00000,
			 0x40c00000,

			 /// vpu register f7
			 0x41980000,
			 0x41d80000,
			 0x41c80000,
			 0x41a00000,
			 0x41800000,
			 0x42000000,
			 0x40400000,
			 0x41300000,

			 /// vpu register f8
			 0x41880000,
			 0x41e00000,
			 0x40e00000,
			 0x41100000,
			 0x40e00000,
			 0x40e00000,
			 0x41600000,
			 0x41b00000,

			 /// vpu register f9
			 0x41880000,
			 0x41500000,
			 0x41880000,
			 0x41c80000,
			 0x41d00000,
			 0x40800000,
			 0x3f800000,
			 0x41d00000,

			 /// vpu register f10
			 0x41500000,
			 0x40c00000,
			 0x41a80000,
			 0x3f800000,
			 0x41e80000,
			 0x41600000,
			 0x41800000,
			 0x41c00000,

			 /// vpu register f11
			 0x40000000,
			 0x41a00000,
			 0x40000000,
			 0x41400000,
			 0x40400000,
			 0x40c00000,
			 0x41d80000,
			 0x41b80000,

			 /// vpu register f12
			 0x41b00000,
			 0x40a00000,
			 0x40800000,
			 0x40000000,
			 0x3f800000,
			 0x42000000,
			 0x40a00000,
			 0x41500000,

			 /// vpu register f13
			 0x42000000,
			 0x41a80000,
			 0x41800000,
			 0x41300000,
			 0x41d00000,
			 0x41100000,
			 0x40000000,
			 0x40800000,

			 /// vpu register f14
			 0x40800000,
			 0x41a00000,
			 0x41a00000,
			 0x41d00000,
			 0x41c80000,
			 0x41100000,
			 0x41b00000,
			 0x40c00000,

			 /// vpu register f15
			 0x41300000,
			 0x40e00000,
			 0x40a00000,
			 0x41d80000,
			 0x41700000,
			 0x41a80000,
			 0x40a00000,
			 0x41100000,

			 /// vpu register f16
			 0x41e80000,
			 0x41d00000,
			 0x41880000,
			 0x41880000,
			 0x41f00000,
			 0x41a80000,
			 0x41900000,
			 0x41700000,

			 /// vpu register f17
			 0x41800000,
			 0x41880000,
			 0x41d80000,
			 0x41d00000,
			 0x41a80000,
			 0x40a00000,
			 0x41e00000,
			 0x41700000,

			 /// vpu register f18
			 0x40e00000,
			 0x41400000,
			 0x41b00000,
			 0x41400000,
			 0x41c00000,
			 0x42000000,
			 0x41300000,
			 0x41500000,

			 /// vpu register f19
			 0x3f800000,
			 0x41100000,
			 0x41b80000,
			 0x41880000,
			 0x41980000,
			 0x41880000,
			 0x40800000,
			 0x41b00000,

			 /// vpu register f20
			 0x41b00000,
			 0x41a80000,
			 0x41200000,
			 0x41200000,
			 0x41880000,
			 0x40400000,
			 0x3f800000,
			 0x41000000,

			 /// vpu register f21
			 0x41d00000,
			 0x41100000,
			 0x41b80000,
			 0x41f80000,
			 0x3f800000,
			 0x41f80000,
			 0x42000000,
			 0x40800000,

			 /// vpu register f22
			 0x41c00000,
			 0x41f80000,
			 0x40a00000,
			 0x41f00000,
			 0x41a80000,
			 0x41e00000,
			 0x41b00000,
			 0x41700000,

			 /// vpu register f23
			 0x41f80000,
			 0x41d80000,
			 0x40400000,
			 0x40000000,
			 0x40000000,
			 0x3f800000,
			 0x41300000,
			 0x40000000,

			 /// vpu register f24
			 0x41a00000,
			 0x41800000,
			 0x41880000,
			 0x40400000,
			 0x41a80000,
			 0x41d00000,
			 0x41f00000,
			 0x40800000,

			 /// vpu register f25
			 0x42000000,
			 0x41e80000,
			 0x41a80000,
			 0x41100000,
			 0x41f80000,
			 0x40a00000,
			 0x3f800000,
			 0x41880000,

			 /// vpu register f26
			 0x41800000,
			 0x40400000,
			 0x41e00000,
			 0x40c00000,
			 0x40800000,
			 0x41e80000,
			 0x40a00000,
			 0x41a00000,

			 /// vpu register f27
			 0x41980000,
			 0x41600000,
			 0x3f800000,
			 0x42000000,
			 0x40400000,
			 0x42000000,
			 0x41800000,
			 0x41200000,

			 /// vpu register f28
			 0x41e80000,
			 0x40400000,
			 0x40e00000,
			 0x41400000,
			 0x41b80000,
			 0x3f800000,
			 0x41c80000,
			 0x41980000,

			 /// vpu register f29
			 0x41f80000,
			 0x42000000,
			 0x40400000,
			 0x41900000,
			 0x41880000,
			 0x41d00000,
			 0x41d80000,
			 0x40a00000,

			 /// vpu register f30
			 0x41a00000,
			 0x41d00000,
			 0x40a00000,
			 0x41b80000,
			 0x40c00000,
			 0x40400000,
			 0x41b80000,
			 0x41800000,

			 /// vpu register f31
			 0x40400000,
			 0x41000000,
			 0x41e00000,
			 0x41400000,
			 0x40400000,
			 0x41700000,
			 0x41000000,
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
		"fmax.ps f7, f2, f29\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f2, f1, f19\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f24, f25, f8\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f18, f21, f3\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f26, f25, f4\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f22, f24, f18\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f25, f5, f18\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f24, f3, f9\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f23, f10, f29\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f23, f22, f25\n"                             ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f28, f15, f22\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f13, f7, f5\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f13, f17, f3\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f26, f23, f17\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f14, f25, f6\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f6, f18, f8\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f2, f23, f12\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f30, f21, f18\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f7, f19, f8\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f17, f10, f14\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f24, f1, f8\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f27, f1, f13\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f20, f11, f3\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f21, f18, f3\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f28, f11, f18\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f16, f8, f21\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f29, f21, f4\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f5, f3, f18\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f15, f19, f7\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f5, f21, f13\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f27, f23, f16\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f12, f7, f7\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f7, f1, f29\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f21, f20, f13\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f29, f30, f10\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f14, f16, f16\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f15, f17, f26\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f4, f11, f24\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f13, f7, f1\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f29, f2, f13\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f8, f18, f27\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f5, f13, f30\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f28, f12, f14\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f23, f29, f17\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f2, f30, f27\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f8, f21, f6\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f17, f10, f21\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f2, f2, f30\n"                               ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f7, f6, f24\n"                               ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f19, f7, f1\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f10, f14, f11\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f2, f0, f3\n"                                ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f11, f27, f25\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f16, f24, f14\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f22, f3, f9\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f11, f0, f7\n"                               ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f3, f26, f20\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f21, f18, f26\n"                             ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f25, f18, f21\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f18, f20, f17\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f30, f29, f15\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f9, f27, f21\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f12, f22, f14\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f20, f16, f19\n"                             ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f28, f11, f30\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f15, f18, f12\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f21, f2, f13\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f20, f30, f5\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f11, f5, f14\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f15, f30, f7\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f16, f10, f13\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f26, f7, f4\n"                               ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f12, f12, f3\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f24, f18, f24\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f22, f2, f12\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f24, f9, f9\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f30, f23, f12\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f12, f14, f12\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f30, f0, f8\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f21, f17, f13\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f14, f10, f15\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f27, f29, f24\n"                             ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f20, f11, f10\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f13, f27, f0\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f24, f2, f4\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f24, f3, f12\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f27, f11, f18\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f5, f21, f8\n"                               ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f15, f22, f17\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f17, f22, f3\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f7, f10, f22\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f10, f12, f14\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f7, f6, f2\n"                                ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f8, f6, f1\n"                                ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f30, f18, f20\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f15, f11, f18\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f18, f9, f12\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f23, f22, f4\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.ps f9, f24, f11\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
