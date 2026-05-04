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
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00000
			 0x7fc00001,                                                  // signaling NaN                               /// 00004
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0000c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00010
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00014
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0001c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00028
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00030
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0003c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x7fc00001,                                                  // signaling NaN                               /// 00048
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0004c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00050
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00054
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00058
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0005c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00060
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00064
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0006c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00070
			 0x0c600000,                                                  // Leading 1s:                                 /// 00074
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00078
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0007c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00088
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00090
			 0x0c700000,                                                  // Leading 1s:                                 /// 00094
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00098
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000a0
			 0x7fc00001,                                                  // signaling NaN                               /// 000a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 000b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0xff800000,                                                  // -inf                                        /// 000d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000e0
			 0xff800000,                                                  // -inf                                        /// 000e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0xff800000,                                                  // -inf                                        /// 000ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00104
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x80000000,                                                  // -zero                                       /// 00110
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00118
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0011c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00120
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x0c400000,                                                  // Leading 1s:                                 /// 00128
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0012c
			 0x4b000000,                                                  // 8388608.0                                   /// 00130
			 0x7fc00001,                                                  // signaling NaN                               /// 00134
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x80011111,                                                  // -9.7958E-41                                 /// 0013c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00140
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00144
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0014c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00150
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00154
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00158
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0015c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00164
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00168
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0016c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00170
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00178
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0017c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00180
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00184
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00188
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0018c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0019c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001bc
			 0x00000000,                                                  // zero                                        /// 001c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 001dc
			 0x3f028f5c,                                                  // 0.51                                        /// 001e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001fc
			 0x00000000,                                                  // zero                                        /// 00200
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0020c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00210
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00214
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0021c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00224
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00228
			 0x0c700000,                                                  // Leading 1s:                                 /// 0022c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00234
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00238
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0023c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00240
			 0x0c780000,                                                  // Leading 1s:                                 /// 00244
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00260
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00264
			 0x0e000007,                                                  // Trailing 1s:                                /// 00268
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0026c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00274
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00278
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0027c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x7fc00001,                                                  // signaling NaN                               /// 00284
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00288
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00294
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00298
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0029c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 002bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e4
			 0x4b000000,                                                  // 8388608.0                                   /// 002e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0c600000,                                                  // Leading 1s:                                 /// 00304
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00308
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0030c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00310
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00314
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00318
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0031c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00324
			 0xff800000,                                                  // -inf                                        /// 00328
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0032c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00334
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00338
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0033c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00344
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00348
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00350
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00354
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0035c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00360
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00368
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00370
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00374
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00378
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00388
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0xff800000,                                                  // -inf                                        /// 00390
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00398
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0039c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003cc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00400
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00404
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00408
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0040c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00414
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00418
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0041c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00420
			 0x55555555,                                                  // 4 random values                             /// 00424
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0042c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00430
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00434
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00438
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00440
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00444
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00448
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00454
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00458
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0045c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00460
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00464
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00468
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0046c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00470
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00474
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00478
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0047c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00480
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00488
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00494
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00498
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x33333333,                                                  // 4 random values                             /// 004fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00500
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00504
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00508
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0050c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00510
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00514
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0xff800000,                                                  // -inf                                        /// 0051c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00520
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00524
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00528
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0052c
			 0x00000000,                                                  // zero                                        /// 00530
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0053c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00540
			 0x3f028f5c,                                                  // 0.51                                        /// 00544
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0054c
			 0x3f028f5c,                                                  // 0.51                                        /// 00550
			 0x7fc00001,                                                  // signaling NaN                               /// 00554
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0055c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00564
			 0x33333333,                                                  // 4 random values                             /// 00568
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0056c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00570
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00578
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0057c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00580
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00584
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00590
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00594
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0059c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005b0
			 0xbf028f5c,                                                  // -0.51                                       /// 005b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 005c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 005c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 005ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005fc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00600
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00608
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0060c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00610
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00614
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00618
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0e000007,                                                  // Trailing 1s:                                /// 00624
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00628
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00630
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x33333333,                                                  // 4 random values                             /// 00638
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00640
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00644
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00648
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0064c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x7f800000,                                                  // inf                                         /// 00654
			 0x0c400000,                                                  // Leading 1s:                                 /// 00658
			 0x55555555,                                                  // 4 random values                             /// 0065c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00664
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00668
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0066c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00670
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00674
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00680
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00688
			 0x0e000007,                                                  // Trailing 1s:                                /// 0068c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00690
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00698
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0069c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006c4
			 0x33333333,                                                  // 4 random values                             /// 006c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 006e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 006f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 006f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00700
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00704
			 0x4b000000,                                                  // 8388608.0                                   /// 00708
			 0x0c780000,                                                  // Leading 1s:                                 /// 0070c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00710
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00714
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00718
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0071c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00720
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00724
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00728
			 0x0e000001,                                                  // Trailing 1s:                                /// 0072c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00730
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00734
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0073c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00744
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00748
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0074c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00750
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00758
			 0x0c600000,                                                  // Leading 1s:                                 /// 0075c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00760
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00768
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00770
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00778
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0077c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00780
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00784
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00788
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00790
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0c600000,                                                  // Leading 1s:                                 /// 007a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007b4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007b8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 007d0
			 0xff800000,                                                  // -inf                                        /// 007d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00800
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00804
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00808
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00810
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00814
			 0x3f028f5c,                                                  // 0.51                                        /// 00818
			 0x4b000000,                                                  // 8388608.0                                   /// 0081c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00824
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00828
			 0x3f028f5c,                                                  // 0.51                                        /// 0082c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00830
			 0x0e000003,                                                  // Trailing 1s:                                /// 00834
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00838
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0083c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00844
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00848
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0084c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00850
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00854
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00858
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0x7fc00001,                                                  // signaling NaN                               /// 00860
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00864
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0086c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00870
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00878
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0087c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00880
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00884
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00888
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0088c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00894
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0089c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x4b000000,                                                  // 8388608.0                                   /// 008a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 008b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008cc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0xbf028f5c,                                                  // -0.51                                       /// 008e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x7fc00001,                                                  // signaling NaN                               /// 008f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00904
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00908
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0xffc00001,                                                  // -signaling NaN                              /// 00910
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00918
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0091c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00920
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00924
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0092c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00930
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00934
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0093c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00940
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00948
			 0x0e000003,                                                  // Trailing 1s:                                /// 0094c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00950
			 0x7f800000,                                                  // inf                                         /// 00954
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00958
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00964
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00968
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0096c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00970
			 0x0c400000,                                                  // Leading 1s:                                 /// 00974
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00978
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0097c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00980
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0098c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00990
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00994
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0e000003,                                                  // Trailing 1s:                                /// 0099c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009b0
			 0xff800000,                                                  // -inf                                        /// 009b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009d8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009f0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 009f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a04
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a10
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a14
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a1c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a28
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a34
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a38
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a48
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a68
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a6c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a74
			 0x7fc00001,                                                  // signaling NaN                               /// 00a78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a80
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a90
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a98
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aa0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aac
			 0xcb000000,                                                  // -8388608.0                                  /// 00ab0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ac0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ac4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ac8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00acc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ad8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ae8
			 0xff800000,                                                  // -inf                                        /// 00aec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00af0
			 0x7f800000,                                                  // inf                                         /// 00af4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00af8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00afc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b18
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b2c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b38
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b40
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b44
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b4c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b58
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x7f800000,                                                  // inf                                         /// 00b60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b78
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b7c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b88
			 0xffc00001,                                                  // -signaling NaN                              /// 00b8c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b94
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00ba0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ba4
			 0x33333333,                                                  // 4 random values                             /// 00ba8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bb0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bb8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bbc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bcc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bd0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bd4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bdc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00be4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bf4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bfc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c00
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c04
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c08
			 0x7fc00001,                                                  // signaling NaN                               /// 00c0c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c10
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c20
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c2c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c30
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c44
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c54
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c60
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c64
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c78
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c7c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ca4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cb4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cb8
			 0x7fc00001,                                                  // signaling NaN                               /// 00cbc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cc4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cc8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ccc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cd0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ce4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cf0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cf8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d00
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d04
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d0c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d14
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d20
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d24
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d28
			 0xff800000,                                                  // -inf                                        /// 00d2c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d30
			 0x00000000,                                                  // zero                                        /// 00d34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d38
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d40
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d44
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d48
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d50
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x3f028f5c,                                                  // 0.51                                        /// 00d68
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d70
			 0x7fc00001,                                                  // signaling NaN                               /// 00d74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d78
			 0x00000000,                                                  // zero                                        /// 00d7c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x7fc00001,                                                  // signaling NaN                               /// 00d94
			 0x7f800000,                                                  // inf                                         /// 00d98
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d9c
			 0x80000000,                                                  // -zero                                       /// 00da0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00da8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00db0
			 0x3f028f5c,                                                  // 0.51                                        /// 00db4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dbc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00de0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00de8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00df0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00df4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dfc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e1c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e20
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e28
			 0x55555555,                                                  // 4 random values                             /// 00e2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e38
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e3c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e44
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e54
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e70
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e84
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e8c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e94
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e98
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eb8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ebc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ec0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ec8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ecc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ed0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ed4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ed8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ee0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ee8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ef0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ef4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00efc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f00
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x3f028f5c,                                                  // 0.51                                        /// 00f08
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f0c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f10
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f18
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f20
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f34
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x80000000,                                                  // -zero                                       /// 00f3c
			 0x55555555,                                                  // 4 random values                             /// 00f40
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f44
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f48
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f4c
			 0x00000000,                                                  // zero                                        /// 00f50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f84
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fa0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fac
			 0x55555555,                                                  // 4 random values                             /// 00fb0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fb8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fbc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fc4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fc8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fcc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fd0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x00000000,                                                  // zero                                        /// 00fd8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fe0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ff0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ff4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ff8
			 0x4b8c4b40                                                  // 18388608.0                                  /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00000
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00008
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0000c
			 0xffc00001,                                                  // -signaling NaN                              /// 00010
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00014
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0001c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00020
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00028
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00030
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0003c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0004c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00050
			 0x80000000,                                                  // -zero                                       /// 00054
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00058
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0005c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00060
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00064
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00068
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0006c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00070
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0007c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00084
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00088
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0008c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00090
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00098
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0009c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000b8
			 0xff800000,                                                  // -inf                                        /// 000bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000e0
			 0x7fc00001,                                                  // signaling NaN                               /// 000e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00100
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00104
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0010c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0011c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00120
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00124
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00128
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0012c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00130
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00138
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0013c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00140
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00144
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00148
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00150
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0015c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00160
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00164
			 0x55555555,                                                  // 4 random values                             /// 00168
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00170
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00174
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00178
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00184
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00188
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00190
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00194
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0019c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0xffc00001,                                                  // -signaling NaN                              /// 001f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00208
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0xcb000000,                                                  // -8388608.0                                  /// 00210
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00214
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00218
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00220
			 0xbf028f5c,                                                  // -0.51                                       /// 00224
			 0x0c700000,                                                  // Leading 1s:                                 /// 00228
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0022c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00230
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00238
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0023c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00240
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00244
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0024c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00258
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0025c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00260
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0026c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00274
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00278
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0027c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00280
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00288
			 0xffc00001,                                                  // -signaling NaN                              /// 0028c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00290
			 0x4b000000,                                                  // 8388608.0                                   /// 00294
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00298
			 0x0e000007,                                                  // Trailing 1s:                                /// 0029c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002b8
			 0x7f800000,                                                  // inf                                         /// 002bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002c4
			 0x00000000,                                                  // zero                                        /// 002c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002d4
			 0x4b000000,                                                  // 8388608.0                                   /// 002d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e0
			 0x80000000,                                                  // -zero                                       /// 002e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002e8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00304
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00308
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0030c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00310
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00320
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0032c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00334
			 0x00000000,                                                  // zero                                        /// 00338
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00340
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0034c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00358
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0035c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00360
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0036c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00374
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00380
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00384
			 0x00011111,                                                  // 9.7958E-41                                  /// 00388
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0038c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00390
			 0x0e000001,                                                  // Trailing 1s:                                /// 00394
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00398
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003a4
			 0xffc00001,                                                  // -signaling NaN                              /// 003a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003b8
			 0x33333333,                                                  // 4 random values                             /// 003bc
			 0xff800000,                                                  // -inf                                        /// 003c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x00000000,                                                  // zero                                        /// 003d0
			 0x80000000,                                                  // -zero                                       /// 003d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 003e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00400
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00404
			 0x0c700000,                                                  // Leading 1s:                                 /// 00408
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0040c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00414
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00418
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00420
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00424
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00428
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0042c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00430
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00434
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x3f028f5c,                                                  // 0.51                                        /// 0043c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00440
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00444
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00448
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0044c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00454
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00458
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00460
			 0x3f028f5c,                                                  // 0.51                                        /// 00464
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00468
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0046c
			 0x4b000000,                                                  // 8388608.0                                   /// 00470
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00474
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00478
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0047c
			 0x7fc00001,                                                  // signaling NaN                               /// 00480
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00484
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00488
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0048c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00490
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00494
			 0x0c700000,                                                  // Leading 1s:                                 /// 00498
			 0x80000000,                                                  // -zero                                       /// 0049c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 004ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004b4
			 0x80000000,                                                  // -zero                                       /// 004b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0xbf028f5c,                                                  // -0.51                                       /// 004fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00504
			 0x4b000000,                                                  // 8388608.0                                   /// 00508
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0050c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x0e000001,                                                  // Trailing 1s:                                /// 00514
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00518
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0051c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x0e000001,                                                  // Trailing 1s:                                /// 00524
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00528
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0052c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00530
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00538
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00540
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00544
			 0x0e000003,                                                  // Trailing 1s:                                /// 00548
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0054c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00550
			 0x3f028f5c,                                                  // 0.51                                        /// 00554
			 0x7f800000,                                                  // inf                                         /// 00558
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0055c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00564
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00568
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0056c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00570
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00578
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00580
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00584
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00588
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00598
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0059c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x80000000,                                                  // -zero                                       /// 005ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005b8
			 0x7f800000,                                                  // inf                                         /// 005bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005e4
			 0x80000000,                                                  // -zero                                       /// 005e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005f0
			 0x4b000000,                                                  // 8388608.0                                   /// 005f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00604
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00608
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0060c
			 0x55555555,                                                  // 4 random values                             /// 00610
			 0x00000000,                                                  // zero                                        /// 00614
			 0x0e000007,                                                  // Trailing 1s:                                /// 00618
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00624
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00630
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0063c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00640
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00644
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00648
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0064c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00650
			 0x0c780000,                                                  // Leading 1s:                                 /// 00654
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00658
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00660
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00664
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0066c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00670
			 0x55555555,                                                  // 4 random values                             /// 00674
			 0x0c700000,                                                  // Leading 1s:                                 /// 00678
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00684
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0068c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00690
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00698
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0069c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 006d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00700
			 0x0e000001,                                                  // Trailing 1s:                                /// 00704
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00708
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0070c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00718
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0071c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00734
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00738
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00740
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00744
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0074c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00754
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0075c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00760
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00764
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0076c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0077c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00780
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00784
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0xcb000000,                                                  // -8388608.0                                  /// 00794
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00798
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0079c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0xff800000,                                                  // -inf                                        /// 007fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00800
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00804
			 0xff800000,                                                  // -inf                                        /// 00808
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00810
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00814
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00818
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0081c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00820
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00824
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00828
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0082c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00830
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00834
			 0x33333333,                                                  // 4 random values                             /// 00838
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0083c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00840
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00844
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00848
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0084c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00850
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00854
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0085c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00868
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0086c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00870
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00874
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00878
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0087c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00884
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00890
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00894
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00898
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0089c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80000000,                                                  // -zero                                       /// 008bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x7f800000,                                                  // inf                                         /// 008f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 008f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00908
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0090c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00910
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00918
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0091c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00920
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00928
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0092c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00930
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00938
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0093c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00940
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0094c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00954
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00958
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0095c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00960
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0096c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00970
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00974
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0097c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00980
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0098c
			 0xffc00001,                                                  // -signaling NaN                              /// 00990
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00994
			 0x0c700000,                                                  // Leading 1s:                                 /// 00998
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0099c
			 0xbf028f5c,                                                  // -0.51                                       /// 009a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009b4
			 0x33333333,                                                  // 4 random values                             /// 009b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a00
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a08
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a20
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a38
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a3c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x55555555,                                                  // 4 random values                             /// 00a50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a5c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a60
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a70
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a78
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a80
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a90
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a98
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0xff800000,                                                  // -inf                                        /// 00ab0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ab4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00abc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ac0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ac8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00acc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00adc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00aec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00af4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00afc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b18
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b1c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b20
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b30
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b34
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b38
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b48
			 0xbf028f5c,                                                  // -0.51                                       /// 00b4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b54
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b58
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b5c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b60
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b64
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b68
			 0xcb000000,                                                  // -8388608.0                                  /// 00b6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b70
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b84
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b88
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b98
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b9c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ba4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ba8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bb0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bb4
			 0x80000000,                                                  // -zero                                       /// 00bb8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bc0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bc4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bcc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bd0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bd4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bd8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00be0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00be4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bf4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c08
			 0xff800000,                                                  // -inf                                        /// 00c0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c14
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c18
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x80000000,                                                  // -zero                                       /// 00c20
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c34
			 0x33333333,                                                  // 4 random values                             /// 00c38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c3c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c48
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c50
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c60
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c6c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c78
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c80
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c84
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c88
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c9c
			 0x80000000,                                                  // -zero                                       /// 00ca0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ca8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cb0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cb8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cc8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cdc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ce0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ce8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cf4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cf8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cfc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d08
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d0c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d14
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d18
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d20
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d28
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d30
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d38
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d40
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d48
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d4c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d58
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d68
			 0x7f800000,                                                  // inf                                         /// 00d6c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d84
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d90
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d98
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00da0
			 0x7f800000,                                                  // inf                                         /// 00da4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00da8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00db0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00db4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dbc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dc0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dc4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dc8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dcc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dd4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dd8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ddc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00de0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00df4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dfc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e10
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e38
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e48
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e64
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e6c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e70
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e7c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ea0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ea4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ea8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00eac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00eb4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ec0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ecc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ed0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ed4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ed8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00edc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ee0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ee4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00eec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ef4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ef8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00efc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f0c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f14
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0xbf028f5c,                                                  // -0.51                                       /// 00f1c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f28
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f38
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f40
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f54
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f64
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f70
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f78
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f80
			 0x00000000,                                                  // zero                                        /// 00f84
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f88
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f98
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fa0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fa4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fa8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fb4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fb8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fbc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fcc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fd4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fe0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ff0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x00011111                                                  // 9.7958E-41                                  /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xddd7bd12, /// 0x0
			 0xc066b525, /// 0x4
			 0x4bd059ae, /// 0x8
			 0x0bad18d2, /// 0xc
			 0x121f084c, /// 0x10
			 0xe7efc408, /// 0x14
			 0x9d1bb2d2, /// 0x18
			 0x72c9f294, /// 0x1c
			 0xc8b7391c, /// 0x20
			 0x7b5eabd3, /// 0x24
			 0x1bf5cc32, /// 0x28
			 0x2b2e2c04, /// 0x2c
			 0x96678576, /// 0x30
			 0x395abed2, /// 0x34
			 0x2b3dd802, /// 0x38
			 0x5dbe42ce, /// 0x3c
			 0xcbc2e3ca, /// 0x40
			 0x5f4fc1b3, /// 0x44
			 0x3225ea93, /// 0x48
			 0x8d42bb7e, /// 0x4c
			 0xe595c28e, /// 0x50
			 0xef3c2a9a, /// 0x54
			 0x8ef911b0, /// 0x58
			 0xe84fe3d1, /// 0x5c
			 0x2111e43a, /// 0x60
			 0x2bdf4039, /// 0x64
			 0xcbc589fc, /// 0x68
			 0xae5a9e08, /// 0x6c
			 0x2e8f59ca, /// 0x70
			 0x3a0bbb70, /// 0x74
			 0x5b2b9b59, /// 0x78
			 0x9b2b6cdf, /// 0x7c
			 0x3c960672, /// 0x80
			 0xe929c7f9, /// 0x84
			 0xcda32fed, /// 0x88
			 0xec1229e4, /// 0x8c
			 0xdce80c55, /// 0x90
			 0xa09e4988, /// 0x94
			 0x8b95fca8, /// 0x98
			 0xb99bb1c6, /// 0x9c
			 0x5ba626d5, /// 0xa0
			 0xa94ee68f, /// 0xa4
			 0x9e92e64b, /// 0xa8
			 0x211737f1, /// 0xac
			 0x7f5f6d71, /// 0xb0
			 0x037066b6, /// 0xb4
			 0x326770a0, /// 0xb8
			 0x0a5f983b, /// 0xbc
			 0x8496f453, /// 0xc0
			 0x540452aa, /// 0xc4
			 0xadd470c4, /// 0xc8
			 0x88fe125d, /// 0xcc
			 0x3cfec592, /// 0xd0
			 0xbd9ed328, /// 0xd4
			 0x46db9b1f, /// 0xd8
			 0xe40b4364, /// 0xdc
			 0xfeb9cca1, /// 0xe0
			 0x91666a1f, /// 0xe4
			 0xcd85103a, /// 0xe8
			 0x1f8981f0, /// 0xec
			 0x0fede1a6, /// 0xf0
			 0x42d523a6, /// 0xf4
			 0xbbd951c2, /// 0xf8
			 0x5167ed76, /// 0xfc
			 0x0372378f, /// 0x100
			 0x05481f8f, /// 0x104
			 0x63bd4dbc, /// 0x108
			 0x62e59ae0, /// 0x10c
			 0x339421bf, /// 0x110
			 0xdcd58fa2, /// 0x114
			 0x99f7afec, /// 0x118
			 0x478bc022, /// 0x11c
			 0x1ef24903, /// 0x120
			 0xffa349e3, /// 0x124
			 0xc115f066, /// 0x128
			 0x3e507cc6, /// 0x12c
			 0x8ede2e60, /// 0x130
			 0x7cc60183, /// 0x134
			 0x44b22727, /// 0x138
			 0xce67ebde, /// 0x13c
			 0x09cc10de, /// 0x140
			 0x65b8113c, /// 0x144
			 0x7c8ca5b4, /// 0x148
			 0xe4d5c0e1, /// 0x14c
			 0x383bfff8, /// 0x150
			 0x1e434c87, /// 0x154
			 0x7e59cf3a, /// 0x158
			 0xe49fa0b8, /// 0x15c
			 0x1b5b32c0, /// 0x160
			 0xabdc8a48, /// 0x164
			 0x42cfbbb8, /// 0x168
			 0xd17c4ad9, /// 0x16c
			 0x85bef964, /// 0x170
			 0x56846213, /// 0x174
			 0x61d0d5d4, /// 0x178
			 0xe48dcd28, /// 0x17c
			 0x078f34b0, /// 0x180
			 0xe19a592c, /// 0x184
			 0xbc6928f0, /// 0x188
			 0x4b01cb4b, /// 0x18c
			 0xc836319b, /// 0x190
			 0xd0a2ceb3, /// 0x194
			 0x459bc454, /// 0x198
			 0x0c13c99a, /// 0x19c
			 0x60911f96, /// 0x1a0
			 0x82cc2470, /// 0x1a4
			 0x2dc69aa4, /// 0x1a8
			 0x3f0f6e72, /// 0x1ac
			 0xcf9340ad, /// 0x1b0
			 0x88390fac, /// 0x1b4
			 0x40cae177, /// 0x1b8
			 0xebf67c30, /// 0x1bc
			 0x730dcffe, /// 0x1c0
			 0xe559e8a1, /// 0x1c4
			 0x703e2b09, /// 0x1c8
			 0x732fa5f7, /// 0x1cc
			 0x93f8e21c, /// 0x1d0
			 0x4af0c8cf, /// 0x1d4
			 0xab83b43f, /// 0x1d8
			 0x62403897, /// 0x1dc
			 0xe0216ada, /// 0x1e0
			 0xcda7829f, /// 0x1e4
			 0x3cbb2230, /// 0x1e8
			 0x1a25bfeb, /// 0x1ec
			 0x0b9fcfe8, /// 0x1f0
			 0xd807fc1f, /// 0x1f4
			 0xc31178b5, /// 0x1f8
			 0xc0dd12e2, /// 0x1fc
			 0x65b6738d, /// 0x200
			 0x88d68b1e, /// 0x204
			 0x4232323c, /// 0x208
			 0x33bee920, /// 0x20c
			 0x8390ffaa, /// 0x210
			 0x68da04d7, /// 0x214
			 0x6af9f464, /// 0x218
			 0x5abf9696, /// 0x21c
			 0x8f72b68f, /// 0x220
			 0xbf73f56d, /// 0x224
			 0x6e30203c, /// 0x228
			 0x5416ec40, /// 0x22c
			 0x90c62d70, /// 0x230
			 0x87817061, /// 0x234
			 0x96196124, /// 0x238
			 0xe779b4a4, /// 0x23c
			 0x24b9b2cd, /// 0x240
			 0x046df83d, /// 0x244
			 0x5b83d9bb, /// 0x248
			 0xde0d51b7, /// 0x24c
			 0xba6365b1, /// 0x250
			 0x1f8e7e70, /// 0x254
			 0x78d20f5f, /// 0x258
			 0x85da3313, /// 0x25c
			 0x32239e96, /// 0x260
			 0x0e43289a, /// 0x264
			 0x27dbc2be, /// 0x268
			 0x08014c17, /// 0x26c
			 0x4f3135b7, /// 0x270
			 0x72caf4f1, /// 0x274
			 0x27efcb88, /// 0x278
			 0xead5e4f2, /// 0x27c
			 0xd23a898c, /// 0x280
			 0x2d5ef40c, /// 0x284
			 0x7f4b6871, /// 0x288
			 0x40ec3046, /// 0x28c
			 0xae94dba0, /// 0x290
			 0x34879a4b, /// 0x294
			 0xec92a57a, /// 0x298
			 0x9ef285fe, /// 0x29c
			 0xe3683245, /// 0x2a0
			 0x72326e12, /// 0x2a4
			 0x92e93d1c, /// 0x2a8
			 0x75c90f5c, /// 0x2ac
			 0x08ecbaa1, /// 0x2b0
			 0xe0563eaa, /// 0x2b4
			 0x630b1f76, /// 0x2b8
			 0x85ff9e8b, /// 0x2bc
			 0xeadcd176, /// 0x2c0
			 0x00f7b43c, /// 0x2c4
			 0x6ca0dbc7, /// 0x2c8
			 0xde88b6f3, /// 0x2cc
			 0xe296a025, /// 0x2d0
			 0x290907f9, /// 0x2d4
			 0xaf6eae94, /// 0x2d8
			 0x241e11ae, /// 0x2dc
			 0x42a2c5bf, /// 0x2e0
			 0x7ecd5bb9, /// 0x2e4
			 0xfdfd8000, /// 0x2e8
			 0x974f1f41, /// 0x2ec
			 0xd6ee8b15, /// 0x2f0
			 0xc76ea657, /// 0x2f4
			 0x870e1ee4, /// 0x2f8
			 0x2c86436c, /// 0x2fc
			 0x77c1b0a7, /// 0x300
			 0x11040245, /// 0x304
			 0xdc0c9a7f, /// 0x308
			 0x52bd9fdb, /// 0x30c
			 0x82b15169, /// 0x310
			 0xbf6a3e98, /// 0x314
			 0x53160198, /// 0x318
			 0xc1c94c86, /// 0x31c
			 0x79be78dd, /// 0x320
			 0xf9c150a9, /// 0x324
			 0x59fd67fd, /// 0x328
			 0x4f571613, /// 0x32c
			 0xb1c86698, /// 0x330
			 0x67ce4f00, /// 0x334
			 0x08e90fb4, /// 0x338
			 0xe5ce3dee, /// 0x33c
			 0x57ee79b2, /// 0x340
			 0x4067f822, /// 0x344
			 0x8995fedd, /// 0x348
			 0x9cfb4351, /// 0x34c
			 0xf16b5ea0, /// 0x350
			 0x9406b3a8, /// 0x354
			 0xb6ad46c3, /// 0x358
			 0xb3c46737, /// 0x35c
			 0x2d61f690, /// 0x360
			 0x997d7de0, /// 0x364
			 0x8fca3096, /// 0x368
			 0x151a9aee, /// 0x36c
			 0x3b27b46b, /// 0x370
			 0xb9cf4768, /// 0x374
			 0x883bd78e, /// 0x378
			 0x43dbb3f4, /// 0x37c
			 0xc05b5f2a, /// 0x380
			 0xfe1acc61, /// 0x384
			 0xb430f98e, /// 0x388
			 0x8986b5e0, /// 0x38c
			 0x4a5863de, /// 0x390
			 0x4b09a4ac, /// 0x394
			 0xd4071af0, /// 0x398
			 0x5203e829, /// 0x39c
			 0xc78d5d89, /// 0x3a0
			 0x70573495, /// 0x3a4
			 0x63117ae2, /// 0x3a8
			 0x74a8f1e6, /// 0x3ac
			 0x9ed7dc8f, /// 0x3b0
			 0xd0ae9291, /// 0x3b4
			 0xdcb18659, /// 0x3b8
			 0x23e60c3d, /// 0x3bc
			 0xffe01d5f, /// 0x3c0
			 0x8db6bd30, /// 0x3c4
			 0x19c09cd5, /// 0x3c8
			 0xd8320ec2, /// 0x3cc
			 0x7845a5bb, /// 0x3d0
			 0x446c154c, /// 0x3d4
			 0x7ae66c67, /// 0x3d8
			 0x248cfa39, /// 0x3dc
			 0xc4227bc3, /// 0x3e0
			 0x8f0f83a1, /// 0x3e4
			 0xe92baab6, /// 0x3e8
			 0x344774b8, /// 0x3ec
			 0x54ceb1e2, /// 0x3f0
			 0x63888574, /// 0x3f4
			 0x1bee38e5, /// 0x3f8
			 0xf5fa146d, /// 0x3fc
			 0xc8d59bf5, /// 0x400
			 0x027c2084, /// 0x404
			 0x58c76984, /// 0x408
			 0xd8c1783b, /// 0x40c
			 0xee004dc8, /// 0x410
			 0xdd751d81, /// 0x414
			 0xaeacc7a7, /// 0x418
			 0xae4f629b, /// 0x41c
			 0x9e4e023e, /// 0x420
			 0x535aa3fc, /// 0x424
			 0xf6bf3d6b, /// 0x428
			 0x05838162, /// 0x42c
			 0x8ab64e43, /// 0x430
			 0x69d94120, /// 0x434
			 0x73a873c9, /// 0x438
			 0x76e5270f, /// 0x43c
			 0x3f0ec396, /// 0x440
			 0x61a5cbc9, /// 0x444
			 0x4fe47f24, /// 0x448
			 0xb569511c, /// 0x44c
			 0xb407298c, /// 0x450
			 0xaedf1b62, /// 0x454
			 0x776cd9e8, /// 0x458
			 0xba5d0444, /// 0x45c
			 0xf2368e0b, /// 0x460
			 0x0a539ca9, /// 0x464
			 0x88a7f0a4, /// 0x468
			 0x8b0b0f7f, /// 0x46c
			 0x90dfe8c0, /// 0x470
			 0x1b1207b8, /// 0x474
			 0x070d25d7, /// 0x478
			 0x85311fa9, /// 0x47c
			 0x7fbfc5ad, /// 0x480
			 0x46b6fdc5, /// 0x484
			 0xec551c97, /// 0x488
			 0xb269ccd0, /// 0x48c
			 0xcab40fc7, /// 0x490
			 0x417447d2, /// 0x494
			 0x3d2140aa, /// 0x498
			 0x24c6ed90, /// 0x49c
			 0x2cee5eee, /// 0x4a0
			 0x2f96f257, /// 0x4a4
			 0x0d4ab9fe, /// 0x4a8
			 0x23ccf4fa, /// 0x4ac
			 0xe66044a1, /// 0x4b0
			 0x369c6c8d, /// 0x4b4
			 0xfce7c7f7, /// 0x4b8
			 0xf76dbbb3, /// 0x4bc
			 0xfa4a06c4, /// 0x4c0
			 0x218fb2c0, /// 0x4c4
			 0x1d0eb742, /// 0x4c8
			 0x1b671e1d, /// 0x4cc
			 0xd299f4bd, /// 0x4d0
			 0xcc0ac673, /// 0x4d4
			 0x1176d493, /// 0x4d8
			 0xfbc45afd, /// 0x4dc
			 0xd1be5c93, /// 0x4e0
			 0xa334fccf, /// 0x4e4
			 0x8a968624, /// 0x4e8
			 0x6bc52cba, /// 0x4ec
			 0x805a4870, /// 0x4f0
			 0xa2543903, /// 0x4f4
			 0x65323354, /// 0x4f8
			 0x13f8d76c, /// 0x4fc
			 0xa2fa7dfe, /// 0x500
			 0x6d7bfe1e, /// 0x504
			 0xcb326947, /// 0x508
			 0xb0e70217, /// 0x50c
			 0xc64c564e, /// 0x510
			 0x4deb3232, /// 0x514
			 0x2d265d57, /// 0x518
			 0xc179cd73, /// 0x51c
			 0x7c85e322, /// 0x520
			 0x780a89c5, /// 0x524
			 0xd21c2441, /// 0x528
			 0xae143902, /// 0x52c
			 0xd90c0258, /// 0x530
			 0xaae32f19, /// 0x534
			 0xb9d049b5, /// 0x538
			 0x95f50d1a, /// 0x53c
			 0x56bf7d0b, /// 0x540
			 0xa28d1390, /// 0x544
			 0x53703bd0, /// 0x548
			 0x269b926c, /// 0x54c
			 0x576e5ac7, /// 0x550
			 0x1c131a55, /// 0x554
			 0x71a74d2d, /// 0x558
			 0x3f5ec4c9, /// 0x55c
			 0x1dc17746, /// 0x560
			 0xaae9a8b9, /// 0x564
			 0x0b9a4d9f, /// 0x568
			 0x2105e526, /// 0x56c
			 0x2f603fe2, /// 0x570
			 0x6efeef66, /// 0x574
			 0x34a0ffd8, /// 0x578
			 0xfbb6d39e, /// 0x57c
			 0x98196013, /// 0x580
			 0xdd142b82, /// 0x584
			 0xf37cf715, /// 0x588
			 0xc075e36b, /// 0x58c
			 0x3489eda1, /// 0x590
			 0x2af8e2e7, /// 0x594
			 0x099de282, /// 0x598
			 0xf17193d3, /// 0x59c
			 0x7ac3899b, /// 0x5a0
			 0xa8d833b3, /// 0x5a4
			 0xd5d9e8f5, /// 0x5a8
			 0xcb1c6365, /// 0x5ac
			 0x1050f36c, /// 0x5b0
			 0x255b3e90, /// 0x5b4
			 0xa9e43dc5, /// 0x5b8
			 0x80aed565, /// 0x5bc
			 0x62c5c680, /// 0x5c0
			 0x716d67c8, /// 0x5c4
			 0x64aa01b4, /// 0x5c8
			 0x26216578, /// 0x5cc
			 0xf0d4baf9, /// 0x5d0
			 0x50a332c8, /// 0x5d4
			 0x1022493d, /// 0x5d8
			 0x6bf665a7, /// 0x5dc
			 0x7fa1a1ab, /// 0x5e0
			 0x48d275c7, /// 0x5e4
			 0x76834d72, /// 0x5e8
			 0x189674a3, /// 0x5ec
			 0x6e850011, /// 0x5f0
			 0x07cb2295, /// 0x5f4
			 0x4c5552df, /// 0x5f8
			 0x27baa18c, /// 0x5fc
			 0x45163be3, /// 0x600
			 0xabe059ee, /// 0x604
			 0xb09a960f, /// 0x608
			 0xd0b5c8e4, /// 0x60c
			 0x93988404, /// 0x610
			 0x1caaf6cc, /// 0x614
			 0xf6fc3de7, /// 0x618
			 0xfcb11af5, /// 0x61c
			 0x70ba2eee, /// 0x620
			 0xde491556, /// 0x624
			 0x45849996, /// 0x628
			 0x0a82547c, /// 0x62c
			 0xe5436d95, /// 0x630
			 0xfaca1ea0, /// 0x634
			 0x82cc6bc1, /// 0x638
			 0x152f9c75, /// 0x63c
			 0xb129d303, /// 0x640
			 0xfcf96e71, /// 0x644
			 0x1c7e2558, /// 0x648
			 0xe71e0a3f, /// 0x64c
			 0x36e7e993, /// 0x650
			 0xa03e987b, /// 0x654
			 0xadaca1ff, /// 0x658
			 0x0abde41b, /// 0x65c
			 0xdb45f070, /// 0x660
			 0x737dd8d7, /// 0x664
			 0x562dc45b, /// 0x668
			 0x35100b77, /// 0x66c
			 0xe416617b, /// 0x670
			 0xf14178aa, /// 0x674
			 0xa21ae818, /// 0x678
			 0x113ee2ce, /// 0x67c
			 0x4be82e38, /// 0x680
			 0x6f07aeb6, /// 0x684
			 0x238c6ae5, /// 0x688
			 0x698cfb1b, /// 0x68c
			 0xcd02a294, /// 0x690
			 0x057013d3, /// 0x694
			 0x368e1266, /// 0x698
			 0x9fbf9e94, /// 0x69c
			 0x693ecdae, /// 0x6a0
			 0x50b890ec, /// 0x6a4
			 0x80081a04, /// 0x6a8
			 0x9fc429bf, /// 0x6ac
			 0xaf298edd, /// 0x6b0
			 0xf79242e9, /// 0x6b4
			 0x4e87b808, /// 0x6b8
			 0xbd37eb1d, /// 0x6bc
			 0x36af0b0a, /// 0x6c0
			 0xda894aca, /// 0x6c4
			 0x79b2099d, /// 0x6c8
			 0x520da6e6, /// 0x6cc
			 0x7d7c0346, /// 0x6d0
			 0xf568b8d5, /// 0x6d4
			 0xfaff6bda, /// 0x6d8
			 0xccb99707, /// 0x6dc
			 0x378f78f3, /// 0x6e0
			 0xc5e211f5, /// 0x6e4
			 0x93ceafde, /// 0x6e8
			 0xfc6f4717, /// 0x6ec
			 0xf3120dcf, /// 0x6f0
			 0xf164632b, /// 0x6f4
			 0x47026458, /// 0x6f8
			 0xd1a4f829, /// 0x6fc
			 0x882bc3df, /// 0x700
			 0x95fd16eb, /// 0x704
			 0x42c9e321, /// 0x708
			 0xe8cd3395, /// 0x70c
			 0xd7c76a74, /// 0x710
			 0x75a16330, /// 0x714
			 0x010d03f9, /// 0x718
			 0x3144d0e4, /// 0x71c
			 0x6c131a6e, /// 0x720
			 0xb32b5087, /// 0x724
			 0x059af790, /// 0x728
			 0x3c54b7f2, /// 0x72c
			 0xe968b735, /// 0x730
			 0x43eda3e5, /// 0x734
			 0x6d327bfd, /// 0x738
			 0x4f92f4a4, /// 0x73c
			 0x85254001, /// 0x740
			 0xb020eb0f, /// 0x744
			 0xe3423604, /// 0x748
			 0x6d528376, /// 0x74c
			 0xaaa21ac2, /// 0x750
			 0xfbfc2d3b, /// 0x754
			 0x9974bea7, /// 0x758
			 0x861493e5, /// 0x75c
			 0x858996e1, /// 0x760
			 0x84e2561b, /// 0x764
			 0x29500841, /// 0x768
			 0x861c291e, /// 0x76c
			 0x311b85ee, /// 0x770
			 0x094caf75, /// 0x774
			 0x43362486, /// 0x778
			 0x0e0971e9, /// 0x77c
			 0x1e062967, /// 0x780
			 0x54b53194, /// 0x784
			 0x3d7379b6, /// 0x788
			 0x09cef6df, /// 0x78c
			 0xb38fadc4, /// 0x790
			 0xf0867ca8, /// 0x794
			 0x75940d5c, /// 0x798
			 0xfe2c8795, /// 0x79c
			 0xb54a0834, /// 0x7a0
			 0x9571ecc8, /// 0x7a4
			 0x92ead14a, /// 0x7a8
			 0xe04d6aa6, /// 0x7ac
			 0x9e5c19cd, /// 0x7b0
			 0x9f15ff9c, /// 0x7b4
			 0xe8443266, /// 0x7b8
			 0x4251e94c, /// 0x7bc
			 0xfa23a26b, /// 0x7c0
			 0x81516531, /// 0x7c4
			 0x5ec2cc06, /// 0x7c8
			 0xad029448, /// 0x7cc
			 0xe22cebd2, /// 0x7d0
			 0xc4fa3b81, /// 0x7d4
			 0xe2e1a75b, /// 0x7d8
			 0xc7381011, /// 0x7dc
			 0x481d60a1, /// 0x7e0
			 0x93b94ac3, /// 0x7e4
			 0xf53fcf07, /// 0x7e8
			 0x924ed8d5, /// 0x7ec
			 0x3b08e63e, /// 0x7f0
			 0x1c2310fa, /// 0x7f4
			 0x969e774f, /// 0x7f8
			 0xeb433a1e, /// 0x7fc
			 0xca166aee, /// 0x800
			 0xa2bd99a8, /// 0x804
			 0x82bafb44, /// 0x808
			 0x7a453343, /// 0x80c
			 0x5f8b74f1, /// 0x810
			 0x62917a03, /// 0x814
			 0x1966fb66, /// 0x818
			 0x935d407e, /// 0x81c
			 0xeeac5252, /// 0x820
			 0xe4745f22, /// 0x824
			 0xc4c4c98b, /// 0x828
			 0xd69a77e1, /// 0x82c
			 0x76e9f5dd, /// 0x830
			 0x674eaa1e, /// 0x834
			 0x622b142d, /// 0x838
			 0x94eb530b, /// 0x83c
			 0xc34d1937, /// 0x840
			 0xe661d6b3, /// 0x844
			 0xe1d4f0c4, /// 0x848
			 0x7d5880b4, /// 0x84c
			 0xb59175a1, /// 0x850
			 0x424a49ae, /// 0x854
			 0xe8536720, /// 0x858
			 0xd38f8bb6, /// 0x85c
			 0xdd9c3a0c, /// 0x860
			 0x39c9286e, /// 0x864
			 0x447fd09b, /// 0x868
			 0xce77cac5, /// 0x86c
			 0x7c72c9c5, /// 0x870
			 0xff706209, /// 0x874
			 0xc3d06696, /// 0x878
			 0x8eba35a0, /// 0x87c
			 0x1eff8f75, /// 0x880
			 0x4358fcc4, /// 0x884
			 0x02e7251d, /// 0x888
			 0x575b42c9, /// 0x88c
			 0x28ea756c, /// 0x890
			 0xbe35d128, /// 0x894
			 0xbb8942e0, /// 0x898
			 0x2cc0b1d6, /// 0x89c
			 0x416efaf3, /// 0x8a0
			 0x64b05408, /// 0x8a4
			 0x5ab450a2, /// 0x8a8
			 0xd1d0f939, /// 0x8ac
			 0xc009693e, /// 0x8b0
			 0x0089c3cb, /// 0x8b4
			 0xb276bc06, /// 0x8b8
			 0x86fb6c9c, /// 0x8bc
			 0xb4284785, /// 0x8c0
			 0x1bc02009, /// 0x8c4
			 0x2f18756f, /// 0x8c8
			 0xed108f8e, /// 0x8cc
			 0xc4fa9055, /// 0x8d0
			 0x936d53cd, /// 0x8d4
			 0xae339bff, /// 0x8d8
			 0xbf16c28a, /// 0x8dc
			 0xb9bbe83d, /// 0x8e0
			 0xf64a4c2b, /// 0x8e4
			 0xbc007a68, /// 0x8e8
			 0x777397ac, /// 0x8ec
			 0xa3e9edc3, /// 0x8f0
			 0x8b0e0e10, /// 0x8f4
			 0x0e151cc0, /// 0x8f8
			 0x6339c32d, /// 0x8fc
			 0x24f0088c, /// 0x900
			 0xc60c32e8, /// 0x904
			 0x9a7ee8d7, /// 0x908
			 0xe48bf98a, /// 0x90c
			 0x0bf32f58, /// 0x910
			 0x066042d4, /// 0x914
			 0x34fb185a, /// 0x918
			 0x69090d21, /// 0x91c
			 0xbd311c99, /// 0x920
			 0xd1d33e56, /// 0x924
			 0x73801b3e, /// 0x928
			 0xd226093d, /// 0x92c
			 0x8702d4cd, /// 0x930
			 0x70cda961, /// 0x934
			 0x2875deed, /// 0x938
			 0xb4a1f75b, /// 0x93c
			 0x376e971a, /// 0x940
			 0xeb7b2740, /// 0x944
			 0x35f615d1, /// 0x948
			 0x78d98bf5, /// 0x94c
			 0xe597b5d5, /// 0x950
			 0xff2ce141, /// 0x954
			 0xe403b7df, /// 0x958
			 0x36662cd8, /// 0x95c
			 0xaaea7ea1, /// 0x960
			 0x76006028, /// 0x964
			 0xea915079, /// 0x968
			 0xed2f823b, /// 0x96c
			 0xfc939ef5, /// 0x970
			 0xea63b0e9, /// 0x974
			 0xcb9d2178, /// 0x978
			 0xd4db47da, /// 0x97c
			 0xd69d34c8, /// 0x980
			 0x3a584914, /// 0x984
			 0xaf0fd35e, /// 0x988
			 0x553802e6, /// 0x98c
			 0x38a27a9c, /// 0x990
			 0x9592de4e, /// 0x994
			 0x33ef138d, /// 0x998
			 0xf300c36a, /// 0x99c
			 0x326da0f4, /// 0x9a0
			 0x073ae907, /// 0x9a4
			 0xcc8141f4, /// 0x9a8
			 0x614f1ff9, /// 0x9ac
			 0xb7b8bcdb, /// 0x9b0
			 0xcd224413, /// 0x9b4
			 0x4b455e1b, /// 0x9b8
			 0x83f1b949, /// 0x9bc
			 0x3ce46a2a, /// 0x9c0
			 0xadbdb9fc, /// 0x9c4
			 0x90868da8, /// 0x9c8
			 0xefeb5802, /// 0x9cc
			 0x3baee553, /// 0x9d0
			 0x7ae8660d, /// 0x9d4
			 0x40d3a584, /// 0x9d8
			 0xb04631a6, /// 0x9dc
			 0x6e33931b, /// 0x9e0
			 0xc331d250, /// 0x9e4
			 0x70e72818, /// 0x9e8
			 0xb57319cc, /// 0x9ec
			 0xce2793ac, /// 0x9f0
			 0x3ac62eb3, /// 0x9f4
			 0xf982d367, /// 0x9f8
			 0x25ec98eb, /// 0x9fc
			 0x2b447196, /// 0xa00
			 0x8dc9f4af, /// 0xa04
			 0x5c84816f, /// 0xa08
			 0x55ada788, /// 0xa0c
			 0x8ae13cf3, /// 0xa10
			 0xbc9752fc, /// 0xa14
			 0x615bdd67, /// 0xa18
			 0x23bd93a2, /// 0xa1c
			 0xa80999ec, /// 0xa20
			 0x10827d08, /// 0xa24
			 0xcd2aee31, /// 0xa28
			 0x7bae1cff, /// 0xa2c
			 0xeaf1a051, /// 0xa30
			 0xae34af28, /// 0xa34
			 0x06d6efdf, /// 0xa38
			 0xb88a3abd, /// 0xa3c
			 0x5bd1e38c, /// 0xa40
			 0xd97fe2bd, /// 0xa44
			 0x976a7517, /// 0xa48
			 0x841e2160, /// 0xa4c
			 0x7f0344e8, /// 0xa50
			 0x4e33ce2a, /// 0xa54
			 0x013e6a7c, /// 0xa58
			 0x4f6c3109, /// 0xa5c
			 0x66caec7d, /// 0xa60
			 0x61110123, /// 0xa64
			 0x6cc14fcc, /// 0xa68
			 0xfa6ff037, /// 0xa6c
			 0xc313dfdf, /// 0xa70
			 0xff7c7709, /// 0xa74
			 0x0cbdb688, /// 0xa78
			 0x3c072d98, /// 0xa7c
			 0xa1907408, /// 0xa80
			 0x569cd652, /// 0xa84
			 0x87b93274, /// 0xa88
			 0x41cbc971, /// 0xa8c
			 0xe8ff9e2d, /// 0xa90
			 0x4f42a961, /// 0xa94
			 0x783ac0b7, /// 0xa98
			 0xbc117d08, /// 0xa9c
			 0xc39406f8, /// 0xaa0
			 0x43b36399, /// 0xaa4
			 0x0ad03316, /// 0xaa8
			 0x227188b6, /// 0xaac
			 0x8d664282, /// 0xab0
			 0x7cf702c8, /// 0xab4
			 0x48a00fcd, /// 0xab8
			 0x8ae3245b, /// 0xabc
			 0x1a0bb7ec, /// 0xac0
			 0x01a5ffcd, /// 0xac4
			 0x8d9a7942, /// 0xac8
			 0xcdd45b6f, /// 0xacc
			 0x19a42dbb, /// 0xad0
			 0x38a691ca, /// 0xad4
			 0x6fbee564, /// 0xad8
			 0x6117f011, /// 0xadc
			 0xbd48a198, /// 0xae0
			 0x9a68e9f2, /// 0xae4
			 0x5b536ff3, /// 0xae8
			 0xdf82d38c, /// 0xaec
			 0x14f42cf1, /// 0xaf0
			 0x4f90889e, /// 0xaf4
			 0xdeb453d2, /// 0xaf8
			 0xa99821ff, /// 0xafc
			 0x9760ae92, /// 0xb00
			 0x80c41441, /// 0xb04
			 0xb31e8432, /// 0xb08
			 0xb0929927, /// 0xb0c
			 0x22efd5f6, /// 0xb10
			 0x4634f854, /// 0xb14
			 0xa3ee24c2, /// 0xb18
			 0x0c5c7c04, /// 0xb1c
			 0x61138875, /// 0xb20
			 0x0d686988, /// 0xb24
			 0x0e42140a, /// 0xb28
			 0x5b1f60d0, /// 0xb2c
			 0xf10eac13, /// 0xb30
			 0xbc509dc0, /// 0xb34
			 0x4c9c8c1b, /// 0xb38
			 0x9f44fad1, /// 0xb3c
			 0xa7b9f475, /// 0xb40
			 0xafcfb1d1, /// 0xb44
			 0xe52a0aa9, /// 0xb48
			 0x4610810b, /// 0xb4c
			 0x1893ea16, /// 0xb50
			 0x6cbe96b8, /// 0xb54
			 0x6a44379f, /// 0xb58
			 0xc4ea6277, /// 0xb5c
			 0xeb04b81f, /// 0xb60
			 0x33c34ced, /// 0xb64
			 0x34a6ef8d, /// 0xb68
			 0x257a64fe, /// 0xb6c
			 0x392641c7, /// 0xb70
			 0xaf503c6f, /// 0xb74
			 0x53f7bc58, /// 0xb78
			 0x6f1e6baa, /// 0xb7c
			 0x3d7174b7, /// 0xb80
			 0xf5c0c3e9, /// 0xb84
			 0x4356c225, /// 0xb88
			 0x30943550, /// 0xb8c
			 0xce0a4795, /// 0xb90
			 0x58de2de8, /// 0xb94
			 0x9ea758f9, /// 0xb98
			 0xe4f23062, /// 0xb9c
			 0x26899197, /// 0xba0
			 0x9ece6f65, /// 0xba4
			 0xb2eea962, /// 0xba8
			 0xa734c60f, /// 0xbac
			 0xf2c644d7, /// 0xbb0
			 0x2918e864, /// 0xbb4
			 0x5aa74b94, /// 0xbb8
			 0xb9971960, /// 0xbbc
			 0xcd3ea1dd, /// 0xbc0
			 0xd536719f, /// 0xbc4
			 0xdec916d5, /// 0xbc8
			 0xd1ce150b, /// 0xbcc
			 0xfeb1432b, /// 0xbd0
			 0xfcc75752, /// 0xbd4
			 0xb1c4f632, /// 0xbd8
			 0x769736fb, /// 0xbdc
			 0x32f9f35c, /// 0xbe0
			 0x4dcc7be0, /// 0xbe4
			 0xafc83241, /// 0xbe8
			 0x1b16453e, /// 0xbec
			 0x6de01a0e, /// 0xbf0
			 0x8f99daf3, /// 0xbf4
			 0x038e9e06, /// 0xbf8
			 0xfcce96ef, /// 0xbfc
			 0x5310be07, /// 0xc00
			 0xab798d2e, /// 0xc04
			 0x8468f51e, /// 0xc08
			 0x2b39b256, /// 0xc0c
			 0x65200af3, /// 0xc10
			 0x99f72305, /// 0xc14
			 0x4ae7370d, /// 0xc18
			 0xf035e80f, /// 0xc1c
			 0x181dfa6b, /// 0xc20
			 0xf36b5ac1, /// 0xc24
			 0x7eef616a, /// 0xc28
			 0x9864390a, /// 0xc2c
			 0x07964252, /// 0xc30
			 0x84083175, /// 0xc34
			 0xce94a76d, /// 0xc38
			 0xc2511b7d, /// 0xc3c
			 0x73c27c3c, /// 0xc40
			 0x714689d0, /// 0xc44
			 0x4efe0491, /// 0xc48
			 0xd709aa84, /// 0xc4c
			 0x04024188, /// 0xc50
			 0xcbac622c, /// 0xc54
			 0xe8b61c26, /// 0xc58
			 0xa8cd60cf, /// 0xc5c
			 0x01cd1360, /// 0xc60
			 0xca458b13, /// 0xc64
			 0x00d55e1c, /// 0xc68
			 0xf4f5156c, /// 0xc6c
			 0xbb24e366, /// 0xc70
			 0xdfa8f8d2, /// 0xc74
			 0x81fae141, /// 0xc78
			 0x9cf540fa, /// 0xc7c
			 0x8dab8a94, /// 0xc80
			 0x7a66e58b, /// 0xc84
			 0xfb50d210, /// 0xc88
			 0x14127345, /// 0xc8c
			 0x9ca14807, /// 0xc90
			 0xe14764de, /// 0xc94
			 0x884d12b3, /// 0xc98
			 0x605fe80e, /// 0xc9c
			 0x0099e42d, /// 0xca0
			 0x18b934f3, /// 0xca4
			 0x92af8a84, /// 0xca8
			 0x21b8009d, /// 0xcac
			 0x22fb84d8, /// 0xcb0
			 0xca5e767d, /// 0xcb4
			 0x119b118f, /// 0xcb8
			 0xf5a7f6db, /// 0xcbc
			 0x7bee9c66, /// 0xcc0
			 0x46aa3ba9, /// 0xcc4
			 0x7179f6cc, /// 0xcc8
			 0xd9b1ea97, /// 0xccc
			 0x5c03df2a, /// 0xcd0
			 0x7485cc10, /// 0xcd4
			 0xa1930b5f, /// 0xcd8
			 0x6dcc0780, /// 0xcdc
			 0xd2f224ad, /// 0xce0
			 0x132cd397, /// 0xce4
			 0xc5fc8233, /// 0xce8
			 0xd6a118c7, /// 0xcec
			 0xafba7648, /// 0xcf0
			 0x4b1aa005, /// 0xcf4
			 0xf3aa078f, /// 0xcf8
			 0x818026a3, /// 0xcfc
			 0x1c8e9474, /// 0xd00
			 0x2f05fdc0, /// 0xd04
			 0x88168d28, /// 0xd08
			 0x191b4337, /// 0xd0c
			 0x1ad1c7be, /// 0xd10
			 0xbc68db28, /// 0xd14
			 0xa2a8d2f9, /// 0xd18
			 0xc2333ee8, /// 0xd1c
			 0x211ef359, /// 0xd20
			 0xc244d510, /// 0xd24
			 0xd3f879a0, /// 0xd28
			 0x0b620c2b, /// 0xd2c
			 0x26d50bad, /// 0xd30
			 0x868ee036, /// 0xd34
			 0x39cbc29a, /// 0xd38
			 0x2774bb2d, /// 0xd3c
			 0xcd3ccd1a, /// 0xd40
			 0xfa14cc3d, /// 0xd44
			 0x80d7199d, /// 0xd48
			 0xfcc70188, /// 0xd4c
			 0x65ee94f5, /// 0xd50
			 0x6ea44af1, /// 0xd54
			 0xb2108bbb, /// 0xd58
			 0xd5d16c1b, /// 0xd5c
			 0xbdc868f4, /// 0xd60
			 0x22dde501, /// 0xd64
			 0x20064133, /// 0xd68
			 0x29d9ec36, /// 0xd6c
			 0xd715ad80, /// 0xd70
			 0xe2f29e56, /// 0xd74
			 0xea78b8de, /// 0xd78
			 0x31cfd157, /// 0xd7c
			 0xa9af7bd7, /// 0xd80
			 0xd7f96ecc, /// 0xd84
			 0xe3c11112, /// 0xd88
			 0xa7abd097, /// 0xd8c
			 0xd5fd11ab, /// 0xd90
			 0x542b15b4, /// 0xd94
			 0x07d38233, /// 0xd98
			 0xed151e65, /// 0xd9c
			 0x118c2323, /// 0xda0
			 0xf84e252c, /// 0xda4
			 0x81d9b9ac, /// 0xda8
			 0x4c69612b, /// 0xdac
			 0x68e72d1f, /// 0xdb0
			 0x1ad25216, /// 0xdb4
			 0x8be31381, /// 0xdb8
			 0x7b2b7c31, /// 0xdbc
			 0x668e72d5, /// 0xdc0
			 0x3c9557ff, /// 0xdc4
			 0x63811dc8, /// 0xdc8
			 0xbb9be624, /// 0xdcc
			 0x4b3cc8fa, /// 0xdd0
			 0x73e07610, /// 0xdd4
			 0xee7d0500, /// 0xdd8
			 0xd340efe7, /// 0xddc
			 0x54b3a480, /// 0xde0
			 0x3df98ae5, /// 0xde4
			 0x155d4113, /// 0xde8
			 0x5a814a25, /// 0xdec
			 0xb58eccad, /// 0xdf0
			 0x200c9282, /// 0xdf4
			 0xd68f4f18, /// 0xdf8
			 0x7e04df56, /// 0xdfc
			 0xc7b81e9a, /// 0xe00
			 0x667c31a7, /// 0xe04
			 0x8bda18e4, /// 0xe08
			 0xd588d5a6, /// 0xe0c
			 0x1e66ac1d, /// 0xe10
			 0x4605521b, /// 0xe14
			 0x4349cd9c, /// 0xe18
			 0x561c0546, /// 0xe1c
			 0xc7293404, /// 0xe20
			 0xea62ad75, /// 0xe24
			 0xbad817f2, /// 0xe28
			 0x0e998ef8, /// 0xe2c
			 0xa6dc05dd, /// 0xe30
			 0x12f34c4f, /// 0xe34
			 0x503ca38f, /// 0xe38
			 0x1a2b181a, /// 0xe3c
			 0x67aab406, /// 0xe40
			 0x8155f6b2, /// 0xe44
			 0x22208d69, /// 0xe48
			 0x273472b2, /// 0xe4c
			 0x7259bf5a, /// 0xe50
			 0x52214c32, /// 0xe54
			 0x972b3359, /// 0xe58
			 0xcaa71183, /// 0xe5c
			 0x3a524f0c, /// 0xe60
			 0x9970a5f7, /// 0xe64
			 0x38766ce7, /// 0xe68
			 0x60218e00, /// 0xe6c
			 0xbeb2558f, /// 0xe70
			 0x49855f49, /// 0xe74
			 0xfaa4cd15, /// 0xe78
			 0x89cf4269, /// 0xe7c
			 0x6006f70a, /// 0xe80
			 0x5572e605, /// 0xe84
			 0x1ad95ee6, /// 0xe88
			 0xf100bcb6, /// 0xe8c
			 0x4d5a1c68, /// 0xe90
			 0x25ca8708, /// 0xe94
			 0x7506c3ae, /// 0xe98
			 0xa07d7635, /// 0xe9c
			 0x2d7a3d7a, /// 0xea0
			 0x2f0f7eff, /// 0xea4
			 0x672c943d, /// 0xea8
			 0xd4986c7d, /// 0xeac
			 0xff115f6d, /// 0xeb0
			 0x7ee2a409, /// 0xeb4
			 0x97f5f668, /// 0xeb8
			 0x27603688, /// 0xebc
			 0x5eaf926d, /// 0xec0
			 0xdda1c6fc, /// 0xec4
			 0xddee50eb, /// 0xec8
			 0xe52ed907, /// 0xecc
			 0x85213c99, /// 0xed0
			 0x46860855, /// 0xed4
			 0xd931e703, /// 0xed8
			 0x40b83d68, /// 0xedc
			 0xe25b5522, /// 0xee0
			 0xc1c547e1, /// 0xee4
			 0x5a21a9d0, /// 0xee8
			 0xb3e839d9, /// 0xeec
			 0x12232873, /// 0xef0
			 0xb1c51e68, /// 0xef4
			 0xe5c72bd1, /// 0xef8
			 0x8f9bcdd9, /// 0xefc
			 0xc7196523, /// 0xf00
			 0x5616d57d, /// 0xf04
			 0x8d38b4c2, /// 0xf08
			 0xf6336d58, /// 0xf0c
			 0xa1504313, /// 0xf10
			 0xaeaa7b6a, /// 0xf14
			 0x3c96d062, /// 0xf18
			 0x2f1a1f39, /// 0xf1c
			 0x04767c36, /// 0xf20
			 0x44c64ec3, /// 0xf24
			 0x903f5f7e, /// 0xf28
			 0x6817d0ed, /// 0xf2c
			 0xbb9428fb, /// 0xf30
			 0x7b19d460, /// 0xf34
			 0xce719923, /// 0xf38
			 0xac9b3762, /// 0xf3c
			 0x713222b4, /// 0xf40
			 0xf26952c9, /// 0xf44
			 0x9819c766, /// 0xf48
			 0x37d050d4, /// 0xf4c
			 0x830ff458, /// 0xf50
			 0xa51b5562, /// 0xf54
			 0x07b3db84, /// 0xf58
			 0x65153803, /// 0xf5c
			 0x6708e129, /// 0xf60
			 0x695d42bf, /// 0xf64
			 0x41c30e50, /// 0xf68
			 0xc94dc6fb, /// 0xf6c
			 0x9aff5e2b, /// 0xf70
			 0x22773947, /// 0xf74
			 0x114d439c, /// 0xf78
			 0x659e4ee8, /// 0xf7c
			 0xcf489e7f, /// 0xf80
			 0xc5b549fc, /// 0xf84
			 0xe9864397, /// 0xf88
			 0x232c989f, /// 0xf8c
			 0xb8741907, /// 0xf90
			 0x1d257121, /// 0xf94
			 0xec5e3697, /// 0xf98
			 0x2073e628, /// 0xf9c
			 0x1ba6157e, /// 0xfa0
			 0x519e524e, /// 0xfa4
			 0xb637e635, /// 0xfa8
			 0x3cdba1b0, /// 0xfac
			 0x9fb9d88b, /// 0xfb0
			 0x43ed3cab, /// 0xfb4
			 0xea9eddca, /// 0xfb8
			 0x2f67fd6b, /// 0xfbc
			 0xc799f46b, /// 0xfc0
			 0xef09082b, /// 0xfc4
			 0xbe4ddfc7, /// 0xfc8
			 0xf8e0b4c4, /// 0xfcc
			 0x53d24fea, /// 0xfd0
			 0x7aaf65d5, /// 0xfd4
			 0xd0179881, /// 0xfd8
			 0xd88c9890, /// 0xfdc
			 0xb1d4c9e5, /// 0xfe0
			 0xf415805d, /// 0xfe4
			 0x81be032f, /// 0xfe8
			 0x6d81a1f8, /// 0xfec
			 0xf4ec141e, /// 0xff0
			 0xc3658341, /// 0xff4
			 0xa2b759c5, /// 0xff8
			 0x6d59b592 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x35735de8, /// 0x0
			 0x9d910971, /// 0x4
			 0xfb93f4a5, /// 0x8
			 0x5d058476, /// 0xc
			 0x5a48479e, /// 0x10
			 0xcc919326, /// 0x14
			 0x79cad2e6, /// 0x18
			 0x832ce8d9, /// 0x1c
			 0x679c985c, /// 0x20
			 0x1f4c6a0e, /// 0x24
			 0xd7ceb567, /// 0x28
			 0xb0102b9f, /// 0x2c
			 0x74c1b4ca, /// 0x30
			 0xe6f2042e, /// 0x34
			 0x30e7bd80, /// 0x38
			 0x69cea2fa, /// 0x3c
			 0x3b60f520, /// 0x40
			 0xa55f5360, /// 0x44
			 0x697b8632, /// 0x48
			 0x5aed19ed, /// 0x4c
			 0x8b57e3f9, /// 0x50
			 0x26d813fc, /// 0x54
			 0x797d0a76, /// 0x58
			 0x0ab6478d, /// 0x5c
			 0x2c4c9e20, /// 0x60
			 0x4891178d, /// 0x64
			 0x3645728c, /// 0x68
			 0x0db6ceea, /// 0x6c
			 0xf732f558, /// 0x70
			 0x0e19e2a7, /// 0x74
			 0x240200d6, /// 0x78
			 0x90b8fec1, /// 0x7c
			 0x68149987, /// 0x80
			 0xe9db6aad, /// 0x84
			 0xba83343b, /// 0x88
			 0x597fd812, /// 0x8c
			 0x95e964ad, /// 0x90
			 0xaf0ff154, /// 0x94
			 0xfb7fb89d, /// 0x98
			 0xc5bc914a, /// 0x9c
			 0x7de196e4, /// 0xa0
			 0x35f037b7, /// 0xa4
			 0x70153d9e, /// 0xa8
			 0xc368e1e3, /// 0xac
			 0xcee9ddf7, /// 0xb0
			 0x108a0ca1, /// 0xb4
			 0xa734dd9d, /// 0xb8
			 0x9e54fd23, /// 0xbc
			 0x15ff90f0, /// 0xc0
			 0x15df9529, /// 0xc4
			 0xe4e20783, /// 0xc8
			 0x048f8b94, /// 0xcc
			 0x30d1f204, /// 0xd0
			 0xf05a2ebe, /// 0xd4
			 0x982bea31, /// 0xd8
			 0x6f6c3371, /// 0xdc
			 0x62d2051a, /// 0xe0
			 0xf2b18502, /// 0xe4
			 0x0e728744, /// 0xe8
			 0x6ba843f8, /// 0xec
			 0xb748f28a, /// 0xf0
			 0x7830cef4, /// 0xf4
			 0x1938be2d, /// 0xf8
			 0x16062c09, /// 0xfc
			 0xf8bd5415, /// 0x100
			 0xdd9a4e91, /// 0x104
			 0xdce51f98, /// 0x108
			 0x930f9b5b, /// 0x10c
			 0x29137ca9, /// 0x110
			 0xa439f078, /// 0x114
			 0xf672a89b, /// 0x118
			 0x938d092b, /// 0x11c
			 0x37dcdc83, /// 0x120
			 0xeeaaceec, /// 0x124
			 0xf3fcaa47, /// 0x128
			 0xe1ea67be, /// 0x12c
			 0x9e20d683, /// 0x130
			 0x77b2af48, /// 0x134
			 0xd3bf8963, /// 0x138
			 0x10e594a6, /// 0x13c
			 0xc366f528, /// 0x140
			 0x016b5ac4, /// 0x144
			 0xbbc05b71, /// 0x148
			 0xa636ae46, /// 0x14c
			 0x19d63e7f, /// 0x150
			 0x511a4ae0, /// 0x154
			 0xf3e61235, /// 0x158
			 0x1b501f23, /// 0x15c
			 0x80c1176c, /// 0x160
			 0x468232f5, /// 0x164
			 0x7535f829, /// 0x168
			 0xa4a48951, /// 0x16c
			 0xbd2988d3, /// 0x170
			 0x00d7505d, /// 0x174
			 0xcfcfc54b, /// 0x178
			 0x9818b38d, /// 0x17c
			 0x0f34bf57, /// 0x180
			 0xa19dded8, /// 0x184
			 0xe9a51982, /// 0x188
			 0x1f0bc911, /// 0x18c
			 0xcc84f3d7, /// 0x190
			 0x9c7fe694, /// 0x194
			 0x28900831, /// 0x198
			 0x1d979f61, /// 0x19c
			 0x0572607f, /// 0x1a0
			 0xb3978e01, /// 0x1a4
			 0xd050b310, /// 0x1a8
			 0x176ffb27, /// 0x1ac
			 0xcb901b9e, /// 0x1b0
			 0xd3748a4b, /// 0x1b4
			 0x69305b17, /// 0x1b8
			 0x6adb5412, /// 0x1bc
			 0x5f72e484, /// 0x1c0
			 0x71b9e030, /// 0x1c4
			 0x3e8df728, /// 0x1c8
			 0x94a56107, /// 0x1cc
			 0x5be27f22, /// 0x1d0
			 0x57d954f1, /// 0x1d4
			 0xf1c919cb, /// 0x1d8
			 0x3b712959, /// 0x1dc
			 0x644c6be0, /// 0x1e0
			 0x52bc4829, /// 0x1e4
			 0x918ce424, /// 0x1e8
			 0x00c9772f, /// 0x1ec
			 0xa851b24b, /// 0x1f0
			 0x1618a5d0, /// 0x1f4
			 0xd8ec7fad, /// 0x1f8
			 0x34baa434, /// 0x1fc
			 0x4efa551e, /// 0x200
			 0xa8d1d3bd, /// 0x204
			 0xf24902ac, /// 0x208
			 0x7c6610a8, /// 0x20c
			 0x2c560904, /// 0x210
			 0x264545c0, /// 0x214
			 0x1671684c, /// 0x218
			 0x6cdc8568, /// 0x21c
			 0x8081c366, /// 0x220
			 0x292209bd, /// 0x224
			 0x32ba15bd, /// 0x228
			 0x58add799, /// 0x22c
			 0xa52c8def, /// 0x230
			 0x0c817393, /// 0x234
			 0x1a970883, /// 0x238
			 0x42caaac0, /// 0x23c
			 0x19629414, /// 0x240
			 0x90102d41, /// 0x244
			 0x605e03f0, /// 0x248
			 0x23d494d3, /// 0x24c
			 0x294810e0, /// 0x250
			 0x030df46b, /// 0x254
			 0xd93a9a56, /// 0x258
			 0xa82e3006, /// 0x25c
			 0x444d7d68, /// 0x260
			 0xb79bf94d, /// 0x264
			 0x69bcc64c, /// 0x268
			 0x1a9b59a9, /// 0x26c
			 0xbf0c3f13, /// 0x270
			 0x8df12750, /// 0x274
			 0x3ab1dc2a, /// 0x278
			 0xec6b6661, /// 0x27c
			 0x68c6d1f7, /// 0x280
			 0xb1320470, /// 0x284
			 0x83e36dd6, /// 0x288
			 0xb4e4aadb, /// 0x28c
			 0xa73c7905, /// 0x290
			 0x89834c38, /// 0x294
			 0xff0b7933, /// 0x298
			 0x919a315f, /// 0x29c
			 0x898ed35f, /// 0x2a0
			 0x82fcffc1, /// 0x2a4
			 0x32dba228, /// 0x2a8
			 0xe8379fae, /// 0x2ac
			 0xe937ff57, /// 0x2b0
			 0xf27463ae, /// 0x2b4
			 0x554d7bc9, /// 0x2b8
			 0x424e72df, /// 0x2bc
			 0x1147dcf0, /// 0x2c0
			 0x713eb65e, /// 0x2c4
			 0xd1bac342, /// 0x2c8
			 0x7ab0b50e, /// 0x2cc
			 0x40d37512, /// 0x2d0
			 0x0fefed31, /// 0x2d4
			 0xd6db19fe, /// 0x2d8
			 0xd16dc475, /// 0x2dc
			 0x27d4dca9, /// 0x2e0
			 0x8521241b, /// 0x2e4
			 0x85cc159e, /// 0x2e8
			 0x958a0d6a, /// 0x2ec
			 0x7342a729, /// 0x2f0
			 0xbae907ad, /// 0x2f4
			 0x5b158103, /// 0x2f8
			 0xa8905173, /// 0x2fc
			 0x20bf3424, /// 0x300
			 0xc537ffa1, /// 0x304
			 0x2cf2a81b, /// 0x308
			 0x39586aa1, /// 0x30c
			 0x92bd3108, /// 0x310
			 0xe882ad67, /// 0x314
			 0x40c7bacb, /// 0x318
			 0x0ea35b8c, /// 0x31c
			 0xa6d028db, /// 0x320
			 0xb3a5b1a7, /// 0x324
			 0xb2c2e297, /// 0x328
			 0xfffc473e, /// 0x32c
			 0xc93978c6, /// 0x330
			 0xc00b85f6, /// 0x334
			 0xd60c6634, /// 0x338
			 0xa46aee22, /// 0x33c
			 0x7b965c82, /// 0x340
			 0x91e19430, /// 0x344
			 0xa8162205, /// 0x348
			 0xd797985e, /// 0x34c
			 0x695c0562, /// 0x350
			 0x83e1b339, /// 0x354
			 0x4ad0ed16, /// 0x358
			 0x2f96f0cd, /// 0x35c
			 0x4d545ac3, /// 0x360
			 0xcd73bb8b, /// 0x364
			 0x73c18d55, /// 0x368
			 0xcc9a91dc, /// 0x36c
			 0xdd612124, /// 0x370
			 0xda21d123, /// 0x374
			 0xc9bf8dc4, /// 0x378
			 0xbaee91fc, /// 0x37c
			 0xfc61380c, /// 0x380
			 0x1f720b5a, /// 0x384
			 0x82f360a5, /// 0x388
			 0xdc58de47, /// 0x38c
			 0x62d27132, /// 0x390
			 0xf203ba25, /// 0x394
			 0x032d0daf, /// 0x398
			 0x8752e132, /// 0x39c
			 0x56e079ff, /// 0x3a0
			 0x87db8fd9, /// 0x3a4
			 0x855838cd, /// 0x3a8
			 0xf60a0b77, /// 0x3ac
			 0x893f8c09, /// 0x3b0
			 0xc9449900, /// 0x3b4
			 0x1166c04d, /// 0x3b8
			 0x8a41785a, /// 0x3bc
			 0x3d4bbd4d, /// 0x3c0
			 0xd48c41ac, /// 0x3c4
			 0xa201b11b, /// 0x3c8
			 0x403afd1e, /// 0x3cc
			 0x706144db, /// 0x3d0
			 0x73432d2b, /// 0x3d4
			 0xae6220b8, /// 0x3d8
			 0xa88df47f, /// 0x3dc
			 0x99c9f607, /// 0x3e0
			 0xc2ed1561, /// 0x3e4
			 0xb1a2bea5, /// 0x3e8
			 0x70237166, /// 0x3ec
			 0x13fa7f23, /// 0x3f0
			 0xe3116697, /// 0x3f4
			 0x5bf7de89, /// 0x3f8
			 0x424d1447, /// 0x3fc
			 0x12176796, /// 0x400
			 0x930a59d8, /// 0x404
			 0xa2df0319, /// 0x408
			 0x914ddf81, /// 0x40c
			 0x8b53594e, /// 0x410
			 0x004cc20c, /// 0x414
			 0x09ae2fa0, /// 0x418
			 0x1aa64b60, /// 0x41c
			 0x9445f948, /// 0x420
			 0xa37ef60a, /// 0x424
			 0x8f2adfce, /// 0x428
			 0xc08a4c78, /// 0x42c
			 0x9ac737c7, /// 0x430
			 0xdc378178, /// 0x434
			 0x064a4197, /// 0x438
			 0x67dbf6fd, /// 0x43c
			 0x8195fbea, /// 0x440
			 0x8c6d24bd, /// 0x444
			 0xabcfd12d, /// 0x448
			 0x607479c0, /// 0x44c
			 0x18590bc6, /// 0x450
			 0x124dc6af, /// 0x454
			 0x660ac916, /// 0x458
			 0x8c642684, /// 0x45c
			 0x59d2d9b5, /// 0x460
			 0xb572594f, /// 0x464
			 0x2bda6b1c, /// 0x468
			 0x0ba86c36, /// 0x46c
			 0x256b5c16, /// 0x470
			 0x620eec3d, /// 0x474
			 0x5bfd8a53, /// 0x478
			 0x5b3a36bb, /// 0x47c
			 0x63a7a3b0, /// 0x480
			 0x2f20e418, /// 0x484
			 0xd3e4343e, /// 0x488
			 0x6a85c607, /// 0x48c
			 0x552c8b66, /// 0x490
			 0xa6f3b61b, /// 0x494
			 0x7aec6efd, /// 0x498
			 0xb14d79a7, /// 0x49c
			 0x57ffbc17, /// 0x4a0
			 0x2f01bfe8, /// 0x4a4
			 0x2f22280b, /// 0x4a8
			 0xce14143e, /// 0x4ac
			 0x916cdef9, /// 0x4b0
			 0x3754fb7d, /// 0x4b4
			 0x8ae703a1, /// 0x4b8
			 0x8aa798a5, /// 0x4bc
			 0x9ff4c4c0, /// 0x4c0
			 0xce001baf, /// 0x4c4
			 0x0d1a9ec4, /// 0x4c8
			 0x5affaf6d, /// 0x4cc
			 0x4dc45365, /// 0x4d0
			 0x72c71505, /// 0x4d4
			 0xb9d2eb71, /// 0x4d8
			 0x674726b9, /// 0x4dc
			 0x3a46fd4d, /// 0x4e0
			 0x16833ec6, /// 0x4e4
			 0x0455e9ce, /// 0x4e8
			 0x01c5dd9d, /// 0x4ec
			 0xdafb30d8, /// 0x4f0
			 0xcbef8ec2, /// 0x4f4
			 0x74198ab2, /// 0x4f8
			 0x455e227b, /// 0x4fc
			 0x3deab26c, /// 0x500
			 0xc5ef02d9, /// 0x504
			 0x4d74a831, /// 0x508
			 0x2587390a, /// 0x50c
			 0x7e6fe7ca, /// 0x510
			 0x3caaaaf1, /// 0x514
			 0xc6a8c340, /// 0x518
			 0xa6cf48fd, /// 0x51c
			 0x0ec211cf, /// 0x520
			 0x4f9bb95c, /// 0x524
			 0xb6b49c7d, /// 0x528
			 0xec9a26b3, /// 0x52c
			 0x94e607f3, /// 0x530
			 0xca85e0ff, /// 0x534
			 0x31f174fa, /// 0x538
			 0xa93f053b, /// 0x53c
			 0xaec397cc, /// 0x540
			 0x880376d4, /// 0x544
			 0x48972a7a, /// 0x548
			 0x771e5a3f, /// 0x54c
			 0x3529e690, /// 0x550
			 0xeb8763d4, /// 0x554
			 0x6484b980, /// 0x558
			 0x949b9d99, /// 0x55c
			 0x045bc8ba, /// 0x560
			 0xd19c1b54, /// 0x564
			 0x2c33b886, /// 0x568
			 0x15a490ce, /// 0x56c
			 0x9443ca8e, /// 0x570
			 0xa573def2, /// 0x574
			 0xcab66cd7, /// 0x578
			 0x744a1f3d, /// 0x57c
			 0x889194c7, /// 0x580
			 0xe81ab6ac, /// 0x584
			 0x3eacbb6a, /// 0x588
			 0xbece5db2, /// 0x58c
			 0x6489197d, /// 0x590
			 0xa0ecb728, /// 0x594
			 0x62487761, /// 0x598
			 0x7ff21e1a, /// 0x59c
			 0x3c2c88f3, /// 0x5a0
			 0xbbf5698b, /// 0x5a4
			 0xd1ac7488, /// 0x5a8
			 0x465125c0, /// 0x5ac
			 0xb687fc20, /// 0x5b0
			 0xed237d75, /// 0x5b4
			 0xb9099e93, /// 0x5b8
			 0x1347993f, /// 0x5bc
			 0xbd998b08, /// 0x5c0
			 0x7d8b0fbc, /// 0x5c4
			 0xc47fa3b6, /// 0x5c8
			 0x030cab3c, /// 0x5cc
			 0x0b23a672, /// 0x5d0
			 0x7f3c3b16, /// 0x5d4
			 0x6fe88c71, /// 0x5d8
			 0xe797db27, /// 0x5dc
			 0x204a2ce8, /// 0x5e0
			 0xfe7cc777, /// 0x5e4
			 0xa9c47091, /// 0x5e8
			 0x64c36fe9, /// 0x5ec
			 0x5b0b4548, /// 0x5f0
			 0x706e0168, /// 0x5f4
			 0x1e919f1e, /// 0x5f8
			 0x35265e44, /// 0x5fc
			 0xd8d3f036, /// 0x600
			 0xf3799e34, /// 0x604
			 0x74a6fa7f, /// 0x608
			 0x10a9ab9c, /// 0x60c
			 0xf79e5528, /// 0x610
			 0xde61e4c9, /// 0x614
			 0x684e3cb1, /// 0x618
			 0x9e3a41de, /// 0x61c
			 0x826d7efe, /// 0x620
			 0x555afdfc, /// 0x624
			 0xa98d3cd4, /// 0x628
			 0x8174a312, /// 0x62c
			 0xd5fbe13d, /// 0x630
			 0x4007778a, /// 0x634
			 0xffe06fc8, /// 0x638
			 0xe13defef, /// 0x63c
			 0x9d1404e7, /// 0x640
			 0x8527bf81, /// 0x644
			 0xd66d9093, /// 0x648
			 0xb8b47530, /// 0x64c
			 0x864efd8c, /// 0x650
			 0x939c6548, /// 0x654
			 0xd7d90207, /// 0x658
			 0xa359140b, /// 0x65c
			 0x7aa3e0b5, /// 0x660
			 0xd3b6ec4b, /// 0x664
			 0x047ad0cd, /// 0x668
			 0x2a15c6b9, /// 0x66c
			 0x2dd4234d, /// 0x670
			 0xf6e00c0a, /// 0x674
			 0x70abb6f6, /// 0x678
			 0xc07b2975, /// 0x67c
			 0x7a823796, /// 0x680
			 0x733c4675, /// 0x684
			 0xac085a92, /// 0x688
			 0xa2a28729, /// 0x68c
			 0xb5e5dcdc, /// 0x690
			 0xf81b48f2, /// 0x694
			 0x4bada4c1, /// 0x698
			 0x3512366c, /// 0x69c
			 0xbeab0c80, /// 0x6a0
			 0x03e212a1, /// 0x6a4
			 0x0b0267b3, /// 0x6a8
			 0x748a956e, /// 0x6ac
			 0x2f9f0528, /// 0x6b0
			 0x3b62b435, /// 0x6b4
			 0x5f7d6c29, /// 0x6b8
			 0xd9bc6e94, /// 0x6bc
			 0x35d7eadf, /// 0x6c0
			 0xa866db77, /// 0x6c4
			 0xf9d75844, /// 0x6c8
			 0x09ef64bd, /// 0x6cc
			 0x94ac23f6, /// 0x6d0
			 0x4165dd54, /// 0x6d4
			 0x20c9439b, /// 0x6d8
			 0x393816d0, /// 0x6dc
			 0x33150e71, /// 0x6e0
			 0xd8175090, /// 0x6e4
			 0x2fc55531, /// 0x6e8
			 0x6241718e, /// 0x6ec
			 0x61df23d9, /// 0x6f0
			 0x65754b0a, /// 0x6f4
			 0xfdff643f, /// 0x6f8
			 0xf51d28e1, /// 0x6fc
			 0xf70428ec, /// 0x700
			 0x772c066c, /// 0x704
			 0x6f6007c4, /// 0x708
			 0x23147082, /// 0x70c
			 0x1a758c0d, /// 0x710
			 0x2b368bcc, /// 0x714
			 0xbb5a1228, /// 0x718
			 0x31cbc17d, /// 0x71c
			 0xfbe10f4e, /// 0x720
			 0x0951868e, /// 0x724
			 0x701faaab, /// 0x728
			 0x97291e67, /// 0x72c
			 0x4e2c028d, /// 0x730
			 0x33aff746, /// 0x734
			 0xde9d6c5f, /// 0x738
			 0x1b4c446c, /// 0x73c
			 0xbfe4ab85, /// 0x740
			 0xa1055732, /// 0x744
			 0x7ed747e3, /// 0x748
			 0x3a585524, /// 0x74c
			 0x73c74524, /// 0x750
			 0x65ffd402, /// 0x754
			 0x80033649, /// 0x758
			 0xa2f3d48f, /// 0x75c
			 0x3276c14e, /// 0x760
			 0xe2f5b1a7, /// 0x764
			 0xa2e60c6c, /// 0x768
			 0x4d577c97, /// 0x76c
			 0xe24a027f, /// 0x770
			 0x96199569, /// 0x774
			 0x7227faf9, /// 0x778
			 0x6db60732, /// 0x77c
			 0x932fb1ec, /// 0x780
			 0xe105917c, /// 0x784
			 0x9c0c3f09, /// 0x788
			 0x145c9c24, /// 0x78c
			 0x55e0d554, /// 0x790
			 0x4b88e650, /// 0x794
			 0x75fea2f7, /// 0x798
			 0x31084a0e, /// 0x79c
			 0x4d11e0d0, /// 0x7a0
			 0x9af6bb43, /// 0x7a4
			 0xfe11d1c7, /// 0x7a8
			 0x1a946050, /// 0x7ac
			 0xad1a14e9, /// 0x7b0
			 0x596a8565, /// 0x7b4
			 0x86f63c88, /// 0x7b8
			 0x2a379e9c, /// 0x7bc
			 0x9573ac1c, /// 0x7c0
			 0xe213ec87, /// 0x7c4
			 0xa83c5ca6, /// 0x7c8
			 0xb5ee8c8c, /// 0x7cc
			 0x9cb1e397, /// 0x7d0
			 0xb4797beb, /// 0x7d4
			 0xfb7c8df7, /// 0x7d8
			 0x68974bc2, /// 0x7dc
			 0x717cb969, /// 0x7e0
			 0xcdc5b3b0, /// 0x7e4
			 0xefe1e1c4, /// 0x7e8
			 0x7105fbdf, /// 0x7ec
			 0x13db262d, /// 0x7f0
			 0xbaa68bf9, /// 0x7f4
			 0xa7ccd19e, /// 0x7f8
			 0xb95c2952, /// 0x7fc
			 0x0a3fd9cd, /// 0x800
			 0xe2892357, /// 0x804
			 0x1010565e, /// 0x808
			 0x70e2bedf, /// 0x80c
			 0x0d685479, /// 0x810
			 0xf11f22cc, /// 0x814
			 0xfcd3b152, /// 0x818
			 0xc807fc86, /// 0x81c
			 0xfbcf9b03, /// 0x820
			 0xdb400d00, /// 0x824
			 0x7cd581d3, /// 0x828
			 0x426500a1, /// 0x82c
			 0x91f12103, /// 0x830
			 0x31d6d1a8, /// 0x834
			 0x110d95ec, /// 0x838
			 0x60c62f98, /// 0x83c
			 0x3e00502b, /// 0x840
			 0x2c86a94e, /// 0x844
			 0x72d2635c, /// 0x848
			 0x9842c3d5, /// 0x84c
			 0xd593f91c, /// 0x850
			 0x45ab6058, /// 0x854
			 0x73976633, /// 0x858
			 0x66b7356c, /// 0x85c
			 0x91d113bf, /// 0x860
			 0x3d56155c, /// 0x864
			 0x2e5ce479, /// 0x868
			 0xfd78b8f9, /// 0x86c
			 0xf7064b4c, /// 0x870
			 0x35e54d82, /// 0x874
			 0x5cd25188, /// 0x878
			 0x2fc6383b, /// 0x87c
			 0x3e830eca, /// 0x880
			 0xe721a28f, /// 0x884
			 0x66071eea, /// 0x888
			 0xe4846e58, /// 0x88c
			 0xc88ddeef, /// 0x890
			 0x971a9b10, /// 0x894
			 0x6bc4bcd2, /// 0x898
			 0x3aae10e8, /// 0x89c
			 0xc28666f2, /// 0x8a0
			 0xce7385eb, /// 0x8a4
			 0x1e64f772, /// 0x8a8
			 0x2223d6d5, /// 0x8ac
			 0x11c1bc9a, /// 0x8b0
			 0x0c39798d, /// 0x8b4
			 0xb5df4962, /// 0x8b8
			 0x6e49609f, /// 0x8bc
			 0xc31ccb85, /// 0x8c0
			 0x39ef1d24, /// 0x8c4
			 0x1d80138a, /// 0x8c8
			 0xa6da57e6, /// 0x8cc
			 0xc6ca432e, /// 0x8d0
			 0x187e0532, /// 0x8d4
			 0xa6abf803, /// 0x8d8
			 0xde07dfd1, /// 0x8dc
			 0x40538be8, /// 0x8e0
			 0x2ba1d52e, /// 0x8e4
			 0x32d45e51, /// 0x8e8
			 0x49ebff8a, /// 0x8ec
			 0x01f92a39, /// 0x8f0
			 0xd2f9e34e, /// 0x8f4
			 0xa2d1acf3, /// 0x8f8
			 0x424dfb42, /// 0x8fc
			 0xff0791a7, /// 0x900
			 0xcf8ebc7f, /// 0x904
			 0x6bf0ff0e, /// 0x908
			 0x365ecca0, /// 0x90c
			 0x278a1a79, /// 0x910
			 0xfb9e3fd0, /// 0x914
			 0xfc58ae6a, /// 0x918
			 0x217100e9, /// 0x91c
			 0x04afb0d1, /// 0x920
			 0x1dbb794a, /// 0x924
			 0x2ea62025, /// 0x928
			 0x9d50fac0, /// 0x92c
			 0x5fc09843, /// 0x930
			 0x07822de8, /// 0x934
			 0x4856e04f, /// 0x938
			 0x5c2b6677, /// 0x93c
			 0x22faf3ec, /// 0x940
			 0xcecb1566, /// 0x944
			 0x56bfa3ed, /// 0x948
			 0x5d808854, /// 0x94c
			 0x8a8b21d9, /// 0x950
			 0x565dac07, /// 0x954
			 0x68136825, /// 0x958
			 0x5afa148b, /// 0x95c
			 0x2ece36b9, /// 0x960
			 0x58480fdf, /// 0x964
			 0x7236530a, /// 0x968
			 0x6addcc2b, /// 0x96c
			 0xe9f64e60, /// 0x970
			 0x45de4dc8, /// 0x974
			 0xe0722993, /// 0x978
			 0x1869f0a9, /// 0x97c
			 0x612beb06, /// 0x980
			 0x1cc7c476, /// 0x984
			 0x7d504f7e, /// 0x988
			 0x49a9b710, /// 0x98c
			 0x18e99bf3, /// 0x990
			 0xfcc23aaa, /// 0x994
			 0x69927764, /// 0x998
			 0xe7bcb8a7, /// 0x99c
			 0x47d4b390, /// 0x9a0
			 0x78ab419e, /// 0x9a4
			 0x13e2e55c, /// 0x9a8
			 0xa1080dc8, /// 0x9ac
			 0xf6c44b89, /// 0x9b0
			 0x02487ab0, /// 0x9b4
			 0xa1bab585, /// 0x9b8
			 0xc8498ec8, /// 0x9bc
			 0x60db4d6c, /// 0x9c0
			 0x0eeb75a4, /// 0x9c4
			 0x4a47e982, /// 0x9c8
			 0xc087b6e2, /// 0x9cc
			 0x9bcd5ac8, /// 0x9d0
			 0xf50ae19f, /// 0x9d4
			 0x23b15506, /// 0x9d8
			 0x9d372a0e, /// 0x9dc
			 0x7c5439b1, /// 0x9e0
			 0x1ba3048d, /// 0x9e4
			 0xd3a05d22, /// 0x9e8
			 0xc4f8239c, /// 0x9ec
			 0x477b6421, /// 0x9f0
			 0xd0cca04d, /// 0x9f4
			 0xf7c41c88, /// 0x9f8
			 0x7d9118e6, /// 0x9fc
			 0x6111ec8d, /// 0xa00
			 0x28c62e68, /// 0xa04
			 0xcf61b33d, /// 0xa08
			 0x0d67a8d6, /// 0xa0c
			 0xe6035b92, /// 0xa10
			 0x7398a563, /// 0xa14
			 0x0e8a6207, /// 0xa18
			 0x796cf06c, /// 0xa1c
			 0x769b9649, /// 0xa20
			 0x5532ecbe, /// 0xa24
			 0x978fe403, /// 0xa28
			 0xcb26b1eb, /// 0xa2c
			 0x52a0f2ee, /// 0xa30
			 0x10d6477e, /// 0xa34
			 0x0e253482, /// 0xa38
			 0xc27ed84d, /// 0xa3c
			 0x9898f599, /// 0xa40
			 0x8ea9b86d, /// 0xa44
			 0x99445388, /// 0xa48
			 0xa3b18d17, /// 0xa4c
			 0xad58bb82, /// 0xa50
			 0xa38e3cde, /// 0xa54
			 0xdf8a1eec, /// 0xa58
			 0xd8125c88, /// 0xa5c
			 0x08b1ac5e, /// 0xa60
			 0x2b6fdaca, /// 0xa64
			 0x5b54db95, /// 0xa68
			 0x34d787a6, /// 0xa6c
			 0x0370140c, /// 0xa70
			 0xeb6377a9, /// 0xa74
			 0xdcee1dc7, /// 0xa78
			 0x295c2a74, /// 0xa7c
			 0xecc1c7ad, /// 0xa80
			 0x8f9dd7c2, /// 0xa84
			 0xc281439d, /// 0xa88
			 0xab3cbb2a, /// 0xa8c
			 0x49b9f9b8, /// 0xa90
			 0xbf33e538, /// 0xa94
			 0x5820fb2a, /// 0xa98
			 0x5c31c79a, /// 0xa9c
			 0x45949b07, /// 0xaa0
			 0x609ac01c, /// 0xaa4
			 0x59e41e46, /// 0xaa8
			 0x88560ceb, /// 0xaac
			 0x2fc17f0f, /// 0xab0
			 0xfffc645c, /// 0xab4
			 0x51e8cb77, /// 0xab8
			 0x08a9333f, /// 0xabc
			 0xc61f1f58, /// 0xac0
			 0xf164fe03, /// 0xac4
			 0x776ac32b, /// 0xac8
			 0x2a94bd19, /// 0xacc
			 0x38b8ed13, /// 0xad0
			 0x42fcab4e, /// 0xad4
			 0x179d2028, /// 0xad8
			 0x6d51c0ec, /// 0xadc
			 0x1d1aa098, /// 0xae0
			 0x00c602b4, /// 0xae4
			 0xed8eaa33, /// 0xae8
			 0x50965997, /// 0xaec
			 0xc11d2984, /// 0xaf0
			 0x0dbb9ec8, /// 0xaf4
			 0x97ceae0f, /// 0xaf8
			 0xb2a1583d, /// 0xafc
			 0xc5a63f10, /// 0xb00
			 0xf48ebb04, /// 0xb04
			 0xfba45e57, /// 0xb08
			 0x84ae7c00, /// 0xb0c
			 0x2ef9613d, /// 0xb10
			 0xef6f8ace, /// 0xb14
			 0x7be232f0, /// 0xb18
			 0x2f934922, /// 0xb1c
			 0x50ee5bbc, /// 0xb20
			 0x7ea17b43, /// 0xb24
			 0xad25c1da, /// 0xb28
			 0x50bd4f24, /// 0xb2c
			 0xf6c23fca, /// 0xb30
			 0x3a426346, /// 0xb34
			 0x10587fee, /// 0xb38
			 0xe25dc988, /// 0xb3c
			 0x07eef199, /// 0xb40
			 0x09bf2eb5, /// 0xb44
			 0x731b4345, /// 0xb48
			 0x07f96fd0, /// 0xb4c
			 0xdde09274, /// 0xb50
			 0x61e93274, /// 0xb54
			 0xb484ad2d, /// 0xb58
			 0x7f7b585b, /// 0xb5c
			 0x8cde9450, /// 0xb60
			 0x2c97b5e2, /// 0xb64
			 0xfa3c6017, /// 0xb68
			 0xfa81a9ae, /// 0xb6c
			 0xc594260b, /// 0xb70
			 0x0f4cbef0, /// 0xb74
			 0x8f39dcab, /// 0xb78
			 0xf9a23262, /// 0xb7c
			 0xad72cbbf, /// 0xb80
			 0xaf1b5bf2, /// 0xb84
			 0xf74a838f, /// 0xb88
			 0x47e2e4cc, /// 0xb8c
			 0x1c9fb050, /// 0xb90
			 0xa24ff0b0, /// 0xb94
			 0xff30437b, /// 0xb98
			 0x3eafbd32, /// 0xb9c
			 0x99c37c0f, /// 0xba0
			 0x2239b0f9, /// 0xba4
			 0x8d411e9a, /// 0xba8
			 0x16981f59, /// 0xbac
			 0x2c3b5dc7, /// 0xbb0
			 0x194d844f, /// 0xbb4
			 0xe5c36ebb, /// 0xbb8
			 0x2e9f0f56, /// 0xbbc
			 0x59b39908, /// 0xbc0
			 0x3d1fdfb2, /// 0xbc4
			 0xb3121d1d, /// 0xbc8
			 0xd66f4635, /// 0xbcc
			 0x05e4e1da, /// 0xbd0
			 0x9c7962ef, /// 0xbd4
			 0xc106d227, /// 0xbd8
			 0x79f8b4bc, /// 0xbdc
			 0x6b7edaae, /// 0xbe0
			 0x65fdaff7, /// 0xbe4
			 0x4d491dfa, /// 0xbe8
			 0xcf9682af, /// 0xbec
			 0x64c2a81d, /// 0xbf0
			 0x6168b174, /// 0xbf4
			 0xaa2231ec, /// 0xbf8
			 0xc4fb8a2e, /// 0xbfc
			 0x8255b915, /// 0xc00
			 0x58b3bd7e, /// 0xc04
			 0x4df1250d, /// 0xc08
			 0x706b6f11, /// 0xc0c
			 0x4570145f, /// 0xc10
			 0x1e69bc10, /// 0xc14
			 0x57d74c76, /// 0xc18
			 0x57dba04e, /// 0xc1c
			 0x092b6c8f, /// 0xc20
			 0x6b130d96, /// 0xc24
			 0xc298610b, /// 0xc28
			 0x4e1b96f9, /// 0xc2c
			 0xa0fc4d71, /// 0xc30
			 0xd0815e4a, /// 0xc34
			 0x4b58d87c, /// 0xc38
			 0xb31a5c75, /// 0xc3c
			 0x603cd880, /// 0xc40
			 0x5ba4c834, /// 0xc44
			 0x5eb9ba33, /// 0xc48
			 0x70c96e9d, /// 0xc4c
			 0xc73340c7, /// 0xc50
			 0x24d650ff, /// 0xc54
			 0x31150be3, /// 0xc58
			 0x1a344c65, /// 0xc5c
			 0x0e7330cd, /// 0xc60
			 0x00f416c3, /// 0xc64
			 0x2b810cd4, /// 0xc68
			 0xb6191f64, /// 0xc6c
			 0xc7f72e31, /// 0xc70
			 0x8b86e5a3, /// 0xc74
			 0x4d4cb97b, /// 0xc78
			 0xb96d7ed3, /// 0xc7c
			 0x7feebfb4, /// 0xc80
			 0x42d9abbf, /// 0xc84
			 0xa2050a26, /// 0xc88
			 0x8a4ec8ae, /// 0xc8c
			 0xa60f0674, /// 0xc90
			 0xa628aca8, /// 0xc94
			 0x4c2f404b, /// 0xc98
			 0x711d7d72, /// 0xc9c
			 0x7facb2fa, /// 0xca0
			 0xa47d2b8e, /// 0xca4
			 0xb0fb399f, /// 0xca8
			 0xf935933b, /// 0xcac
			 0x1dc42ed6, /// 0xcb0
			 0x1ed036b5, /// 0xcb4
			 0x4e46d3d0, /// 0xcb8
			 0xc89acadf, /// 0xcbc
			 0xaa5993fb, /// 0xcc0
			 0x56a74905, /// 0xcc4
			 0xb5534411, /// 0xcc8
			 0x38d95c3f, /// 0xccc
			 0x32a95a16, /// 0xcd0
			 0x1528871d, /// 0xcd4
			 0x30a0001b, /// 0xcd8
			 0xf9d92703, /// 0xcdc
			 0x066f1c7e, /// 0xce0
			 0xc194fa6b, /// 0xce4
			 0x8d7a05c5, /// 0xce8
			 0x2bad0fc8, /// 0xcec
			 0xf11746d8, /// 0xcf0
			 0x031f49f0, /// 0xcf4
			 0x51d7bde8, /// 0xcf8
			 0xc71fabbb, /// 0xcfc
			 0xa4bd7c08, /// 0xd00
			 0x2576194e, /// 0xd04
			 0xdc0caaea, /// 0xd08
			 0x5b162fa7, /// 0xd0c
			 0xbf322277, /// 0xd10
			 0xf4e0e16a, /// 0xd14
			 0x9ceb7de4, /// 0xd18
			 0xa9bc9b90, /// 0xd1c
			 0x5b16e2f8, /// 0xd20
			 0xb4c0aa56, /// 0xd24
			 0x532b0d2f, /// 0xd28
			 0xa347d490, /// 0xd2c
			 0x61dab36d, /// 0xd30
			 0x2ebcbfc2, /// 0xd34
			 0xf2857b45, /// 0xd38
			 0xaf1e45de, /// 0xd3c
			 0xb01429ce, /// 0xd40
			 0x2e8a75b3, /// 0xd44
			 0x0aeca612, /// 0xd48
			 0x53dd4590, /// 0xd4c
			 0xa8e4937b, /// 0xd50
			 0x0a214634, /// 0xd54
			 0x1c5a5362, /// 0xd58
			 0x02ca4e88, /// 0xd5c
			 0xf7cd938d, /// 0xd60
			 0x3d0cc24f, /// 0xd64
			 0xfa9d5853, /// 0xd68
			 0x369b49f8, /// 0xd6c
			 0x82f5ec61, /// 0xd70
			 0x4086079b, /// 0xd74
			 0x4e36d750, /// 0xd78
			 0x06642140, /// 0xd7c
			 0x2075b1c5, /// 0xd80
			 0x11451f51, /// 0xd84
			 0xb22a4229, /// 0xd88
			 0x96258053, /// 0xd8c
			 0xd69522c2, /// 0xd90
			 0xe1170f28, /// 0xd94
			 0x8e9a9faf, /// 0xd98
			 0x77da2317, /// 0xd9c
			 0x390675ff, /// 0xda0
			 0xb97789d2, /// 0xda4
			 0x31e77e5a, /// 0xda8
			 0x93dab6d4, /// 0xdac
			 0x8c32361e, /// 0xdb0
			 0x50a51dea, /// 0xdb4
			 0xa10cae32, /// 0xdb8
			 0x7fa30f54, /// 0xdbc
			 0xd7b735ea, /// 0xdc0
			 0xfd4c7dbb, /// 0xdc4
			 0x9ab7e563, /// 0xdc8
			 0x1652d840, /// 0xdcc
			 0x28893a5b, /// 0xdd0
			 0xe2e1155c, /// 0xdd4
			 0x8d74a1e0, /// 0xdd8
			 0xa63f4048, /// 0xddc
			 0xc3306dc3, /// 0xde0
			 0xd8e43131, /// 0xde4
			 0x0dbff966, /// 0xde8
			 0x61fbd983, /// 0xdec
			 0x666c1bc8, /// 0xdf0
			 0xb94c0b04, /// 0xdf4
			 0x17636a12, /// 0xdf8
			 0x9c51f657, /// 0xdfc
			 0xcd83d5ed, /// 0xe00
			 0x3d49803d, /// 0xe04
			 0x76a20ef8, /// 0xe08
			 0xa2dc5be2, /// 0xe0c
			 0x6675bd7c, /// 0xe10
			 0xe5756d3e, /// 0xe14
			 0x0666d90f, /// 0xe18
			 0xd8a38097, /// 0xe1c
			 0x9b42d85f, /// 0xe20
			 0x4737e3da, /// 0xe24
			 0x1dc77151, /// 0xe28
			 0xf1a5dd56, /// 0xe2c
			 0x00852559, /// 0xe30
			 0x986488a9, /// 0xe34
			 0x6c3c0e9d, /// 0xe38
			 0xbde1149a, /// 0xe3c
			 0xad5c7a05, /// 0xe40
			 0xcfc5ff10, /// 0xe44
			 0x0646be18, /// 0xe48
			 0x40f1fce2, /// 0xe4c
			 0xa3995e8a, /// 0xe50
			 0xd66591e2, /// 0xe54
			 0x487a2b52, /// 0xe58
			 0xe4da3311, /// 0xe5c
			 0x001183de, /// 0xe60
			 0xcb0a1e80, /// 0xe64
			 0xe5ec1954, /// 0xe68
			 0x469960f8, /// 0xe6c
			 0xb878ea4a, /// 0xe70
			 0x8b865ad1, /// 0xe74
			 0xf628f9a0, /// 0xe78
			 0x869203af, /// 0xe7c
			 0xb50d895a, /// 0xe80
			 0xb2e60427, /// 0xe84
			 0x69f81a0f, /// 0xe88
			 0xdf830d9d, /// 0xe8c
			 0x697532dc, /// 0xe90
			 0xf0cb75fc, /// 0xe94
			 0x2eb6f73c, /// 0xe98
			 0xa80a603b, /// 0xe9c
			 0x03201345, /// 0xea0
			 0xee84bb48, /// 0xea4
			 0x1830c156, /// 0xea8
			 0x1648081b, /// 0xeac
			 0x7b033fab, /// 0xeb0
			 0xd636ca7c, /// 0xeb4
			 0x607cdde3, /// 0xeb8
			 0x99442641, /// 0xebc
			 0x51d302a4, /// 0xec0
			 0xfef375fc, /// 0xec4
			 0x5fa3463b, /// 0xec8
			 0xfb30cd4e, /// 0xecc
			 0xb8442545, /// 0xed0
			 0x8fa05534, /// 0xed4
			 0x2b638aef, /// 0xed8
			 0x765a3958, /// 0xedc
			 0x5d978010, /// 0xee0
			 0xdc010ec5, /// 0xee4
			 0xb2e9af86, /// 0xee8
			 0x56151301, /// 0xeec
			 0x40d2d02f, /// 0xef0
			 0xf2197152, /// 0xef4
			 0x36f13b21, /// 0xef8
			 0x5b8dbf53, /// 0xefc
			 0x3def79dc, /// 0xf00
			 0x664c9519, /// 0xf04
			 0x45efff1b, /// 0xf08
			 0x6497c352, /// 0xf0c
			 0x90b422a1, /// 0xf10
			 0xa127cd4f, /// 0xf14
			 0xf97e67f8, /// 0xf18
			 0x2c31997a, /// 0xf1c
			 0xd2c8e24e, /// 0xf20
			 0xbaff45da, /// 0xf24
			 0xb52ba029, /// 0xf28
			 0x40c0ef7d, /// 0xf2c
			 0x31bd9023, /// 0xf30
			 0xb519665d, /// 0xf34
			 0x8534d193, /// 0xf38
			 0x15ecf0ac, /// 0xf3c
			 0x763bae97, /// 0xf40
			 0x800eb272, /// 0xf44
			 0xbc5d4a79, /// 0xf48
			 0xeccdd201, /// 0xf4c
			 0xb3cd48f7, /// 0xf50
			 0x53060a90, /// 0xf54
			 0xc5c5d29b, /// 0xf58
			 0xb72b448b, /// 0xf5c
			 0xcef3fe94, /// 0xf60
			 0x0ef1993d, /// 0xf64
			 0xfa5ef5e8, /// 0xf68
			 0xb5f51e27, /// 0xf6c
			 0xf769e8fa, /// 0xf70
			 0x51f490f3, /// 0xf74
			 0x7b27c691, /// 0xf78
			 0xe5ae3e3d, /// 0xf7c
			 0xac586491, /// 0xf80
			 0xcc8beb7a, /// 0xf84
			 0xcd937607, /// 0xf88
			 0xb6905fd0, /// 0xf8c
			 0xf9ab9ee6, /// 0xf90
			 0x7735dfb1, /// 0xf94
			 0x60da73fe, /// 0xf98
			 0x9af5a94b, /// 0xf9c
			 0xfc7a7859, /// 0xfa0
			 0xa3afeb47, /// 0xfa4
			 0x68498d2e, /// 0xfa8
			 0x732ebdb9, /// 0xfac
			 0xbe876d4a, /// 0xfb0
			 0x0cc29fa8, /// 0xfb4
			 0xb720996f, /// 0xfb8
			 0x9207f04a, /// 0xfbc
			 0x0551d25b, /// 0xfc0
			 0xf6b9e125, /// 0xfc4
			 0xd84753ec, /// 0xfc8
			 0x6391da08, /// 0xfcc
			 0x6583d880, /// 0xfd0
			 0x882eaa6c, /// 0xfd4
			 0xb53295cd, /// 0xfd8
			 0xb35803c4, /// 0xfdc
			 0x3e6850b2, /// 0xfe0
			 0x4c7941c1, /// 0xfe4
			 0x58174f2e, /// 0xfe8
			 0x044da6c6, /// 0xfec
			 0x33c7445b, /// 0xff0
			 0xcecadbd6, /// 0xff4
			 0xb44b1ef4, /// 0xff8
			 0xd1ec2743 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x224bc2f3, /// 0x0
			 0x0634ea27, /// 0x4
			 0x3033b758, /// 0x8
			 0xd4faf762, /// 0xc
			 0x2111dd3a, /// 0x10
			 0x577608e0, /// 0x14
			 0x8005c247, /// 0x18
			 0x98692d99, /// 0x1c
			 0x538c83ed, /// 0x20
			 0xe6801e48, /// 0x24
			 0x68924695, /// 0x28
			 0x456b2dc1, /// 0x2c
			 0x5c6db0c4, /// 0x30
			 0x3e79bb71, /// 0x34
			 0xd07de771, /// 0x38
			 0xf6b17d65, /// 0x3c
			 0x1e8e68c4, /// 0x40
			 0xe6ce5240, /// 0x44
			 0x3c432164, /// 0x48
			 0xb5a689bc, /// 0x4c
			 0x66dcf32b, /// 0x50
			 0x26f329a1, /// 0x54
			 0xfc2f9fd6, /// 0x58
			 0x150f4111, /// 0x5c
			 0x0394ceb9, /// 0x60
			 0x1b18101b, /// 0x64
			 0xe79cfb4b, /// 0x68
			 0x2d2df230, /// 0x6c
			 0x942d273f, /// 0x70
			 0x108d0603, /// 0x74
			 0x0f909111, /// 0x78
			 0x178b8c26, /// 0x7c
			 0x8bb97080, /// 0x80
			 0xb5ede58b, /// 0x84
			 0xbd779ab6, /// 0x88
			 0x44dff418, /// 0x8c
			 0x2c18508a, /// 0x90
			 0x1735090c, /// 0x94
			 0x164c966a, /// 0x98
			 0x2c5e6ff1, /// 0x9c
			 0x680be0fd, /// 0xa0
			 0xbda73d65, /// 0xa4
			 0xde5c1cb6, /// 0xa8
			 0xc0c5f7c5, /// 0xac
			 0x2fcb4c50, /// 0xb0
			 0x0d72c159, /// 0xb4
			 0x635046ae, /// 0xb8
			 0x5dbf3d5e, /// 0xbc
			 0x7b933780, /// 0xc0
			 0x4e7f1f8f, /// 0xc4
			 0xfb9f22f0, /// 0xc8
			 0xfc9bc21a, /// 0xcc
			 0xaef77f44, /// 0xd0
			 0x2b5a9d5d, /// 0xd4
			 0x2567ad11, /// 0xd8
			 0x8191a04c, /// 0xdc
			 0x5179c995, /// 0xe0
			 0x90b2cb2a, /// 0xe4
			 0x175e4c8e, /// 0xe8
			 0x2f4ba894, /// 0xec
			 0x72e564b1, /// 0xf0
			 0x0dec8884, /// 0xf4
			 0x16e28ae7, /// 0xf8
			 0xd7d17259, /// 0xfc
			 0x8ddc8fd3, /// 0x100
			 0x95016d7a, /// 0x104
			 0x044a8191, /// 0x108
			 0x914a31ec, /// 0x10c
			 0xcd140fe6, /// 0x110
			 0x9a63cbc9, /// 0x114
			 0x0e792b17, /// 0x118
			 0x3a2ac895, /// 0x11c
			 0xd9faef3c, /// 0x120
			 0x23e1d8e7, /// 0x124
			 0xbdc5da62, /// 0x128
			 0xbe09622b, /// 0x12c
			 0x949351dd, /// 0x130
			 0xee3957f9, /// 0x134
			 0xe7a8102a, /// 0x138
			 0x04df387a, /// 0x13c
			 0xc8d1e263, /// 0x140
			 0x3953454d, /// 0x144
			 0x6caacac7, /// 0x148
			 0xc466b7f5, /// 0x14c
			 0x1069fd70, /// 0x150
			 0x696fc7bc, /// 0x154
			 0x2dda723c, /// 0x158
			 0x771bfd33, /// 0x15c
			 0x67cccaf0, /// 0x160
			 0xce110b7e, /// 0x164
			 0xdbd6145a, /// 0x168
			 0x457ff060, /// 0x16c
			 0xd898f308, /// 0x170
			 0x1b7a3276, /// 0x174
			 0x2471b7fa, /// 0x178
			 0x12f6ea3c, /// 0x17c
			 0xec65319d, /// 0x180
			 0x6a77b595, /// 0x184
			 0xbd85c910, /// 0x188
			 0x01cebc46, /// 0x18c
			 0x9c68db27, /// 0x190
			 0xab9f7b6e, /// 0x194
			 0x38e8a68f, /// 0x198
			 0xb85da94a, /// 0x19c
			 0x48f3d334, /// 0x1a0
			 0x53749967, /// 0x1a4
			 0x2ca79068, /// 0x1a8
			 0x3a49219d, /// 0x1ac
			 0xcfdf8056, /// 0x1b0
			 0xfa868871, /// 0x1b4
			 0x325f2d24, /// 0x1b8
			 0x33a50288, /// 0x1bc
			 0xe85fbe3b, /// 0x1c0
			 0xaa430aed, /// 0x1c4
			 0xccb09e0f, /// 0x1c8
			 0x44609a34, /// 0x1cc
			 0x655aaeef, /// 0x1d0
			 0x1f3356f8, /// 0x1d4
			 0xbff2d262, /// 0x1d8
			 0x106ac6cc, /// 0x1dc
			 0x110bdbb4, /// 0x1e0
			 0x5105ec39, /// 0x1e4
			 0xece7e6cd, /// 0x1e8
			 0x91b6f342, /// 0x1ec
			 0x0f1f58c6, /// 0x1f0
			 0xb133aef1, /// 0x1f4
			 0x7dedaf14, /// 0x1f8
			 0x259669a8, /// 0x1fc
			 0xf6e0e67e, /// 0x200
			 0xdef8bdce, /// 0x204
			 0xb26807cb, /// 0x208
			 0xb3df33fc, /// 0x20c
			 0x4f3858e5, /// 0x210
			 0x7d27c7be, /// 0x214
			 0x42f0990a, /// 0x218
			 0x59e212e6, /// 0x21c
			 0xc21a1c08, /// 0x220
			 0xd9a35cc7, /// 0x224
			 0x8fb8ddd9, /// 0x228
			 0xc1d9452e, /// 0x22c
			 0x01eef1f0, /// 0x230
			 0x34cda6e1, /// 0x234
			 0x4373687f, /// 0x238
			 0xbb9e61b4, /// 0x23c
			 0x43f25615, /// 0x240
			 0x1ef86b52, /// 0x244
			 0xe2b39e5c, /// 0x248
			 0x84113b50, /// 0x24c
			 0xcf5f3094, /// 0x250
			 0x1218b750, /// 0x254
			 0x76163ff9, /// 0x258
			 0x9e78334c, /// 0x25c
			 0xac4ec988, /// 0x260
			 0x8df711ad, /// 0x264
			 0x7006cf9a, /// 0x268
			 0x2b4e682a, /// 0x26c
			 0x5c5fcd67, /// 0x270
			 0x25d5fb91, /// 0x274
			 0x0b2937f7, /// 0x278
			 0x735beb85, /// 0x27c
			 0x5bb56949, /// 0x280
			 0xa6868be4, /// 0x284
			 0x6f4c7cb3, /// 0x288
			 0xa04f162e, /// 0x28c
			 0x6d1c0d69, /// 0x290
			 0xf7a1701f, /// 0x294
			 0x70fe6c11, /// 0x298
			 0x9f783d63, /// 0x29c
			 0xdbd5d42c, /// 0x2a0
			 0x9191d457, /// 0x2a4
			 0x87643432, /// 0x2a8
			 0xe716fb15, /// 0x2ac
			 0xc16af585, /// 0x2b0
			 0x7bfadeac, /// 0x2b4
			 0x8fe2d7ed, /// 0x2b8
			 0x7eb389d3, /// 0x2bc
			 0x96d7608e, /// 0x2c0
			 0x3ea83b1d, /// 0x2c4
			 0xdfd8a34f, /// 0x2c8
			 0x8f499f12, /// 0x2cc
			 0xda2cec43, /// 0x2d0
			 0x418f89a5, /// 0x2d4
			 0x20285c8b, /// 0x2d8
			 0xed6606d1, /// 0x2dc
			 0x64c9590e, /// 0x2e0
			 0x63af1251, /// 0x2e4
			 0xb06d519e, /// 0x2e8
			 0x2d226442, /// 0x2ec
			 0xf084c9f5, /// 0x2f0
			 0x554cc3d8, /// 0x2f4
			 0x3e5e26fa, /// 0x2f8
			 0xeb43cf63, /// 0x2fc
			 0xb2969a4b, /// 0x300
			 0xa7f4d6ed, /// 0x304
			 0x7d7bedf5, /// 0x308
			 0x143123de, /// 0x30c
			 0xa7e52c9c, /// 0x310
			 0x7d5c4821, /// 0x314
			 0x4e034075, /// 0x318
			 0x1c55a394, /// 0x31c
			 0x3462c937, /// 0x320
			 0x6cb6dc64, /// 0x324
			 0x79493126, /// 0x328
			 0x6dd7e325, /// 0x32c
			 0xc73526db, /// 0x330
			 0x6e31dabf, /// 0x334
			 0xdd03612c, /// 0x338
			 0xd6144deb, /// 0x33c
			 0xf1204069, /// 0x340
			 0x3c35663c, /// 0x344
			 0x52e9e25f, /// 0x348
			 0x4e369637, /// 0x34c
			 0xc2e12384, /// 0x350
			 0xad14a472, /// 0x354
			 0x0ba7613f, /// 0x358
			 0x262783c8, /// 0x35c
			 0x5a0840f8, /// 0x360
			 0x4622ff62, /// 0x364
			 0x62457f1c, /// 0x368
			 0xdc9ccfad, /// 0x36c
			 0x59c94c2d, /// 0x370
			 0x4bc85119, /// 0x374
			 0x65e84c33, /// 0x378
			 0x6cf8fd2b, /// 0x37c
			 0xdea2961c, /// 0x380
			 0x058ec802, /// 0x384
			 0x30859d02, /// 0x388
			 0x5b4fc66f, /// 0x38c
			 0x816b730a, /// 0x390
			 0x2d2002fd, /// 0x394
			 0x2f66d38f, /// 0x398
			 0x10915084, /// 0x39c
			 0xea55ae78, /// 0x3a0
			 0xcbed5130, /// 0x3a4
			 0xa2c579cf, /// 0x3a8
			 0xe9d96941, /// 0x3ac
			 0x006cac99, /// 0x3b0
			 0xe9611e7e, /// 0x3b4
			 0x9b7ae67e, /// 0x3b8
			 0x47ff2960, /// 0x3bc
			 0xda8d5521, /// 0x3c0
			 0x5a4e5c18, /// 0x3c4
			 0x86b82b50, /// 0x3c8
			 0x1082259b, /// 0x3cc
			 0x553c1383, /// 0x3d0
			 0x23bb4cf6, /// 0x3d4
			 0xf75c6732, /// 0x3d8
			 0xbf307df2, /// 0x3dc
			 0x03bc9fe1, /// 0x3e0
			 0x111742aa, /// 0x3e4
			 0xa1405002, /// 0x3e8
			 0x03b1f61c, /// 0x3ec
			 0x5271705b, /// 0x3f0
			 0x10ef4ca9, /// 0x3f4
			 0x646d242e, /// 0x3f8
			 0x0d0ffeb7, /// 0x3fc
			 0x875b8764, /// 0x400
			 0x1b54d32d, /// 0x404
			 0x4789db43, /// 0x408
			 0xf366bd7f, /// 0x40c
			 0xe91fedf8, /// 0x410
			 0x6cbd2892, /// 0x414
			 0xff6b9a06, /// 0x418
			 0x1f218778, /// 0x41c
			 0xfb4a49d9, /// 0x420
			 0xb834e3fb, /// 0x424
			 0x49c7b6f1, /// 0x428
			 0x74807252, /// 0x42c
			 0x78e3e2b2, /// 0x430
			 0x185b4f4b, /// 0x434
			 0x0d6e4112, /// 0x438
			 0x8482f7e7, /// 0x43c
			 0xa7b0ad86, /// 0x440
			 0x6ca76b9c, /// 0x444
			 0x50ccdee1, /// 0x448
			 0xec679b1d, /// 0x44c
			 0x28293732, /// 0x450
			 0xf309ea89, /// 0x454
			 0x2d554d02, /// 0x458
			 0x7fd28cbb, /// 0x45c
			 0xf67b13bd, /// 0x460
			 0x1a7c281f, /// 0x464
			 0xc8c5611a, /// 0x468
			 0xeda9e838, /// 0x46c
			 0x6ba7b799, /// 0x470
			 0xe8514862, /// 0x474
			 0x2faf33fc, /// 0x478
			 0x0bcf7d54, /// 0x47c
			 0xb5fdf751, /// 0x480
			 0x15fec98f, /// 0x484
			 0x27dcf18f, /// 0x488
			 0x66c38f60, /// 0x48c
			 0x86c92888, /// 0x490
			 0xacb4adcf, /// 0x494
			 0x1e560e26, /// 0x498
			 0xac6f2ca9, /// 0x49c
			 0xa82808e4, /// 0x4a0
			 0x32d7f412, /// 0x4a4
			 0xf75e7510, /// 0x4a8
			 0x14129bf9, /// 0x4ac
			 0x1ef7b627, /// 0x4b0
			 0xf138d266, /// 0x4b4
			 0x4745d18c, /// 0x4b8
			 0x394e17bb, /// 0x4bc
			 0xcb4aad8e, /// 0x4c0
			 0x1cb4a660, /// 0x4c4
			 0x2b897b1d, /// 0x4c8
			 0x4a82d90f, /// 0x4cc
			 0x81bf6731, /// 0x4d0
			 0x3c879ba8, /// 0x4d4
			 0x626a2b7b, /// 0x4d8
			 0xa607ff33, /// 0x4dc
			 0x4082b620, /// 0x4e0
			 0xb11e07b3, /// 0x4e4
			 0x1d8ef472, /// 0x4e8
			 0xd2ec7aaf, /// 0x4ec
			 0xc5b5e312, /// 0x4f0
			 0xbacdfa0c, /// 0x4f4
			 0x7f87ad1a, /// 0x4f8
			 0x124c0ff4, /// 0x4fc
			 0xea5d4219, /// 0x500
			 0x019b81a3, /// 0x504
			 0x9ae9777d, /// 0x508
			 0xb160726f, /// 0x50c
			 0xc20b47dd, /// 0x510
			 0xb4048014, /// 0x514
			 0x82e5856d, /// 0x518
			 0x08311a6d, /// 0x51c
			 0xf76926ae, /// 0x520
			 0xc9d41175, /// 0x524
			 0x60254f1d, /// 0x528
			 0xd57865b3, /// 0x52c
			 0x78482cbc, /// 0x530
			 0x032e9726, /// 0x534
			 0xe31b9c1c, /// 0x538
			 0xa0fda5ad, /// 0x53c
			 0x83796f2f, /// 0x540
			 0xc0c2032b, /// 0x544
			 0x23fff583, /// 0x548
			 0xd40f8f66, /// 0x54c
			 0x49fbf7c0, /// 0x550
			 0x5509f047, /// 0x554
			 0x4b477098, /// 0x558
			 0x1e169a26, /// 0x55c
			 0xd58fe710, /// 0x560
			 0x2190fba3, /// 0x564
			 0xff2f7545, /// 0x568
			 0x9a34e236, /// 0x56c
			 0x33d34ff9, /// 0x570
			 0xab314061, /// 0x574
			 0x7e3f6329, /// 0x578
			 0x6f1a2331, /// 0x57c
			 0xade115a3, /// 0x580
			 0x88f9c130, /// 0x584
			 0x1b00dd6e, /// 0x588
			 0x7aaf807e, /// 0x58c
			 0xabafbe93, /// 0x590
			 0x52b19698, /// 0x594
			 0x763a71d2, /// 0x598
			 0xb58a4970, /// 0x59c
			 0x78ff131f, /// 0x5a0
			 0x8882daba, /// 0x5a4
			 0x00befd28, /// 0x5a8
			 0x0ad567c7, /// 0x5ac
			 0x88638e05, /// 0x5b0
			 0xbfe0a0ba, /// 0x5b4
			 0x445913a3, /// 0x5b8
			 0x4ea17bae, /// 0x5bc
			 0x4a69ec33, /// 0x5c0
			 0xec5cdb58, /// 0x5c4
			 0xc72b8a7a, /// 0x5c8
			 0xd21afec6, /// 0x5cc
			 0x801cf39b, /// 0x5d0
			 0x8e161a53, /// 0x5d4
			 0x6059e8ec, /// 0x5d8
			 0x6e4e7418, /// 0x5dc
			 0x5dd028f1, /// 0x5e0
			 0xe38ecb30, /// 0x5e4
			 0x2a76fc66, /// 0x5e8
			 0x918b29a0, /// 0x5ec
			 0xd87f691a, /// 0x5f0
			 0x36851dad, /// 0x5f4
			 0x5629d45d, /// 0x5f8
			 0xf4721136, /// 0x5fc
			 0xeb1635c2, /// 0x600
			 0x7d22b3a7, /// 0x604
			 0x2f99bc5d, /// 0x608
			 0x4927ae32, /// 0x60c
			 0xad418fcb, /// 0x610
			 0xd6b3d3d1, /// 0x614
			 0xb1e8479a, /// 0x618
			 0x2306fd3b, /// 0x61c
			 0x2545d0d5, /// 0x620
			 0x6328d500, /// 0x624
			 0x171562cb, /// 0x628
			 0xba4ba316, /// 0x62c
			 0x750aa43e, /// 0x630
			 0x9f22a50a, /// 0x634
			 0x297ab5c2, /// 0x638
			 0x830f289e, /// 0x63c
			 0x1f5f9d0b, /// 0x640
			 0x54c2fa1a, /// 0x644
			 0xbfb4ccac, /// 0x648
			 0xbc9f7b41, /// 0x64c
			 0xd682b6ed, /// 0x650
			 0x658985c2, /// 0x654
			 0x502c72e3, /// 0x658
			 0xf2a16706, /// 0x65c
			 0x286f39aa, /// 0x660
			 0x6333020f, /// 0x664
			 0x4f9ecce9, /// 0x668
			 0x83c5c297, /// 0x66c
			 0x52211aff, /// 0x670
			 0x7805bd2f, /// 0x674
			 0x3ffe48ef, /// 0x678
			 0x991ef9ec, /// 0x67c
			 0xca515508, /// 0x680
			 0x7779f126, /// 0x684
			 0x2c546a35, /// 0x688
			 0x88af7c0f, /// 0x68c
			 0xf08062bb, /// 0x690
			 0x42505744, /// 0x694
			 0x2afb4bc7, /// 0x698
			 0x2593da93, /// 0x69c
			 0x73f1e531, /// 0x6a0
			 0x4251c016, /// 0x6a4
			 0xefe68c83, /// 0x6a8
			 0x9bd1f786, /// 0x6ac
			 0xe791d290, /// 0x6b0
			 0x004b5831, /// 0x6b4
			 0x81cbdb1d, /// 0x6b8
			 0x757b73df, /// 0x6bc
			 0x00c95dca, /// 0x6c0
			 0xf20256c7, /// 0x6c4
			 0x09b4f38e, /// 0x6c8
			 0x0f8da4be, /// 0x6cc
			 0xa8451ac9, /// 0x6d0
			 0x5e05f76f, /// 0x6d4
			 0x075903e3, /// 0x6d8
			 0x63b3db7c, /// 0x6dc
			 0xf2ef949c, /// 0x6e0
			 0xa2e4434b, /// 0x6e4
			 0x9b0353d2, /// 0x6e8
			 0x97c38ea2, /// 0x6ec
			 0xf5bc5038, /// 0x6f0
			 0x9028612e, /// 0x6f4
			 0x45063d9a, /// 0x6f8
			 0x586737d4, /// 0x6fc
			 0xf4c980a3, /// 0x700
			 0xcbd93163, /// 0x704
			 0x27cb83c7, /// 0x708
			 0x38164362, /// 0x70c
			 0x0326c119, /// 0x710
			 0x765e5558, /// 0x714
			 0x9efbbad0, /// 0x718
			 0x2fe484ae, /// 0x71c
			 0x5e870510, /// 0x720
			 0x1e4c652a, /// 0x724
			 0x44b6e6b0, /// 0x728
			 0x2c008fd8, /// 0x72c
			 0xa5326f7e, /// 0x730
			 0xf55d962b, /// 0x734
			 0x529217c5, /// 0x738
			 0xd6622f2b, /// 0x73c
			 0xfa9fb496, /// 0x740
			 0xc2c1b71f, /// 0x744
			 0xa1aa7321, /// 0x748
			 0xd77c6f69, /// 0x74c
			 0x7ed9e878, /// 0x750
			 0x088a290a, /// 0x754
			 0x3752be8d, /// 0x758
			 0xb3b7f7ab, /// 0x75c
			 0xe673831f, /// 0x760
			 0x8a32b5ab, /// 0x764
			 0x7b2016aa, /// 0x768
			 0xe9644495, /// 0x76c
			 0xb5aa9cca, /// 0x770
			 0x6a5f130d, /// 0x774
			 0xc8d5dc4e, /// 0x778
			 0x9eea9c05, /// 0x77c
			 0x51a46d2c, /// 0x780
			 0x125c70fe, /// 0x784
			 0x8a03f06f, /// 0x788
			 0x7d57a8e0, /// 0x78c
			 0x4e9ba990, /// 0x790
			 0xd7fe5733, /// 0x794
			 0x2374f97a, /// 0x798
			 0x3483bf7a, /// 0x79c
			 0xf9e62360, /// 0x7a0
			 0x9cda5b1d, /// 0x7a4
			 0xefe01346, /// 0x7a8
			 0x88aaffc3, /// 0x7ac
			 0x969625d8, /// 0x7b0
			 0x5ec2ac37, /// 0x7b4
			 0xd430acab, /// 0x7b8
			 0xe46b71ba, /// 0x7bc
			 0x29698a28, /// 0x7c0
			 0x114e7b96, /// 0x7c4
			 0x8a3ff7e2, /// 0x7c8
			 0x3e5a20e5, /// 0x7cc
			 0xacddea08, /// 0x7d0
			 0x010a7ff1, /// 0x7d4
			 0x8a6d8508, /// 0x7d8
			 0x23cbcf28, /// 0x7dc
			 0x61100302, /// 0x7e0
			 0x4b62602a, /// 0x7e4
			 0xecdc36d7, /// 0x7e8
			 0x4b30725f, /// 0x7ec
			 0x3f38a0be, /// 0x7f0
			 0x7d8f4474, /// 0x7f4
			 0x3fb7b8eb, /// 0x7f8
			 0x76a7d70d, /// 0x7fc
			 0x1ae849df, /// 0x800
			 0x0c77e61d, /// 0x804
			 0xd97498e5, /// 0x808
			 0xcead1cb4, /// 0x80c
			 0xb80f61dc, /// 0x810
			 0x25aa572f, /// 0x814
			 0xdb8b0ca1, /// 0x818
			 0x4e2917f4, /// 0x81c
			 0x8ba7859d, /// 0x820
			 0x85fbdf4a, /// 0x824
			 0xefb1e112, /// 0x828
			 0x164b15bb, /// 0x82c
			 0xeb3b9703, /// 0x830
			 0xd129505e, /// 0x834
			 0xed37451e, /// 0x838
			 0x636429eb, /// 0x83c
			 0xdbe9e94a, /// 0x840
			 0xba3b06b0, /// 0x844
			 0x4eda0953, /// 0x848
			 0xd5610112, /// 0x84c
			 0x5ea4a2c6, /// 0x850
			 0x4f6ee21f, /// 0x854
			 0x5ece83e5, /// 0x858
			 0x8ea2687f, /// 0x85c
			 0xf24074ea, /// 0x860
			 0xbe447316, /// 0x864
			 0x3a6f9a7b, /// 0x868
			 0x5a0c2154, /// 0x86c
			 0x8a30a7ef, /// 0x870
			 0x9bece555, /// 0x874
			 0x3853067f, /// 0x878
			 0x496f7995, /// 0x87c
			 0x4ba4b39b, /// 0x880
			 0x80c9e225, /// 0x884
			 0x37533ea8, /// 0x888
			 0x8b6c40c8, /// 0x88c
			 0xc3a2c18c, /// 0x890
			 0xfbd4a068, /// 0x894
			 0xd8afc1df, /// 0x898
			 0x38282b64, /// 0x89c
			 0xd78b5780, /// 0x8a0
			 0x620d7fd4, /// 0x8a4
			 0xf78980df, /// 0x8a8
			 0x3661bc1b, /// 0x8ac
			 0xe7d7c2a1, /// 0x8b0
			 0xcf7d8a79, /// 0x8b4
			 0x10daef79, /// 0x8b8
			 0x79a271a8, /// 0x8bc
			 0xd6fa3b33, /// 0x8c0
			 0x448a5420, /// 0x8c4
			 0xb4fd7273, /// 0x8c8
			 0xb307c0b4, /// 0x8cc
			 0xa1b2462b, /// 0x8d0
			 0x86a1a65f, /// 0x8d4
			 0xb9571586, /// 0x8d8
			 0x7704a47c, /// 0x8dc
			 0xeaab1a39, /// 0x8e0
			 0x3c1a8741, /// 0x8e4
			 0xbf0aaebf, /// 0x8e8
			 0xfd4dfee8, /// 0x8ec
			 0xf56402e3, /// 0x8f0
			 0x0036b423, /// 0x8f4
			 0x0facf7c6, /// 0x8f8
			 0x6ae114a9, /// 0x8fc
			 0x77fd7c0d, /// 0x900
			 0x65e75088, /// 0x904
			 0xb41ed01c, /// 0x908
			 0x59a99888, /// 0x90c
			 0x28d9ce1b, /// 0x910
			 0x97663726, /// 0x914
			 0xe48fbc88, /// 0x918
			 0xdc541ca0, /// 0x91c
			 0xdfc25648, /// 0x920
			 0x30de5017, /// 0x924
			 0x4bb05572, /// 0x928
			 0x4b65268e, /// 0x92c
			 0x6a1fb68d, /// 0x930
			 0x89e9da0b, /// 0x934
			 0xa4511e03, /// 0x938
			 0xfaf7d6b9, /// 0x93c
			 0x31ba773c, /// 0x940
			 0xba2d41e7, /// 0x944
			 0x3a767579, /// 0x948
			 0x8b27c203, /// 0x94c
			 0x9f69bd80, /// 0x950
			 0x8f556dad, /// 0x954
			 0xc6d69e02, /// 0x958
			 0x820ee63d, /// 0x95c
			 0x37daf184, /// 0x960
			 0xce16858d, /// 0x964
			 0x9ad1134e, /// 0x968
			 0x6fe2b255, /// 0x96c
			 0x3af6fc2a, /// 0x970
			 0x48eb90fd, /// 0x974
			 0x06d21040, /// 0x978
			 0xa2335c7d, /// 0x97c
			 0x32ce3169, /// 0x980
			 0x30f1864d, /// 0x984
			 0x07fc5dfa, /// 0x988
			 0x58fbbd56, /// 0x98c
			 0xb3faefaa, /// 0x990
			 0x556d115c, /// 0x994
			 0x2b8e72e8, /// 0x998
			 0x872a6dd0, /// 0x99c
			 0xe0b57455, /// 0x9a0
			 0xda1129b2, /// 0x9a4
			 0x14ff201e, /// 0x9a8
			 0xac13fbef, /// 0x9ac
			 0xb7df044d, /// 0x9b0
			 0xe2fba279, /// 0x9b4
			 0x2ec16068, /// 0x9b8
			 0xf7371c9f, /// 0x9bc
			 0x7cbbffc6, /// 0x9c0
			 0xe26eac2c, /// 0x9c4
			 0x5054c4e0, /// 0x9c8
			 0x0f376275, /// 0x9cc
			 0x18ecf110, /// 0x9d0
			 0x2007f03b, /// 0x9d4
			 0x1e297b55, /// 0x9d8
			 0x76669cc8, /// 0x9dc
			 0x11651c23, /// 0x9e0
			 0x588810c0, /// 0x9e4
			 0x40876d36, /// 0x9e8
			 0x63ea60d1, /// 0x9ec
			 0xa7654e74, /// 0x9f0
			 0xe760aa26, /// 0x9f4
			 0x1b095483, /// 0x9f8
			 0xf8371dcc, /// 0x9fc
			 0x0b45587d, /// 0xa00
			 0xb0412532, /// 0xa04
			 0x845a6361, /// 0xa08
			 0x2b8de5ee, /// 0xa0c
			 0x20ef31b0, /// 0xa10
			 0x3426a6b2, /// 0xa14
			 0xdc80181e, /// 0xa18
			 0x128e23dc, /// 0xa1c
			 0x59f3dcec, /// 0xa20
			 0x4d6c0541, /// 0xa24
			 0xd0b04910, /// 0xa28
			 0x9668d6e1, /// 0xa2c
			 0x303cf5fa, /// 0xa30
			 0x5ae1d7b5, /// 0xa34
			 0x59ace0f3, /// 0xa38
			 0x4a4ef457, /// 0xa3c
			 0x00368214, /// 0xa40
			 0x1997c1e6, /// 0xa44
			 0xf01f466f, /// 0xa48
			 0x2c3d7c50, /// 0xa4c
			 0xf1c4db2a, /// 0xa50
			 0x17972242, /// 0xa54
			 0x6178af4d, /// 0xa58
			 0xd5e0cc40, /// 0xa5c
			 0xcd7b5dc8, /// 0xa60
			 0xd4c0d6a1, /// 0xa64
			 0x83b53c0b, /// 0xa68
			 0x2508ef82, /// 0xa6c
			 0x5a0ca301, /// 0xa70
			 0x54e09ecb, /// 0xa74
			 0xbabdcd6d, /// 0xa78
			 0x6095c5c8, /// 0xa7c
			 0xc87602b2, /// 0xa80
			 0xd40bab57, /// 0xa84
			 0x0ef25b80, /// 0xa88
			 0x5f636604, /// 0xa8c
			 0xdd37598f, /// 0xa90
			 0x33be26ee, /// 0xa94
			 0xe5591e45, /// 0xa98
			 0x4aab3c6f, /// 0xa9c
			 0xb34c4459, /// 0xaa0
			 0xdc01ad11, /// 0xaa4
			 0x69598718, /// 0xaa8
			 0xc19007df, /// 0xaac
			 0x7437b57d, /// 0xab0
			 0x1d774321, /// 0xab4
			 0xdbfa094a, /// 0xab8
			 0xc0c40024, /// 0xabc
			 0x3178a850, /// 0xac0
			 0x3a328090, /// 0xac4
			 0x732126ba, /// 0xac8
			 0x922039e0, /// 0xacc
			 0xa7ec7aff, /// 0xad0
			 0xb1d8f123, /// 0xad4
			 0x10f84a6f, /// 0xad8
			 0x5e2dcf15, /// 0xadc
			 0x4b924b6d, /// 0xae0
			 0x706aa2ec, /// 0xae4
			 0xa1e308bd, /// 0xae8
			 0x66712fea, /// 0xaec
			 0xe0d8a0d8, /// 0xaf0
			 0xad362199, /// 0xaf4
			 0xefdb6323, /// 0xaf8
			 0xc9630c23, /// 0xafc
			 0x82fc9c51, /// 0xb00
			 0xd572f09a, /// 0xb04
			 0xb14dd5e7, /// 0xb08
			 0x69e165b3, /// 0xb0c
			 0x2c403dea, /// 0xb10
			 0xa64f331e, /// 0xb14
			 0xfd451aaa, /// 0xb18
			 0xe42a4963, /// 0xb1c
			 0xeeb637f0, /// 0xb20
			 0x1152ea0e, /// 0xb24
			 0xb4f4a838, /// 0xb28
			 0x86ede7f6, /// 0xb2c
			 0x73dfa176, /// 0xb30
			 0x93e014dd, /// 0xb34
			 0xde8eb636, /// 0xb38
			 0x6f8f0140, /// 0xb3c
			 0x9b8c7670, /// 0xb40
			 0x57ef9607, /// 0xb44
			 0x66048fb5, /// 0xb48
			 0x8170d962, /// 0xb4c
			 0xa3063fa1, /// 0xb50
			 0x4bc6152e, /// 0xb54
			 0x75c15c62, /// 0xb58
			 0x56fde563, /// 0xb5c
			 0x79bc165c, /// 0xb60
			 0x593fffd8, /// 0xb64
			 0x1473653a, /// 0xb68
			 0xc380dead, /// 0xb6c
			 0x747402a3, /// 0xb70
			 0x1d988db1, /// 0xb74
			 0xda830a50, /// 0xb78
			 0x0d557006, /// 0xb7c
			 0x08968c08, /// 0xb80
			 0xe606724e, /// 0xb84
			 0x37b140df, /// 0xb88
			 0x26c202b0, /// 0xb8c
			 0xcc04e0d6, /// 0xb90
			 0x30066725, /// 0xb94
			 0x05b11d49, /// 0xb98
			 0x7f52e284, /// 0xb9c
			 0x38a0c9ea, /// 0xba0
			 0xe997164a, /// 0xba4
			 0x6b3776a6, /// 0xba8
			 0x032d0090, /// 0xbac
			 0xd6211dd2, /// 0xbb0
			 0x652d3e6e, /// 0xbb4
			 0x2e30aec3, /// 0xbb8
			 0xc0a5a83e, /// 0xbbc
			 0xda80cc7d, /// 0xbc0
			 0x6d5bf364, /// 0xbc4
			 0x622a5cb6, /// 0xbc8
			 0xbc0742cc, /// 0xbcc
			 0x095c3be7, /// 0xbd0
			 0xadcf11f9, /// 0xbd4
			 0x4a55527e, /// 0xbd8
			 0x77c736fa, /// 0xbdc
			 0x64b99dd2, /// 0xbe0
			 0xcb247339, /// 0xbe4
			 0x6121c5bf, /// 0xbe8
			 0x0a944cc6, /// 0xbec
			 0x8cf31cae, /// 0xbf0
			 0x61f618d4, /// 0xbf4
			 0xe9cd8d39, /// 0xbf8
			 0x70cdddb9, /// 0xbfc
			 0x2a0c9bbe, /// 0xc00
			 0x1b0811b9, /// 0xc04
			 0x4b29ff21, /// 0xc08
			 0x6364c581, /// 0xc0c
			 0xf19aa934, /// 0xc10
			 0x28ba034b, /// 0xc14
			 0x880e1754, /// 0xc18
			 0x2bd2cf51, /// 0xc1c
			 0xdad0882a, /// 0xc20
			 0x2482b3ec, /// 0xc24
			 0x522b0b50, /// 0xc28
			 0x856478db, /// 0xc2c
			 0xbe999859, /// 0xc30
			 0x4440ea16, /// 0xc34
			 0xafdcec21, /// 0xc38
			 0xc8281067, /// 0xc3c
			 0x80b755ea, /// 0xc40
			 0xe590363c, /// 0xc44
			 0x5679a011, /// 0xc48
			 0x792eb79a, /// 0xc4c
			 0x95a9b022, /// 0xc50
			 0xa87fcad5, /// 0xc54
			 0x48335750, /// 0xc58
			 0xf1ff2862, /// 0xc5c
			 0x54046692, /// 0xc60
			 0x9ed38383, /// 0xc64
			 0x13594dc2, /// 0xc68
			 0x2bf7e766, /// 0xc6c
			 0x8ab931c6, /// 0xc70
			 0x35b9e5f3, /// 0xc74
			 0xe41141c4, /// 0xc78
			 0x253c2865, /// 0xc7c
			 0x4626fa32, /// 0xc80
			 0x0075226b, /// 0xc84
			 0xdcb00ee6, /// 0xc88
			 0xdd5cac9c, /// 0xc8c
			 0x45441684, /// 0xc90
			 0x61b5afcd, /// 0xc94
			 0xc3591350, /// 0xc98
			 0x25537b4a, /// 0xc9c
			 0x5977516f, /// 0xca0
			 0x49f4b089, /// 0xca4
			 0x713fa1aa, /// 0xca8
			 0x1b237c1c, /// 0xcac
			 0xc47fb4a4, /// 0xcb0
			 0xe1966df7, /// 0xcb4
			 0x6be5f1ac, /// 0xcb8
			 0xcef48e82, /// 0xcbc
			 0xbf57db07, /// 0xcc0
			 0x5606e774, /// 0xcc4
			 0x854d65f1, /// 0xcc8
			 0xe45d766c, /// 0xccc
			 0xb65a76c3, /// 0xcd0
			 0x057f08c4, /// 0xcd4
			 0xfefa7c2e, /// 0xcd8
			 0x2a4a71ba, /// 0xcdc
			 0x3ea8141e, /// 0xce0
			 0xcf03f887, /// 0xce4
			 0x273fe3a6, /// 0xce8
			 0xeaaa97a7, /// 0xcec
			 0x347f879a, /// 0xcf0
			 0x0d3cf3c1, /// 0xcf4
			 0x3a3eb5f6, /// 0xcf8
			 0xee12deaa, /// 0xcfc
			 0x0ad252c0, /// 0xd00
			 0x6c907b43, /// 0xd04
			 0xe62f063d, /// 0xd08
			 0x7dd3a83a, /// 0xd0c
			 0x2ffa0e0b, /// 0xd10
			 0x5d2d2677, /// 0xd14
			 0x28db45eb, /// 0xd18
			 0xb0d2110f, /// 0xd1c
			 0x99049288, /// 0xd20
			 0xb88a70d9, /// 0xd24
			 0x11632a37, /// 0xd28
			 0xfb920c8b, /// 0xd2c
			 0x541446ce, /// 0xd30
			 0x7dce3b71, /// 0xd34
			 0x254ab02e, /// 0xd38
			 0x5977f9f0, /// 0xd3c
			 0x6923d092, /// 0xd40
			 0x0ec64e35, /// 0xd44
			 0xbb9cd013, /// 0xd48
			 0xf9fe38fb, /// 0xd4c
			 0xb8143d28, /// 0xd50
			 0x8cd75c62, /// 0xd54
			 0x58b22e5b, /// 0xd58
			 0x257d1c81, /// 0xd5c
			 0x3ed62fd9, /// 0xd60
			 0x551407e0, /// 0xd64
			 0xc4fba54d, /// 0xd68
			 0xf16ebce9, /// 0xd6c
			 0x8bc296a8, /// 0xd70
			 0x7587a154, /// 0xd74
			 0xee90b5ae, /// 0xd78
			 0x3bccec27, /// 0xd7c
			 0x2f80195f, /// 0xd80
			 0x406a80f2, /// 0xd84
			 0x636346cd, /// 0xd88
			 0x6476207e, /// 0xd8c
			 0x07b6b356, /// 0xd90
			 0x07133375, /// 0xd94
			 0xe13edd1e, /// 0xd98
			 0xc92c5be4, /// 0xd9c
			 0x445ee02b, /// 0xda0
			 0xe8730a91, /// 0xda4
			 0x4160f1a3, /// 0xda8
			 0x1fdebb01, /// 0xdac
			 0xab04180d, /// 0xdb0
			 0xdc2c80a9, /// 0xdb4
			 0x5c5ce58b, /// 0xdb8
			 0xad53f7e9, /// 0xdbc
			 0x1b7f32d4, /// 0xdc0
			 0x4337c4ce, /// 0xdc4
			 0xfb8c767a, /// 0xdc8
			 0x95e25be9, /// 0xdcc
			 0x2a39a87d, /// 0xdd0
			 0x791ad205, /// 0xdd4
			 0xcaa12679, /// 0xdd8
			 0x90d5afef, /// 0xddc
			 0x34f0ba07, /// 0xde0
			 0x4eef9564, /// 0xde4
			 0xe46e1771, /// 0xde8
			 0x1c45170d, /// 0xdec
			 0x893824ef, /// 0xdf0
			 0x0568850f, /// 0xdf4
			 0x66cb42a5, /// 0xdf8
			 0x002d72b8, /// 0xdfc
			 0x7bc644a9, /// 0xe00
			 0x8162f974, /// 0xe04
			 0xdf97dbe8, /// 0xe08
			 0xd422a4ba, /// 0xe0c
			 0x87086bcc, /// 0xe10
			 0xd2b83657, /// 0xe14
			 0x4753b135, /// 0xe18
			 0xf26ea293, /// 0xe1c
			 0xd9a82846, /// 0xe20
			 0x9d2c4735, /// 0xe24
			 0xbc4780bf, /// 0xe28
			 0xa5bfaa14, /// 0xe2c
			 0x4ec0b2aa, /// 0xe30
			 0x07ff484a, /// 0xe34
			 0x143943e7, /// 0xe38
			 0xfaed1110, /// 0xe3c
			 0x873e071d, /// 0xe40
			 0x02e4e26a, /// 0xe44
			 0x9e165f35, /// 0xe48
			 0x476a9cdd, /// 0xe4c
			 0x1c87e508, /// 0xe50
			 0x2fb86424, /// 0xe54
			 0x03e48462, /// 0xe58
			 0x8f5fceae, /// 0xe5c
			 0x4ec2876b, /// 0xe60
			 0x4265d3c6, /// 0xe64
			 0x3ec7cce8, /// 0xe68
			 0xcde94ee0, /// 0xe6c
			 0x8f984685, /// 0xe70
			 0x7ce25329, /// 0xe74
			 0xe29fc9ec, /// 0xe78
			 0x7cf24fd9, /// 0xe7c
			 0x4f8af7e8, /// 0xe80
			 0x05b80b6b, /// 0xe84
			 0x9f13c55c, /// 0xe88
			 0x59621969, /// 0xe8c
			 0xa2f986b9, /// 0xe90
			 0x97e5331f, /// 0xe94
			 0xd4d6d8e2, /// 0xe98
			 0x3dffc2f9, /// 0xe9c
			 0xe025deca, /// 0xea0
			 0x16a13330, /// 0xea4
			 0x6d976587, /// 0xea8
			 0x10a57b41, /// 0xeac
			 0xd41a0a83, /// 0xeb0
			 0x9ab50283, /// 0xeb4
			 0x20866b50, /// 0xeb8
			 0x4ff55828, /// 0xebc
			 0x7ae5d118, /// 0xec0
			 0x888759e4, /// 0xec4
			 0x39c0befc, /// 0xec8
			 0xe2b4534e, /// 0xecc
			 0x6e9241bb, /// 0xed0
			 0x505e34a2, /// 0xed4
			 0x072f5f77, /// 0xed8
			 0x01d85ca2, /// 0xedc
			 0x68b65322, /// 0xee0
			 0x008bb265, /// 0xee4
			 0x2c1f9305, /// 0xee8
			 0x4f7a5724, /// 0xeec
			 0x2500a97d, /// 0xef0
			 0xfb90a028, /// 0xef4
			 0x5c4b0bfd, /// 0xef8
			 0xe73e2077, /// 0xefc
			 0x08398945, /// 0xf00
			 0xb95f0c6b, /// 0xf04
			 0xd8597e57, /// 0xf08
			 0xc1e1290b, /// 0xf0c
			 0x774bf21c, /// 0xf10
			 0x1def6e1f, /// 0xf14
			 0x52d3f4ad, /// 0xf18
			 0x680f306c, /// 0xf1c
			 0x4c531997, /// 0xf20
			 0x8a6e4c32, /// 0xf24
			 0xc1a8d885, /// 0xf28
			 0xfc25c290, /// 0xf2c
			 0xbdcf6dde, /// 0xf30
			 0x7d3f1d05, /// 0xf34
			 0x6fdfa52f, /// 0xf38
			 0x06842fd0, /// 0xf3c
			 0x9a51dbc4, /// 0xf40
			 0x3ce0a10b, /// 0xf44
			 0x46b23a7a, /// 0xf48
			 0x4671b56a, /// 0xf4c
			 0x98fcc89e, /// 0xf50
			 0x332efe8a, /// 0xf54
			 0xc1e0de51, /// 0xf58
			 0x15846a46, /// 0xf5c
			 0xf8cc8256, /// 0xf60
			 0x6b754b4b, /// 0xf64
			 0xf8b7c4b3, /// 0xf68
			 0xad673b18, /// 0xf6c
			 0x9828a3bd, /// 0xf70
			 0x576e2720, /// 0xf74
			 0x0dea479f, /// 0xf78
			 0xe22651e3, /// 0xf7c
			 0xf14b87df, /// 0xf80
			 0x294f19e8, /// 0xf84
			 0x8ee11a9a, /// 0xf88
			 0xa143defd, /// 0xf8c
			 0x4a01b125, /// 0xf90
			 0xe33e4688, /// 0xf94
			 0xf22daf01, /// 0xf98
			 0x4f8402a7, /// 0xf9c
			 0xf95a0a87, /// 0xfa0
			 0x46bfa4cf, /// 0xfa4
			 0xce767533, /// 0xfa8
			 0xf0b261b9, /// 0xfac
			 0x688e8718, /// 0xfb0
			 0x32d9eb67, /// 0xfb4
			 0x4cc7e432, /// 0xfb8
			 0xa5e1b472, /// 0xfbc
			 0x12e9792f, /// 0xfc0
			 0xe5c57853, /// 0xfc4
			 0xf6d2f41d, /// 0xfc8
			 0x68396a71, /// 0xfcc
			 0x13953261, /// 0xfd0
			 0xa2e0b2b3, /// 0xfd4
			 0x8e636366, /// 0xfd8
			 0xda2a3d8c, /// 0xfdc
			 0x3f4af78d, /// 0xfe0
			 0xaf779b91, /// 0xfe4
			 0xb8534710, /// 0xfe8
			 0x85a3d496, /// 0xfec
			 0x23a42fd7, /// 0xff0
			 0x3d0e9db1, /// 0xff4
			 0xe50f6aa7, /// 0xff8
			 0x62d331ef /// last
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
			 0x000007c8,
			 0x00000738,
			 0x000003f0,
			 0x00000218,
			 0x00000070,
			 0x000001e0,
			 0x000000ac,
			 0x00000234,

			 /// vpu register f2
			 0x40400000,
			 0x41c80000,
			 0x41900000,
			 0x40000000,
			 0x41900000,
			 0x40800000,
			 0x41d80000,
			 0x41e00000,

			 /// vpu register f3
			 0x41300000,
			 0x41c80000,
			 0x41c80000,
			 0x41c00000,
			 0x41b00000,
			 0x40400000,
			 0x41200000,
			 0x41600000,

			 /// vpu register f4
			 0x41b80000,
			 0x41300000,
			 0x41880000,
			 0x40e00000,
			 0x40000000,
			 0x41e00000,
			 0x41600000,
			 0x41b80000,

			 /// vpu register f5
			 0x40400000,
			 0x41300000,
			 0x41f80000,
			 0x41200000,
			 0x41000000,
			 0x3f800000,
			 0x40e00000,
			 0x41c00000,

			 /// vpu register f6
			 0x40c00000,
			 0x40400000,
			 0x41200000,
			 0x41a00000,
			 0x41f00000,
			 0x41f80000,
			 0x41000000,
			 0x41800000,

			 /// vpu register f7
			 0x3f800000,
			 0x41c80000,
			 0x40800000,
			 0x41b80000,
			 0x41100000,
			 0x41a80000,
			 0x40c00000,
			 0x41a00000,

			 /// vpu register f8
			 0x40800000,
			 0x41600000,
			 0x41d80000,
			 0x41300000,
			 0x41e00000,
			 0x41980000,
			 0x41b00000,
			 0x41100000,

			 /// vpu register f9
			 0x40000000,
			 0x40c00000,
			 0x42000000,
			 0x40c00000,
			 0x41000000,
			 0x41980000,
			 0x41f00000,
			 0x40e00000,

			 /// vpu register f10
			 0x41e80000,
			 0x41500000,
			 0x41000000,
			 0x41100000,
			 0x41f00000,
			 0x41c80000,
			 0x41700000,
			 0x41700000,

			 /// vpu register f11
			 0x41800000,
			 0x41d80000,
			 0x41e00000,
			 0x41400000,
			 0x41880000,
			 0x41700000,
			 0x40e00000,
			 0x40000000,

			 /// vpu register f12
			 0x41500000,
			 0x41980000,
			 0x40a00000,
			 0x41980000,
			 0x40800000,
			 0x41c80000,
			 0x41e80000,
			 0x41e00000,

			 /// vpu register f13
			 0x41f00000,
			 0x41c80000,
			 0x40000000,
			 0x40800000,
			 0x40e00000,
			 0x41c80000,
			 0x41e00000,
			 0x41300000,

			 /// vpu register f14
			 0x41800000,
			 0x41d80000,
			 0x41c80000,
			 0x41800000,
			 0x41800000,
			 0x41c80000,
			 0x40a00000,
			 0x40a00000,

			 /// vpu register f15
			 0x41500000,
			 0x41e00000,
			 0x41b00000,
			 0x41500000,
			 0x3f800000,
			 0x41b80000,
			 0x41300000,
			 0x41a80000,

			 /// vpu register f16
			 0x41100000,
			 0x40800000,
			 0x41880000,
			 0x41e00000,
			 0x41400000,
			 0x41a00000,
			 0x41e80000,
			 0x41980000,

			 /// vpu register f17
			 0x40000000,
			 0x41c80000,
			 0x41100000,
			 0x41000000,
			 0x40a00000,
			 0x41c00000,
			 0x41d80000,
			 0x40000000,

			 /// vpu register f18
			 0x40000000,
			 0x41c80000,
			 0x41d80000,
			 0x41880000,
			 0x3f800000,
			 0x40e00000,
			 0x40000000,
			 0x41200000,

			 /// vpu register f19
			 0x41880000,
			 0x41a80000,
			 0x40400000,
			 0x41900000,
			 0x40000000,
			 0x41d80000,
			 0x42000000,
			 0x41c80000,

			 /// vpu register f20
			 0x41500000,
			 0x41b80000,
			 0x40000000,
			 0x40e00000,
			 0x41900000,
			 0x41400000,
			 0x41a80000,
			 0x41980000,

			 /// vpu register f21
			 0x41d80000,
			 0x41d00000,
			 0x40000000,
			 0x41c00000,
			 0x41880000,
			 0x40e00000,
			 0x41400000,
			 0x41d00000,

			 /// vpu register f22
			 0x41c80000,
			 0x41000000,
			 0x40a00000,
			 0x41b00000,
			 0x41d80000,
			 0x41800000,
			 0x41d80000,
			 0x40000000,

			 /// vpu register f23
			 0x40400000,
			 0x3f800000,
			 0x41b80000,
			 0x41e00000,
			 0x40c00000,
			 0x41a00000,
			 0x41b80000,
			 0x40a00000,

			 /// vpu register f24
			 0x41b00000,
			 0x41100000,
			 0x40e00000,
			 0x41f00000,
			 0x41000000,
			 0x41300000,
			 0x3f800000,
			 0x41d80000,

			 /// vpu register f25
			 0x41980000,
			 0x41b00000,
			 0x40a00000,
			 0x40800000,
			 0x41800000,
			 0x40400000,
			 0x41700000,
			 0x41300000,

			 /// vpu register f26
			 0x40a00000,
			 0x41a80000,
			 0x40400000,
			 0x40a00000,
			 0x41980000,
			 0x41a80000,
			 0x41900000,
			 0x41d80000,

			 /// vpu register f27
			 0x41b80000,
			 0x41600000,
			 0x41e00000,
			 0x41a80000,
			 0x41f80000,
			 0x41300000,
			 0x41f80000,
			 0x40000000,

			 /// vpu register f28
			 0x41200000,
			 0x3f800000,
			 0x41c00000,
			 0x40800000,
			 0x41200000,
			 0x41600000,
			 0x41000000,
			 0x41e00000,

			 /// vpu register f29
			 0x41d00000,
			 0x41b00000,
			 0x41f00000,
			 0x41b00000,
			 0x41b00000,
			 0x40c00000,
			 0x41c00000,
			 0x41e00000,

			 /// vpu register f30
			 0x42000000,
			 0x41a80000,
			 0x40e00000,
			 0x40000000,
			 0x41c00000,
			 0x41880000,
			 0x41700000,
			 0x41600000,

			 /// vpu register f31
			 0x41d80000,
			 0x40800000,
			 0x40000000,
			 0x41d80000,
			 0x41500000,
			 0x41000000,
			 0x41f80000,
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
		"nop\n"
		"nop\n"
		"fcvt.s.wu f17, x11, rup\n"                           ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f2, x15, rmm\n"                            ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f12, x13, rne\n"                           ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f13, x11, rdn\n"                           ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f30, x11, rdn\n"                           ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f21, x11, dyn\n"                           ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f19, x15, rdn\n"                           ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f20, x14, rup\n"                           ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f17, x13, rdn\n"                           ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f28, x14, rup\n"                           ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f24, x12, rup\n"                           ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f9, x13, rdn\n"                            ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f21, x14, rne\n"                           ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f23, x12, dyn\n"                           ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f17, x12, dyn\n"                           ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f20, x13, rmm\n"                           ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f20, x14, rtz\n"                           ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f26, x12, dyn\n"                           ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f13, x13, rdn\n"                           ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f17, x11, rtz\n"                           ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f6, x12, rup\n"                            ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f12, x13, rup\n"                           ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f12, x14, rne\n"                           ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f4, x12, rdn\n"                            ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f2, x13, rup\n"                            ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f19, x15, rdn\n"                           ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f19, x15, rne\n"                           ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f30, x12, rdn\n"                           ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f12, x14, rmm\n"                           ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f2, x14, rtz\n"                            ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f21, x14, rne\n"                           ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f26, x15, rne\n"                           ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f19, x12, rup\n"                           ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f15, x13, dyn\n"                           ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f7, x12, rtz\n"                            ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f7, x14, dyn\n"                            ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f14, x12, rmm\n"                           ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f29, x14, rup\n"                           ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f23, x12, rmm\n"                           ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f9, x11, rtz\n"                            ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f17, x14, rtz\n"                           ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f18, x11, rtz\n"                           ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f28, x13, rup\n"                           ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f27, x12, dyn\n"                           ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f10, x12, dyn\n"                           ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f16, x15, rup\n"                           ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f30, x13, dyn\n"                           ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f11, x14, rmm\n"                           ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f4, x12, rmm\n"                            ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f3, x11, dyn\n"                            ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f6, x13, dyn\n"                            ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f24, x11, dyn\n"                           ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f13, x13, rne\n"                           ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f12, x13, rup\n"                           ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f26, x14, rmm\n"                           ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f18, x14, rup\n"                           ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f15, x11, rne\n"                           ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f28, x12, rne\n"                           ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f8, x13, rne\n"                            ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f8, x15, rdn\n"                            ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f22, x11, rne\n"                           ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f12, x12, rdn\n"                           ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f30, x12, dyn\n"                           ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f3, x12, rdn\n"                            ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f4, x13, rmm\n"                            ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f18, x13, dyn\n"                           ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f15, x12, rdn\n"                           ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f19, x14, dyn\n"                           ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f13, x11, rmm\n"                           ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f10, x12, rtz\n"                           ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f12, x13, rdn\n"                           ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f25, x11, rmm\n"                           ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f17, x12, rdn\n"                           ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f13, x12, rne\n"                           ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f30, x14, rmm\n"                           ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f21, x12, rdn\n"                           ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f7, x15, rtz\n"                            ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f18, x14, rtz\n"                           ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f9, x12, rne\n"                            ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f24, x11, rtz\n"                           ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f27, x15, rne\n"                           ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f23, x11, rmm\n"                           ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f19, x11, rne\n"                           ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f3, x11, rdn\n"                            ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f23, x15, rdn\n"                           ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f24, x14, rne\n"                           ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f16, x15, rne\n"                           ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f26, x12, rne\n"                           ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f10, x12, rdn\n"                           ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f12, x11, rup\n"                           ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f11, x14, rtz\n"                           ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f7, x15, rmm\n"                            ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f16, x15, dyn\n"                           ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f16, x12, dyn\n"                           ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f17, x11, rtz\n"                           ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f14, x13, dyn\n"                           ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f23, x13, dyn\n"                           ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f20, x13, rmm\n"                           ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.s.wu f23, x15, rne\n"                           ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
