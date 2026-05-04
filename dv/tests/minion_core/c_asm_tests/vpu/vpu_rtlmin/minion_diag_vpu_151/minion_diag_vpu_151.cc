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
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00000
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00004
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00008
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0000c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00010
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00014
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00018
			 0xbf028f5c,                                                  // -0.51                                       /// 0001c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00020
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0002c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00030
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00034
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00038
			 0xcb000000,                                                  // -8388608.0                                  /// 0003c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00040
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00044
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00048
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0004c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00050
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00054
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00058
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0005c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00060
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00068
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0006c
			 0x55555555,                                                  // 4 random values                             /// 00070
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00074
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0007c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00088
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00094
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00098
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000a0
			 0x00000000,                                                  // zero                                        /// 000a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 000b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 000cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 000e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 000e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00104
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00108
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0010c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00110
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0011c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00120
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00128
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0012c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00134
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00138
			 0x80000000,                                                  // -zero                                       /// 0013c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00144
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00148
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0014c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00154
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00158
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00160
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00164
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00168
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00170
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0017c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x80000000,                                                  // -zero                                       /// 0018c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00190
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00194
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00198
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0019c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00200
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00204
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00208
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0xcb000000,                                                  // -8388608.0                                  /// 00210
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00214
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x0e000003,                                                  // Trailing 1s:                                /// 0021c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00228
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0022c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00230
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00234
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00238
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00240
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x4b000000,                                                  // 8388608.0                                   /// 0024c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00250
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00254
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00258
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00260
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00264
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00268
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0026c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00270
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00274
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00280
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0xcb000000,                                                  // -8388608.0                                  /// 00288
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0028c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00290
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00294
			 0x0c780000,                                                  // Leading 1s:                                 /// 00298
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x3f028f5c,                                                  // 0.51                                        /// 002a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x33333333,                                                  // 4 random values                             /// 002bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00300
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00304
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0030c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00310
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00314
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00318
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00320
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00324
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00328
			 0x7f800000,                                                  // inf                                         /// 0032c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00330
			 0x0e000001,                                                  // Trailing 1s:                                /// 00334
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0033c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00340
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00344
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00348
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00350
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00354
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00358
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0035c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00368
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00370
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00374
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00378
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0037c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00388
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0038c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00390
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0039c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x00000000,                                                  // zero                                        /// 003b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003d4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00400
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00408
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0040c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00410
			 0x33333333,                                                  // 4 random values                             /// 00414
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x7fc00001,                                                  // signaling NaN                               /// 00420
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00428
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0042c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00430
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00434
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00438
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0043c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00444
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00448
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0044c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00450
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x0e000003,                                                  // Trailing 1s:                                /// 00458
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00460
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00464
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00468
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0046c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0xbf028f5c,                                                  // -0.51                                       /// 00474
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00478
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0047c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00480
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00484
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00488
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00490
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00494
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00498
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004bc
			 0x33333333,                                                  // 4 random values                             /// 004c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004c4
			 0xcb000000,                                                  // -8388608.0                                  /// 004c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x33333333,                                                  // 4 random values                             /// 004ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0050c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00510
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0c780000,                                                  // Leading 1s:                                 /// 0051c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00520
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00524
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00528
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0052c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00534
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x0c780000,                                                  // Leading 1s:                                 /// 0053c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00540
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00548
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0054c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00554
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00558
			 0x0e000003,                                                  // Trailing 1s:                                /// 0055c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00560
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00564
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00568
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0056c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x80000000,                                                  // -zero                                       /// 0057c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x80011111,                                                  // -9.7958E-41                                 /// 00584
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00588
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0058c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00590
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00594
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00598
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0059c
			 0x0c780000,                                                  // Leading 1s:                                 /// 005a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00600
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00604
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0c700000,                                                  // Leading 1s:                                 /// 0060c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00610
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00620
			 0x0c400000,                                                  // Leading 1s:                                 /// 00624
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00630
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00638
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00640
			 0x0c600000,                                                  // Leading 1s:                                 /// 00644
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00648
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0xff800000,                                                  // -inf                                        /// 00654
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00658
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0065c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00660
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00664
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x0e000001,                                                  // Trailing 1s:                                /// 0066c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00670
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00674
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0067c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0xff800000,                                                  // -inf                                        /// 0068c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00694
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00698
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0069c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 006c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006f0
			 0x55555555,                                                  // 4 random values                             /// 006f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00700
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00704
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00708
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0070c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00710
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00714
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0xffc00001,                                                  // -signaling NaN                              /// 0071c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00724
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00730
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00734
			 0xffc00001,                                                  // -signaling NaN                              /// 00738
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0073c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00740
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00744
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00748
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00750
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00754
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00758
			 0xbf028f5c,                                                  // -0.51                                       /// 0075c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00760
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00768
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0076c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00770
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00778
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0077c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00784
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00788
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0078c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00790
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00794
			 0x0c700000,                                                  // Leading 1s:                                 /// 00798
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007a0
			 0x55555555,                                                  // 4 random values                             /// 007a4
			 0x7f800000,                                                  // inf                                         /// 007a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00800
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00804
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00808
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0080c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00810
			 0x00011111,                                                  // 9.7958E-41                                  /// 00814
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00818
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0081c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00820
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00824
			 0x0e000007,                                                  // Trailing 1s:                                /// 00828
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00830
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00834
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00840
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00844
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0084c
			 0x7fc00001,                                                  // signaling NaN                               /// 00850
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00858
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00860
			 0x4b000000,                                                  // 8388608.0                                   /// 00864
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00868
			 0x4b000000,                                                  // 8388608.0                                   /// 0086c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00870
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0c600000,                                                  // Leading 1s:                                 /// 00878
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0087c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00880
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00890
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00894
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0089c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008a0
			 0xffc00001,                                                  // -signaling NaN                              /// 008a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 008b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008f8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x33333333,                                                  // 4 random values                             /// 00904
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0090c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00910
			 0x0e000001,                                                  // Trailing 1s:                                /// 00914
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00918
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0091c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00920
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0xbf028f5c,                                                  // -0.51                                       /// 00928
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00930
			 0x0e000007,                                                  // Trailing 1s:                                /// 00934
			 0x00011111,                                                  // 9.7958E-41                                  /// 00938
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0093c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00944
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0094c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00950
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00954
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00960
			 0x7fc00001,                                                  // signaling NaN                               /// 00964
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00968
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0096c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00974
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0097c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00980
			 0x7f800000,                                                  // inf                                         /// 00984
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00990
			 0x3f028f5c,                                                  // 0.51                                        /// 00994
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00998
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 009c8
			 0x7fc00001,                                                  // signaling NaN                               /// 009cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0xffc00001,                                                  // -signaling NaN                              /// 009e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a0c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a20
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a3c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a40
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a44
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a54
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a5c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a60
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a68
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a6c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a74
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a7c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a94
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x00000000,                                                  // zero                                        /// 00aa8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00aac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ab0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00abc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ac0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00acc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ad0
			 0x33333333,                                                  // 4 random values                             /// 00ad4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00adc
			 0xff800000,                                                  // -inf                                        /// 00ae0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00aec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00af0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00af4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00af8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b00
			 0x7f800000,                                                  // inf                                         /// 00b04
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b0c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b18
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b1c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b28
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x80000000,                                                  // -zero                                       /// 00b38
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b3c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b40
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b48
			 0x00000000,                                                  // zero                                        /// 00b4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b54
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b58
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b64
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b78
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x7f800000,                                                  // inf                                         /// 00b8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b90
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ba0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ba4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ba8
			 0xbf028f5c,                                                  // -0.51                                       /// 00bac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bb8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bbc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bc8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bcc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bd0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bd4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bd8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bdc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00be4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00be8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bf0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bf8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c0c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c14
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c18
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c24
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c28
			 0x4b000000,                                                  // 8388608.0                                   /// 00c2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c30
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c48
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x7f800000,                                                  // inf                                         /// 00c50
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c54
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c58
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c60
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c68
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c8c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c90
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ca4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ca8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cb0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cb4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cbc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cc0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cd0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cdc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ce4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ce8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cf0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cf8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cfc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d04
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d14
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d18
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d20
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d24
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d30
			 0x4b000000,                                                  // 8388608.0                                   /// 00d34
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d38
			 0x55555555,                                                  // 4 random values                             /// 00d3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d5c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d60
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d68
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d88
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d8c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d90
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d94
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d9c
			 0x55555555,                                                  // 4 random values                             /// 00da0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00da4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00da8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00db8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dc0
			 0x80000000,                                                  // -zero                                       /// 00dc4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dc8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dcc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dd0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ddc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00de0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00de4
			 0x80000000,                                                  // -zero                                       /// 00de8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dfc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e08
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e0c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e14
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e40
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e44
			 0xffc00001,                                                  // -signaling NaN                              /// 00e48
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e50
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e54
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e60
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e68
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e70
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e88
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e94
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e98
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ea8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00eac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00eb0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ebc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ec0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ec4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x00000000,                                                  // zero                                        /// 00ecc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ee4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ee8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00eec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ef0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f00
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f04
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f34
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0xbf028f5c,                                                  // -0.51                                       /// 00f3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f48
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f50
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f5c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f64
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f70
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f74
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f80
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f84
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f90
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fa8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fb4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fbc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fc0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fc8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fd0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fd8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fe4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ff4
			 0x80000000,                                                  // -zero                                       /// 00ff8
			 0xffc00000                                                  // -cquiet NaN                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80000000,                                                  // -zero                                       /// 00000
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00004
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00008
			 0x00011111,                                                  // 9.7958E-41                                  /// 0000c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00010
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00018
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0001c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00028
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0002c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0xff800000,                                                  // -inf                                        /// 00034
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00040
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0xbf028f5c,                                                  // -0.51                                       /// 00048
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00050
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00054
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00058
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00064
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00068
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0006c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00070
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0007c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00080
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00084
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00088
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0008c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00090
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00094
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00098
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0009c
			 0x7f800000,                                                  // inf                                         /// 000a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000cc
			 0x55555555,                                                  // 4 random values                             /// 000d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000f8
			 0x4b000000,                                                  // 8388608.0                                   /// 000fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0xff800000,                                                  // -inf                                        /// 00104
			 0xcb000000,                                                  // -8388608.0                                  /// 00108
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00110
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00114
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00118
			 0x00011111,                                                  // 9.7958E-41                                  /// 0011c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00120
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00124
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00128
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0012c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00130
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00134
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0013c
			 0x3f028f5c,                                                  // 0.51                                        /// 00140
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0014c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0e000007,                                                  // Trailing 1s:                                /// 00154
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0015c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00160
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00164
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00168
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0016c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00170
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00174
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00178
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0017c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00180
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00188
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00190
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00198
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0019c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001e8
			 0x4b000000,                                                  // 8388608.0                                   /// 001ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 001f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x4b000000,                                                  // 8388608.0                                   /// 00208
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0020c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00214
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00218
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0021c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00220
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x80011111,                                                  // -9.7958E-41                                 /// 00228
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0022c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00230
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00238
			 0x55555555,                                                  // 4 random values                             /// 0023c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00240
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00248
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0024c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00258
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0025c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00260
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0xbf028f5c,                                                  // -0.51                                       /// 00268
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0026c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00270
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00274
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00278
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00280
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00284
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00288
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x7f800000,                                                  // inf                                         /// 00290
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 002c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x3f028f5c,                                                  // 0.51                                        /// 002f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002f8
			 0x33333333,                                                  // 4 random values                             /// 002fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00300
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00304
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00308
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0030c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00314
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00324
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00328
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00330
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0033c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00348
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0034c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00358
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00364
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00368
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00370
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00378
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0037c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00380
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00388
			 0x4b000000,                                                  // 8388608.0                                   /// 0038c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00390
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00394
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00398
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 003a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003d0
			 0x7f800000,                                                  // inf                                         /// 003d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00400
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00404
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00410
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00414
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00418
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0041c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00424
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00428
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00430
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00438
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0043c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0c780000,                                                  // Leading 1s:                                 /// 00444
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x4b000000,                                                  // 8388608.0                                   /// 0044c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00450
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00458
			 0x33333333,                                                  // 4 random values                             /// 0045c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00460
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00468
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00474
			 0x0e000007,                                                  // Trailing 1s:                                /// 00478
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0047c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00480
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x33333333,                                                  // 4 random values                             /// 00488
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0048c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00490
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00494
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00498
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004c4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00500
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00504
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00508
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0050c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00510
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00518
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0051c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00520
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00524
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00528
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00530
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00534
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00540
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00544
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0xbf028f5c,                                                  // -0.51                                       /// 0054c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00554
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00560
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00564
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0c780000,                                                  // Leading 1s:                                 /// 0056c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00570
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0057c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00580
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00584
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00590
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00594
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00598
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0059c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 005f0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00600
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00604
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00608
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0060c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00610
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00614
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00618
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00620
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00624
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00628
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0062c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00630
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00634
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0063c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00640
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00644
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00648
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00650
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00658
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00660
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00664
			 0x7fc00001,                                                  // signaling NaN                               /// 00668
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00670
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x80000000,                                                  // -zero                                       /// 00678
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0067c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00680
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00684
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00688
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00694
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00698
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0069c
			 0x7fc00001,                                                  // signaling NaN                               /// 006a0
			 0xffc00001,                                                  // -signaling NaN                              /// 006a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 006d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00700
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00704
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00708
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x0c780000,                                                  // Leading 1s:                                 /// 00714
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0071c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00720
			 0x0e000001,                                                  // Trailing 1s:                                /// 00724
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00728
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00730
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0073c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00740
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00744
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00748
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0074c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00750
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00754
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00758
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0075c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00760
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00770
			 0x0e000007,                                                  // Trailing 1s:                                /// 00774
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x33333333,                                                  // 4 random values                             /// 00788
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0078c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00790
			 0xffc00001,                                                  // -signaling NaN                              /// 00794
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0e000001,                                                  // Trailing 1s:                                /// 0079c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00810
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00818
			 0x80011111,                                                  // -9.7958E-41                                 /// 0081c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00824
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00828
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0082c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00834
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00838
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0083c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x00011111,                                                  // 9.7958E-41                                  /// 00844
			 0x3f028f5c,                                                  // 0.51                                        /// 00848
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00850
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00854
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00858
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0085c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00864
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00870
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00874
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0087c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00880
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00884
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00888
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0088c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00894
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00898
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008c4
			 0x7f800000,                                                  // inf                                         /// 008c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00900
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00904
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0090c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00910
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00914
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00918
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00920
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00924
			 0x0e000007,                                                  // Trailing 1s:                                /// 00928
			 0xbf028f5c,                                                  // -0.51                                       /// 0092c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00930
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00934
			 0xcb000000,                                                  // -8388608.0                                  /// 00938
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0093c
			 0x00000000,                                                  // zero                                        /// 00940
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0094c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00954
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0095c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00960
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00964
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00968
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00970
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00974
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00978
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00980
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00984
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0098c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00994
			 0x7fc00001,                                                  // signaling NaN                               /// 00998
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0099c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009b8
			 0xcb000000,                                                  // -8388608.0                                  /// 009bc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009d4
			 0x55555555,                                                  // 4 random values                             /// 009d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009e4
			 0xff800000,                                                  // -inf                                        /// 009e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 009ec
			 0xbf028f5c,                                                  // -0.51                                       /// 009f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a0c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a18
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a24
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a28
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00000000,                                                  // zero                                        /// 00a30
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a38
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a40
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a44
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a48
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a50
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a54
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a60
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a64
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a68
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a74
			 0xcb000000,                                                  // -8388608.0                                  /// 00a78
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a8c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a90
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a94
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00aa0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00aa8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00aac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ab4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ac0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ac4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00acc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ad0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ad8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00adc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ae0
			 0x80000000,                                                  // -zero                                       /// 00ae4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ae8
			 0x3f028f5c,                                                  // 0.51                                        /// 00aec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00af0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00af4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00af8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00afc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b2c
			 0x00000000,                                                  // zero                                        /// 00b30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b3c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b40
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b4c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b50
			 0x3f028f5c,                                                  // 0.51                                        /// 00b54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b70
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b88
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b8c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b90
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b9c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ba4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bb0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bb8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bbc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bc0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bc4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bd0
			 0x7f800000,                                                  // inf                                         /// 00bd4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bdc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00be0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00be8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c00
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c14
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c18
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c20
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c24
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c2c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c44
			 0x33333333,                                                  // 4 random values                             /// 00c48
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c78
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c7c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c8c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c94
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ca4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ca8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cb8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cbc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cc8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ccc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cd4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cdc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ce4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ce8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cf0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cf4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cf8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d04
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d0c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d28
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d30
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d54
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d60
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d64
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d68
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d74
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d80
			 0xffc00001,                                                  // -signaling NaN                              /// 00d84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d9c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00da0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00da8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00db4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00db8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dbc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dc0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dc4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dc8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dcc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dd0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dd4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dd8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00de4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00de8
			 0x3f028f5c,                                                  // 0.51                                        /// 00dec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00df0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00df4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0xbf028f5c,                                                  // -0.51                                       /// 00dfc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e00
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e14
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e18
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e1c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e3c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e50
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e58
			 0x4b000000,                                                  // 8388608.0                                   /// 00e5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e68
			 0x33333333,                                                  // 4 random values                             /// 00e6c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e80
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e88
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e98
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ea0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ea4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ea8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00eac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00eb0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0xbf028f5c,                                                  // -0.51                                       /// 00ebc
			 0xff800000,                                                  // -inf                                        /// 00ec0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ec4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ec8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ecc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ed0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ed4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ed8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00edc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ee0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ee4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ee8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00eec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ef0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ef8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f04
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f1c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f20
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f24
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f30
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f40
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f48
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f60
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f68
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f70
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f74
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f80
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f88
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f8c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f90
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f98
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fb0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fb4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fc4
			 0x33333333,                                                  // 4 random values                             /// 00fc8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fd0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fd8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fdc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fe0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fe4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ff0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ff8
			 0x0c700000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xfc06062b, /// 0x0
			 0x3c9ef345, /// 0x4
			 0x77040197, /// 0x8
			 0x69af0207, /// 0xc
			 0x736bb6cf, /// 0x10
			 0x1fb0524e, /// 0x14
			 0xd7f3a54d, /// 0x18
			 0x4c4c4299, /// 0x1c
			 0xa6b830a3, /// 0x20
			 0x64b7e51a, /// 0x24
			 0xea25b3e0, /// 0x28
			 0xfe18dfda, /// 0x2c
			 0xa992675f, /// 0x30
			 0xbb6e9c72, /// 0x34
			 0x999fe328, /// 0x38
			 0xeb0293de, /// 0x3c
			 0xc2d2e0f5, /// 0x40
			 0x8b1d4567, /// 0x44
			 0x9559841a, /// 0x48
			 0x299eefcb, /// 0x4c
			 0xfb573727, /// 0x50
			 0x9c366433, /// 0x54
			 0xc23d6a49, /// 0x58
			 0xe98ab243, /// 0x5c
			 0xd76f56aa, /// 0x60
			 0xa4064d7e, /// 0x64
			 0xbedc1ad6, /// 0x68
			 0x63ea2ac0, /// 0x6c
			 0x2b960bbb, /// 0x70
			 0x12c060b0, /// 0x74
			 0xfd662758, /// 0x78
			 0xe6a8f536, /// 0x7c
			 0x4f5770a0, /// 0x80
			 0x2b0b8976, /// 0x84
			 0x1d680be5, /// 0x88
			 0x90b3f6e8, /// 0x8c
			 0xf77c4d02, /// 0x90
			 0xf1e3704a, /// 0x94
			 0x77ae17c0, /// 0x98
			 0x3b33dda2, /// 0x9c
			 0x1bfcc4b8, /// 0xa0
			 0xbc2eac63, /// 0xa4
			 0xa854e56c, /// 0xa8
			 0xe334b177, /// 0xac
			 0xe346f854, /// 0xb0
			 0x226cf33b, /// 0xb4
			 0x891adcd3, /// 0xb8
			 0xc897d51b, /// 0xbc
			 0xb9fb72dd, /// 0xc0
			 0x229886d2, /// 0xc4
			 0x5130b01d, /// 0xc8
			 0x79b036fa, /// 0xcc
			 0x8ceee434, /// 0xd0
			 0x8ba09f66, /// 0xd4
			 0x1c9b1676, /// 0xd8
			 0xf2f44c73, /// 0xdc
			 0xdcb88c4c, /// 0xe0
			 0x271a8797, /// 0xe4
			 0xde010d78, /// 0xe8
			 0x71277982, /// 0xec
			 0xf9bb221a, /// 0xf0
			 0x2b2ce3bf, /// 0xf4
			 0x60ec7010, /// 0xf8
			 0xeb5c01e3, /// 0xfc
			 0x4c64ce7a, /// 0x100
			 0x9996daca, /// 0x104
			 0x0d1b0efb, /// 0x108
			 0x9fec2e5d, /// 0x10c
			 0xc38172cb, /// 0x110
			 0xe73c6ed3, /// 0x114
			 0xf5618789, /// 0x118
			 0xbd031f6b, /// 0x11c
			 0xb9881f99, /// 0x120
			 0x856ebb16, /// 0x124
			 0x5c91d29a, /// 0x128
			 0x4d774a7d, /// 0x12c
			 0x9112371c, /// 0x130
			 0x94e20cbb, /// 0x134
			 0x2b979223, /// 0x138
			 0x9ccf4ebd, /// 0x13c
			 0x4b26453d, /// 0x140
			 0x7de596ed, /// 0x144
			 0x6a12c141, /// 0x148
			 0xdf052515, /// 0x14c
			 0xcb1542b7, /// 0x150
			 0xcf503814, /// 0x154
			 0x50fc784d, /// 0x158
			 0x57c47596, /// 0x15c
			 0x5023e928, /// 0x160
			 0x0bc6a297, /// 0x164
			 0x8e410620, /// 0x168
			 0x515aae64, /// 0x16c
			 0x0ea645b8, /// 0x170
			 0xeb144dbd, /// 0x174
			 0x7a3875ce, /// 0x178
			 0xe206fec9, /// 0x17c
			 0x59aec070, /// 0x180
			 0x27de9c30, /// 0x184
			 0x9a39c065, /// 0x188
			 0x316d55bb, /// 0x18c
			 0x6dc63d6e, /// 0x190
			 0x26d4edee, /// 0x194
			 0xec9aba17, /// 0x198
			 0x4b3b48eb, /// 0x19c
			 0x0d30096c, /// 0x1a0
			 0xaadb02f9, /// 0x1a4
			 0x1b4b804c, /// 0x1a8
			 0x9c6444cc, /// 0x1ac
			 0xe6e6c728, /// 0x1b0
			 0x418f654c, /// 0x1b4
			 0x80339d51, /// 0x1b8
			 0x0268d21c, /// 0x1bc
			 0xd6debcc0, /// 0x1c0
			 0x55d6fc26, /// 0x1c4
			 0x1af999ca, /// 0x1c8
			 0x215364b6, /// 0x1cc
			 0x35f02447, /// 0x1d0
			 0x54403f74, /// 0x1d4
			 0x8cdd285e, /// 0x1d8
			 0xcb129c85, /// 0x1dc
			 0xb9d95507, /// 0x1e0
			 0x0660eee6, /// 0x1e4
			 0x3048b2f3, /// 0x1e8
			 0x127023d1, /// 0x1ec
			 0xc38a1257, /// 0x1f0
			 0x1362ac19, /// 0x1f4
			 0x4579933c, /// 0x1f8
			 0x11d46daf, /// 0x1fc
			 0xd048be48, /// 0x200
			 0x8b53956b, /// 0x204
			 0xb72463d5, /// 0x208
			 0x73ae47bb, /// 0x20c
			 0xaebad251, /// 0x210
			 0xeeb33e59, /// 0x214
			 0x5bf36994, /// 0x218
			 0xe0c27a28, /// 0x21c
			 0x5a977c7f, /// 0x220
			 0xef6bdf09, /// 0x224
			 0x57f78838, /// 0x228
			 0x94df86ad, /// 0x22c
			 0xd192b565, /// 0x230
			 0x50e37610, /// 0x234
			 0x59248878, /// 0x238
			 0x29b040d4, /// 0x23c
			 0x420fab8a, /// 0x240
			 0xf700d862, /// 0x244
			 0x35837e55, /// 0x248
			 0x45bc8887, /// 0x24c
			 0xe8005d67, /// 0x250
			 0x391f5b48, /// 0x254
			 0x99cf0328, /// 0x258
			 0xed7d4c90, /// 0x25c
			 0xf962bc7b, /// 0x260
			 0x0c87a208, /// 0x264
			 0x708749c1, /// 0x268
			 0xc90001c7, /// 0x26c
			 0x3af00c7a, /// 0x270
			 0xee5edcfe, /// 0x274
			 0x16f51422, /// 0x278
			 0x53e0dbca, /// 0x27c
			 0xd2ef1ddf, /// 0x280
			 0x2c9d7efc, /// 0x284
			 0x7f75a2fa, /// 0x288
			 0x42897e62, /// 0x28c
			 0x06402587, /// 0x290
			 0xee82a469, /// 0x294
			 0x7ea18d56, /// 0x298
			 0xe6139e8d, /// 0x29c
			 0x717051c1, /// 0x2a0
			 0xc2132b95, /// 0x2a4
			 0x77557b05, /// 0x2a8
			 0xdf9879a4, /// 0x2ac
			 0xc7860f02, /// 0x2b0
			 0x2b5845f7, /// 0x2b4
			 0x0a601a4e, /// 0x2b8
			 0xfc1c4ef3, /// 0x2bc
			 0x07de4651, /// 0x2c0
			 0x86a731ed, /// 0x2c4
			 0xe10332c9, /// 0x2c8
			 0xdab0fbfd, /// 0x2cc
			 0xb09fa905, /// 0x2d0
			 0x36ea8460, /// 0x2d4
			 0x62ae6a51, /// 0x2d8
			 0xa76e1950, /// 0x2dc
			 0x2be1357a, /// 0x2e0
			 0xabe386f2, /// 0x2e4
			 0x381ef798, /// 0x2e8
			 0x6b81523d, /// 0x2ec
			 0xfdf5dcd6, /// 0x2f0
			 0x15f07c1c, /// 0x2f4
			 0xc95ce5c6, /// 0x2f8
			 0xf683b236, /// 0x2fc
			 0x837c0975, /// 0x300
			 0x5be47e47, /// 0x304
			 0x3466008b, /// 0x308
			 0x57c347c8, /// 0x30c
			 0xfeb800cf, /// 0x310
			 0x1e7dbaa0, /// 0x314
			 0x1c9584b6, /// 0x318
			 0x31d9eae7, /// 0x31c
			 0x34777b10, /// 0x320
			 0x2a278a2e, /// 0x324
			 0x4bb8a8d8, /// 0x328
			 0xc92529f4, /// 0x32c
			 0x85d75a72, /// 0x330
			 0x1786b374, /// 0x334
			 0xd74818fe, /// 0x338
			 0x7702ec18, /// 0x33c
			 0x0f1474ed, /// 0x340
			 0x5e78cd13, /// 0x344
			 0xa7cc5570, /// 0x348
			 0x469fa5fc, /// 0x34c
			 0x92cd2f3f, /// 0x350
			 0xc0e077b8, /// 0x354
			 0xdc762a2f, /// 0x358
			 0xfbf5565d, /// 0x35c
			 0xec920b2e, /// 0x360
			 0xe47cb50d, /// 0x364
			 0x7b0af50c, /// 0x368
			 0xcdb9b955, /// 0x36c
			 0xc21f01a1, /// 0x370
			 0x9684360d, /// 0x374
			 0xb9d1eef1, /// 0x378
			 0xc6bab57b, /// 0x37c
			 0xe5bc145c, /// 0x380
			 0xd6a4fab1, /// 0x384
			 0x6c7d2446, /// 0x388
			 0xabf4d954, /// 0x38c
			 0x7edb3051, /// 0x390
			 0x0265b116, /// 0x394
			 0x64659613, /// 0x398
			 0x246818a6, /// 0x39c
			 0xd9184b1f, /// 0x3a0
			 0x7482742d, /// 0x3a4
			 0xebc5bd0f, /// 0x3a8
			 0x6dac084a, /// 0x3ac
			 0x4494af43, /// 0x3b0
			 0x82ceeb25, /// 0x3b4
			 0x1a894d13, /// 0x3b8
			 0xda818811, /// 0x3bc
			 0xa5f7c459, /// 0x3c0
			 0x965732dc, /// 0x3c4
			 0xa4f949ed, /// 0x3c8
			 0xcab61a29, /// 0x3cc
			 0xcc545751, /// 0x3d0
			 0xfc96c5f3, /// 0x3d4
			 0x63c540bf, /// 0x3d8
			 0xad045d99, /// 0x3dc
			 0x5b2455d7, /// 0x3e0
			 0xaab520b2, /// 0x3e4
			 0x6fea7a2b, /// 0x3e8
			 0x2f0f0c73, /// 0x3ec
			 0x87e1ebef, /// 0x3f0
			 0x420875d4, /// 0x3f4
			 0xc3c7d82c, /// 0x3f8
			 0xc2663756, /// 0x3fc
			 0x52a940c4, /// 0x400
			 0x5e51657c, /// 0x404
			 0x3b9aa575, /// 0x408
			 0xb70f5baf, /// 0x40c
			 0x23f09f8e, /// 0x410
			 0x8a141525, /// 0x414
			 0xf1891a9b, /// 0x418
			 0xde79783d, /// 0x41c
			 0xd8cf1587, /// 0x420
			 0x5cd62ba6, /// 0x424
			 0x81aebd9d, /// 0x428
			 0x719f2d95, /// 0x42c
			 0x0c0c1fe1, /// 0x430
			 0xa78d18cf, /// 0x434
			 0x5a845ffe, /// 0x438
			 0xd6842e01, /// 0x43c
			 0xb6b651d0, /// 0x440
			 0x9e54d637, /// 0x444
			 0xda01f5c7, /// 0x448
			 0xf83c421b, /// 0x44c
			 0xdef4d4b9, /// 0x450
			 0xefaa154b, /// 0x454
			 0x1aff0942, /// 0x458
			 0xa341c651, /// 0x45c
			 0x0348111e, /// 0x460
			 0xc6fd2958, /// 0x464
			 0xd97bef11, /// 0x468
			 0xe4b517dc, /// 0x46c
			 0xcbe5594e, /// 0x470
			 0x0d2479fa, /// 0x474
			 0x8ecfdac5, /// 0x478
			 0x33a4b33a, /// 0x47c
			 0x58257510, /// 0x480
			 0x327df247, /// 0x484
			 0x057de547, /// 0x488
			 0xdcc86d15, /// 0x48c
			 0xc55b42be, /// 0x490
			 0x05b08c5c, /// 0x494
			 0x0a32c53f, /// 0x498
			 0x0809fef4, /// 0x49c
			 0x8bae5130, /// 0x4a0
			 0xdd6c1944, /// 0x4a4
			 0xd4f85ec2, /// 0x4a8
			 0xab387d2e, /// 0x4ac
			 0x81158a10, /// 0x4b0
			 0x28ec6729, /// 0x4b4
			 0x7bd5d8ee, /// 0x4b8
			 0x06b00efe, /// 0x4bc
			 0x1b094474, /// 0x4c0
			 0x40343a9f, /// 0x4c4
			 0x8fba41e7, /// 0x4c8
			 0xd6dd7a61, /// 0x4cc
			 0x5ba3ebb0, /// 0x4d0
			 0xfe1f0f2c, /// 0x4d4
			 0x10c5db51, /// 0x4d8
			 0x10745fc5, /// 0x4dc
			 0x94959e07, /// 0x4e0
			 0xdc905ff5, /// 0x4e4
			 0xed06a825, /// 0x4e8
			 0x5e56b2f8, /// 0x4ec
			 0xe80b0101, /// 0x4f0
			 0x062d37ff, /// 0x4f4
			 0x6c0cbaaf, /// 0x4f8
			 0xe77ab4b1, /// 0x4fc
			 0x519ec10b, /// 0x500
			 0xcca2e005, /// 0x504
			 0xd0ef2f6d, /// 0x508
			 0xcfc450a2, /// 0x50c
			 0x33fb56bc, /// 0x510
			 0x9f290aed, /// 0x514
			 0xdcb98a63, /// 0x518
			 0x382cc8f0, /// 0x51c
			 0xf53df1a3, /// 0x520
			 0x3c27bb37, /// 0x524
			 0x735c2e9b, /// 0x528
			 0x63f28f9b, /// 0x52c
			 0x289b32b6, /// 0x530
			 0xc0815ec8, /// 0x534
			 0x8add2a44, /// 0x538
			 0x82bc127a, /// 0x53c
			 0xeb6bf643, /// 0x540
			 0x53f8f70e, /// 0x544
			 0x436e3add, /// 0x548
			 0xe51569e4, /// 0x54c
			 0xd5a1bbf4, /// 0x550
			 0x780c299f, /// 0x554
			 0x5ab86cf3, /// 0x558
			 0x00adc98d, /// 0x55c
			 0xf808fd3b, /// 0x560
			 0xbbbebb76, /// 0x564
			 0x6f3a42de, /// 0x568
			 0xbb7aee98, /// 0x56c
			 0x397ddc24, /// 0x570
			 0x04415810, /// 0x574
			 0x355e0dc0, /// 0x578
			 0x578cabcc, /// 0x57c
			 0x40d9193f, /// 0x580
			 0x55b8e943, /// 0x584
			 0x9cb67aad, /// 0x588
			 0x03597cde, /// 0x58c
			 0x26bb0e14, /// 0x590
			 0x726dd9ff, /// 0x594
			 0xf652aaa9, /// 0x598
			 0xae5ad645, /// 0x59c
			 0x2ee7edc0, /// 0x5a0
			 0x4b172b34, /// 0x5a4
			 0x0d07ff96, /// 0x5a8
			 0x7d9d8467, /// 0x5ac
			 0xd1458c81, /// 0x5b0
			 0xdb446f23, /// 0x5b4
			 0x3d244cc2, /// 0x5b8
			 0x7bbf2d5e, /// 0x5bc
			 0xadb3a55f, /// 0x5c0
			 0x5c8f193c, /// 0x5c4
			 0x266c6bec, /// 0x5c8
			 0xdd0bafa5, /// 0x5cc
			 0x0ab7bb3d, /// 0x5d0
			 0xb29ec915, /// 0x5d4
			 0xf57e26b9, /// 0x5d8
			 0x4291a7ff, /// 0x5dc
			 0x1b1224f7, /// 0x5e0
			 0x290b1795, /// 0x5e4
			 0x957f75b1, /// 0x5e8
			 0x14e09518, /// 0x5ec
			 0xa1036d61, /// 0x5f0
			 0x3c904309, /// 0x5f4
			 0x47242246, /// 0x5f8
			 0x7bb8ff77, /// 0x5fc
			 0x3b4e74cd, /// 0x600
			 0x6fa18ddb, /// 0x604
			 0x53ed4b19, /// 0x608
			 0x52e85fe2, /// 0x60c
			 0x8bc6e50f, /// 0x610
			 0x0263f886, /// 0x614
			 0xda903b9d, /// 0x618
			 0x80ad8b71, /// 0x61c
			 0xb7f1333a, /// 0x620
			 0x2adf4089, /// 0x624
			 0x74bff9db, /// 0x628
			 0x1dc0fb52, /// 0x62c
			 0x8a63bd4f, /// 0x630
			 0x181e8436, /// 0x634
			 0xae4236ff, /// 0x638
			 0xae4399cc, /// 0x63c
			 0x4e3f4a81, /// 0x640
			 0xa45b4fd3, /// 0x644
			 0x3e991191, /// 0x648
			 0x8d113244, /// 0x64c
			 0x1494a1f2, /// 0x650
			 0x8d6e1977, /// 0x654
			 0x66ee26a5, /// 0x658
			 0x7ab6c28e, /// 0x65c
			 0xded880ad, /// 0x660
			 0x117197dc, /// 0x664
			 0xba675821, /// 0x668
			 0x40de0981, /// 0x66c
			 0x39a5715f, /// 0x670
			 0x98efff19, /// 0x674
			 0xd5de99db, /// 0x678
			 0x1d7cdbc5, /// 0x67c
			 0xba721c25, /// 0x680
			 0x1b2ae74b, /// 0x684
			 0xb96c46ac, /// 0x688
			 0x7e4a8a8f, /// 0x68c
			 0xb523c042, /// 0x690
			 0x1583cc3f, /// 0x694
			 0x642ddd8a, /// 0x698
			 0xa8c647e4, /// 0x69c
			 0x0e2b49dc, /// 0x6a0
			 0xff81c5e5, /// 0x6a4
			 0x226e6ae9, /// 0x6a8
			 0xff50489c, /// 0x6ac
			 0x74116ed2, /// 0x6b0
			 0xd8e444f2, /// 0x6b4
			 0x4a1ca6b3, /// 0x6b8
			 0xea00b9b0, /// 0x6bc
			 0xc235a1b1, /// 0x6c0
			 0xc2377b35, /// 0x6c4
			 0x6e6c75bf, /// 0x6c8
			 0x53a103d4, /// 0x6cc
			 0xbe18e967, /// 0x6d0
			 0x51bc1cfd, /// 0x6d4
			 0xe67f506b, /// 0x6d8
			 0x9d896624, /// 0x6dc
			 0x756404ff, /// 0x6e0
			 0xfcb1a458, /// 0x6e4
			 0xec8238a2, /// 0x6e8
			 0x46e8b213, /// 0x6ec
			 0xbb767533, /// 0x6f0
			 0x6644ef68, /// 0x6f4
			 0x6fa3c443, /// 0x6f8
			 0xd1ec58d6, /// 0x6fc
			 0x7067dd79, /// 0x700
			 0xd6131a28, /// 0x704
			 0x5080e015, /// 0x708
			 0x0bc09d21, /// 0x70c
			 0x2acc04c6, /// 0x710
			 0x81e41ca3, /// 0x714
			 0x5b769830, /// 0x718
			 0xd680c004, /// 0x71c
			 0xe8f8e6f4, /// 0x720
			 0x2fa5e6e3, /// 0x724
			 0x61b180e2, /// 0x728
			 0x4361faa1, /// 0x72c
			 0xc75c3e4e, /// 0x730
			 0x2b80187f, /// 0x734
			 0xc8e210d3, /// 0x738
			 0xb604c56d, /// 0x73c
			 0x2e7173ce, /// 0x740
			 0x1462f10d, /// 0x744
			 0x24474cab, /// 0x748
			 0x862829b9, /// 0x74c
			 0xf1a13ecf, /// 0x750
			 0x504bf45a, /// 0x754
			 0xf4ca8343, /// 0x758
			 0xe251a691, /// 0x75c
			 0x23933232, /// 0x760
			 0x61c3d3d8, /// 0x764
			 0xd44df875, /// 0x768
			 0xb9d64090, /// 0x76c
			 0x2abaeb4e, /// 0x770
			 0x26773729, /// 0x774
			 0xdcc5ec93, /// 0x778
			 0x32123627, /// 0x77c
			 0xf621c622, /// 0x780
			 0xbe18a02f, /// 0x784
			 0x8981f320, /// 0x788
			 0x1c29cc72, /// 0x78c
			 0xc9486ef8, /// 0x790
			 0x16acc230, /// 0x794
			 0x06ef9094, /// 0x798
			 0xf77a2e62, /// 0x79c
			 0xa95ab14c, /// 0x7a0
			 0xe153c83f, /// 0x7a4
			 0xe6933cd7, /// 0x7a8
			 0xf732d4fd, /// 0x7ac
			 0x97b68eb3, /// 0x7b0
			 0x35a1f9ea, /// 0x7b4
			 0xbb2a66c6, /// 0x7b8
			 0x84ad6331, /// 0x7bc
			 0xa6924d83, /// 0x7c0
			 0x83ca3a0e, /// 0x7c4
			 0x59332bae, /// 0x7c8
			 0x8b1259d3, /// 0x7cc
			 0xe56e41a6, /// 0x7d0
			 0x6d414de9, /// 0x7d4
			 0x45924dd5, /// 0x7d8
			 0xc8db3dee, /// 0x7dc
			 0x1a108112, /// 0x7e0
			 0x643b677d, /// 0x7e4
			 0x56d1bee9, /// 0x7e8
			 0xdaed09ae, /// 0x7ec
			 0xa09319f6, /// 0x7f0
			 0x5c5a23ed, /// 0x7f4
			 0x410a956c, /// 0x7f8
			 0x9fb877b8, /// 0x7fc
			 0xede65bf9, /// 0x800
			 0xfc58ce66, /// 0x804
			 0xb1a26162, /// 0x808
			 0xddb222ca, /// 0x80c
			 0xc295393b, /// 0x810
			 0xdf9cc546, /// 0x814
			 0xa0f31e32, /// 0x818
			 0xb517541e, /// 0x81c
			 0x2e0a2a67, /// 0x820
			 0xcd130cde, /// 0x824
			 0x1936da26, /// 0x828
			 0x943d5b0b, /// 0x82c
			 0x8384e36e, /// 0x830
			 0xf8f8d85d, /// 0x834
			 0x2a90dd49, /// 0x838
			 0x7523dc50, /// 0x83c
			 0x51676e11, /// 0x840
			 0xe7161b0b, /// 0x844
			 0x96c1fcb1, /// 0x848
			 0x72304df1, /// 0x84c
			 0x04ca1e7d, /// 0x850
			 0x109c9ea7, /// 0x854
			 0xaccc7585, /// 0x858
			 0x3331c991, /// 0x85c
			 0x6446e6de, /// 0x860
			 0xeabb916d, /// 0x864
			 0x4215f369, /// 0x868
			 0x65d42534, /// 0x86c
			 0xba21facf, /// 0x870
			 0x7bd9b1a6, /// 0x874
			 0x37a95f47, /// 0x878
			 0xb6112754, /// 0x87c
			 0x4b8b74d7, /// 0x880
			 0x19279689, /// 0x884
			 0x0d11c93e, /// 0x888
			 0xcf8c4158, /// 0x88c
			 0xea1122ff, /// 0x890
			 0x1e05477c, /// 0x894
			 0xc45d79cb, /// 0x898
			 0x7530d8f5, /// 0x89c
			 0x9b9b0786, /// 0x8a0
			 0x50a05ea1, /// 0x8a4
			 0xccaed13d, /// 0x8a8
			 0xc60b0904, /// 0x8ac
			 0x2b5a4b4d, /// 0x8b0
			 0x764e0dbd, /// 0x8b4
			 0x30e3f917, /// 0x8b8
			 0x43411e17, /// 0x8bc
			 0x7520acb9, /// 0x8c0
			 0x2e430c94, /// 0x8c4
			 0xfb90e381, /// 0x8c8
			 0x3258c8cf, /// 0x8cc
			 0x15d7075c, /// 0x8d0
			 0x9c094101, /// 0x8d4
			 0x1327d20d, /// 0x8d8
			 0x10191723, /// 0x8dc
			 0x853b4bd0, /// 0x8e0
			 0x898d7236, /// 0x8e4
			 0xc3e99c9a, /// 0x8e8
			 0xe7daeb18, /// 0x8ec
			 0x6e1a9c1d, /// 0x8f0
			 0xc7614868, /// 0x8f4
			 0xac4bb11a, /// 0x8f8
			 0xd83b78f2, /// 0x8fc
			 0xe90b29ea, /// 0x900
			 0xb9dea0d5, /// 0x904
			 0xab7ae9fc, /// 0x908
			 0x58f5fec6, /// 0x90c
			 0x4cd86b78, /// 0x910
			 0x4b68067d, /// 0x914
			 0x0d443b9b, /// 0x918
			 0xf4670698, /// 0x91c
			 0xaf978a2a, /// 0x920
			 0x2ef8f912, /// 0x924
			 0x6d4b7442, /// 0x928
			 0x77afd1a0, /// 0x92c
			 0xf7494aff, /// 0x930
			 0x6853bafc, /// 0x934
			 0xdf4dbb7c, /// 0x938
			 0x4ec59618, /// 0x93c
			 0xda0f69cb, /// 0x940
			 0x962a7b16, /// 0x944
			 0x4c405a00, /// 0x948
			 0x770f91b5, /// 0x94c
			 0xde2dd9cc, /// 0x950
			 0x70beb556, /// 0x954
			 0xc051bb9d, /// 0x958
			 0xfb1f8af2, /// 0x95c
			 0x7bced422, /// 0x960
			 0x561ac4ea, /// 0x964
			 0x977bf11e, /// 0x968
			 0x29e6cd8a, /// 0x96c
			 0x1b4e9c5e, /// 0x970
			 0xf1863bbd, /// 0x974
			 0x6dbe6eb5, /// 0x978
			 0x61b51d5a, /// 0x97c
			 0xf905c186, /// 0x980
			 0x7f7b0648, /// 0x984
			 0x421cc265, /// 0x988
			 0x16e72b73, /// 0x98c
			 0xb8add9cd, /// 0x990
			 0xbbb4729c, /// 0x994
			 0x229441d7, /// 0x998
			 0x14f683d6, /// 0x99c
			 0x23dc781c, /// 0x9a0
			 0xb6d1534b, /// 0x9a4
			 0x57870f4f, /// 0x9a8
			 0xd1bd88e0, /// 0x9ac
			 0x1f16487e, /// 0x9b0
			 0x44684efb, /// 0x9b4
			 0x41bb9417, /// 0x9b8
			 0x9e12c6e0, /// 0x9bc
			 0xde4e0105, /// 0x9c0
			 0x864e8701, /// 0x9c4
			 0x04248868, /// 0x9c8
			 0x5b51fc19, /// 0x9cc
			 0xceb1c6ec, /// 0x9d0
			 0xd7281919, /// 0x9d4
			 0xe81e77fe, /// 0x9d8
			 0xc9344e34, /// 0x9dc
			 0x01d76a7f, /// 0x9e0
			 0xeeb3d1a3, /// 0x9e4
			 0x965a69c1, /// 0x9e8
			 0xbae45e01, /// 0x9ec
			 0x3ee88ca1, /// 0x9f0
			 0x26a6b484, /// 0x9f4
			 0x0c33e582, /// 0x9f8
			 0xf7c55d21, /// 0x9fc
			 0x68b1be11, /// 0xa00
			 0xcd71fd58, /// 0xa04
			 0x82e8193b, /// 0xa08
			 0xbec20b50, /// 0xa0c
			 0x75c5b80c, /// 0xa10
			 0x842049ec, /// 0xa14
			 0x496c8058, /// 0xa18
			 0xd51560be, /// 0xa1c
			 0x88fee5df, /// 0xa20
			 0x27b1942f, /// 0xa24
			 0x77e0a608, /// 0xa28
			 0xf96df9e0, /// 0xa2c
			 0xb39fb568, /// 0xa30
			 0xfe3269af, /// 0xa34
			 0x2c17334f, /// 0xa38
			 0x531cd1eb, /// 0xa3c
			 0xfbe03a6d, /// 0xa40
			 0xcdaba9c7, /// 0xa44
			 0x102ebcb2, /// 0xa48
			 0xbccf769d, /// 0xa4c
			 0xb51f37cf, /// 0xa50
			 0x5a02c664, /// 0xa54
			 0x47ab6330, /// 0xa58
			 0x83826507, /// 0xa5c
			 0xe6ac98ad, /// 0xa60
			 0x088a3ce6, /// 0xa64
			 0xfc9a81a2, /// 0xa68
			 0x416c5460, /// 0xa6c
			 0x06a7d7a4, /// 0xa70
			 0xac567e21, /// 0xa74
			 0xc73f9fdb, /// 0xa78
			 0x4982076b, /// 0xa7c
			 0xc92aae85, /// 0xa80
			 0x0a57972b, /// 0xa84
			 0x3922719b, /// 0xa88
			 0x82b8034f, /// 0xa8c
			 0x5cdb46b5, /// 0xa90
			 0x095afe16, /// 0xa94
			 0xdbac37f7, /// 0xa98
			 0x3b7f3c83, /// 0xa9c
			 0xa2d74d7b, /// 0xaa0
			 0x79e5c4cb, /// 0xaa4
			 0x86286155, /// 0xaa8
			 0x2fdaefc6, /// 0xaac
			 0x50f2ccbd, /// 0xab0
			 0xf4b0a2c8, /// 0xab4
			 0x129498fa, /// 0xab8
			 0x5d8d6131, /// 0xabc
			 0xb9820e91, /// 0xac0
			 0x773b8eab, /// 0xac4
			 0x3c23184a, /// 0xac8
			 0xe9b1a7f3, /// 0xacc
			 0x96fb559f, /// 0xad0
			 0x40847c55, /// 0xad4
			 0x37de76cb, /// 0xad8
			 0x0f6cef52, /// 0xadc
			 0x91eac869, /// 0xae0
			 0x051cdbb4, /// 0xae4
			 0x06e74832, /// 0xae8
			 0x19c7b00b, /// 0xaec
			 0xd8255971, /// 0xaf0
			 0xf89ae04f, /// 0xaf4
			 0xf40bf48a, /// 0xaf8
			 0x3482a25e, /// 0xafc
			 0x3a2c21d9, /// 0xb00
			 0x12e6e20e, /// 0xb04
			 0xe837a7b8, /// 0xb08
			 0x3dcdbe92, /// 0xb0c
			 0xf90c3082, /// 0xb10
			 0x2b7f45dd, /// 0xb14
			 0x323f8a84, /// 0xb18
			 0x07f160c9, /// 0xb1c
			 0xb9401c0b, /// 0xb20
			 0xa5b793ad, /// 0xb24
			 0x72cc3573, /// 0xb28
			 0x845c1c12, /// 0xb2c
			 0x1f395d3f, /// 0xb30
			 0x40f69aac, /// 0xb34
			 0xd8bd0975, /// 0xb38
			 0x62f72f5a, /// 0xb3c
			 0xc2d10726, /// 0xb40
			 0x555910c0, /// 0xb44
			 0x153eafee, /// 0xb48
			 0xbab70243, /// 0xb4c
			 0x9f93b722, /// 0xb50
			 0x7c9fba69, /// 0xb54
			 0x7b53977c, /// 0xb58
			 0xd363d989, /// 0xb5c
			 0x9c18d7ee, /// 0xb60
			 0xd9b8204a, /// 0xb64
			 0xd51d8553, /// 0xb68
			 0x3004cfbd, /// 0xb6c
			 0xf328b908, /// 0xb70
			 0xc18c1269, /// 0xb74
			 0x228b659f, /// 0xb78
			 0x0d7a4e5f, /// 0xb7c
			 0xf5bd2eb9, /// 0xb80
			 0x9d6de38f, /// 0xb84
			 0x028b42e4, /// 0xb88
			 0xfba04b80, /// 0xb8c
			 0x028afbdf, /// 0xb90
			 0x8e110b92, /// 0xb94
			 0xb04eafd8, /// 0xb98
			 0x4522b926, /// 0xb9c
			 0x64d7fbcf, /// 0xba0
			 0x773a6e76, /// 0xba4
			 0x651b1867, /// 0xba8
			 0x2d9c4109, /// 0xbac
			 0xf24338e9, /// 0xbb0
			 0x719d39f1, /// 0xbb4
			 0x7c8939f8, /// 0xbb8
			 0x4c60dc46, /// 0xbbc
			 0xec8dd9ff, /// 0xbc0
			 0x19f1807c, /// 0xbc4
			 0xd46c27e8, /// 0xbc8
			 0x7054f956, /// 0xbcc
			 0x7e0f90f4, /// 0xbd0
			 0xab65c15a, /// 0xbd4
			 0x445f7c49, /// 0xbd8
			 0x813f4268, /// 0xbdc
			 0x6536e3c3, /// 0xbe0
			 0x808d43c5, /// 0xbe4
			 0x82a4563e, /// 0xbe8
			 0x8d542bca, /// 0xbec
			 0x03024486, /// 0xbf0
			 0xa30d518b, /// 0xbf4
			 0x019f115e, /// 0xbf8
			 0xdf1b1dd8, /// 0xbfc
			 0x44473a6a, /// 0xc00
			 0x377d47c2, /// 0xc04
			 0xe690c80c, /// 0xc08
			 0xa120dd79, /// 0xc0c
			 0xe3a91eac, /// 0xc10
			 0x107e2052, /// 0xc14
			 0x660e852a, /// 0xc18
			 0xd9344782, /// 0xc1c
			 0xe475f85e, /// 0xc20
			 0xd2c2cb9c, /// 0xc24
			 0x91240594, /// 0xc28
			 0xc42839bd, /// 0xc2c
			 0xec40bf1f, /// 0xc30
			 0xcbfdd10a, /// 0xc34
			 0xac8733fe, /// 0xc38
			 0x3750debc, /// 0xc3c
			 0x9d45c7fd, /// 0xc40
			 0xbddd927f, /// 0xc44
			 0xec2f69b4, /// 0xc48
			 0xa7a463a0, /// 0xc4c
			 0xd520b9e5, /// 0xc50
			 0xfaa9dd8f, /// 0xc54
			 0xef876176, /// 0xc58
			 0x39169c24, /// 0xc5c
			 0x0263da21, /// 0xc60
			 0x4f0a608c, /// 0xc64
			 0xba76f71c, /// 0xc68
			 0x4bd80d52, /// 0xc6c
			 0x06205aac, /// 0xc70
			 0xa187375c, /// 0xc74
			 0xb52178c1, /// 0xc78
			 0x90446cd0, /// 0xc7c
			 0xbde6ae87, /// 0xc80
			 0xac5470b4, /// 0xc84
			 0x9f54b40f, /// 0xc88
			 0xef4186e3, /// 0xc8c
			 0xe42dd937, /// 0xc90
			 0x2b96a193, /// 0xc94
			 0x07151d72, /// 0xc98
			 0x02181372, /// 0xc9c
			 0xce516f61, /// 0xca0
			 0xea6a5471, /// 0xca4
			 0x08f9532b, /// 0xca8
			 0x8068295e, /// 0xcac
			 0xd2679979, /// 0xcb0
			 0x2c6afcbd, /// 0xcb4
			 0x8d458941, /// 0xcb8
			 0x63ac703e, /// 0xcbc
			 0x6d0009a9, /// 0xcc0
			 0x21939351, /// 0xcc4
			 0x3bbcf2de, /// 0xcc8
			 0x08bde311, /// 0xccc
			 0x76ae3f18, /// 0xcd0
			 0xe98524c6, /// 0xcd4
			 0x4c587254, /// 0xcd8
			 0x3a721c9b, /// 0xcdc
			 0xbce67a99, /// 0xce0
			 0x875930b4, /// 0xce4
			 0xf51ac6a6, /// 0xce8
			 0x944a03af, /// 0xcec
			 0x2b7e1770, /// 0xcf0
			 0x16ff18b5, /// 0xcf4
			 0x7cc8556c, /// 0xcf8
			 0x8d634c96, /// 0xcfc
			 0x85cbff32, /// 0xd00
			 0x0d756165, /// 0xd04
			 0xde4249f9, /// 0xd08
			 0xb7f0473a, /// 0xd0c
			 0xd7be5800, /// 0xd10
			 0x150c4363, /// 0xd14
			 0x81b48afe, /// 0xd18
			 0x55548125, /// 0xd1c
			 0xbf97898b, /// 0xd20
			 0x8dd05ad8, /// 0xd24
			 0x29e71d77, /// 0xd28
			 0xd20ad4e8, /// 0xd2c
			 0x39f3e316, /// 0xd30
			 0xe430bcfc, /// 0xd34
			 0x57f2af79, /// 0xd38
			 0x2a5867b6, /// 0xd3c
			 0xfe60806d, /// 0xd40
			 0x05fb4221, /// 0xd44
			 0x4ee38486, /// 0xd48
			 0xc64cafd4, /// 0xd4c
			 0x52f6e0d3, /// 0xd50
			 0x9509fd94, /// 0xd54
			 0x36c80f44, /// 0xd58
			 0x2eaa2dfe, /// 0xd5c
			 0x1433ac39, /// 0xd60
			 0x7fbaff70, /// 0xd64
			 0x6d9bd2e0, /// 0xd68
			 0x97e245e0, /// 0xd6c
			 0x70a320ad, /// 0xd70
			 0x5d95811e, /// 0xd74
			 0x84ddc769, /// 0xd78
			 0x1f24cda0, /// 0xd7c
			 0xcff5cdf8, /// 0xd80
			 0x0d83a880, /// 0xd84
			 0xda5684bd, /// 0xd88
			 0xc3e144f6, /// 0xd8c
			 0x12f0d30a, /// 0xd90
			 0xe0f693fb, /// 0xd94
			 0x0d1eb4dd, /// 0xd98
			 0xe512ce75, /// 0xd9c
			 0x454d2130, /// 0xda0
			 0xae72c05c, /// 0xda4
			 0x99b96cd2, /// 0xda8
			 0x65b6a8f3, /// 0xdac
			 0x604d9bdf, /// 0xdb0
			 0x9d755689, /// 0xdb4
			 0x5d313583, /// 0xdb8
			 0x2faaee0e, /// 0xdbc
			 0x49aadd8d, /// 0xdc0
			 0x18100192, /// 0xdc4
			 0x302158c3, /// 0xdc8
			 0x34455252, /// 0xdcc
			 0x983831a8, /// 0xdd0
			 0xb4678b00, /// 0xdd4
			 0x884c8907, /// 0xdd8
			 0x23cb4972, /// 0xddc
			 0x2d16e27f, /// 0xde0
			 0xbe4bb727, /// 0xde4
			 0x68010a0f, /// 0xde8
			 0x0444eeeb, /// 0xdec
			 0x74ff9c54, /// 0xdf0
			 0xe3a492d0, /// 0xdf4
			 0x487a2171, /// 0xdf8
			 0xaa7d8d4a, /// 0xdfc
			 0x1a6a4aa2, /// 0xe00
			 0xe7647325, /// 0xe04
			 0x84dcd732, /// 0xe08
			 0xcd56d556, /// 0xe0c
			 0x6e834353, /// 0xe10
			 0xae3fd7d4, /// 0xe14
			 0x96641cc7, /// 0xe18
			 0xa41924da, /// 0xe1c
			 0xf495e148, /// 0xe20
			 0x54d9b245, /// 0xe24
			 0x3bf73d21, /// 0xe28
			 0xc0ab7612, /// 0xe2c
			 0x561fff03, /// 0xe30
			 0x2c9429fd, /// 0xe34
			 0xbfc92fe3, /// 0xe38
			 0x02fcc91a, /// 0xe3c
			 0xfd0faca8, /// 0xe40
			 0xef8a4c4f, /// 0xe44
			 0x2e56b58b, /// 0xe48
			 0xfbc2fb6a, /// 0xe4c
			 0x7796db8b, /// 0xe50
			 0xa6191686, /// 0xe54
			 0xaf566c84, /// 0xe58
			 0x4f86ecbc, /// 0xe5c
			 0xc05eecc1, /// 0xe60
			 0x7b13fd45, /// 0xe64
			 0x17f4270a, /// 0xe68
			 0xaa635617, /// 0xe6c
			 0x9a368c74, /// 0xe70
			 0x6eea30ff, /// 0xe74
			 0xef90dfdf, /// 0xe78
			 0xc666a4c2, /// 0xe7c
			 0x70ff2e0f, /// 0xe80
			 0x0aa50a57, /// 0xe84
			 0x3ddd8b66, /// 0xe88
			 0xb9f93441, /// 0xe8c
			 0x849b3884, /// 0xe90
			 0x78f61b59, /// 0xe94
			 0x0d197627, /// 0xe98
			 0x0fb2b43b, /// 0xe9c
			 0x47ce0f17, /// 0xea0
			 0xc10bb1a9, /// 0xea4
			 0xdc143836, /// 0xea8
			 0x3031fbd2, /// 0xeac
			 0xbbfb275a, /// 0xeb0
			 0xde1bcbd0, /// 0xeb4
			 0xda55c98a, /// 0xeb8
			 0x808cbdca, /// 0xebc
			 0xec501f36, /// 0xec0
			 0xdd30016e, /// 0xec4
			 0x42536b7b, /// 0xec8
			 0xe5bc3596, /// 0xecc
			 0x1f898d57, /// 0xed0
			 0xf6bde2c7, /// 0xed4
			 0xfc56eb99, /// 0xed8
			 0x72fb5caa, /// 0xedc
			 0x2e5baa7e, /// 0xee0
			 0x5a9adaa0, /// 0xee4
			 0x8f5bad53, /// 0xee8
			 0x2136ae94, /// 0xeec
			 0x187e469b, /// 0xef0
			 0xdae81a1d, /// 0xef4
			 0xe7b83883, /// 0xef8
			 0x1074f8b4, /// 0xefc
			 0x6e7be623, /// 0xf00
			 0xae65b2a7, /// 0xf04
			 0x5348a65d, /// 0xf08
			 0x5b215c71, /// 0xf0c
			 0x73bc382d, /// 0xf10
			 0xf0a77e4a, /// 0xf14
			 0x874ba3e2, /// 0xf18
			 0x3b07f7f9, /// 0xf1c
			 0x92225139, /// 0xf20
			 0xfa260f27, /// 0xf24
			 0x458c0af4, /// 0xf28
			 0x895421de, /// 0xf2c
			 0x53084337, /// 0xf30
			 0x1d7f8cf9, /// 0xf34
			 0x601bb064, /// 0xf38
			 0x850c2091, /// 0xf3c
			 0xc4291be7, /// 0xf40
			 0xb6e5ecf6, /// 0xf44
			 0x911e0a9b, /// 0xf48
			 0xedaf356f, /// 0xf4c
			 0x6e1b3d26, /// 0xf50
			 0x565fbed6, /// 0xf54
			 0x8c044c30, /// 0xf58
			 0x0ef51e55, /// 0xf5c
			 0x4a0c4932, /// 0xf60
			 0x0fc13780, /// 0xf64
			 0x9d345f1a, /// 0xf68
			 0x08eaeba4, /// 0xf6c
			 0x0dc68693, /// 0xf70
			 0xbe7ff1c0, /// 0xf74
			 0xf5ffc612, /// 0xf78
			 0xac51e8c4, /// 0xf7c
			 0xf98eb927, /// 0xf80
			 0x4f9dc861, /// 0xf84
			 0x20929540, /// 0xf88
			 0x0287e180, /// 0xf8c
			 0xf0f56b39, /// 0xf90
			 0xab33bfd2, /// 0xf94
			 0xa8613744, /// 0xf98
			 0x6918a64d, /// 0xf9c
			 0x2ad71ff7, /// 0xfa0
			 0x47d3150b, /// 0xfa4
			 0x59e83ed0, /// 0xfa8
			 0xb9518c12, /// 0xfac
			 0x8520ec31, /// 0xfb0
			 0x32004b74, /// 0xfb4
			 0xcdf24d20, /// 0xfb8
			 0x3e59f31d, /// 0xfbc
			 0x6b34ae2f, /// 0xfc0
			 0xd2ed0fae, /// 0xfc4
			 0xe669a0e7, /// 0xfc8
			 0x16408604, /// 0xfcc
			 0x255bddd7, /// 0xfd0
			 0xa27b2412, /// 0xfd4
			 0x2771ce47, /// 0xfd8
			 0xe5c5c1fc, /// 0xfdc
			 0x541266c1, /// 0xfe0
			 0xf78c10a1, /// 0xfe4
			 0xf014754c, /// 0xfe8
			 0xcc9c2bf3, /// 0xfec
			 0xdb8906a1, /// 0xff0
			 0xdaa34dec, /// 0xff4
			 0x20dce13b, /// 0xff8
			 0x4dcf00b0 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x06e9733f, /// 0x0
			 0xc999bfc0, /// 0x4
			 0x6084b476, /// 0x8
			 0x9bf88ee6, /// 0xc
			 0x65f456cb, /// 0x10
			 0x4d5eeb90, /// 0x14
			 0xc807907a, /// 0x18
			 0xe824ab6d, /// 0x1c
			 0x9a4a82ea, /// 0x20
			 0xbfffec2e, /// 0x24
			 0x9203d71c, /// 0x28
			 0xda475fa7, /// 0x2c
			 0x581fe2b9, /// 0x30
			 0x1c08bffd, /// 0x34
			 0x52223bf1, /// 0x38
			 0x7a6c26e2, /// 0x3c
			 0xeca2458b, /// 0x40
			 0x8e9dc4a8, /// 0x44
			 0x5b07a544, /// 0x48
			 0xb6499f9e, /// 0x4c
			 0xfcd00e30, /// 0x50
			 0xeea99f81, /// 0x54
			 0x3c23940f, /// 0x58
			 0x81be24a4, /// 0x5c
			 0x86e2257f, /// 0x60
			 0x6c8dcb41, /// 0x64
			 0x69419749, /// 0x68
			 0x0318a8b6, /// 0x6c
			 0xa45ebe33, /// 0x70
			 0x6d033d95, /// 0x74
			 0xf68eed16, /// 0x78
			 0xf2852825, /// 0x7c
			 0x950556b0, /// 0x80
			 0x7d106478, /// 0x84
			 0x87dc7cee, /// 0x88
			 0xd86fba36, /// 0x8c
			 0xc2b7dcc1, /// 0x90
			 0x0adc32e7, /// 0x94
			 0xcaa30d9f, /// 0x98
			 0x1b889fd8, /// 0x9c
			 0x82720821, /// 0xa0
			 0x184354f8, /// 0xa4
			 0x43db83af, /// 0xa8
			 0x8d12f2d9, /// 0xac
			 0x05995429, /// 0xb0
			 0xf93e029b, /// 0xb4
			 0xba0836fc, /// 0xb8
			 0x90b19414, /// 0xbc
			 0xfa6df4d2, /// 0xc0
			 0x001ff1f5, /// 0xc4
			 0x8bdd18c4, /// 0xc8
			 0x6fc2332f, /// 0xcc
			 0x7a97ae21, /// 0xd0
			 0x7dd1296a, /// 0xd4
			 0xa35f2211, /// 0xd8
			 0xc95ce1f3, /// 0xdc
			 0x338d6df2, /// 0xe0
			 0xb2a1ba2f, /// 0xe4
			 0xfe5cb9d1, /// 0xe8
			 0x4ab6faa4, /// 0xec
			 0xbdef8d0c, /// 0xf0
			 0x4af978d4, /// 0xf4
			 0xda38c73d, /// 0xf8
			 0x79115975, /// 0xfc
			 0x10ef8f93, /// 0x100
			 0x3a98488a, /// 0x104
			 0x3b1e8cb6, /// 0x108
			 0xd3665257, /// 0x10c
			 0x414824be, /// 0x110
			 0x66b4cda2, /// 0x114
			 0xe136bf63, /// 0x118
			 0x7fe95e66, /// 0x11c
			 0xf2bf9d8e, /// 0x120
			 0xf19a6994, /// 0x124
			 0xdb43ae56, /// 0x128
			 0xa11bfece, /// 0x12c
			 0xe2f71ada, /// 0x130
			 0x2b173c9a, /// 0x134
			 0xdade6873, /// 0x138
			 0xf0406045, /// 0x13c
			 0x2e50e506, /// 0x140
			 0x818ee73b, /// 0x144
			 0xe686e9d0, /// 0x148
			 0x7f88e108, /// 0x14c
			 0x836af89a, /// 0x150
			 0x32eba608, /// 0x154
			 0x7c0c12d9, /// 0x158
			 0xe5c92e98, /// 0x15c
			 0xa6503364, /// 0x160
			 0x4ef95add, /// 0x164
			 0x16eea71b, /// 0x168
			 0xeaaa29a3, /// 0x16c
			 0x8a86db83, /// 0x170
			 0x65e7c734, /// 0x174
			 0x9d085385, /// 0x178
			 0x66d28087, /// 0x17c
			 0x7a32d220, /// 0x180
			 0xdf9e97b7, /// 0x184
			 0x598dcfcb, /// 0x188
			 0x52dd4fb7, /// 0x18c
			 0x71dbd8b9, /// 0x190
			 0xb854fe21, /// 0x194
			 0x1f93f755, /// 0x198
			 0xde767eba, /// 0x19c
			 0xa8c3cb7d, /// 0x1a0
			 0xc42c4bc7, /// 0x1a4
			 0x0551328c, /// 0x1a8
			 0x5694fa2b, /// 0x1ac
			 0x51c892f4, /// 0x1b0
			 0x2efc786b, /// 0x1b4
			 0xe6d1cf53, /// 0x1b8
			 0xdf639efe, /// 0x1bc
			 0xc6bb14cd, /// 0x1c0
			 0x66a2ed64, /// 0x1c4
			 0x3e6f870d, /// 0x1c8
			 0x68ef80c6, /// 0x1cc
			 0xe1879013, /// 0x1d0
			 0xfa5325a2, /// 0x1d4
			 0x6bb30132, /// 0x1d8
			 0x3bc5ae54, /// 0x1dc
			 0x4a5c2db4, /// 0x1e0
			 0xd31bfb46, /// 0x1e4
			 0xf80d9acd, /// 0x1e8
			 0xa9eee8aa, /// 0x1ec
			 0xf52be74d, /// 0x1f0
			 0x8442ae00, /// 0x1f4
			 0x6b300ea9, /// 0x1f8
			 0x39b9a84d, /// 0x1fc
			 0xd836ab92, /// 0x200
			 0x69774ef3, /// 0x204
			 0xc755abb1, /// 0x208
			 0xde0f00b5, /// 0x20c
			 0x0901fd0e, /// 0x210
			 0x51341d64, /// 0x214
			 0x83143c36, /// 0x218
			 0x69602c9d, /// 0x21c
			 0x535f4a51, /// 0x220
			 0x237289b8, /// 0x224
			 0x1ed5f122, /// 0x228
			 0xc50768b2, /// 0x22c
			 0x70714e18, /// 0x230
			 0xc85b2a40, /// 0x234
			 0x4c3afc69, /// 0x238
			 0x266f9694, /// 0x23c
			 0x18beeb82, /// 0x240
			 0x1a62badc, /// 0x244
			 0xe067737f, /// 0x248
			 0x08a736e6, /// 0x24c
			 0x36d7bc22, /// 0x250
			 0x436dc39c, /// 0x254
			 0xe48e8e8e, /// 0x258
			 0xb0cad2df, /// 0x25c
			 0x37448c54, /// 0x260
			 0x2fc0d5e9, /// 0x264
			 0x52bc898f, /// 0x268
			 0x3fcf9ec8, /// 0x26c
			 0x648c9e6a, /// 0x270
			 0x2910d7b7, /// 0x274
			 0xa2c0ad01, /// 0x278
			 0x69367d3d, /// 0x27c
			 0x21f11e6b, /// 0x280
			 0x96d54c01, /// 0x284
			 0xc4ffcb66, /// 0x288
			 0x4ff501bc, /// 0x28c
			 0x2dd350e4, /// 0x290
			 0xb7e8c0d5, /// 0x294
			 0x3a5c349e, /// 0x298
			 0x7929860b, /// 0x29c
			 0x99960cf7, /// 0x2a0
			 0x07f2dd6f, /// 0x2a4
			 0x3164e019, /// 0x2a8
			 0xbeef8979, /// 0x2ac
			 0xe2e3f3d2, /// 0x2b0
			 0x765bbdc1, /// 0x2b4
			 0x1b4a5840, /// 0x2b8
			 0x552ba5e2, /// 0x2bc
			 0x2a23fcdf, /// 0x2c0
			 0xa5bba33f, /// 0x2c4
			 0x165301c7, /// 0x2c8
			 0x9529e4ec, /// 0x2cc
			 0x5a0e5f4c, /// 0x2d0
			 0x690fd527, /// 0x2d4
			 0xa0856afa, /// 0x2d8
			 0xfe7bab10, /// 0x2dc
			 0x682fadec, /// 0x2e0
			 0x27093c0b, /// 0x2e4
			 0x10fb636e, /// 0x2e8
			 0x63a6ce6e, /// 0x2ec
			 0x42072731, /// 0x2f0
			 0x9ee0ec03, /// 0x2f4
			 0x4c0d9558, /// 0x2f8
			 0x7bf711d7, /// 0x2fc
			 0x95dd0788, /// 0x300
			 0x71e0b0f6, /// 0x304
			 0x2ac047a4, /// 0x308
			 0x15766de9, /// 0x30c
			 0xcf9f97c7, /// 0x310
			 0x3832f77e, /// 0x314
			 0xbfd728d0, /// 0x318
			 0xd0a88457, /// 0x31c
			 0xa801a562, /// 0x320
			 0xfb2f1708, /// 0x324
			 0x66ccd2e7, /// 0x328
			 0x46dcdb87, /// 0x32c
			 0x0b4568cc, /// 0x330
			 0xdb0c7771, /// 0x334
			 0x702cd0bf, /// 0x338
			 0x70cd5247, /// 0x33c
			 0x14a76473, /// 0x340
			 0x6486c937, /// 0x344
			 0xd1707aaf, /// 0x348
			 0xda5c1fce, /// 0x34c
			 0xf343af53, /// 0x350
			 0x63e2e211, /// 0x354
			 0xfe69403e, /// 0x358
			 0xf0f7e201, /// 0x35c
			 0xe03e6c9e, /// 0x360
			 0xe4b30919, /// 0x364
			 0xe0a90317, /// 0x368
			 0xf59469ac, /// 0x36c
			 0xf737ea33, /// 0x370
			 0x2193de1e, /// 0x374
			 0xf555932e, /// 0x378
			 0x16abf7d5, /// 0x37c
			 0xe23f4fc7, /// 0x380
			 0x6413909c, /// 0x384
			 0xd7bd7dfc, /// 0x388
			 0xad33077f, /// 0x38c
			 0x84785b23, /// 0x390
			 0x4284e0b8, /// 0x394
			 0x605649cc, /// 0x398
			 0x26790ee8, /// 0x39c
			 0x5832e264, /// 0x3a0
			 0xddc87c55, /// 0x3a4
			 0x39b97c85, /// 0x3a8
			 0x46cba511, /// 0x3ac
			 0xa1c74c17, /// 0x3b0
			 0xf1a32d5f, /// 0x3b4
			 0x8f8825c9, /// 0x3b8
			 0x32face2b, /// 0x3bc
			 0x9efcded1, /// 0x3c0
			 0x20578fa6, /// 0x3c4
			 0x48d447f6, /// 0x3c8
			 0xbd1bb937, /// 0x3cc
			 0xf9beb2be, /// 0x3d0
			 0x6c6c648c, /// 0x3d4
			 0xaa732e12, /// 0x3d8
			 0x9f778f8b, /// 0x3dc
			 0x74c0f692, /// 0x3e0
			 0x014c13c0, /// 0x3e4
			 0x8e8a2b02, /// 0x3e8
			 0x91c56515, /// 0x3ec
			 0xb6eb85b1, /// 0x3f0
			 0x2d676e82, /// 0x3f4
			 0x03ee763b, /// 0x3f8
			 0x6ed0317a, /// 0x3fc
			 0x820f2a66, /// 0x400
			 0x15468579, /// 0x404
			 0x139fe638, /// 0x408
			 0xca5272a0, /// 0x40c
			 0x0525b210, /// 0x410
			 0x91de57ca, /// 0x414
			 0x493b6dcc, /// 0x418
			 0x366e3f9b, /// 0x41c
			 0x63e9314d, /// 0x420
			 0x4f77ec23, /// 0x424
			 0x1e862cbe, /// 0x428
			 0xcd194a4e, /// 0x42c
			 0xf24c68e6, /// 0x430
			 0xf2de2f6a, /// 0x434
			 0xd30c9a1d, /// 0x438
			 0x2ce7240b, /// 0x43c
			 0xfde8813f, /// 0x440
			 0x5d6d9008, /// 0x444
			 0x15c11c96, /// 0x448
			 0xe21c9be6, /// 0x44c
			 0x6d1d6761, /// 0x450
			 0xff549042, /// 0x454
			 0x363d4a21, /// 0x458
			 0xdffa4f68, /// 0x45c
			 0x8bbdb662, /// 0x460
			 0x629b1765, /// 0x464
			 0x4fccc00c, /// 0x468
			 0xed696b5a, /// 0x46c
			 0x643ce1e4, /// 0x470
			 0x948730c8, /// 0x474
			 0x08b50537, /// 0x478
			 0x1ac760da, /// 0x47c
			 0xe3e1a2f3, /// 0x480
			 0xf9c160f4, /// 0x484
			 0x621d7ecd, /// 0x488
			 0x2fa3413e, /// 0x48c
			 0xf4e84b7d, /// 0x490
			 0xc1cc170b, /// 0x494
			 0x10f5cc01, /// 0x498
			 0x672cd56a, /// 0x49c
			 0xe09ba687, /// 0x4a0
			 0xfb8ba8ef, /// 0x4a4
			 0x588e2e92, /// 0x4a8
			 0x00ee21fa, /// 0x4ac
			 0x685f7751, /// 0x4b0
			 0x7bd9f683, /// 0x4b4
			 0xa5a4c234, /// 0x4b8
			 0xc70160d0, /// 0x4bc
			 0x6192c701, /// 0x4c0
			 0x2685b012, /// 0x4c4
			 0xbcb3d66c, /// 0x4c8
			 0x5c4c567a, /// 0x4cc
			 0x7abf41bf, /// 0x4d0
			 0x49d4345b, /// 0x4d4
			 0x013f3af8, /// 0x4d8
			 0xb302406b, /// 0x4dc
			 0xfff074c3, /// 0x4e0
			 0x76e4883f, /// 0x4e4
			 0x645e54ec, /// 0x4e8
			 0x3130fcc9, /// 0x4ec
			 0x112a8c10, /// 0x4f0
			 0x93ec9b9f, /// 0x4f4
			 0x81aa1263, /// 0x4f8
			 0xf74c43f3, /// 0x4fc
			 0x68a9067e, /// 0x500
			 0x9207a5e0, /// 0x504
			 0x077e163d, /// 0x508
			 0xd355388e, /// 0x50c
			 0x1671f65f, /// 0x510
			 0x84e3c40d, /// 0x514
			 0x522fffe9, /// 0x518
			 0xc4eff2a6, /// 0x51c
			 0x7f33574f, /// 0x520
			 0x66bbd0d6, /// 0x524
			 0xdde3dff5, /// 0x528
			 0xfc58a406, /// 0x52c
			 0x63e14d77, /// 0x530
			 0x3c214c7b, /// 0x534
			 0x85493b13, /// 0x538
			 0xfef37ef3, /// 0x53c
			 0x4ca9e271, /// 0x540
			 0x50a7f119, /// 0x544
			 0x0235bd3f, /// 0x548
			 0x90e71e95, /// 0x54c
			 0x648f8047, /// 0x550
			 0x3a4fed8c, /// 0x554
			 0x4fb53d93, /// 0x558
			 0xbf0ca446, /// 0x55c
			 0x44ddcb39, /// 0x560
			 0xabd801d7, /// 0x564
			 0x98c23743, /// 0x568
			 0x36637dd5, /// 0x56c
			 0x8e44c028, /// 0x570
			 0x9d8649bc, /// 0x574
			 0x69b7641b, /// 0x578
			 0x8350331b, /// 0x57c
			 0x350df2fb, /// 0x580
			 0xb2419c94, /// 0x584
			 0x70c6951b, /// 0x588
			 0xca7697cb, /// 0x58c
			 0x28d20335, /// 0x590
			 0xd62cda36, /// 0x594
			 0xbf02ad34, /// 0x598
			 0xa4310370, /// 0x59c
			 0xa6aef773, /// 0x5a0
			 0x4c701950, /// 0x5a4
			 0x56c000aa, /// 0x5a8
			 0x67bf0b20, /// 0x5ac
			 0x9bd959fd, /// 0x5b0
			 0x3045ba83, /// 0x5b4
			 0xf916528f, /// 0x5b8
			 0xf4d4dc2c, /// 0x5bc
			 0x6782d32f, /// 0x5c0
			 0x0f7c8624, /// 0x5c4
			 0xd32b2324, /// 0x5c8
			 0x71977bf4, /// 0x5cc
			 0x8de96c75, /// 0x5d0
			 0xe33edbe4, /// 0x5d4
			 0xfd8dba46, /// 0x5d8
			 0xcef1c73b, /// 0x5dc
			 0xec9baf61, /// 0x5e0
			 0xa0e26629, /// 0x5e4
			 0x12a1d41a, /// 0x5e8
			 0x02b757c4, /// 0x5ec
			 0x0485a2f4, /// 0x5f0
			 0x8efdcab1, /// 0x5f4
			 0x77d04ba2, /// 0x5f8
			 0x507d275e, /// 0x5fc
			 0x92d65417, /// 0x600
			 0x5a023185, /// 0x604
			 0xbb822421, /// 0x608
			 0x1273a255, /// 0x60c
			 0xa9ae1de2, /// 0x610
			 0x56665509, /// 0x614
			 0x4fcab638, /// 0x618
			 0xf5c3c838, /// 0x61c
			 0x6a531f68, /// 0x620
			 0x4def03fa, /// 0x624
			 0xd0d3929e, /// 0x628
			 0xed304b45, /// 0x62c
			 0xa8310c53, /// 0x630
			 0x9d1dd196, /// 0x634
			 0x3abe0d2e, /// 0x638
			 0xcd2265f4, /// 0x63c
			 0xb2a25b82, /// 0x640
			 0xf568c8cc, /// 0x644
			 0x1390a816, /// 0x648
			 0x6e9249ab, /// 0x64c
			 0x5d13bc0e, /// 0x650
			 0xacc1325d, /// 0x654
			 0x33878c65, /// 0x658
			 0x8d9a13d3, /// 0x65c
			 0xdee7943e, /// 0x660
			 0x7520a537, /// 0x664
			 0x3f854c90, /// 0x668
			 0xada0b743, /// 0x66c
			 0x7d989cdd, /// 0x670
			 0x3be1fadb, /// 0x674
			 0x2cfd40f8, /// 0x678
			 0xa6e9d2e5, /// 0x67c
			 0x765ecf93, /// 0x680
			 0x717ad8ba, /// 0x684
			 0x0a14e2c5, /// 0x688
			 0x41b9d38f, /// 0x68c
			 0x4f348671, /// 0x690
			 0xbc113e02, /// 0x694
			 0x05743c55, /// 0x698
			 0x22e01eb0, /// 0x69c
			 0x15a551b7, /// 0x6a0
			 0x4296b180, /// 0x6a4
			 0x1e6dff49, /// 0x6a8
			 0x5fb689bb, /// 0x6ac
			 0xfe7b9d44, /// 0x6b0
			 0xee49a3ad, /// 0x6b4
			 0xbbeb3ad2, /// 0x6b8
			 0x04b5021e, /// 0x6bc
			 0x4aefa5ac, /// 0x6c0
			 0xa53a63c2, /// 0x6c4
			 0x1e410e35, /// 0x6c8
			 0xd4122830, /// 0x6cc
			 0xaaf3cf51, /// 0x6d0
			 0x7e26ac88, /// 0x6d4
			 0x09818bde, /// 0x6d8
			 0xb38aefa4, /// 0x6dc
			 0x28751241, /// 0x6e0
			 0x450d4b57, /// 0x6e4
			 0x032b063a, /// 0x6e8
			 0xa304385a, /// 0x6ec
			 0xe7dbd12b, /// 0x6f0
			 0x4ddee571, /// 0x6f4
			 0xeb7e19a9, /// 0x6f8
			 0x22f5ba47, /// 0x6fc
			 0x5714a7b6, /// 0x700
			 0xb67884bb, /// 0x704
			 0xcc31e765, /// 0x708
			 0x5b7e9bc1, /// 0x70c
			 0x9516ed0d, /// 0x710
			 0x2a1c3b5e, /// 0x714
			 0xe5853e71, /// 0x718
			 0x6f952609, /// 0x71c
			 0x614fa5c9, /// 0x720
			 0xa23c7cc5, /// 0x724
			 0x208b6c34, /// 0x728
			 0x3957f2ef, /// 0x72c
			 0x5961871b, /// 0x730
			 0x6521450a, /// 0x734
			 0x76b091f6, /// 0x738
			 0x895642b7, /// 0x73c
			 0x02f33338, /// 0x740
			 0xebb7f08e, /// 0x744
			 0x53024275, /// 0x748
			 0xe0b0b624, /// 0x74c
			 0xb3b8eea7, /// 0x750
			 0x29e610cb, /// 0x754
			 0x8ea0446a, /// 0x758
			 0xe5b07c72, /// 0x75c
			 0xc174c8b9, /// 0x760
			 0x43a7aeb0, /// 0x764
			 0x636f1eb7, /// 0x768
			 0xfdf458f9, /// 0x76c
			 0x244a5b42, /// 0x770
			 0x65c5b348, /// 0x774
			 0x609903a8, /// 0x778
			 0x09092926, /// 0x77c
			 0xbd3ce769, /// 0x780
			 0x24c9f5e1, /// 0x784
			 0x24117bc2, /// 0x788
			 0x1c012e61, /// 0x78c
			 0xb822c201, /// 0x790
			 0x8d314027, /// 0x794
			 0x2dcb6e7e, /// 0x798
			 0x0d7a688b, /// 0x79c
			 0x47d2dc38, /// 0x7a0
			 0x08f241e5, /// 0x7a4
			 0x42966ea0, /// 0x7a8
			 0x95d1a817, /// 0x7ac
			 0xdf8125a5, /// 0x7b0
			 0x3e63d646, /// 0x7b4
			 0x1aed8c76, /// 0x7b8
			 0x6d020e08, /// 0x7bc
			 0x68bab857, /// 0x7c0
			 0x593f8035, /// 0x7c4
			 0x23fed56a, /// 0x7c8
			 0x943640e4, /// 0x7cc
			 0xe875d950, /// 0x7d0
			 0x7860b2cc, /// 0x7d4
			 0x8639c04f, /// 0x7d8
			 0xeb0fbf76, /// 0x7dc
			 0x18394ac0, /// 0x7e0
			 0x407c6752, /// 0x7e4
			 0xa4feb5c8, /// 0x7e8
			 0x4e1a4a44, /// 0x7ec
			 0x2e0dddf5, /// 0x7f0
			 0x41d17df3, /// 0x7f4
			 0xeee35cca, /// 0x7f8
			 0xf5eee871, /// 0x7fc
			 0x1986e9ae, /// 0x800
			 0xc113cefa, /// 0x804
			 0x34fce1e0, /// 0x808
			 0xfc4fa435, /// 0x80c
			 0x32c2dfaa, /// 0x810
			 0x3936ecbe, /// 0x814
			 0x68ebbdca, /// 0x818
			 0x8fd6c36e, /// 0x81c
			 0xd85ac878, /// 0x820
			 0x7c3d27a8, /// 0x824
			 0xbec90ca1, /// 0x828
			 0x401e09a9, /// 0x82c
			 0x4400e4aa, /// 0x830
			 0x30c3243c, /// 0x834
			 0xed4e039d, /// 0x838
			 0x66009816, /// 0x83c
			 0x462e5195, /// 0x840
			 0x37a9693d, /// 0x844
			 0x86c3620d, /// 0x848
			 0xda01cdf1, /// 0x84c
			 0x6532fd88, /// 0x850
			 0x7708ebf1, /// 0x854
			 0x9aeb71f6, /// 0x858
			 0x4eb5e5da, /// 0x85c
			 0x6bac42d6, /// 0x860
			 0x2f71de3d, /// 0x864
			 0x93c87cbd, /// 0x868
			 0x75feb1cb, /// 0x86c
			 0xc84c7f93, /// 0x870
			 0x329e45ca, /// 0x874
			 0x2e82de30, /// 0x878
			 0x7da6866f, /// 0x87c
			 0xe78c939e, /// 0x880
			 0x392b411d, /// 0x884
			 0xe10555d5, /// 0x888
			 0x9037f7c5, /// 0x88c
			 0xb0a80eea, /// 0x890
			 0x46d19457, /// 0x894
			 0x7dd8d788, /// 0x898
			 0x980eee8f, /// 0x89c
			 0xd790479c, /// 0x8a0
			 0xac2107c5, /// 0x8a4
			 0x3f81ad70, /// 0x8a8
			 0xbccf270d, /// 0x8ac
			 0x4ecc7e54, /// 0x8b0
			 0x83d0458a, /// 0x8b4
			 0x0c9b22d3, /// 0x8b8
			 0x96ec7066, /// 0x8bc
			 0xfa8a328b, /// 0x8c0
			 0x59962a35, /// 0x8c4
			 0x626568da, /// 0x8c8
			 0x52870e67, /// 0x8cc
			 0xcac1e8cf, /// 0x8d0
			 0xeebb7ea4, /// 0x8d4
			 0xbdbb5496, /// 0x8d8
			 0x322a0408, /// 0x8dc
			 0xf20a873e, /// 0x8e0
			 0xc8c7a9f4, /// 0x8e4
			 0xeeced0a4, /// 0x8e8
			 0xea4c9dac, /// 0x8ec
			 0xd7308a07, /// 0x8f0
			 0xad6c9028, /// 0x8f4
			 0x44917b4d, /// 0x8f8
			 0x485ff94e, /// 0x8fc
			 0x8895c720, /// 0x900
			 0x4b06c930, /// 0x904
			 0x6161ef39, /// 0x908
			 0x1b517920, /// 0x90c
			 0xee834db1, /// 0x910
			 0xaa29c831, /// 0x914
			 0x09f4286e, /// 0x918
			 0xd5b27c9a, /// 0x91c
			 0x9d157fe7, /// 0x920
			 0x37985fb6, /// 0x924
			 0xf9f950ae, /// 0x928
			 0xe3bfb290, /// 0x92c
			 0xfafacbab, /// 0x930
			 0x57894f0f, /// 0x934
			 0xdbfed5fc, /// 0x938
			 0x63dbc871, /// 0x93c
			 0x1254a2c9, /// 0x940
			 0x592ea29a, /// 0x944
			 0x77496d05, /// 0x948
			 0x252766f0, /// 0x94c
			 0xa75bd17b, /// 0x950
			 0x0a103439, /// 0x954
			 0xcc2b0fe6, /// 0x958
			 0x69e66071, /// 0x95c
			 0xade8b1ee, /// 0x960
			 0xb6ee9457, /// 0x964
			 0x422a8906, /// 0x968
			 0x13c6a492, /// 0x96c
			 0xdaa3e276, /// 0x970
			 0x730d0980, /// 0x974
			 0x66dc3b07, /// 0x978
			 0x6c4ff745, /// 0x97c
			 0x8f89478b, /// 0x980
			 0x89fbb20e, /// 0x984
			 0x9d119b45, /// 0x988
			 0x11670b9e, /// 0x98c
			 0xed7929de, /// 0x990
			 0x2aa10c92, /// 0x994
			 0x102f0a2c, /// 0x998
			 0x5d358d22, /// 0x99c
			 0xc8e7961c, /// 0x9a0
			 0x7a3f601e, /// 0x9a4
			 0x2770e5fb, /// 0x9a8
			 0xa29770e0, /// 0x9ac
			 0x7db0b1c7, /// 0x9b0
			 0x3caf0b7a, /// 0x9b4
			 0x8c8984f6, /// 0x9b8
			 0xdd7398b4, /// 0x9bc
			 0x3ce92b46, /// 0x9c0
			 0xbf3b1810, /// 0x9c4
			 0x4e89b5a1, /// 0x9c8
			 0x6c61b84d, /// 0x9cc
			 0x1b6a48e1, /// 0x9d0
			 0x19afae8a, /// 0x9d4
			 0xe1621745, /// 0x9d8
			 0x3155b52f, /// 0x9dc
			 0xe6e4e3d3, /// 0x9e0
			 0xb7698d0d, /// 0x9e4
			 0x5ee4d4de, /// 0x9e8
			 0x7579c066, /// 0x9ec
			 0x83e1ccb5, /// 0x9f0
			 0xf9324c1f, /// 0x9f4
			 0xbbf6305c, /// 0x9f8
			 0x9637e2f0, /// 0x9fc
			 0x3a9d3788, /// 0xa00
			 0xe25ae9dd, /// 0xa04
			 0x0b81999a, /// 0xa08
			 0xfba291fd, /// 0xa0c
			 0x0394348d, /// 0xa10
			 0x8d685c30, /// 0xa14
			 0xa4a1a83f, /// 0xa18
			 0x777c3f23, /// 0xa1c
			 0x53e81770, /// 0xa20
			 0x7fd011da, /// 0xa24
			 0x254bc825, /// 0xa28
			 0x69510341, /// 0xa2c
			 0x4527800d, /// 0xa30
			 0xadbddde0, /// 0xa34
			 0x505b2d9a, /// 0xa38
			 0x60933416, /// 0xa3c
			 0x3d1ae60c, /// 0xa40
			 0x0428bb95, /// 0xa44
			 0xa42c4476, /// 0xa48
			 0xf44cce6b, /// 0xa4c
			 0x6fec3863, /// 0xa50
			 0x83338193, /// 0xa54
			 0xd19d71f0, /// 0xa58
			 0x92da2e5a, /// 0xa5c
			 0xe76b3e1b, /// 0xa60
			 0x3df09496, /// 0xa64
			 0xf77e6ef0, /// 0xa68
			 0x2b149f78, /// 0xa6c
			 0x5b751a86, /// 0xa70
			 0x7c39a8f3, /// 0xa74
			 0x8333ceb0, /// 0xa78
			 0x466bf227, /// 0xa7c
			 0x68261330, /// 0xa80
			 0x99ee27df, /// 0xa84
			 0xda9d93db, /// 0xa88
			 0xaffedb17, /// 0xa8c
			 0x453c5aa5, /// 0xa90
			 0x5e431203, /// 0xa94
			 0xf05971e8, /// 0xa98
			 0xeb85f8f8, /// 0xa9c
			 0x1147ff71, /// 0xaa0
			 0xb6eb2904, /// 0xaa4
			 0x4a7efe9c, /// 0xaa8
			 0x8b4cbb1f, /// 0xaac
			 0x2e97e5f2, /// 0xab0
			 0x8cb166aa, /// 0xab4
			 0x4a84cdf0, /// 0xab8
			 0xb1cf8751, /// 0xabc
			 0x5bf86a68, /// 0xac0
			 0x1c859580, /// 0xac4
			 0x949fc77a, /// 0xac8
			 0x755f77e0, /// 0xacc
			 0x98efb64f, /// 0xad0
			 0xfc910fb0, /// 0xad4
			 0x57d303de, /// 0xad8
			 0x39ccd65e, /// 0xadc
			 0xf14861d3, /// 0xae0
			 0xc294b159, /// 0xae4
			 0x5d7ed047, /// 0xae8
			 0x0958d2ab, /// 0xaec
			 0x051b7d8e, /// 0xaf0
			 0x29a28658, /// 0xaf4
			 0x22deffac, /// 0xaf8
			 0x74487cea, /// 0xafc
			 0x69bea1db, /// 0xb00
			 0x99a0272d, /// 0xb04
			 0x631f9c60, /// 0xb08
			 0x53914923, /// 0xb0c
			 0x0ab60765, /// 0xb10
			 0x5ae2fdd7, /// 0xb14
			 0x6765bbd4, /// 0xb18
			 0xb460c50e, /// 0xb1c
			 0x551780ac, /// 0xb20
			 0xae823f2e, /// 0xb24
			 0x4cd60a93, /// 0xb28
			 0x72c630f1, /// 0xb2c
			 0x3804dc86, /// 0xb30
			 0xb9d659ab, /// 0xb34
			 0x34d62637, /// 0xb38
			 0x31e6d1fb, /// 0xb3c
			 0x713da588, /// 0xb40
			 0xbe9304fe, /// 0xb44
			 0xd5135974, /// 0xb48
			 0x62b84d1b, /// 0xb4c
			 0x18ef0406, /// 0xb50
			 0x84b99562, /// 0xb54
			 0x26c60766, /// 0xb58
			 0x14696a1a, /// 0xb5c
			 0x09e4e8b4, /// 0xb60
			 0x8c4ea4af, /// 0xb64
			 0xc3292331, /// 0xb68
			 0x61dd5b60, /// 0xb6c
			 0x2550452d, /// 0xb70
			 0xc7331480, /// 0xb74
			 0x9ac62bc5, /// 0xb78
			 0x577781cd, /// 0xb7c
			 0x303fca50, /// 0xb80
			 0xc7d14117, /// 0xb84
			 0x3156873c, /// 0xb88
			 0x5101a313, /// 0xb8c
			 0x0f9a268d, /// 0xb90
			 0xf089b0af, /// 0xb94
			 0xdda361a4, /// 0xb98
			 0x40e07aef, /// 0xb9c
			 0xd31e9738, /// 0xba0
			 0xe46abd06, /// 0xba4
			 0x14ff9be3, /// 0xba8
			 0x6a8edd31, /// 0xbac
			 0x8743bbd2, /// 0xbb0
			 0x08851ee7, /// 0xbb4
			 0x923d4f93, /// 0xbb8
			 0xb1b6e739, /// 0xbbc
			 0x7f96cda8, /// 0xbc0
			 0x68559e6e, /// 0xbc4
			 0x815a6854, /// 0xbc8
			 0x4626b2ca, /// 0xbcc
			 0xf252c710, /// 0xbd0
			 0xb4e15b4f, /// 0xbd4
			 0xc6fdfcad, /// 0xbd8
			 0x9efeba61, /// 0xbdc
			 0x4593c760, /// 0xbe0
			 0x05450403, /// 0xbe4
			 0x366e37de, /// 0xbe8
			 0x600f4a6f, /// 0xbec
			 0x6978a639, /// 0xbf0
			 0x33bf2d28, /// 0xbf4
			 0x55b8d08c, /// 0xbf8
			 0xc31a1df4, /// 0xbfc
			 0x1b94e6aa, /// 0xc00
			 0xaa540e27, /// 0xc04
			 0x725148c5, /// 0xc08
			 0x05dfbf87, /// 0xc0c
			 0x4805f788, /// 0xc10
			 0xeeee767e, /// 0xc14
			 0x184035e6, /// 0xc18
			 0xe6ba21b8, /// 0xc1c
			 0xb609e189, /// 0xc20
			 0x580cc31e, /// 0xc24
			 0x211f3148, /// 0xc28
			 0x84c7513b, /// 0xc2c
			 0x6f6c444a, /// 0xc30
			 0xec7283ea, /// 0xc34
			 0x40db03a8, /// 0xc38
			 0x4478f1d4, /// 0xc3c
			 0x6a8d88c2, /// 0xc40
			 0x65e2494e, /// 0xc44
			 0x5e67a4e0, /// 0xc48
			 0xae3993d5, /// 0xc4c
			 0x20a14ec0, /// 0xc50
			 0x6ed00200, /// 0xc54
			 0x8e2d9e44, /// 0xc58
			 0xc8ad4a4d, /// 0xc5c
			 0xdab25ab5, /// 0xc60
			 0xbcf9944c, /// 0xc64
			 0x00198911, /// 0xc68
			 0x3626c8c3, /// 0xc6c
			 0xc0d3da73, /// 0xc70
			 0x10193868, /// 0xc74
			 0x57444a0b, /// 0xc78
			 0x850065b2, /// 0xc7c
			 0xf1b6052b, /// 0xc80
			 0x97dc9100, /// 0xc84
			 0x0bea3572, /// 0xc88
			 0x5043f1af, /// 0xc8c
			 0xa6aa2a6e, /// 0xc90
			 0x3133ea05, /// 0xc94
			 0xfcf20be5, /// 0xc98
			 0x50896a45, /// 0xc9c
			 0xb4b3d3a5, /// 0xca0
			 0x70b6e76e, /// 0xca4
			 0xc7d5ff2f, /// 0xca8
			 0xa4c414b6, /// 0xcac
			 0xcce77253, /// 0xcb0
			 0xd5f81b96, /// 0xcb4
			 0x877a4633, /// 0xcb8
			 0x9df622bd, /// 0xcbc
			 0x23e43b38, /// 0xcc0
			 0xa1d247aa, /// 0xcc4
			 0xaee7c902, /// 0xcc8
			 0x37910453, /// 0xccc
			 0x25eeeb20, /// 0xcd0
			 0xe627cb47, /// 0xcd4
			 0x795229e0, /// 0xcd8
			 0x3a84ddca, /// 0xcdc
			 0xc340397a, /// 0xce0
			 0x2a281106, /// 0xce4
			 0x63c0f3cc, /// 0xce8
			 0x378d19f7, /// 0xcec
			 0x46fa0ae2, /// 0xcf0
			 0xffbdae0c, /// 0xcf4
			 0xe1cf31dd, /// 0xcf8
			 0xd5c92f2a, /// 0xcfc
			 0x2ee3598c, /// 0xd00
			 0x9c46bf72, /// 0xd04
			 0x3a33a296, /// 0xd08
			 0xc5db0c58, /// 0xd0c
			 0x211b61b0, /// 0xd10
			 0xab16f4af, /// 0xd14
			 0x0e137c69, /// 0xd18
			 0x849a04a5, /// 0xd1c
			 0xbcbb73b9, /// 0xd20
			 0xbf015b0a, /// 0xd24
			 0x5c0c6260, /// 0xd28
			 0x33ccb42e, /// 0xd2c
			 0x58e1e11f, /// 0xd30
			 0x7754ffc5, /// 0xd34
			 0x09f1c327, /// 0xd38
			 0x9e4c81c8, /// 0xd3c
			 0x0b47808d, /// 0xd40
			 0x31df9036, /// 0xd44
			 0x8c752316, /// 0xd48
			 0x9bba4cdf, /// 0xd4c
			 0x1bc8cb25, /// 0xd50
			 0x7ae785c3, /// 0xd54
			 0xf06a39ff, /// 0xd58
			 0xe0cd12d8, /// 0xd5c
			 0x247c53c3, /// 0xd60
			 0xd80a52e2, /// 0xd64
			 0xbdbb4d92, /// 0xd68
			 0x9ee39455, /// 0xd6c
			 0x5838a681, /// 0xd70
			 0x5b5f52ce, /// 0xd74
			 0xa1981274, /// 0xd78
			 0xc514264a, /// 0xd7c
			 0x23f5052e, /// 0xd80
			 0x5b73b1bb, /// 0xd84
			 0x72a7fa66, /// 0xd88
			 0x568fd174, /// 0xd8c
			 0xc3055d78, /// 0xd90
			 0x5dd96b94, /// 0xd94
			 0x667c5cb5, /// 0xd98
			 0x281ed520, /// 0xd9c
			 0xe1d03d04, /// 0xda0
			 0x28409a27, /// 0xda4
			 0x32dc0b92, /// 0xda8
			 0xec3ac11e, /// 0xdac
			 0x7f7cb258, /// 0xdb0
			 0x9a66a4ba, /// 0xdb4
			 0x582dca31, /// 0xdb8
			 0x80f54569, /// 0xdbc
			 0xbabb5b84, /// 0xdc0
			 0xb4cd9a2c, /// 0xdc4
			 0xe76fc19b, /// 0xdc8
			 0x51f7714e, /// 0xdcc
			 0xc25179a9, /// 0xdd0
			 0x010be2da, /// 0xdd4
			 0x6151da3e, /// 0xdd8
			 0xf64d33bd, /// 0xddc
			 0xf2723fcf, /// 0xde0
			 0x358690c7, /// 0xde4
			 0xa03b0d8d, /// 0xde8
			 0x724d41ca, /// 0xdec
			 0x71333b22, /// 0xdf0
			 0xb1659fd7, /// 0xdf4
			 0xcfa3eb04, /// 0xdf8
			 0x955dfdba, /// 0xdfc
			 0xb72268d9, /// 0xe00
			 0x68c272e9, /// 0xe04
			 0xf34ba708, /// 0xe08
			 0xf1acefb9, /// 0xe0c
			 0x381dd8df, /// 0xe10
			 0x2c519c4f, /// 0xe14
			 0x5fcf50cc, /// 0xe18
			 0xe4383827, /// 0xe1c
			 0xcf9c6a6b, /// 0xe20
			 0x8d461dbc, /// 0xe24
			 0x387f400e, /// 0xe28
			 0xf3416a8b, /// 0xe2c
			 0xb0df30fe, /// 0xe30
			 0xe83e7e9c, /// 0xe34
			 0x979f4021, /// 0xe38
			 0xe40dfe5b, /// 0xe3c
			 0x6311cb31, /// 0xe40
			 0x2aab2049, /// 0xe44
			 0x1f379ae9, /// 0xe48
			 0xdc1da793, /// 0xe4c
			 0x71924b8f, /// 0xe50
			 0x2f9b9812, /// 0xe54
			 0xafdfc5bf, /// 0xe58
			 0x67c0dae9, /// 0xe5c
			 0x84ce02b5, /// 0xe60
			 0xabb84546, /// 0xe64
			 0x3b4be042, /// 0xe68
			 0x4720f9dc, /// 0xe6c
			 0x906b80a1, /// 0xe70
			 0xa46a6455, /// 0xe74
			 0xb5830402, /// 0xe78
			 0xfc7f7f45, /// 0xe7c
			 0xa48780f3, /// 0xe80
			 0x2e009bea, /// 0xe84
			 0x5f846c51, /// 0xe88
			 0x7c929e26, /// 0xe8c
			 0x97c74fbb, /// 0xe90
			 0x58243d15, /// 0xe94
			 0x0bac1d6b, /// 0xe98
			 0x8c6ee7f5, /// 0xe9c
			 0xb890540f, /// 0xea0
			 0xeb2e2464, /// 0xea4
			 0x86ba4faa, /// 0xea8
			 0xd3b3c565, /// 0xeac
			 0x1f607c8a, /// 0xeb0
			 0xe20e17a5, /// 0xeb4
			 0x13318e61, /// 0xeb8
			 0x08bb6bf1, /// 0xebc
			 0x38717fea, /// 0xec0
			 0x74289b06, /// 0xec4
			 0xb35febb9, /// 0xec8
			 0xb227141a, /// 0xecc
			 0xeacf5641, /// 0xed0
			 0xeb32fb99, /// 0xed4
			 0x77cf3425, /// 0xed8
			 0xbce79080, /// 0xedc
			 0xa040c719, /// 0xee0
			 0xc898f11b, /// 0xee4
			 0xb675935d, /// 0xee8
			 0x19212d65, /// 0xeec
			 0xa9aaec76, /// 0xef0
			 0x9651199f, /// 0xef4
			 0x334e8246, /// 0xef8
			 0xfd51c1a3, /// 0xefc
			 0xc8bea305, /// 0xf00
			 0x284f52a4, /// 0xf04
			 0xa1e14d35, /// 0xf08
			 0x2403f0d9, /// 0xf0c
			 0x11be6b01, /// 0xf10
			 0xd9a8c4f5, /// 0xf14
			 0xfd991c1f, /// 0xf18
			 0x17b05910, /// 0xf1c
			 0x91433515, /// 0xf20
			 0x5c9e728a, /// 0xf24
			 0xb27bbc5e, /// 0xf28
			 0x27b54ad4, /// 0xf2c
			 0x838d1f82, /// 0xf30
			 0x855fa20d, /// 0xf34
			 0xed2b7844, /// 0xf38
			 0x0e79b165, /// 0xf3c
			 0x80654e09, /// 0xf40
			 0x4393a730, /// 0xf44
			 0xc8687b7b, /// 0xf48
			 0x4e722fe6, /// 0xf4c
			 0xc2fe1c4f, /// 0xf50
			 0x2f6f70f0, /// 0xf54
			 0xd805c4c5, /// 0xf58
			 0x3ba2b3be, /// 0xf5c
			 0x936f6843, /// 0xf60
			 0x17e9680b, /// 0xf64
			 0xae830311, /// 0xf68
			 0xa620216d, /// 0xf6c
			 0x4088887e, /// 0xf70
			 0xde779cb3, /// 0xf74
			 0x356484e1, /// 0xf78
			 0xa0c902d8, /// 0xf7c
			 0x17212618, /// 0xf80
			 0x0343f4c6, /// 0xf84
			 0x7b838bf8, /// 0xf88
			 0x095b4959, /// 0xf8c
			 0xcad63962, /// 0xf90
			 0xdefc873c, /// 0xf94
			 0x8ae47137, /// 0xf98
			 0x3286030d, /// 0xf9c
			 0xdeafaaeb, /// 0xfa0
			 0x921c55a8, /// 0xfa4
			 0xe54336be, /// 0xfa8
			 0x39fe744d, /// 0xfac
			 0x691981e9, /// 0xfb0
			 0xa9a0cf5e, /// 0xfb4
			 0x483b1477, /// 0xfb8
			 0x503554f1, /// 0xfbc
			 0xec0c6d74, /// 0xfc0
			 0x776a6406, /// 0xfc4
			 0x6c128261, /// 0xfc8
			 0x5f3eb910, /// 0xfcc
			 0x1f148c1f, /// 0xfd0
			 0x274168e4, /// 0xfd4
			 0x3c5525d1, /// 0xfd8
			 0x292f0892, /// 0xfdc
			 0xbe0fa7f0, /// 0xfe0
			 0x34d6d49a, /// 0xfe4
			 0x753b3bfb, /// 0xfe8
			 0xd8bc73af, /// 0xfec
			 0xcc94d8bb, /// 0xff0
			 0xc8ce5ab2, /// 0xff4
			 0x2372c1d3, /// 0xff8
			 0x5b353839 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00000
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00004
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00010
			 0x33333333,                                                  // 4 random values                             /// 00014
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00018
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0001c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00020
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x7fc00001,                                                  // signaling NaN                               /// 00028
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0002c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00030
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00034
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0003c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00040
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00048
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0004c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00050
			 0xffc00001,                                                  // -signaling NaN                              /// 00054
			 0x0e000003,                                                  // Trailing 1s:                                /// 00058
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0005c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00060
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00068
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00070
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00074
			 0xcb000000,                                                  // -8388608.0                                  /// 00078
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0xffc00001,                                                  // -signaling NaN                              /// 00080
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0x80000000,                                                  // -zero                                       /// 0008c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00090
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00098
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0009c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00108
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00114
			 0x7f800000,                                                  // inf                                         /// 00118
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00128
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00130
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0013c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00144
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00148
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00150
			 0x0e000001,                                                  // Trailing 1s:                                /// 00154
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00160
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00164
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x7f800000,                                                  // inf                                         /// 0016c
			 0xcb000000,                                                  // -8388608.0                                  /// 00170
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0017c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00180
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00184
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00188
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0018c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00190
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x0c780000,                                                  // Leading 1s:                                 /// 00198
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0xcb000000,                                                  // -8388608.0                                  /// 001c0
			 0x3f028f5c,                                                  // 0.51                                        /// 001c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 001e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 001ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00200
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00204
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00208
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00210
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00218
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0021c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00220
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0xff800000,                                                  // -inf                                        /// 00228
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00234
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00238
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0023c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00240
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00244
			 0xbf028f5c,                                                  // -0.51                                       /// 00248
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0024c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00250
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00258
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x7f800000,                                                  // inf                                         /// 00260
			 0x0e000001,                                                  // Trailing 1s:                                /// 00264
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00268
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00270
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00274
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0e000001,                                                  // Trailing 1s:                                /// 0027c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00280
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00284
			 0x0e000003,                                                  // Trailing 1s:                                /// 00288
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0028c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00290
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00294
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00298
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0029c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 002c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00300
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00308
			 0x0c400000,                                                  // Leading 1s:                                 /// 0030c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00310
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00314
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00320
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00324
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00328
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0032c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00330
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00334
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00344
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00348
			 0x0c700000,                                                  // Leading 1s:                                 /// 0034c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00350
			 0x0c400000,                                                  // Leading 1s:                                 /// 00354
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00358
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0035c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00368
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00370
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00378
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00380
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0038c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00390
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00398
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0039c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003a0
			 0x80000000,                                                  // -zero                                       /// 003a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003ac
			 0x7fc00001,                                                  // signaling NaN                               /// 003b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 003d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003dc
			 0x4b000000,                                                  // 8388608.0                                   /// 003e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x7f800000,                                                  // inf                                         /// 003f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x33333333,                                                  // 4 random values                             /// 003fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00404
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0040c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00410
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00414
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0041c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00420
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00424
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00428
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00434
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00438
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0043c
			 0x55555555,                                                  // 4 random values                             /// 00440
			 0xff800000,                                                  // -inf                                        /// 00444
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00011111,                                                  // 9.7958E-41                                  /// 0044c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00454
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0045c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00460
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00468
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0046c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00470
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0x33333333,                                                  // 4 random values                             /// 0047c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00480
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00484
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0048c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00490
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00494
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0049c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004b0
			 0xff800000,                                                  // -inf                                        /// 004b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004d0
			 0xcb000000,                                                  // -8388608.0                                  /// 004d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 004fc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00500
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00504
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00508
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0050c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00514
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0051c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00520
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00524
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0052c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00530
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00538
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0053c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00544
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00548
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0054c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00550
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00554
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00558
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0055c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00564
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00570
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00578
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0057c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00584
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00588
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0058c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00590
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00594
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0059c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00604
			 0x0c780000,                                                  // Leading 1s:                                 /// 00608
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00610
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x00011111,                                                  // 9.7958E-41                                  /// 00618
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x3f028f5c,                                                  // 0.51                                        /// 00620
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00628
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0062c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00630
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00638
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0063c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00640
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00644
			 0x0c600000,                                                  // Leading 1s:                                 /// 00648
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00650
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00654
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00658
			 0x3f028f5c,                                                  // 0.51                                        /// 0065c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00660
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00664
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00668
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0066c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00678
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0067c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00680
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00684
			 0x0e000007,                                                  // Trailing 1s:                                /// 00688
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00690
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00694
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00698
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006a4
			 0x7f800000,                                                  // inf                                         /// 006a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006e8
			 0xff800000,                                                  // -inf                                        /// 006ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00700
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00704
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00708
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0070c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00710
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00714
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00718
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0071c
			 0x33333333,                                                  // 4 random values                             /// 00720
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x55555555,                                                  // 4 random values                             /// 0072c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00734
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0073c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00740
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00748
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0074c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00750
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0075c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00764
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00768
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0076c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00770
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00778
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00780
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00788
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x0c400000,                                                  // Leading 1s:                                 /// 00798
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0079c
			 0x33333333,                                                  // 4 random values                             /// 007a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007d4
			 0x3f028f5c,                                                  // 0.51                                        /// 007d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007f8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00800
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00804
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0x80011111,                                                  // -9.7958E-41                                 /// 00818
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0081c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00824
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00828
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x55555555,                                                  // 4 random values                             /// 00830
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00834
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00838
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00840
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00844
			 0x0c700000,                                                  // Leading 1s:                                 /// 00848
			 0xcb000000,                                                  // -8388608.0                                  /// 0084c
			 0xff800000,                                                  // -inf                                        /// 00850
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00858
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0085c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00860
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0086c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00878
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0087c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00880
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00884
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00890
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00894
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00898
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0089c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 008a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x33333333,                                                  // 4 random values                             /// 008b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 008cc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008e0
			 0x7fc00001,                                                  // signaling NaN                               /// 008e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 008fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00000000,                                                  // zero                                        /// 00908
			 0x0c780000,                                                  // Leading 1s:                                 /// 0090c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00918
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0091c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00920
			 0x0e000001,                                                  // Trailing 1s:                                /// 00924
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0092c
			 0xcb000000,                                                  // -8388608.0                                  /// 00930
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00934
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00938
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0093c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00940
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0xff800000,                                                  // -inf                                        /// 00948
			 0xbf028f5c,                                                  // -0.51                                       /// 0094c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00950
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00958
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0095c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00960
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00964
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00968
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0096c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00970
			 0xff800000,                                                  // -inf                                        /// 00974
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00978
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0097c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00980
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00988
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00990
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0099c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80000000,                                                  // -zero                                       /// 009a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x80000000,                                                  // -zero                                       /// 009bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009c8
			 0xff800000,                                                  // -inf                                        /// 009cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 009d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 009f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a14
			 0x33333333,                                                  // 4 random values                             /// 00a18
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a1c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a34
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a40
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a44
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a54
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a58
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a64
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a68
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0xcb000000,                                                  // -8388608.0                                  /// 00a7c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a84
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a88
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a90
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a9c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aa0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00aa4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ab4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ab8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00abc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ac0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ac4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ac8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00acc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ad0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ad8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00adc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ae0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ae4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x55555555,                                                  // 4 random values                             /// 00af0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00afc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b00
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b0c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b14
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b28
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b2c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b38
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b3c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b58
			 0x4b000000,                                                  // 8388608.0                                   /// 00b5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b74
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b88
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b8c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ba0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ba4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x3f028f5c,                                                  // 0.51                                        /// 00bc0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bc4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bc8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x55555555,                                                  // 4 random values                             /// 00bd0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bd4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00be0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00be4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00be8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bf0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bf4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c00
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c08
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c0c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c18
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c40
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c64
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c68
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c6c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c84
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c8c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c90
			 0x33333333,                                                  // 4 random values                             /// 00c94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c9c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ca0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ca4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ca8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cb8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cc8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cd0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cd4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cdc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ce0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cf0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cfc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d04
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d10
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d14
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d30
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d38
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d40
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d4c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d58
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d5c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x00000000,                                                  // zero                                        /// 00d68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d90
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d94
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d98
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00da4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00da8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00db4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00db8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dc0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dd0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dd4
			 0x00000000,                                                  // zero                                        /// 00dd8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ddc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00de0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00de4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00de8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00df8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e00
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e04
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e08
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e0c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e10
			 0x4b000000,                                                  // 8388608.0                                   /// 00e14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e18
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e1c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e24
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e2c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e30
			 0xffc00001,                                                  // -signaling NaN                              /// 00e34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e3c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e40
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e48
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e50
			 0x3f028f5c,                                                  // 0.51                                        /// 00e54
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e68
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e70
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e78
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e80
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e84
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e88
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e8c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e90
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e98
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ea0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ea8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00eac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00eb0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0xff800000,                                                  // -inf                                        /// 00eb8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ec0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ec4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ed0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ed4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00edc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ee8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ef0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ef8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f00
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f18
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f1c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f30
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f40
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f44
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f48
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f58
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f68
			 0xcb000000,                                                  // -8388608.0                                  /// 00f6c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f70
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f80
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f8c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f90
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fa0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x7fc00001,                                                  // signaling NaN                               /// 00fb8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fc4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fcc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fd0
			 0x55555555,                                                  // 4 random values                             /// 00fd4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fdc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fe0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fe4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fe8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ff8
			 0x0e00000f                                                  // Trailing 1s:                                /// last
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
			 0x00000038,
			 0x000005a0,
			 0x00000704,
			 0x0000003c,
			 0x000003e4,
			 0x00000110,
			 0x00000294,
			 0x00000328,

			 /// vpu register f2
			 0x41100000,
			 0x40e00000,
			 0x3f800000,
			 0x41f00000,
			 0x41b00000,
			 0x40400000,
			 0x41000000,
			 0x41f80000,

			 /// vpu register f3
			 0x41500000,
			 0x40e00000,
			 0x41300000,
			 0x41a80000,
			 0x40a00000,
			 0x41c00000,
			 0x41c00000,
			 0x41100000,

			 /// vpu register f4
			 0x41880000,
			 0x42000000,
			 0x41200000,
			 0x41100000,
			 0x41b00000,
			 0x41a00000,
			 0x41b00000,
			 0x41c80000,

			 /// vpu register f5
			 0x41980000,
			 0x41b80000,
			 0x41b00000,
			 0x41f80000,
			 0x41980000,
			 0x41b80000,
			 0x40000000,
			 0x41b80000,

			 /// vpu register f6
			 0x41c80000,
			 0x40800000,
			 0x41f80000,
			 0x40a00000,
			 0x41400000,
			 0x42000000,
			 0x41d00000,
			 0x41b00000,

			 /// vpu register f7
			 0x41700000,
			 0x41800000,
			 0x41800000,
			 0x41980000,
			 0x41500000,
			 0x41200000,
			 0x41e80000,
			 0x41980000,

			 /// vpu register f8
			 0x41600000,
			 0x41a80000,
			 0x41e00000,
			 0x41200000,
			 0x41300000,
			 0x41800000,
			 0x40000000,
			 0x40c00000,

			 /// vpu register f9
			 0x41200000,
			 0x41c00000,
			 0x42000000,
			 0x41400000,
			 0x41880000,
			 0x41b00000,
			 0x41500000,
			 0x40000000,

			 /// vpu register f10
			 0x41b80000,
			 0x41300000,
			 0x41000000,
			 0x41d00000,
			 0x40a00000,
			 0x41600000,
			 0x41a00000,
			 0x41980000,

			 /// vpu register f11
			 0x40c00000,
			 0x41f00000,
			 0x40800000,
			 0x40800000,
			 0x41a00000,
			 0x40400000,
			 0x40a00000,
			 0x40c00000,

			 /// vpu register f12
			 0x41800000,
			 0x41a00000,
			 0x41a80000,
			 0x41a80000,
			 0x41a80000,
			 0x41600000,
			 0x40800000,
			 0x41900000,

			 /// vpu register f13
			 0x41800000,
			 0x41a80000,
			 0x42000000,
			 0x40a00000,
			 0x40400000,
			 0x3f800000,
			 0x41d80000,
			 0x3f800000,

			 /// vpu register f14
			 0x40400000,
			 0x41400000,
			 0x40800000,
			 0x41a80000,
			 0x41900000,
			 0x41c00000,
			 0x41e80000,
			 0x41c80000,

			 /// vpu register f15
			 0x41880000,
			 0x41e00000,
			 0x40000000,
			 0x41b80000,
			 0x41c00000,
			 0x41700000,
			 0x41700000,
			 0x40000000,

			 /// vpu register f16
			 0x41c80000,
			 0x41c80000,
			 0x41500000,
			 0x41880000,
			 0x41f00000,
			 0x41000000,
			 0x41d80000,
			 0x3f800000,

			 /// vpu register f17
			 0x41880000,
			 0x41200000,
			 0x41880000,
			 0x41d80000,
			 0x41e00000,
			 0x41300000,
			 0x41c00000,
			 0x40a00000,

			 /// vpu register f18
			 0x41980000,
			 0x41880000,
			 0x41900000,
			 0x40000000,
			 0x41900000,
			 0x41200000,
			 0x41c00000,
			 0x41500000,

			 /// vpu register f19
			 0x40a00000,
			 0x41a00000,
			 0x41400000,
			 0x41700000,
			 0x41c00000,
			 0x40c00000,
			 0x41e80000,
			 0x40400000,

			 /// vpu register f20
			 0x41e80000,
			 0x3f800000,
			 0x41d00000,
			 0x41700000,
			 0x41700000,
			 0x41e80000,
			 0x41700000,
			 0x41880000,

			 /// vpu register f21
			 0x41c00000,
			 0x41d80000,
			 0x41a80000,
			 0x41980000,
			 0x41e80000,
			 0x40e00000,
			 0x41f00000,
			 0x40800000,

			 /// vpu register f22
			 0x41880000,
			 0x42000000,
			 0x41c80000,
			 0x40000000,
			 0x41880000,
			 0x41900000,
			 0x41b80000,
			 0x41a80000,

			 /// vpu register f23
			 0x42000000,
			 0x41700000,
			 0x41900000,
			 0x41d80000,
			 0x41800000,
			 0x40c00000,
			 0x42000000,
			 0x40a00000,

			 /// vpu register f24
			 0x3f800000,
			 0x41400000,
			 0x41300000,
			 0x41000000,
			 0x41000000,
			 0x41800000,
			 0x41a00000,
			 0x41900000,

			 /// vpu register f25
			 0x41f80000,
			 0x41880000,
			 0x41980000,
			 0x41a00000,
			 0x41700000,
			 0x41800000,
			 0x41900000,
			 0x41200000,

			 /// vpu register f26
			 0x40c00000,
			 0x41300000,
			 0x40e00000,
			 0x41e80000,
			 0x40c00000,
			 0x41c80000,
			 0x41e80000,
			 0x3f800000,

			 /// vpu register f27
			 0x41e80000,
			 0x41700000,
			 0x41a00000,
			 0x41100000,
			 0x41700000,
			 0x42000000,
			 0x41100000,
			 0x41b00000,

			 /// vpu register f28
			 0x41000000,
			 0x41b80000,
			 0x41a80000,
			 0x41100000,
			 0x41200000,
			 0x40e00000,
			 0x41b80000,
			 0x40e00000,

			 /// vpu register f29
			 0x41c00000,
			 0x41900000,
			 0x41980000,
			 0x41e00000,
			 0x41d80000,
			 0x40800000,
			 0x40000000,
			 0x41a80000,

			 /// vpu register f30
			 0x41980000,
			 0x41c80000,
			 0x40400000,
			 0x41500000,
			 0x41a00000,
			 0x41d80000,
			 0x41900000,
			 0x41100000,

			 /// vpu register f31
			 0x41980000,
			 0x40e00000,
			 0x41700000,
			 0x41b80000,
			 0x41900000,
			 0x41f80000,
			 0x41c80000,
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
		"fslli.pi f20, f7, 11\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f17, f9, 8\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f6, f30, 14\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f11, f12, 6\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f22, f22, 14\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f8, f22, 6\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f9, f30, 2\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f19, f1, 0\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f22, f29, 9\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f24, f20, 5\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f16, f30, 7\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f8, f10, 3\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f29, f3, 3\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f14, f25, 14\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f3, f23, 3\n"                               ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f18, f21, 6\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f9, f30, 1\n"                               ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f19, f29, 13\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f9, f21, 5\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f11, f27, 9\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f9, f23, 1\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f6, f21, 10\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f17, f26, 3\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f28, f19, 0\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f25, f4, 1\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f13, f16, 8\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f19, f30, 14\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f18, f26, 14\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f16, f8, 7\n"                               ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f7, f11, 7\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f26, f9, 11\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f5, f17, 9\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f14, f20, 4\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f21, f30, 4\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f30, f9, 14\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f11, f28, 10\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f24, f30, 8\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f2, f8, 11\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f30, f1, 2\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f2, f0, 14\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f3, f7, 3\n"                                ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f20, f8, 12\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f28, f16, 6\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f10, f8, 1\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f3, f3, 14\n"                               ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f5, f5, 12\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f15, f2, 1\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f22, f28, 7\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f14, f12, 4\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f7, f21, 3\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f20, f7, 4\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f28, f7, 9\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f4, f16, 3\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f27, f16, 8\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f9, f19, 12\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f18, f6, 8\n"                               ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f16, f13, 9\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f20, f5, 6\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f7, f0, 6\n"                                ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f28, f18, 15\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f6, f25, 10\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f13, f14, 4\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f30, f14, 12\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f28, f0, 7\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f6, f17, 12\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f8, f24, 13\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f12, f27, 15\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f9, f6, 13\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f17, f28, 11\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f9, f23, 1\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f4, f20, 3\n"                               ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f22, f27, 9\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f10, f14, 14\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f10, f23, 12\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f24, f20, 8\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f25, f17, 8\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f11, f15, 9\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f11, f10, 13\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f18, f1, 15\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f20, f22, 9\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f28, f25, 2\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f13, f11, 1\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f29, f16, 9\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f23, f17, 11\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f25, f8, 8\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f4, f2, 0\n"                                ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f11, f7, 15\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f15, f19, 15\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f19, f9, 11\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f30, f7, 12\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f18, f11, 6\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f14, f29, 3\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f3, f29, 13\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f11, f1, 11\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f2, f22, 8\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f13, f26, 14\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f12, f17, 5\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f13, f11, 4\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fslli.pi f4, f2, 6\n"                                ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
