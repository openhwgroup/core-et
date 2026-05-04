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
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00000
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x0c600000,                                                  // Leading 1s:                                 /// 00008
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0000c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00010
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0001c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00020
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00024
			 0x7f800000,                                                  // inf                                         /// 00028
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0002c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00030
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00038
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00040
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00044
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00048
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0004c
			 0x3f028f5c,                                                  // 0.51                                        /// 00050
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00054
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00058
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00060
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00064
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00068
			 0x00011111,                                                  // 9.7958E-41                                  /// 0006c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00070
			 0x0e000007,                                                  // Trailing 1s:                                /// 00074
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00078
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0007c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x0e000003,                                                  // Trailing 1s:                                /// 00084
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0008c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00090
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00094
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00098
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 000ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x4b000000,                                                  // 8388608.0                                   /// 000cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00104
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00108
			 0x55555555,                                                  // 4 random values                             /// 0010c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00110
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00118
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00120
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00128
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0012c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00130
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00134
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0013c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00140
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00144
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00148
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0014c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x00011111,                                                  // 9.7958E-41                                  /// 00154
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00158
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00160
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00164
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x00011111,                                                  // 9.7958E-41                                  /// 0016c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00170
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00174
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00178
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00184
			 0x00000000,                                                  // zero                                        /// 00188
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0018c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00190
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x00011111,                                                  // 9.7958E-41                                  /// 00198
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0019c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001f8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00204
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00210
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00218
			 0xff800000,                                                  // -inf                                        /// 0021c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00220
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00224
			 0x0c600000,                                                  // Leading 1s:                                 /// 00228
			 0x7fc00001,                                                  // signaling NaN                               /// 0022c
			 0x3f028f5c,                                                  // 0.51                                        /// 00230
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00234
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00240
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00248
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00250
			 0x0c700000,                                                  // Leading 1s:                                 /// 00254
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0025c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00260
			 0x0e000007,                                                  // Trailing 1s:                                /// 00264
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00268
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00278
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00280
			 0x7f800000,                                                  // inf                                         /// 00284
			 0x0c600000,                                                  // Leading 1s:                                 /// 00288
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0028c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00294
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002dc
			 0x80000000,                                                  // -zero                                       /// 002e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00304
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00308
			 0x0e000001,                                                  // Trailing 1s:                                /// 0030c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00310
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00314
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00318
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0031c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00320
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00324
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00328
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0032c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00330
			 0x80000000,                                                  // -zero                                       /// 00334
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00338
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00340
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00344
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00354
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0035c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00360
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00364
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0036c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00370
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00374
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0037c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00380
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00388
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0038c
			 0xff800000,                                                  // -inf                                        /// 00390
			 0x0c780000,                                                  // Leading 1s:                                 /// 00394
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00398
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0039c
			 0x55555555,                                                  // 4 random values                             /// 003a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003d8
			 0xcb000000,                                                  // -8388608.0                                  /// 003dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003e8
			 0x33333333,                                                  // 4 random values                             /// 003ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00400
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00404
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0040c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00410
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00418
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0042c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00430
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x0c780000,                                                  // Leading 1s:                                 /// 0043c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00440
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00450
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00458
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0045c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00460
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00464
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00468
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00474
			 0x80011111,                                                  // -9.7958E-41                                 /// 00478
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0047c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00480
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00484
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00488
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00490
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0049c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0xbf028f5c,                                                  // -0.51                                       /// 004bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00500
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00504
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00510
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00514
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00518
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0051c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0052c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00530
			 0x7f800000,                                                  // inf                                         /// 00534
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00538
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00540
			 0x0c400000,                                                  // Leading 1s:                                 /// 00544
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00548
			 0x7fc00001,                                                  // signaling NaN                               /// 0054c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0055c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00560
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00564
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x80000000,                                                  // -zero                                       /// 0056c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00570
			 0x80000000,                                                  // -zero                                       /// 00574
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00578
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00594
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00598
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0059c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005bc
			 0xffc00001,                                                  // -signaling NaN                              /// 005c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0xffc00001,                                                  // -signaling NaN                              /// 005f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00600
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00608
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0060c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00610
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00618
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0061c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x0e000001,                                                  // Trailing 1s:                                /// 00624
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00628
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00634
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00638
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00640
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0064c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00658
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0065c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00660
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00664
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00668
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0066c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00684
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00688
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0068c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00690
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006a0
			 0x80000000,                                                  // -zero                                       /// 006a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 006b8
			 0x00000000,                                                  // zero                                        /// 006bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00704
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x7f800000,                                                  // inf                                         /// 0070c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00710
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00714
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00718
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0071c
			 0x55555555,                                                  // 4 random values                             /// 00720
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00724
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00728
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00730
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00734
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00738
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00740
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00744
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0074c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00750
			 0xcb000000,                                                  // -8388608.0                                  /// 00754
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00760
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00764
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00768
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00770
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0xff800000,                                                  // -inf                                        /// 00778
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x3f028f5c,                                                  // 0.51                                        /// 00780
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00784
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00788
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0078c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 007b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 007bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007c8
			 0xbf028f5c,                                                  // -0.51                                       /// 007cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007ec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007f4
			 0xffc00001,                                                  // -signaling NaN                              /// 007f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00804
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00808
			 0x00011111,                                                  // 9.7958E-41                                  /// 0080c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00810
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00814
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0081c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00820
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00824
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00828
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0082c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00830
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00834
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00840
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00844
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x7f800000,                                                  // inf                                         /// 0084c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00854
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00858
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00860
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00864
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00874
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00878
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00888
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0088c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00890
			 0x0e000001,                                                  // Trailing 1s:                                /// 00894
			 0xff800000,                                                  // -inf                                        /// 00898
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0089c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008bc
			 0x33333333,                                                  // 4 random values                             /// 008c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x3f028f5c,                                                  // 0.51                                        /// 008e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00900
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x55555555,                                                  // 4 random values                             /// 00910
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0091c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00920
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00924
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00928
			 0x00011111,                                                  // 9.7958E-41                                  /// 0092c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00930
			 0x0c400000,                                                  // Leading 1s:                                 /// 00934
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0093c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00940
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00944
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00948
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0094c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00950
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00954
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00960
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00964
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00968
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0096c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00970
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00974
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00978
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0097c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0xbf028f5c,                                                  // -0.51                                       /// 00988
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0098c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00998
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0099c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0xffc00001,                                                  // -signaling NaN                              /// 009a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009f4
			 0x7f800000,                                                  // inf                                         /// 009f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a00
			 0x33333333,                                                  // 4 random values                             /// 00a04
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a10
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0xbf028f5c,                                                  // -0.51                                       /// 00a18
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a1c
			 0x33333333,                                                  // 4 random values                             /// 00a20
			 0x33333333,                                                  // 4 random values                             /// 00a24
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a28
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a34
			 0x4b000000,                                                  // 8388608.0                                   /// 00a38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a3c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a48
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a54
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a64
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a80
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a88
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a94
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aa0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00aa4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00aac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ab0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ab8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ac0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ac4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ac8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00acc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ad0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ad4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ad8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00adc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ae4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ae8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00af0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00af4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0xbf028f5c,                                                  // -0.51                                       /// 00afc
			 0x80000000,                                                  // -zero                                       /// 00b00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b04
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b08
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b14
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b18
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b24
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b30
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b34
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b38
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b3c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b40
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b4c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b54
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b58
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b68
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b7c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b90
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ba8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bb8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bbc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bc0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bc4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00bd0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bd4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bdc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00be0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00be4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00be8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bf4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bf8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c04
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c18
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c1c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c3c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c44
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x3f028f5c,                                                  // 0.51                                        /// 00c54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c60
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c68
			 0x3f028f5c,                                                  // 0.51                                        /// 00c6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c78
			 0x4b000000,                                                  // 8388608.0                                   /// 00c7c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c80
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c84
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c98
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c9c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ca8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cb0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cb8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cbc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cd0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cd4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ce0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ce4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ce8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cf4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cf8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cfc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d00
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d04
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d18
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d20
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d28
			 0xffc00001,                                                  // -signaling NaN                              /// 00d2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d30
			 0x3f028f5c,                                                  // 0.51                                        /// 00d34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d3c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d54
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d5c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d6c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d80
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d88
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d90
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d94
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d9c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00da0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00da4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00da8
			 0x00000000,                                                  // zero                                        /// 00dac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00db0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00db4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00db8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dbc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dc4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dc8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dcc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dd0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dd4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dd8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00de8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00df0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00df4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00df8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dfc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e04
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e08
			 0x80000000,                                                  // -zero                                       /// 00e0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e60
			 0x7f800000,                                                  // inf                                         /// 00e64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e6c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e70
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e78
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e90
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e94
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ea4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00eac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00eb0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00eb4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ebc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ec0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ec4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ec8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ed0
			 0x33333333,                                                  // 4 random values                             /// 00ed4
			 0x7f800000,                                                  // inf                                         /// 00ed8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00edc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ee0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ef0
			 0x7f800000,                                                  // inf                                         /// 00ef4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ef8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f00
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x33333333,                                                  // 4 random values                             /// 00f08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f14
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f18
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f20
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00f30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f34
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f48
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f50
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f58
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f60
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f70
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x55555555,                                                  // 4 random values                             /// 00f84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f88
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f9c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fa0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fa8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fb4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fbc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fc8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fd0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fd4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fe4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fe8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ff0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ff8
			 0x00800003 // min norm + 3ulp                               // subnormals +ve                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00000
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00004
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00008
			 0xbf028f5c,                                                  // -0.51                                       /// 0000c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00010
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00018
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0001c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00024
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00028
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00034
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00038
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0003c
			 0xcb000000,                                                  // -8388608.0                                  /// 00040
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00044
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0004c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00050
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00054
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0005c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00064
			 0x0c600000,                                                  // Leading 1s:                                 /// 00068
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0006c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00070
			 0xcb000000,                                                  // -8388608.0                                  /// 00074
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00078
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0007c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00080
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00084
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00088
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0008c
			 0x80000000,                                                  // -zero                                       /// 00090
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000a0
			 0xbf028f5c,                                                  // -0.51                                       /// 000a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000b0
			 0x00000000,                                                  // zero                                        /// 000b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000bc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000e4
			 0xcb000000,                                                  // -8388608.0                                  /// 000e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x0c600000,                                                  // Leading 1s:                                 /// 00108
			 0x00011111,                                                  // 9.7958E-41                                  /// 0010c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00110
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00118
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0011c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00120
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00124
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00128
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0013c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00144
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00148
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0014c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00150
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00154
			 0x0c780000,                                                  // Leading 1s:                                 /// 00158
			 0x00000000,                                                  // zero                                        /// 0015c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00160
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00164
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0016c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00170
			 0x7fc00001,                                                  // signaling NaN                               /// 00174
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00178
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0017c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00180
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00184
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00188
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80000000,                                                  // -zero                                       /// 00190
			 0xffc00001,                                                  // -signaling NaN                              /// 00194
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0019c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001b8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 001f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00200
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00204
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00210
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00214
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x00011111,                                                  // 9.7958E-41                                  /// 0021c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00220
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00224
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x33333333,                                                  // 4 random values                             /// 0022c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00230
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00234
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00238
			 0x7fc00001,                                                  // signaling NaN                               /// 0023c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00240
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00244
			 0x0c780000,                                                  // Leading 1s:                                 /// 00248
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0024c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00250
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00258
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0025c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00260
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00264
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00268
			 0x0e000003,                                                  // Trailing 1s:                                /// 0026c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00270
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00274
			 0x33333333,                                                  // 4 random values                             /// 00278
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0027c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00284
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00288
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0xffc00001,                                                  // -signaling NaN                              /// 00294
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00298
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0c600000,                                                  // Leading 1s:                                 /// 002a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x33333333,                                                  // 4 random values                             /// 002c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002dc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x33333333,                                                  // 4 random values                             /// 00300
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00308
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00318
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0031c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00324
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00330
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00334
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00338
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0xbf028f5c,                                                  // -0.51                                       /// 00340
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00344
			 0x4b000000,                                                  // 8388608.0                                   /// 00348
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00350
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00354
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0035c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00364
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00368
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0036c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00370
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00378
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0037c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00380
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00384
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00388
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0038c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00390
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00394
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 003a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00400
			 0x0e000007,                                                  // Trailing 1s:                                /// 00404
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00408
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00410
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00418
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00420
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00424
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0042c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00430
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00434
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00438
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0043c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00440
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00444
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00448
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0044c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0045c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00464
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00470
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00474
			 0xbf028f5c,                                                  // -0.51                                       /// 00478
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0047c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00484
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00488
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00490
			 0x0c780000,                                                  // Leading 1s:                                 /// 00494
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004a0
			 0x33333333,                                                  // 4 random values                             /// 004a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x7fc00001,                                                  // signaling NaN                               /// 004d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004e0
			 0xcb000000,                                                  // -8388608.0                                  /// 004e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00500
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00504
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00508
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0050c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00514
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00518
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0051c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00534
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0053c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00540
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00554
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00558
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00560
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00564
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00568
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00574
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x7fc00001,                                                  // signaling NaN                               /// 00584
			 0x80000000,                                                  // -zero                                       /// 00588
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0058c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00590
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00594
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0059c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 005f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00600
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00608
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00610
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00614
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0061c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00620
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00624
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0062c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00630
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0063c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00640
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00644
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0064c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00650
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00654
			 0x0c700000,                                                  // Leading 1s:                                 /// 00658
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00660
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00664
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0066c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00670
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00674
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0067c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0068c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00690
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00694
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00698
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006d4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x7fc00001,                                                  // signaling NaN                               /// 006dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 006f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00700
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0070c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00710
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00714
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00720
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00724
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00728
			 0x3f028f5c,                                                  // 0.51                                        /// 0072c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00730
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00734
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0073c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00740
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00744
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0074c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00750
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00754
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00758
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0075c
			 0x7fc00001,                                                  // signaling NaN                               /// 00760
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00764
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00768
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0076c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00770
			 0x80000000,                                                  // -zero                                       /// 00774
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00778
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0077c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00780
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00784
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00788
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0078c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00798
			 0x4b000000,                                                  // 8388608.0                                   /// 0079c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 007d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x4b000000,                                                  // 8388608.0                                   /// 007e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007ec
			 0x4b000000,                                                  // 8388608.0                                   /// 007f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x4b000000,                                                  // 8388608.0                                   /// 007fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00804
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0080c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00810
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00818
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0081c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00824
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00828
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0082c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00830
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00834
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00838
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0083c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00840
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00011111,                                                  // 9.7958E-41                                  /// 00848
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0084c
			 0x3f028f5c,                                                  // 0.51                                        /// 00850
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00854
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00860
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00864
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00874
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00878
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0087c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00880
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00888
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0088c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00890
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00894
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x0c780000,                                                  // Leading 1s:                                 /// 0089c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008b4
			 0x7fc00001,                                                  // signaling NaN                               /// 008b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008d0
			 0xffc00001,                                                  // -signaling NaN                              /// 008d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x80000000,                                                  // -zero                                       /// 00908
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0090c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00910
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00918
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0091c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00924
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00928
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00930
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00934
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00938
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0093c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00940
			 0x3f028f5c,                                                  // 0.51                                        /// 00944
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00950
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00964
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00968
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0096c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00970
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0097c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00980
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00984
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00988
			 0x0e000007,                                                  // Trailing 1s:                                /// 0098c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00990
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00998
			 0x7f800000,                                                  // inf                                         /// 0099c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 009a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 009b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x00000000,                                                  // zero                                        /// 009dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a0c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a1c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a24
			 0x80000000,                                                  // -zero                                       /// 00a28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a44
			 0x7fc00001,                                                  // signaling NaN                               /// 00a48
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a54
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a60
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a84
			 0xcb000000,                                                  // -8388608.0                                  /// 00a88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a90
			 0xffc00001,                                                  // -signaling NaN                              /// 00a94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x55555555,                                                  // 4 random values                             /// 00a9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00aa4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x3f028f5c,                                                  // 0.51                                        /// 00aac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ab4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ab8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00abc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ac0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ac4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ac8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00acc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ad4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ad8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ae4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ae8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00af4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00af8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00afc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b10
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b14
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b18
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b28
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b30
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b38
			 0x3f028f5c,                                                  // 0.51                                        /// 00b3c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b40
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b44
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b48
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b50
			 0xbf028f5c,                                                  // -0.51                                       /// 00b54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b5c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b60
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b64
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x4b000000,                                                  // 8388608.0                                   /// 00b70
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b78
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b7c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b84
			 0xbf028f5c,                                                  // -0.51                                       /// 00b88
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ba0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ba4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x00000000,                                                  // zero                                        /// 00bac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bb0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bb4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bbc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bc4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x80000000,                                                  // -zero                                       /// 00bcc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bd8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00be4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bf8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bfc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c04
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c08
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c0c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c10
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c1c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c28
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c38
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c58
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c60
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c84
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c88
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c94
			 0x7fc00001,                                                  // signaling NaN                               /// 00c98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00ca0
			 0xff800000,                                                  // -inf                                        /// 00ca4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ca8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cb0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cb4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cb8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cc0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0xbf028f5c,                                                  // -0.51                                       /// 00cc8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ccc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00000000,                                                  // zero                                        /// 00cd4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cd8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cdc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ce0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ce4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ce8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cf0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cf4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cf8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d00
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x3f028f5c,                                                  // 0.51                                        /// 00d1c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d48
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d4c
			 0x00000000,                                                  // zero                                        /// 00d50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d54
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d58
			 0xff800000,                                                  // -inf                                        /// 00d5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d60
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d68
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d70
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d78
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d7c
			 0x7f800000,                                                  // inf                                         /// 00d80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d84
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d88
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d8c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d94
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00db0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00db8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dc0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dc4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dcc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dd4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dd8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00de0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0xcb000000,                                                  // -8388608.0                                  /// 00df0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00df4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00df8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e00
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e04
			 0x7fc00001,                                                  // signaling NaN                               /// 00e08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e10
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e14
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e1c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e28
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e30
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0xff800000,                                                  // -inf                                        /// 00e38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e3c
			 0xff800000,                                                  // -inf                                        /// 00e40
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e4c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e60
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e7c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e80
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e84
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e90
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e94
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e98
			 0x80000000,                                                  // -zero                                       /// 00e9c
			 0x80000000,                                                  // -zero                                       /// 00ea0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ea4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ea8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00eac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00eb0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00eb4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00eb8
			 0xbf028f5c,                                                  // -0.51                                       /// 00ebc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ec0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ec4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ed8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00edc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ee4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x3f028f5c,                                                  // 0.51                                        /// 00eec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ef0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ef4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ef8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00efc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f04
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f18
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f28
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f40
			 0x3f028f5c,                                                  // 0.51                                        /// 00f44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f50
			 0x4b000000,                                                  // 8388608.0                                   /// 00f54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f58
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f5c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f64
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x3f028f5c,                                                  // 0.51                                        /// 00f6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f7c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f80
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f84
			 0x55555555,                                                  // 4 random values                             /// 00f88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fa4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fa8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fb0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fb4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fb8
			 0x55555555,                                                  // 4 random values                             /// 00fbc
			 0x00000000,                                                  // zero                                        /// 00fc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fc4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fc8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fd4
			 0xff800000,                                                  // -inf                                        /// 00fd8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fdc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fe0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fe8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ff0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ff4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0f7ffffe                                                  // all bit of mantissa set upto -3ulp          /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xf2bcfaaa, /// 0x0
			 0xd8cf4dfd, /// 0x4
			 0xb813c32e, /// 0x8
			 0x34a0e04a, /// 0xc
			 0x78431a22, /// 0x10
			 0x8dfca629, /// 0x14
			 0xfc0787eb, /// 0x18
			 0x368ef960, /// 0x1c
			 0xd13dbc01, /// 0x20
			 0x731de6d8, /// 0x24
			 0xd998cca8, /// 0x28
			 0xd135afa2, /// 0x2c
			 0x85202822, /// 0x30
			 0x4b025207, /// 0x34
			 0x1ffb8099, /// 0x38
			 0x56756e83, /// 0x3c
			 0x9adf8c28, /// 0x40
			 0xa28c1296, /// 0x44
			 0x4e78de2e, /// 0x48
			 0xbf715c5e, /// 0x4c
			 0x4c421af9, /// 0x50
			 0xbc97d6d0, /// 0x54
			 0x3b288b6b, /// 0x58
			 0x72298497, /// 0x5c
			 0x564aa67d, /// 0x60
			 0x0ceac6c9, /// 0x64
			 0x9a98c9bc, /// 0x68
			 0xf709a70a, /// 0x6c
			 0x1470c4be, /// 0x70
			 0x33440349, /// 0x74
			 0x8914dc42, /// 0x78
			 0x9d0051a3, /// 0x7c
			 0x9673a28d, /// 0x80
			 0x5e189928, /// 0x84
			 0x71302514, /// 0x88
			 0x503688e3, /// 0x8c
			 0xebf94e2e, /// 0x90
			 0xaa5e9c9e, /// 0x94
			 0xb169c6e2, /// 0x98
			 0xa41f62af, /// 0x9c
			 0x30301596, /// 0xa0
			 0x4c0b7ff7, /// 0xa4
			 0x255c2cc9, /// 0xa8
			 0x96b08c36, /// 0xac
			 0xeea19064, /// 0xb0
			 0x527c1233, /// 0xb4
			 0xa6f0969c, /// 0xb8
			 0xbc388f36, /// 0xbc
			 0xe71c7d02, /// 0xc0
			 0x3f955961, /// 0xc4
			 0x30816b63, /// 0xc8
			 0xab9c303a, /// 0xcc
			 0x5ec1afd5, /// 0xd0
			 0x801352bc, /// 0xd4
			 0x177095b8, /// 0xd8
			 0x067fe7e8, /// 0xdc
			 0x003603b6, /// 0xe0
			 0x0692d88f, /// 0xe4
			 0x5266cd0d, /// 0xe8
			 0x129d20a5, /// 0xec
			 0x9f2ec46f, /// 0xf0
			 0x3049c17a, /// 0xf4
			 0x0389e67d, /// 0xf8
			 0x485669fb, /// 0xfc
			 0xc7346979, /// 0x100
			 0x00065634, /// 0x104
			 0x9e851ea1, /// 0x108
			 0x1b711f92, /// 0x10c
			 0x2d601ad6, /// 0x110
			 0x22309a8f, /// 0x114
			 0xeaa728cf, /// 0x118
			 0x5f32120b, /// 0x11c
			 0xc530f7c1, /// 0x120
			 0x9c3b2958, /// 0x124
			 0x7a7751d0, /// 0x128
			 0x875d335d, /// 0x12c
			 0x95a23425, /// 0x130
			 0xaf55ace9, /// 0x134
			 0x53ee606e, /// 0x138
			 0x37b86e1c, /// 0x13c
			 0x8f6fd462, /// 0x140
			 0xc314b7b1, /// 0x144
			 0x535bad49, /// 0x148
			 0xf66d6121, /// 0x14c
			 0x9b794f18, /// 0x150
			 0xda56f845, /// 0x154
			 0x0ee6dd69, /// 0x158
			 0x71b7e880, /// 0x15c
			 0xf7daa9ad, /// 0x160
			 0x8257b050, /// 0x164
			 0x35977380, /// 0x168
			 0x6cd8aaae, /// 0x16c
			 0x1773ca9b, /// 0x170
			 0x366ce643, /// 0x174
			 0xb9ce4056, /// 0x178
			 0xeec68fcf, /// 0x17c
			 0xd834a51e, /// 0x180
			 0x6cd8b03f, /// 0x184
			 0xabeab9e0, /// 0x188
			 0x9f50be0b, /// 0x18c
			 0x9ac1e411, /// 0x190
			 0x1b724fe6, /// 0x194
			 0xa4db3205, /// 0x198
			 0xa01e6783, /// 0x19c
			 0x9cc9bc39, /// 0x1a0
			 0xc92a2046, /// 0x1a4
			 0xc6477877, /// 0x1a8
			 0x7f2eb907, /// 0x1ac
			 0x596e6c73, /// 0x1b0
			 0xd7c09d96, /// 0x1b4
			 0x2d08a3c2, /// 0x1b8
			 0xbf4525f0, /// 0x1bc
			 0xa1275526, /// 0x1c0
			 0xb4710ebf, /// 0x1c4
			 0xbeb34f3d, /// 0x1c8
			 0x28690987, /// 0x1cc
			 0x3cab80d4, /// 0x1d0
			 0xebdae2fd, /// 0x1d4
			 0x6b7ad23b, /// 0x1d8
			 0x21b9d46d, /// 0x1dc
			 0x297a04a8, /// 0x1e0
			 0x11182acd, /// 0x1e4
			 0x8e704fa3, /// 0x1e8
			 0x52231020, /// 0x1ec
			 0x0caa9567, /// 0x1f0
			 0x799b4d86, /// 0x1f4
			 0xb22b40e8, /// 0x1f8
			 0x535e9d64, /// 0x1fc
			 0xe2b36d7d, /// 0x200
			 0xa371fd11, /// 0x204
			 0xc290ff9a, /// 0x208
			 0x19220943, /// 0x20c
			 0x8be67f7b, /// 0x210
			 0xe1758f25, /// 0x214
			 0x5244316c, /// 0x218
			 0x87f0202f, /// 0x21c
			 0x09534feb, /// 0x220
			 0xb99b52cf, /// 0x224
			 0x20540747, /// 0x228
			 0xdc64ab46, /// 0x22c
			 0x7b40931b, /// 0x230
			 0xc9aeb4d4, /// 0x234
			 0x00d72aae, /// 0x238
			 0x3142c258, /// 0x23c
			 0xf5de24a1, /// 0x240
			 0x6ae81106, /// 0x244
			 0x094d1186, /// 0x248
			 0xf5d25d88, /// 0x24c
			 0x00404e02, /// 0x250
			 0x2b1fb054, /// 0x254
			 0x9f7b3abf, /// 0x258
			 0x52dd53d6, /// 0x25c
			 0xb8c2deb1, /// 0x260
			 0x9d6eb7ab, /// 0x264
			 0x1afbc9bd, /// 0x268
			 0x84bd4821, /// 0x26c
			 0x17abd5d6, /// 0x270
			 0x175b609c, /// 0x274
			 0xcf52fa8d, /// 0x278
			 0x594b5762, /// 0x27c
			 0xc5707a73, /// 0x280
			 0xf37d41b0, /// 0x284
			 0x8ca4de23, /// 0x288
			 0x02e75e6d, /// 0x28c
			 0xc726d883, /// 0x290
			 0xb1cb87dd, /// 0x294
			 0x410b4ae9, /// 0x298
			 0x06d8b1b3, /// 0x29c
			 0xe12c8e0c, /// 0x2a0
			 0x215a4b1b, /// 0x2a4
			 0x524fea67, /// 0x2a8
			 0x29fc3dd5, /// 0x2ac
			 0x64cc7f10, /// 0x2b0
			 0x48a63c0c, /// 0x2b4
			 0xfb052654, /// 0x2b8
			 0x942481e3, /// 0x2bc
			 0x9d2dba0a, /// 0x2c0
			 0xd7a15c73, /// 0x2c4
			 0xd25df21e, /// 0x2c8
			 0x8a87e57c, /// 0x2cc
			 0x925148d8, /// 0x2d0
			 0x1335da96, /// 0x2d4
			 0x684e13a0, /// 0x2d8
			 0x9811f46f, /// 0x2dc
			 0xdfb526b1, /// 0x2e0
			 0x2578b0b2, /// 0x2e4
			 0x79cabc4a, /// 0x2e8
			 0xa3ddcfc7, /// 0x2ec
			 0x85d267cf, /// 0x2f0
			 0x6ec2fdf1, /// 0x2f4
			 0x26fd521f, /// 0x2f8
			 0x96dd438a, /// 0x2fc
			 0x3f5108d3, /// 0x300
			 0xb4fc85cd, /// 0x304
			 0xc5e7440c, /// 0x308
			 0x2c5c8b5b, /// 0x30c
			 0x5ab0ee75, /// 0x310
			 0xf093adcc, /// 0x314
			 0xc27b92f0, /// 0x318
			 0x7f65070e, /// 0x31c
			 0x20eb6172, /// 0x320
			 0x4634bd32, /// 0x324
			 0x2ff364ca, /// 0x328
			 0x047b136a, /// 0x32c
			 0xe6723dd5, /// 0x330
			 0xedff1003, /// 0x334
			 0x22c3f3ca, /// 0x338
			 0x5ab744fe, /// 0x33c
			 0x653f45a7, /// 0x340
			 0xdc869d51, /// 0x344
			 0x7dd9fb17, /// 0x348
			 0x69b874ed, /// 0x34c
			 0x0da669a0, /// 0x350
			 0xd16f43ad, /// 0x354
			 0xe192a2f4, /// 0x358
			 0x78307d47, /// 0x35c
			 0xb32b2e32, /// 0x360
			 0x07c01fff, /// 0x364
			 0x64b5d61d, /// 0x368
			 0xd6826938, /// 0x36c
			 0x52febfe0, /// 0x370
			 0xcba4be4c, /// 0x374
			 0x85d6dd16, /// 0x378
			 0x4b18391a, /// 0x37c
			 0x1797a373, /// 0x380
			 0xcd32c178, /// 0x384
			 0x1d132995, /// 0x388
			 0x3de2fc8d, /// 0x38c
			 0x27cc1434, /// 0x390
			 0xc1f553af, /// 0x394
			 0x3a108ba2, /// 0x398
			 0x646f243f, /// 0x39c
			 0x1333ede3, /// 0x3a0
			 0x8d302f36, /// 0x3a4
			 0xd18a5879, /// 0x3a8
			 0x4431a9cb, /// 0x3ac
			 0xcfd0d633, /// 0x3b0
			 0x0adeba8d, /// 0x3b4
			 0xa4338375, /// 0x3b8
			 0x2337018f, /// 0x3bc
			 0x8056ca96, /// 0x3c0
			 0xa5498a85, /// 0x3c4
			 0x7c57ddc5, /// 0x3c8
			 0xb8024f64, /// 0x3cc
			 0xa16fb684, /// 0x3d0
			 0xef32be09, /// 0x3d4
			 0x2144fdf3, /// 0x3d8
			 0xd192d6e8, /// 0x3dc
			 0x8bcec07e, /// 0x3e0
			 0x5d37f933, /// 0x3e4
			 0x0730f019, /// 0x3e8
			 0x7bba3de3, /// 0x3ec
			 0x589020a5, /// 0x3f0
			 0x3bc0c715, /// 0x3f4
			 0xdad01140, /// 0x3f8
			 0xc8aeb779, /// 0x3fc
			 0x39bf0599, /// 0x400
			 0xab048191, /// 0x404
			 0x16e01455, /// 0x408
			 0xd1c09b80, /// 0x40c
			 0x74972ad3, /// 0x410
			 0x97d1e490, /// 0x414
			 0xf6042ab9, /// 0x418
			 0x57b42d1a, /// 0x41c
			 0x8d0632b6, /// 0x420
			 0xea6ee7af, /// 0x424
			 0xec3fe0f4, /// 0x428
			 0x1682ba66, /// 0x42c
			 0x63fdc649, /// 0x430
			 0x2a5aaeec, /// 0x434
			 0x45184910, /// 0x438
			 0xed838bb2, /// 0x43c
			 0x330aae29, /// 0x440
			 0x127b4024, /// 0x444
			 0xb0c90695, /// 0x448
			 0xd5177e3b, /// 0x44c
			 0xc8553046, /// 0x450
			 0xe14edcc1, /// 0x454
			 0xbed79100, /// 0x458
			 0xbbc40d2d, /// 0x45c
			 0xf259e9a1, /// 0x460
			 0x4435bf22, /// 0x464
			 0xa2074cdb, /// 0x468
			 0xe4462509, /// 0x46c
			 0x5d91edff, /// 0x470
			 0x6d52bd7b, /// 0x474
			 0xb5e91b5a, /// 0x478
			 0xc2a9904f, /// 0x47c
			 0x89b4d13a, /// 0x480
			 0x1fad42dd, /// 0x484
			 0x202581fe, /// 0x488
			 0x688420a4, /// 0x48c
			 0x5185b61a, /// 0x490
			 0x4bdf156e, /// 0x494
			 0x9e4b07f1, /// 0x498
			 0x28a71f3c, /// 0x49c
			 0x38af8b3d, /// 0x4a0
			 0xff127b27, /// 0x4a4
			 0x8628ed5a, /// 0x4a8
			 0x643b3ff8, /// 0x4ac
			 0xf761a7be, /// 0x4b0
			 0x6abe25f8, /// 0x4b4
			 0x4cc4e5c9, /// 0x4b8
			 0xf64d1a3d, /// 0x4bc
			 0xff5104a3, /// 0x4c0
			 0x3f96477b, /// 0x4c4
			 0x2ed4546e, /// 0x4c8
			 0x6b18a84f, /// 0x4cc
			 0xbb1384ea, /// 0x4d0
			 0x8a59b13b, /// 0x4d4
			 0xd949f0e8, /// 0x4d8
			 0xf0ccc4fa, /// 0x4dc
			 0xc5c5fe17, /// 0x4e0
			 0x35b65b95, /// 0x4e4
			 0xc70f0c7a, /// 0x4e8
			 0x487c7bfa, /// 0x4ec
			 0xf7cfd14d, /// 0x4f0
			 0x6097747d, /// 0x4f4
			 0x27a1e6c9, /// 0x4f8
			 0x699f1800, /// 0x4fc
			 0x08b87521, /// 0x500
			 0xc0c46e92, /// 0x504
			 0x1b4113a5, /// 0x508
			 0xb9c70d4e, /// 0x50c
			 0x5954c945, /// 0x510
			 0x1b6d088d, /// 0x514
			 0xfdb1f3cd, /// 0x518
			 0xf1d77e9c, /// 0x51c
			 0x25ee333d, /// 0x520
			 0x982b0226, /// 0x524
			 0xfcc10453, /// 0x528
			 0x014ae2f1, /// 0x52c
			 0x7abdd88b, /// 0x530
			 0xdb8b3b85, /// 0x534
			 0xb2ccb3b7, /// 0x538
			 0xcb84d05a, /// 0x53c
			 0x45fdedb7, /// 0x540
			 0x6452a47a, /// 0x544
			 0x2e3dc724, /// 0x548
			 0x38c4579a, /// 0x54c
			 0xf24d95b2, /// 0x550
			 0xf6289a39, /// 0x554
			 0x2e9e97eb, /// 0x558
			 0x830d6a30, /// 0x55c
			 0x8016744a, /// 0x560
			 0xaa6d04db, /// 0x564
			 0x9b465741, /// 0x568
			 0xebd5bd45, /// 0x56c
			 0xa55f162f, /// 0x570
			 0x6fd8afb2, /// 0x574
			 0xfbd385a3, /// 0x578
			 0xbd636043, /// 0x57c
			 0x1854b6ec, /// 0x580
			 0x93e35554, /// 0x584
			 0x8f543987, /// 0x588
			 0x5ecad822, /// 0x58c
			 0x2ba0f3cf, /// 0x590
			 0xc844186a, /// 0x594
			 0xd42ad0bd, /// 0x598
			 0xa248932a, /// 0x59c
			 0xf3bcff2c, /// 0x5a0
			 0xbdb0c9b8, /// 0x5a4
			 0x07a0e77c, /// 0x5a8
			 0x7dbf5afb, /// 0x5ac
			 0xd7a82e3c, /// 0x5b0
			 0x30ca2fd6, /// 0x5b4
			 0x17e04725, /// 0x5b8
			 0xc4fdba3a, /// 0x5bc
			 0xd6d88857, /// 0x5c0
			 0xb4c46c2b, /// 0x5c4
			 0x5cd246f8, /// 0x5c8
			 0x6fe65b14, /// 0x5cc
			 0x499fa0bd, /// 0x5d0
			 0x4b303df1, /// 0x5d4
			 0x359e2d73, /// 0x5d8
			 0xc4517f98, /// 0x5dc
			 0x83fbb155, /// 0x5e0
			 0x56a45c7a, /// 0x5e4
			 0x7c330beb, /// 0x5e8
			 0x7e69bf32, /// 0x5ec
			 0xbd47ae6e, /// 0x5f0
			 0xdfa24b65, /// 0x5f4
			 0x0a6107f6, /// 0x5f8
			 0x3537c717, /// 0x5fc
			 0x2850064a, /// 0x600
			 0x974b1fca, /// 0x604
			 0x01aad6fb, /// 0x608
			 0x74b499f8, /// 0x60c
			 0x013eedfb, /// 0x610
			 0x2c5d3850, /// 0x614
			 0x736aea1f, /// 0x618
			 0x8c942c83, /// 0x61c
			 0xaadf5d4a, /// 0x620
			 0xb7066b83, /// 0x624
			 0x5815fd76, /// 0x628
			 0x1f0988d9, /// 0x62c
			 0xaae5fb77, /// 0x630
			 0x36c3e29e, /// 0x634
			 0xecd6f343, /// 0x638
			 0x32182289, /// 0x63c
			 0x777775b3, /// 0x640
			 0x8d8b9877, /// 0x644
			 0xe4fa5de8, /// 0x648
			 0x3165cc3f, /// 0x64c
			 0x76ce1ed8, /// 0x650
			 0xdd4479c0, /// 0x654
			 0xc0d07ffc, /// 0x658
			 0xf5703715, /// 0x65c
			 0x740c4523, /// 0x660
			 0x6a17ab0c, /// 0x664
			 0x770594de, /// 0x668
			 0xd5df2de9, /// 0x66c
			 0xb3daf278, /// 0x670
			 0xd2f40822, /// 0x674
			 0x937ee82c, /// 0x678
			 0x1dffa38f, /// 0x67c
			 0x38b771f7, /// 0x680
			 0x2f61f577, /// 0x684
			 0x422ca13a, /// 0x688
			 0xce25b452, /// 0x68c
			 0x01cedea3, /// 0x690
			 0x8a75d8e9, /// 0x694
			 0xbd5d8efa, /// 0x698
			 0xead3fa37, /// 0x69c
			 0xd2c13c17, /// 0x6a0
			 0x8da6e20a, /// 0x6a4
			 0xe4bc3291, /// 0x6a8
			 0xce34c4c3, /// 0x6ac
			 0xbebe71e3, /// 0x6b0
			 0x602cba67, /// 0x6b4
			 0x3c236cbd, /// 0x6b8
			 0x7a17d139, /// 0x6bc
			 0x9fef513a, /// 0x6c0
			 0x494de87f, /// 0x6c4
			 0x366d0882, /// 0x6c8
			 0x39d96948, /// 0x6cc
			 0xcc96564f, /// 0x6d0
			 0x867f9b80, /// 0x6d4
			 0x02e3dd98, /// 0x6d8
			 0x20ed3fba, /// 0x6dc
			 0x0de95838, /// 0x6e0
			 0x208ce41b, /// 0x6e4
			 0x2113c2fc, /// 0x6e8
			 0xa09ee663, /// 0x6ec
			 0x6d65baed, /// 0x6f0
			 0xa573c3d2, /// 0x6f4
			 0xa6132854, /// 0x6f8
			 0xac369aa5, /// 0x6fc
			 0xc983448a, /// 0x700
			 0x33921795, /// 0x704
			 0xc34c5854, /// 0x708
			 0xa99fe4d4, /// 0x70c
			 0x383a37c5, /// 0x710
			 0x50154c48, /// 0x714
			 0x6245ea69, /// 0x718
			 0x7f9618a5, /// 0x71c
			 0x9d027177, /// 0x720
			 0x5677c307, /// 0x724
			 0x0beeff1a, /// 0x728
			 0xaba999f9, /// 0x72c
			 0x9d125819, /// 0x730
			 0xbc15da50, /// 0x734
			 0x7094a30f, /// 0x738
			 0xb48a520c, /// 0x73c
			 0x81c07439, /// 0x740
			 0x80824e4a, /// 0x744
			 0x50db6ae0, /// 0x748
			 0xf4e70e24, /// 0x74c
			 0xb93d4915, /// 0x750
			 0x46834c57, /// 0x754
			 0x031d65a4, /// 0x758
			 0x0532c363, /// 0x75c
			 0x97c980a6, /// 0x760
			 0xc3ed6025, /// 0x764
			 0x021c1476, /// 0x768
			 0x12310ad2, /// 0x76c
			 0xfc33edb1, /// 0x770
			 0xcc9976e6, /// 0x774
			 0xb1131dbc, /// 0x778
			 0xd9edbc59, /// 0x77c
			 0x140d0d4e, /// 0x780
			 0xe85b60c3, /// 0x784
			 0x2bbccc47, /// 0x788
			 0x87f7f3b7, /// 0x78c
			 0xa7c2bc50, /// 0x790
			 0xd1eb6e03, /// 0x794
			 0x2cfebb07, /// 0x798
			 0xa918dd83, /// 0x79c
			 0x86940f3c, /// 0x7a0
			 0x820a1ddd, /// 0x7a4
			 0xbbcb154d, /// 0x7a8
			 0x03d1b05f, /// 0x7ac
			 0xbdbbfa2f, /// 0x7b0
			 0x3e53884d, /// 0x7b4
			 0x9facac6c, /// 0x7b8
			 0xc53f133a, /// 0x7bc
			 0x45345ce3, /// 0x7c0
			 0x8ecc643c, /// 0x7c4
			 0xe57b2b22, /// 0x7c8
			 0xff276f0a, /// 0x7cc
			 0xaf25bf21, /// 0x7d0
			 0x881dc6f0, /// 0x7d4
			 0x778511b5, /// 0x7d8
			 0x0f8b0782, /// 0x7dc
			 0x585e5d63, /// 0x7e0
			 0xdd556f28, /// 0x7e4
			 0xa5014971, /// 0x7e8
			 0x38466e6f, /// 0x7ec
			 0x89bf3f5d, /// 0x7f0
			 0x5156cd5c, /// 0x7f4
			 0xb9242bda, /// 0x7f8
			 0x75316299, /// 0x7fc
			 0x638a334a, /// 0x800
			 0xf08dc98c, /// 0x804
			 0x016a5e62, /// 0x808
			 0x2d82bbfc, /// 0x80c
			 0x6910dd9d, /// 0x810
			 0xc680d632, /// 0x814
			 0xb9cd3cd9, /// 0x818
			 0xc1394b87, /// 0x81c
			 0x06024ae9, /// 0x820
			 0xe5cc3ba9, /// 0x824
			 0xcb48a7f3, /// 0x828
			 0xd9a10480, /// 0x82c
			 0xe5f8412f, /// 0x830
			 0x0e40d5e8, /// 0x834
			 0xfce4c75f, /// 0x838
			 0x84971c51, /// 0x83c
			 0xdff99d4f, /// 0x840
			 0x69e7a3a9, /// 0x844
			 0x4ad56bd2, /// 0x848
			 0x0653485d, /// 0x84c
			 0xa634e537, /// 0x850
			 0xf8eac93c, /// 0x854
			 0xbe012cf0, /// 0x858
			 0x35764912, /// 0x85c
			 0x5a112082, /// 0x860
			 0x8971f4a5, /// 0x864
			 0xd9bc8d42, /// 0x868
			 0xa675a320, /// 0x86c
			 0xa856aca0, /// 0x870
			 0x6c14f0ef, /// 0x874
			 0x29e71700, /// 0x878
			 0x12484426, /// 0x87c
			 0xc92cd505, /// 0x880
			 0x7e44dca2, /// 0x884
			 0xa011ffce, /// 0x888
			 0xf347a9a6, /// 0x88c
			 0x12bcad98, /// 0x890
			 0x4a9e78d7, /// 0x894
			 0xec844430, /// 0x898
			 0x605ff1c0, /// 0x89c
			 0x17445943, /// 0x8a0
			 0xdd3c7bfc, /// 0x8a4
			 0x789b6ceb, /// 0x8a8
			 0xc0ae2a43, /// 0x8ac
			 0x8eab673b, /// 0x8b0
			 0xd414e121, /// 0x8b4
			 0x3a97b374, /// 0x8b8
			 0xbe62b9f8, /// 0x8bc
			 0x70d63b83, /// 0x8c0
			 0xaf2e6137, /// 0x8c4
			 0x3e6c84cb, /// 0x8c8
			 0x8e891753, /// 0x8cc
			 0xf9e731fc, /// 0x8d0
			 0xf84810e3, /// 0x8d4
			 0x76c07c9d, /// 0x8d8
			 0x1aa9b86b, /// 0x8dc
			 0x41202ff0, /// 0x8e0
			 0x0b8ffe38, /// 0x8e4
			 0x7009412f, /// 0x8e8
			 0x7bbc0d92, /// 0x8ec
			 0x9e848d8b, /// 0x8f0
			 0xb84bcd0a, /// 0x8f4
			 0xab562586, /// 0x8f8
			 0xef75e794, /// 0x8fc
			 0xe7a2d92a, /// 0x900
			 0x46af4d37, /// 0x904
			 0x8ba7cecd, /// 0x908
			 0x208d0540, /// 0x90c
			 0xd7f29caa, /// 0x910
			 0x89247f1d, /// 0x914
			 0x63212a83, /// 0x918
			 0xeaa91e94, /// 0x91c
			 0x3a1451ba, /// 0x920
			 0xd8148429, /// 0x924
			 0x4499aace, /// 0x928
			 0x1a7d709d, /// 0x92c
			 0xe203e93d, /// 0x930
			 0x793c7ca5, /// 0x934
			 0x71c419cc, /// 0x938
			 0x95e4dd66, /// 0x93c
			 0x9ca85d0e, /// 0x940
			 0xe4054129, /// 0x944
			 0x19c22fbf, /// 0x948
			 0xe89e7708, /// 0x94c
			 0x03a38ebd, /// 0x950
			 0xdacb074f, /// 0x954
			 0x284c588f, /// 0x958
			 0xd36f5b36, /// 0x95c
			 0x1e69f3b4, /// 0x960
			 0x4272055b, /// 0x964
			 0x772a7ac3, /// 0x968
			 0xafe3ae21, /// 0x96c
			 0x63e7fdae, /// 0x970
			 0x811af021, /// 0x974
			 0xc674ef06, /// 0x978
			 0x9ff845d7, /// 0x97c
			 0x6f012eb9, /// 0x980
			 0x01fa7adb, /// 0x984
			 0x2368f27a, /// 0x988
			 0x77fce68b, /// 0x98c
			 0x11a1888a, /// 0x990
			 0xbdeafc5f, /// 0x994
			 0xccfabaf6, /// 0x998
			 0xaee40ddd, /// 0x99c
			 0xef2de3aa, /// 0x9a0
			 0xab30bbb8, /// 0x9a4
			 0xf0103d31, /// 0x9a8
			 0x0908877c, /// 0x9ac
			 0x2314d05b, /// 0x9b0
			 0xa7795e1e, /// 0x9b4
			 0x470ce193, /// 0x9b8
			 0xdc881ce3, /// 0x9bc
			 0x43b00073, /// 0x9c0
			 0x657654dd, /// 0x9c4
			 0xc1f07e89, /// 0x9c8
			 0x89b4b05c, /// 0x9cc
			 0x57ec516f, /// 0x9d0
			 0xd792495a, /// 0x9d4
			 0x7dfaea43, /// 0x9d8
			 0x53a1f1ec, /// 0x9dc
			 0x9e070ccd, /// 0x9e0
			 0xbdfd79cd, /// 0x9e4
			 0x5aff46e6, /// 0x9e8
			 0x3e61ffad, /// 0x9ec
			 0xae7f6806, /// 0x9f0
			 0xd6c6cf66, /// 0x9f4
			 0xfa9f0fbc, /// 0x9f8
			 0x535e64a5, /// 0x9fc
			 0x60ff05ff, /// 0xa00
			 0x4dc6b1ef, /// 0xa04
			 0xa745e3f2, /// 0xa08
			 0xa37e4df3, /// 0xa0c
			 0x7c56d335, /// 0xa10
			 0x20df4cd6, /// 0xa14
			 0x3c6782f0, /// 0xa18
			 0xe37ea12b, /// 0xa1c
			 0x3d783341, /// 0xa20
			 0x22816920, /// 0xa24
			 0xe595dd80, /// 0xa28
			 0x8e4b6ee2, /// 0xa2c
			 0x4b66a977, /// 0xa30
			 0x82e22dae, /// 0xa34
			 0x341945ee, /// 0xa38
			 0x99273ddc, /// 0xa3c
			 0x7dea1579, /// 0xa40
			 0x29dee72e, /// 0xa44
			 0x7ce3e368, /// 0xa48
			 0xc93bbf59, /// 0xa4c
			 0xb4d597ef, /// 0xa50
			 0xe2a76989, /// 0xa54
			 0xeb8e1ca5, /// 0xa58
			 0x120c76a0, /// 0xa5c
			 0x5bdf75e7, /// 0xa60
			 0x165dfb2b, /// 0xa64
			 0x78f392b5, /// 0xa68
			 0x9114d8a8, /// 0xa6c
			 0x3bed8e5e, /// 0xa70
			 0x721f9298, /// 0xa74
			 0x99f5397a, /// 0xa78
			 0x18cc5762, /// 0xa7c
			 0xafdbfba7, /// 0xa80
			 0x619c8d45, /// 0xa84
			 0x6380a283, /// 0xa88
			 0x81cee255, /// 0xa8c
			 0x42919a83, /// 0xa90
			 0x5ec1829a, /// 0xa94
			 0x231c111b, /// 0xa98
			 0xbec4d70d, /// 0xa9c
			 0x3424e37e, /// 0xaa0
			 0xa26a5813, /// 0xaa4
			 0x9799c4f5, /// 0xaa8
			 0x5fd776b1, /// 0xaac
			 0x80c1e1a8, /// 0xab0
			 0x2283f2e6, /// 0xab4
			 0x098a0b3e, /// 0xab8
			 0xa7f919dc, /// 0xabc
			 0xfb7ea6d7, /// 0xac0
			 0xb9dd07ca, /// 0xac4
			 0xb36f3d2f, /// 0xac8
			 0xe036b929, /// 0xacc
			 0xdc6d5824, /// 0xad0
			 0x2d116f70, /// 0xad4
			 0xcd545094, /// 0xad8
			 0x952c503c, /// 0xadc
			 0x84317d8c, /// 0xae0
			 0x6e0afe54, /// 0xae4
			 0x60253769, /// 0xae8
			 0x2403eabd, /// 0xaec
			 0x5d5db8a9, /// 0xaf0
			 0x5fe800a1, /// 0xaf4
			 0xff692603, /// 0xaf8
			 0xb02d91fb, /// 0xafc
			 0xcfd15ed6, /// 0xb00
			 0x09e2bf9d, /// 0xb04
			 0x3b331968, /// 0xb08
			 0x58b5e441, /// 0xb0c
			 0x2375008e, /// 0xb10
			 0x26a30d76, /// 0xb14
			 0x5a17bddb, /// 0xb18
			 0x41d7d4b0, /// 0xb1c
			 0xd513ab17, /// 0xb20
			 0x40f8d082, /// 0xb24
			 0xb902c96a, /// 0xb28
			 0xccbf4ec9, /// 0xb2c
			 0x114771bd, /// 0xb30
			 0x021ba456, /// 0xb34
			 0x4437dbf9, /// 0xb38
			 0xa464a731, /// 0xb3c
			 0xe0208048, /// 0xb40
			 0xa3e4cdf0, /// 0xb44
			 0xfd53d902, /// 0xb48
			 0x15af98ab, /// 0xb4c
			 0x5b9c0b30, /// 0xb50
			 0x6b01d96e, /// 0xb54
			 0xb2604ca4, /// 0xb58
			 0x29562f52, /// 0xb5c
			 0x964dd10f, /// 0xb60
			 0x003bce9a, /// 0xb64
			 0x128dceeb, /// 0xb68
			 0xdcb53e01, /// 0xb6c
			 0x005e642b, /// 0xb70
			 0x5bc60619, /// 0xb74
			 0xc4cd84aa, /// 0xb78
			 0x74af2312, /// 0xb7c
			 0xc04b7eb4, /// 0xb80
			 0x3260fc93, /// 0xb84
			 0xe4992694, /// 0xb88
			 0x2b30c938, /// 0xb8c
			 0x2a15ca02, /// 0xb90
			 0xad7a3874, /// 0xb94
			 0x158e4e52, /// 0xb98
			 0xfcdf86f2, /// 0xb9c
			 0x62918fd2, /// 0xba0
			 0x75dd24a1, /// 0xba4
			 0x8eb7efd3, /// 0xba8
			 0x86b35186, /// 0xbac
			 0xf7b51f5d, /// 0xbb0
			 0x8ce8ac4b, /// 0xbb4
			 0x3af5662e, /// 0xbb8
			 0xd9dcf87a, /// 0xbbc
			 0x76cb0773, /// 0xbc0
			 0x4a34494a, /// 0xbc4
			 0x104f51fc, /// 0xbc8
			 0xdd3e31d9, /// 0xbcc
			 0xf6f24700, /// 0xbd0
			 0xf7245398, /// 0xbd4
			 0x81d96bed, /// 0xbd8
			 0x5920aa48, /// 0xbdc
			 0xe76b0105, /// 0xbe0
			 0x88d37e83, /// 0xbe4
			 0xd49241e6, /// 0xbe8
			 0x80f328c9, /// 0xbec
			 0x355e2554, /// 0xbf0
			 0x880f169c, /// 0xbf4
			 0x155eb035, /// 0xbf8
			 0x78ec5434, /// 0xbfc
			 0xf3ef2c6e, /// 0xc00
			 0x5adf1f51, /// 0xc04
			 0x2edfae0d, /// 0xc08
			 0x38a08462, /// 0xc0c
			 0x80ad52d0, /// 0xc10
			 0xf49a50d6, /// 0xc14
			 0x6408fa19, /// 0xc18
			 0xa9770970, /// 0xc1c
			 0x22957d82, /// 0xc20
			 0x8f3ec8bf, /// 0xc24
			 0x941ae7ab, /// 0xc28
			 0x0525fa1d, /// 0xc2c
			 0x1c4f1b16, /// 0xc30
			 0x94149401, /// 0xc34
			 0x390d9e6a, /// 0xc38
			 0xfbda5c1e, /// 0xc3c
			 0x20a01522, /// 0xc40
			 0xd567a5d6, /// 0xc44
			 0x810ab27b, /// 0xc48
			 0x62a472e5, /// 0xc4c
			 0x035f15bd, /// 0xc50
			 0x5fb99ad5, /// 0xc54
			 0xb03029fb, /// 0xc58
			 0x26a28255, /// 0xc5c
			 0xf425df2b, /// 0xc60
			 0xe16e260a, /// 0xc64
			 0x01210a9f, /// 0xc68
			 0x7a1b2869, /// 0xc6c
			 0x3761beef, /// 0xc70
			 0x0d33ec22, /// 0xc74
			 0x7f0eee2f, /// 0xc78
			 0xfaba6d84, /// 0xc7c
			 0x68afd76d, /// 0xc80
			 0xf48428f0, /// 0xc84
			 0x7e14604c, /// 0xc88
			 0xc1ee0821, /// 0xc8c
			 0xd8fbfb74, /// 0xc90
			 0xe86d4f19, /// 0xc94
			 0x4a1f71b2, /// 0xc98
			 0x2c26ce10, /// 0xc9c
			 0x5418bb10, /// 0xca0
			 0x0e945d1a, /// 0xca4
			 0x9d6eb457, /// 0xca8
			 0x34b4339f, /// 0xcac
			 0xe1bda89f, /// 0xcb0
			 0xd3ed23b9, /// 0xcb4
			 0x081a196c, /// 0xcb8
			 0xfeb203b2, /// 0xcbc
			 0x8d0ee795, /// 0xcc0
			 0x362e0795, /// 0xcc4
			 0x0194e8d6, /// 0xcc8
			 0x0278f401, /// 0xccc
			 0x3095860d, /// 0xcd0
			 0x77294d72, /// 0xcd4
			 0x2f4826a2, /// 0xcd8
			 0x76df6da3, /// 0xcdc
			 0xf4446a34, /// 0xce0
			 0x3feb2781, /// 0xce4
			 0xa9e5cd7f, /// 0xce8
			 0x50dd7719, /// 0xcec
			 0x6dc56ec2, /// 0xcf0
			 0xf4d86bfc, /// 0xcf4
			 0x80bad7ff, /// 0xcf8
			 0x83f6a2df, /// 0xcfc
			 0x88d48c99, /// 0xd00
			 0x7bea8d4a, /// 0xd04
			 0x30bd6a6d, /// 0xd08
			 0x98c29e9d, /// 0xd0c
			 0x566ae7b9, /// 0xd10
			 0x2caae92f, /// 0xd14
			 0xe0fbe189, /// 0xd18
			 0x6358096f, /// 0xd1c
			 0x4f3cdbd5, /// 0xd20
			 0xcf732b9f, /// 0xd24
			 0x1cdf6daa, /// 0xd28
			 0xb8c740d2, /// 0xd2c
			 0x20b6e821, /// 0xd30
			 0x3114b4f9, /// 0xd34
			 0x6c0f579b, /// 0xd38
			 0xad3d3e88, /// 0xd3c
			 0x5cc2cca8, /// 0xd40
			 0xa2cccdc0, /// 0xd44
			 0xa95a292e, /// 0xd48
			 0x7fb3bf8f, /// 0xd4c
			 0x101c4ed4, /// 0xd50
			 0x064bfab9, /// 0xd54
			 0xd6867914, /// 0xd58
			 0x6818da61, /// 0xd5c
			 0xc9624ac7, /// 0xd60
			 0x2436d4fb, /// 0xd64
			 0x133cc15b, /// 0xd68
			 0x796dc6a4, /// 0xd6c
			 0xe1d92f7a, /// 0xd70
			 0xc83c4989, /// 0xd74
			 0x861c074a, /// 0xd78
			 0x7fdbcaf3, /// 0xd7c
			 0xa9daa7af, /// 0xd80
			 0x628951f0, /// 0xd84
			 0xce8d3695, /// 0xd88
			 0xa40cea4c, /// 0xd8c
			 0x52f2d5c4, /// 0xd90
			 0xa5413da1, /// 0xd94
			 0x15f48c52, /// 0xd98
			 0x1b224deb, /// 0xd9c
			 0x733c2404, /// 0xda0
			 0x9b4f94ef, /// 0xda4
			 0xdd9fc1e2, /// 0xda8
			 0x6b538098, /// 0xdac
			 0xbacae197, /// 0xdb0
			 0x012b1232, /// 0xdb4
			 0x89b33a72, /// 0xdb8
			 0xae57229a, /// 0xdbc
			 0x4856dc26, /// 0xdc0
			 0x39438ff3, /// 0xdc4
			 0x3c1bd685, /// 0xdc8
			 0xdd51fcec, /// 0xdcc
			 0xf1a0b749, /// 0xdd0
			 0x3a3f754c, /// 0xdd4
			 0x7d1ac10b, /// 0xdd8
			 0x9d58e629, /// 0xddc
			 0xf7f758b3, /// 0xde0
			 0xb0e7c25b, /// 0xde4
			 0xad60e944, /// 0xde8
			 0x4dbc8afb, /// 0xdec
			 0x85f28749, /// 0xdf0
			 0x3a63489f, /// 0xdf4
			 0x01cc881a, /// 0xdf8
			 0x1ba576df, /// 0xdfc
			 0x81a2f45f, /// 0xe00
			 0x3d67b1c8, /// 0xe04
			 0x995cadc4, /// 0xe08
			 0x81be410a, /// 0xe0c
			 0x9b368e77, /// 0xe10
			 0xecd5c4dc, /// 0xe14
			 0x954fdee5, /// 0xe18
			 0x0bdb8643, /// 0xe1c
			 0x8f3e6706, /// 0xe20
			 0x710e7f57, /// 0xe24
			 0xf5e77bcf, /// 0xe28
			 0x1d2fcc84, /// 0xe2c
			 0xd358facb, /// 0xe30
			 0x3f5c978a, /// 0xe34
			 0x070e00b1, /// 0xe38
			 0x040e6b40, /// 0xe3c
			 0xb9394c8d, /// 0xe40
			 0xeb0268f7, /// 0xe44
			 0x9cb08cd9, /// 0xe48
			 0xf6c8c9ce, /// 0xe4c
			 0x637d949d, /// 0xe50
			 0x58c36fbe, /// 0xe54
			 0xf9355699, /// 0xe58
			 0x18e38ce4, /// 0xe5c
			 0xd7c2aa9b, /// 0xe60
			 0xd19a3ed5, /// 0xe64
			 0x9fde7c13, /// 0xe68
			 0x987134ee, /// 0xe6c
			 0xf0970e3e, /// 0xe70
			 0x82980193, /// 0xe74
			 0x738deac7, /// 0xe78
			 0x8dd82400, /// 0xe7c
			 0x39ce5d83, /// 0xe80
			 0xcc7f735c, /// 0xe84
			 0x09c7b3d6, /// 0xe88
			 0xa5c6ad9d, /// 0xe8c
			 0x484c4ed0, /// 0xe90
			 0x5d7e4808, /// 0xe94
			 0xc3d271f3, /// 0xe98
			 0x63508084, /// 0xe9c
			 0x35869d82, /// 0xea0
			 0x84625ebd, /// 0xea4
			 0x3987a7b1, /// 0xea8
			 0xd4c99502, /// 0xeac
			 0x6a8c065d, /// 0xeb0
			 0x9643f8a6, /// 0xeb4
			 0xb9cda329, /// 0xeb8
			 0x1def5f07, /// 0xebc
			 0xe77b4ea3, /// 0xec0
			 0x7f8b3654, /// 0xec4
			 0x41648988, /// 0xec8
			 0xea08bf4b, /// 0xecc
			 0xc7776ccd, /// 0xed0
			 0x667a2944, /// 0xed4
			 0x5c6b580a, /// 0xed8
			 0x4866b17b, /// 0xedc
			 0x59a2fde1, /// 0xee0
			 0x0b3a2645, /// 0xee4
			 0x7e8c327a, /// 0xee8
			 0x4e277ed0, /// 0xeec
			 0xa63ab147, /// 0xef0
			 0xfe90680e, /// 0xef4
			 0xdca58699, /// 0xef8
			 0xd6f41c78, /// 0xefc
			 0xe4a5e55d, /// 0xf00
			 0xbd0c56dc, /// 0xf04
			 0x98d22173, /// 0xf08
			 0x39462e8b, /// 0xf0c
			 0xef8c9b53, /// 0xf10
			 0x1b2269b9, /// 0xf14
			 0xba897d74, /// 0xf18
			 0xbabdee8a, /// 0xf1c
			 0x540c4c8e, /// 0xf20
			 0xd0a553c1, /// 0xf24
			 0xa9897480, /// 0xf28
			 0x1e26951a, /// 0xf2c
			 0x079b505b, /// 0xf30
			 0xf5d7b037, /// 0xf34
			 0x5524120d, /// 0xf38
			 0x2d48c479, /// 0xf3c
			 0x91389c16, /// 0xf40
			 0x818b9ff2, /// 0xf44
			 0x0f500ebf, /// 0xf48
			 0x714e1748, /// 0xf4c
			 0x78469627, /// 0xf50
			 0x13ef1d4b, /// 0xf54
			 0xe9a2a211, /// 0xf58
			 0xba9c60a1, /// 0xf5c
			 0x23d80cfc, /// 0xf60
			 0x08f6f15c, /// 0xf64
			 0x6f017d7e, /// 0xf68
			 0x56041626, /// 0xf6c
			 0x838bf36d, /// 0xf70
			 0xc0d0bc42, /// 0xf74
			 0x17a51c79, /// 0xf78
			 0xd8f7160e, /// 0xf7c
			 0xff8f5dae, /// 0xf80
			 0x2dabb425, /// 0xf84
			 0x73ff7eaf, /// 0xf88
			 0xa426de3e, /// 0xf8c
			 0xee646f65, /// 0xf90
			 0x2fab84de, /// 0xf94
			 0x0fae9d0a, /// 0xf98
			 0xca7e09f4, /// 0xf9c
			 0xa188ac25, /// 0xfa0
			 0x773f69e3, /// 0xfa4
			 0x9d1d1131, /// 0xfa8
			 0xdef87413, /// 0xfac
			 0xcc1cb303, /// 0xfb0
			 0x2983ed6a, /// 0xfb4
			 0x475d276b, /// 0xfb8
			 0x32d62c49, /// 0xfbc
			 0xfc6e0504, /// 0xfc0
			 0xcb4df004, /// 0xfc4
			 0x1dccf958, /// 0xfc8
			 0xeefa57a0, /// 0xfcc
			 0xb21583c5, /// 0xfd0
			 0x3924e9f3, /// 0xfd4
			 0x3f52fc86, /// 0xfd8
			 0x54bd1c56, /// 0xfdc
			 0xc3049d11, /// 0xfe0
			 0x7a7aa4c1, /// 0xfe4
			 0x5531f435, /// 0xfe8
			 0x7dd64ca3, /// 0xfec
			 0x9ebfba4c, /// 0xff0
			 0x3cd5d7d4, /// 0xff4
			 0xfa947b09, /// 0xff8
			 0x0d67b2f6 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00011111,                                                  // 9.7958E-41                                  /// 00000
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00008
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0000c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00010
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00014
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00020
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00024
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00028
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0002c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00030
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00034
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00038
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0003c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00040
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00044
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0004c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00050
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00054
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00058
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0005c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00074
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00078
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00080
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00088
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0008c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00090
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00094
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00098
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0009c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x4b000000,                                                  // 8388608.0                                   /// 000bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 000d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000e0
			 0xcb000000,                                                  // -8388608.0                                  /// 000e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000e8
			 0x00000000,                                                  // zero                                        /// 000ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00108
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00110
			 0x0e000007,                                                  // Trailing 1s:                                /// 00114
			 0x0e000003,                                                  // Trailing 1s:                                /// 00118
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0011c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00120
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00124
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00128
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0012c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00138
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0013c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x00011111,                                                  // 9.7958E-41                                  /// 00144
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00148
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0014c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x3f028f5c,                                                  // 0.51                                        /// 00158
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0015c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00164
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00168
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0017c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0xcb000000,                                                  // -8388608.0                                  /// 00184
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00188
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0018c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0xff800000,                                                  // -inf                                        /// 00194
			 0xffc00001,                                                  // -signaling NaN                              /// 00198
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0019c
			 0x7fc00001,                                                  // signaling NaN                               /// 001a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001a4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 001c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001e4
			 0xbf028f5c,                                                  // -0.51                                       /// 001e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0020c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00210
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x55555555,                                                  // 4 random values                             /// 0021c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00220
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0022c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00230
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00234
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0023c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00244
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0024c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00254
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00258
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0025c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00264
			 0x0e000001,                                                  // Trailing 1s:                                /// 00268
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00274
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00278
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00284
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00288
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0028c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00290
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00294
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0029c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002ac
			 0x33333333,                                                  // 4 random values                             /// 002b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00300
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00304
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00308
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00310
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00314
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0031c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0032c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00330
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00338
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0033c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00344
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00348
			 0xff800000,                                                  // -inf                                        /// 0034c
			 0xbf028f5c,                                                  // -0.51                                       /// 00350
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00358
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0035c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0e000003,                                                  // Trailing 1s:                                /// 00364
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00370
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00374
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00378
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00380
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00388
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00390
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00394
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0039c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003f4
			 0xffc00001,                                                  // -signaling NaN                              /// 003f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00404
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00408
			 0x0e000007,                                                  // Trailing 1s:                                /// 0040c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00410
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00414
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x00011111,                                                  // 9.7958E-41                                  /// 0041c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00424
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00438
			 0x55555555,                                                  // 4 random values                             /// 0043c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00440
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00444
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00448
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0044c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00450
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0045c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00460
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00464
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0046c
			 0x55555555,                                                  // 4 random values                             /// 00470
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00478
			 0x33333333,                                                  // 4 random values                             /// 0047c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00484
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x0e000003,                                                  // Trailing 1s:                                /// 00494
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00498
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0049c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004a8
			 0x33333333,                                                  // 4 random values                             /// 004ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 004e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 004f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00504
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00508
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0050c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00510
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00514
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00518
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0051c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00520
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0052c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00530
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00534
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00538
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0053c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00544
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00548
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0054c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00550
			 0x00000000,                                                  // zero                                        /// 00554
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0055c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00564
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0057c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00584
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0058c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00590
			 0xcb000000,                                                  // -8388608.0                                  /// 00594
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00598
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0059c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 005e0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005e8
			 0x7fc00001,                                                  // signaling NaN                               /// 005ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00604
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00608
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0060c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00610
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00614
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00618
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00620
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00624
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0062c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00634
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00638
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00644
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00648
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0064c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00650
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00654
			 0xffc00001,                                                  // -signaling NaN                              /// 00658
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00660
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00668
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0066c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00670
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00674
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00678
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0067c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00684
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0068c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00690
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00698
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0069c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006b0
			 0x0c780000,                                                  // Leading 1s:                                 /// 006b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006b8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 006e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00700
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00704
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0070c
			 0xbf028f5c,                                                  // -0.51                                       /// 00710
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00714
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00718
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00720
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00724
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00728
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0072c
			 0x33333333,                                                  // 4 random values                             /// 00730
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00734
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00738
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00740
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00744
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00748
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0074c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00754
			 0x4b000000,                                                  // 8388608.0                                   /// 00758
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00760
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0076c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00770
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x7fc00001,                                                  // signaling NaN                               /// 00778
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00780
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x0c780000,                                                  // Leading 1s:                                 /// 00788
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0xffc00001,                                                  // -signaling NaN                              /// 00790
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00794
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00798
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0079c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007b4
			 0xcb000000,                                                  // -8388608.0                                  /// 007b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007c0
			 0x33333333,                                                  // 4 random values                             /// 007c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007d4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x80000000,                                                  // -zero                                       /// 007e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00804
			 0x80000000,                                                  // -zero                                       /// 00808
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0080c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00810
			 0x7fc00001,                                                  // signaling NaN                               /// 00814
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0081c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00820
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00828
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0082c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00830
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00834
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0083c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00850
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00854
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0xcb000000,                                                  // -8388608.0                                  /// 0085c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00860
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00864
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00868
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0086c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00874
			 0x0c600000,                                                  // Leading 1s:                                 /// 00878
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00880
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00884
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0088c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00890
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00898
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008c0
			 0x3f028f5c,                                                  // 0.51                                        /// 008c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008d0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 008e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00900
			 0xff800000,                                                  // -inf                                        /// 00904
			 0x80000000,                                                  // -zero                                       /// 00908
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0090c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x80011111,                                                  // -9.7958E-41                                 /// 00914
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00918
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0091c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00920
			 0x00000000,                                                  // zero                                        /// 00924
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00928
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00930
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00934
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00938
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0093c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00940
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x7fc00001,                                                  // signaling NaN                               /// 0094c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00950
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00954
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00958
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00964
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00968
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00974
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00978
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0097c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00984
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0098c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00990
			 0xff800000,                                                  // -inf                                        /// 00994
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00998
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 009c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009c4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a0c
			 0x00000000,                                                  // zero                                        /// 00a10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a1c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a2c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a3c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a40
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a50
			 0x3f028f5c,                                                  // 0.51                                        /// 00a54
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a5c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a68
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a7c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a84
			 0xff800000,                                                  // -inf                                        /// 00a88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a90
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a9c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00aa0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00aa4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ab0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ab4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ab8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00abc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ac0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ac4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ac8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00acc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ad8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x55555555,                                                  // 4 random values                             /// 00ae4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ae8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00aec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00af0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00af4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b04
			 0x7fc00001,                                                  // signaling NaN                               /// 00b08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b10
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b18
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b24
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b4c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b50
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b58
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b60
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b64
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b78
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b7c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b84
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b90
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b9c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bc0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bc4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bd0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bd4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bd8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bdc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00be0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00be4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00be8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bf4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bf8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c04
			 0x7fc00001,                                                  // signaling NaN                               /// 00c08
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c14
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c1c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c28
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c2c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c30
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c3c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c48
			 0x33333333,                                                  // 4 random values                             /// 00c4c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c50
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c54
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c58
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c60
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c80
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c88
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c8c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c9c
			 0x7f800000,                                                  // inf                                         /// 00ca0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ca4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cb4
			 0xff800000,                                                  // -inf                                        /// 00cb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00cbc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cc0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ccc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cd0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cd4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ce0
			 0xff800000,                                                  // -inf                                        /// 00ce4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ce8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cf0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cf4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cfc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d00
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d04
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d08
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d10
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d38
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d6c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d70
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00000000,                                                  // zero                                        /// 00d78
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d80
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d8c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00da0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00da4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x7f800000,                                                  // inf                                         /// 00db0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00db4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dc0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dcc
			 0x7fc00001,                                                  // signaling NaN                               /// 00dd0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dd4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd8
			 0x00000000,                                                  // zero                                        /// 00ddc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00de0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00de8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00df0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00df4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e14
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e18
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e1c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e28
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e2c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e40
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e44
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e48
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e60
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e74
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0xbf028f5c,                                                  // -0.51                                       /// 00e84
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e94
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e9c
			 0x7f800000,                                                  // inf                                         /// 00ea0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00eb0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00eb4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00ebc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ec0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ed0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ed4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ee4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ee8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00eec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ef8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f0c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f18
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f28
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f2c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f30
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f34
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f3c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f40
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f48
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f6c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f78
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f7c
			 0x7f800000,                                                  // inf                                         /// 00f80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f88
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f90
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f98
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f9c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fa0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fa4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fa8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fb4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fbc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fc8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fcc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fd4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fd8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fe0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fe8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ff0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ff4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ff8
			 0x80200000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xf9890cd9, /// 0x0
			 0xc68ff80b, /// 0x4
			 0x85198678, /// 0x8
			 0x459bea21, /// 0xc
			 0xa90515af, /// 0x10
			 0x025dcff3, /// 0x14
			 0x50b826ea, /// 0x18
			 0x0ea10272, /// 0x1c
			 0xdfd31971, /// 0x20
			 0x0fdd4334, /// 0x24
			 0x56aba908, /// 0x28
			 0x212f2545, /// 0x2c
			 0x64b804ab, /// 0x30
			 0x6e16aeeb, /// 0x34
			 0x881fe038, /// 0x38
			 0x5f7a0771, /// 0x3c
			 0x98e20afa, /// 0x40
			 0x5a30a546, /// 0x44
			 0x1dd0f28c, /// 0x48
			 0xd11d5f2d, /// 0x4c
			 0x10a70bf2, /// 0x50
			 0x2ff3950b, /// 0x54
			 0xde58b267, /// 0x58
			 0x4ebfbd57, /// 0x5c
			 0x1a26319c, /// 0x60
			 0x9d5f53af, /// 0x64
			 0x9d95fa1f, /// 0x68
			 0x02902481, /// 0x6c
			 0xb87dd318, /// 0x70
			 0x9e4a402d, /// 0x74
			 0x2690a66c, /// 0x78
			 0xea4ee556, /// 0x7c
			 0x0efc6ed5, /// 0x80
			 0x6d76e8af, /// 0x84
			 0xab245103, /// 0x88
			 0xb4266af0, /// 0x8c
			 0xa7e980a4, /// 0x90
			 0xdfebd556, /// 0x94
			 0xdf0e53cc, /// 0x98
			 0x0de3c7f4, /// 0x9c
			 0xee5a42b7, /// 0xa0
			 0xb7f52ee3, /// 0xa4
			 0x0fb63d09, /// 0xa8
			 0x5e003679, /// 0xac
			 0xa6c2c6be, /// 0xb0
			 0xa07cb3f0, /// 0xb4
			 0x4034f463, /// 0xb8
			 0x8e0383ca, /// 0xbc
			 0x8da219b2, /// 0xc0
			 0x1febdcc9, /// 0xc4
			 0x79667647, /// 0xc8
			 0xebd54859, /// 0xcc
			 0xc29a0ce0, /// 0xd0
			 0x45112394, /// 0xd4
			 0x4c8ea6fa, /// 0xd8
			 0xf7e57dec, /// 0xdc
			 0x23d2a619, /// 0xe0
			 0x09d0e826, /// 0xe4
			 0x3acb5a67, /// 0xe8
			 0x98c0736b, /// 0xec
			 0x283771d7, /// 0xf0
			 0x14542b75, /// 0xf4
			 0xa6340215, /// 0xf8
			 0xc1aaa42e, /// 0xfc
			 0xa4e6638a, /// 0x100
			 0xced0b732, /// 0x104
			 0x6f05dabf, /// 0x108
			 0x19f540f8, /// 0x10c
			 0xc888600d, /// 0x110
			 0xf6f76122, /// 0x114
			 0xf7367378, /// 0x118
			 0xa90c1741, /// 0x11c
			 0x1bf0c386, /// 0x120
			 0x25a2240c, /// 0x124
			 0x3eb0187b, /// 0x128
			 0x4f2b727d, /// 0x12c
			 0x4086beae, /// 0x130
			 0xa0313492, /// 0x134
			 0xda8e0b25, /// 0x138
			 0x8e25bfa5, /// 0x13c
			 0x86a4bab5, /// 0x140
			 0xd7011149, /// 0x144
			 0xc5aaafb0, /// 0x148
			 0x5ec17b50, /// 0x14c
			 0x8cfb91c9, /// 0x150
			 0xd99fbaf7, /// 0x154
			 0x9f1c927b, /// 0x158
			 0x0354a969, /// 0x15c
			 0x426bc2c0, /// 0x160
			 0x0f05cd12, /// 0x164
			 0x1b8e270d, /// 0x168
			 0xec4fcdce, /// 0x16c
			 0x17872ce0, /// 0x170
			 0x5d6925e7, /// 0x174
			 0x557942f9, /// 0x178
			 0xbb45c239, /// 0x17c
			 0x0070a473, /// 0x180
			 0x025bf87c, /// 0x184
			 0x8d8f8bee, /// 0x188
			 0x13a040ee, /// 0x18c
			 0xe4a4dfa1, /// 0x190
			 0xdf12d52a, /// 0x194
			 0x4e655c15, /// 0x198
			 0x2afa29e9, /// 0x19c
			 0x0dc890eb, /// 0x1a0
			 0x4c40463c, /// 0x1a4
			 0x5c8a1a1a, /// 0x1a8
			 0xe0e6c727, /// 0x1ac
			 0x8a24dff6, /// 0x1b0
			 0x86415e90, /// 0x1b4
			 0xe488966f, /// 0x1b8
			 0xaac5eb2e, /// 0x1bc
			 0x572dd211, /// 0x1c0
			 0xe51e308f, /// 0x1c4
			 0xfbfce2aa, /// 0x1c8
			 0x1226b19f, /// 0x1cc
			 0xfc684d87, /// 0x1d0
			 0x7a66f8de, /// 0x1d4
			 0x61cfc7d2, /// 0x1d8
			 0x05b93d89, /// 0x1dc
			 0x2f62638c, /// 0x1e0
			 0x2745fdd8, /// 0x1e4
			 0x4b30a186, /// 0x1e8
			 0x15221e98, /// 0x1ec
			 0xbba561e9, /// 0x1f0
			 0x676c7595, /// 0x1f4
			 0xcbe8af0d, /// 0x1f8
			 0x8cae8f7d, /// 0x1fc
			 0x3f7bddcb, /// 0x200
			 0xe5ef6d0d, /// 0x204
			 0x904feb46, /// 0x208
			 0x07769df9, /// 0x20c
			 0xb8140ef9, /// 0x210
			 0xcc77077f, /// 0x214
			 0xa45f89cc, /// 0x218
			 0x2b634cca, /// 0x21c
			 0xebf75ff6, /// 0x220
			 0xdc6d12ee, /// 0x224
			 0x82a9c9eb, /// 0x228
			 0x14ac2838, /// 0x22c
			 0x77a9b238, /// 0x230
			 0x85c36e1e, /// 0x234
			 0x8f2e884a, /// 0x238
			 0x925e174f, /// 0x23c
			 0x172d1793, /// 0x240
			 0xa94b3e10, /// 0x244
			 0x0c96918d, /// 0x248
			 0xd41a4f5d, /// 0x24c
			 0x4956c236, /// 0x250
			 0x16800b1d, /// 0x254
			 0xfefb8b93, /// 0x258
			 0x14b54aae, /// 0x25c
			 0x49710b8f, /// 0x260
			 0x83af8ee3, /// 0x264
			 0x72b7bb09, /// 0x268
			 0xf25dfed3, /// 0x26c
			 0x6e1a4024, /// 0x270
			 0x8acfbb0d, /// 0x274
			 0x8c4890c2, /// 0x278
			 0x76d87541, /// 0x27c
			 0x9cba09de, /// 0x280
			 0x2533ef70, /// 0x284
			 0x6abd06f3, /// 0x288
			 0x2ea9aa45, /// 0x28c
			 0xec83a144, /// 0x290
			 0x72f9c929, /// 0x294
			 0xab65f9ec, /// 0x298
			 0x162e20c0, /// 0x29c
			 0xfe1d7667, /// 0x2a0
			 0xb7ac6a42, /// 0x2a4
			 0x5b2d1365, /// 0x2a8
			 0xab9a51aa, /// 0x2ac
			 0x899ef3d8, /// 0x2b0
			 0x8ffe5b16, /// 0x2b4
			 0xa9ac1f86, /// 0x2b8
			 0x67e8ab2f, /// 0x2bc
			 0x02275f70, /// 0x2c0
			 0x09b78ec7, /// 0x2c4
			 0x8ce00153, /// 0x2c8
			 0x71b6ac96, /// 0x2cc
			 0xd4791d4a, /// 0x2d0
			 0x139f67e6, /// 0x2d4
			 0xdb482e25, /// 0x2d8
			 0x9133b6db, /// 0x2dc
			 0xa26d4178, /// 0x2e0
			 0xfeb43469, /// 0x2e4
			 0xe97e5003, /// 0x2e8
			 0xb18e8f0a, /// 0x2ec
			 0x25e1a7bd, /// 0x2f0
			 0x078e38a4, /// 0x2f4
			 0xd437d4cb, /// 0x2f8
			 0x592bd582, /// 0x2fc
			 0x8afb547c, /// 0x300
			 0xef143fbc, /// 0x304
			 0x58f3ff7a, /// 0x308
			 0x22a634da, /// 0x30c
			 0x3a4bf238, /// 0x310
			 0x06644b22, /// 0x314
			 0xd4d3a35d, /// 0x318
			 0x84109e99, /// 0x31c
			 0x6babc73b, /// 0x320
			 0x4516e231, /// 0x324
			 0x366911bb, /// 0x328
			 0x59a5e47d, /// 0x32c
			 0xc6a76c8f, /// 0x330
			 0x6d986c7d, /// 0x334
			 0x597c8d92, /// 0x338
			 0xbe58027b, /// 0x33c
			 0x10f8269e, /// 0x340
			 0x2a5c9e33, /// 0x344
			 0xfbd01502, /// 0x348
			 0x1a15397e, /// 0x34c
			 0x78d2ee37, /// 0x350
			 0x9fd04588, /// 0x354
			 0xdd598c25, /// 0x358
			 0xc212e3c5, /// 0x35c
			 0x80003357, /// 0x360
			 0xdf3dddc8, /// 0x364
			 0x35263967, /// 0x368
			 0xa1cd8432, /// 0x36c
			 0xb2edd6b5, /// 0x370
			 0xf84f0482, /// 0x374
			 0xc6061f28, /// 0x378
			 0x45949870, /// 0x37c
			 0xcef44f40, /// 0x380
			 0x40d65cd6, /// 0x384
			 0x799c3edf, /// 0x388
			 0x05c2fcda, /// 0x38c
			 0xc46d6c9d, /// 0x390
			 0x5bac572f, /// 0x394
			 0xc6861870, /// 0x398
			 0x164a79c3, /// 0x39c
			 0x824984df, /// 0x3a0
			 0x1324badf, /// 0x3a4
			 0xc0f5c3a0, /// 0x3a8
			 0xb6af62bc, /// 0x3ac
			 0xd3ed7e62, /// 0x3b0
			 0x60913381, /// 0x3b4
			 0xf035cd5c, /// 0x3b8
			 0x76d19dc2, /// 0x3bc
			 0x505c00e0, /// 0x3c0
			 0x2e0329aa, /// 0x3c4
			 0x0b398db2, /// 0x3c8
			 0x7fd557a8, /// 0x3cc
			 0xd4fc7c14, /// 0x3d0
			 0x7d6f8b94, /// 0x3d4
			 0x2c8f8ea9, /// 0x3d8
			 0xb36dfdc1, /// 0x3dc
			 0x6bcc6e58, /// 0x3e0
			 0x9f0f29bb, /// 0x3e4
			 0x415805b3, /// 0x3e8
			 0x34e6bc3b, /// 0x3ec
			 0x23eb2c2f, /// 0x3f0
			 0xbad8047a, /// 0x3f4
			 0xcd7fff46, /// 0x3f8
			 0x8eed95cc, /// 0x3fc
			 0x3d1b7823, /// 0x400
			 0x9f05458c, /// 0x404
			 0xd38e96bf, /// 0x408
			 0x58c4cbe6, /// 0x40c
			 0x55c22487, /// 0x410
			 0xf1a55035, /// 0x414
			 0x8437fc17, /// 0x418
			 0x36950d75, /// 0x41c
			 0xdd29e942, /// 0x420
			 0xc10ac809, /// 0x424
			 0x33ef6415, /// 0x428
			 0x94bc1f3e, /// 0x42c
			 0x6c2f500e, /// 0x430
			 0x3b92f7a4, /// 0x434
			 0x4439fb2e, /// 0x438
			 0x36b78523, /// 0x43c
			 0xd98b5cf5, /// 0x440
			 0x0d5d87d1, /// 0x444
			 0x9b923b62, /// 0x448
			 0xc3700f4c, /// 0x44c
			 0x841d4b9d, /// 0x450
			 0x0a703ae2, /// 0x454
			 0x2e9ecf90, /// 0x458
			 0xd88689c3, /// 0x45c
			 0x8e819a5f, /// 0x460
			 0x32ff9f1e, /// 0x464
			 0x207097de, /// 0x468
			 0xa8addf37, /// 0x46c
			 0x0ec2a9f2, /// 0x470
			 0xbf2db05d, /// 0x474
			 0x26f1134c, /// 0x478
			 0x51821f21, /// 0x47c
			 0xabc4a4e6, /// 0x480
			 0x9caf76d6, /// 0x484
			 0xb4f4b3be, /// 0x488
			 0x689ffe12, /// 0x48c
			 0x5b7ead2a, /// 0x490
			 0x933a0dd5, /// 0x494
			 0x94f2f6b2, /// 0x498
			 0x68fe31be, /// 0x49c
			 0xa4b7c0c6, /// 0x4a0
			 0x3110969d, /// 0x4a4
			 0x139b8612, /// 0x4a8
			 0x7001b90e, /// 0x4ac
			 0x2dfe98a0, /// 0x4b0
			 0xb0c1c397, /// 0x4b4
			 0x3e763578, /// 0x4b8
			 0xcca6a3d2, /// 0x4bc
			 0x2a954d28, /// 0x4c0
			 0xf6fe8b9f, /// 0x4c4
			 0x4abcfbb7, /// 0x4c8
			 0xcaef5480, /// 0x4cc
			 0xce30890a, /// 0x4d0
			 0x2182d24d, /// 0x4d4
			 0xfe97f637, /// 0x4d8
			 0x514800eb, /// 0x4dc
			 0x7959d3b1, /// 0x4e0
			 0x82028c6c, /// 0x4e4
			 0xb466ca05, /// 0x4e8
			 0xcd6ef105, /// 0x4ec
			 0xb227858a, /// 0x4f0
			 0x84a7a61e, /// 0x4f4
			 0x89495e26, /// 0x4f8
			 0x4f10a2ae, /// 0x4fc
			 0xfbc51769, /// 0x500
			 0x72f1f5b9, /// 0x504
			 0x024e81d8, /// 0x508
			 0x2f61f78e, /// 0x50c
			 0x9f7f0e09, /// 0x510
			 0xffa51a89, /// 0x514
			 0xaf05248d, /// 0x518
			 0x3428fd83, /// 0x51c
			 0xa1f84b5c, /// 0x520
			 0x56e83c18, /// 0x524
			 0x42ca5bdf, /// 0x528
			 0x6c6d4d38, /// 0x52c
			 0x34bfe229, /// 0x530
			 0xc43ba054, /// 0x534
			 0x526969bd, /// 0x538
			 0x4521aa0d, /// 0x53c
			 0xbc3794d5, /// 0x540
			 0x3d289dae, /// 0x544
			 0xfc3c8ea4, /// 0x548
			 0x83543533, /// 0x54c
			 0x1c192406, /// 0x550
			 0x75a23cd6, /// 0x554
			 0x9b77206c, /// 0x558
			 0x88ed7546, /// 0x55c
			 0x5eecfa1c, /// 0x560
			 0x4d772559, /// 0x564
			 0xad1bd179, /// 0x568
			 0x0bb98868, /// 0x56c
			 0xc95621a0, /// 0x570
			 0xa966e2c6, /// 0x574
			 0x007889fa, /// 0x578
			 0xe57bc679, /// 0x57c
			 0xc1204577, /// 0x580
			 0xbfff19f9, /// 0x584
			 0x130fc6f4, /// 0x588
			 0x3cf63988, /// 0x58c
			 0xad8266e6, /// 0x590
			 0x00f5d14d, /// 0x594
			 0x73552e55, /// 0x598
			 0x15a3193c, /// 0x59c
			 0x4815a175, /// 0x5a0
			 0x2caa23ef, /// 0x5a4
			 0x74a07367, /// 0x5a8
			 0x74cf45b9, /// 0x5ac
			 0xb901b2d1, /// 0x5b0
			 0x6bc9d87e, /// 0x5b4
			 0xaeb2977c, /// 0x5b8
			 0xf7d54298, /// 0x5bc
			 0xe7f0b9d8, /// 0x5c0
			 0x840e6c82, /// 0x5c4
			 0x4b73a835, /// 0x5c8
			 0x150951cb, /// 0x5cc
			 0x6cda2a14, /// 0x5d0
			 0x0870e3a1, /// 0x5d4
			 0x981bd5f1, /// 0x5d8
			 0xb0ded1d5, /// 0x5dc
			 0xa6d376b1, /// 0x5e0
			 0xd999059b, /// 0x5e4
			 0xdfec793a, /// 0x5e8
			 0xb7f81619, /// 0x5ec
			 0xa75f5102, /// 0x5f0
			 0x5fad88ee, /// 0x5f4
			 0xa72f9e3c, /// 0x5f8
			 0xe1737b6a, /// 0x5fc
			 0xe2aab1ae, /// 0x600
			 0x49251707, /// 0x604
			 0x652557a0, /// 0x608
			 0xc6bb58fa, /// 0x60c
			 0x0ea01346, /// 0x610
			 0x35226f74, /// 0x614
			 0x444a1d82, /// 0x618
			 0x42244d1c, /// 0x61c
			 0xcdaa0391, /// 0x620
			 0x557aed11, /// 0x624
			 0xb7011e07, /// 0x628
			 0x3caae85b, /// 0x62c
			 0x4da1b896, /// 0x630
			 0x91edf63c, /// 0x634
			 0x4c836c2e, /// 0x638
			 0xa410d57d, /// 0x63c
			 0x26f0695a, /// 0x640
			 0x7480594d, /// 0x644
			 0x6986acd6, /// 0x648
			 0x2ed27e69, /// 0x64c
			 0xdd56da54, /// 0x650
			 0xfefc50d2, /// 0x654
			 0xd03eef99, /// 0x658
			 0xdd2d85aa, /// 0x65c
			 0xb1255d67, /// 0x660
			 0xb26f91f0, /// 0x664
			 0xb302416f, /// 0x668
			 0xa8d6ee04, /// 0x66c
			 0x89d0da9c, /// 0x670
			 0x1f715fef, /// 0x674
			 0xb0c6b155, /// 0x678
			 0x233fb557, /// 0x67c
			 0xa6c0ead7, /// 0x680
			 0xdddc8671, /// 0x684
			 0x7f8e0c88, /// 0x688
			 0xb5f458ab, /// 0x68c
			 0x29cdce9b, /// 0x690
			 0x454ebf1e, /// 0x694
			 0x4fff6aa3, /// 0x698
			 0xe70dc92d, /// 0x69c
			 0x9a9cf324, /// 0x6a0
			 0x3372bb61, /// 0x6a4
			 0x4a8d01a8, /// 0x6a8
			 0x21bbf5ae, /// 0x6ac
			 0x53b709be, /// 0x6b0
			 0x17cca61b, /// 0x6b4
			 0xbf9d8bf5, /// 0x6b8
			 0x221defd6, /// 0x6bc
			 0xf8d70fa3, /// 0x6c0
			 0x835ffe84, /// 0x6c4
			 0x985ae1c9, /// 0x6c8
			 0xc5050e6a, /// 0x6cc
			 0xee48d242, /// 0x6d0
			 0x91678b63, /// 0x6d4
			 0xf11a2b72, /// 0x6d8
			 0x5828ed6b, /// 0x6dc
			 0xb570c17a, /// 0x6e0
			 0x453cce45, /// 0x6e4
			 0xac371b1c, /// 0x6e8
			 0x9a4d9257, /// 0x6ec
			 0xb4d4f776, /// 0x6f0
			 0x0d88293d, /// 0x6f4
			 0xcb2d91e4, /// 0x6f8
			 0x28c9a281, /// 0x6fc
			 0xa6595cbb, /// 0x700
			 0x86545c89, /// 0x704
			 0x6c712d51, /// 0x708
			 0xfd7137a2, /// 0x70c
			 0x61e1f072, /// 0x710
			 0x5485ec07, /// 0x714
			 0xb790162d, /// 0x718
			 0x54c14230, /// 0x71c
			 0x48819e93, /// 0x720
			 0x64c434d3, /// 0x724
			 0xe9ed5918, /// 0x728
			 0xa9eff64c, /// 0x72c
			 0xb34f1668, /// 0x730
			 0x73a7abf3, /// 0x734
			 0x72dc3e68, /// 0x738
			 0x9ff306fb, /// 0x73c
			 0x62074a75, /// 0x740
			 0x1edc32ba, /// 0x744
			 0x6824b499, /// 0x748
			 0x648a5527, /// 0x74c
			 0x15aaf010, /// 0x750
			 0xabd45427, /// 0x754
			 0xd1bb9280, /// 0x758
			 0xb9a929f3, /// 0x75c
			 0x615a39e4, /// 0x760
			 0xccb17a2b, /// 0x764
			 0xc624eb46, /// 0x768
			 0xa85e0993, /// 0x76c
			 0xeba19285, /// 0x770
			 0xbde29336, /// 0x774
			 0x6412b3df, /// 0x778
			 0x08bee069, /// 0x77c
			 0xa13b958f, /// 0x780
			 0xc30215ec, /// 0x784
			 0x792251ec, /// 0x788
			 0xbb075066, /// 0x78c
			 0x2525eefc, /// 0x790
			 0x1d9ac5dd, /// 0x794
			 0xa208a0ec, /// 0x798
			 0x35e4a88e, /// 0x79c
			 0x8328f32e, /// 0x7a0
			 0x32cd6c06, /// 0x7a4
			 0x6637a895, /// 0x7a8
			 0x699b66da, /// 0x7ac
			 0x16871fc1, /// 0x7b0
			 0x3ae3c9ab, /// 0x7b4
			 0xf03fdd0b, /// 0x7b8
			 0x4855dabd, /// 0x7bc
			 0x4a5bdf23, /// 0x7c0
			 0x18d3db2a, /// 0x7c4
			 0xc677868b, /// 0x7c8
			 0xad51af40, /// 0x7cc
			 0xb2a6c28f, /// 0x7d0
			 0x3e149184, /// 0x7d4
			 0x4290c925, /// 0x7d8
			 0x73dd38b7, /// 0x7dc
			 0x4ee98ee1, /// 0x7e0
			 0x3d118ed8, /// 0x7e4
			 0x6f611d00, /// 0x7e8
			 0x876b957c, /// 0x7ec
			 0xe688bb29, /// 0x7f0
			 0x396c7b8e, /// 0x7f4
			 0x065a5700, /// 0x7f8
			 0xa6841168, /// 0x7fc
			 0xf4e820ca, /// 0x800
			 0xd61b7873, /// 0x804
			 0x49ac1d7c, /// 0x808
			 0xbfbf3588, /// 0x80c
			 0xf9e9aad2, /// 0x810
			 0x8ab7b2ec, /// 0x814
			 0x986f0213, /// 0x818
			 0x05a2980a, /// 0x81c
			 0x082241f5, /// 0x820
			 0xea6bebfd, /// 0x824
			 0xacf282ba, /// 0x828
			 0xa6ebbd56, /// 0x82c
			 0xc33d8e7f, /// 0x830
			 0x45838491, /// 0x834
			 0x10450801, /// 0x838
			 0x608b9c1f, /// 0x83c
			 0x8441b625, /// 0x840
			 0xc965a689, /// 0x844
			 0x5bc3dea6, /// 0x848
			 0x14a377bc, /// 0x84c
			 0x4ff7d6e1, /// 0x850
			 0x19846eb0, /// 0x854
			 0x8ab0be52, /// 0x858
			 0x3de148f6, /// 0x85c
			 0x20c8a6f6, /// 0x860
			 0x989a9342, /// 0x864
			 0x79d98083, /// 0x868
			 0x01d20dad, /// 0x86c
			 0x8cafbdb8, /// 0x870
			 0x8d3013d3, /// 0x874
			 0xa74fb027, /// 0x878
			 0xdff85945, /// 0x87c
			 0x153e8923, /// 0x880
			 0x1e9add01, /// 0x884
			 0x3454dffe, /// 0x888
			 0xb8de54ee, /// 0x88c
			 0x343dfd99, /// 0x890
			 0xc6759624, /// 0x894
			 0xe8e2a235, /// 0x898
			 0x4c5f3a85, /// 0x89c
			 0x1699dfe9, /// 0x8a0
			 0x2d5bdac3, /// 0x8a4
			 0xdd903e94, /// 0x8a8
			 0x7f5c4ffa, /// 0x8ac
			 0x97999ab0, /// 0x8b0
			 0xe55aac7a, /// 0x8b4
			 0xacee494c, /// 0x8b8
			 0x73619ba4, /// 0x8bc
			 0x78cc65ab, /// 0x8c0
			 0x7790ee36, /// 0x8c4
			 0x48b14146, /// 0x8c8
			 0x49e78af4, /// 0x8cc
			 0x8831cbcb, /// 0x8d0
			 0x8beac8e7, /// 0x8d4
			 0x2a0844c0, /// 0x8d8
			 0x65d1a488, /// 0x8dc
			 0x2ec0aa5d, /// 0x8e0
			 0x9f7c2ac1, /// 0x8e4
			 0x9413a0f2, /// 0x8e8
			 0xe8b7452b, /// 0x8ec
			 0x691521e4, /// 0x8f0
			 0x0c787350, /// 0x8f4
			 0x7c78959b, /// 0x8f8
			 0xc432499d, /// 0x8fc
			 0x2763a860, /// 0x900
			 0xe48afc3a, /// 0x904
			 0xe26ac7d8, /// 0x908
			 0xfbae56a2, /// 0x90c
			 0x85925cde, /// 0x910
			 0xf9930c24, /// 0x914
			 0xd490a8c9, /// 0x918
			 0x1e8b2bf9, /// 0x91c
			 0x27f37354, /// 0x920
			 0x1f3f81c2, /// 0x924
			 0x818cbf5d, /// 0x928
			 0xd1ac92c8, /// 0x92c
			 0x6972de52, /// 0x930
			 0xff55bf62, /// 0x934
			 0xfd422428, /// 0x938
			 0x9d974349, /// 0x93c
			 0xd9935f4b, /// 0x940
			 0x055cb5c5, /// 0x944
			 0x5cc57a61, /// 0x948
			 0x0b8231a8, /// 0x94c
			 0x03a1cd43, /// 0x950
			 0xa9cadf7a, /// 0x954
			 0x532d032d, /// 0x958
			 0xe165c222, /// 0x95c
			 0xd496065b, /// 0x960
			 0x78ef0e7f, /// 0x964
			 0xd52aa50e, /// 0x968
			 0xed912c63, /// 0x96c
			 0x58f93927, /// 0x970
			 0x48835c8a, /// 0x974
			 0xe251736f, /// 0x978
			 0x2bb99ef0, /// 0x97c
			 0x2e2d5268, /// 0x980
			 0xf5b863cd, /// 0x984
			 0x226a84ec, /// 0x988
			 0xc257bcaf, /// 0x98c
			 0x926e3232, /// 0x990
			 0xbbac2161, /// 0x994
			 0x7b0e1932, /// 0x998
			 0xb4b06afd, /// 0x99c
			 0x898cb3c3, /// 0x9a0
			 0xe73e1a16, /// 0x9a4
			 0x619b48b4, /// 0x9a8
			 0xb7b3062d, /// 0x9ac
			 0x726df1d8, /// 0x9b0
			 0x31364b1a, /// 0x9b4
			 0x33192c72, /// 0x9b8
			 0x41845dca, /// 0x9bc
			 0x9d569127, /// 0x9c0
			 0x4c4fa51d, /// 0x9c4
			 0x148297fe, /// 0x9c8
			 0x6ba67a68, /// 0x9cc
			 0x9b6d016d, /// 0x9d0
			 0xb9ec34e5, /// 0x9d4
			 0xb6cdef89, /// 0x9d8
			 0x6a430551, /// 0x9dc
			 0x831bf854, /// 0x9e0
			 0x5dea2c52, /// 0x9e4
			 0xf218fdbd, /// 0x9e8
			 0xf0d97902, /// 0x9ec
			 0x524e4681, /// 0x9f0
			 0x5b3ea965, /// 0x9f4
			 0x865af52e, /// 0x9f8
			 0x377c0234, /// 0x9fc
			 0xbc021ef2, /// 0xa00
			 0xb98d568b, /// 0xa04
			 0x0709d95a, /// 0xa08
			 0xa18f267c, /// 0xa0c
			 0x0c33f6db, /// 0xa10
			 0x0fdfbe04, /// 0xa14
			 0xf59d2591, /// 0xa18
			 0xdd562f36, /// 0xa1c
			 0xf8abc13c, /// 0xa20
			 0x4a97c6e2, /// 0xa24
			 0x50b1da25, /// 0xa28
			 0x9382fd5d, /// 0xa2c
			 0x095e1545, /// 0xa30
			 0x901a43f6, /// 0xa34
			 0xb0cce4a4, /// 0xa38
			 0x53308bd9, /// 0xa3c
			 0x798e83c4, /// 0xa40
			 0x16c33922, /// 0xa44
			 0xb3efab7d, /// 0xa48
			 0x0e617455, /// 0xa4c
			 0xeb90d444, /// 0xa50
			 0xc0219bd6, /// 0xa54
			 0x0d165f5d, /// 0xa58
			 0xda6bd843, /// 0xa5c
			 0xae10b52f, /// 0xa60
			 0xd6279079, /// 0xa64
			 0x4814ec2d, /// 0xa68
			 0x57298e4d, /// 0xa6c
			 0xcdad4d80, /// 0xa70
			 0xf0b30029, /// 0xa74
			 0x030b5897, /// 0xa78
			 0xcbec69b1, /// 0xa7c
			 0x500edc96, /// 0xa80
			 0xc2daa5d9, /// 0xa84
			 0xdb1ee747, /// 0xa88
			 0xc3ac7aab, /// 0xa8c
			 0xf9cac01c, /// 0xa90
			 0x40635041, /// 0xa94
			 0xe1430143, /// 0xa98
			 0x17eb62fb, /// 0xa9c
			 0xfb8d6f46, /// 0xaa0
			 0x6875f99a, /// 0xaa4
			 0xc83c424c, /// 0xaa8
			 0x79b9fb1e, /// 0xaac
			 0x70fb0c10, /// 0xab0
			 0xab739f60, /// 0xab4
			 0xe684d4a1, /// 0xab8
			 0x5de1c9ce, /// 0xabc
			 0x209391e0, /// 0xac0
			 0x5b0cd74f, /// 0xac4
			 0x944e0d90, /// 0xac8
			 0x9bfa49ca, /// 0xacc
			 0x1f3542db, /// 0xad0
			 0x68e66b33, /// 0xad4
			 0xa251a41b, /// 0xad8
			 0x51d4a5ae, /// 0xadc
			 0xd3c68801, /// 0xae0
			 0x089a6328, /// 0xae4
			 0x2bcac364, /// 0xae8
			 0x509e822d, /// 0xaec
			 0xe43136a1, /// 0xaf0
			 0x63f326a0, /// 0xaf4
			 0x5c66dd01, /// 0xaf8
			 0x806ff3f1, /// 0xafc
			 0xb66bb1e8, /// 0xb00
			 0x272b52fa, /// 0xb04
			 0x9df2bd6c, /// 0xb08
			 0x91d1f097, /// 0xb0c
			 0xcf03a26b, /// 0xb10
			 0x63a45197, /// 0xb14
			 0xd3ae2ccf, /// 0xb18
			 0x7cb69683, /// 0xb1c
			 0xcf5bf589, /// 0xb20
			 0x40714137, /// 0xb24
			 0x864d1aec, /// 0xb28
			 0x715ad5d5, /// 0xb2c
			 0x53078ca6, /// 0xb30
			 0x452f01e4, /// 0xb34
			 0xb6ec7a9f, /// 0xb38
			 0xf99038f8, /// 0xb3c
			 0x1c2ce1e3, /// 0xb40
			 0xad32af53, /// 0xb44
			 0x050e0e48, /// 0xb48
			 0xde2c8a39, /// 0xb4c
			 0x66a3f2ad, /// 0xb50
			 0x33c2938c, /// 0xb54
			 0xb60442e6, /// 0xb58
			 0x1ac7a79f, /// 0xb5c
			 0x99f8ec4a, /// 0xb60
			 0x8ac5b843, /// 0xb64
			 0xc94a8aa4, /// 0xb68
			 0x055f9501, /// 0xb6c
			 0x41252da0, /// 0xb70
			 0x009c7cff, /// 0xb74
			 0x0be3be4d, /// 0xb78
			 0xf961b839, /// 0xb7c
			 0xd2a479e6, /// 0xb80
			 0x6e408e99, /// 0xb84
			 0xdb9e9757, /// 0xb88
			 0x5f39986a, /// 0xb8c
			 0x3c1c3854, /// 0xb90
			 0x4658bb32, /// 0xb94
			 0xc74d1480, /// 0xb98
			 0xb59c68e4, /// 0xb9c
			 0xaf82d957, /// 0xba0
			 0x510340f5, /// 0xba4
			 0xd69422f1, /// 0xba8
			 0x864a070f, /// 0xbac
			 0x99b37343, /// 0xbb0
			 0x7ddcb880, /// 0xbb4
			 0xf093d5c1, /// 0xbb8
			 0x2400d4b4, /// 0xbbc
			 0x912e6ca5, /// 0xbc0
			 0xafe8d9e7, /// 0xbc4
			 0xa0a65fa2, /// 0xbc8
			 0x1af6d3f7, /// 0xbcc
			 0x27283a8c, /// 0xbd0
			 0xde639ff5, /// 0xbd4
			 0x8c9b2fc5, /// 0xbd8
			 0xfda09d9d, /// 0xbdc
			 0xb161862f, /// 0xbe0
			 0xa2b67b02, /// 0xbe4
			 0xc001486b, /// 0xbe8
			 0xc6aa0bf2, /// 0xbec
			 0xac89e701, /// 0xbf0
			 0xcd298a83, /// 0xbf4
			 0x9ffa96be, /// 0xbf8
			 0x5573c70d, /// 0xbfc
			 0x6c0510e5, /// 0xc00
			 0xdd8dd5cc, /// 0xc04
			 0xe3a80359, /// 0xc08
			 0x7a2b982b, /// 0xc0c
			 0x9b3f5d14, /// 0xc10
			 0x222f9fbe, /// 0xc14
			 0x23bd34e9, /// 0xc18
			 0x339d36fe, /// 0xc1c
			 0x2185e464, /// 0xc20
			 0xdf4fbeb5, /// 0xc24
			 0xe48132a0, /// 0xc28
			 0x2b277135, /// 0xc2c
			 0x9cc35379, /// 0xc30
			 0x9c5091f1, /// 0xc34
			 0x2bbc3e2f, /// 0xc38
			 0xc5306e67, /// 0xc3c
			 0x079c2a22, /// 0xc40
			 0x978b447d, /// 0xc44
			 0xb96afc48, /// 0xc48
			 0xe0eaa744, /// 0xc4c
			 0x5a59a228, /// 0xc50
			 0x9b42aa60, /// 0xc54
			 0xa39bb288, /// 0xc58
			 0xbe549bef, /// 0xc5c
			 0xdf7adc21, /// 0xc60
			 0x44ed7b60, /// 0xc64
			 0xa4772726, /// 0xc68
			 0xe07aec20, /// 0xc6c
			 0xa59b44c9, /// 0xc70
			 0x6efb5b92, /// 0xc74
			 0x142feb9f, /// 0xc78
			 0x66564c49, /// 0xc7c
			 0x89d456a0, /// 0xc80
			 0x02ec0a9d, /// 0xc84
			 0x06c4f197, /// 0xc88
			 0x64606c73, /// 0xc8c
			 0xa6983bc7, /// 0xc90
			 0x401af4ae, /// 0xc94
			 0x8fd33303, /// 0xc98
			 0xd3911e99, /// 0xc9c
			 0x8f5d38c1, /// 0xca0
			 0xe9caaa91, /// 0xca4
			 0x6f27792a, /// 0xca8
			 0x2b36290c, /// 0xcac
			 0x4c3ba70d, /// 0xcb0
			 0x0428061b, /// 0xcb4
			 0xa097eaac, /// 0xcb8
			 0xa1d7ed66, /// 0xcbc
			 0xca7af520, /// 0xcc0
			 0xf16ababf, /// 0xcc4
			 0xd2863a81, /// 0xcc8
			 0xaee5223c, /// 0xccc
			 0x59c643a6, /// 0xcd0
			 0xbdd6cf99, /// 0xcd4
			 0x9ad220fa, /// 0xcd8
			 0xe496b718, /// 0xcdc
			 0x7042b8d9, /// 0xce0
			 0xb099d0cd, /// 0xce4
			 0x154ef121, /// 0xce8
			 0x81100099, /// 0xcec
			 0x226a43e9, /// 0xcf0
			 0x0a93fbf9, /// 0xcf4
			 0xfc05058c, /// 0xcf8
			 0x36facbb2, /// 0xcfc
			 0x1980afab, /// 0xd00
			 0x33e70791, /// 0xd04
			 0xc39cbfb4, /// 0xd08
			 0x56133a88, /// 0xd0c
			 0x5f666d9d, /// 0xd10
			 0x29bae118, /// 0xd14
			 0x4a1a4436, /// 0xd18
			 0x407288fd, /// 0xd1c
			 0xbc4e3dca, /// 0xd20
			 0x4291a847, /// 0xd24
			 0x831994fc, /// 0xd28
			 0x8b75f28a, /// 0xd2c
			 0x4828641b, /// 0xd30
			 0x01e293d2, /// 0xd34
			 0xa47045f1, /// 0xd38
			 0x243d5fa1, /// 0xd3c
			 0x07a59a79, /// 0xd40
			 0x4feeaca2, /// 0xd44
			 0x5e042ebe, /// 0xd48
			 0x4fddd84d, /// 0xd4c
			 0xf90e3686, /// 0xd50
			 0xd4e31af8, /// 0xd54
			 0xbc90a179, /// 0xd58
			 0x7aa6b42f, /// 0xd5c
			 0x784f75ac, /// 0xd60
			 0xc6747ecc, /// 0xd64
			 0x72cf96e6, /// 0xd68
			 0x3f636aa0, /// 0xd6c
			 0xa75ab3ee, /// 0xd70
			 0x9c660559, /// 0xd74
			 0x4907f340, /// 0xd78
			 0x70960643, /// 0xd7c
			 0xe88666fc, /// 0xd80
			 0x91814375, /// 0xd84
			 0x860a3eab, /// 0xd88
			 0xa339d7b8, /// 0xd8c
			 0x65303601, /// 0xd90
			 0x1be3d896, /// 0xd94
			 0xeb97b132, /// 0xd98
			 0x11b40d77, /// 0xd9c
			 0xb03f4845, /// 0xda0
			 0x5291a9a8, /// 0xda4
			 0xce6175a5, /// 0xda8
			 0x50d26e21, /// 0xdac
			 0x55361330, /// 0xdb0
			 0x1246dc73, /// 0xdb4
			 0x3d7ed11a, /// 0xdb8
			 0x3f65a1b9, /// 0xdbc
			 0x0b208b1f, /// 0xdc0
			 0xb8c3df72, /// 0xdc4
			 0x8c6347d1, /// 0xdc8
			 0x7bdb27b4, /// 0xdcc
			 0x153f1dbe, /// 0xdd0
			 0xeda554fd, /// 0xdd4
			 0x720d5ae9, /// 0xdd8
			 0x46bdd51d, /// 0xddc
			 0x1af93259, /// 0xde0
			 0x343a98d6, /// 0xde4
			 0x285d822c, /// 0xde8
			 0x130c4190, /// 0xdec
			 0x42f89760, /// 0xdf0
			 0x300f5cb6, /// 0xdf4
			 0xaf9b0e6f, /// 0xdf8
			 0x2973591c, /// 0xdfc
			 0xc12dd79a, /// 0xe00
			 0xb40cfd6a, /// 0xe04
			 0x127973a6, /// 0xe08
			 0x114731b2, /// 0xe0c
			 0x9d7df5e4, /// 0xe10
			 0x40d20307, /// 0xe14
			 0x3bc54475, /// 0xe18
			 0x6b2c5705, /// 0xe1c
			 0xc9552706, /// 0xe20
			 0x57b661f4, /// 0xe24
			 0x192d28bf, /// 0xe28
			 0xf5bc38be, /// 0xe2c
			 0x0a447b1c, /// 0xe30
			 0x6c04ed4e, /// 0xe34
			 0x7b1f8b9e, /// 0xe38
			 0x14a541c7, /// 0xe3c
			 0xd9da69d0, /// 0xe40
			 0x8e65657a, /// 0xe44
			 0x619312b1, /// 0xe48
			 0xc3b2031b, /// 0xe4c
			 0xd712d8ce, /// 0xe50
			 0x24759219, /// 0xe54
			 0x0ba61aa2, /// 0xe58
			 0x7b549f51, /// 0xe5c
			 0xfd4a13e8, /// 0xe60
			 0xdc877d9d, /// 0xe64
			 0x1668b442, /// 0xe68
			 0x2bd64e27, /// 0xe6c
			 0x9b059e7f, /// 0xe70
			 0x38f2e483, /// 0xe74
			 0xfa8461dd, /// 0xe78
			 0xd89ded4d, /// 0xe7c
			 0x07ba6c30, /// 0xe80
			 0x25b8d7be, /// 0xe84
			 0x6896763e, /// 0xe88
			 0x3851eef6, /// 0xe8c
			 0xb4d64d9a, /// 0xe90
			 0xb1a0342f, /// 0xe94
			 0x1cb19ebd, /// 0xe98
			 0x24e2a996, /// 0xe9c
			 0xac99746a, /// 0xea0
			 0x49e11532, /// 0xea4
			 0xb1b99c48, /// 0xea8
			 0x0a90269f, /// 0xeac
			 0xab3184a5, /// 0xeb0
			 0x06d850ef, /// 0xeb4
			 0xf1e8ab24, /// 0xeb8
			 0x7d012e38, /// 0xebc
			 0x025b0f90, /// 0xec0
			 0x33e0f35e, /// 0xec4
			 0xd3725079, /// 0xec8
			 0x05dea413, /// 0xecc
			 0x2b5e5088, /// 0xed0
			 0xeedb564d, /// 0xed4
			 0x88d12ee6, /// 0xed8
			 0xc6b2a10b, /// 0xedc
			 0xa0060ce8, /// 0xee0
			 0x4ef4fcb0, /// 0xee4
			 0x117d4a2d, /// 0xee8
			 0x5a790b3b, /// 0xeec
			 0xeeba1ff1, /// 0xef0
			 0x1301948c, /// 0xef4
			 0xaae6f6fa, /// 0xef8
			 0xd8825d4e, /// 0xefc
			 0x4a770996, /// 0xf00
			 0x0b3c4297, /// 0xf04
			 0x42103eac, /// 0xf08
			 0x00ccce34, /// 0xf0c
			 0xfd1ee6ec, /// 0xf10
			 0xacd7fcc9, /// 0xf14
			 0xcddd53df, /// 0xf18
			 0x857a9db1, /// 0xf1c
			 0x4d9a36d5, /// 0xf20
			 0x353b91ce, /// 0xf24
			 0x4d6baa2f, /// 0xf28
			 0x6dc7923b, /// 0xf2c
			 0x7c7a1b6b, /// 0xf30
			 0x63cefea1, /// 0xf34
			 0x734de72c, /// 0xf38
			 0x3fc24866, /// 0xf3c
			 0xe09a0b51, /// 0xf40
			 0xa84c7f15, /// 0xf44
			 0xe36709af, /// 0xf48
			 0x01b4e597, /// 0xf4c
			 0xc9b5e2a4, /// 0xf50
			 0x832d5572, /// 0xf54
			 0x5b657a16, /// 0xf58
			 0xade958e7, /// 0xf5c
			 0x28f7dd2c, /// 0xf60
			 0x0b8556ef, /// 0xf64
			 0xcb4d8027, /// 0xf68
			 0xdb3db588, /// 0xf6c
			 0x06730485, /// 0xf70
			 0x384322d9, /// 0xf74
			 0x1842cf94, /// 0xf78
			 0x3cc2fe93, /// 0xf7c
			 0x1f37b25b, /// 0xf80
			 0xdf3fa0c4, /// 0xf84
			 0x47a44f0a, /// 0xf88
			 0xefa4d934, /// 0xf8c
			 0x141106b7, /// 0xf90
			 0xdd3c5db7, /// 0xf94
			 0x97d37082, /// 0xf98
			 0x489bd81d, /// 0xf9c
			 0x19d55377, /// 0xfa0
			 0x75de4ffb, /// 0xfa4
			 0xa0f8e208, /// 0xfa8
			 0x80283ccc, /// 0xfac
			 0x9e39a84c, /// 0xfb0
			 0x43fd872f, /// 0xfb4
			 0xeb12a0c8, /// 0xfb8
			 0x7c88a257, /// 0xfbc
			 0x3441ea92, /// 0xfc0
			 0x1974918e, /// 0xfc4
			 0xe3597ba0, /// 0xfc8
			 0x211a9b6d, /// 0xfcc
			 0x33cc022a, /// 0xfd0
			 0x80a29da1, /// 0xfd4
			 0x584cb690, /// 0xfd8
			 0xb2417f3d, /// 0xfdc
			 0xfb82e449, /// 0xfe0
			 0xd21f5ccc, /// 0xfe4
			 0xb5a34e61, /// 0xfe8
			 0xb03bc949, /// 0xfec
			 0xa1889e14, /// 0xff0
			 0x9f2be0be, /// 0xff4
			 0x3e3fdf10, /// 0xff8
			 0x5f37c049 /// last
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
			 0x00000308,
			 0x000003f0,
			 0x000000e4,
			 0x000002ac,
			 0x000004d0,
			 0x000005f8,
			 0x00000760,
			 0x0000000c,

			 /// vpu register f2
			 0x41300000,
			 0x41b00000,
			 0x41b80000,
			 0x41900000,
			 0x41880000,
			 0x41900000,
			 0x41200000,
			 0x41100000,

			 /// vpu register f3
			 0x41800000,
			 0x40c00000,
			 0x41e00000,
			 0x41880000,
			 0x40e00000,
			 0x41900000,
			 0x41000000,
			 0x41700000,

			 /// vpu register f4
			 0x40a00000,
			 0x41e00000,
			 0x41d00000,
			 0x41f00000,
			 0x41d00000,
			 0x41f80000,
			 0x41d80000,
			 0x41600000,

			 /// vpu register f5
			 0x41300000,
			 0x41b80000,
			 0x41300000,
			 0x41d00000,
			 0x41500000,
			 0x40800000,
			 0x41600000,
			 0x41100000,

			 /// vpu register f6
			 0x41100000,
			 0x41600000,
			 0x41c00000,
			 0x40c00000,
			 0x41700000,
			 0x41000000,
			 0x41880000,
			 0x41700000,

			 /// vpu register f7
			 0x41b00000,
			 0x41e80000,
			 0x40800000,
			 0x41f00000,
			 0x41000000,
			 0x41c00000,
			 0x3f800000,
			 0x41f00000,

			 /// vpu register f8
			 0x40c00000,
			 0x41a00000,
			 0x41100000,
			 0x41f80000,
			 0x41500000,
			 0x41700000,
			 0x41b80000,
			 0x41800000,

			 /// vpu register f9
			 0x41000000,
			 0x40800000,
			 0x41c00000,
			 0x41500000,
			 0x41c00000,
			 0x41b00000,
			 0x41d00000,
			 0x41980000,

			 /// vpu register f10
			 0x41980000,
			 0x41000000,
			 0x41800000,
			 0x41f00000,
			 0x41b80000,
			 0x41a80000,
			 0x41900000,
			 0x41c00000,

			 /// vpu register f11
			 0x41e80000,
			 0x41800000,
			 0x41e00000,
			 0x41b80000,
			 0x41800000,
			 0x41400000,
			 0x41100000,
			 0x41500000,

			 /// vpu register f12
			 0x41d00000,
			 0x41e80000,
			 0x41d80000,
			 0x40a00000,
			 0x3f800000,
			 0x40c00000,
			 0x41c00000,
			 0x41200000,

			 /// vpu register f13
			 0x41f80000,
			 0x41f00000,
			 0x41880000,
			 0x41800000,
			 0x41400000,
			 0x42000000,
			 0x41800000,
			 0x40800000,

			 /// vpu register f14
			 0x42000000,
			 0x41800000,
			 0x41600000,
			 0x41f00000,
			 0x41200000,
			 0x41400000,
			 0x41f80000,
			 0x41d80000,

			 /// vpu register f15
			 0x41c00000,
			 0x41e80000,
			 0x41a80000,
			 0x41a80000,
			 0x41c00000,
			 0x41980000,
			 0x41600000,
			 0x40e00000,

			 /// vpu register f16
			 0x41e00000,
			 0x41000000,
			 0x40800000,
			 0x41700000,
			 0x42000000,
			 0x41400000,
			 0x41980000,
			 0x41e80000,

			 /// vpu register f17
			 0x41e80000,
			 0x40a00000,
			 0x41e80000,
			 0x41c00000,
			 0x41b00000,
			 0x41100000,
			 0x41300000,
			 0x40c00000,

			 /// vpu register f18
			 0x41900000,
			 0x40c00000,
			 0x40400000,
			 0x41d80000,
			 0x41e80000,
			 0x41700000,
			 0x40400000,
			 0x40a00000,

			 /// vpu register f19
			 0x41400000,
			 0x40400000,
			 0x41800000,
			 0x41880000,
			 0x3f800000,
			 0x41c80000,
			 0x41200000,
			 0x41c80000,

			 /// vpu register f20
			 0x41980000,
			 0x41000000,
			 0x41000000,
			 0x41e00000,
			 0x40c00000,
			 0x41100000,
			 0x41980000,
			 0x41d80000,

			 /// vpu register f21
			 0x40000000,
			 0x41b80000,
			 0x41900000,
			 0x41700000,
			 0x41e80000,
			 0x41100000,
			 0x41900000,
			 0x41000000,

			 /// vpu register f22
			 0x41e80000,
			 0x40e00000,
			 0x41e00000,
			 0x40400000,
			 0x41b80000,
			 0x41900000,
			 0x40e00000,
			 0x41880000,

			 /// vpu register f23
			 0x41c80000,
			 0x41200000,
			 0x42000000,
			 0x41e80000,
			 0x40400000,
			 0x41b80000,
			 0x41500000,
			 0x41300000,

			 /// vpu register f24
			 0x40a00000,
			 0x41700000,
			 0x41b80000,
			 0x40c00000,
			 0x41200000,
			 0x41e80000,
			 0x41f80000,
			 0x41b00000,

			 /// vpu register f25
			 0x40800000,
			 0x40000000,
			 0x41600000,
			 0x41700000,
			 0x41300000,
			 0x40e00000,
			 0x41300000,
			 0x40800000,

			 /// vpu register f26
			 0x40c00000,
			 0x41c00000,
			 0x40800000,
			 0x41f80000,
			 0x41900000,
			 0x41d80000,
			 0x41880000,
			 0x41c00000,

			 /// vpu register f27
			 0x41b00000,
			 0x41f00000,
			 0x41700000,
			 0x41c00000,
			 0x41a00000,
			 0x41880000,
			 0x41c80000,
			 0x41a00000,

			 /// vpu register f28
			 0x41300000,
			 0x40c00000,
			 0x41f80000,
			 0x41a00000,
			 0x41d00000,
			 0x40000000,
			 0x41e80000,
			 0x41a00000,

			 /// vpu register f29
			 0x41d00000,
			 0x41d00000,
			 0x41b80000,
			 0x41000000,
			 0x41100000,
			 0x41d80000,
			 0x41f00000,
			 0x40a00000,

			 /// vpu register f30
			 0x41b80000,
			 0x40800000,
			 0x41400000,
			 0x41800000,
			 0x41f80000,
			 0x41600000,
			 0x41700000,
			 0x41300000,

			 /// vpu register f31
			 0x40000000,
			 0x41200000,
			 0x41e80000,
			 0x41e80000,
			 0x41500000,
			 0x41880000,
			 0x40400000,
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
		"fg32b.ps f28, x11 (x12)\n"                           ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f25, x11 (x12)\n"                           ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f28, x12 (x14)\n"                           ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f26, x13 (x13)\n"                           ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f25, x14 (x12)\n"                           ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f16, x12 (x14)\n"                           ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f30, x15 (x11)\n"                           ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f19, x15 (x12)\n"                           ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f20, x11 (x14)\n"                           ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f17, x13 (x11)\n"                           ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f6, x12 (x13)\n"                            ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f11, x12 (x14)\n"                           ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f27, x14 (x11)\n"                           ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f22, x12 (x14)\n"                           ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f20, x11 (x15)\n"                           ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f28, x13 (x11)\n"                           ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f15, x14 (x11)\n"                           ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f25, x13 (x14)\n"                           ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f20, x13 (x15)\n"                           ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f18, x13 (x11)\n"                           ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f26, x12 (x12)\n"                           ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f13, x13 (x14)\n"                           ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f15, x14 (x11)\n"                           ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f4, x14 (x12)\n"                            ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f2, x15 (x15)\n"                            ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f19, x15 (x14)\n"                           ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f23, x14 (x14)\n"                           ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f14, x12 (x15)\n"                           ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f29, x13 (x15)\n"                           ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f24, x15 (x14)\n"                           ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f28, x15 (x14)\n"                           ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f15, x14 (x13)\n"                           ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f22, x12 (x11)\n"                           ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f5, x13 (x12)\n"                            ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f12, x12 (x14)\n"                           ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f24, x13 (x13)\n"                           ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f11, x11 (x14)\n"                           ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f28, x12 (x12)\n"                           ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f25, x14 (x12)\n"                           ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f12, x15 (x13)\n"                           ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f10, x15 (x14)\n"                           ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f27, x13 (x14)\n"                           ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f3, x12 (x14)\n"                            ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f30, x11 (x13)\n"                           ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f4, x12 (x12)\n"                            ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f30, x13 (x12)\n"                           ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f16, x13 (x14)\n"                           ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f25, x12 (x12)\n"                           ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f29, x11 (x15)\n"                           ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f7, x12 (x13)\n"                            ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f30, x14 (x14)\n"                           ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f6, x11 (x15)\n"                            ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f26, x14 (x13)\n"                           ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f9, x12 (x14)\n"                            ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f16, x14 (x12)\n"                           ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f25, x11 (x12)\n"                           ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f23, x15 (x12)\n"                           ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f7, x11 (x11)\n"                            ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f25, x14 (x11)\n"                           ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f7, x12 (x11)\n"                            ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f10, x13 (x11)\n"                           ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f21, x13 (x14)\n"                           ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f17, x13 (x13)\n"                           ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f16, x12 (x13)\n"                           ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f30, x12 (x15)\n"                           ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f13, x11 (x13)\n"                           ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f5, x15 (x14)\n"                            ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f28, x15 (x11)\n"                           ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f30, x13 (x12)\n"                           ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f5, x11 (x12)\n"                            ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f19, x15 (x12)\n"                           ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f22, x15 (x15)\n"                           ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f27, x14 (x13)\n"                           ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f4, x13 (x15)\n"                            ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f5, x13 (x12)\n"                            ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f23, x12 (x12)\n"                           ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f24, x11 (x12)\n"                           ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f25, x15 (x12)\n"                           ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f15, x11 (x12)\n"                           ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f29, x11 (x13)\n"                           ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f9, x14 (x13)\n"                            ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f25, x12 (x11)\n"                           ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f14, x11 (x13)\n"                           ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f22, x14 (x11)\n"                           ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f16, x15 (x11)\n"                           ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f23, x12 (x13)\n"                           ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f5, x12 (x11)\n"                            ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f28, x12 (x11)\n"                           ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f15, x11 (x15)\n"                           ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f14, x14 (x14)\n"                           ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f17, x12 (x11)\n"                           ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f26, x14 (x11)\n"                           ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f7, x14 (x12)\n"                            ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f15, x14 (x12)\n"                           ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f17, x12 (x14)\n"                           ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f15, x13 (x15)\n"                           ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f27, x14 (x11)\n"                           ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f21, x15 (x15)\n"                           ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fg32b.ps f9, x14 (x14)\n"                            ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
