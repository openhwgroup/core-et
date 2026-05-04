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
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00000
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00004
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0000c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00010
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00014
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00018
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0001c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00020
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00024
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00028
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0002c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00034
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00038
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0003c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00040
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00044
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0004c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00054
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00058
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0005c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00060
			 0x00011111,                                                  // 9.7958E-41                                  /// 00064
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00068
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00070
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00074
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00078
			 0xffc00001,                                                  // -signaling NaN                              /// 0007c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00080
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00088
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0008c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x0e000001,                                                  // Trailing 1s:                                /// 00094
			 0x0c780000,                                                  // Leading 1s:                                 /// 00098
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0009c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00104
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00108
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00110
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00118
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0011c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00120
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00124
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00128
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0012c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00130
			 0xbf028f5c,                                                  // -0.51                                       /// 00134
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00138
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0c400000,                                                  // Leading 1s:                                 /// 00144
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00148
			 0x55555555,                                                  // 4 random values                             /// 0014c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00150
			 0x00000000,                                                  // zero                                        /// 00154
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0015c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00160
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00164
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00168
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0016c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00170
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0017c
			 0x7fc00001,                                                  // signaling NaN                               /// 00180
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00184
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00188
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0018c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00190
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0019c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 001cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001f4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 001fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0020c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00210
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0021c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00220
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00228
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0022c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00230
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00234
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00238
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0023c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00240
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0c600000,                                                  // Leading 1s:                                 /// 00248
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00250
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0025c
			 0x80000000,                                                  // -zero                                       /// 00260
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00264
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00268
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0026c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00270
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00274
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00278
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00280
			 0x0e000007,                                                  // Trailing 1s:                                /// 00284
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00298
			 0x4b000000,                                                  // 8388608.0                                   /// 0029c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002a0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002e8
			 0x00000000,                                                  // zero                                        /// 002ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002fc
			 0x00000000,                                                  // zero                                        /// 00300
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00304
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00308
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0030c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00310
			 0x0c400000,                                                  // Leading 1s:                                 /// 00314
			 0x80011111,                                                  // -9.7958E-41                                 /// 00318
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00320
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00324
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00328
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00330
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00338
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0033c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00340
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00344
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0034c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00350
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00354
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00360
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0036c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0x0c700000,                                                  // Leading 1s:                                 /// 00374
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00380
			 0x0e000007,                                                  // Trailing 1s:                                /// 00384
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00388
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0038c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00390
			 0xbf028f5c,                                                  // -0.51                                       /// 00394
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00398
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0039c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003a8
			 0xcb000000,                                                  // -8388608.0                                  /// 003ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 003c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 003c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 003f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00408
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0040c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00414
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00418
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0041c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00430
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0043c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00440
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00448
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00450
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00454
			 0x0e000007,                                                  // Trailing 1s:                                /// 00458
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0045c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00460
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00468
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00470
			 0xffc00001,                                                  // -signaling NaN                              /// 00474
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00478
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00488
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0048c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00490
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00494
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00498
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 004d4
			 0x80000000,                                                  // -zero                                       /// 004d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 004e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004f0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00500
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00508
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0050c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00510
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0051c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00524
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00528
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00530
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00534
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00538
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0053c
			 0x80000000,                                                  // -zero                                       /// 00540
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00544
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00548
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00550
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0055c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00560
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00564
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00568
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0056c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00570
			 0x33333333,                                                  // 4 random values                             /// 00574
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00578
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0057c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00588
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00598
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0059c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005a0
			 0xbf028f5c,                                                  // -0.51                                       /// 005a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 005ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005dc
			 0xffc00001,                                                  // -signaling NaN                              /// 005e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00600
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00604
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00608
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0060c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00610
			 0x4b000000,                                                  // 8388608.0                                   /// 00614
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0061c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00620
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00624
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00628
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0062c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00634
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00638
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0063c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00644
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00648
			 0xffc00001,                                                  // -signaling NaN                              /// 0064c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x0e000001,                                                  // Trailing 1s:                                /// 0065c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00660
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00668
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0066c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00670
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00678
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00680
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00684
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0068c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00698
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0069c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00704
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0070c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00710
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0071c
			 0xbf028f5c,                                                  // -0.51                                       /// 00720
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00724
			 0x33333333,                                                  // 4 random values                             /// 00728
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00734
			 0x0e000007,                                                  // Trailing 1s:                                /// 00738
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0073c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00740
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00744
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0074c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00750
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00754
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00758
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0075c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00760
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x0e000007,                                                  // Trailing 1s:                                /// 0076c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00770
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00778
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0077c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00788
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00790
			 0x80000000,                                                  // -zero                                       /// 00794
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00798
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 007e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0080c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00810
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00814
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00818
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0081c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0082c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00830
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00834
			 0x55555555,                                                  // 4 random values                             /// 00838
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0083c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0084c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00858
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0085c
			 0xcb000000,                                                  // -8388608.0                                  /// 00860
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00868
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0086c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00870
			 0x00000000,                                                  // zero                                        /// 00874
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00878
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00880
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00884
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00888
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0088c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00898
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0089c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00900
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00904
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00908
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00910
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00918
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00920
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00928
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0092c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00930
			 0xff800000,                                                  // -inf                                        /// 00934
			 0x00000000,                                                  // zero                                        /// 00938
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0093c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00940
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00948
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0094c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00950
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00954
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0096c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00974
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00980
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00984
			 0xcb000000,                                                  // -8388608.0                                  /// 00988
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0098c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00990
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00998
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009a8
			 0xbf028f5c,                                                  // -0.51                                       /// 009ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 009e0
			 0x4b000000,                                                  // 8388608.0                                   /// 009e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009ec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009f8
			 0x80000000,                                                  // -zero                                       /// 009fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a00
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a04
			 0xbf028f5c,                                                  // -0.51                                       /// 00a08
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a18
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a28
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a2c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a38
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a40
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a44
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a50
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a54
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a64
			 0x3f028f5c,                                                  // 0.51                                        /// 00a68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a74
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a7c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a80
			 0x4b000000,                                                  // 8388608.0                                   /// 00a84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00000000,                                                  // zero                                        /// 00a8c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a90
			 0xbf028f5c,                                                  // -0.51                                       /// 00a94
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00aa0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00aa4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab0
			 0x80000000,                                                  // -zero                                       /// 00ab4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ab8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ac4
			 0x80000000,                                                  // -zero                                       /// 00ac8
			 0x7fc00001,                                                  // signaling NaN                               /// 00acc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ad0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ad4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ad8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ae0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ae8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00aec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00af0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00af4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00af8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00afc
			 0xcb000000,                                                  // -8388608.0                                  /// 00b00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0xff800000,                                                  // -inf                                        /// 00b08
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b10
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b28
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x00000000,                                                  // zero                                        /// 00b44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b58
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b64
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b78
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b7c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x4b000000,                                                  // 8388608.0                                   /// 00b84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b90
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b94
			 0xffc00001,                                                  // -signaling NaN                              /// 00b98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b9c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bb0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bb4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bb8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bc4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bcc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bd4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00be0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00be8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bf4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bfc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c18
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c24
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c28
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c2c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c34
			 0xcb000000,                                                  // -8388608.0                                  /// 00c38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x55555555,                                                  // 4 random values                             /// 00c48
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c68
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c88
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c8c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c90
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c98
			 0x7f800000,                                                  // inf                                         /// 00c9c
			 0xff800000,                                                  // -inf                                        /// 00ca0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ca4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ca8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cb0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cb4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cbc
			 0x3f028f5c,                                                  // 0.51                                        /// 00cc0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cc8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ccc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cd0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cd4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ce0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ce4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cf0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cf8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x7fc00001,                                                  // signaling NaN                               /// 00d00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d04
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d14
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d1c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d30
			 0x7f800000,                                                  // inf                                         /// 00d34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d40
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x7fc00001,                                                  // signaling NaN                               /// 00d5c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d88
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x00000000,                                                  // zero                                        /// 00d94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d98
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00da0
			 0xbf028f5c,                                                  // -0.51                                       /// 00da4
			 0xcb000000,                                                  // -8388608.0                                  /// 00da8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00db0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00db4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00db8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dbc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dc0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dc4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dc8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dcc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dd4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00de0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00de4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00df0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00df4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e00
			 0xff800000,                                                  // -inf                                        /// 00e04
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e0c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e18
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e24
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e28
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e44
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e48
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e78
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e80
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e84
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e90
			 0x7fc00001,                                                  // signaling NaN                               /// 00e94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e9c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ea0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00eb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ebc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ec4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ec8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ecc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ed8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00edc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ee0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ee8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ef0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ef4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00efc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f24
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f28
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f34
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f60
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f7c
			 0x33333333,                                                  // 4 random values                             /// 00f80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f84
			 0xff800000,                                                  // -inf                                        /// 00f88
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0xbf028f5c,                                                  // -0.51                                       /// 00fa0
			 0x7f800000,                                                  // inf                                         /// 00fa4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fa8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fac
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fb0
			 0xffc00001,                                                  // -signaling NaN                              /// 00fb4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fbc
			 0x33333333,                                                  // 4 random values                             /// 00fc0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fc4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fc8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fcc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fd0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fdc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fe0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fe4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fe8
			 0x00000000,                                                  // zero                                        /// 00fec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ff0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ff8
			 0x0e000007                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xd06bca36, /// 0x0
			 0xf804b382, /// 0x4
			 0x54415ea9, /// 0x8
			 0xe6940930, /// 0xc
			 0x26e6b894, /// 0x10
			 0xa8127050, /// 0x14
			 0x323b79b0, /// 0x18
			 0x391b749a, /// 0x1c
			 0x1be479dd, /// 0x20
			 0xb61f2acd, /// 0x24
			 0x754b744f, /// 0x28
			 0x088cb2b1, /// 0x2c
			 0x10790f9c, /// 0x30
			 0x552a47b7, /// 0x34
			 0x386b5295, /// 0x38
			 0xc5fc82d2, /// 0x3c
			 0x17c72641, /// 0x40
			 0xdc365ddd, /// 0x44
			 0xd88a35ec, /// 0x48
			 0x4d9f022e, /// 0x4c
			 0xcae9e9e1, /// 0x50
			 0x31fd877a, /// 0x54
			 0x15cd8c99, /// 0x58
			 0x572be001, /// 0x5c
			 0xc2f8e218, /// 0x60
			 0x9d3869da, /// 0x64
			 0x9b79477f, /// 0x68
			 0x204bd48c, /// 0x6c
			 0xd34606fd, /// 0x70
			 0xdcf8eed2, /// 0x74
			 0xed347ca7, /// 0x78
			 0x2f953057, /// 0x7c
			 0x62ec2412, /// 0x80
			 0x178ded21, /// 0x84
			 0x6ca1e9dd, /// 0x88
			 0x67eb9642, /// 0x8c
			 0xe7247078, /// 0x90
			 0xf9f2565e, /// 0x94
			 0xf5f14ce9, /// 0x98
			 0x963e09e3, /// 0x9c
			 0x71df01f4, /// 0xa0
			 0x39153075, /// 0xa4
			 0x0fdd2fe9, /// 0xa8
			 0x4569e3b4, /// 0xac
			 0xeef4d918, /// 0xb0
			 0x89272d5c, /// 0xb4
			 0x843569ad, /// 0xb8
			 0xca071329, /// 0xbc
			 0xd47f3b12, /// 0xc0
			 0xd3407759, /// 0xc4
			 0x0dce1819, /// 0xc8
			 0x0871bea4, /// 0xcc
			 0x13c01fe0, /// 0xd0
			 0x64a4f7c4, /// 0xd4
			 0xbeb676e4, /// 0xd8
			 0x8d0f0cc6, /// 0xdc
			 0x600895f6, /// 0xe0
			 0xe866effc, /// 0xe4
			 0x690c6120, /// 0xe8
			 0x2bab3389, /// 0xec
			 0x054be719, /// 0xf0
			 0xa689a869, /// 0xf4
			 0xe5c3ce2b, /// 0xf8
			 0x0b9a055b, /// 0xfc
			 0x937bf8b0, /// 0x100
			 0xee309ba0, /// 0x104
			 0x4cb8ad30, /// 0x108
			 0x09360e83, /// 0x10c
			 0x09ab4592, /// 0x110
			 0xc1cdda38, /// 0x114
			 0xacac05f7, /// 0x118
			 0x3deeca90, /// 0x11c
			 0x3cb4d799, /// 0x120
			 0xed3c1220, /// 0x124
			 0x054edadc, /// 0x128
			 0x6831ae33, /// 0x12c
			 0xd5b85c47, /// 0x130
			 0xd625fe7c, /// 0x134
			 0x655b5b93, /// 0x138
			 0x3335f0fa, /// 0x13c
			 0x25f3b72b, /// 0x140
			 0xf76b21b2, /// 0x144
			 0x4e5321ef, /// 0x148
			 0x8c72f3cc, /// 0x14c
			 0x156d4462, /// 0x150
			 0x22efd72a, /// 0x154
			 0x0ac5d6db, /// 0x158
			 0x900cc606, /// 0x15c
			 0x2cfb9324, /// 0x160
			 0x200c4090, /// 0x164
			 0x905b3892, /// 0x168
			 0xa854a67d, /// 0x16c
			 0xb91fd3c7, /// 0x170
			 0x76d86212, /// 0x174
			 0xab3a58bd, /// 0x178
			 0x6a9842dc, /// 0x17c
			 0xd3f59b33, /// 0x180
			 0xd384b7cd, /// 0x184
			 0x0d991544, /// 0x188
			 0xd57abe0c, /// 0x18c
			 0x4ef953a1, /// 0x190
			 0x65991baa, /// 0x194
			 0x5e7557a0, /// 0x198
			 0x82415592, /// 0x19c
			 0x5dfb58e1, /// 0x1a0
			 0xf0231088, /// 0x1a4
			 0xb532598b, /// 0x1a8
			 0x8895d279, /// 0x1ac
			 0xe150c9b4, /// 0x1b0
			 0xb90e406c, /// 0x1b4
			 0x6b1ae836, /// 0x1b8
			 0x62ad1ca6, /// 0x1bc
			 0x88a9d546, /// 0x1c0
			 0x0b611be6, /// 0x1c4
			 0x68603521, /// 0x1c8
			 0x3e7dd39f, /// 0x1cc
			 0x56c03827, /// 0x1d0
			 0x4c1ae70c, /// 0x1d4
			 0x2d6fe049, /// 0x1d8
			 0xb33e3b8f, /// 0x1dc
			 0xda71339c, /// 0x1e0
			 0x58042cfe, /// 0x1e4
			 0x0c93b938, /// 0x1e8
			 0x8e5bec20, /// 0x1ec
			 0x658878c9, /// 0x1f0
			 0x0599ce49, /// 0x1f4
			 0x9247e852, /// 0x1f8
			 0x113be388, /// 0x1fc
			 0xc8bef9fb, /// 0x200
			 0xd8ddad58, /// 0x204
			 0xc292a44b, /// 0x208
			 0xe5983d80, /// 0x20c
			 0x148a969a, /// 0x210
			 0x45b1824e, /// 0x214
			 0x2b71294b, /// 0x218
			 0x0f9ea970, /// 0x21c
			 0x9abd59a4, /// 0x220
			 0xc88ba450, /// 0x224
			 0xe6ede114, /// 0x228
			 0x4c08bc14, /// 0x22c
			 0xa558ce80, /// 0x230
			 0x4cc8e2a5, /// 0x234
			 0x5eadefea, /// 0x238
			 0x732b3e6c, /// 0x23c
			 0x9567f61a, /// 0x240
			 0xdfe3f53d, /// 0x244
			 0x9c37d6e7, /// 0x248
			 0x0d3b7d26, /// 0x24c
			 0x1b290ab7, /// 0x250
			 0x937c3b83, /// 0x254
			 0x27745dfa, /// 0x258
			 0x24858610, /// 0x25c
			 0x067afef4, /// 0x260
			 0x0e3fe9ff, /// 0x264
			 0xfcfec063, /// 0x268
			 0xb2c14132, /// 0x26c
			 0x9153f04d, /// 0x270
			 0x9d216f1a, /// 0x274
			 0x49a7d319, /// 0x278
			 0xd4d75411, /// 0x27c
			 0xb1acedc3, /// 0x280
			 0x0d2682da, /// 0x284
			 0x7afba684, /// 0x288
			 0xe9787471, /// 0x28c
			 0x51f7a509, /// 0x290
			 0x684dabdb, /// 0x294
			 0xff499728, /// 0x298
			 0x84353eba, /// 0x29c
			 0x8a5f1ccd, /// 0x2a0
			 0x5a5cb016, /// 0x2a4
			 0x197f0151, /// 0x2a8
			 0x567c6b30, /// 0x2ac
			 0xbd1ba79a, /// 0x2b0
			 0x9c043676, /// 0x2b4
			 0xa50bf8e8, /// 0x2b8
			 0x0df0c6c0, /// 0x2bc
			 0x5b2db4e6, /// 0x2c0
			 0xaffd24a6, /// 0x2c4
			 0x799f4e17, /// 0x2c8
			 0x83703eab, /// 0x2cc
			 0x6f0cba62, /// 0x2d0
			 0x2385f55e, /// 0x2d4
			 0xfbb7b6f8, /// 0x2d8
			 0x102b21bb, /// 0x2dc
			 0xe9482f76, /// 0x2e0
			 0x398bac39, /// 0x2e4
			 0x33a9ec23, /// 0x2e8
			 0xd50d2eb8, /// 0x2ec
			 0x0ba882cf, /// 0x2f0
			 0x1c448fc9, /// 0x2f4
			 0xdf20af45, /// 0x2f8
			 0x72be0fb9, /// 0x2fc
			 0xb313122d, /// 0x300
			 0x8eaa97e1, /// 0x304
			 0xdbdd4fc4, /// 0x308
			 0x79a97fee, /// 0x30c
			 0x54b23699, /// 0x310
			 0x5d2fd1c8, /// 0x314
			 0x8c39e011, /// 0x318
			 0x6985010c, /// 0x31c
			 0x8878c244, /// 0x320
			 0x711fd6e7, /// 0x324
			 0x92f92df3, /// 0x328
			 0x0f15b749, /// 0x32c
			 0xb30d4f77, /// 0x330
			 0xe45cefa2, /// 0x334
			 0x5de60316, /// 0x338
			 0x65a16a09, /// 0x33c
			 0x673e4413, /// 0x340
			 0xe4aea56f, /// 0x344
			 0x8f32012b, /// 0x348
			 0x29859296, /// 0x34c
			 0x24eaf240, /// 0x350
			 0xde3f4e08, /// 0x354
			 0xed97a251, /// 0x358
			 0xaf685e6f, /// 0x35c
			 0xb971fff6, /// 0x360
			 0xc48c2ff3, /// 0x364
			 0xef29c1dc, /// 0x368
			 0x01a45c0d, /// 0x36c
			 0x2b01c0c4, /// 0x370
			 0xaebf00cd, /// 0x374
			 0x8847d985, /// 0x378
			 0xa40d7e42, /// 0x37c
			 0xbd07ea32, /// 0x380
			 0x817c87c5, /// 0x384
			 0xfe872467, /// 0x388
			 0xcb75733c, /// 0x38c
			 0x96b8bf42, /// 0x390
			 0x68a9e9e2, /// 0x394
			 0x43a2c84f, /// 0x398
			 0x59304215, /// 0x39c
			 0xa4bb1b84, /// 0x3a0
			 0xf34513fc, /// 0x3a4
			 0xbb69c330, /// 0x3a8
			 0xb8395f71, /// 0x3ac
			 0xa7648593, /// 0x3b0
			 0x3ed1fcef, /// 0x3b4
			 0xb0180cdf, /// 0x3b8
			 0xa7857fc3, /// 0x3bc
			 0x2e5c9edb, /// 0x3c0
			 0xf5ad678c, /// 0x3c4
			 0xb508d510, /// 0x3c8
			 0x89c9f20e, /// 0x3cc
			 0xb2412758, /// 0x3d0
			 0xfc420faf, /// 0x3d4
			 0xe24f30e2, /// 0x3d8
			 0x4e9b169c, /// 0x3dc
			 0x6f1815db, /// 0x3e0
			 0x28b5d18e, /// 0x3e4
			 0xb7991ad3, /// 0x3e8
			 0x6c089317, /// 0x3ec
			 0x5541621d, /// 0x3f0
			 0x93a5dd50, /// 0x3f4
			 0xb575e46e, /// 0x3f8
			 0x410d1290, /// 0x3fc
			 0x9c1be387, /// 0x400
			 0x7645f3ae, /// 0x404
			 0x9f5520c9, /// 0x408
			 0xc5450584, /// 0x40c
			 0x737c525d, /// 0x410
			 0x5cfa651f, /// 0x414
			 0x84954a05, /// 0x418
			 0xf8dde0fe, /// 0x41c
			 0x65b4535b, /// 0x420
			 0x3fe28b1f, /// 0x424
			 0x2ca9130a, /// 0x428
			 0xb21cdf3a, /// 0x42c
			 0xcb2176a0, /// 0x430
			 0x9e5ad4dd, /// 0x434
			 0x7a9769a2, /// 0x438
			 0x1f2709e5, /// 0x43c
			 0xe7af09c7, /// 0x440
			 0xfb1c8b0e, /// 0x444
			 0x8373f71e, /// 0x448
			 0x3df98d7b, /// 0x44c
			 0x5848a221, /// 0x450
			 0x4de16d77, /// 0x454
			 0xe904cdab, /// 0x458
			 0x321636d8, /// 0x45c
			 0x4f1272a0, /// 0x460
			 0xc11282a4, /// 0x464
			 0x4d3296ad, /// 0x468
			 0xf1ddee24, /// 0x46c
			 0x8c68deda, /// 0x470
			 0x7d9df5dc, /// 0x474
			 0x423cd2d1, /// 0x478
			 0x20f0ef35, /// 0x47c
			 0x9ca3ce03, /// 0x480
			 0x34bd613e, /// 0x484
			 0xce7fa878, /// 0x488
			 0x849caaa4, /// 0x48c
			 0x99df9678, /// 0x490
			 0xb5eae423, /// 0x494
			 0x5b0af0a5, /// 0x498
			 0xdf33843f, /// 0x49c
			 0xef4d9031, /// 0x4a0
			 0x2e39daf6, /// 0x4a4
			 0xa9c56abe, /// 0x4a8
			 0xc96f701a, /// 0x4ac
			 0xad73da71, /// 0x4b0
			 0xce8607f0, /// 0x4b4
			 0x64d4e413, /// 0x4b8
			 0x98a2854a, /// 0x4bc
			 0x7d21a1f7, /// 0x4c0
			 0x5edc14dc, /// 0x4c4
			 0xe2405cf1, /// 0x4c8
			 0xfa5c2518, /// 0x4cc
			 0x84ded2df, /// 0x4d0
			 0x24e5c7a3, /// 0x4d4
			 0x621a3b5f, /// 0x4d8
			 0xc0bc4d51, /// 0x4dc
			 0x6d2fae9c, /// 0x4e0
			 0x7ed5c690, /// 0x4e4
			 0x067027f1, /// 0x4e8
			 0x6bb5604c, /// 0x4ec
			 0x1c359081, /// 0x4f0
			 0x4a5745b4, /// 0x4f4
			 0xd9addebc, /// 0x4f8
			 0x73a581e7, /// 0x4fc
			 0x4045a492, /// 0x500
			 0xb60705fc, /// 0x504
			 0xf82cccfd, /// 0x508
			 0x4fca6382, /// 0x50c
			 0xf9d74e3b, /// 0x510
			 0x068417e0, /// 0x514
			 0xd31fb808, /// 0x518
			 0x4aaa37b9, /// 0x51c
			 0x09c42460, /// 0x520
			 0x11dc19c2, /// 0x524
			 0xf191a70c, /// 0x528
			 0x79edec65, /// 0x52c
			 0x34488cb6, /// 0x530
			 0x7df4f932, /// 0x534
			 0xc3f5fd24, /// 0x538
			 0xc43d6f1b, /// 0x53c
			 0xa407ac12, /// 0x540
			 0x3fe3998d, /// 0x544
			 0x3882fec1, /// 0x548
			 0x2f0d17f3, /// 0x54c
			 0x07d6d243, /// 0x550
			 0x3c214023, /// 0x554
			 0x161cb9ac, /// 0x558
			 0x5658f2d5, /// 0x55c
			 0xafea1324, /// 0x560
			 0x3a64a2f5, /// 0x564
			 0xa4ee8940, /// 0x568
			 0x7b941ca8, /// 0x56c
			 0x261d022d, /// 0x570
			 0xca1ee9df, /// 0x574
			 0x62467375, /// 0x578
			 0x6cbfc444, /// 0x57c
			 0x8b657ac3, /// 0x580
			 0x22ea3583, /// 0x584
			 0x824c0fe6, /// 0x588
			 0x710dcb16, /// 0x58c
			 0x339c536f, /// 0x590
			 0x699c3d3c, /// 0x594
			 0xc39135de, /// 0x598
			 0xbeac6158, /// 0x59c
			 0x65d0af62, /// 0x5a0
			 0x4cbc42e4, /// 0x5a4
			 0x319b065c, /// 0x5a8
			 0xcbf24da3, /// 0x5ac
			 0x716ac453, /// 0x5b0
			 0xd667b880, /// 0x5b4
			 0xb29ee8ac, /// 0x5b8
			 0xe3196f01, /// 0x5bc
			 0x0bb748eb, /// 0x5c0
			 0xefd9df34, /// 0x5c4
			 0xf488b5e7, /// 0x5c8
			 0xa836ea9c, /// 0x5cc
			 0x10ea68e5, /// 0x5d0
			 0x1d4d6171, /// 0x5d4
			 0x60568b81, /// 0x5d8
			 0xf5d23df5, /// 0x5dc
			 0x37114d59, /// 0x5e0
			 0xc6cd8163, /// 0x5e4
			 0xcda7a8b7, /// 0x5e8
			 0xa307ba41, /// 0x5ec
			 0x3a78d830, /// 0x5f0
			 0x321c4490, /// 0x5f4
			 0x7321394d, /// 0x5f8
			 0xe1216571, /// 0x5fc
			 0x43072b19, /// 0x600
			 0xe6935736, /// 0x604
			 0x923ff557, /// 0x608
			 0x3815e750, /// 0x60c
			 0xee23836a, /// 0x610
			 0xc68babb9, /// 0x614
			 0xc44294cd, /// 0x618
			 0x359b0739, /// 0x61c
			 0x2c51c957, /// 0x620
			 0xf7ea9fef, /// 0x624
			 0x8568264b, /// 0x628
			 0x721b258e, /// 0x62c
			 0xe832a4e1, /// 0x630
			 0x9789ec52, /// 0x634
			 0x5d316906, /// 0x638
			 0x5c7bce68, /// 0x63c
			 0x997c2819, /// 0x640
			 0xb908056b, /// 0x644
			 0x819fea6e, /// 0x648
			 0x15c7b2ea, /// 0x64c
			 0x677c319e, /// 0x650
			 0xa8af15aa, /// 0x654
			 0x27fe807a, /// 0x658
			 0xfcc3af8a, /// 0x65c
			 0x86269869, /// 0x660
			 0xf03f098c, /// 0x664
			 0xb2fc2b4b, /// 0x668
			 0xe6d1a910, /// 0x66c
			 0x98311561, /// 0x670
			 0xa43e7ede, /// 0x674
			 0xb8f2dd2a, /// 0x678
			 0xf0ca79a7, /// 0x67c
			 0xee86575c, /// 0x680
			 0xa2522953, /// 0x684
			 0xb9333955, /// 0x688
			 0x4528a474, /// 0x68c
			 0x1a8f1374, /// 0x690
			 0x4ae11fd9, /// 0x694
			 0xd84a2886, /// 0x698
			 0xb10d24bc, /// 0x69c
			 0x1a60b606, /// 0x6a0
			 0x357998c7, /// 0x6a4
			 0xfe007b8a, /// 0x6a8
			 0x9373b53f, /// 0x6ac
			 0x3d0cb8d6, /// 0x6b0
			 0x7d812dd5, /// 0x6b4
			 0xdfe2d75b, /// 0x6b8
			 0x86fa6aa4, /// 0x6bc
			 0x6f4d7f31, /// 0x6c0
			 0xf6f98ad1, /// 0x6c4
			 0x92175f52, /// 0x6c8
			 0x3c18c2c3, /// 0x6cc
			 0xe6ef3ed8, /// 0x6d0
			 0xf2baad86, /// 0x6d4
			 0xd75ff9e4, /// 0x6d8
			 0x1ab0b72a, /// 0x6dc
			 0x7f6abce2, /// 0x6e0
			 0x04a009b2, /// 0x6e4
			 0x9fee4167, /// 0x6e8
			 0xbca4d5f7, /// 0x6ec
			 0x592c1fcb, /// 0x6f0
			 0x911ea2ef, /// 0x6f4
			 0xd5bb4536, /// 0x6f8
			 0xdf818e98, /// 0x6fc
			 0x3a90a753, /// 0x700
			 0xe3732c06, /// 0x704
			 0x16440b25, /// 0x708
			 0xd7361319, /// 0x70c
			 0x3c440263, /// 0x710
			 0x238d1393, /// 0x714
			 0x99acd61b, /// 0x718
			 0x1c9e9cae, /// 0x71c
			 0xbaad520c, /// 0x720
			 0x776b2271, /// 0x724
			 0x1f37c4f3, /// 0x728
			 0xe2800573, /// 0x72c
			 0x3639950e, /// 0x730
			 0x2c5d7488, /// 0x734
			 0x1bde3507, /// 0x738
			 0x5d717a72, /// 0x73c
			 0xe6de952f, /// 0x740
			 0x0bc56ad4, /// 0x744
			 0x0f561277, /// 0x748
			 0x08e611e4, /// 0x74c
			 0x3dff12d8, /// 0x750
			 0xc347dda1, /// 0x754
			 0x7619d0ae, /// 0x758
			 0x431c935f, /// 0x75c
			 0x8725819f, /// 0x760
			 0x2f5ce66a, /// 0x764
			 0x2aeab50d, /// 0x768
			 0xf1181b66, /// 0x76c
			 0xa2f8e8d7, /// 0x770
			 0x6dbdf527, /// 0x774
			 0xa620f4a5, /// 0x778
			 0x18d9c551, /// 0x77c
			 0x8476eebf, /// 0x780
			 0x76d06d75, /// 0x784
			 0x7e58b133, /// 0x788
			 0x97d8d790, /// 0x78c
			 0x58974b1c, /// 0x790
			 0x6caed51f, /// 0x794
			 0xe41c83ea, /// 0x798
			 0x2fe114b0, /// 0x79c
			 0xe8405214, /// 0x7a0
			 0xae540da1, /// 0x7a4
			 0x4a2d8a13, /// 0x7a8
			 0xa6d91f18, /// 0x7ac
			 0x74c0d450, /// 0x7b0
			 0x1c512fc9, /// 0x7b4
			 0x64887f9a, /// 0x7b8
			 0xd1c68302, /// 0x7bc
			 0xeffca948, /// 0x7c0
			 0x52abf83d, /// 0x7c4
			 0xec95eedd, /// 0x7c8
			 0xb0c55934, /// 0x7cc
			 0xf54b33fc, /// 0x7d0
			 0xba3cf66a, /// 0x7d4
			 0xde780ea8, /// 0x7d8
			 0xe3ebe069, /// 0x7dc
			 0x58d1f442, /// 0x7e0
			 0xa1228518, /// 0x7e4
			 0xb02a1dbf, /// 0x7e8
			 0xa722b954, /// 0x7ec
			 0x1215c5a4, /// 0x7f0
			 0xb8a7bcad, /// 0x7f4
			 0xc791891c, /// 0x7f8
			 0xa60075c5, /// 0x7fc
			 0x6e66a8ea, /// 0x800
			 0xa9aaa799, /// 0x804
			 0xf1cdea3d, /// 0x808
			 0xdaeb8032, /// 0x80c
			 0xef57c25d, /// 0x810
			 0xa99e290c, /// 0x814
			 0xb0441023, /// 0x818
			 0x162cf879, /// 0x81c
			 0x2664f3be, /// 0x820
			 0xedb68010, /// 0x824
			 0x0e74a93f, /// 0x828
			 0x6f80328d, /// 0x82c
			 0x85850602, /// 0x830
			 0xd0b4c9be, /// 0x834
			 0x8e290254, /// 0x838
			 0xa08efcae, /// 0x83c
			 0x7621015c, /// 0x840
			 0xa64581ab, /// 0x844
			 0x35783325, /// 0x848
			 0x16839f90, /// 0x84c
			 0xe2834301, /// 0x850
			 0xd811878a, /// 0x854
			 0xe3614d44, /// 0x858
			 0xa3bb9664, /// 0x85c
			 0x310627fa, /// 0x860
			 0xf704981d, /// 0x864
			 0xb1fc6f12, /// 0x868
			 0xa1080442, /// 0x86c
			 0x31b63d55, /// 0x870
			 0xbf8f8984, /// 0x874
			 0xecc1f83a, /// 0x878
			 0x1a9ed84a, /// 0x87c
			 0x9da24d47, /// 0x880
			 0x313fe86b, /// 0x884
			 0xbf40a663, /// 0x888
			 0x0cf389c2, /// 0x88c
			 0x2ee89b4c, /// 0x890
			 0xa9adbfc1, /// 0x894
			 0x873e275b, /// 0x898
			 0xfef79498, /// 0x89c
			 0x47f5b653, /// 0x8a0
			 0x8f94619a, /// 0x8a4
			 0x6e1ead0b, /// 0x8a8
			 0x1bff3cbd, /// 0x8ac
			 0xc50e64ce, /// 0x8b0
			 0xc6d613ce, /// 0x8b4
			 0x05945b23, /// 0x8b8
			 0xefb01135, /// 0x8bc
			 0x388276b2, /// 0x8c0
			 0x939010b8, /// 0x8c4
			 0x4cfa4b92, /// 0x8c8
			 0x5ebfa5f2, /// 0x8cc
			 0xcabff89e, /// 0x8d0
			 0x9862ab60, /// 0x8d4
			 0x4e64f985, /// 0x8d8
			 0x401d6e15, /// 0x8dc
			 0x8c6b734b, /// 0x8e0
			 0xcff73583, /// 0x8e4
			 0x49775db3, /// 0x8e8
			 0xd83ba049, /// 0x8ec
			 0x41ed4df6, /// 0x8f0
			 0xc97b2a97, /// 0x8f4
			 0xbd559387, /// 0x8f8
			 0xd6a12de5, /// 0x8fc
			 0x8b2d2503, /// 0x900
			 0x2d43ee72, /// 0x904
			 0xd7b3f8b6, /// 0x908
			 0x793fd3cd, /// 0x90c
			 0x2e934464, /// 0x910
			 0x880eaed8, /// 0x914
			 0x92929c73, /// 0x918
			 0x0e9db363, /// 0x91c
			 0x057fb1f6, /// 0x920
			 0x807ba4ad, /// 0x924
			 0x90a0f4f7, /// 0x928
			 0x17d47a94, /// 0x92c
			 0x504f0869, /// 0x930
			 0xcf4c9138, /// 0x934
			 0x4bd75d92, /// 0x938
			 0xc44e4a27, /// 0x93c
			 0x18dba34c, /// 0x940
			 0x22c77d96, /// 0x944
			 0x74c6d3f8, /// 0x948
			 0xc73cd8a5, /// 0x94c
			 0xf47c6047, /// 0x950
			 0xf83ccbff, /// 0x954
			 0x1ee7776c, /// 0x958
			 0xf1c3f71e, /// 0x95c
			 0x6234fa8a, /// 0x960
			 0x28bb6b96, /// 0x964
			 0xbd31d6cc, /// 0x968
			 0xbb7eb1a5, /// 0x96c
			 0x08efffaf, /// 0x970
			 0xb6836640, /// 0x974
			 0xcf8653d1, /// 0x978
			 0xc7b63038, /// 0x97c
			 0xa8707f44, /// 0x980
			 0x43087753, /// 0x984
			 0x42d2ac86, /// 0x988
			 0xfaa17c33, /// 0x98c
			 0x4da3e82c, /// 0x990
			 0x603d126f, /// 0x994
			 0x0653e724, /// 0x998
			 0x422606b1, /// 0x99c
			 0xead5d8a0, /// 0x9a0
			 0x14d9a65d, /// 0x9a4
			 0x3b2c8df9, /// 0x9a8
			 0x8c6879aa, /// 0x9ac
			 0x0cb665fc, /// 0x9b0
			 0x1c9053d9, /// 0x9b4
			 0xb99760e3, /// 0x9b8
			 0xfb9fe0cb, /// 0x9bc
			 0x858c61ef, /// 0x9c0
			 0x89a0870f, /// 0x9c4
			 0x73c96b99, /// 0x9c8
			 0x98cb6672, /// 0x9cc
			 0xea520d4b, /// 0x9d0
			 0x2dda7e43, /// 0x9d4
			 0x1c085d46, /// 0x9d8
			 0x645e7e33, /// 0x9dc
			 0x0efa9756, /// 0x9e0
			 0x69ccfce7, /// 0x9e4
			 0xe958a49d, /// 0x9e8
			 0xe1b8cbf1, /// 0x9ec
			 0xa5202ccb, /// 0x9f0
			 0x02e4d47a, /// 0x9f4
			 0x0a6c896e, /// 0x9f8
			 0x7d76aea1, /// 0x9fc
			 0x91739b42, /// 0xa00
			 0x620073d6, /// 0xa04
			 0xda4718a9, /// 0xa08
			 0x29215fc2, /// 0xa0c
			 0x4ef155c5, /// 0xa10
			 0x7cd70588, /// 0xa14
			 0xa039d9d4, /// 0xa18
			 0xf09eca78, /// 0xa1c
			 0xf6c5d627, /// 0xa20
			 0xca972927, /// 0xa24
			 0xf04b2a01, /// 0xa28
			 0x39ce222a, /// 0xa2c
			 0x470d5018, /// 0xa30
			 0x073136d5, /// 0xa34
			 0xa568b513, /// 0xa38
			 0x91bce79d, /// 0xa3c
			 0x3b62b85e, /// 0xa40
			 0x643fbc6f, /// 0xa44
			 0xc31a4d3e, /// 0xa48
			 0x7e2644f0, /// 0xa4c
			 0x865198de, /// 0xa50
			 0x7bf148e1, /// 0xa54
			 0x4ea9cfe6, /// 0xa58
			 0xd9830818, /// 0xa5c
			 0x80bea174, /// 0xa60
			 0x54ecfacd, /// 0xa64
			 0xb6bfd0f1, /// 0xa68
			 0x065c1c9c, /// 0xa6c
			 0xc0b03ed4, /// 0xa70
			 0x72f4dc6c, /// 0xa74
			 0x4d206b54, /// 0xa78
			 0xf9a01cee, /// 0xa7c
			 0xae1b7439, /// 0xa80
			 0x5753d305, /// 0xa84
			 0x1baa530b, /// 0xa88
			 0xa25bcc5b, /// 0xa8c
			 0x20829960, /// 0xa90
			 0xc061ffbb, /// 0xa94
			 0x96b88ddd, /// 0xa98
			 0x10150af5, /// 0xa9c
			 0xfebadeb0, /// 0xaa0
			 0x0064eb66, /// 0xaa4
			 0xfe5927ed, /// 0xaa8
			 0x1336a6cf, /// 0xaac
			 0x04a2cd28, /// 0xab0
			 0x5b7baf92, /// 0xab4
			 0xcf5586ae, /// 0xab8
			 0x44fd5f9a, /// 0xabc
			 0x7149f1b1, /// 0xac0
			 0xdefeea52, /// 0xac4
			 0x02d79391, /// 0xac8
			 0xbd8b63de, /// 0xacc
			 0x37a9a7ba, /// 0xad0
			 0x5e8a6c01, /// 0xad4
			 0xb2bfe9d4, /// 0xad8
			 0x4acf8ebd, /// 0xadc
			 0x068ff973, /// 0xae0
			 0x75638596, /// 0xae4
			 0x6559b9a2, /// 0xae8
			 0xe4e4d868, /// 0xaec
			 0x96103d7d, /// 0xaf0
			 0x691bcde7, /// 0xaf4
			 0xd31876b9, /// 0xaf8
			 0xfa366d03, /// 0xafc
			 0x2c1d7e5a, /// 0xb00
			 0x4d0b93e1, /// 0xb04
			 0x023c2f4e, /// 0xb08
			 0xc8c45595, /// 0xb0c
			 0x72717d8d, /// 0xb10
			 0x17d60577, /// 0xb14
			 0xbf8ec069, /// 0xb18
			 0x69e94f7e, /// 0xb1c
			 0xd77bc7b3, /// 0xb20
			 0x45ca69b5, /// 0xb24
			 0x066eb5f6, /// 0xb28
			 0xcb60b773, /// 0xb2c
			 0x8cd97759, /// 0xb30
			 0x81fb30a3, /// 0xb34
			 0x30dcd610, /// 0xb38
			 0x2f9ca9ba, /// 0xb3c
			 0x6a72301e, /// 0xb40
			 0x5ce50a78, /// 0xb44
			 0xa0509b97, /// 0xb48
			 0xfa489577, /// 0xb4c
			 0xefeb0d66, /// 0xb50
			 0x5233ec10, /// 0xb54
			 0x4d97f8ca, /// 0xb58
			 0xbf69f0ef, /// 0xb5c
			 0x06cbf25d, /// 0xb60
			 0x3ba05783, /// 0xb64
			 0xad35364e, /// 0xb68
			 0x968aa933, /// 0xb6c
			 0x7fd2d9ac, /// 0xb70
			 0x26083350, /// 0xb74
			 0x285c4626, /// 0xb78
			 0x11b3fd5b, /// 0xb7c
			 0x2cd6b334, /// 0xb80
			 0x77ae29bb, /// 0xb84
			 0xfc02c09f, /// 0xb88
			 0x86875835, /// 0xb8c
			 0xb6812090, /// 0xb90
			 0x95be57e1, /// 0xb94
			 0xa177e112, /// 0xb98
			 0x50ec4c0d, /// 0xb9c
			 0x793a1466, /// 0xba0
			 0xe147018f, /// 0xba4
			 0x242edac3, /// 0xba8
			 0xa7d9b730, /// 0xbac
			 0x3f0492fb, /// 0xbb0
			 0xd730a532, /// 0xbb4
			 0x2250f192, /// 0xbb8
			 0x34401f58, /// 0xbbc
			 0xd34e4491, /// 0xbc0
			 0x34c78611, /// 0xbc4
			 0x25f2b532, /// 0xbc8
			 0x3353758b, /// 0xbcc
			 0xcc0b63f0, /// 0xbd0
			 0x67db33e9, /// 0xbd4
			 0x6f8e98d6, /// 0xbd8
			 0xd0d52f79, /// 0xbdc
			 0x9cfe0b0d, /// 0xbe0
			 0x1b44631f, /// 0xbe4
			 0x7e38da24, /// 0xbe8
			 0xe89c1182, /// 0xbec
			 0x11a774a0, /// 0xbf0
			 0xdac0ff1a, /// 0xbf4
			 0xd7fdc243, /// 0xbf8
			 0x940840ff, /// 0xbfc
			 0x7eb3cd0b, /// 0xc00
			 0x4f6b89c4, /// 0xc04
			 0x2651cdfc, /// 0xc08
			 0x7cb518ef, /// 0xc0c
			 0xbda34ac3, /// 0xc10
			 0x28c77f20, /// 0xc14
			 0x877e0748, /// 0xc18
			 0x45b4867c, /// 0xc1c
			 0xe33c46ad, /// 0xc20
			 0x6086cf4e, /// 0xc24
			 0x376cb1a5, /// 0xc28
			 0x56571d16, /// 0xc2c
			 0xe8c697b1, /// 0xc30
			 0x48776c4b, /// 0xc34
			 0xeb7755f2, /// 0xc38
			 0x17c192e9, /// 0xc3c
			 0x2d97cc40, /// 0xc40
			 0xc0473660, /// 0xc44
			 0x1c81fe04, /// 0xc48
			 0x751cf5c8, /// 0xc4c
			 0xc8e0a27c, /// 0xc50
			 0x2adbcf9c, /// 0xc54
			 0x415eefc7, /// 0xc58
			 0x912e0ca9, /// 0xc5c
			 0x62000fd2, /// 0xc60
			 0xb8a1d2c2, /// 0xc64
			 0x45429bbe, /// 0xc68
			 0xd48f2868, /// 0xc6c
			 0x1bb27adb, /// 0xc70
			 0xd915bbff, /// 0xc74
			 0xe88b0670, /// 0xc78
			 0x714b7944, /// 0xc7c
			 0x6c8f4f4f, /// 0xc80
			 0x71c13c86, /// 0xc84
			 0xec71fa49, /// 0xc88
			 0xc82bb0d4, /// 0xc8c
			 0xcf59f37a, /// 0xc90
			 0xc0558164, /// 0xc94
			 0x5995d3ce, /// 0xc98
			 0x385b7dd7, /// 0xc9c
			 0x5fab8d49, /// 0xca0
			 0x0444325f, /// 0xca4
			 0x5ad9eb90, /// 0xca8
			 0x06aefc39, /// 0xcac
			 0xc8a3953a, /// 0xcb0
			 0xbf2833ad, /// 0xcb4
			 0x8db14009, /// 0xcb8
			 0x18ab8f40, /// 0xcbc
			 0xbcb4b5a7, /// 0xcc0
			 0xf1a12d37, /// 0xcc4
			 0x2fb6a32a, /// 0xcc8
			 0x32884dd0, /// 0xccc
			 0x76e84a71, /// 0xcd0
			 0xeef3732b, /// 0xcd4
			 0x9957c3ed, /// 0xcd8
			 0x0a6bdbf6, /// 0xcdc
			 0x24e14116, /// 0xce0
			 0xcdeb7496, /// 0xce4
			 0x50f9e28c, /// 0xce8
			 0xc19732a7, /// 0xcec
			 0xc1138aa5, /// 0xcf0
			 0xa67cc056, /// 0xcf4
			 0x0f0d3643, /// 0xcf8
			 0xe73b7716, /// 0xcfc
			 0x4246f2d7, /// 0xd00
			 0x1489f3fc, /// 0xd04
			 0x59dab2d8, /// 0xd08
			 0x71b437e3, /// 0xd0c
			 0xb44ff6d2, /// 0xd10
			 0x9ead726d, /// 0xd14
			 0x4588ece4, /// 0xd18
			 0x97b3f5ca, /// 0xd1c
			 0x76c375d1, /// 0xd20
			 0xea51db69, /// 0xd24
			 0x265ef3b7, /// 0xd28
			 0x5f1d46ea, /// 0xd2c
			 0x2d56614d, /// 0xd30
			 0xb807119c, /// 0xd34
			 0x2cf8a8ff, /// 0xd38
			 0x31ead93b, /// 0xd3c
			 0xff6d0f26, /// 0xd40
			 0x857f2e1b, /// 0xd44
			 0x362c59ad, /// 0xd48
			 0xb3e63b90, /// 0xd4c
			 0x3915ffd7, /// 0xd50
			 0xe3630711, /// 0xd54
			 0x72e9744b, /// 0xd58
			 0x11cec995, /// 0xd5c
			 0xc1bd16fa, /// 0xd60
			 0x155bdb00, /// 0xd64
			 0xbe44b95a, /// 0xd68
			 0x7358aec3, /// 0xd6c
			 0xbefd73a5, /// 0xd70
			 0x336cef5b, /// 0xd74
			 0x7cc88ee1, /// 0xd78
			 0x77a3422a, /// 0xd7c
			 0x56d3a0d3, /// 0xd80
			 0x29022aa4, /// 0xd84
			 0x7cfef6c4, /// 0xd88
			 0xcc329a81, /// 0xd8c
			 0x831b012f, /// 0xd90
			 0x3536c5f6, /// 0xd94
			 0x8ceacae6, /// 0xd98
			 0xddc21748, /// 0xd9c
			 0x1b8c3c9a, /// 0xda0
			 0x7a9f1d23, /// 0xda4
			 0x9ff7f702, /// 0xda8
			 0x0ff11e28, /// 0xdac
			 0x218a3724, /// 0xdb0
			 0x9a36f48f, /// 0xdb4
			 0xd053121e, /// 0xdb8
			 0x0159dc2c, /// 0xdbc
			 0x896a9729, /// 0xdc0
			 0x75444717, /// 0xdc4
			 0x6d7aeaf8, /// 0xdc8
			 0x722a8c1d, /// 0xdcc
			 0xbf874053, /// 0xdd0
			 0x3eb78844, /// 0xdd4
			 0x550953e9, /// 0xdd8
			 0xee726fc7, /// 0xddc
			 0x634514d7, /// 0xde0
			 0x4c56dd27, /// 0xde4
			 0x1aac3a23, /// 0xde8
			 0x7beade48, /// 0xdec
			 0x8f2cd2f7, /// 0xdf0
			 0x09be11a5, /// 0xdf4
			 0x83825f4d, /// 0xdf8
			 0x9de0d793, /// 0xdfc
			 0xd00cec1d, /// 0xe00
			 0x85a47ad0, /// 0xe04
			 0x8e16e2a2, /// 0xe08
			 0xd44ab1d6, /// 0xe0c
			 0x8405333a, /// 0xe10
			 0xa3bd87a7, /// 0xe14
			 0xac40f52e, /// 0xe18
			 0x6a671671, /// 0xe1c
			 0x8258f267, /// 0xe20
			 0x8fd4f146, /// 0xe24
			 0xbcb98d60, /// 0xe28
			 0x9d6fa93f, /// 0xe2c
			 0xdac779ad, /// 0xe30
			 0xdbbda4d7, /// 0xe34
			 0x9c72aeaa, /// 0xe38
			 0x197df4f4, /// 0xe3c
			 0x661d2dfc, /// 0xe40
			 0x9fe163d9, /// 0xe44
			 0x58ab0395, /// 0xe48
			 0xddcaeb29, /// 0xe4c
			 0xac4d3467, /// 0xe50
			 0x7541d23b, /// 0xe54
			 0xdc1654cd, /// 0xe58
			 0x5993a298, /// 0xe5c
			 0xec530cbd, /// 0xe60
			 0xa6afa9c2, /// 0xe64
			 0x6c2992c1, /// 0xe68
			 0x325aa80d, /// 0xe6c
			 0x148d45c6, /// 0xe70
			 0xc11c16f0, /// 0xe74
			 0x07524d1a, /// 0xe78
			 0xbbc5b689, /// 0xe7c
			 0x6a97b4a1, /// 0xe80
			 0xb7996568, /// 0xe84
			 0xd0dff00c, /// 0xe88
			 0x2e78e5dc, /// 0xe8c
			 0x0237e8da, /// 0xe90
			 0x1643e48a, /// 0xe94
			 0x19bacf99, /// 0xe98
			 0x9d93e99a, /// 0xe9c
			 0xd31144a1, /// 0xea0
			 0x544ce1db, /// 0xea4
			 0xaf9b71d1, /// 0xea8
			 0x708dfc97, /// 0xeac
			 0xeae25fa6, /// 0xeb0
			 0xba0e7c8f, /// 0xeb4
			 0xa858f78d, /// 0xeb8
			 0xf298a7b9, /// 0xebc
			 0x608c3a34, /// 0xec0
			 0xbaf2709e, /// 0xec4
			 0x265cea96, /// 0xec8
			 0xebdd833c, /// 0xecc
			 0x820d82ea, /// 0xed0
			 0xf4d0316c, /// 0xed4
			 0xb523d552, /// 0xed8
			 0x2c57c285, /// 0xedc
			 0x66190e29, /// 0xee0
			 0x0fe6aadf, /// 0xee4
			 0xd6a3419d, /// 0xee8
			 0x9eaef03c, /// 0xeec
			 0xe087fe7b, /// 0xef0
			 0x0e1ecd93, /// 0xef4
			 0xca56bdea, /// 0xef8
			 0xb40fead5, /// 0xefc
			 0x75db92a5, /// 0xf00
			 0x4bb98655, /// 0xf04
			 0xe0bd82db, /// 0xf08
			 0x1edcf142, /// 0xf0c
			 0xb88f3e6c, /// 0xf10
			 0x3797b2ca, /// 0xf14
			 0x1cb35f40, /// 0xf18
			 0x8d16a7cc, /// 0xf1c
			 0xd79f94bc, /// 0xf20
			 0xa001186a, /// 0xf24
			 0x9c3d1807, /// 0xf28
			 0xcf1c8491, /// 0xf2c
			 0xc8b20a38, /// 0xf30
			 0xcdf7bf6d, /// 0xf34
			 0x12832892, /// 0xf38
			 0x6fd333b0, /// 0xf3c
			 0x4606c173, /// 0xf40
			 0x6b2a4a85, /// 0xf44
			 0x6afb760c, /// 0xf48
			 0x62fe61f8, /// 0xf4c
			 0x30c484cb, /// 0xf50
			 0x9b72d8e6, /// 0xf54
			 0x81c5154a, /// 0xf58
			 0x5c570925, /// 0xf5c
			 0xc30a134f, /// 0xf60
			 0xf604daae, /// 0xf64
			 0x80e8daac, /// 0xf68
			 0x3c9a4103, /// 0xf6c
			 0x85e07fd5, /// 0xf70
			 0x501b36fb, /// 0xf74
			 0xdea22ac3, /// 0xf78
			 0x0de64c19, /// 0xf7c
			 0x233df97e, /// 0xf80
			 0xcc8ec278, /// 0xf84
			 0xb2cd52a1, /// 0xf88
			 0x27c43b48, /// 0xf8c
			 0x651c5ec2, /// 0xf90
			 0x72634cbd, /// 0xf94
			 0x5902e632, /// 0xf98
			 0x7502d757, /// 0xf9c
			 0x370b8b34, /// 0xfa0
			 0x46a8abe8, /// 0xfa4
			 0x307387d1, /// 0xfa8
			 0x96862210, /// 0xfac
			 0x63ff8b2c, /// 0xfb0
			 0x00ba98dc, /// 0xfb4
			 0xf4555c4c, /// 0xfb8
			 0xf9999c23, /// 0xfbc
			 0x3293ef4c, /// 0xfc0
			 0x57dcf5f5, /// 0xfc4
			 0x30835d41, /// 0xfc8
			 0x243852cb, /// 0xfcc
			 0xa6205624, /// 0xfd0
			 0xe6b0b82f, /// 0xfd4
			 0x6dae1850, /// 0xfd8
			 0xb3327a04, /// 0xfdc
			 0x283c9a4f, /// 0xfe0
			 0x3fd442f4, /// 0xfe4
			 0x3977c24b, /// 0xfe8
			 0x40519006, /// 0xfec
			 0xa4114683, /// 0xff0
			 0xb72e09e1, /// 0xff4
			 0xd8cd0e38, /// 0xff8
			 0x93bc108f /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00000
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00004
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00008
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0000c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00010
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00014
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00018
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00028
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0002c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00030
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00034
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0003c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00040
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00048
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0004c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00058
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0005c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00060
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0006c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x4b000000,                                                  // 8388608.0                                   /// 00074
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00084
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00088
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0008c
			 0x55555555,                                                  // 4 random values                             /// 00090
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00094
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0e000001,                                                  // Trailing 1s:                                /// 0009c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 000b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00104
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0010c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00110
			 0x80000000,                                                  // -zero                                       /// 00114
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00118
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00120
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00124
			 0x7fc00001,                                                  // signaling NaN                               /// 00128
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00130
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00134
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00138
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0013c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00140
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00144
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00148
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00154
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0015c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00164
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00168
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0016c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00174
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00178
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0017c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00194
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00198
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0xbf028f5c,                                                  // -0.51                                       /// 001c8
			 0x7fc00001,                                                  // signaling NaN                               /// 001cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 001e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 001ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001f0
			 0x00000000,                                                  // zero                                        /// 001f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00200
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00204
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0020c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00210
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00214
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x00000000,                                                  // zero                                        /// 0021c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x80000000,                                                  // -zero                                       /// 00224
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00228
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0022c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00234
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0023c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0024c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00250
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00260
			 0x3f028f5c,                                                  // 0.51                                        /// 00264
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00268
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0026c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00270
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00274
			 0x0c780000,                                                  // Leading 1s:                                 /// 00278
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0027c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00280
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00284
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00288
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0028c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00294
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00298
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 002ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00300
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00304
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00308
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0030c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00310
			 0x80000000,                                                  // -zero                                       /// 00314
			 0x80000000,                                                  // -zero                                       /// 00318
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0031c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00320
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00324
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00330
			 0xbf028f5c,                                                  // -0.51                                       /// 00334
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00338
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0033c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00340
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00344
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00348
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00350
			 0xbf028f5c,                                                  // -0.51                                       /// 00354
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00358
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0035c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00360
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00364
			 0xcb000000,                                                  // -8388608.0                                  /// 00368
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00370
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00374
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00378
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00384
			 0x7f800000,                                                  // inf                                         /// 00388
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0038c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00390
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00394
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00398
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0039c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 003e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00400
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00408
			 0x0e000007,                                                  // Trailing 1s:                                /// 0040c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00410
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00414
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00418
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0041c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00420
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00424
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0042c
			 0x7f800000,                                                  // inf                                         /// 00430
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00438
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00440
			 0x7f800000,                                                  // inf                                         /// 00444
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00450
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00458
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00460
			 0x33333333,                                                  // 4 random values                             /// 00464
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00468
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00470
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00474
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00478
			 0xbf028f5c,                                                  // -0.51                                       /// 0047c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00484
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00488
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0048c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00494
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0049c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 004a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00508
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00520
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00524
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0052c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00530
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00534
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0053c
			 0xcb000000,                                                  // -8388608.0                                  /// 00540
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00550
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00554
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00558
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00560
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00564
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00568
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0056c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00570
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00574
			 0x0e000001,                                                  // Trailing 1s:                                /// 00578
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00584
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0xcb000000,                                                  // -8388608.0                                  /// 0058c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00590
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00594
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00598
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0059c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 005c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005f8
			 0x3f028f5c,                                                  // 0.51                                        /// 005fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00600
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00608
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0060c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00614
			 0xcb000000,                                                  // -8388608.0                                  /// 00618
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0061c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0062c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00630
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x00000000,                                                  // zero                                        /// 00638
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0063c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00640
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00644
			 0x0c600000,                                                  // Leading 1s:                                 /// 00648
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0064c
			 0xff800000,                                                  // -inf                                        /// 00650
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00654
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0065c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00660
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0xff800000,                                                  // -inf                                        /// 00668
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0066c
			 0xcb000000,                                                  // -8388608.0                                  /// 00670
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00674
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00680
			 0xbf028f5c,                                                  // -0.51                                       /// 00684
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00688
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00690
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00694
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00698
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0069c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006a0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006b4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 006bc
			 0x7fc00001,                                                  // signaling NaN                               /// 006c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006d0
			 0xffc00001,                                                  // -signaling NaN                              /// 006d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006f8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00704
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x55555555,                                                  // 4 random values                             /// 00710
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00714
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00718
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00720
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00724
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00728
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00730
			 0x80000000,                                                  // -zero                                       /// 00734
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00738
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0073c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00740
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00744
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0074c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00758
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x4b000000,                                                  // 8388608.0                                   /// 00760
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00764
			 0x0c780000,                                                  // Leading 1s:                                 /// 00768
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00770
			 0x0c780000,                                                  // Leading 1s:                                 /// 00774
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00778
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00000000,                                                  // zero                                        /// 00780
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00784
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00798
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00800
			 0x00000000,                                                  // zero                                        /// 00804
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00808
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0080c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00810
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00814
			 0x0e000003,                                                  // Trailing 1s:                                /// 00818
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00820
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0xcb000000,                                                  // -8388608.0                                  /// 00828
			 0x80000000,                                                  // -zero                                       /// 0082c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00834
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00838
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0083c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00840
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00844
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00848
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00850
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00858
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x7fc00001,                                                  // signaling NaN                               /// 00860
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00868
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00878
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00884
			 0x7fc00001,                                                  // signaling NaN                               /// 00888
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00890
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0089c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 008cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008ec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00900
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00904
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00908
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0090c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00910
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00918
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0091c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00924
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00930
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00934
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0093c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00940
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00944
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00948
			 0x80011111,                                                  // -9.7958E-41                                 /// 0094c
			 0x80000000,                                                  // -zero                                       /// 00950
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00960
			 0x0c400000,                                                  // Leading 1s:                                 /// 00964
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0096c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00974
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0xff800000,                                                  // -inf                                        /// 0097c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00980
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00984
			 0xff800000,                                                  // -inf                                        /// 00988
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0098c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00998
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0099c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 009b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0xffc00001,                                                  // -signaling NaN                              /// 009cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00000000,                                                  // zero                                        /// 009d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009f8
			 0x4b000000,                                                  // 8388608.0                                   /// 009fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a08
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a1c
			 0x55555555,                                                  // 4 random values                             /// 00a20
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a28
			 0xbf028f5c,                                                  // -0.51                                       /// 00a2c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a30
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a38
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a40
			 0x7f800000,                                                  // inf                                         /// 00a44
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a4c
			 0x80000000,                                                  // -zero                                       /// 00a50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a60
			 0x80000000,                                                  // -zero                                       /// 00a64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a68
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a74
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a88
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a98
			 0x3f028f5c,                                                  // 0.51                                        /// 00a9c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00aa0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00aa4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00aa8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ab4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00abc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ac0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ac8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ad4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00adc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ae4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ae8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00aec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00af0
			 0x4b000000,                                                  // 8388608.0                                   /// 00af4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00af8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00afc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b08
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0xff800000,                                                  // -inf                                        /// 00b18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b24
			 0xff800000,                                                  // -inf                                        /// 00b28
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b30
			 0x4b000000,                                                  // 8388608.0                                   /// 00b34
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b38
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b3c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b50
			 0xbf028f5c,                                                  // -0.51                                       /// 00b54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b58
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b60
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0x7fc00001,                                                  // signaling NaN                               /// 00b68
			 0x7fc00001,                                                  // signaling NaN                               /// 00b6c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b70
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b80
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b84
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b90
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b94
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b9c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ba0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ba8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bb0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x7f800000,                                                  // inf                                         /// 00bbc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0xff800000,                                                  // -inf                                        /// 00bc4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bc8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bd0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bec
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bf0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bf8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c00
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c10
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c18
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c1c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c20
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c28
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c6c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c84
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c88
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ca8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cb0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cb4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ccc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cd0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cd4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cd8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cdc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ce0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ce4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ce8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cf0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cf8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cfc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d10
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d18
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d30
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d38
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d40
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d64
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d80
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d98
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d9c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00da4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00db4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00db8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dbc
			 0xbf028f5c,                                                  // -0.51                                       /// 00dc0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dc4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dc8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dcc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dd4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dd8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00de0
			 0x55555555,                                                  // 4 random values                             /// 00de4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00de8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00df0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0xcb000000,                                                  // -8388608.0                                  /// 00df8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dfc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e08
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e0c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e34
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0xbf028f5c,                                                  // -0.51                                       /// 00e4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e58
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e5c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e60
			 0x00000000,                                                  // zero                                        /// 00e64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e80
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e94
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ea0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ea8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00eac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00eb0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00eb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eb8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ebc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ec0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ec4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ec8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ecc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ed4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ed8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00edc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ee0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ee4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ee8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ef4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ef8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f04
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f0c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f14
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f18
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f1c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f24
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f28
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f2c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x55555555,                                                  // 4 random values                             /// 00f38
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f3c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f48
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f60
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f80
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f8c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fa0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fa4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fa8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fb4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fb8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fbc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fc0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fc4
			 0x80000000,                                                  // -zero                                       /// 00fc8
			 0xbf028f5c,                                                  // -0.51                                       /// 00fcc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fd0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fd4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fd8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fdc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fe0
			 0x55555555,                                                  // 4 random values                             /// 00fe4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fe8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ff0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ff4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ff8
			 0x0c7fffc0                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xcf5829de, /// 0x0
			 0x76611ce2, /// 0x4
			 0x8c71be14, /// 0x8
			 0x590d136b, /// 0xc
			 0x77e0a155, /// 0x10
			 0x0f837d96, /// 0x14
			 0x76c69a2e, /// 0x18
			 0x8f0c094d, /// 0x1c
			 0x4af5fe97, /// 0x20
			 0xbafa0d12, /// 0x24
			 0x19c21d2f, /// 0x28
			 0xe4491676, /// 0x2c
			 0xfcc82e8f, /// 0x30
			 0xb7042d02, /// 0x34
			 0xf8414525, /// 0x38
			 0xe33e613b, /// 0x3c
			 0xa850a00f, /// 0x40
			 0x133f8266, /// 0x44
			 0x5a149be5, /// 0x48
			 0x3256df10, /// 0x4c
			 0xa9c5243a, /// 0x50
			 0x7db5b096, /// 0x54
			 0xba715640, /// 0x58
			 0x7355b37b, /// 0x5c
			 0xa6839044, /// 0x60
			 0xdd9e8c43, /// 0x64
			 0x0904b174, /// 0x68
			 0x1c29558c, /// 0x6c
			 0x861c055d, /// 0x70
			 0x83f851f4, /// 0x74
			 0xaaaa2b2e, /// 0x78
			 0xf8bdcc79, /// 0x7c
			 0x974b9f74, /// 0x80
			 0xadca2813, /// 0x84
			 0xeaae3849, /// 0x88
			 0x9b62651a, /// 0x8c
			 0x0c51bd3a, /// 0x90
			 0x8c78bb00, /// 0x94
			 0x2fe60836, /// 0x98
			 0x6b4b5c68, /// 0x9c
			 0xbb7d0373, /// 0xa0
			 0x94948ca8, /// 0xa4
			 0x83ebb27e, /// 0xa8
			 0x86d58f1c, /// 0xac
			 0xaf856b7e, /// 0xb0
			 0xf4e4800f, /// 0xb4
			 0x69e7d780, /// 0xb8
			 0xb649217d, /// 0xbc
			 0xb74c59bf, /// 0xc0
			 0x01d5a884, /// 0xc4
			 0xf2c6fd0d, /// 0xc8
			 0xa4528814, /// 0xcc
			 0x57af9ca4, /// 0xd0
			 0xc3da5d02, /// 0xd4
			 0x2549a382, /// 0xd8
			 0x594e3321, /// 0xdc
			 0x6614661d, /// 0xe0
			 0xf0ad0e5d, /// 0xe4
			 0x7f24f76f, /// 0xe8
			 0x9eb553d7, /// 0xec
			 0xcf6f6c58, /// 0xf0
			 0x9bb8e7cf, /// 0xf4
			 0x929037f9, /// 0xf8
			 0xbeaf99e1, /// 0xfc
			 0x872b588d, /// 0x100
			 0x5f62d15c, /// 0x104
			 0x904f3ded, /// 0x108
			 0xd4d2896a, /// 0x10c
			 0x1165ba77, /// 0x110
			 0x81931e41, /// 0x114
			 0x0d51991a, /// 0x118
			 0xf5a063bf, /// 0x11c
			 0x47d8e360, /// 0x120
			 0xcf9b2658, /// 0x124
			 0x42730c2e, /// 0x128
			 0x7b9ad823, /// 0x12c
			 0x9c008f22, /// 0x130
			 0x0f0e1442, /// 0x134
			 0x3e3d7080, /// 0x138
			 0x6ff4d10a, /// 0x13c
			 0x8681abd7, /// 0x140
			 0x1cccbc0a, /// 0x144
			 0xf1dcce13, /// 0x148
			 0xec7732bc, /// 0x14c
			 0x81e205e0, /// 0x150
			 0xfbe67cb8, /// 0x154
			 0x310a9076, /// 0x158
			 0x0aed2a1b, /// 0x15c
			 0xab80cab7, /// 0x160
			 0x1a37101e, /// 0x164
			 0x918426f0, /// 0x168
			 0x0800f1f1, /// 0x16c
			 0x77388637, /// 0x170
			 0x0e4f1181, /// 0x174
			 0x7eb3e5b4, /// 0x178
			 0xbbcf6265, /// 0x17c
			 0xd69cfa62, /// 0x180
			 0x04a9f1e5, /// 0x184
			 0xa9af5eae, /// 0x188
			 0x96013997, /// 0x18c
			 0x8f0ed002, /// 0x190
			 0x12df0ca0, /// 0x194
			 0x76b184ce, /// 0x198
			 0x082a21c3, /// 0x19c
			 0x2300ce7d, /// 0x1a0
			 0xbbec6d7a, /// 0x1a4
			 0x8cd50a04, /// 0x1a8
			 0x87f83325, /// 0x1ac
			 0x92b5dafc, /// 0x1b0
			 0xb8d20ae2, /// 0x1b4
			 0x020fad49, /// 0x1b8
			 0xf85bcb5d, /// 0x1bc
			 0xf9db4176, /// 0x1c0
			 0x5e32ff67, /// 0x1c4
			 0xc7bd77b4, /// 0x1c8
			 0x8578b5d2, /// 0x1cc
			 0xe8d99079, /// 0x1d0
			 0x32785044, /// 0x1d4
			 0x002e3774, /// 0x1d8
			 0x13dc6314, /// 0x1dc
			 0x965bb2e7, /// 0x1e0
			 0xb27f5272, /// 0x1e4
			 0x5c81ed37, /// 0x1e8
			 0x1f565187, /// 0x1ec
			 0x5de70b4b, /// 0x1f0
			 0x06b37367, /// 0x1f4
			 0x2e3f3915, /// 0x1f8
			 0x4c47d532, /// 0x1fc
			 0x885383ea, /// 0x200
			 0x23ae06cf, /// 0x204
			 0x62b4c523, /// 0x208
			 0x7de7f701, /// 0x20c
			 0x9324140b, /// 0x210
			 0x4a0032f8, /// 0x214
			 0x9721036d, /// 0x218
			 0x96278695, /// 0x21c
			 0x01bdc464, /// 0x220
			 0x2e49404f, /// 0x224
			 0x2138c494, /// 0x228
			 0x7d8e3ea6, /// 0x22c
			 0x902d0d4a, /// 0x230
			 0x52029fca, /// 0x234
			 0xc6b4f6a0, /// 0x238
			 0xba94de69, /// 0x23c
			 0x06d5f597, /// 0x240
			 0x7f05b8f9, /// 0x244
			 0xf115f998, /// 0x248
			 0x572c43d2, /// 0x24c
			 0xe4feefbf, /// 0x250
			 0x2c57b74c, /// 0x254
			 0x3e390d27, /// 0x258
			 0x06bd2ddd, /// 0x25c
			 0x9393481f, /// 0x260
			 0xca8676cb, /// 0x264
			 0xbb4e3014, /// 0x268
			 0x30673e9d, /// 0x26c
			 0x097b9123, /// 0x270
			 0x744aa0d6, /// 0x274
			 0x974d01c1, /// 0x278
			 0xeaf3ebec, /// 0x27c
			 0x025a4eeb, /// 0x280
			 0x978b80ed, /// 0x284
			 0xdfc95302, /// 0x288
			 0xac6a77a4, /// 0x28c
			 0x44bee213, /// 0x290
			 0x0a3d3c0f, /// 0x294
			 0x7d4e7be0, /// 0x298
			 0xe135aa0a, /// 0x29c
			 0x18d7049a, /// 0x2a0
			 0xf00a5469, /// 0x2a4
			 0x92162dbe, /// 0x2a8
			 0x08ab395b, /// 0x2ac
			 0x34dd4670, /// 0x2b0
			 0x531384f4, /// 0x2b4
			 0x6c9753c4, /// 0x2b8
			 0x1f467499, /// 0x2bc
			 0x59ddb8c5, /// 0x2c0
			 0xe9df1f22, /// 0x2c4
			 0x5cf2fbea, /// 0x2c8
			 0xf9d33e11, /// 0x2cc
			 0xcc4dd24d, /// 0x2d0
			 0x8f716664, /// 0x2d4
			 0x370193c7, /// 0x2d8
			 0xd7808d85, /// 0x2dc
			 0xe02f39a3, /// 0x2e0
			 0x93c52269, /// 0x2e4
			 0x89db495a, /// 0x2e8
			 0x746d07b1, /// 0x2ec
			 0xc835fe00, /// 0x2f0
			 0x4f1a3f92, /// 0x2f4
			 0xa8cabe2b, /// 0x2f8
			 0xdaaf3c18, /// 0x2fc
			 0x9777c2da, /// 0x300
			 0xde36b02f, /// 0x304
			 0x9ecbf0c1, /// 0x308
			 0xadbf29a3, /// 0x30c
			 0x81001ad3, /// 0x310
			 0xd5667daf, /// 0x314
			 0x0c998894, /// 0x318
			 0x31d8e3b9, /// 0x31c
			 0xbb27b27d, /// 0x320
			 0x47fc3277, /// 0x324
			 0xe1768327, /// 0x328
			 0xa721d4ad, /// 0x32c
			 0x0b9a92e7, /// 0x330
			 0xb02c2a96, /// 0x334
			 0xeb8d85d8, /// 0x338
			 0x680c1b1c, /// 0x33c
			 0xeb43301c, /// 0x340
			 0x4abdc4e1, /// 0x344
			 0x2fd5f17d, /// 0x348
			 0x474d1680, /// 0x34c
			 0xaa48684f, /// 0x350
			 0xd30e8014, /// 0x354
			 0x0c22780c, /// 0x358
			 0xcae6c191, /// 0x35c
			 0x9a71938f, /// 0x360
			 0x4336bc35, /// 0x364
			 0x5a6ab1ae, /// 0x368
			 0xe73a1219, /// 0x36c
			 0xe65cc512, /// 0x370
			 0x19d8d310, /// 0x374
			 0x391d66f6, /// 0x378
			 0x39106a5f, /// 0x37c
			 0x6bca9cff, /// 0x380
			 0x04a0df74, /// 0x384
			 0xc331e066, /// 0x388
			 0xc9eccbd1, /// 0x38c
			 0x5c123c3f, /// 0x390
			 0xc4f828e7, /// 0x394
			 0x99a512a1, /// 0x398
			 0x8746610e, /// 0x39c
			 0xc6b44db8, /// 0x3a0
			 0x451674e8, /// 0x3a4
			 0x21152726, /// 0x3a8
			 0xc8a110a5, /// 0x3ac
			 0x18b2f76b, /// 0x3b0
			 0x04210bd4, /// 0x3b4
			 0x3c96e92f, /// 0x3b8
			 0xa26bf21d, /// 0x3bc
			 0x833116c9, /// 0x3c0
			 0xc4ba5f8f, /// 0x3c4
			 0xe8ca1ca5, /// 0x3c8
			 0xb5a18d10, /// 0x3cc
			 0xe4aea049, /// 0x3d0
			 0xe8e91703, /// 0x3d4
			 0x0a0e77f4, /// 0x3d8
			 0xc9629429, /// 0x3dc
			 0xd835be2a, /// 0x3e0
			 0xaf0a4d11, /// 0x3e4
			 0x7b464022, /// 0x3e8
			 0xe1d97dbe, /// 0x3ec
			 0x04215bef, /// 0x3f0
			 0x56f66ac0, /// 0x3f4
			 0x6f12e7de, /// 0x3f8
			 0xec2b5248, /// 0x3fc
			 0x90b84668, /// 0x400
			 0xfa01c935, /// 0x404
			 0xe34e96ab, /// 0x408
			 0x496e29ec, /// 0x40c
			 0x20522093, /// 0x410
			 0x10c53577, /// 0x414
			 0xd09d6169, /// 0x418
			 0x9fe59be6, /// 0x41c
			 0xcf3cbe5d, /// 0x420
			 0x3da8baaa, /// 0x424
			 0x62d72787, /// 0x428
			 0xff338d2c, /// 0x42c
			 0xc96c905a, /// 0x430
			 0x73f1358e, /// 0x434
			 0xe4986983, /// 0x438
			 0xabfb3613, /// 0x43c
			 0xe57d3535, /// 0x440
			 0xf36df5b2, /// 0x444
			 0x2947b871, /// 0x448
			 0xfc568faf, /// 0x44c
			 0x1ec9a7b9, /// 0x450
			 0xc383e05e, /// 0x454
			 0xb66b2bfa, /// 0x458
			 0x17aa34d4, /// 0x45c
			 0x335c2af3, /// 0x460
			 0x58a17939, /// 0x464
			 0x12b2fa6b, /// 0x468
			 0x9649e7db, /// 0x46c
			 0x8c04257b, /// 0x470
			 0xa1f771c1, /// 0x474
			 0x9f7594bc, /// 0x478
			 0xc9e59fa9, /// 0x47c
			 0x10d3465e, /// 0x480
			 0x83ebea3f, /// 0x484
			 0xb38e936a, /// 0x488
			 0x7aa4a90f, /// 0x48c
			 0xf182e66e, /// 0x490
			 0x7436f03c, /// 0x494
			 0xddf9f20c, /// 0x498
			 0xf0c05955, /// 0x49c
			 0x3d2f4d52, /// 0x4a0
			 0x324c83bc, /// 0x4a4
			 0x714a922b, /// 0x4a8
			 0xe7093fd9, /// 0x4ac
			 0xf62a3be9, /// 0x4b0
			 0xfe257f59, /// 0x4b4
			 0x1cfa487d, /// 0x4b8
			 0x0cc0070f, /// 0x4bc
			 0x3cd68c66, /// 0x4c0
			 0x347132cc, /// 0x4c4
			 0xe04e8eea, /// 0x4c8
			 0x2733d96b, /// 0x4cc
			 0x030a61b7, /// 0x4d0
			 0xb3bf6064, /// 0x4d4
			 0x0b192535, /// 0x4d8
			 0x601f72ac, /// 0x4dc
			 0x8bc9494d, /// 0x4e0
			 0x81f16870, /// 0x4e4
			 0xa1a9831c, /// 0x4e8
			 0x6d3fa8a5, /// 0x4ec
			 0x83b24916, /// 0x4f0
			 0xc7ea8492, /// 0x4f4
			 0x98d4cd93, /// 0x4f8
			 0x16b303b5, /// 0x4fc
			 0xf28cf059, /// 0x500
			 0xbf0bd18d, /// 0x504
			 0xcb8bcf0b, /// 0x508
			 0x3851c3f0, /// 0x50c
			 0x79751fd3, /// 0x510
			 0x4ec7f8b3, /// 0x514
			 0x1454bd16, /// 0x518
			 0x34a97f47, /// 0x51c
			 0x06b5c63c, /// 0x520
			 0x0cc7d168, /// 0x524
			 0x531229b9, /// 0x528
			 0x5c76a2ac, /// 0x52c
			 0x982c1d84, /// 0x530
			 0xe32c9d0f, /// 0x534
			 0x91e8c36e, /// 0x538
			 0x951df766, /// 0x53c
			 0x5ec30191, /// 0x540
			 0xbd552de3, /// 0x544
			 0xe5559f15, /// 0x548
			 0x064e86b1, /// 0x54c
			 0xd6a1e25c, /// 0x550
			 0xe55c0902, /// 0x554
			 0xa38e8f0f, /// 0x558
			 0x315d797c, /// 0x55c
			 0x999e8824, /// 0x560
			 0x9a2367c9, /// 0x564
			 0x93bf2d41, /// 0x568
			 0xb43ccc54, /// 0x56c
			 0x948fa798, /// 0x570
			 0xb6196e7d, /// 0x574
			 0x24ca8d70, /// 0x578
			 0x937bc65d, /// 0x57c
			 0x00ba6d23, /// 0x580
			 0xcdd888b7, /// 0x584
			 0x9f6aa493, /// 0x588
			 0xed91d698, /// 0x58c
			 0x07b455f1, /// 0x590
			 0x65d888fe, /// 0x594
			 0x30b00216, /// 0x598
			 0x53f3627d, /// 0x59c
			 0xee6de469, /// 0x5a0
			 0x8e889aeb, /// 0x5a4
			 0xfc08504b, /// 0x5a8
			 0x2845f044, /// 0x5ac
			 0x7a1fe0b0, /// 0x5b0
			 0x16a6f352, /// 0x5b4
			 0x16ea69ca, /// 0x5b8
			 0xeb00a9c8, /// 0x5bc
			 0x33332720, /// 0x5c0
			 0x29375109, /// 0x5c4
			 0x0248d7d0, /// 0x5c8
			 0xb927d74d, /// 0x5cc
			 0x9d3f5b63, /// 0x5d0
			 0x0147e837, /// 0x5d4
			 0x372b3bde, /// 0x5d8
			 0x4688e4ea, /// 0x5dc
			 0xd8555edc, /// 0x5e0
			 0xdff3e1d4, /// 0x5e4
			 0x74d14830, /// 0x5e8
			 0x52d2aa0e, /// 0x5ec
			 0x76434dd9, /// 0x5f0
			 0xd041f940, /// 0x5f4
			 0x2493b894, /// 0x5f8
			 0x6638fe76, /// 0x5fc
			 0xafcb9648, /// 0x600
			 0xd5d16aac, /// 0x604
			 0x77443f4b, /// 0x608
			 0x1e6f20bb, /// 0x60c
			 0x86559717, /// 0x610
			 0x80440884, /// 0x614
			 0xfc630308, /// 0x618
			 0xcbbae0fe, /// 0x61c
			 0x6d7152a1, /// 0x620
			 0xe59e8825, /// 0x624
			 0xd0a1465e, /// 0x628
			 0x0e6fc10f, /// 0x62c
			 0xec577658, /// 0x630
			 0xc7fd674b, /// 0x634
			 0xa9242e37, /// 0x638
			 0x48e474a7, /// 0x63c
			 0x5ea0671f, /// 0x640
			 0x04a8994d, /// 0x644
			 0x6bc51d38, /// 0x648
			 0xba9338ac, /// 0x64c
			 0x3e40aa16, /// 0x650
			 0x4b8fab23, /// 0x654
			 0x65bff435, /// 0x658
			 0x1a3be625, /// 0x65c
			 0x372d588d, /// 0x660
			 0xb1502e68, /// 0x664
			 0x5a5a26cc, /// 0x668
			 0x59feaf54, /// 0x66c
			 0x350a4267, /// 0x670
			 0xbfffa6ca, /// 0x674
			 0x190fe165, /// 0x678
			 0x3bf6a8c9, /// 0x67c
			 0x6bdb6453, /// 0x680
			 0x57f4d120, /// 0x684
			 0x4e064121, /// 0x688
			 0x657d1bea, /// 0x68c
			 0x2b166a43, /// 0x690
			 0xe34d55c9, /// 0x694
			 0x7bf504db, /// 0x698
			 0x99169fb9, /// 0x69c
			 0x5b44d8d3, /// 0x6a0
			 0xcfffd288, /// 0x6a4
			 0xc7a83745, /// 0x6a8
			 0x9b185dfe, /// 0x6ac
			 0xe7d74b36, /// 0x6b0
			 0xd3451c45, /// 0x6b4
			 0xe7d31798, /// 0x6b8
			 0x27737f99, /// 0x6bc
			 0x1a9460b5, /// 0x6c0
			 0x8c3a95c5, /// 0x6c4
			 0x3a865d35, /// 0x6c8
			 0x21e64194, /// 0x6cc
			 0x745dc316, /// 0x6d0
			 0xb03b3fbc, /// 0x6d4
			 0xe1c50c78, /// 0x6d8
			 0x594fb7cc, /// 0x6dc
			 0x0a6fa45d, /// 0x6e0
			 0x81a6e6db, /// 0x6e4
			 0xfb425c5b, /// 0x6e8
			 0x7c76437b, /// 0x6ec
			 0x1632da8c, /// 0x6f0
			 0x7ac4ccfc, /// 0x6f4
			 0x851f53a6, /// 0x6f8
			 0x7e86ab7e, /// 0x6fc
			 0x2923ef9d, /// 0x700
			 0xddbc0fd0, /// 0x704
			 0x0c84f4e7, /// 0x708
			 0xb84b785f, /// 0x70c
			 0xbb399a50, /// 0x710
			 0xa05a60bc, /// 0x714
			 0xfec61cd1, /// 0x718
			 0x22c10052, /// 0x71c
			 0xb646cdc1, /// 0x720
			 0x2f6a9a6a, /// 0x724
			 0x16fbffa9, /// 0x728
			 0x0dfb68b4, /// 0x72c
			 0x6c0354e9, /// 0x730
			 0xb2338f61, /// 0x734
			 0x37b87da8, /// 0x738
			 0xd4d87602, /// 0x73c
			 0x874774ce, /// 0x740
			 0xf60ff9ba, /// 0x744
			 0xef78f0c1, /// 0x748
			 0x1102a729, /// 0x74c
			 0xcc1ad291, /// 0x750
			 0xb10c40f9, /// 0x754
			 0xe1ce7825, /// 0x758
			 0x9ecbac17, /// 0x75c
			 0xc4f912a4, /// 0x760
			 0x72402096, /// 0x764
			 0x21a501fc, /// 0x768
			 0x7d1b4960, /// 0x76c
			 0x85cb7792, /// 0x770
			 0xb7ef8f07, /// 0x774
			 0xda4bb0e1, /// 0x778
			 0x85056c48, /// 0x77c
			 0x385b15dc, /// 0x780
			 0xb9d92bec, /// 0x784
			 0xd05da429, /// 0x788
			 0xcebf1da8, /// 0x78c
			 0xb358648a, /// 0x790
			 0x3e2c4f25, /// 0x794
			 0x88dced3d, /// 0x798
			 0xf66e6834, /// 0x79c
			 0x1586cfe1, /// 0x7a0
			 0xb0162b96, /// 0x7a4
			 0x1bdd4bd0, /// 0x7a8
			 0x240ab820, /// 0x7ac
			 0x6c08987d, /// 0x7b0
			 0x70dec2e0, /// 0x7b4
			 0xcb435b98, /// 0x7b8
			 0x86bf63e9, /// 0x7bc
			 0x5e26e306, /// 0x7c0
			 0xfde1cc1a, /// 0x7c4
			 0x9a69bae2, /// 0x7c8
			 0x19e6a32a, /// 0x7cc
			 0x1952ed24, /// 0x7d0
			 0xbd373553, /// 0x7d4
			 0x7261f541, /// 0x7d8
			 0xdc996301, /// 0x7dc
			 0xafcc0c10, /// 0x7e0
			 0x81b32cec, /// 0x7e4
			 0x2aed1394, /// 0x7e8
			 0x692de969, /// 0x7ec
			 0x7cff6df0, /// 0x7f0
			 0x528f52b5, /// 0x7f4
			 0x350ba69a, /// 0x7f8
			 0x2a40cc12, /// 0x7fc
			 0x14f97fbe, /// 0x800
			 0x27048fff, /// 0x804
			 0xb917acf1, /// 0x808
			 0x249b1b94, /// 0x80c
			 0x8d0dcd4c, /// 0x810
			 0x9097a738, /// 0x814
			 0x2b332fc8, /// 0x818
			 0xa86885e0, /// 0x81c
			 0xddddcddd, /// 0x820
			 0xd71eaf3a, /// 0x824
			 0x8dd1dd39, /// 0x828
			 0x720001a5, /// 0x82c
			 0xbc1c5083, /// 0x830
			 0x61b55214, /// 0x834
			 0xf5bba854, /// 0x838
			 0xa67339c4, /// 0x83c
			 0x4036d1f4, /// 0x840
			 0x6c63eb78, /// 0x844
			 0x091ee487, /// 0x848
			 0x9a8bf079, /// 0x84c
			 0x69256006, /// 0x850
			 0x0d3650ae, /// 0x854
			 0x6e3d6027, /// 0x858
			 0x9a5993f2, /// 0x85c
			 0x10483ffd, /// 0x860
			 0x4e6e8fd9, /// 0x864
			 0xcd322175, /// 0x868
			 0x12cf3a95, /// 0x86c
			 0x8d446dc6, /// 0x870
			 0x21d46a98, /// 0x874
			 0xa2e5c791, /// 0x878
			 0x6df3ca06, /// 0x87c
			 0x6f3df684, /// 0x880
			 0x4ce44df5, /// 0x884
			 0xb8f797e4, /// 0x888
			 0xb10b53b2, /// 0x88c
			 0xf6e0fa12, /// 0x890
			 0x0aa8c377, /// 0x894
			 0x89cd7cad, /// 0x898
			 0xf4438b9e, /// 0x89c
			 0x242b864f, /// 0x8a0
			 0x5e5d40af, /// 0x8a4
			 0x9540dc53, /// 0x8a8
			 0xa0824dab, /// 0x8ac
			 0x94377630, /// 0x8b0
			 0x1e467c15, /// 0x8b4
			 0xa69c131d, /// 0x8b8
			 0x634322f3, /// 0x8bc
			 0x75f124ec, /// 0x8c0
			 0xbad57150, /// 0x8c4
			 0xb8ad57d3, /// 0x8c8
			 0xb8dc5f10, /// 0x8cc
			 0x48b9aae8, /// 0x8d0
			 0x1a405a47, /// 0x8d4
			 0x3090da67, /// 0x8d8
			 0x18a10894, /// 0x8dc
			 0x868bb7d3, /// 0x8e0
			 0x3db2fa15, /// 0x8e4
			 0x3e54166c, /// 0x8e8
			 0xf60e72fe, /// 0x8ec
			 0x6ee2d7d9, /// 0x8f0
			 0x658e4835, /// 0x8f4
			 0x92ffcb67, /// 0x8f8
			 0x9224c94a, /// 0x8fc
			 0x91619667, /// 0x900
			 0xa3b8860c, /// 0x904
			 0x42f4a921, /// 0x908
			 0x2b25c650, /// 0x90c
			 0x976def77, /// 0x910
			 0x0b251b55, /// 0x914
			 0x3d08be5e, /// 0x918
			 0x02a8cce6, /// 0x91c
			 0x5baef6a3, /// 0x920
			 0xc6e9e3ba, /// 0x924
			 0xc8d97581, /// 0x928
			 0xa9c8e12a, /// 0x92c
			 0x21a6da82, /// 0x930
			 0xa41c4ad8, /// 0x934
			 0x04340dfa, /// 0x938
			 0xae2abe6c, /// 0x93c
			 0x410768d0, /// 0x940
			 0xcb8c3eec, /// 0x944
			 0xcdbfe511, /// 0x948
			 0xa3ec7469, /// 0x94c
			 0xe5e98558, /// 0x950
			 0x6e65d87b, /// 0x954
			 0x9455d454, /// 0x958
			 0xf581e53c, /// 0x95c
			 0x8153980a, /// 0x960
			 0x5e91b64b, /// 0x964
			 0xb0ce946a, /// 0x968
			 0xb87daa4f, /// 0x96c
			 0x3513ac4b, /// 0x970
			 0x272ba567, /// 0x974
			 0x1a671e36, /// 0x978
			 0xc6d1b849, /// 0x97c
			 0x4bd59829, /// 0x980
			 0x01cd222a, /// 0x984
			 0x3f4d44c9, /// 0x988
			 0x40816add, /// 0x98c
			 0xe8e308e1, /// 0x990
			 0x018db000, /// 0x994
			 0xa9e38c8d, /// 0x998
			 0x512fc0bb, /// 0x99c
			 0x48371f9e, /// 0x9a0
			 0x0456fc3e, /// 0x9a4
			 0x11b946fa, /// 0x9a8
			 0x8f1c0f8c, /// 0x9ac
			 0xcdad5e57, /// 0x9b0
			 0xe51a030a, /// 0x9b4
			 0x3ef00220, /// 0x9b8
			 0x68574cf6, /// 0x9bc
			 0x170e5b2e, /// 0x9c0
			 0x97560a22, /// 0x9c4
			 0x8f121471, /// 0x9c8
			 0x61b2d117, /// 0x9cc
			 0x90d55ea0, /// 0x9d0
			 0xe23740c6, /// 0x9d4
			 0x43a0cc86, /// 0x9d8
			 0xe5bfa1bd, /// 0x9dc
			 0xb495b951, /// 0x9e0
			 0x8d7648c7, /// 0x9e4
			 0x07c930b9, /// 0x9e8
			 0x11a600e4, /// 0x9ec
			 0x72382644, /// 0x9f0
			 0x172437a2, /// 0x9f4
			 0x1ddd57b2, /// 0x9f8
			 0x5c80ffe3, /// 0x9fc
			 0xe0ca9225, /// 0xa00
			 0xc043afe6, /// 0xa04
			 0x135d779a, /// 0xa08
			 0x410e3438, /// 0xa0c
			 0x5655ca70, /// 0xa10
			 0x328c3cb0, /// 0xa14
			 0x2ab1c4d3, /// 0xa18
			 0xb4bdd596, /// 0xa1c
			 0x2bb2a465, /// 0xa20
			 0x5462cd17, /// 0xa24
			 0xfdebf75f, /// 0xa28
			 0xd93aa575, /// 0xa2c
			 0x5a9997df, /// 0xa30
			 0x18c9b1bf, /// 0xa34
			 0x58957ee5, /// 0xa38
			 0xd6eedea7, /// 0xa3c
			 0xb77db195, /// 0xa40
			 0x62b8af10, /// 0xa44
			 0x0f63a0f6, /// 0xa48
			 0xcea4a1ed, /// 0xa4c
			 0xdca96fd7, /// 0xa50
			 0x2ade162b, /// 0xa54
			 0x577a0af9, /// 0xa58
			 0xf1a45ec1, /// 0xa5c
			 0x5c9ecfb9, /// 0xa60
			 0x7c7dc93e, /// 0xa64
			 0xb7a7535e, /// 0xa68
			 0x00740ad8, /// 0xa6c
			 0x7859a0b0, /// 0xa70
			 0xbbdf6a6a, /// 0xa74
			 0x78e68756, /// 0xa78
			 0x675b5e27, /// 0xa7c
			 0x1bba7d5d, /// 0xa80
			 0x3d90c9e5, /// 0xa84
			 0x59998328, /// 0xa88
			 0x2cc3a9a4, /// 0xa8c
			 0xf4933a60, /// 0xa90
			 0x55b07141, /// 0xa94
			 0x57315eb5, /// 0xa98
			 0xb15f849b, /// 0xa9c
			 0xb73fb12d, /// 0xaa0
			 0x878bdc99, /// 0xaa4
			 0x44d6b0a9, /// 0xaa8
			 0xa224e0d4, /// 0xaac
			 0x36499f0a, /// 0xab0
			 0x5ba32214, /// 0xab4
			 0x2e9dc42f, /// 0xab8
			 0xd0a7cd57, /// 0xabc
			 0x947f5671, /// 0xac0
			 0xb41992fa, /// 0xac4
			 0x1e0cb27c, /// 0xac8
			 0x8d9d1425, /// 0xacc
			 0x6f505196, /// 0xad0
			 0xaa8f829a, /// 0xad4
			 0x960600f6, /// 0xad8
			 0xf1960e41, /// 0xadc
			 0x926c65bc, /// 0xae0
			 0xd954ef5c, /// 0xae4
			 0x39adc52c, /// 0xae8
			 0x4e5bc312, /// 0xaec
			 0x6fee9134, /// 0xaf0
			 0x47ec0ac0, /// 0xaf4
			 0x0c3f78c6, /// 0xaf8
			 0xc7a2eb9d, /// 0xafc
			 0x8199c773, /// 0xb00
			 0x13fe1c21, /// 0xb04
			 0xfcbca577, /// 0xb08
			 0xb1a12103, /// 0xb0c
			 0x1389f2d3, /// 0xb10
			 0x269f1b9d, /// 0xb14
			 0x6d55f291, /// 0xb18
			 0x0db75213, /// 0xb1c
			 0xba58282c, /// 0xb20
			 0x3951b54a, /// 0xb24
			 0xf77b286c, /// 0xb28
			 0xdeb94001, /// 0xb2c
			 0x1906086c, /// 0xb30
			 0x50645603, /// 0xb34
			 0x5f711794, /// 0xb38
			 0x52ec9bf7, /// 0xb3c
			 0x735b9d63, /// 0xb40
			 0x2657d8e8, /// 0xb44
			 0xf6e28bc5, /// 0xb48
			 0x9ab708f1, /// 0xb4c
			 0x01fb60ed, /// 0xb50
			 0xcde8901c, /// 0xb54
			 0xf4d847a8, /// 0xb58
			 0xe6c8c674, /// 0xb5c
			 0x92e29a17, /// 0xb60
			 0xfdbd7d8b, /// 0xb64
			 0xd0434f31, /// 0xb68
			 0xa6892735, /// 0xb6c
			 0x05b3c0f7, /// 0xb70
			 0x7b741614, /// 0xb74
			 0x610cc45e, /// 0xb78
			 0xc21682f6, /// 0xb7c
			 0x4e13b4a4, /// 0xb80
			 0x3bdaf1ab, /// 0xb84
			 0xb8fe58a8, /// 0xb88
			 0xedff2572, /// 0xb8c
			 0x85be5188, /// 0xb90
			 0x04e870ec, /// 0xb94
			 0xe29d09f4, /// 0xb98
			 0x93a65c68, /// 0xb9c
			 0xbae9c883, /// 0xba0
			 0x4f0656d0, /// 0xba4
			 0x72634e10, /// 0xba8
			 0x81284125, /// 0xbac
			 0xf9f902c4, /// 0xbb0
			 0xd951fea1, /// 0xbb4
			 0x23d2f6b5, /// 0xbb8
			 0x4ce6b267, /// 0xbbc
			 0x7624efe9, /// 0xbc0
			 0x2b353c5c, /// 0xbc4
			 0xeced91a4, /// 0xbc8
			 0x5ce716c6, /// 0xbcc
			 0x07b04c0e, /// 0xbd0
			 0xf03571ae, /// 0xbd4
			 0xca5467ce, /// 0xbd8
			 0x66164fc3, /// 0xbdc
			 0xcfddf090, /// 0xbe0
			 0xa01eb123, /// 0xbe4
			 0xf0847bd2, /// 0xbe8
			 0xfc99d4c4, /// 0xbec
			 0x9cf3b97b, /// 0xbf0
			 0xd75806cb, /// 0xbf4
			 0x582b1c48, /// 0xbf8
			 0xd0a31471, /// 0xbfc
			 0xf602bf60, /// 0xc00
			 0xf55ae3dd, /// 0xc04
			 0x7f29b135, /// 0xc08
			 0xf7599b05, /// 0xc0c
			 0x6302d353, /// 0xc10
			 0xfb9c4381, /// 0xc14
			 0x920084ee, /// 0xc18
			 0xa571b60f, /// 0xc1c
			 0x4f24c5de, /// 0xc20
			 0x650eef68, /// 0xc24
			 0x002812eb, /// 0xc28
			 0x3c30ea91, /// 0xc2c
			 0xb4684a09, /// 0xc30
			 0x9012e22a, /// 0xc34
			 0xe9df79b5, /// 0xc38
			 0x6a43ab8a, /// 0xc3c
			 0xc1f5d553, /// 0xc40
			 0xea1e252e, /// 0xc44
			 0xa74f58c7, /// 0xc48
			 0xa366b617, /// 0xc4c
			 0x3e29d55a, /// 0xc50
			 0x85506a5c, /// 0xc54
			 0x11644ac8, /// 0xc58
			 0x6119c53e, /// 0xc5c
			 0xf4f594dc, /// 0xc60
			 0xcb893339, /// 0xc64
			 0xe8a957d1, /// 0xc68
			 0x3ae81e44, /// 0xc6c
			 0x1fa2d02d, /// 0xc70
			 0x79ce22ff, /// 0xc74
			 0x4fbcfedb, /// 0xc78
			 0x07ad1f5a, /// 0xc7c
			 0x9f892fed, /// 0xc80
			 0x4acae058, /// 0xc84
			 0xa1a82179, /// 0xc88
			 0xe5fe7da0, /// 0xc8c
			 0x4c582624, /// 0xc90
			 0x75325611, /// 0xc94
			 0x90cb146f, /// 0xc98
			 0xe8208053, /// 0xc9c
			 0xd4831151, /// 0xca0
			 0x47b4a9b3, /// 0xca4
			 0x5bf88af4, /// 0xca8
			 0x13858033, /// 0xcac
			 0x2920303c, /// 0xcb0
			 0xc2c78d31, /// 0xcb4
			 0x1e0206ce, /// 0xcb8
			 0x82709868, /// 0xcbc
			 0x2600d8e5, /// 0xcc0
			 0x046f372c, /// 0xcc4
			 0xce32c769, /// 0xcc8
			 0x7b842db9, /// 0xccc
			 0x0c1fb549, /// 0xcd0
			 0x79f3ccbc, /// 0xcd4
			 0xcf43b70a, /// 0xcd8
			 0xec25ea6f, /// 0xcdc
			 0x0d569e38, /// 0xce0
			 0xf8da0275, /// 0xce4
			 0x9c1f73cf, /// 0xce8
			 0xb7500834, /// 0xcec
			 0x9beb5a8e, /// 0xcf0
			 0x83f66287, /// 0xcf4
			 0xb4f85ef3, /// 0xcf8
			 0xba4c18fd, /// 0xcfc
			 0x5aa623b9, /// 0xd00
			 0x36810fb4, /// 0xd04
			 0x2a999359, /// 0xd08
			 0x36550323, /// 0xd0c
			 0x938c663c, /// 0xd10
			 0xb0160d40, /// 0xd14
			 0xa11c197c, /// 0xd18
			 0xb954bd3c, /// 0xd1c
			 0x7b585e12, /// 0xd20
			 0x6fb7064f, /// 0xd24
			 0xe596aaec, /// 0xd28
			 0x18457dbb, /// 0xd2c
			 0xa54c51eb, /// 0xd30
			 0x5c0d5259, /// 0xd34
			 0x715f51e4, /// 0xd38
			 0x406e95bc, /// 0xd3c
			 0xfb58f9d4, /// 0xd40
			 0x6315aa5a, /// 0xd44
			 0x2ec2d2eb, /// 0xd48
			 0x38024e0e, /// 0xd4c
			 0x5a76afc8, /// 0xd50
			 0x91011736, /// 0xd54
			 0xf6e1881e, /// 0xd58
			 0x7f319547, /// 0xd5c
			 0xcff3531a, /// 0xd60
			 0xc3481cd5, /// 0xd64
			 0xa4f86dc3, /// 0xd68
			 0xa335d181, /// 0xd6c
			 0xb41ce7ec, /// 0xd70
			 0x336d5138, /// 0xd74
			 0xba07db0f, /// 0xd78
			 0xf7026907, /// 0xd7c
			 0x899558da, /// 0xd80
			 0xd7a3e56b, /// 0xd84
			 0x251ed3f6, /// 0xd88
			 0xd9bc9bb5, /// 0xd8c
			 0x45e39f13, /// 0xd90
			 0xbb32f37e, /// 0xd94
			 0x8c15381d, /// 0xd98
			 0x476286d3, /// 0xd9c
			 0x1e2587b7, /// 0xda0
			 0xd2b07405, /// 0xda4
			 0xb20880d8, /// 0xda8
			 0x643c9af7, /// 0xdac
			 0x987723f0, /// 0xdb0
			 0x4c269757, /// 0xdb4
			 0x9c33cd11, /// 0xdb8
			 0xc72ea353, /// 0xdbc
			 0x91cf1914, /// 0xdc0
			 0xcce7d8bf, /// 0xdc4
			 0x72429a4b, /// 0xdc8
			 0xe120eaa3, /// 0xdcc
			 0x5f207496, /// 0xdd0
			 0xa4a33afd, /// 0xdd4
			 0xd5f211bc, /// 0xdd8
			 0x148604f8, /// 0xddc
			 0xf08e139d, /// 0xde0
			 0x69ecaf35, /// 0xde4
			 0xe14d6b02, /// 0xde8
			 0x1ded8e66, /// 0xdec
			 0x498c00bb, /// 0xdf0
			 0xc1aa1482, /// 0xdf4
			 0x924947fc, /// 0xdf8
			 0x9b4b02f9, /// 0xdfc
			 0x48d231e2, /// 0xe00
			 0xb263b1a6, /// 0xe04
			 0x403c7955, /// 0xe08
			 0x58f5ab57, /// 0xe0c
			 0xcd0c0e00, /// 0xe10
			 0xa1399d47, /// 0xe14
			 0xe3ba5243, /// 0xe18
			 0x60c22607, /// 0xe1c
			 0x5acff818, /// 0xe20
			 0xc4c3d4c1, /// 0xe24
			 0x694fe9d8, /// 0xe28
			 0xc147188e, /// 0xe2c
			 0xba9efcb9, /// 0xe30
			 0x6b303847, /// 0xe34
			 0xc0847de9, /// 0xe38
			 0x3a4e3e6e, /// 0xe3c
			 0x9befbd3e, /// 0xe40
			 0xafea3b3a, /// 0xe44
			 0x2728c6ab, /// 0xe48
			 0xf86f2ad9, /// 0xe4c
			 0xf68ab53d, /// 0xe50
			 0xbeb35cc9, /// 0xe54
			 0x8ce371c7, /// 0xe58
			 0x7eba7f33, /// 0xe5c
			 0x1d7c5b26, /// 0xe60
			 0xe7940c71, /// 0xe64
			 0x8d089ce3, /// 0xe68
			 0xf65e951b, /// 0xe6c
			 0xf97e4a49, /// 0xe70
			 0x448c9202, /// 0xe74
			 0x6515df17, /// 0xe78
			 0xd466f275, /// 0xe7c
			 0x4ec4c872, /// 0xe80
			 0x34095c49, /// 0xe84
			 0xe233d127, /// 0xe88
			 0x978ba765, /// 0xe8c
			 0x1c89474d, /// 0xe90
			 0xfe7d8064, /// 0xe94
			 0x40e23672, /// 0xe98
			 0xd88c3b38, /// 0xe9c
			 0xa77f11d3, /// 0xea0
			 0x09e01b3e, /// 0xea4
			 0xf5c80909, /// 0xea8
			 0x87338d46, /// 0xeac
			 0x81931792, /// 0xeb0
			 0x47603eb5, /// 0xeb4
			 0x4436502f, /// 0xeb8
			 0x5c0e3cf7, /// 0xebc
			 0xa510fcbe, /// 0xec0
			 0x817fa15e, /// 0xec4
			 0x70421916, /// 0xec8
			 0x02fde1e5, /// 0xecc
			 0xa9b1bef3, /// 0xed0
			 0xe6110b3c, /// 0xed4
			 0xe9f159f4, /// 0xed8
			 0x65d7764a, /// 0xedc
			 0x2a325a16, /// 0xee0
			 0x222e9f92, /// 0xee4
			 0x2a6b4701, /// 0xee8
			 0xffb2c1d5, /// 0xeec
			 0x7615fc88, /// 0xef0
			 0x8bae841e, /// 0xef4
			 0xf01f0eaa, /// 0xef8
			 0xe8408025, /// 0xefc
			 0xa91fc4a9, /// 0xf00
			 0xc486c941, /// 0xf04
			 0x316bd352, /// 0xf08
			 0x72ecdf50, /// 0xf0c
			 0x956c09f3, /// 0xf10
			 0x10994e59, /// 0xf14
			 0xeb6b972a, /// 0xf18
			 0xb5121f7a, /// 0xf1c
			 0xaa522fbe, /// 0xf20
			 0x2584da95, /// 0xf24
			 0xfbd5eab0, /// 0xf28
			 0x22f3d07d, /// 0xf2c
			 0xc4c197cb, /// 0xf30
			 0x7e786b72, /// 0xf34
			 0x8424fdf6, /// 0xf38
			 0x59c2565a, /// 0xf3c
			 0x9588631d, /// 0xf40
			 0xdc888bda, /// 0xf44
			 0x28bdbe12, /// 0xf48
			 0x66868304, /// 0xf4c
			 0xd4ff0f85, /// 0xf50
			 0xecbdeb5e, /// 0xf54
			 0xb04469c6, /// 0xf58
			 0xf0736703, /// 0xf5c
			 0x3480de5c, /// 0xf60
			 0x7a1cebb2, /// 0xf64
			 0xc4ffe758, /// 0xf68
			 0xe1f4df1b, /// 0xf6c
			 0x93da1905, /// 0xf70
			 0xbdfcf118, /// 0xf74
			 0x054e51a8, /// 0xf78
			 0x2468b35b, /// 0xf7c
			 0x061955cd, /// 0xf80
			 0xf863e5bd, /// 0xf84
			 0xe3a033b8, /// 0xf88
			 0x8b2f998e, /// 0xf8c
			 0x33876b53, /// 0xf90
			 0x7cdb9279, /// 0xf94
			 0x14f3f563, /// 0xf98
			 0x683dbc9a, /// 0xf9c
			 0xedb348a0, /// 0xfa0
			 0x0a979721, /// 0xfa4
			 0x911629ee, /// 0xfa8
			 0x52963a39, /// 0xfac
			 0x9336e225, /// 0xfb0
			 0xb8e77284, /// 0xfb4
			 0x46fb8cb2, /// 0xfb8
			 0x40a45742, /// 0xfbc
			 0xab0d6c51, /// 0xfc0
			 0x687c79af, /// 0xfc4
			 0x719196b8, /// 0xfc8
			 0xb62ccc8c, /// 0xfcc
			 0xe047b498, /// 0xfd0
			 0x0f98cd40, /// 0xfd4
			 0x772f6a70, /// 0xfd8
			 0xd1ec7e20, /// 0xfdc
			 0xd2eb0531, /// 0xfe0
			 0xeecbf931, /// 0xfe4
			 0x92e9081c, /// 0xfe8
			 0xb46b62b2, /// 0xfec
			 0x6a4c1d60, /// 0xff0
			 0x6a427c72, /// 0xff4
			 0x2bdaaa86, /// 0xff8
			 0x943cc3b5 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x1fb03ff8, /// 0x0
			 0xdf2d3a22, /// 0x4
			 0x69d559ed, /// 0x8
			 0xaa86d696, /// 0xc
			 0x2b641fcc, /// 0x10
			 0x98d7f08a, /// 0x14
			 0xee9b5aff, /// 0x18
			 0x6754fd28, /// 0x1c
			 0x9fb90377, /// 0x20
			 0x0727b31a, /// 0x24
			 0x4f5b9e15, /// 0x28
			 0x8989f639, /// 0x2c
			 0x57c6f6d4, /// 0x30
			 0xed11cc66, /// 0x34
			 0x76e06cc2, /// 0x38
			 0x0d40b3b3, /// 0x3c
			 0x3e38c776, /// 0x40
			 0x42c0a4ec, /// 0x44
			 0x8ab56888, /// 0x48
			 0x213128c6, /// 0x4c
			 0xa9876ed0, /// 0x50
			 0xe19229d4, /// 0x54
			 0xf3615cad, /// 0x58
			 0x02865389, /// 0x5c
			 0x22afacff, /// 0x60
			 0xca354724, /// 0x64
			 0x2fcf185e, /// 0x68
			 0x8472e9bb, /// 0x6c
			 0x84af3e19, /// 0x70
			 0x11bb13b3, /// 0x74
			 0x58622715, /// 0x78
			 0x7d6fb3f0, /// 0x7c
			 0x50c29426, /// 0x80
			 0x43987e06, /// 0x84
			 0xf628f868, /// 0x88
			 0x30eff3de, /// 0x8c
			 0xc6c2b4f7, /// 0x90
			 0x2600e72f, /// 0x94
			 0x44fdf8ab, /// 0x98
			 0x081ad8a0, /// 0x9c
			 0x231f598d, /// 0xa0
			 0x74657140, /// 0xa4
			 0xa2fd0208, /// 0xa8
			 0x4a71b068, /// 0xac
			 0x017b9f7f, /// 0xb0
			 0xf631dcb7, /// 0xb4
			 0x1d3c435b, /// 0xb8
			 0x3b56d1a1, /// 0xbc
			 0xaad10545, /// 0xc0
			 0xdd00276a, /// 0xc4
			 0xa394685a, /// 0xc8
			 0x790ffb8d, /// 0xcc
			 0xb07d7a3c, /// 0xd0
			 0x9fbbcd60, /// 0xd4
			 0x599d9e6c, /// 0xd8
			 0x61f905b3, /// 0xdc
			 0xd3d41fe8, /// 0xe0
			 0x2762babf, /// 0xe4
			 0x219bcf02, /// 0xe8
			 0x1180c4cc, /// 0xec
			 0x92e7daa7, /// 0xf0
			 0xe82aedb9, /// 0xf4
			 0x95ea0069, /// 0xf8
			 0x8af66bce, /// 0xfc
			 0xfa799edc, /// 0x100
			 0xefa7daec, /// 0x104
			 0xabac44ce, /// 0x108
			 0x704bdbdd, /// 0x10c
			 0x02a4f542, /// 0x110
			 0x5955697d, /// 0x114
			 0xb9332f29, /// 0x118
			 0xbcd4dfc7, /// 0x11c
			 0xf91a7ff4, /// 0x120
			 0xf6312f70, /// 0x124
			 0xec20abc2, /// 0x128
			 0xc3237189, /// 0x12c
			 0xeb588512, /// 0x130
			 0xfccd4dbe, /// 0x134
			 0xc1e2aeea, /// 0x138
			 0xe4ca5cc7, /// 0x13c
			 0x2e2661b4, /// 0x140
			 0x53751e96, /// 0x144
			 0xc039ed7c, /// 0x148
			 0xddd0d87b, /// 0x14c
			 0x44edee1a, /// 0x150
			 0x33e4ec76, /// 0x154
			 0x5a182c33, /// 0x158
			 0x9878c61d, /// 0x15c
			 0x5b785741, /// 0x160
			 0x8e908785, /// 0x164
			 0x09e2de55, /// 0x168
			 0xb1025849, /// 0x16c
			 0x147c9c63, /// 0x170
			 0xd56355d3, /// 0x174
			 0x7cedc17f, /// 0x178
			 0xeddadcc5, /// 0x17c
			 0x4139fb34, /// 0x180
			 0x24bab4e3, /// 0x184
			 0x20b04f26, /// 0x188
			 0x5c821e1c, /// 0x18c
			 0x22e7dbb2, /// 0x190
			 0x435d9e5e, /// 0x194
			 0x5cbda318, /// 0x198
			 0x93b0c186, /// 0x19c
			 0xe7a97a02, /// 0x1a0
			 0x07b67eae, /// 0x1a4
			 0x349996d9, /// 0x1a8
			 0xe9785cff, /// 0x1ac
			 0x6bec59d0, /// 0x1b0
			 0xc718286c, /// 0x1b4
			 0xfc9b18a8, /// 0x1b8
			 0xba135512, /// 0x1bc
			 0xe91b03bd, /// 0x1c0
			 0x6759b435, /// 0x1c4
			 0x9f78aebb, /// 0x1c8
			 0x60c040b2, /// 0x1cc
			 0x03329120, /// 0x1d0
			 0xc0250f38, /// 0x1d4
			 0xb7dd8397, /// 0x1d8
			 0x11fc31ca, /// 0x1dc
			 0xa7215662, /// 0x1e0
			 0xb2cdde18, /// 0x1e4
			 0x432a5dcb, /// 0x1e8
			 0xbcc01ea6, /// 0x1ec
			 0x5be4ebf8, /// 0x1f0
			 0xc86ba325, /// 0x1f4
			 0x0ae764a0, /// 0x1f8
			 0xd2193c44, /// 0x1fc
			 0x4323530f, /// 0x200
			 0xd0cfb564, /// 0x204
			 0xbf91436e, /// 0x208
			 0x048c91ef, /// 0x20c
			 0xfc2c5c16, /// 0x210
			 0xe7ca7667, /// 0x214
			 0x4303bed4, /// 0x218
			 0x8fc505ed, /// 0x21c
			 0x2e8a2a6e, /// 0x220
			 0x1f19e374, /// 0x224
			 0x25340a82, /// 0x228
			 0xc2194707, /// 0x22c
			 0xef75cb0e, /// 0x230
			 0x192b85fc, /// 0x234
			 0x7ab1a65e, /// 0x238
			 0x252ead63, /// 0x23c
			 0x605bd0ca, /// 0x240
			 0xf2380f4c, /// 0x244
			 0x3aef9e62, /// 0x248
			 0xa1c78c63, /// 0x24c
			 0x9147c3a9, /// 0x250
			 0xfacca0de, /// 0x254
			 0x15925898, /// 0x258
			 0xef8fdefd, /// 0x25c
			 0x7d318113, /// 0x260
			 0x982207b8, /// 0x264
			 0xfd892462, /// 0x268
			 0x9eb1b22e, /// 0x26c
			 0x9dd73223, /// 0x270
			 0x90f3619d, /// 0x274
			 0x70196228, /// 0x278
			 0x6e458b61, /// 0x27c
			 0x25b23087, /// 0x280
			 0x4d10aa89, /// 0x284
			 0x2d03fd99, /// 0x288
			 0xb79def09, /// 0x28c
			 0xad65107e, /// 0x290
			 0x34a5884e, /// 0x294
			 0xe22a7083, /// 0x298
			 0x52efc6ff, /// 0x29c
			 0x10c4c0e7, /// 0x2a0
			 0x9137a9f8, /// 0x2a4
			 0xe0436421, /// 0x2a8
			 0x0f22b5ec, /// 0x2ac
			 0x4598c189, /// 0x2b0
			 0xb092a50c, /// 0x2b4
			 0xb73f315e, /// 0x2b8
			 0xc1a9dad7, /// 0x2bc
			 0xc4a04de1, /// 0x2c0
			 0x870e404b, /// 0x2c4
			 0xf96d2cfd, /// 0x2c8
			 0x920c493c, /// 0x2cc
			 0xac6e2ad9, /// 0x2d0
			 0xc59cc65a, /// 0x2d4
			 0x3fd3de29, /// 0x2d8
			 0x4c910cf9, /// 0x2dc
			 0x626f0e61, /// 0x2e0
			 0x89d538e6, /// 0x2e4
			 0x7c9af8d9, /// 0x2e8
			 0x74c5b011, /// 0x2ec
			 0xae1798d6, /// 0x2f0
			 0xb677a0d3, /// 0x2f4
			 0x61b1ee10, /// 0x2f8
			 0xcb7813b4, /// 0x2fc
			 0x0f7c0705, /// 0x300
			 0x20921502, /// 0x304
			 0x1a962795, /// 0x308
			 0x26c73177, /// 0x30c
			 0xd5ca9ed6, /// 0x310
			 0x478ef392, /// 0x314
			 0xfe386d6f, /// 0x318
			 0x824416fc, /// 0x31c
			 0x0e362878, /// 0x320
			 0x915c20ec, /// 0x324
			 0x10cd57b4, /// 0x328
			 0xca2299fd, /// 0x32c
			 0x30e19edf, /// 0x330
			 0x7afa5121, /// 0x334
			 0xfbd668b7, /// 0x338
			 0xbd3209c7, /// 0x33c
			 0xbed8f859, /// 0x340
			 0x23fa56fa, /// 0x344
			 0x010b0ab1, /// 0x348
			 0xc8cba40a, /// 0x34c
			 0x619ba2e0, /// 0x350
			 0x5a4d3ee1, /// 0x354
			 0xf52c252e, /// 0x358
			 0xc7db5545, /// 0x35c
			 0x7309e460, /// 0x360
			 0x86266e24, /// 0x364
			 0x6f2a7d0a, /// 0x368
			 0xa403813a, /// 0x36c
			 0xcb8fe8c5, /// 0x370
			 0xc0c077c0, /// 0x374
			 0x0a5b181a, /// 0x378
			 0x3d62bd59, /// 0x37c
			 0xa2387e17, /// 0x380
			 0x20641804, /// 0x384
			 0xa9ab8625, /// 0x388
			 0xa1bb2ec7, /// 0x38c
			 0xd8a37ea3, /// 0x390
			 0xcb441e36, /// 0x394
			 0x14d79180, /// 0x398
			 0xdde29759, /// 0x39c
			 0x7a810d6f, /// 0x3a0
			 0x05eae7ad, /// 0x3a4
			 0xe6d4b698, /// 0x3a8
			 0x269992d3, /// 0x3ac
			 0xcdb678ba, /// 0x3b0
			 0xd1783f8c, /// 0x3b4
			 0xeb003537, /// 0x3b8
			 0xc12cf80a, /// 0x3bc
			 0x32cb39ce, /// 0x3c0
			 0xde623162, /// 0x3c4
			 0x1892f901, /// 0x3c8
			 0xadea2e74, /// 0x3cc
			 0x4859f7b5, /// 0x3d0
			 0x2b0668ae, /// 0x3d4
			 0x7747aa49, /// 0x3d8
			 0x90d5c201, /// 0x3dc
			 0xb4449afd, /// 0x3e0
			 0x5876d044, /// 0x3e4
			 0x48564297, /// 0x3e8
			 0xc60a9a95, /// 0x3ec
			 0x3b9e49ab, /// 0x3f0
			 0x5260b95f, /// 0x3f4
			 0xe5b047e1, /// 0x3f8
			 0xe95ff5fe, /// 0x3fc
			 0x11db05fd, /// 0x400
			 0xc645cca7, /// 0x404
			 0x45f175ec, /// 0x408
			 0x265754e8, /// 0x40c
			 0xb1c3028a, /// 0x410
			 0x44d1ea05, /// 0x414
			 0x6716a400, /// 0x418
			 0xff612b18, /// 0x41c
			 0x400239fd, /// 0x420
			 0x0f61b4c0, /// 0x424
			 0x82f87131, /// 0x428
			 0x699a96d4, /// 0x42c
			 0xfe9562f7, /// 0x430
			 0xc67293ab, /// 0x434
			 0xa766a444, /// 0x438
			 0xaafb57b1, /// 0x43c
			 0xf75be4ab, /// 0x440
			 0x6e65f8ba, /// 0x444
			 0x936123b8, /// 0x448
			 0xba6f8775, /// 0x44c
			 0xe3cdf75d, /// 0x450
			 0x9ef37e0f, /// 0x454
			 0xd1b7fa53, /// 0x458
			 0x9946cd20, /// 0x45c
			 0xc2206863, /// 0x460
			 0x42ce12ca, /// 0x464
			 0x83d3ce06, /// 0x468
			 0x30372808, /// 0x46c
			 0xe724f67f, /// 0x470
			 0x6862acce, /// 0x474
			 0x48949b36, /// 0x478
			 0xa7826bf4, /// 0x47c
			 0x0b9e60ed, /// 0x480
			 0x8d1d8223, /// 0x484
			 0x47a0d99f, /// 0x488
			 0xad1cd039, /// 0x48c
			 0xff01f28a, /// 0x490
			 0x5c0e786b, /// 0x494
			 0xcf9f33c5, /// 0x498
			 0x35d264d3, /// 0x49c
			 0x1cf55f18, /// 0x4a0
			 0xea693d5a, /// 0x4a4
			 0x3a04f565, /// 0x4a8
			 0x0d0868ab, /// 0x4ac
			 0xcfd3a7c5, /// 0x4b0
			 0xbf9fffd9, /// 0x4b4
			 0x3615c279, /// 0x4b8
			 0x91e00ea6, /// 0x4bc
			 0x01db5489, /// 0x4c0
			 0x52559c98, /// 0x4c4
			 0x051527f6, /// 0x4c8
			 0x51788e69, /// 0x4cc
			 0x4f16fd13, /// 0x4d0
			 0x1bbbb86b, /// 0x4d4
			 0x94635558, /// 0x4d8
			 0x00420829, /// 0x4dc
			 0x0d42fb0e, /// 0x4e0
			 0x299290b9, /// 0x4e4
			 0x33af5ada, /// 0x4e8
			 0xdb876086, /// 0x4ec
			 0x70f4b751, /// 0x4f0
			 0xfa2f557e, /// 0x4f4
			 0x7c253852, /// 0x4f8
			 0x99dc3fc4, /// 0x4fc
			 0xaaf9c199, /// 0x500
			 0x53d8e740, /// 0x504
			 0x9a7e5cfd, /// 0x508
			 0x1d80a7a2, /// 0x50c
			 0xea45b0c6, /// 0x510
			 0xf7e3c500, /// 0x514
			 0xb916bb6a, /// 0x518
			 0x24bc0367, /// 0x51c
			 0x4229091a, /// 0x520
			 0xe4422a70, /// 0x524
			 0xe8dd8205, /// 0x528
			 0xeecef84d, /// 0x52c
			 0x96b245aa, /// 0x530
			 0xa3867f26, /// 0x534
			 0x4069ec4e, /// 0x538
			 0xd4aff162, /// 0x53c
			 0xc3f8c6ec, /// 0x540
			 0x157a071f, /// 0x544
			 0x9de633c3, /// 0x548
			 0x5540099d, /// 0x54c
			 0x2c7480b8, /// 0x550
			 0x23d39631, /// 0x554
			 0x6a6779fc, /// 0x558
			 0x3b423341, /// 0x55c
			 0x62578c87, /// 0x560
			 0x1cc959f9, /// 0x564
			 0xedf6946e, /// 0x568
			 0xfc25d5d1, /// 0x56c
			 0x02bfc6cd, /// 0x570
			 0xbd2c4b6a, /// 0x574
			 0x0c5d5dba, /// 0x578
			 0xdafdb828, /// 0x57c
			 0x1000ee0f, /// 0x580
			 0xd4ae5823, /// 0x584
			 0x22377c91, /// 0x588
			 0x9961806f, /// 0x58c
			 0xcf1850f1, /// 0x590
			 0x2ba46c7d, /// 0x594
			 0x80ab0330, /// 0x598
			 0x707b05be, /// 0x59c
			 0x54113eab, /// 0x5a0
			 0x7aa76909, /// 0x5a4
			 0x90179c23, /// 0x5a8
			 0x22910bc0, /// 0x5ac
			 0x16b24d0f, /// 0x5b0
			 0x1669306f, /// 0x5b4
			 0xb1771766, /// 0x5b8
			 0x5e4a080e, /// 0x5bc
			 0xfc494635, /// 0x5c0
			 0xcba1fc4f, /// 0x5c4
			 0x44cd7e44, /// 0x5c8
			 0x44b5377c, /// 0x5cc
			 0x36b3121b, /// 0x5d0
			 0xa3a7ce1f, /// 0x5d4
			 0xee0c66ad, /// 0x5d8
			 0xed8b4dd1, /// 0x5dc
			 0xb67a764a, /// 0x5e0
			 0x575c5d96, /// 0x5e4
			 0xb6e83251, /// 0x5e8
			 0x0cc73814, /// 0x5ec
			 0xbdcbb3c5, /// 0x5f0
			 0x2cbf2300, /// 0x5f4
			 0x628d8b06, /// 0x5f8
			 0xe80707d9, /// 0x5fc
			 0xd417a7d9, /// 0x600
			 0xbc47772a, /// 0x604
			 0x681657b7, /// 0x608
			 0xae85e116, /// 0x60c
			 0x498fffba, /// 0x610
			 0xd0375576, /// 0x614
			 0x3a8ad139, /// 0x618
			 0xe9c4a36e, /// 0x61c
			 0xd897aaa2, /// 0x620
			 0xd9dd3811, /// 0x624
			 0xa6b9a2ae, /// 0x628
			 0xcdd3ad7a, /// 0x62c
			 0x172f6157, /// 0x630
			 0x8173820c, /// 0x634
			 0x04031695, /// 0x638
			 0x67f3ff6c, /// 0x63c
			 0x5f2caabd, /// 0x640
			 0xbb7b634b, /// 0x644
			 0x1bafc278, /// 0x648
			 0x3b007134, /// 0x64c
			 0x065b463d, /// 0x650
			 0x28677564, /// 0x654
			 0x12a41795, /// 0x658
			 0x44d0282f, /// 0x65c
			 0x4144664b, /// 0x660
			 0xd5349c54, /// 0x664
			 0x4c0108b1, /// 0x668
			 0x41f2698b, /// 0x66c
			 0x5d6c605b, /// 0x670
			 0xf872db73, /// 0x674
			 0x9ca98a60, /// 0x678
			 0x9a8352e3, /// 0x67c
			 0xafea028e, /// 0x680
			 0x3bdef348, /// 0x684
			 0x73ef2c9f, /// 0x688
			 0xe45cd05d, /// 0x68c
			 0x34f1f326, /// 0x690
			 0x6104e590, /// 0x694
			 0x0a555478, /// 0x698
			 0xedd19be5, /// 0x69c
			 0xf92ff642, /// 0x6a0
			 0x093812a4, /// 0x6a4
			 0xed72b21e, /// 0x6a8
			 0x2417f03f, /// 0x6ac
			 0x34c25b73, /// 0x6b0
			 0x2875d93d, /// 0x6b4
			 0x41a930b1, /// 0x6b8
			 0xb337303f, /// 0x6bc
			 0x9189feee, /// 0x6c0
			 0xb339a71a, /// 0x6c4
			 0x97679101, /// 0x6c8
			 0x0851baf0, /// 0x6cc
			 0x690619a8, /// 0x6d0
			 0xda0b5317, /// 0x6d4
			 0xe3a89b8b, /// 0x6d8
			 0x046e5a60, /// 0x6dc
			 0xb9d73702, /// 0x6e0
			 0x9fe94eef, /// 0x6e4
			 0x63d865b0, /// 0x6e8
			 0x2503c72c, /// 0x6ec
			 0x873848c7, /// 0x6f0
			 0x95badb8f, /// 0x6f4
			 0x3715a4e8, /// 0x6f8
			 0xa79c1261, /// 0x6fc
			 0x9368f651, /// 0x700
			 0x880445e1, /// 0x704
			 0x7ee44209, /// 0x708
			 0x39c29cd9, /// 0x70c
			 0x75d35671, /// 0x710
			 0xad559096, /// 0x714
			 0x5c2d748b, /// 0x718
			 0x08fcf3ed, /// 0x71c
			 0xf4b4b7e0, /// 0x720
			 0x62279386, /// 0x724
			 0x96872f2d, /// 0x728
			 0x32ec5ab8, /// 0x72c
			 0xc715ad17, /// 0x730
			 0x85b8adf6, /// 0x734
			 0x7473b712, /// 0x738
			 0x22df288f, /// 0x73c
			 0xc2924cce, /// 0x740
			 0x6ff43835, /// 0x744
			 0x5b31202d, /// 0x748
			 0x120c140a, /// 0x74c
			 0x5971031e, /// 0x750
			 0x6031c33d, /// 0x754
			 0xf08b76cd, /// 0x758
			 0x690b0dcd, /// 0x75c
			 0xcd40f1c2, /// 0x760
			 0xdee16a17, /// 0x764
			 0xa6a75834, /// 0x768
			 0xbbc66ae8, /// 0x76c
			 0xc1d426c1, /// 0x770
			 0x72a56172, /// 0x774
			 0xb21db693, /// 0x778
			 0x6c99c2f0, /// 0x77c
			 0x8caf3989, /// 0x780
			 0x9dd894bd, /// 0x784
			 0x3e781df3, /// 0x788
			 0x3afc6f9c, /// 0x78c
			 0x7ec643a6, /// 0x790
			 0x6483194f, /// 0x794
			 0x5cf7da94, /// 0x798
			 0x91139dd9, /// 0x79c
			 0x1e3ab738, /// 0x7a0
			 0x1cd4fc35, /// 0x7a4
			 0xa90e64c6, /// 0x7a8
			 0x10a00784, /// 0x7ac
			 0x31c4c64b, /// 0x7b0
			 0xe8addab7, /// 0x7b4
			 0x40f3dd1d, /// 0x7b8
			 0x4067f9f8, /// 0x7bc
			 0x0ca6be3d, /// 0x7c0
			 0x947b1f8c, /// 0x7c4
			 0x06edc677, /// 0x7c8
			 0x542cae64, /// 0x7cc
			 0x9099ab54, /// 0x7d0
			 0x4f4f55ee, /// 0x7d4
			 0xac2d1eba, /// 0x7d8
			 0xbdd5ed3c, /// 0x7dc
			 0xc4e3b04a, /// 0x7e0
			 0x4394b8d2, /// 0x7e4
			 0x63454b61, /// 0x7e8
			 0x454bc50a, /// 0x7ec
			 0xaf4c11e5, /// 0x7f0
			 0xbfe652be, /// 0x7f4
			 0xf43dac3f, /// 0x7f8
			 0x3ac736c0, /// 0x7fc
			 0xf006b41a, /// 0x800
			 0x64ade8d6, /// 0x804
			 0x41211534, /// 0x808
			 0x5e08aeae, /// 0x80c
			 0x943e647c, /// 0x810
			 0xb3c94f5e, /// 0x814
			 0xf4383720, /// 0x818
			 0x12728c57, /// 0x81c
			 0xa39c591a, /// 0x820
			 0x476de9cd, /// 0x824
			 0xd5829463, /// 0x828
			 0x0da5269b, /// 0x82c
			 0x45833499, /// 0x830
			 0xba563636, /// 0x834
			 0x59747259, /// 0x838
			 0xf03bafe6, /// 0x83c
			 0x392d2ac9, /// 0x840
			 0x33c28eac, /// 0x844
			 0x4a060ea6, /// 0x848
			 0xb27a938f, /// 0x84c
			 0x20599146, /// 0x850
			 0xaa422613, /// 0x854
			 0x177002ba, /// 0x858
			 0x27781e56, /// 0x85c
			 0x360c63d2, /// 0x860
			 0xced5aff4, /// 0x864
			 0x96d5b00c, /// 0x868
			 0xe6a7de69, /// 0x86c
			 0x0271a7fd, /// 0x870
			 0x7f488ff9, /// 0x874
			 0x30204fb0, /// 0x878
			 0x3fe6b77d, /// 0x87c
			 0x1854a020, /// 0x880
			 0x6ba679b7, /// 0x884
			 0x7bd80764, /// 0x888
			 0x9d3f48f6, /// 0x88c
			 0x7a01fb93, /// 0x890
			 0xc31c851f, /// 0x894
			 0xad2218b7, /// 0x898
			 0x0b77a871, /// 0x89c
			 0x14736af7, /// 0x8a0
			 0x858b36bf, /// 0x8a4
			 0xcaf27e77, /// 0x8a8
			 0x156e1162, /// 0x8ac
			 0x651ca841, /// 0x8b0
			 0xc7b3ee1e, /// 0x8b4
			 0xf03a4ca5, /// 0x8b8
			 0x28cb0826, /// 0x8bc
			 0xf99e73e0, /// 0x8c0
			 0x9b305775, /// 0x8c4
			 0x57a9ac40, /// 0x8c8
			 0x1cc4ff95, /// 0x8cc
			 0x2ecb4f9a, /// 0x8d0
			 0xfd092f48, /// 0x8d4
			 0x73edbf5d, /// 0x8d8
			 0x3ebaf8ff, /// 0x8dc
			 0x3b02bf9e, /// 0x8e0
			 0xd057d9db, /// 0x8e4
			 0x5eaf6431, /// 0x8e8
			 0x236b2427, /// 0x8ec
			 0x0d927e2c, /// 0x8f0
			 0x92ef4b88, /// 0x8f4
			 0xeb4ae39d, /// 0x8f8
			 0xcb2063cc, /// 0x8fc
			 0x7bc8b498, /// 0x900
			 0xe275dd9c, /// 0x904
			 0x2f07ec3c, /// 0x908
			 0x57ec0396, /// 0x90c
			 0xc04ec608, /// 0x910
			 0x2394a72b, /// 0x914
			 0x19b9da86, /// 0x918
			 0x9895342c, /// 0x91c
			 0xcf967b1e, /// 0x920
			 0xb905f111, /// 0x924
			 0xab454ac9, /// 0x928
			 0x7e6c20b3, /// 0x92c
			 0x5e0d2d3e, /// 0x930
			 0x7e98e3cb, /// 0x934
			 0x4b5eb3ef, /// 0x938
			 0x4f1da899, /// 0x93c
			 0x262e87b5, /// 0x940
			 0xbeb9f8e0, /// 0x944
			 0xd24dc46a, /// 0x948
			 0x2d9a0aab, /// 0x94c
			 0x7c62b1cb, /// 0x950
			 0x90fd3def, /// 0x954
			 0x5ce7e57e, /// 0x958
			 0xff4a0e8c, /// 0x95c
			 0xe54a8d3a, /// 0x960
			 0x1c5331f6, /// 0x964
			 0xe67bc83e, /// 0x968
			 0xae0e9346, /// 0x96c
			 0xcdff59d4, /// 0x970
			 0xf2573456, /// 0x974
			 0x54ae77e4, /// 0x978
			 0x0adf1096, /// 0x97c
			 0x9109a9c2, /// 0x980
			 0x137ed945, /// 0x984
			 0xf3ded94e, /// 0x988
			 0xc5483b11, /// 0x98c
			 0x93e54c55, /// 0x990
			 0x810efbe0, /// 0x994
			 0xb00c9391, /// 0x998
			 0x3032fc2e, /// 0x99c
			 0x9cfe265f, /// 0x9a0
			 0x2aa767fc, /// 0x9a4
			 0x0eb9948c, /// 0x9a8
			 0x8faf4cae, /// 0x9ac
			 0x7613777b, /// 0x9b0
			 0x87f3b1d4, /// 0x9b4
			 0xecdf540f, /// 0x9b8
			 0x67f34797, /// 0x9bc
			 0xdbd17dda, /// 0x9c0
			 0xcc8469d6, /// 0x9c4
			 0x92e58cd6, /// 0x9c8
			 0x12773336, /// 0x9cc
			 0xff181f66, /// 0x9d0
			 0x7ab3985a, /// 0x9d4
			 0x4f2178a2, /// 0x9d8
			 0x7fb54830, /// 0x9dc
			 0x896aa909, /// 0x9e0
			 0x8ce2c851, /// 0x9e4
			 0x0ab40d42, /// 0x9e8
			 0xda39c13e, /// 0x9ec
			 0x56f09ea1, /// 0x9f0
			 0x454a1c5a, /// 0x9f4
			 0x8af912a6, /// 0x9f8
			 0x08279876, /// 0x9fc
			 0xaf7eb473, /// 0xa00
			 0x26184789, /// 0xa04
			 0xaed0d1c0, /// 0xa08
			 0x8f2f57b7, /// 0xa0c
			 0x63476d99, /// 0xa10
			 0x3f3f53ac, /// 0xa14
			 0x2160e507, /// 0xa18
			 0xd1af312d, /// 0xa1c
			 0x9c4f7368, /// 0xa20
			 0x7aef58c7, /// 0xa24
			 0xe17b9905, /// 0xa28
			 0x7f88efbc, /// 0xa2c
			 0x79d03a36, /// 0xa30
			 0x7674350a, /// 0xa34
			 0xf3bc1b0e, /// 0xa38
			 0xcb075902, /// 0xa3c
			 0x72eabde0, /// 0xa40
			 0x1f06e42d, /// 0xa44
			 0x17d5bc19, /// 0xa48
			 0x5dd3cf38, /// 0xa4c
			 0x61e29a7b, /// 0xa50
			 0xea0460cd, /// 0xa54
			 0xb826081c, /// 0xa58
			 0x89ba843a, /// 0xa5c
			 0x63cad3bb, /// 0xa60
			 0x3727f664, /// 0xa64
			 0x97de7bdc, /// 0xa68
			 0xdea0cc43, /// 0xa6c
			 0x98b1792f, /// 0xa70
			 0xdc5fa871, /// 0xa74
			 0xd87dd32d, /// 0xa78
			 0x9730ca20, /// 0xa7c
			 0xad02ded3, /// 0xa80
			 0xc01222a5, /// 0xa84
			 0x3a16336f, /// 0xa88
			 0xa157c25e, /// 0xa8c
			 0x824a60c0, /// 0xa90
			 0x5d70109a, /// 0xa94
			 0xe613aa71, /// 0xa98
			 0xa12ea2c4, /// 0xa9c
			 0x0d499581, /// 0xaa0
			 0xa3fd6221, /// 0xaa4
			 0x537ac6bc, /// 0xaa8
			 0x36be7924, /// 0xaac
			 0x40963f97, /// 0xab0
			 0x77f3bb99, /// 0xab4
			 0xaaf8b9e3, /// 0xab8
			 0xe773b1fe, /// 0xabc
			 0xd0907a28, /// 0xac0
			 0x5a8a38d2, /// 0xac4
			 0x150596de, /// 0xac8
			 0x95f792af, /// 0xacc
			 0x524b8886, /// 0xad0
			 0xeeeebe2e, /// 0xad4
			 0x90804545, /// 0xad8
			 0xff709e3a, /// 0xadc
			 0x22a60f75, /// 0xae0
			 0x991c1cb1, /// 0xae4
			 0x713177a7, /// 0xae8
			 0x5d7e2c27, /// 0xaec
			 0xe10b9ed7, /// 0xaf0
			 0xd86e3c08, /// 0xaf4
			 0x197dbb8c, /// 0xaf8
			 0x1c3f1cdc, /// 0xafc
			 0xba0953f6, /// 0xb00
			 0xb613cd57, /// 0xb04
			 0xffc473da, /// 0xb08
			 0x77086824, /// 0xb0c
			 0x762856c9, /// 0xb10
			 0x82c55382, /// 0xb14
			 0xdda7128d, /// 0xb18
			 0x21a28029, /// 0xb1c
			 0xca50a30c, /// 0xb20
			 0x49c22495, /// 0xb24
			 0xb5ec8b74, /// 0xb28
			 0x8490801c, /// 0xb2c
			 0x24dc42eb, /// 0xb30
			 0x09c9cf46, /// 0xb34
			 0x578024fc, /// 0xb38
			 0x8fceb18b, /// 0xb3c
			 0x0fd31154, /// 0xb40
			 0x07483b81, /// 0xb44
			 0xe2cd2642, /// 0xb48
			 0x839e712b, /// 0xb4c
			 0x6c39f42a, /// 0xb50
			 0xbdee418f, /// 0xb54
			 0x33fce0ef, /// 0xb58
			 0xe76eedeb, /// 0xb5c
			 0xe1e90cea, /// 0xb60
			 0xca5882f5, /// 0xb64
			 0x3e5d90bb, /// 0xb68
			 0x613519d4, /// 0xb6c
			 0x732aefa4, /// 0xb70
			 0x47b4ee94, /// 0xb74
			 0x2085b40a, /// 0xb78
			 0x48bd69d4, /// 0xb7c
			 0x022488e2, /// 0xb80
			 0x2cda5f4a, /// 0xb84
			 0x0ee2b8c1, /// 0xb88
			 0xf51de624, /// 0xb8c
			 0x68bb00f2, /// 0xb90
			 0x1a0482b2, /// 0xb94
			 0xb56b5566, /// 0xb98
			 0x4089b93b, /// 0xb9c
			 0x56754dcf, /// 0xba0
			 0x16afcbae, /// 0xba4
			 0xedd4ec7a, /// 0xba8
			 0x563f5981, /// 0xbac
			 0x652e5ea1, /// 0xbb0
			 0x5b96bf36, /// 0xbb4
			 0x0f7e96a5, /// 0xbb8
			 0xc9d5ab25, /// 0xbbc
			 0xfd86602e, /// 0xbc0
			 0x1fd7e2c0, /// 0xbc4
			 0xf9174503, /// 0xbc8
			 0x6bbef900, /// 0xbcc
			 0x5e1d6f23, /// 0xbd0
			 0x8946b9af, /// 0xbd4
			 0xec09d51e, /// 0xbd8
			 0xbf7691d8, /// 0xbdc
			 0xe62d4ba8, /// 0xbe0
			 0xc1ebd28e, /// 0xbe4
			 0x831820db, /// 0xbe8
			 0xc1c85ae4, /// 0xbec
			 0x1c3572ba, /// 0xbf0
			 0x536e45a0, /// 0xbf4
			 0x89d7fc54, /// 0xbf8
			 0x6dabe920, /// 0xbfc
			 0xd8386029, /// 0xc00
			 0x01a3864c, /// 0xc04
			 0xde5b3b77, /// 0xc08
			 0x6b170bb2, /// 0xc0c
			 0xd77599d3, /// 0xc10
			 0xff44106a, /// 0xc14
			 0x044ba312, /// 0xc18
			 0x47cf1486, /// 0xc1c
			 0xd9bae7f9, /// 0xc20
			 0x659bb70f, /// 0xc24
			 0x466e178b, /// 0xc28
			 0x8c105279, /// 0xc2c
			 0x1f23e405, /// 0xc30
			 0xe0210673, /// 0xc34
			 0x3d80f83c, /// 0xc38
			 0x9a45851c, /// 0xc3c
			 0xf3bba2eb, /// 0xc40
			 0xee6101c5, /// 0xc44
			 0x7b678c5a, /// 0xc48
			 0xa21e7dec, /// 0xc4c
			 0xc92b8f02, /// 0xc50
			 0x327782a7, /// 0xc54
			 0xc2ea09e6, /// 0xc58
			 0x00074207, /// 0xc5c
			 0xaeb3f067, /// 0xc60
			 0x19e580a3, /// 0xc64
			 0xe34460fc, /// 0xc68
			 0x41451279, /// 0xc6c
			 0x11af38a4, /// 0xc70
			 0x4aedaf54, /// 0xc74
			 0xe142a9e3, /// 0xc78
			 0xbad14e46, /// 0xc7c
			 0x16973e69, /// 0xc80
			 0xf699d4d9, /// 0xc84
			 0x152d849a, /// 0xc88
			 0xb5732702, /// 0xc8c
			 0x5eba0f8a, /// 0xc90
			 0x119f024b, /// 0xc94
			 0xd0dd9003, /// 0xc98
			 0xdfafd27e, /// 0xc9c
			 0x8c4f03fb, /// 0xca0
			 0xef938ce0, /// 0xca4
			 0xdd793cac, /// 0xca8
			 0x58ff07d5, /// 0xcac
			 0xec3a1090, /// 0xcb0
			 0x16c0a32f, /// 0xcb4
			 0x80d1c9a2, /// 0xcb8
			 0x67eeffc0, /// 0xcbc
			 0xd11fb0f3, /// 0xcc0
			 0xd5453c4d, /// 0xcc4
			 0x7ec01b92, /// 0xcc8
			 0xcfc5f09e, /// 0xccc
			 0x98901f47, /// 0xcd0
			 0xbf8ca928, /// 0xcd4
			 0x21cf2797, /// 0xcd8
			 0xedf5e7f6, /// 0xcdc
			 0xfdd38541, /// 0xce0
			 0xae26da6d, /// 0xce4
			 0x35a49589, /// 0xce8
			 0x7c47a24e, /// 0xcec
			 0x57e867a7, /// 0xcf0
			 0x6ffd54c6, /// 0xcf4
			 0x719fa9dc, /// 0xcf8
			 0xfd25091b, /// 0xcfc
			 0x4cf862d4, /// 0xd00
			 0x266384e7, /// 0xd04
			 0x496507bc, /// 0xd08
			 0xb2c43056, /// 0xd0c
			 0x32e23de4, /// 0xd10
			 0x76b1de5b, /// 0xd14
			 0x9b43c7d5, /// 0xd18
			 0x7d792177, /// 0xd1c
			 0x39756001, /// 0xd20
			 0x7c225d7a, /// 0xd24
			 0xedc42f5a, /// 0xd28
			 0x9e3edabb, /// 0xd2c
			 0x9a16c4d8, /// 0xd30
			 0xb5e8600b, /// 0xd34
			 0x9e9af1ed, /// 0xd38
			 0xf9c45e65, /// 0xd3c
			 0x6369aa29, /// 0xd40
			 0x822bd935, /// 0xd44
			 0x6a646abe, /// 0xd48
			 0x6166c64e, /// 0xd4c
			 0x606bdf00, /// 0xd50
			 0x4cbc1e55, /// 0xd54
			 0x3c7811a3, /// 0xd58
			 0x2b5821b5, /// 0xd5c
			 0xd4b9318f, /// 0xd60
			 0x5899a0e0, /// 0xd64
			 0xe8babfea, /// 0xd68
			 0x899779d8, /// 0xd6c
			 0xea8b4d1e, /// 0xd70
			 0x21868429, /// 0xd74
			 0x25e6fadd, /// 0xd78
			 0xcf433531, /// 0xd7c
			 0xfa97daaf, /// 0xd80
			 0xeb0136c2, /// 0xd84
			 0xe2386762, /// 0xd88
			 0x84ecc2df, /// 0xd8c
			 0x1781b73b, /// 0xd90
			 0xf2f2c52d, /// 0xd94
			 0x3c090343, /// 0xd98
			 0x6dbcac44, /// 0xd9c
			 0xfa70c636, /// 0xda0
			 0x26623961, /// 0xda4
			 0xa0a1fe4e, /// 0xda8
			 0xb7d3c84c, /// 0xdac
			 0x23562293, /// 0xdb0
			 0x5e65af77, /// 0xdb4
			 0xbbda3694, /// 0xdb8
			 0xb75a9cde, /// 0xdbc
			 0x6b57ce88, /// 0xdc0
			 0x3f2ba7c3, /// 0xdc4
			 0x85835f2b, /// 0xdc8
			 0x0752ba8f, /// 0xdcc
			 0x24e367dd, /// 0xdd0
			 0xbeb3ca05, /// 0xdd4
			 0x23e9fc3a, /// 0xdd8
			 0x3d7f6c82, /// 0xddc
			 0xda85fa16, /// 0xde0
			 0x633c6ba7, /// 0xde4
			 0x9f15f1ad, /// 0xde8
			 0x21596b00, /// 0xdec
			 0xa999932f, /// 0xdf0
			 0x826b04fc, /// 0xdf4
			 0xac1e5536, /// 0xdf8
			 0xaa4ff24f, /// 0xdfc
			 0x01d4b3ee, /// 0xe00
			 0x85a925bb, /// 0xe04
			 0x3328296f, /// 0xe08
			 0x73719e7e, /// 0xe0c
			 0xf565a1c0, /// 0xe10
			 0xf161902f, /// 0xe14
			 0xd2daaa7d, /// 0xe18
			 0x97cf6ae0, /// 0xe1c
			 0xb1c14ec6, /// 0xe20
			 0xff6c46a6, /// 0xe24
			 0xe59702d6, /// 0xe28
			 0x2d9e5c87, /// 0xe2c
			 0x0cfb8c4f, /// 0xe30
			 0x8b4afb79, /// 0xe34
			 0x77522b7e, /// 0xe38
			 0x949e92d6, /// 0xe3c
			 0x4edd0c25, /// 0xe40
			 0xd01820db, /// 0xe44
			 0x623422be, /// 0xe48
			 0xadf16688, /// 0xe4c
			 0xd0dc1ea2, /// 0xe50
			 0xf14bf48f, /// 0xe54
			 0xb37540ab, /// 0xe58
			 0x8100d8cb, /// 0xe5c
			 0x9ef12bf8, /// 0xe60
			 0x21ba2673, /// 0xe64
			 0x9aa54371, /// 0xe68
			 0xb6313527, /// 0xe6c
			 0xc825f3ba, /// 0xe70
			 0xd1dc1381, /// 0xe74
			 0xf44f9c3a, /// 0xe78
			 0xd0b08013, /// 0xe7c
			 0x33d298d5, /// 0xe80
			 0x5f58db30, /// 0xe84
			 0x8a49b685, /// 0xe88
			 0x7faf37fe, /// 0xe8c
			 0x1d58ecf5, /// 0xe90
			 0x773aba1b, /// 0xe94
			 0x98547bb6, /// 0xe98
			 0x61e5468f, /// 0xe9c
			 0x96a92e41, /// 0xea0
			 0x2293c4ea, /// 0xea4
			 0x4dddfcb9, /// 0xea8
			 0xa6ee6ee0, /// 0xeac
			 0xa786e1b8, /// 0xeb0
			 0x167ff84a, /// 0xeb4
			 0xb33e3833, /// 0xeb8
			 0xa91dcc67, /// 0xebc
			 0x4d11b5f4, /// 0xec0
			 0x3cf60474, /// 0xec4
			 0xaada5d7b, /// 0xec8
			 0xe26cfbad, /// 0xecc
			 0x3297ee32, /// 0xed0
			 0xf07d1016, /// 0xed4
			 0xdd9fb338, /// 0xed8
			 0xdf9a38e0, /// 0xedc
			 0x956816f1, /// 0xee0
			 0xf69cd6ba, /// 0xee4
			 0x562eb29c, /// 0xee8
			 0xb9dcf711, /// 0xeec
			 0x25b0d141, /// 0xef0
			 0xed3fc081, /// 0xef4
			 0x13636ba8, /// 0xef8
			 0x8442bc7d, /// 0xefc
			 0x7d978c7b, /// 0xf00
			 0xa0ecc949, /// 0xf04
			 0xe16735f9, /// 0xf08
			 0xfb9766d6, /// 0xf0c
			 0x77cbcc9c, /// 0xf10
			 0x9e669743, /// 0xf14
			 0x104c10bc, /// 0xf18
			 0x0dd14947, /// 0xf1c
			 0x400d709e, /// 0xf20
			 0x7915fdf5, /// 0xf24
			 0xcab87d78, /// 0xf28
			 0x390fd830, /// 0xf2c
			 0xfee2ccf7, /// 0xf30
			 0xbc05cf31, /// 0xf34
			 0x4ddf179e, /// 0xf38
			 0x72e7be49, /// 0xf3c
			 0xef19150c, /// 0xf40
			 0xe85b3574, /// 0xf44
			 0xbba6abe8, /// 0xf48
			 0x8d6a6312, /// 0xf4c
			 0xd6c73a5a, /// 0xf50
			 0x5b7898ac, /// 0xf54
			 0x3754d684, /// 0xf58
			 0xda86d94c, /// 0xf5c
			 0x67429830, /// 0xf60
			 0xbd750ba7, /// 0xf64
			 0x13bf3fc6, /// 0xf68
			 0x1f00d573, /// 0xf6c
			 0xa1cfd565, /// 0xf70
			 0xdb31ce85, /// 0xf74
			 0x9fa09a83, /// 0xf78
			 0xc1de4e60, /// 0xf7c
			 0x590664a6, /// 0xf80
			 0xd13d9543, /// 0xf84
			 0x4482b5aa, /// 0xf88
			 0xb48f8ddf, /// 0xf8c
			 0x30a72bee, /// 0xf90
			 0x3f7a7f8a, /// 0xf94
			 0x9a1741ec, /// 0xf98
			 0xf8405dc6, /// 0xf9c
			 0x9154d7b3, /// 0xfa0
			 0xd5177a0c, /// 0xfa4
			 0x717d015d, /// 0xfa8
			 0x1b73ffa7, /// 0xfac
			 0x89b61497, /// 0xfb0
			 0x012ff0b5, /// 0xfb4
			 0xf9299fcf, /// 0xfb8
			 0x157cdde0, /// 0xfbc
			 0x0f9a18b3, /// 0xfc0
			 0xdd694bcd, /// 0xfc4
			 0x114c1d85, /// 0xfc8
			 0xca388f92, /// 0xfcc
			 0xe7741031, /// 0xfd0
			 0x9e888dc9, /// 0xfd4
			 0xb1a4102b, /// 0xfd8
			 0x97a449d9, /// 0xfdc
			 0xbe3ce135, /// 0xfe0
			 0x7f4718d0, /// 0xfe4
			 0x2eee6318, /// 0xfe8
			 0x8ed89da6, /// 0xfec
			 0x140c5b30, /// 0xff0
			 0x90b855c5, /// 0xff4
			 0xf279c934, /// 0xff8
			 0x996313bb /// last
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
			 0x00000570,
			 0x000006f4,
			 0x000006b8,
			 0x0000063c,
			 0x000000e8,
			 0x00000624,
			 0x000005e4,
			 0x000006a0,

			 /// vpu register f2
			 0x41800000,
			 0x40e00000,
			 0x41400000,
			 0x41a80000,
			 0x40800000,
			 0x41b00000,
			 0x40000000,
			 0x41900000,

			 /// vpu register f3
			 0x41c00000,
			 0x41e00000,
			 0x41c00000,
			 0x40e00000,
			 0x41b80000,
			 0x40800000,
			 0x41880000,
			 0x41d80000,

			 /// vpu register f4
			 0x3f800000,
			 0x41d00000,
			 0x41a80000,
			 0x41100000,
			 0x41400000,
			 0x41600000,
			 0x41e80000,
			 0x41300000,

			 /// vpu register f5
			 0x40800000,
			 0x41700000,
			 0x41880000,
			 0x41a80000,
			 0x41c00000,
			 0x41a80000,
			 0x41100000,
			 0x41b80000,

			 /// vpu register f6
			 0x41c80000,
			 0x40400000,
			 0x41b00000,
			 0x41700000,
			 0x41a00000,
			 0x41b80000,
			 0x41f00000,
			 0x42000000,

			 /// vpu register f7
			 0x41300000,
			 0x41e80000,
			 0x41d80000,
			 0x40a00000,
			 0x41100000,
			 0x40e00000,
			 0x41980000,
			 0x41400000,

			 /// vpu register f8
			 0x40000000,
			 0x41980000,
			 0x41b00000,
			 0x41f00000,
			 0x41500000,
			 0x41a80000,
			 0x41980000,
			 0x41900000,

			 /// vpu register f9
			 0x41a80000,
			 0x41300000,
			 0x41600000,
			 0x41400000,
			 0x41d00000,
			 0x41e00000,
			 0x41100000,
			 0x41c00000,

			 /// vpu register f10
			 0x41000000,
			 0x41900000,
			 0x41100000,
			 0x41700000,
			 0x3f800000,
			 0x3f800000,
			 0x41980000,
			 0x41980000,

			 /// vpu register f11
			 0x41f00000,
			 0x41300000,
			 0x40000000,
			 0x40a00000,
			 0x40400000,
			 0x41100000,
			 0x42000000,
			 0x41900000,

			 /// vpu register f12
			 0x41700000,
			 0x42000000,
			 0x3f800000,
			 0x41c80000,
			 0x40800000,
			 0x3f800000,
			 0x41c80000,
			 0x41a00000,

			 /// vpu register f13
			 0x41880000,
			 0x42000000,
			 0x41a80000,
			 0x41f00000,
			 0x41a80000,
			 0x41c80000,
			 0x41400000,
			 0x41b00000,

			 /// vpu register f14
			 0x41a00000,
			 0x41f00000,
			 0x41b00000,
			 0x41700000,
			 0x41e80000,
			 0x41a80000,
			 0x40800000,
			 0x41b00000,

			 /// vpu register f15
			 0x41400000,
			 0x41c80000,
			 0x41c00000,
			 0x41f00000,
			 0x40e00000,
			 0x41e00000,
			 0x41c00000,
			 0x42000000,

			 /// vpu register f16
			 0x41400000,
			 0x3f800000,
			 0x41f00000,
			 0x41a80000,
			 0x41c00000,
			 0x41600000,
			 0x41880000,
			 0x40c00000,

			 /// vpu register f17
			 0x40e00000,
			 0x41000000,
			 0x41e00000,
			 0x41a00000,
			 0x41d80000,
			 0x41880000,
			 0x41800000,
			 0x41700000,

			 /// vpu register f18
			 0x41400000,
			 0x40a00000,
			 0x40800000,
			 0x41b00000,
			 0x41300000,
			 0x42000000,
			 0x41600000,
			 0x41f00000,

			 /// vpu register f19
			 0x40c00000,
			 0x41980000,
			 0x41800000,
			 0x41900000,
			 0x40e00000,
			 0x41f80000,
			 0x41c80000,
			 0x41a80000,

			 /// vpu register f20
			 0x40400000,
			 0x41d00000,
			 0x40400000,
			 0x41500000,
			 0x41e00000,
			 0x41f80000,
			 0x41800000,
			 0x41900000,

			 /// vpu register f21
			 0x41800000,
			 0x40a00000,
			 0x41f00000,
			 0x41800000,
			 0x41000000,
			 0x40e00000,
			 0x41200000,
			 0x41a80000,

			 /// vpu register f22
			 0x41500000,
			 0x41c00000,
			 0x41e80000,
			 0x41100000,
			 0x41a80000,
			 0x40c00000,
			 0x41f00000,
			 0x41000000,

			 /// vpu register f23
			 0x41d80000,
			 0x40400000,
			 0x40800000,
			 0x41800000,
			 0x41000000,
			 0x41880000,
			 0x41100000,
			 0x41d80000,

			 /// vpu register f24
			 0x41a80000,
			 0x3f800000,
			 0x41100000,
			 0x41c80000,
			 0x41e00000,
			 0x41700000,
			 0x41f00000,
			 0x40e00000,

			 /// vpu register f25
			 0x41100000,
			 0x41500000,
			 0x41980000,
			 0x40400000,
			 0x41f00000,
			 0x41900000,
			 0x41c00000,
			 0x41700000,

			 /// vpu register f26
			 0x41c00000,
			 0x41600000,
			 0x41b80000,
			 0x41e80000,
			 0x41c80000,
			 0x40a00000,
			 0x41000000,
			 0x41e00000,

			 /// vpu register f27
			 0x41600000,
			 0x41a80000,
			 0x41600000,
			 0x41880000,
			 0x41d00000,
			 0x41a80000,
			 0x40c00000,
			 0x42000000,

			 /// vpu register f28
			 0x41500000,
			 0x41300000,
			 0x41c00000,
			 0x40a00000,
			 0x41700000,
			 0x40000000,
			 0x41d00000,
			 0x41c00000,

			 /// vpu register f29
			 0x40e00000,
			 0x41c00000,
			 0x41d00000,
			 0x41a00000,
			 0x41d80000,
			 0x40a00000,
			 0x40e00000,
			 0x41800000,

			 /// vpu register f30
			 0x41500000,
			 0x41600000,
			 0x41700000,
			 0x41800000,
			 0x40a00000,
			 0x41700000,
			 0x40e00000,
			 0x40000000,

			 /// vpu register f31
			 0x41200000,
			 0x40400000,
			 0x41880000,
			 0x41700000,
			 0x41400000,
			 0x41100000,
			 0x42000000,
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
		"fcvt.pw.ps f20, f14, rup\n"                          ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f14, f11, dyn\n"                          ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f19, f21, rdn\n"                          ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f19, f21, dyn\n"                          ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f21, f14, rtz\n"                          ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f25, f6, dyn\n"                           ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f8, f30, dyn\n"                           ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f29, f7, rdn\n"                           ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f5, f7, rdn\n"                            ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f19, f22, rmm\n"                          ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f12, f11, rdn\n"                          ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f15, f19, rtz\n"                          ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f21, f29, rup\n"                          ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f23, f11, rtz\n"                          ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f21, f15, rmm\n"                          ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f12, f17, rmm\n"                          ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f7, f7, rmm\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f18, f30, rup\n"                          ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f30, f23, rdn\n"                          ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f17, f0, dyn\n"                           ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f21, f6, rtz\n"                           ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f12, f8, rdn\n"                           ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f3, f12, rmm\n"                           ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f17, f0, rmm\n"                           ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f20, f14, rup\n"                          ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f10, f0, rne\n"                           ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f26, f24, rdn\n"                          ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f28, f14, rup\n"                          ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f22, f27, rdn\n"                          ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f2, f4, rdn\n"                            ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f21, f11, rup\n"                          ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f5, f25, rtz\n"                           ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f14, f20, rne\n"                          ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f26, f21, rup\n"                          ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f19, f30, rmm\n"                          ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f20, f17, rmm\n"                          ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f28, f20, rtz\n"                          ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f26, f16, rup\n"                          ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f12, f7, rmm\n"                           ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f14, f28, rup\n"                          ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f26, f4, rdn\n"                           ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f24, f13, rne\n"                          ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f26, f1, rdn\n"                           ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f18, f12, rtz\n"                          ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f14, f10, rmm\n"                          ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f16, f4, rdn\n"                           ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f4, f12, rup\n"                           ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f28, f26, rup\n"                          ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f8, f0, rmm\n"                            ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f9, f15, rne\n"                           ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f5, f20, rmm\n"                           ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f7, f20, rdn\n"                           ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f21, f14, rne\n"                          ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f18, f4, dyn\n"                           ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f13, f6, rup\n"                           ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f19, f22, rne\n"                          ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f21, f11, rdn\n"                          ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f11, f25, rdn\n"                          ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f20, f9, dyn\n"                           ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f2, f23, rup\n"                           ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f8, f20, rne\n"                           ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f22, f28, rtz\n"                          ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f27, f28, dyn\n"                          ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f25, f29, rup\n"                          ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f29, f19, rne\n"                          ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f8, f12, rdn\n"                           ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f30, f28, rne\n"                          ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f29, f13, dyn\n"                          ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f3, f28, rup\n"                           ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f27, f25, dyn\n"                          ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f16, f23, rmm\n"                          ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f14, f19, dyn\n"                          ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f14, f30, rtz\n"                          ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f22, f13, rmm\n"                          ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f30, f2, rup\n"                           ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f18, f25, rdn\n"                          ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f8, f23, rup\n"                           ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f21, f4, dyn\n"                           ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f7, f22, rmm\n"                           ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f23, f9, rtz\n"                           ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f26, f22, rne\n"                          ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f13, f12, rup\n"                          ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f3, f9, rup\n"                            ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f13, f16, rmm\n"                          ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f21, f4, rup\n"                           ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f13, f29, rmm\n"                          ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f9, f27, rup\n"                           ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f17, f14, rne\n"                          ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f26, f28, rdn\n"                          ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f25, f14, rup\n"                          ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f17, f24, rtz\n"                          ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f6, f12, rdn\n"                           ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f18, f11, rmm\n"                          ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f11, f29, rtz\n"                          ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f14, f5, dyn\n"                           ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f11, f18, rne\n"                          ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f23, f9, rne\n"                           ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f18, f5, rmm\n"                           ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.pw.ps f22, f27, rtz\n"                          ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
