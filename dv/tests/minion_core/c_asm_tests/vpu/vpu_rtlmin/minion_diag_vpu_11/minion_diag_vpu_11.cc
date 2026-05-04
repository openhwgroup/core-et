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
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00008
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0000c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00010
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00014
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00018
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00020
			 0x0c700000,                                                  // Leading 1s:                                 /// 00024
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00028
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0002c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00038
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0003c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00040
			 0x55555555,                                                  // 4 random values                             /// 00044
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00048
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00050
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00058
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0005c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00060
			 0x0c780000,                                                  // Leading 1s:                                 /// 00064
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0006c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00074
			 0x0c780000,                                                  // Leading 1s:                                 /// 00078
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0007c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00080
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00084
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00088
			 0x33333333,                                                  // 4 random values                             /// 0008c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00090
			 0x0c400000,                                                  // Leading 1s:                                 /// 00094
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0009c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x4b000000,                                                  // 8388608.0                                   /// 000b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000bc
			 0x7f800000,                                                  // inf                                         /// 000c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000f0
			 0xcb000000,                                                  // -8388608.0                                  /// 000f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00100
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00104
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00108
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00110
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00114
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0011c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00120
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x00011111,                                                  // 9.7958E-41                                  /// 0012c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00130
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00134
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00138
			 0x80011111,                                                  // -9.7958E-41                                 /// 0013c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00140
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00144
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00148
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0014c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00150
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00154
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00158
			 0x0e000007,                                                  // Trailing 1s:                                /// 0015c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00160
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00164
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0016c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00170
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00174
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00178
			 0x00011111,                                                  // 9.7958E-41                                  /// 0017c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00188
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0018c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00190
			 0xcb000000,                                                  // -8388608.0                                  /// 00194
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00198
			 0x7f800000,                                                  // inf                                         /// 0019c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001e0
			 0xcb000000,                                                  // -8388608.0                                  /// 001e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 001e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001ec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00204
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00208
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0020c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00210
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00214
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00218
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0021c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00220
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00224
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00234
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00238
			 0x0c400000,                                                  // Leading 1s:                                 /// 0023c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00240
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00248
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0024c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00254
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00258
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00264
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0026c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00270
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00278
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0xbf028f5c,                                                  // -0.51                                       /// 00280
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00284
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00288
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00290
			 0xcb000000,                                                  // -8388608.0                                  /// 00294
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00298
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0029c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0xff800000,                                                  // -inf                                        /// 002d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00300
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0030c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00310
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00314
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00318
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00320
			 0x80011111,                                                  // -9.7958E-41                                 /// 00324
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00328
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0032c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00338
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0033c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00340
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00344
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00348
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00350
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00354
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0035c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00364
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00370
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00374
			 0x0e000007,                                                  // Trailing 1s:                                /// 00378
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0037c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00388
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0038c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00390
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00394
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0039c
			 0x00000000,                                                  // zero                                        /// 003a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 003d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 003f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0040c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00414
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00418
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00428
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00434
			 0xff800000,                                                  // -inf                                        /// 00438
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00440
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00444
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0044c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00458
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x00000000,                                                  // zero                                        /// 00460
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00464
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00478
			 0x0e000007,                                                  // Trailing 1s:                                /// 0047c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00480
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00484
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00488
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0048c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00490
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00494
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00498
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0049c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004b8
			 0x55555555,                                                  // 4 random values                             /// 004bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004c4
			 0x80000000,                                                  // -zero                                       /// 004c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00500
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0050c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00514
			 0x55555555,                                                  // 4 random values                             /// 00518
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0051c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0052c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00530
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x7f800000,                                                  // inf                                         /// 00540
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00548
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0054c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00550
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00554
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00558
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00570
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0e000003,                                                  // Trailing 1s:                                /// 00578
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00584
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00588
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0058c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00594
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00598
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0059c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005ac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 005d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005e4
			 0x7fc00001,                                                  // signaling NaN                               /// 005e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00600
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0060c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00610
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00614
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00618
			 0xffc00001,                                                  // -signaling NaN                              /// 0061c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00624
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00628
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00630
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00638
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0063c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00640
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00644
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00648
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0064c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00650
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00654
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00658
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0065c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0066c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00680
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00684
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00690
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00694
			 0x4b000000,                                                  // 8388608.0                                   /// 00698
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0069c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006a0
			 0x3f028f5c,                                                  // 0.51                                        /// 006a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006c0
			 0x7fc00001,                                                  // signaling NaN                               /// 006c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 006e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00700
			 0x4b000000,                                                  // 8388608.0                                   /// 00704
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00714
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0071c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00720
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00728
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00734
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00738
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0073c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00740
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00744
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00748
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0074c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00750
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00754
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0075c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00768
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0076c
			 0x3f028f5c,                                                  // 0.51                                        /// 00770
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00774
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00778
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0077c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00780
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0c780000,                                                  // Leading 1s:                                 /// 00788
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0078c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00790
			 0xff800000,                                                  // -inf                                        /// 00794
			 0x0e000003,                                                  // Trailing 1s:                                /// 00798
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0079c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00800
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0080c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00814
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0081c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00820
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00824
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00828
			 0x80000000,                                                  // -zero                                       /// 0082c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00830
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x0c780000,                                                  // Leading 1s:                                 /// 0083c
			 0xffc00001,                                                  // -signaling NaN                              /// 00840
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00844
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00848
			 0x0e000001,                                                  // Trailing 1s:                                /// 0084c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00850
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00854
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0085c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00860
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00864
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00868
			 0xbf028f5c,                                                  // -0.51                                       /// 0086c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00870
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00874
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00878
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0087c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x55555555,                                                  // 4 random values                             /// 00884
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00888
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x33333333,                                                  // 4 random values                             /// 00898
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0089c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 008bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00900
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00904
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00908
			 0x80011111,                                                  // -9.7958E-41                                 /// 0090c
			 0x7f800000,                                                  // inf                                         /// 00910
			 0x0c700000,                                                  // Leading 1s:                                 /// 00914
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00920
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00928
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0092c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00930
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00934
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00938
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00940
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00944
			 0x0e000003,                                                  // Trailing 1s:                                /// 00948
			 0x0c600000,                                                  // Leading 1s:                                 /// 0094c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00950
			 0x33333333,                                                  // 4 random values                             /// 00954
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00958
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0095c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00960
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00964
			 0x80011111,                                                  // -9.7958E-41                                 /// 00968
			 0x00011111,                                                  // 9.7958E-41                                  /// 0096c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00970
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00974
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00978
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0097c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0c700000,                                                  // Leading 1s:                                 /// 00984
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00988
			 0x55555555,                                                  // 4 random values                             /// 0098c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00990
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00994
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00998
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x0e000007,                                                  // Trailing 1s:                                /// 009a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x7f800000,                                                  // inf                                         /// 009b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a00
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a0c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a14
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a1c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a34
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a44
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a48
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a4c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a50
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a64
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x33333333,                                                  // 4 random values                             /// 00a70
			 0x7fc00001,                                                  // signaling NaN                               /// 00a74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a78
			 0x80000000,                                                  // -zero                                       /// 00a7c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a84
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a88
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a90
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a9c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00aa0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00aac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ab0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ab4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ab8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ac0
			 0x33333333,                                                  // 4 random values                             /// 00ac4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ac8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ad4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00adc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ae0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ae4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ae8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00af0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00af4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00afc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b04
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b10
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b18
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b1c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b24
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b2c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b30
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b34
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b38
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b3c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b54
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b60
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b6c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b80
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b94
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ba0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ba4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bb0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bbc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bc0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bc8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bd0
			 0x4b000000,                                                  // 8388608.0                                   /// 00bd4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bd8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bdc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00be0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00be4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00be8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bf4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c14
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c18
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c28
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c34
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c38
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c44
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c54
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c68
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c70
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c74
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c78
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c80
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c88
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ca4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ca8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cc0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cc4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cc8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cd0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cd4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cd8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cdc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cf0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cf4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cf8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cfc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d04
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d0c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d14
			 0xcb000000,                                                  // -8388608.0                                  /// 00d18
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d20
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d24
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d28
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d30
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d34
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d40
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d44
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d48
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d50
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d58
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d74
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00da0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00da4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00da8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00db8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dc0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dc4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dd4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dd8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ddc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00de0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00de4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00df0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00df4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e00
			 0x33333333,                                                  // 4 random values                             /// 00e04
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e08
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e10
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e18
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e1c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e24
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e2c
			 0x00000000,                                                  // zero                                        /// 00e30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e3c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e40
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0xcb000000,                                                  // -8388608.0                                  /// 00e48
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e4c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e68
			 0x00000000,                                                  // zero                                        /// 00e6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e70
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e74
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e88
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0xff800000,                                                  // -inf                                        /// 00ea4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ea8
			 0x7f800000,                                                  // inf                                         /// 00eac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00eb8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ebc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ecc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ed0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ed8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ee0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ee4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00eec
			 0x33333333,                                                  // 4 random values                             /// 00ef0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ef4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ef8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f00
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f0c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f18
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f20
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f3c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f44
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f60
			 0x33333333,                                                  // 4 random values                             /// 00f64
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f74
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f88
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f9c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fa0
			 0x33333333,                                                  // 4 random values                             /// 00fa4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fc0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fc4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fc8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fcc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fd4
			 0xff800000,                                                  // -inf                                        /// 00fd8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fdc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fe4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fe8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ff0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ff8
			 0x7fbfffff // SNaN                                          // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x39dea1e5, /// 0x0
			 0x000da874, /// 0x4
			 0xb7842504, /// 0x8
			 0xc43c06cc, /// 0xc
			 0xdcf21cfc, /// 0x10
			 0xb6054be6, /// 0x14
			 0x55bccdca, /// 0x18
			 0xbad5b28b, /// 0x1c
			 0x10548a56, /// 0x20
			 0x4f32ab4d, /// 0x24
			 0x0aca8042, /// 0x28
			 0x79385206, /// 0x2c
			 0xa4eb8f0c, /// 0x30
			 0xb7ab8e0a, /// 0x34
			 0x23370b85, /// 0x38
			 0x1bbcd08a, /// 0x3c
			 0x9e9295bb, /// 0x40
			 0x6d0ff38e, /// 0x44
			 0x2704c8fa, /// 0x48
			 0xc1af79ca, /// 0x4c
			 0x1609c67a, /// 0x50
			 0x0fd120fd, /// 0x54
			 0xf887f48a, /// 0x58
			 0x31b17bcd, /// 0x5c
			 0xc9c887f3, /// 0x60
			 0x0190d919, /// 0x64
			 0x81bba43a, /// 0x68
			 0x4a925dc6, /// 0x6c
			 0x18a1dca6, /// 0x70
			 0xbce2b091, /// 0x74
			 0x3df18687, /// 0x78
			 0xaf407915, /// 0x7c
			 0x18d83815, /// 0x80
			 0xc8448b17, /// 0x84
			 0xaa042e80, /// 0x88
			 0x6b01bd6a, /// 0x8c
			 0xdd8a2b89, /// 0x90
			 0x59b9c3bc, /// 0x94
			 0x495881fa, /// 0x98
			 0x7f6085ec, /// 0x9c
			 0x85e1c8ea, /// 0xa0
			 0x9538eecf, /// 0xa4
			 0x1d83d963, /// 0xa8
			 0x418c0bfa, /// 0xac
			 0x1308b737, /// 0xb0
			 0x48a58c37, /// 0xb4
			 0xd4537d0f, /// 0xb8
			 0x744be490, /// 0xbc
			 0xb49ad1ef, /// 0xc0
			 0x52534efb, /// 0xc4
			 0x48e33835, /// 0xc8
			 0xdb1a84fa, /// 0xcc
			 0xc419b614, /// 0xd0
			 0xc8daefe3, /// 0xd4
			 0x432981d8, /// 0xd8
			 0x7a17bb9e, /// 0xdc
			 0x5cfb2627, /// 0xe0
			 0x2cd8ee57, /// 0xe4
			 0xdf649a57, /// 0xe8
			 0xfae1ea8d, /// 0xec
			 0x7d49c34d, /// 0xf0
			 0xf4fa8c5d, /// 0xf4
			 0xb2fb66c9, /// 0xf8
			 0x29e0c3af, /// 0xfc
			 0x834d3ff1, /// 0x100
			 0x59434869, /// 0x104
			 0xba4b6b30, /// 0x108
			 0x4f4395ff, /// 0x10c
			 0x698f905c, /// 0x110
			 0xf6d93068, /// 0x114
			 0xe4506f9f, /// 0x118
			 0xba9ee0d6, /// 0x11c
			 0x8ad5da56, /// 0x120
			 0x7943ce16, /// 0x124
			 0xbb512e1a, /// 0x128
			 0xa8546fc0, /// 0x12c
			 0x2ffa1ffc, /// 0x130
			 0xa578f09f, /// 0x134
			 0xaf4d551f, /// 0x138
			 0xb30f0f72, /// 0x13c
			 0x721bd270, /// 0x140
			 0x4ac87f27, /// 0x144
			 0x80259b9e, /// 0x148
			 0x025f760d, /// 0x14c
			 0x1a87c8ea, /// 0x150
			 0xfa260eac, /// 0x154
			 0x1caa9aa6, /// 0x158
			 0x10abd747, /// 0x15c
			 0x9229eee2, /// 0x160
			 0x632cf5f0, /// 0x164
			 0x093b7a66, /// 0x168
			 0xfa254459, /// 0x16c
			 0x47324b6a, /// 0x170
			 0x424649d7, /// 0x174
			 0x42dfec83, /// 0x178
			 0x12dabb79, /// 0x17c
			 0x6f3ea070, /// 0x180
			 0x8fe7fc25, /// 0x184
			 0xedef825e, /// 0x188
			 0x6e10df19, /// 0x18c
			 0x6768700a, /// 0x190
			 0xc4c6d8cf, /// 0x194
			 0x84a32875, /// 0x198
			 0x041fee5f, /// 0x19c
			 0xfcb1af9e, /// 0x1a0
			 0x5b55f02d, /// 0x1a4
			 0x0a42e0ee, /// 0x1a8
			 0xc08424ad, /// 0x1ac
			 0xf70089e9, /// 0x1b0
			 0x33f9dba2, /// 0x1b4
			 0xb5674f93, /// 0x1b8
			 0x741134d6, /// 0x1bc
			 0x1b5812cc, /// 0x1c0
			 0x6ab70c5d, /// 0x1c4
			 0x86eb7664, /// 0x1c8
			 0x68eae837, /// 0x1cc
			 0xd4d1086d, /// 0x1d0
			 0x5e6f06cd, /// 0x1d4
			 0xb3075791, /// 0x1d8
			 0xb48376f3, /// 0x1dc
			 0x6a6fff5a, /// 0x1e0
			 0xec87ccd6, /// 0x1e4
			 0xcf4a5171, /// 0x1e8
			 0x616f8c7d, /// 0x1ec
			 0x27ba15ec, /// 0x1f0
			 0xef3a0fd9, /// 0x1f4
			 0x63e49ba4, /// 0x1f8
			 0x8cc2f7b3, /// 0x1fc
			 0x3a249095, /// 0x200
			 0x0d933715, /// 0x204
			 0xbc86abf8, /// 0x208
			 0x8f282bca, /// 0x20c
			 0xb143d075, /// 0x210
			 0xa9600d02, /// 0x214
			 0x8bafbc13, /// 0x218
			 0xd94f96aa, /// 0x21c
			 0x7cea0a55, /// 0x220
			 0x8443fd6a, /// 0x224
			 0x6e114475, /// 0x228
			 0x2322d86e, /// 0x22c
			 0xb44f9e6e, /// 0x230
			 0x3f8d14c3, /// 0x234
			 0x506e4063, /// 0x238
			 0x68663a48, /// 0x23c
			 0xf24a709a, /// 0x240
			 0x49001b48, /// 0x244
			 0x7817d5b3, /// 0x248
			 0x4bbdb833, /// 0x24c
			 0xd65aa8b6, /// 0x250
			 0xf2dfac6a, /// 0x254
			 0xa1e586fc, /// 0x258
			 0x119e76f1, /// 0x25c
			 0xb4dcbed2, /// 0x260
			 0x968b6d2c, /// 0x264
			 0xc7eb69dd, /// 0x268
			 0xe0ad342a, /// 0x26c
			 0x152a305a, /// 0x270
			 0x9d7321da, /// 0x274
			 0x11020fbd, /// 0x278
			 0x63d20e71, /// 0x27c
			 0x3c804880, /// 0x280
			 0xc3a9dcc3, /// 0x284
			 0x49765b2d, /// 0x288
			 0x80236207, /// 0x28c
			 0xa1baed7f, /// 0x290
			 0xa9310049, /// 0x294
			 0xd5de9ef4, /// 0x298
			 0x291a76f7, /// 0x29c
			 0x6e08b41d, /// 0x2a0
			 0x35de022b, /// 0x2a4
			 0x289c9060, /// 0x2a8
			 0x951579d5, /// 0x2ac
			 0xf6513ae9, /// 0x2b0
			 0x282f0da2, /// 0x2b4
			 0x29f40467, /// 0x2b8
			 0x555bdf18, /// 0x2bc
			 0xe5f5defd, /// 0x2c0
			 0x4717ac17, /// 0x2c4
			 0x8afccf2a, /// 0x2c8
			 0x7d8e810f, /// 0x2cc
			 0xa96e83f0, /// 0x2d0
			 0xe5aed0bc, /// 0x2d4
			 0x3d231408, /// 0x2d8
			 0xcdd42bd1, /// 0x2dc
			 0x66c07dac, /// 0x2e0
			 0x1a7df85b, /// 0x2e4
			 0xcbad9bcf, /// 0x2e8
			 0x712c1e7e, /// 0x2ec
			 0xe9e17a08, /// 0x2f0
			 0x407d1edb, /// 0x2f4
			 0xb26bc8e5, /// 0x2f8
			 0x06291294, /// 0x2fc
			 0x85331c37, /// 0x300
			 0x500541bb, /// 0x304
			 0xe4a95fbb, /// 0x308
			 0xd0ee0f3d, /// 0x30c
			 0x5f46f77e, /// 0x310
			 0x9565e5bb, /// 0x314
			 0x696e44a6, /// 0x318
			 0xc1e23617, /// 0x31c
			 0x350f575a, /// 0x320
			 0x83c02b5a, /// 0x324
			 0x0fab6017, /// 0x328
			 0x3b08ae37, /// 0x32c
			 0x7837781c, /// 0x330
			 0xe529f48e, /// 0x334
			 0xe058d061, /// 0x338
			 0x7511c8dd, /// 0x33c
			 0x10068454, /// 0x340
			 0xde35db74, /// 0x344
			 0x64d008f5, /// 0x348
			 0x46384f60, /// 0x34c
			 0x6b955fc2, /// 0x350
			 0x1ce48d93, /// 0x354
			 0x3929611e, /// 0x358
			 0xbd9205f6, /// 0x35c
			 0xfa0863b7, /// 0x360
			 0x796c6528, /// 0x364
			 0xb6092a8b, /// 0x368
			 0x9b9898fe, /// 0x36c
			 0x814fc1c7, /// 0x370
			 0x364c55bc, /// 0x374
			 0xc42df6c1, /// 0x378
			 0xb376c156, /// 0x37c
			 0x8413cbc2, /// 0x380
			 0x0e51e8dd, /// 0x384
			 0x74d38626, /// 0x388
			 0x1c5afd53, /// 0x38c
			 0x00ed69a7, /// 0x390
			 0xf291c4e5, /// 0x394
			 0xf8dc7f01, /// 0x398
			 0xdc259e8d, /// 0x39c
			 0x4bb7c195, /// 0x3a0
			 0xcc78ba37, /// 0x3a4
			 0x90223592, /// 0x3a8
			 0xfffb9389, /// 0x3ac
			 0xe85cc79c, /// 0x3b0
			 0xcf297db0, /// 0x3b4
			 0x2e02343c, /// 0x3b8
			 0x4f774ab2, /// 0x3bc
			 0x86c3eef1, /// 0x3c0
			 0x11814220, /// 0x3c4
			 0x08797131, /// 0x3c8
			 0x32d9b453, /// 0x3cc
			 0xb2261340, /// 0x3d0
			 0xf1dab30f, /// 0x3d4
			 0x521bdd4a, /// 0x3d8
			 0xf2728306, /// 0x3dc
			 0xcfa72265, /// 0x3e0
			 0x83d4a027, /// 0x3e4
			 0x50db5e43, /// 0x3e8
			 0x2004fa3f, /// 0x3ec
			 0xdc6e6dcd, /// 0x3f0
			 0xa54646f1, /// 0x3f4
			 0x38edc81e, /// 0x3f8
			 0x516abe43, /// 0x3fc
			 0xb31f8070, /// 0x400
			 0xdc522d7d, /// 0x404
			 0x58c8ba58, /// 0x408
			 0x21860261, /// 0x40c
			 0x44f265d8, /// 0x410
			 0x64be80b7, /// 0x414
			 0xcaafaea1, /// 0x418
			 0x3115ee19, /// 0x41c
			 0x71067762, /// 0x420
			 0x10a9ec46, /// 0x424
			 0x8ea5faf3, /// 0x428
			 0x7759c53e, /// 0x42c
			 0xc432aa07, /// 0x430
			 0x44d39d2d, /// 0x434
			 0x548505df, /// 0x438
			 0xe38d2829, /// 0x43c
			 0x9d81863a, /// 0x440
			 0x864e65b6, /// 0x444
			 0x014480af, /// 0x448
			 0xabd4349a, /// 0x44c
			 0x08b59b10, /// 0x450
			 0x2ac882bf, /// 0x454
			 0x7fd60af6, /// 0x458
			 0xc6a2b2e4, /// 0x45c
			 0x1061c867, /// 0x460
			 0xbab6a00f, /// 0x464
			 0xdd17b6fa, /// 0x468
			 0xbae339eb, /// 0x46c
			 0x95ad37d4, /// 0x470
			 0x2f9c0b2e, /// 0x474
			 0xbbaf9366, /// 0x478
			 0x2916f989, /// 0x47c
			 0xc32a7558, /// 0x480
			 0x2956bf64, /// 0x484
			 0x730007e3, /// 0x488
			 0x5383aefa, /// 0x48c
			 0xbad9d6f1, /// 0x490
			 0xeef0b80e, /// 0x494
			 0x87583b99, /// 0x498
			 0xe5aedd3f, /// 0x49c
			 0xb5880104, /// 0x4a0
			 0xfc1eb40c, /// 0x4a4
			 0xb4877b7c, /// 0x4a8
			 0x0c4b3acb, /// 0x4ac
			 0x1e3a5668, /// 0x4b0
			 0x1cba70d3, /// 0x4b4
			 0x5343d9a3, /// 0x4b8
			 0x3f414a6d, /// 0x4bc
			 0xd08c4fac, /// 0x4c0
			 0x93c108ba, /// 0x4c4
			 0x88aae180, /// 0x4c8
			 0x069bd1bb, /// 0x4cc
			 0xdfd22145, /// 0x4d0
			 0x2ec831a4, /// 0x4d4
			 0x4744071e, /// 0x4d8
			 0xde5c6d5e, /// 0x4dc
			 0xa83f3a08, /// 0x4e0
			 0x319b409f, /// 0x4e4
			 0xad544a40, /// 0x4e8
			 0xa388a52e, /// 0x4ec
			 0x6f03b88e, /// 0x4f0
			 0xce8ad157, /// 0x4f4
			 0x1d53b41a, /// 0x4f8
			 0xafc416ec, /// 0x4fc
			 0x86cbb26b, /// 0x500
			 0x5468bd0f, /// 0x504
			 0x789e81ec, /// 0x508
			 0xadf3528b, /// 0x50c
			 0x75d86f63, /// 0x510
			 0xedaf1d1c, /// 0x514
			 0x8ea92dd5, /// 0x518
			 0x23ebf25a, /// 0x51c
			 0x95fe3df4, /// 0x520
			 0x4c61596d, /// 0x524
			 0x3226b0cd, /// 0x528
			 0xdafbaa1c, /// 0x52c
			 0x8681dbe2, /// 0x530
			 0x2193319a, /// 0x534
			 0xbfb7746e, /// 0x538
			 0x6e928f37, /// 0x53c
			 0x6710a923, /// 0x540
			 0xb0af7474, /// 0x544
			 0xd4494af3, /// 0x548
			 0x5969f460, /// 0x54c
			 0x53d3c060, /// 0x550
			 0x3b7da1a0, /// 0x554
			 0x2b2b6b5d, /// 0x558
			 0xebb66411, /// 0x55c
			 0x480a5fd3, /// 0x560
			 0xcf698f01, /// 0x564
			 0x40934e08, /// 0x568
			 0x8ca99c60, /// 0x56c
			 0x800b1ab2, /// 0x570
			 0xa4a02be9, /// 0x574
			 0x208ec471, /// 0x578
			 0xc2bbaaca, /// 0x57c
			 0x84657b4e, /// 0x580
			 0x7f0fc379, /// 0x584
			 0x79abfe32, /// 0x588
			 0x76c0d1b6, /// 0x58c
			 0x5a8b07c1, /// 0x590
			 0x8fd8478d, /// 0x594
			 0xea8cbfd6, /// 0x598
			 0xb14a5979, /// 0x59c
			 0x8b26f701, /// 0x5a0
			 0x502c39a5, /// 0x5a4
			 0xdcab3df2, /// 0x5a8
			 0x651339b0, /// 0x5ac
			 0x336fb617, /// 0x5b0
			 0x4f4a95f8, /// 0x5b4
			 0x77409890, /// 0x5b8
			 0xd045e105, /// 0x5bc
			 0xce73b5ca, /// 0x5c0
			 0x8086a339, /// 0x5c4
			 0xc01f4496, /// 0x5c8
			 0x8e0f7b41, /// 0x5cc
			 0x06f1b3f4, /// 0x5d0
			 0x8b6cf652, /// 0x5d4
			 0xaa15de33, /// 0x5d8
			 0x27674c25, /// 0x5dc
			 0x1579ec3e, /// 0x5e0
			 0xdde1d959, /// 0x5e4
			 0x8b4766d3, /// 0x5e8
			 0x26fe6e14, /// 0x5ec
			 0x6a7e6f46, /// 0x5f0
			 0x81537edd, /// 0x5f4
			 0x0d1d58f2, /// 0x5f8
			 0x41a5fc7e, /// 0x5fc
			 0x0c8d6cda, /// 0x600
			 0xd5dc445e, /// 0x604
			 0xf2c9df9f, /// 0x608
			 0x182dfe2e, /// 0x60c
			 0x3d3fa0cb, /// 0x610
			 0x2043744d, /// 0x614
			 0x1b0cc917, /// 0x618
			 0x44232eb8, /// 0x61c
			 0x5b23baa0, /// 0x620
			 0xc2a3edfd, /// 0x624
			 0x28e1115b, /// 0x628
			 0xae8c75ff, /// 0x62c
			 0xc336d892, /// 0x630
			 0x323d1597, /// 0x634
			 0x6c24c190, /// 0x638
			 0x84e271c0, /// 0x63c
			 0x76a20def, /// 0x640
			 0x0b5cdb25, /// 0x644
			 0x0c5e8319, /// 0x648
			 0xa3ca077a, /// 0x64c
			 0x834330a2, /// 0x650
			 0x18c372ac, /// 0x654
			 0xb4d9fdc8, /// 0x658
			 0xc4bdeec4, /// 0x65c
			 0x3a23f567, /// 0x660
			 0x67a9f492, /// 0x664
			 0xe28463aa, /// 0x668
			 0x347395df, /// 0x66c
			 0x57c61bd4, /// 0x670
			 0xf4d9ddae, /// 0x674
			 0x4d60d9de, /// 0x678
			 0x5b6f666a, /// 0x67c
			 0xd39c4997, /// 0x680
			 0x2eab9ed4, /// 0x684
			 0x2be03a1d, /// 0x688
			 0x2764c440, /// 0x68c
			 0x66a9c3e9, /// 0x690
			 0x5cbb697e, /// 0x694
			 0xc0771605, /// 0x698
			 0x342b56f4, /// 0x69c
			 0x95a9e7ed, /// 0x6a0
			 0xba8403a4, /// 0x6a4
			 0x5ba30d08, /// 0x6a8
			 0x4b69f90d, /// 0x6ac
			 0x6ce2c393, /// 0x6b0
			 0xadc4ff8e, /// 0x6b4
			 0xd46c751b, /// 0x6b8
			 0x400e75d4, /// 0x6bc
			 0x8c08c32c, /// 0x6c0
			 0x8f483d07, /// 0x6c4
			 0x722d9021, /// 0x6c8
			 0x4d6767ec, /// 0x6cc
			 0x49e0fd06, /// 0x6d0
			 0xe25e5ac0, /// 0x6d4
			 0xd293340a, /// 0x6d8
			 0x126df0b5, /// 0x6dc
			 0x68805ffb, /// 0x6e0
			 0xd58880f5, /// 0x6e4
			 0x27470c08, /// 0x6e8
			 0x5d12e41c, /// 0x6ec
			 0x1109bff4, /// 0x6f0
			 0xaaa1ff00, /// 0x6f4
			 0xdbc1d4c6, /// 0x6f8
			 0x06611d37, /// 0x6fc
			 0xf4980f74, /// 0x700
			 0x925401d1, /// 0x704
			 0x5efefd4c, /// 0x708
			 0x2fdb1dc5, /// 0x70c
			 0xc9555cfd, /// 0x710
			 0x738c8009, /// 0x714
			 0x24046113, /// 0x718
			 0xbadaf965, /// 0x71c
			 0xe1bbc000, /// 0x720
			 0x9dcb6244, /// 0x724
			 0xc2cc618b, /// 0x728
			 0x0eb7afc2, /// 0x72c
			 0x96c5529c, /// 0x730
			 0xe36f13f1, /// 0x734
			 0x5e17d51a, /// 0x738
			 0x2d13ff00, /// 0x73c
			 0x1bc0ccb7, /// 0x740
			 0x0b879845, /// 0x744
			 0x0c74673b, /// 0x748
			 0x54784339, /// 0x74c
			 0x4dbeae65, /// 0x750
			 0xd7b7447e, /// 0x754
			 0x0581220a, /// 0x758
			 0x27e24bd5, /// 0x75c
			 0x3590b2dc, /// 0x760
			 0x7fb4f787, /// 0x764
			 0x6bc0a9ea, /// 0x768
			 0x4205c8b1, /// 0x76c
			 0x839a8b31, /// 0x770
			 0xc6405e42, /// 0x774
			 0xb28f97a4, /// 0x778
			 0x91bd52d9, /// 0x77c
			 0x1510507b, /// 0x780
			 0xb32b8560, /// 0x784
			 0x4bf2a840, /// 0x788
			 0x99baab49, /// 0x78c
			 0x68a02a3b, /// 0x790
			 0xa00bb15a, /// 0x794
			 0x1636fe38, /// 0x798
			 0x4f263ac7, /// 0x79c
			 0xaac4de2e, /// 0x7a0
			 0x968ac48f, /// 0x7a4
			 0x39db9568, /// 0x7a8
			 0x83aa9eec, /// 0x7ac
			 0xeed0dfaa, /// 0x7b0
			 0x06df3c3d, /// 0x7b4
			 0xbd0578fa, /// 0x7b8
			 0x4f58f2b6, /// 0x7bc
			 0x12d9a1f7, /// 0x7c0
			 0x312e5601, /// 0x7c4
			 0xc4c7f00c, /// 0x7c8
			 0x7571b357, /// 0x7cc
			 0xbdc25f51, /// 0x7d0
			 0xb20e4d3f, /// 0x7d4
			 0xae610142, /// 0x7d8
			 0x9b074972, /// 0x7dc
			 0xef0ef6d9, /// 0x7e0
			 0x1e56cf45, /// 0x7e4
			 0x302c465d, /// 0x7e8
			 0x4542aa06, /// 0x7ec
			 0x9fdd07f8, /// 0x7f0
			 0x38814cfd, /// 0x7f4
			 0xfcb9097b, /// 0x7f8
			 0xe2e2bcd1, /// 0x7fc
			 0xcfdc993a, /// 0x800
			 0x82c2f540, /// 0x804
			 0x6ab706ce, /// 0x808
			 0x1957e85e, /// 0x80c
			 0xa8d46930, /// 0x810
			 0xa88d87ae, /// 0x814
			 0x802fae37, /// 0x818
			 0x529b8a17, /// 0x81c
			 0x7d480261, /// 0x820
			 0x68c0197b, /// 0x824
			 0xd4b4573f, /// 0x828
			 0xa001359a, /// 0x82c
			 0x72dc37eb, /// 0x830
			 0xdce08cd1, /// 0x834
			 0xf76d07fa, /// 0x838
			 0xaa10a205, /// 0x83c
			 0x326e1e7d, /// 0x840
			 0xf89869e6, /// 0x844
			 0x0aec06b4, /// 0x848
			 0x49495664, /// 0x84c
			 0x6641e758, /// 0x850
			 0x46a00f1b, /// 0x854
			 0x3ba8799b, /// 0x858
			 0xad62f196, /// 0x85c
			 0xb91f02fc, /// 0x860
			 0x9b457844, /// 0x864
			 0x75ca3943, /// 0x868
			 0xa3c245fe, /// 0x86c
			 0xb7a09eaa, /// 0x870
			 0x4d826c76, /// 0x874
			 0xda60ea07, /// 0x878
			 0x417717f9, /// 0x87c
			 0x8c2c14d1, /// 0x880
			 0x250a53fb, /// 0x884
			 0xd757b547, /// 0x888
			 0x97ef8ea9, /// 0x88c
			 0x569835d1, /// 0x890
			 0xc1603fdd, /// 0x894
			 0x627e937e, /// 0x898
			 0x3a12e117, /// 0x89c
			 0xacf7f096, /// 0x8a0
			 0x7f21937b, /// 0x8a4
			 0x915ad3bb, /// 0x8a8
			 0xf342ba84, /// 0x8ac
			 0x609592e6, /// 0x8b0
			 0x3ad64bcd, /// 0x8b4
			 0xe93cd534, /// 0x8b8
			 0x202e0145, /// 0x8bc
			 0xab6ea726, /// 0x8c0
			 0x47464c98, /// 0x8c4
			 0x6924d777, /// 0x8c8
			 0x28cd1e52, /// 0x8cc
			 0xd611caee, /// 0x8d0
			 0xfaee530c, /// 0x8d4
			 0x842cdc45, /// 0x8d8
			 0x2e0a97d9, /// 0x8dc
			 0x5d3705dc, /// 0x8e0
			 0xc70d836c, /// 0x8e4
			 0xf579b508, /// 0x8e8
			 0x3ecfd773, /// 0x8ec
			 0x8c2613b1, /// 0x8f0
			 0xef4297de, /// 0x8f4
			 0x91b59c70, /// 0x8f8
			 0x701520da, /// 0x8fc
			 0xce675317, /// 0x900
			 0xead728d4, /// 0x904
			 0x75ee02b1, /// 0x908
			 0xd30e007c, /// 0x90c
			 0x21c2d4b8, /// 0x910
			 0xe874c989, /// 0x914
			 0x10028e02, /// 0x918
			 0xd47740b9, /// 0x91c
			 0xb5abd547, /// 0x920
			 0xb33a078d, /// 0x924
			 0xe44c1c28, /// 0x928
			 0x14be46de, /// 0x92c
			 0xa8a96352, /// 0x930
			 0xaf2e2f9c, /// 0x934
			 0xb15a1c5f, /// 0x938
			 0xd1aca023, /// 0x93c
			 0x9a9c9aa2, /// 0x940
			 0x7871067b, /// 0x944
			 0x90f925ae, /// 0x948
			 0xf9f12760, /// 0x94c
			 0x523388d0, /// 0x950
			 0x9b2218aa, /// 0x954
			 0x11f7c803, /// 0x958
			 0x7b3cd3bf, /// 0x95c
			 0x9e1c6562, /// 0x960
			 0x633473fa, /// 0x964
			 0x4c3c461c, /// 0x968
			 0x3b7683a7, /// 0x96c
			 0x4efade3c, /// 0x970
			 0x5c958b94, /// 0x974
			 0xc11fd8bf, /// 0x978
			 0xb7afebf9, /// 0x97c
			 0x059d1d34, /// 0x980
			 0xce2bcf09, /// 0x984
			 0xea647ed7, /// 0x988
			 0xb483cf9a, /// 0x98c
			 0xdfa02d43, /// 0x990
			 0x5eb6e0cd, /// 0x994
			 0xcf8a796b, /// 0x998
			 0x1d7977fc, /// 0x99c
			 0xa3488b0d, /// 0x9a0
			 0xc8946e25, /// 0x9a4
			 0x235f81e0, /// 0x9a8
			 0xcacd72d1, /// 0x9ac
			 0x795aaa0c, /// 0x9b0
			 0xdc4a5088, /// 0x9b4
			 0x490d64b5, /// 0x9b8
			 0xe01736bd, /// 0x9bc
			 0x111f17e1, /// 0x9c0
			 0xe942dbc0, /// 0x9c4
			 0x7e1d9d47, /// 0x9c8
			 0xa6a7b7c6, /// 0x9cc
			 0x4d496843, /// 0x9d0
			 0x6708c7e4, /// 0x9d4
			 0x19e57b0f, /// 0x9d8
			 0x3a5bc443, /// 0x9dc
			 0x50039d32, /// 0x9e0
			 0x619dded2, /// 0x9e4
			 0x98443fb9, /// 0x9e8
			 0xf9915ec6, /// 0x9ec
			 0xc8c5d1b0, /// 0x9f0
			 0xff5e2011, /// 0x9f4
			 0x3f1ccc2e, /// 0x9f8
			 0x28d9f139, /// 0x9fc
			 0xf9f72e38, /// 0xa00
			 0x0a2987c4, /// 0xa04
			 0x1e6af429, /// 0xa08
			 0x5b2b0abf, /// 0xa0c
			 0x01832fe7, /// 0xa10
			 0x1858ff37, /// 0xa14
			 0x04f19419, /// 0xa18
			 0x71460b0f, /// 0xa1c
			 0x38f1959c, /// 0xa20
			 0xa68e320c, /// 0xa24
			 0xcaf55dc0, /// 0xa28
			 0x28c41af4, /// 0xa2c
			 0x66029993, /// 0xa30
			 0x2b630c89, /// 0xa34
			 0xd21c1288, /// 0xa38
			 0x43ea9152, /// 0xa3c
			 0xb4c4f57a, /// 0xa40
			 0x52eba85c, /// 0xa44
			 0x0160c324, /// 0xa48
			 0x21b5fbc4, /// 0xa4c
			 0x967a5421, /// 0xa50
			 0x5134f44e, /// 0xa54
			 0x4cbf67ac, /// 0xa58
			 0x2b43a4c0, /// 0xa5c
			 0x1fb88b33, /// 0xa60
			 0xf523a9b9, /// 0xa64
			 0x4356febc, /// 0xa68
			 0x05abb54e, /// 0xa6c
			 0xf17523ff, /// 0xa70
			 0xe226157c, /// 0xa74
			 0x58bd6dac, /// 0xa78
			 0xc068757c, /// 0xa7c
			 0x0674bf05, /// 0xa80
			 0x2151e635, /// 0xa84
			 0x5aaeedce, /// 0xa88
			 0xd9ee83ad, /// 0xa8c
			 0xd3bc564f, /// 0xa90
			 0xc487e124, /// 0xa94
			 0xa6d9383b, /// 0xa98
			 0x55a07c4f, /// 0xa9c
			 0x0626c5ad, /// 0xaa0
			 0xffd0b22b, /// 0xaa4
			 0x531fd175, /// 0xaa8
			 0x198b1647, /// 0xaac
			 0x7887068b, /// 0xab0
			 0x75336368, /// 0xab4
			 0xfd93234c, /// 0xab8
			 0x229b1015, /// 0xabc
			 0x05f2367c, /// 0xac0
			 0x804e4b78, /// 0xac4
			 0x3e59c1b9, /// 0xac8
			 0xa9fde117, /// 0xacc
			 0x2b2dd276, /// 0xad0
			 0xa838369f, /// 0xad4
			 0x2e7d6644, /// 0xad8
			 0xbf0e7d79, /// 0xadc
			 0x72dce078, /// 0xae0
			 0x642ef33e, /// 0xae4
			 0x9e495a62, /// 0xae8
			 0x0ff32197, /// 0xaec
			 0x8c0f2199, /// 0xaf0
			 0xef3ddb39, /// 0xaf4
			 0x5725896b, /// 0xaf8
			 0xffd8cc80, /// 0xafc
			 0x4cc68176, /// 0xb00
			 0xbaf78615, /// 0xb04
			 0xcab88705, /// 0xb08
			 0x0ab628fa, /// 0xb0c
			 0xe06b32f0, /// 0xb10
			 0xefa9b55a, /// 0xb14
			 0xdd0fc232, /// 0xb18
			 0x6becb6bd, /// 0xb1c
			 0x51619c30, /// 0xb20
			 0xb2fb418e, /// 0xb24
			 0x2e3b9347, /// 0xb28
			 0x818250ad, /// 0xb2c
			 0x8d25f5d4, /// 0xb30
			 0x0899b200, /// 0xb34
			 0x9361f56b, /// 0xb38
			 0xde101980, /// 0xb3c
			 0x49bbc9e6, /// 0xb40
			 0xbc95985c, /// 0xb44
			 0x7e82b20a, /// 0xb48
			 0xb48b9bc5, /// 0xb4c
			 0xb25b643c, /// 0xb50
			 0x9e268021, /// 0xb54
			 0x271c799b, /// 0xb58
			 0x80226462, /// 0xb5c
			 0x940b6f62, /// 0xb60
			 0x96670ed5, /// 0xb64
			 0xe5aef76a, /// 0xb68
			 0x2d54e7e1, /// 0xb6c
			 0x6b85d996, /// 0xb70
			 0x730f0119, /// 0xb74
			 0x76bf8c5f, /// 0xb78
			 0x27ce5c1e, /// 0xb7c
			 0xe02076ea, /// 0xb80
			 0xf8ed210e, /// 0xb84
			 0x561a0e9e, /// 0xb88
			 0xbbe2d607, /// 0xb8c
			 0x01045dff, /// 0xb90
			 0x31f029ae, /// 0xb94
			 0x29518b10, /// 0xb98
			 0x7c98a981, /// 0xb9c
			 0xb07558d2, /// 0xba0
			 0x7a8336d0, /// 0xba4
			 0x68cc4d0e, /// 0xba8
			 0x09f396b5, /// 0xbac
			 0x824e811f, /// 0xbb0
			 0x7defc1d4, /// 0xbb4
			 0x8b5bde4b, /// 0xbb8
			 0xaafcb5ff, /// 0xbbc
			 0xedd1b701, /// 0xbc0
			 0xd27e8f62, /// 0xbc4
			 0x3e524aca, /// 0xbc8
			 0xf2d7ef4a, /// 0xbcc
			 0x86248496, /// 0xbd0
			 0xc6753029, /// 0xbd4
			 0x40a415c8, /// 0xbd8
			 0x6bed5856, /// 0xbdc
			 0x4c0c5629, /// 0xbe0
			 0x3c44e577, /// 0xbe4
			 0xdc1050b1, /// 0xbe8
			 0x90621df2, /// 0xbec
			 0xdad6c41f, /// 0xbf0
			 0xee906537, /// 0xbf4
			 0x6a3c430d, /// 0xbf8
			 0xe2cae7c0, /// 0xbfc
			 0xf3de9754, /// 0xc00
			 0x3c6e74b1, /// 0xc04
			 0xe18f7d17, /// 0xc08
			 0x6bbbfecf, /// 0xc0c
			 0x5191aabd, /// 0xc10
			 0x01902ef7, /// 0xc14
			 0x2f231863, /// 0xc18
			 0x66088166, /// 0xc1c
			 0x21b74a3c, /// 0xc20
			 0x14f9266b, /// 0xc24
			 0x9ba49f64, /// 0xc28
			 0x46930f86, /// 0xc2c
			 0x08959ada, /// 0xc30
			 0x07500ad9, /// 0xc34
			 0x2e4937bd, /// 0xc38
			 0x855409f4, /// 0xc3c
			 0xe2634ea3, /// 0xc40
			 0x858a4556, /// 0xc44
			 0x838f5068, /// 0xc48
			 0x5fde96f9, /// 0xc4c
			 0x094e0ccb, /// 0xc50
			 0x457c6a29, /// 0xc54
			 0x3444b1ab, /// 0xc58
			 0x7c7abe29, /// 0xc5c
			 0xf2405383, /// 0xc60
			 0x9c8e159f, /// 0xc64
			 0x7d9fb012, /// 0xc68
			 0xd79fb28b, /// 0xc6c
			 0x1c1ee525, /// 0xc70
			 0x6fa4f0e8, /// 0xc74
			 0x9186b58d, /// 0xc78
			 0xefddcb45, /// 0xc7c
			 0xebc276d0, /// 0xc80
			 0x9410ce61, /// 0xc84
			 0x4d0f3559, /// 0xc88
			 0x8e88af73, /// 0xc8c
			 0x1faa3dd4, /// 0xc90
			 0xef0fd5b7, /// 0xc94
			 0x66bfd4d3, /// 0xc98
			 0xd20d7e69, /// 0xc9c
			 0x4421479e, /// 0xca0
			 0x30642d3e, /// 0xca4
			 0x0149a32b, /// 0xca8
			 0x3aa1e587, /// 0xcac
			 0x465486d6, /// 0xcb0
			 0x7587e4a3, /// 0xcb4
			 0x1fb81ab8, /// 0xcb8
			 0x09426a2d, /// 0xcbc
			 0xc0ccd328, /// 0xcc0
			 0xb78897b8, /// 0xcc4
			 0x687f8f6d, /// 0xcc8
			 0x4c253033, /// 0xccc
			 0xf49ed4c1, /// 0xcd0
			 0xedd5a2cd, /// 0xcd4
			 0x13f74d37, /// 0xcd8
			 0x851d067f, /// 0xcdc
			 0xb5c35775, /// 0xce0
			 0x66aa4149, /// 0xce4
			 0x5e0652e6, /// 0xce8
			 0xeaaaacdb, /// 0xcec
			 0x0da232f1, /// 0xcf0
			 0xf7fcb64f, /// 0xcf4
			 0x8e4b7e28, /// 0xcf8
			 0xf75ae7be, /// 0xcfc
			 0x82cb99cc, /// 0xd00
			 0x9da7fbc6, /// 0xd04
			 0x0eaa1329, /// 0xd08
			 0x1d412b72, /// 0xd0c
			 0x5989024c, /// 0xd10
			 0x43867ca2, /// 0xd14
			 0x6b2eb0f6, /// 0xd18
			 0x7147d0c1, /// 0xd1c
			 0x2a05250f, /// 0xd20
			 0x0b552786, /// 0xd24
			 0xfb1c543c, /// 0xd28
			 0x96b2c4a5, /// 0xd2c
			 0x5893d550, /// 0xd30
			 0xc022482c, /// 0xd34
			 0xab004070, /// 0xd38
			 0xd94d586e, /// 0xd3c
			 0xe15b48c1, /// 0xd40
			 0x61c119c4, /// 0xd44
			 0xca82332e, /// 0xd48
			 0x48045f1a, /// 0xd4c
			 0x54382fc5, /// 0xd50
			 0x18b742fd, /// 0xd54
			 0x93199249, /// 0xd58
			 0xcd7b0743, /// 0xd5c
			 0x80caae5b, /// 0xd60
			 0x5459e3a6, /// 0xd64
			 0x36ee92f4, /// 0xd68
			 0x8dbd941c, /// 0xd6c
			 0x875b026a, /// 0xd70
			 0x24d5dff3, /// 0xd74
			 0x80b8c333, /// 0xd78
			 0x492bc56a, /// 0xd7c
			 0x89780886, /// 0xd80
			 0xd3fd4d28, /// 0xd84
			 0x8caa5b31, /// 0xd88
			 0xf29b004e, /// 0xd8c
			 0xfeb917cf, /// 0xd90
			 0x58a63997, /// 0xd94
			 0x0d0a3da2, /// 0xd98
			 0xfc35d098, /// 0xd9c
			 0x9f0eeb5a, /// 0xda0
			 0x16d60add, /// 0xda4
			 0x7fec73ed, /// 0xda8
			 0x8fdd9ec5, /// 0xdac
			 0x9731a465, /// 0xdb0
			 0x74dc3041, /// 0xdb4
			 0xa3211ab6, /// 0xdb8
			 0x3ebf0be8, /// 0xdbc
			 0xec54b69b, /// 0xdc0
			 0xea0ff044, /// 0xdc4
			 0x27acf22f, /// 0xdc8
			 0xd6bacc6d, /// 0xdcc
			 0x6ae14692, /// 0xdd0
			 0x7c7a98f1, /// 0xdd4
			 0x5d3aa2d1, /// 0xdd8
			 0xc66414ff, /// 0xddc
			 0xfee959b2, /// 0xde0
			 0x9beda280, /// 0xde4
			 0x518da0cd, /// 0xde8
			 0x1500b585, /// 0xdec
			 0x6354a2a9, /// 0xdf0
			 0xdc5cd405, /// 0xdf4
			 0x8f821ff9, /// 0xdf8
			 0xad71aa86, /// 0xdfc
			 0x9a22f0db, /// 0xe00
			 0x5d5d11f6, /// 0xe04
			 0x8ac2ac75, /// 0xe08
			 0x88441887, /// 0xe0c
			 0xf95f8d16, /// 0xe10
			 0x5ea3d515, /// 0xe14
			 0xf8abbaf0, /// 0xe18
			 0xf13a09af, /// 0xe1c
			 0x82b2c4e9, /// 0xe20
			 0x0005fedd, /// 0xe24
			 0x8eefc1ac, /// 0xe28
			 0xd887e0e2, /// 0xe2c
			 0x9fdf0fa7, /// 0xe30
			 0x5709f32d, /// 0xe34
			 0xedab8b7c, /// 0xe38
			 0x11ebd5f0, /// 0xe3c
			 0xa6f666c5, /// 0xe40
			 0x88e7963f, /// 0xe44
			 0xf4a0799c, /// 0xe48
			 0x535fbb9c, /// 0xe4c
			 0xbd86f0f5, /// 0xe50
			 0x8efa7c62, /// 0xe54
			 0x3f3ab7c6, /// 0xe58
			 0x2e4e9af5, /// 0xe5c
			 0x04834042, /// 0xe60
			 0x51520daf, /// 0xe64
			 0x35495bf9, /// 0xe68
			 0xcc30ca9c, /// 0xe6c
			 0xaf61c45a, /// 0xe70
			 0x643f7f29, /// 0xe74
			 0x025c4ad1, /// 0xe78
			 0x2bd6dea2, /// 0xe7c
			 0xb67e6137, /// 0xe80
			 0x9b4cc0be, /// 0xe84
			 0x1a8df637, /// 0xe88
			 0x1011c732, /// 0xe8c
			 0xc7ef98a3, /// 0xe90
			 0x38b2217d, /// 0xe94
			 0xd5940c25, /// 0xe98
			 0x97ea8d35, /// 0xe9c
			 0x6cd6873b, /// 0xea0
			 0xfe30bf8d, /// 0xea4
			 0x0fa8c336, /// 0xea8
			 0x1b6b7dca, /// 0xeac
			 0x5f586828, /// 0xeb0
			 0x010d7295, /// 0xeb4
			 0x549b83a3, /// 0xeb8
			 0xe2255859, /// 0xebc
			 0x769d8590, /// 0xec0
			 0xc754ebcb, /// 0xec4
			 0x8e350dd4, /// 0xec8
			 0x34778cf8, /// 0xecc
			 0x93b18d37, /// 0xed0
			 0x60aecfa8, /// 0xed4
			 0x70be89ca, /// 0xed8
			 0x152764b2, /// 0xedc
			 0x06a3104a, /// 0xee0
			 0x92882b11, /// 0xee4
			 0xb950914c, /// 0xee8
			 0x71b192f9, /// 0xeec
			 0xf8e88a8b, /// 0xef0
			 0x5acf0c31, /// 0xef4
			 0x5a0d81c3, /// 0xef8
			 0xd87c9c0f, /// 0xefc
			 0x8c3b7210, /// 0xf00
			 0x18c5b1d1, /// 0xf04
			 0x1ca570b7, /// 0xf08
			 0xaaf5da55, /// 0xf0c
			 0x12892d7c, /// 0xf10
			 0xb3ff137d, /// 0xf14
			 0xca40685d, /// 0xf18
			 0xb8d6639b, /// 0xf1c
			 0xa094396d, /// 0xf20
			 0xd4f027ea, /// 0xf24
			 0x07f3c041, /// 0xf28
			 0x58af3659, /// 0xf2c
			 0x186926e3, /// 0xf30
			 0x3e50ade2, /// 0xf34
			 0xb0eacbb7, /// 0xf38
			 0xba707922, /// 0xf3c
			 0xced2886b, /// 0xf40
			 0x736fada0, /// 0xf44
			 0xd5a7ab66, /// 0xf48
			 0x7e56f9dc, /// 0xf4c
			 0x98d53221, /// 0xf50
			 0x575b5da9, /// 0xf54
			 0x73d7edb5, /// 0xf58
			 0x077c0581, /// 0xf5c
			 0x7f7f1b08, /// 0xf60
			 0xdec80923, /// 0xf64
			 0xffc5a3d1, /// 0xf68
			 0x4e86898c, /// 0xf6c
			 0x0a001e5c, /// 0xf70
			 0xf1030ddd, /// 0xf74
			 0x70df5de2, /// 0xf78
			 0x03074a3c, /// 0xf7c
			 0x62488126, /// 0xf80
			 0xb65e7b9e, /// 0xf84
			 0x71eb5cef, /// 0xf88
			 0x5f35d152, /// 0xf8c
			 0x79156893, /// 0xf90
			 0x6d79af90, /// 0xf94
			 0x38f8e765, /// 0xf98
			 0xbc505378, /// 0xf9c
			 0xf4a2c2ee, /// 0xfa0
			 0xa0eb04d4, /// 0xfa4
			 0x6df338e4, /// 0xfa8
			 0xcfa52904, /// 0xfac
			 0xf90995b4, /// 0xfb0
			 0x280e44fb, /// 0xfb4
			 0x6fe1c8b9, /// 0xfb8
			 0x1ab1b823, /// 0xfbc
			 0xba94e675, /// 0xfc0
			 0x51703637, /// 0xfc4
			 0xe9904daf, /// 0xfc8
			 0xd2c5e17f, /// 0xfcc
			 0x068516c9, /// 0xfd0
			 0x1010f6fd, /// 0xfd4
			 0x27e918b4, /// 0xfd8
			 0x8762a951, /// 0xfdc
			 0x3b242220, /// 0xfe0
			 0x662b7b53, /// 0xfe4
			 0x7dab0eeb, /// 0xfe8
			 0xabf22e55, /// 0xfec
			 0x8176b9cb, /// 0xff0
			 0x1805e873, /// 0xff4
			 0xbc01b4be, /// 0xff8
			 0x03717d84 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x52b05814, /// 0x0
			 0xb34f20d0, /// 0x4
			 0x51eda424, /// 0x8
			 0x847f3d08, /// 0xc
			 0x723e9b61, /// 0x10
			 0x42c8511a, /// 0x14
			 0xd5a5e8ba, /// 0x18
			 0xb7f52590, /// 0x1c
			 0x9599d18a, /// 0x20
			 0x264f3ecf, /// 0x24
			 0x7022790d, /// 0x28
			 0x72345f0c, /// 0x2c
			 0x3314af83, /// 0x30
			 0xfeb61a42, /// 0x34
			 0xb9166d65, /// 0x38
			 0x8288bc39, /// 0x3c
			 0xfee7d32c, /// 0x40
			 0xd117e536, /// 0x44
			 0xe3e6221e, /// 0x48
			 0x9a68ffb6, /// 0x4c
			 0xa6d006b3, /// 0x50
			 0xa013e369, /// 0x54
			 0x734dd8f9, /// 0x58
			 0xa20e7497, /// 0x5c
			 0x49e3656a, /// 0x60
			 0xef1589fd, /// 0x64
			 0x3533653a, /// 0x68
			 0x6b00c154, /// 0x6c
			 0x135cdccf, /// 0x70
			 0x2c71509e, /// 0x74
			 0x83483386, /// 0x78
			 0x4b7e2d35, /// 0x7c
			 0x29257769, /// 0x80
			 0x52432713, /// 0x84
			 0x9a153285, /// 0x88
			 0x37164c41, /// 0x8c
			 0x1e502ad9, /// 0x90
			 0xb8a4f85e, /// 0x94
			 0x6f3e367e, /// 0x98
			 0x7dba22b0, /// 0x9c
			 0xdebfd4e9, /// 0xa0
			 0xe67e7456, /// 0xa4
			 0x03a022f3, /// 0xa8
			 0x7d6d6db2, /// 0xac
			 0x404750ce, /// 0xb0
			 0x3667d306, /// 0xb4
			 0x68fdf700, /// 0xb8
			 0xd2bf071a, /// 0xbc
			 0x41558da6, /// 0xc0
			 0xc13bcf80, /// 0xc4
			 0x8d536700, /// 0xc8
			 0x0d0a3a03, /// 0xcc
			 0xb26abab7, /// 0xd0
			 0x237d7b9c, /// 0xd4
			 0x5b5bea1d, /// 0xd8
			 0x6fd37295, /// 0xdc
			 0xe9100d1c, /// 0xe0
			 0x3e4771c8, /// 0xe4
			 0xc282fa2a, /// 0xe8
			 0xf134fefb, /// 0xec
			 0x461d05a5, /// 0xf0
			 0xd2a86c76, /// 0xf4
			 0x2a2e1109, /// 0xf8
			 0x38205967, /// 0xfc
			 0xdbd3cc84, /// 0x100
			 0x4b13b123, /// 0x104
			 0x2884d339, /// 0x108
			 0xc80747cb, /// 0x10c
			 0x2628818b, /// 0x110
			 0xda55a9cf, /// 0x114
			 0x4fb4f4fd, /// 0x118
			 0x18db49f1, /// 0x11c
			 0x3be8ea6e, /// 0x120
			 0xf9df2f1f, /// 0x124
			 0xd68dbe00, /// 0x128
			 0x5b7c12ae, /// 0x12c
			 0x276d46ee, /// 0x130
			 0x698bbcae, /// 0x134
			 0x9dc382db, /// 0x138
			 0x756ab7de, /// 0x13c
			 0xd8528bf7, /// 0x140
			 0x7ab57408, /// 0x144
			 0xe2023c10, /// 0x148
			 0x865354a0, /// 0x14c
			 0x03a8bf51, /// 0x150
			 0xb8f05c50, /// 0x154
			 0x6f2517bd, /// 0x158
			 0x1c650f7b, /// 0x15c
			 0x62f2b3a8, /// 0x160
			 0xb06d6c25, /// 0x164
			 0x3c75821e, /// 0x168
			 0xe6e3e4d1, /// 0x16c
			 0x01672a3c, /// 0x170
			 0xf33b8a35, /// 0x174
			 0x7d8d90b3, /// 0x178
			 0x8b3bd580, /// 0x17c
			 0x55c4cd9e, /// 0x180
			 0x2bebd18f, /// 0x184
			 0xbfa1e54a, /// 0x188
			 0x369a9251, /// 0x18c
			 0xcb66ed37, /// 0x190
			 0xa3406850, /// 0x194
			 0x1ea930fb, /// 0x198
			 0x535f311a, /// 0x19c
			 0xaa696ac9, /// 0x1a0
			 0xd7b21196, /// 0x1a4
			 0xf9a0591c, /// 0x1a8
			 0xdbf45327, /// 0x1ac
			 0xdf49dfbb, /// 0x1b0
			 0xde2506ca, /// 0x1b4
			 0xaca9ea14, /// 0x1b8
			 0x7ce090bd, /// 0x1bc
			 0x3539143e, /// 0x1c0
			 0x2febd584, /// 0x1c4
			 0x693fe416, /// 0x1c8
			 0x67c9a338, /// 0x1cc
			 0x51c97b13, /// 0x1d0
			 0xb6a73a0f, /// 0x1d4
			 0x97edc84a, /// 0x1d8
			 0xa0e9562d, /// 0x1dc
			 0xee3bee2e, /// 0x1e0
			 0xc5823896, /// 0x1e4
			 0xb5e62ac1, /// 0x1e8
			 0xe302436b, /// 0x1ec
			 0xf10666f7, /// 0x1f0
			 0xb4d09410, /// 0x1f4
			 0xea304446, /// 0x1f8
			 0x0ddf9cc5, /// 0x1fc
			 0x57594925, /// 0x200
			 0xab0cc04e, /// 0x204
			 0x4b763472, /// 0x208
			 0x556adad6, /// 0x20c
			 0x108168cd, /// 0x210
			 0xa4946d60, /// 0x214
			 0x12443aea, /// 0x218
			 0x0d6d27a8, /// 0x21c
			 0xd43911df, /// 0x220
			 0x6603c4a7, /// 0x224
			 0xb2232d19, /// 0x228
			 0x4a43c6f8, /// 0x22c
			 0xa167f785, /// 0x230
			 0xefba20f9, /// 0x234
			 0x4d18d356, /// 0x238
			 0x9e720d3b, /// 0x23c
			 0x4ba14e48, /// 0x240
			 0x8b36713c, /// 0x244
			 0x82ab1ecb, /// 0x248
			 0x8f4a1d75, /// 0x24c
			 0x774378b8, /// 0x250
			 0xb1d0b6f3, /// 0x254
			 0x8d543acb, /// 0x258
			 0x1d08712f, /// 0x25c
			 0x72a81fd1, /// 0x260
			 0x23b709e9, /// 0x264
			 0x0b7e63bf, /// 0x268
			 0x5ed2afb7, /// 0x26c
			 0xfc5be7f2, /// 0x270
			 0x2a350da3, /// 0x274
			 0x0e1a511e, /// 0x278
			 0x9636e028, /// 0x27c
			 0xa42178b2, /// 0x280
			 0x4a352087, /// 0x284
			 0xd47af38b, /// 0x288
			 0x1faace2c, /// 0x28c
			 0x96bfebf3, /// 0x290
			 0x8111bb5e, /// 0x294
			 0x43832614, /// 0x298
			 0x0c39fb35, /// 0x29c
			 0x5319c62c, /// 0x2a0
			 0x8d842744, /// 0x2a4
			 0x483fc66c, /// 0x2a8
			 0x1aa2c17c, /// 0x2ac
			 0xb1d92fea, /// 0x2b0
			 0xd80f7fb5, /// 0x2b4
			 0xa6b62586, /// 0x2b8
			 0x4c157968, /// 0x2bc
			 0xd52c7a63, /// 0x2c0
			 0xe887448a, /// 0x2c4
			 0x9dfe5a01, /// 0x2c8
			 0x1f8b3355, /// 0x2cc
			 0xbef83f50, /// 0x2d0
			 0x24441a1e, /// 0x2d4
			 0x181b502b, /// 0x2d8
			 0x7beb3bf9, /// 0x2dc
			 0x7d0aaabf, /// 0x2e0
			 0x37eed781, /// 0x2e4
			 0xeff56389, /// 0x2e8
			 0x6ca5fe7c, /// 0x2ec
			 0x61605ec0, /// 0x2f0
			 0x33153c16, /// 0x2f4
			 0xda7af981, /// 0x2f8
			 0x863a5941, /// 0x2fc
			 0x5aedf8e9, /// 0x300
			 0xcfc07bf5, /// 0x304
			 0x5e37e6ef, /// 0x308
			 0x939e2218, /// 0x30c
			 0xa2c327ef, /// 0x310
			 0x9a7b462c, /// 0x314
			 0x2c32b684, /// 0x318
			 0x7b342388, /// 0x31c
			 0x4ae9aabe, /// 0x320
			 0xb7f2a3a0, /// 0x324
			 0x62666795, /// 0x328
			 0x50b67ced, /// 0x32c
			 0x3a899a1b, /// 0x330
			 0x27c65ba5, /// 0x334
			 0xde057cf1, /// 0x338
			 0x3dfaa8cd, /// 0x33c
			 0x7f33c1d8, /// 0x340
			 0xc8888eb3, /// 0x344
			 0x3cdd83dd, /// 0x348
			 0x4775e79c, /// 0x34c
			 0x1be00490, /// 0x350
			 0xedf5bfeb, /// 0x354
			 0x3573c9fb, /// 0x358
			 0xfbfb5955, /// 0x35c
			 0x5929403e, /// 0x360
			 0x987d7e7d, /// 0x364
			 0x451251b2, /// 0x368
			 0xf1942133, /// 0x36c
			 0x022aab84, /// 0x370
			 0x9983ab2f, /// 0x374
			 0x16f151fa, /// 0x378
			 0x6d98f423, /// 0x37c
			 0x6abd0f3b, /// 0x380
			 0x316a91b4, /// 0x384
			 0x6d2f119f, /// 0x388
			 0x3d98922f, /// 0x38c
			 0xa4d4d3b3, /// 0x390
			 0xcf9038bc, /// 0x394
			 0x1e529879, /// 0x398
			 0x0cb2c1ea, /// 0x39c
			 0x0a8cf26f, /// 0x3a0
			 0x27d8c24d, /// 0x3a4
			 0x5c7cea15, /// 0x3a8
			 0x124d57c4, /// 0x3ac
			 0x6dd274a7, /// 0x3b0
			 0x67055a08, /// 0x3b4
			 0x2cfa732d, /// 0x3b8
			 0x14cd12d8, /// 0x3bc
			 0x8b1b9a9e, /// 0x3c0
			 0xb2df027a, /// 0x3c4
			 0xafecd616, /// 0x3c8
			 0x2ba6b4be, /// 0x3cc
			 0xb92b1d25, /// 0x3d0
			 0xab7d3e14, /// 0x3d4
			 0xd1622627, /// 0x3d8
			 0x2bf4f27c, /// 0x3dc
			 0x95a3c28a, /// 0x3e0
			 0xad18c030, /// 0x3e4
			 0xadc65a01, /// 0x3e8
			 0x07a94b67, /// 0x3ec
			 0x6fc7fb1e, /// 0x3f0
			 0x49580fa1, /// 0x3f4
			 0xbd27ff24, /// 0x3f8
			 0xaa8a7a77, /// 0x3fc
			 0x8a3994d1, /// 0x400
			 0x1354464d, /// 0x404
			 0x77b3bb92, /// 0x408
			 0xf913711b, /// 0x40c
			 0x3b30c859, /// 0x410
			 0x8bcdfdec, /// 0x414
			 0x0404be3a, /// 0x418
			 0xa4aa015d, /// 0x41c
			 0x7c27f6b4, /// 0x420
			 0x590eb257, /// 0x424
			 0x4c58733c, /// 0x428
			 0xb4203394, /// 0x42c
			 0xd2e6320f, /// 0x430
			 0xdba73110, /// 0x434
			 0x93c3e038, /// 0x438
			 0x4a4a218b, /// 0x43c
			 0x3dcf2ea8, /// 0x440
			 0x9633e168, /// 0x444
			 0x034e2793, /// 0x448
			 0x8740d891, /// 0x44c
			 0x373cf51e, /// 0x450
			 0x97dd44d6, /// 0x454
			 0x9ae31079, /// 0x458
			 0x312d738b, /// 0x45c
			 0xf63fddf8, /// 0x460
			 0x5e14b27b, /// 0x464
			 0x77b55ad0, /// 0x468
			 0x652313b9, /// 0x46c
			 0xf689b4b4, /// 0x470
			 0x21e83301, /// 0x474
			 0xa6a891a0, /// 0x478
			 0x7ee64df9, /// 0x47c
			 0xe1a6b0b1, /// 0x480
			 0xf59bdf78, /// 0x484
			 0x60c69042, /// 0x488
			 0xae2f6481, /// 0x48c
			 0xd99bf1d2, /// 0x490
			 0x9f0b5aca, /// 0x494
			 0x53f805a6, /// 0x498
			 0xd9abe0eb, /// 0x49c
			 0xaa6128e7, /// 0x4a0
			 0x9281317f, /// 0x4a4
			 0xebe32c7e, /// 0x4a8
			 0xb3d6d883, /// 0x4ac
			 0x8d4a03a7, /// 0x4b0
			 0xb2f9a055, /// 0x4b4
			 0x23f9b83e, /// 0x4b8
			 0xedb2c087, /// 0x4bc
			 0xbb5418bd, /// 0x4c0
			 0x4307075b, /// 0x4c4
			 0x010351f5, /// 0x4c8
			 0x9d1ef361, /// 0x4cc
			 0xafde9e75, /// 0x4d0
			 0x028f0efc, /// 0x4d4
			 0xc91da3af, /// 0x4d8
			 0x211866c6, /// 0x4dc
			 0xa6401593, /// 0x4e0
			 0xca65a6fa, /// 0x4e4
			 0x73ead827, /// 0x4e8
			 0xe424bde1, /// 0x4ec
			 0xbe688e1b, /// 0x4f0
			 0xb556fffe, /// 0x4f4
			 0xa11089c4, /// 0x4f8
			 0x3bc42e48, /// 0x4fc
			 0x28aae127, /// 0x500
			 0x8d16f0aa, /// 0x504
			 0x80179026, /// 0x508
			 0x6ffcac64, /// 0x50c
			 0x9d3b258f, /// 0x510
			 0x2452a067, /// 0x514
			 0xbbc70885, /// 0x518
			 0x42c82279, /// 0x51c
			 0x977da76c, /// 0x520
			 0x28e666ec, /// 0x524
			 0xb7212696, /// 0x528
			 0x8e4ce7fe, /// 0x52c
			 0x1a8008d5, /// 0x530
			 0x34f3d3e0, /// 0x534
			 0x6d1ab3b5, /// 0x538
			 0x1b5981ad, /// 0x53c
			 0x0f19d668, /// 0x540
			 0x9e80757c, /// 0x544
			 0x74f1e862, /// 0x548
			 0x02b1b0ed, /// 0x54c
			 0x6fd0c0f0, /// 0x550
			 0xa34f68d8, /// 0x554
			 0x51c53694, /// 0x558
			 0x9471739d, /// 0x55c
			 0x32b8d343, /// 0x560
			 0xaa708c3e, /// 0x564
			 0x3d7bed9f, /// 0x568
			 0x5ff0a4bf, /// 0x56c
			 0x7eb1ff77, /// 0x570
			 0x3b37dc7c, /// 0x574
			 0xd604a282, /// 0x578
			 0xec660380, /// 0x57c
			 0xfd919f62, /// 0x580
			 0x3ffc2569, /// 0x584
			 0x9b5ce43a, /// 0x588
			 0x499a1017, /// 0x58c
			 0xb12b6a0d, /// 0x590
			 0x014738e7, /// 0x594
			 0xcce44572, /// 0x598
			 0x67765881, /// 0x59c
			 0x4f1d4d88, /// 0x5a0
			 0x34d3498e, /// 0x5a4
			 0xf6e58763, /// 0x5a8
			 0x0b538e90, /// 0x5ac
			 0x08d64992, /// 0x5b0
			 0x6ea9bf3a, /// 0x5b4
			 0x1d52521b, /// 0x5b8
			 0x9de59b77, /// 0x5bc
			 0x392432a8, /// 0x5c0
			 0xd8572295, /// 0x5c4
			 0x1443c1d4, /// 0x5c8
			 0x75d86995, /// 0x5cc
			 0x9685abc1, /// 0x5d0
			 0x298bfcfe, /// 0x5d4
			 0xb8bc60f2, /// 0x5d8
			 0x0b3772fe, /// 0x5dc
			 0x15e0a226, /// 0x5e0
			 0x82fb94ce, /// 0x5e4
			 0xf0eb1286, /// 0x5e8
			 0x59dbdb0c, /// 0x5ec
			 0x105abfb5, /// 0x5f0
			 0xe0dafcd1, /// 0x5f4
			 0xee30d505, /// 0x5f8
			 0x7af36fe7, /// 0x5fc
			 0xf0bb5004, /// 0x600
			 0x9925b60a, /// 0x604
			 0xd17afe80, /// 0x608
			 0x4a1cf2e4, /// 0x60c
			 0xb455de74, /// 0x610
			 0xca649a75, /// 0x614
			 0x2915cc55, /// 0x618
			 0x9de96651, /// 0x61c
			 0xfc8cc741, /// 0x620
			 0xf33fc298, /// 0x624
			 0x031e324f, /// 0x628
			 0xb1dbe1e6, /// 0x62c
			 0x28e06bbb, /// 0x630
			 0xc7031b4f, /// 0x634
			 0x8f8b355c, /// 0x638
			 0xe09ec38d, /// 0x63c
			 0x5f76504f, /// 0x640
			 0xa9b309fc, /// 0x644
			 0x5f0deb7c, /// 0x648
			 0xc80b1957, /// 0x64c
			 0xacd8679c, /// 0x650
			 0x90d1d822, /// 0x654
			 0xfb4e89bf, /// 0x658
			 0x9e9ce662, /// 0x65c
			 0x9a751ad1, /// 0x660
			 0x3c3080b4, /// 0x664
			 0xdd2e2312, /// 0x668
			 0x6ce1ab5a, /// 0x66c
			 0xd01609a6, /// 0x670
			 0x0e99a17a, /// 0x674
			 0x26536d49, /// 0x678
			 0xaab9b070, /// 0x67c
			 0x0d3bcb06, /// 0x680
			 0x734de18e, /// 0x684
			 0xd4ce7eff, /// 0x688
			 0x97442775, /// 0x68c
			 0x50e303d7, /// 0x690
			 0xefde8aff, /// 0x694
			 0x9c054b93, /// 0x698
			 0x563e2010, /// 0x69c
			 0xf46c2e74, /// 0x6a0
			 0xb44f0dbe, /// 0x6a4
			 0xcfdfdcdf, /// 0x6a8
			 0x105d675b, /// 0x6ac
			 0x72577082, /// 0x6b0
			 0xd347af91, /// 0x6b4
			 0x401990cb, /// 0x6b8
			 0xcf7a0930, /// 0x6bc
			 0xb5aba37a, /// 0x6c0
			 0xbe53084d, /// 0x6c4
			 0xc02d1cfb, /// 0x6c8
			 0xa66d5e19, /// 0x6cc
			 0x707b8488, /// 0x6d0
			 0x9e445691, /// 0x6d4
			 0x2ba34f73, /// 0x6d8
			 0xad0429c6, /// 0x6dc
			 0xbea8cc60, /// 0x6e0
			 0xeebbb535, /// 0x6e4
			 0x2768c77d, /// 0x6e8
			 0xb68c76c1, /// 0x6ec
			 0xf284bcd8, /// 0x6f0
			 0x923b198b, /// 0x6f4
			 0x7a39f98b, /// 0x6f8
			 0x2ff11c8c, /// 0x6fc
			 0x6641da38, /// 0x700
			 0x58e7bbef, /// 0x704
			 0x7e09597e, /// 0x708
			 0x4c44a28a, /// 0x70c
			 0xd5894300, /// 0x710
			 0x9893439c, /// 0x714
			 0x149e2bac, /// 0x718
			 0x6d210de1, /// 0x71c
			 0xb652e681, /// 0x720
			 0xb89a1a1d, /// 0x724
			 0x65973f84, /// 0x728
			 0x752ab232, /// 0x72c
			 0xe0fe925c, /// 0x730
			 0xbbde0581, /// 0x734
			 0x75f67b0b, /// 0x738
			 0x64cabac6, /// 0x73c
			 0x6f8697da, /// 0x740
			 0xbf2f3e2f, /// 0x744
			 0xe43d0135, /// 0x748
			 0xeeff5b19, /// 0x74c
			 0x607ae544, /// 0x750
			 0x7565d41a, /// 0x754
			 0x791b5605, /// 0x758
			 0xa52a0ef6, /// 0x75c
			 0x617b9cc6, /// 0x760
			 0x8ad44227, /// 0x764
			 0x80fa13fe, /// 0x768
			 0x2e98b5d9, /// 0x76c
			 0xa1677abb, /// 0x770
			 0x80a4816c, /// 0x774
			 0x07ffa4cd, /// 0x778
			 0x21d8f840, /// 0x77c
			 0x576ed9a6, /// 0x780
			 0xe8fee85c, /// 0x784
			 0xe3ed3464, /// 0x788
			 0xc58c4943, /// 0x78c
			 0x0982dd76, /// 0x790
			 0xb4feef6c, /// 0x794
			 0xd85deceb, /// 0x798
			 0x18f00bdd, /// 0x79c
			 0x111f2f95, /// 0x7a0
			 0x125350be, /// 0x7a4
			 0x771ceb86, /// 0x7a8
			 0x5eb9daee, /// 0x7ac
			 0x067d023a, /// 0x7b0
			 0x380dc325, /// 0x7b4
			 0xadaeec73, /// 0x7b8
			 0xf9f2079a, /// 0x7bc
			 0x067fd897, /// 0x7c0
			 0x2a87dcbc, /// 0x7c4
			 0xde561c39, /// 0x7c8
			 0xd97d2791, /// 0x7cc
			 0x0334a051, /// 0x7d0
			 0xbf37e9f0, /// 0x7d4
			 0x588e9945, /// 0x7d8
			 0xa8b0af5f, /// 0x7dc
			 0xf12c1d36, /// 0x7e0
			 0x0c9b1dd0, /// 0x7e4
			 0x30f63ff5, /// 0x7e8
			 0x3562fd85, /// 0x7ec
			 0x09cf53b2, /// 0x7f0
			 0x20f523f1, /// 0x7f4
			 0xfe191514, /// 0x7f8
			 0xd026ddae, /// 0x7fc
			 0x81c6cd88, /// 0x800
			 0xed6535c9, /// 0x804
			 0xe350ad31, /// 0x808
			 0x8d91352a, /// 0x80c
			 0x6d21de1d, /// 0x810
			 0x8139434f, /// 0x814
			 0xfd99f924, /// 0x818
			 0x09cce3bc, /// 0x81c
			 0xa670fe07, /// 0x820
			 0xef8525fd, /// 0x824
			 0x00b64aae, /// 0x828
			 0x97b17db3, /// 0x82c
			 0x7ff8a3e3, /// 0x830
			 0x042a3fbb, /// 0x834
			 0x4a907bfe, /// 0x838
			 0x3dfb3eed, /// 0x83c
			 0x22df13d5, /// 0x840
			 0xaf1db4dc, /// 0x844
			 0x812e238b, /// 0x848
			 0x46671771, /// 0x84c
			 0x280bbf5c, /// 0x850
			 0x24e80ff3, /// 0x854
			 0xed715926, /// 0x858
			 0xa7e59156, /// 0x85c
			 0x7b5d67cb, /// 0x860
			 0x9f1eee37, /// 0x864
			 0x90d3d838, /// 0x868
			 0x17f00e3c, /// 0x86c
			 0xc26e805d, /// 0x870
			 0xf8e9fe22, /// 0x874
			 0xfb8d267b, /// 0x878
			 0x1d2c4db9, /// 0x87c
			 0xf94a1e04, /// 0x880
			 0x17090b98, /// 0x884
			 0xdc9ab874, /// 0x888
			 0x5123e7c9, /// 0x88c
			 0x60d5f392, /// 0x890
			 0x201b6d4b, /// 0x894
			 0x42dcc059, /// 0x898
			 0xd7cb2327, /// 0x89c
			 0xacc664a9, /// 0x8a0
			 0xff7e8d3a, /// 0x8a4
			 0xcc4c4b60, /// 0x8a8
			 0x56f710d8, /// 0x8ac
			 0x8074d2aa, /// 0x8b0
			 0xfea1e5f4, /// 0x8b4
			 0x653f3bfa, /// 0x8b8
			 0x9eff4339, /// 0x8bc
			 0xa0f5e1c2, /// 0x8c0
			 0xde367f4f, /// 0x8c4
			 0xe921b629, /// 0x8c8
			 0x147cb2ef, /// 0x8cc
			 0x25880420, /// 0x8d0
			 0xca20579d, /// 0x8d4
			 0xc97f8274, /// 0x8d8
			 0xb18d4df3, /// 0x8dc
			 0xa208c693, /// 0x8e0
			 0xe5991e63, /// 0x8e4
			 0xf8831e3f, /// 0x8e8
			 0x22e9e9e5, /// 0x8ec
			 0x93390c14, /// 0x8f0
			 0x0c7dddb5, /// 0x8f4
			 0x0a194b01, /// 0x8f8
			 0x5fc8a590, /// 0x8fc
			 0x3892fa9c, /// 0x900
			 0x0e484f6a, /// 0x904
			 0xf7a12718, /// 0x908
			 0x2aebddbb, /// 0x90c
			 0x002d3e2c, /// 0x910
			 0x9385d184, /// 0x914
			 0x5db0b67c, /// 0x918
			 0x9b5a7404, /// 0x91c
			 0x395c52d2, /// 0x920
			 0x48f56419, /// 0x924
			 0xd8fe7c7f, /// 0x928
			 0xb873e6a7, /// 0x92c
			 0xbfa6cacc, /// 0x930
			 0x70085873, /// 0x934
			 0x1a404ef7, /// 0x938
			 0xa42655ef, /// 0x93c
			 0xafdf66d8, /// 0x940
			 0x406dedb9, /// 0x944
			 0xe7fd8e07, /// 0x948
			 0x5afd2503, /// 0x94c
			 0x73ba6f9c, /// 0x950
			 0xfb1832fc, /// 0x954
			 0xe28b47f1, /// 0x958
			 0x6bb5c0d4, /// 0x95c
			 0x69dcff87, /// 0x960
			 0x313f9a6c, /// 0x964
			 0xe87bb77a, /// 0x968
			 0x1a4745ec, /// 0x96c
			 0x288bc9dd, /// 0x970
			 0xd015ed2f, /// 0x974
			 0x4f110d63, /// 0x978
			 0x69fccd64, /// 0x97c
			 0x103e1568, /// 0x980
			 0x58502549, /// 0x984
			 0x618ba935, /// 0x988
			 0x66788349, /// 0x98c
			 0x40f9f678, /// 0x990
			 0xc0999416, /// 0x994
			 0x650b9b10, /// 0x998
			 0xb9b247ca, /// 0x99c
			 0x7c636db0, /// 0x9a0
			 0x10692ca0, /// 0x9a4
			 0xf2a1de2d, /// 0x9a8
			 0xb1c2d0b1, /// 0x9ac
			 0x0190747d, /// 0x9b0
			 0xadbf0bae, /// 0x9b4
			 0x073f67e8, /// 0x9b8
			 0xeed21b71, /// 0x9bc
			 0x02ea13c1, /// 0x9c0
			 0xfb7d5710, /// 0x9c4
			 0x19723d56, /// 0x9c8
			 0xcc16b7c0, /// 0x9cc
			 0x840a4595, /// 0x9d0
			 0x244523ff, /// 0x9d4
			 0x232cf5ef, /// 0x9d8
			 0x2bd1b508, /// 0x9dc
			 0x1e31bd85, /// 0x9e0
			 0x710e7039, /// 0x9e4
			 0x43dbffd2, /// 0x9e8
			 0x485b1ce5, /// 0x9ec
			 0xef6531da, /// 0x9f0
			 0xdc2cdd33, /// 0x9f4
			 0x0a14afba, /// 0x9f8
			 0x890f9df0, /// 0x9fc
			 0xc34f431f, /// 0xa00
			 0xa935af2e, /// 0xa04
			 0x6cee22c8, /// 0xa08
			 0x36713d43, /// 0xa0c
			 0x533b6641, /// 0xa10
			 0x9c974dbe, /// 0xa14
			 0x6a6aafda, /// 0xa18
			 0x8b4e9bc2, /// 0xa1c
			 0x57fc8da0, /// 0xa20
			 0x013919aa, /// 0xa24
			 0x8d2e42ca, /// 0xa28
			 0xcda179d9, /// 0xa2c
			 0x1574ed45, /// 0xa30
			 0xf21dfd80, /// 0xa34
			 0xdb9cda31, /// 0xa38
			 0xadb4c9dc, /// 0xa3c
			 0x619a7981, /// 0xa40
			 0x3fe52c51, /// 0xa44
			 0x57be1446, /// 0xa48
			 0x453327a2, /// 0xa4c
			 0x082db713, /// 0xa50
			 0x7cc6d586, /// 0xa54
			 0x09b6c239, /// 0xa58
			 0x2215a78d, /// 0xa5c
			 0xd5ed1e25, /// 0xa60
			 0x3e5221bf, /// 0xa64
			 0xf26e1886, /// 0xa68
			 0xd0fae50e, /// 0xa6c
			 0x53df528e, /// 0xa70
			 0x267a3b75, /// 0xa74
			 0x37012568, /// 0xa78
			 0xe6ee6a73, /// 0xa7c
			 0xa9b13f48, /// 0xa80
			 0xbad39017, /// 0xa84
			 0x6e8122a2, /// 0xa88
			 0xd1cdd5c7, /// 0xa8c
			 0x01f2670f, /// 0xa90
			 0xccedfe17, /// 0xa94
			 0x775dc3af, /// 0xa98
			 0xf54e7802, /// 0xa9c
			 0x4a5c43b2, /// 0xaa0
			 0xf5e61ab4, /// 0xaa4
			 0x3eaa361d, /// 0xaa8
			 0x9de6a413, /// 0xaac
			 0x5b616915, /// 0xab0
			 0x4da7d639, /// 0xab4
			 0x5d780c3d, /// 0xab8
			 0x22f41661, /// 0xabc
			 0xddf71359, /// 0xac0
			 0x36f31c0e, /// 0xac4
			 0xf0a72063, /// 0xac8
			 0x45c10609, /// 0xacc
			 0x2e404cb0, /// 0xad0
			 0x3041db20, /// 0xad4
			 0x315c9781, /// 0xad8
			 0x29c8cbcb, /// 0xadc
			 0x95a5f44e, /// 0xae0
			 0x56a50644, /// 0xae4
			 0x5c163018, /// 0xae8
			 0x3051d6a8, /// 0xaec
			 0x15afc554, /// 0xaf0
			 0xc2bc6a3b, /// 0xaf4
			 0xe24fd08d, /// 0xaf8
			 0xcb5ea98f, /// 0xafc
			 0xf143c353, /// 0xb00
			 0x52cb3386, /// 0xb04
			 0x546e27d6, /// 0xb08
			 0x22ef561f, /// 0xb0c
			 0xf737da56, /// 0xb10
			 0xb1b0a35f, /// 0xb14
			 0x73d18339, /// 0xb18
			 0xd5dc5276, /// 0xb1c
			 0x470968d0, /// 0xb20
			 0xe74a1462, /// 0xb24
			 0x80a4bbb7, /// 0xb28
			 0xef8adf38, /// 0xb2c
			 0x9913fe53, /// 0xb30
			 0x7014b3c7, /// 0xb34
			 0x3c22f5dc, /// 0xb38
			 0x104b209d, /// 0xb3c
			 0x9c824497, /// 0xb40
			 0xd46241e1, /// 0xb44
			 0x245de7a0, /// 0xb48
			 0x77b6a9a3, /// 0xb4c
			 0xc47b2a24, /// 0xb50
			 0x0366e9f1, /// 0xb54
			 0x80efee7f, /// 0xb58
			 0x9278f347, /// 0xb5c
			 0xfb8bdfba, /// 0xb60
			 0x47b14db1, /// 0xb64
			 0x409a51b9, /// 0xb68
			 0xcbe2db9a, /// 0xb6c
			 0x33c9d817, /// 0xb70
			 0x4fa91a9b, /// 0xb74
			 0x580e68a3, /// 0xb78
			 0x82be5080, /// 0xb7c
			 0xa8c2cdff, /// 0xb80
			 0xf8ebe35a, /// 0xb84
			 0x49edcfe9, /// 0xb88
			 0xe66d8339, /// 0xb8c
			 0x4be10ff8, /// 0xb90
			 0x2275797a, /// 0xb94
			 0xd3544f50, /// 0xb98
			 0x9e272929, /// 0xb9c
			 0xc009a148, /// 0xba0
			 0xea179226, /// 0xba4
			 0x518ad354, /// 0xba8
			 0x986cd701, /// 0xbac
			 0x5fe0a238, /// 0xbb0
			 0xf4495aad, /// 0xbb4
			 0xbffc5b1f, /// 0xbb8
			 0x924a806e, /// 0xbbc
			 0x08cbb4ef, /// 0xbc0
			 0x99d2b967, /// 0xbc4
			 0x33151dc7, /// 0xbc8
			 0x91296681, /// 0xbcc
			 0x03a86a04, /// 0xbd0
			 0x0f2f25a0, /// 0xbd4
			 0xcf83eeef, /// 0xbd8
			 0x372b03af, /// 0xbdc
			 0x33a2a192, /// 0xbe0
			 0x0a2c51c9, /// 0xbe4
			 0x61fd3531, /// 0xbe8
			 0x84e394d4, /// 0xbec
			 0x3a081713, /// 0xbf0
			 0x5cdd83e2, /// 0xbf4
			 0x34d0aa09, /// 0xbf8
			 0xe0b13341, /// 0xbfc
			 0x80d2520f, /// 0xc00
			 0x2be76efb, /// 0xc04
			 0xc3efaeec, /// 0xc08
			 0x4b3a3158, /// 0xc0c
			 0xd534cb94, /// 0xc10
			 0xcf28e98c, /// 0xc14
			 0x96ea9a2e, /// 0xc18
			 0x9743d323, /// 0xc1c
			 0x19fa43fc, /// 0xc20
			 0xb3cc235a, /// 0xc24
			 0x04a19106, /// 0xc28
			 0xe3151b97, /// 0xc2c
			 0x7404a9df, /// 0xc30
			 0xba33d067, /// 0xc34
			 0xccea5ed6, /// 0xc38
			 0x47d1c686, /// 0xc3c
			 0xf2389388, /// 0xc40
			 0xa18e4bd9, /// 0xc44
			 0x6156fefe, /// 0xc48
			 0x4b11f06e, /// 0xc4c
			 0x2742ff99, /// 0xc50
			 0x8b47ca2e, /// 0xc54
			 0xd2037047, /// 0xc58
			 0x7b743677, /// 0xc5c
			 0x9ec9f6f3, /// 0xc60
			 0xc6f0a46e, /// 0xc64
			 0x9453a278, /// 0xc68
			 0x9ada00a2, /// 0xc6c
			 0x3acbf851, /// 0xc70
			 0x005f0c80, /// 0xc74
			 0xd61df06c, /// 0xc78
			 0x3fe2ddf0, /// 0xc7c
			 0xbbd0ad49, /// 0xc80
			 0x792ded8c, /// 0xc84
			 0x532b1cc0, /// 0xc88
			 0x7c9ebc4d, /// 0xc8c
			 0x73994bb5, /// 0xc90
			 0x299d4844, /// 0xc94
			 0x055a0391, /// 0xc98
			 0x8e429484, /// 0xc9c
			 0x89d70c8c, /// 0xca0
			 0xea903f87, /// 0xca4
			 0xadd46056, /// 0xca8
			 0x21ba7cd2, /// 0xcac
			 0x47c619b3, /// 0xcb0
			 0x656642b1, /// 0xcb4
			 0x477138cc, /// 0xcb8
			 0x0f12f823, /// 0xcbc
			 0x9bb0b184, /// 0xcc0
			 0x3ab56757, /// 0xcc4
			 0x9576a2e9, /// 0xcc8
			 0xdcebd990, /// 0xccc
			 0xb98044c9, /// 0xcd0
			 0x81bc7e31, /// 0xcd4
			 0xe8cac20d, /// 0xcd8
			 0x59cd9591, /// 0xcdc
			 0x3b6a40a3, /// 0xce0
			 0x8d2aca8d, /// 0xce4
			 0xac30a118, /// 0xce8
			 0xa093990e, /// 0xcec
			 0x7f2f9b5e, /// 0xcf0
			 0xb0b6095c, /// 0xcf4
			 0xf583fa76, /// 0xcf8
			 0xdd3002ab, /// 0xcfc
			 0x0258bfba, /// 0xd00
			 0xcb5b981c, /// 0xd04
			 0x9f34de1a, /// 0xd08
			 0xe692ae27, /// 0xd0c
			 0x919fccb9, /// 0xd10
			 0xda77e309, /// 0xd14
			 0xa9fb8328, /// 0xd18
			 0x1f20d131, /// 0xd1c
			 0x242ce1fb, /// 0xd20
			 0x76be60e6, /// 0xd24
			 0xc64a7a51, /// 0xd28
			 0x69a19feb, /// 0xd2c
			 0x3f096623, /// 0xd30
			 0x19103ce7, /// 0xd34
			 0x36e53e6a, /// 0xd38
			 0xaf7af16c, /// 0xd3c
			 0x02401633, /// 0xd40
			 0x116efb1e, /// 0xd44
			 0x06ee90fd, /// 0xd48
			 0xea8b0753, /// 0xd4c
			 0x34fbc3f2, /// 0xd50
			 0xe5b227ab, /// 0xd54
			 0x698bd1cb, /// 0xd58
			 0x7b57f160, /// 0xd5c
			 0x47e21943, /// 0xd60
			 0x3ccb1551, /// 0xd64
			 0x1b9793a1, /// 0xd68
			 0x08a69dba, /// 0xd6c
			 0xd0df8174, /// 0xd70
			 0xf00cafd6, /// 0xd74
			 0x98cb7453, /// 0xd78
			 0x34646c2d, /// 0xd7c
			 0xa380d04a, /// 0xd80
			 0x434aeb63, /// 0xd84
			 0xfa52e6af, /// 0xd88
			 0xd8740df7, /// 0xd8c
			 0x0cb05f0b, /// 0xd90
			 0xbb4b16d4, /// 0xd94
			 0x6e3c4c7c, /// 0xd98
			 0x637674fb, /// 0xd9c
			 0x9ac18269, /// 0xda0
			 0x85245890, /// 0xda4
			 0xae491256, /// 0xda8
			 0x36cad2dd, /// 0xdac
			 0x9ccd4ab8, /// 0xdb0
			 0xf3709ef4, /// 0xdb4
			 0x36629f53, /// 0xdb8
			 0x88133666, /// 0xdbc
			 0x97a7e3a5, /// 0xdc0
			 0x88bb9656, /// 0xdc4
			 0xf58b0656, /// 0xdc8
			 0x5f39e884, /// 0xdcc
			 0x030bdfca, /// 0xdd0
			 0xde61e315, /// 0xdd4
			 0x308a8303, /// 0xdd8
			 0xb606432b, /// 0xddc
			 0xa7c0f15b, /// 0xde0
			 0xc08ee4b0, /// 0xde4
			 0x21f57cda, /// 0xde8
			 0xc8073239, /// 0xdec
			 0xf694392b, /// 0xdf0
			 0x87224f4e, /// 0xdf4
			 0x4bb951ce, /// 0xdf8
			 0x0d87d8fb, /// 0xdfc
			 0xaba0fa66, /// 0xe00
			 0x43333196, /// 0xe04
			 0xc0f10015, /// 0xe08
			 0xb07dfc58, /// 0xe0c
			 0x4d7d8ce6, /// 0xe10
			 0x1fabea29, /// 0xe14
			 0x6bd312be, /// 0xe18
			 0xd41761e5, /// 0xe1c
			 0xf4a58391, /// 0xe20
			 0xf2d93c42, /// 0xe24
			 0x249f307d, /// 0xe28
			 0x4cf4de81, /// 0xe2c
			 0xd68bf626, /// 0xe30
			 0xb5921313, /// 0xe34
			 0x2df17965, /// 0xe38
			 0x1e15b57d, /// 0xe3c
			 0x2c25bbde, /// 0xe40
			 0xf2bacd93, /// 0xe44
			 0x6adbafb0, /// 0xe48
			 0x44a4defd, /// 0xe4c
			 0xc97797b8, /// 0xe50
			 0x9cee293a, /// 0xe54
			 0x63fded9d, /// 0xe58
			 0x9a7f36ce, /// 0xe5c
			 0xee8d0f9a, /// 0xe60
			 0xcf86b872, /// 0xe64
			 0xd1b1b444, /// 0xe68
			 0xaae39a84, /// 0xe6c
			 0x720d7662, /// 0xe70
			 0xdf814229, /// 0xe74
			 0x452f6fcd, /// 0xe78
			 0x62d0a3ec, /// 0xe7c
			 0x2f962e46, /// 0xe80
			 0xcb284163, /// 0xe84
			 0xecf91003, /// 0xe88
			 0x967c414c, /// 0xe8c
			 0x45a64f24, /// 0xe90
			 0xe4453f95, /// 0xe94
			 0xe286d5c2, /// 0xe98
			 0xc67df9be, /// 0xe9c
			 0x02ac6ccc, /// 0xea0
			 0x1271d2a2, /// 0xea4
			 0xdafb253b, /// 0xea8
			 0x1e2b4678, /// 0xeac
			 0x230b7f84, /// 0xeb0
			 0x14f5a684, /// 0xeb4
			 0x52d47b70, /// 0xeb8
			 0xe29f30c7, /// 0xebc
			 0x343a355c, /// 0xec0
			 0x37ede5c8, /// 0xec4
			 0x531c1228, /// 0xec8
			 0xb30ee82f, /// 0xecc
			 0xdc073dee, /// 0xed0
			 0x828884da, /// 0xed4
			 0x48351e00, /// 0xed8
			 0xeb447c1b, /// 0xedc
			 0xc9cc83f7, /// 0xee0
			 0x4cba3cda, /// 0xee4
			 0xacbe1e51, /// 0xee8
			 0xc012d235, /// 0xeec
			 0x7b2d85a7, /// 0xef0
			 0xc43b77be, /// 0xef4
			 0x47a6173f, /// 0xef8
			 0xdd2c577b, /// 0xefc
			 0x2083c177, /// 0xf00
			 0xe89785b2, /// 0xf04
			 0xa48983c0, /// 0xf08
			 0x4dc7c38e, /// 0xf0c
			 0x3f1b672a, /// 0xf10
			 0xd4b2ade4, /// 0xf14
			 0x7587fc07, /// 0xf18
			 0xd3041b6d, /// 0xf1c
			 0x0b8fcd3f, /// 0xf20
			 0x50b93c22, /// 0xf24
			 0xf67664b2, /// 0xf28
			 0xbcef9418, /// 0xf2c
			 0x9cf6713c, /// 0xf30
			 0xae9c5d93, /// 0xf34
			 0x3a51422a, /// 0xf38
			 0x1f2b3093, /// 0xf3c
			 0x7a504c0e, /// 0xf40
			 0x54df6ee0, /// 0xf44
			 0x2870e01d, /// 0xf48
			 0x2f3dbd4d, /// 0xf4c
			 0x257f8c62, /// 0xf50
			 0xac2f2853, /// 0xf54
			 0x74b8bc63, /// 0xf58
			 0xf01a2e67, /// 0xf5c
			 0x9bfd717d, /// 0xf60
			 0x2c55dcc2, /// 0xf64
			 0xb5dcc7be, /// 0xf68
			 0xff04e438, /// 0xf6c
			 0x5d33378b, /// 0xf70
			 0x247bbab6, /// 0xf74
			 0xfc9512f1, /// 0xf78
			 0x07e1a096, /// 0xf7c
			 0x3aa39f4e, /// 0xf80
			 0x9aa9dfe6, /// 0xf84
			 0x65a25485, /// 0xf88
			 0xbf6954cb, /// 0xf8c
			 0x99eadd0a, /// 0xf90
			 0xd611542e, /// 0xf94
			 0x5c6d070f, /// 0xf98
			 0x5cd2403b, /// 0xf9c
			 0x68a2595d, /// 0xfa0
			 0x13bdba01, /// 0xfa4
			 0x95269c91, /// 0xfa8
			 0xb4df2942, /// 0xfac
			 0x8a8da0f1, /// 0xfb0
			 0x0af9e91b, /// 0xfb4
			 0x60580b05, /// 0xfb8
			 0xa14674da, /// 0xfbc
			 0x05f368f0, /// 0xfc0
			 0xf2ddebde, /// 0xfc4
			 0x366532ad, /// 0xfc8
			 0x842d5003, /// 0xfcc
			 0x28ef686d, /// 0xfd0
			 0xf3804705, /// 0xfd4
			 0xf5a31c6f, /// 0xfd8
			 0xb5738c78, /// 0xfdc
			 0x8d37abbf, /// 0xfe0
			 0x4d82ac1a, /// 0xfe4
			 0x34e95fec, /// 0xfe8
			 0x94fd9444, /// 0xfec
			 0x2a8d1fce, /// 0xff0
			 0x055933f2, /// 0xff4
			 0x5dfa828a, /// 0xff8
			 0xa2a58227 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00000
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00008
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0000c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00010
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00014
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00018
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00020
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00024
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00028
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0002c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00038
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00044
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00048
			 0x7f800000,                                                  // inf                                         /// 0004c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00050
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00054
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00058
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00060
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00068
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00070
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00078
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00080
			 0x7fc00001,                                                  // signaling NaN                               /// 00084
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0008c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00090
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00098
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0009c
			 0xbf028f5c,                                                  // -0.51                                       /// 000a0
			 0xcb000000,                                                  // -8388608.0                                  /// 000a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 000b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000b4
			 0x4b000000,                                                  // 8388608.0                                   /// 000b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x55555555,                                                  // 4 random values                             /// 000c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0x4b000000,                                                  // 8388608.0                                   /// 000e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00104
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00108
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0010c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00114
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00118
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00124
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00128
			 0x00000000,                                                  // zero                                        /// 0012c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00134
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00140
			 0x80000000,                                                  // -zero                                       /// 00144
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0014c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00158
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0015c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00168
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0016c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00170
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00174
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00178
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0017c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00184
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00188
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0018c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00194
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00198
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001c4
			 0xffc00001,                                                  // -signaling NaN                              /// 001c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00200
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00208
			 0x55555555,                                                  // 4 random values                             /// 0020c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00210
			 0x0c600000,                                                  // Leading 1s:                                 /// 00214
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00224
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00228
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0022c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00230
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00234
			 0x4b000000,                                                  // 8388608.0                                   /// 00238
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00240
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00244
			 0xcb000000,                                                  // -8388608.0                                  /// 00248
			 0x0c400000,                                                  // Leading 1s:                                 /// 0024c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x7f800000,                                                  // inf                                         /// 00258
			 0x0c400000,                                                  // Leading 1s:                                 /// 0025c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00260
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0026c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00270
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00278
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00280
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00284
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00288
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0028c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00290
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00294
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00298
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0029c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 002a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002b4
			 0xff800000,                                                  // -inf                                        /// 002b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00300
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00304
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00308
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0030c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00310
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00314
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00318
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0032c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0xbf028f5c,                                                  // -0.51                                       /// 00334
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00338
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0033c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x00000000,                                                  // zero                                        /// 0034c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00354
			 0x0e000007,                                                  // Trailing 1s:                                /// 00358
			 0x80011111,                                                  // -9.7958E-41                                 /// 0035c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00360
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00364
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0036c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00370
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00374
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00378
			 0x00000000,                                                  // zero                                        /// 0037c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00380
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00384
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00388
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0038c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00390
			 0x80000000,                                                  // -zero                                       /// 00394
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0039c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0x3f028f5c,                                                  // 0.51                                        /// 003b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 003c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0xbf028f5c,                                                  // -0.51                                       /// 003f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00404
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0040c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00410
			 0x00000000,                                                  // zero                                        /// 00414
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00428
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0042c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00430
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00434
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0043c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00440
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00448
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00454
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00464
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00468
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00470
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00474
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0047c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00480
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0048c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00490
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00494
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0049c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0c400000,                                                  // Leading 1s:                                 /// 004c0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004cc
			 0xffc00001,                                                  // -signaling NaN                              /// 004d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00500
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x3f028f5c,                                                  // 0.51                                        /// 00508
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0050c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00514
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00518
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0051c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0052c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00530
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00534
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0053c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00540
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00544
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0054c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00558
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0055c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0056c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00570
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00574
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00578
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0057c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00584
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x0e000001,                                                  // Trailing 1s:                                /// 0058c
			 0x7f800000,                                                  // inf                                         /// 00590
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00594
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00598
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005b4
			 0xcb000000,                                                  // -8388608.0                                  /// 005b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 005f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00600
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00604
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00608
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0060c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00610
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00618
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x80000000,                                                  // -zero                                       /// 00620
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00624
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00628
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0062c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00630
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00638
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00648
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0064c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00658
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0065c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00660
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00664
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00668
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0066c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00678
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0067c
			 0xffc00001,                                                  // -signaling NaN                              /// 00680
			 0x80011111,                                                  // -9.7958E-41                                 /// 00684
			 0x80000000,                                                  // -zero                                       /// 00688
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0068c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00690
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00694
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00698
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x33333333,                                                  // 4 random values                             /// 006c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00708
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00714
			 0x0e000003,                                                  // Trailing 1s:                                /// 00718
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0071c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00720
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00724
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00730
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00734
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0073c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00744
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00748
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0074c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00754
			 0x7f800000,                                                  // inf                                         /// 00758
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0075c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00760
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00768
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0076c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00770
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00780
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0078c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00790
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00794
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00798
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0079c
			 0x0c400000,                                                  // Leading 1s:                                 /// 007a0
			 0xff800000,                                                  // -inf                                        /// 007a4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007ac
			 0x80000000,                                                  // -zero                                       /// 007b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 007dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0xffc00001,                                                  // -signaling NaN                              /// 0080c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00810
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00814
			 0x00011111,                                                  // 9.7958E-41                                  /// 00818
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0081c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00820
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00824
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00830
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0083c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00840
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00844
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0084c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00854
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0085c
			 0x4b000000,                                                  // 8388608.0                                   /// 00860
			 0xff800000,                                                  // -inf                                        /// 00864
			 0x3f028f5c,                                                  // 0.51                                        /// 00868
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0086c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00874
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00878
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0087c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00884
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00888
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00890
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0089c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x00000000,                                                  // zero                                        /// 008e4
			 0x7f800000,                                                  // inf                                         /// 008e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00900
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00904
			 0x0e000001,                                                  // Trailing 1s:                                /// 00908
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0090c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00910
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00914
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00918
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0091c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00920
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00924
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0092c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00930
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00938
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0093c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00944
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00950
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00954
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00958
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0095c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00960
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00968
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00970
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00974
			 0x0c700000,                                                  // Leading 1s:                                 /// 00978
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00980
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00984
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00988
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00998
			 0x0e000003,                                                  // Trailing 1s:                                /// 0099c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009c0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a10
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a1c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a28
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a40
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a44
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a4c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a54
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a58
			 0x3f028f5c,                                                  // 0.51                                        /// 00a5c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a60
			 0x7fc00001,                                                  // signaling NaN                               /// 00a64
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x7f800000,                                                  // inf                                         /// 00a6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a70
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a78
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a8c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a90
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a94
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aa0
			 0x7f800000,                                                  // inf                                         /// 00aa4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00aa8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00aac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ab8
			 0x3f028f5c,                                                  // 0.51                                        /// 00abc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ac4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ad0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ad4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ad8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00adc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ae8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00aec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00af0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00af4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00af8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b08
			 0x3f028f5c,                                                  // 0.51                                        /// 00b0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b14
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x3f028f5c,                                                  // 0.51                                        /// 00b24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0x00000000,                                                  // zero                                        /// 00b2c
			 0x3f028f5c,                                                  // 0.51                                        /// 00b30
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b34
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b38
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b60
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b64
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b70
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b74
			 0xbf028f5c,                                                  // -0.51                                       /// 00b78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b7c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b8c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x55555555,                                                  // 4 random values                             /// 00b94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b98
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ba0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ba8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bb0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bb8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bbc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bc4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bcc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bd0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bd4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bd8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bdc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00be0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00be8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bf0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bf4
			 0x33333333,                                                  // 4 random values                             /// 00bf8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bfc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c28
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c3c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c44
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c4c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c50
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c58
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c6c
			 0xff800000,                                                  // -inf                                        /// 00c70
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c7c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c88
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ca4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x55555555,                                                  // 4 random values                             /// 00cb4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cd4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cdc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ce0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ce4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ce8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cf0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cf4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cf8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cfc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d00
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d04
			 0x3f028f5c,                                                  // 0.51                                        /// 00d08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d1c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d20
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d30
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d34
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d44
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d48
			 0xff800000,                                                  // -inf                                        /// 00d4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d64
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d74
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d78
			 0x55555555,                                                  // 4 random values                             /// 00d7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d84
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d88
			 0xff800000,                                                  // -inf                                        /// 00d8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d98
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d9c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00da0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00da8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00db0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00db4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00db8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dbc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dc0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dc4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dc8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dd0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dd4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dd8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00de4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00de8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00df0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00df4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00df8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dfc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e00
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e08
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e14
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x55555555,                                                  // 4 random values                             /// 00e1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e20
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e30
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e38
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e4c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e50
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e58
			 0xcb000000,                                                  // -8388608.0                                  /// 00e5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e64
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e68
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e6c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e78
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e8c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ea4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00eac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eb0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00eb4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ebc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ec0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ec4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ec8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ecc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ed0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ed4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ed8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00edc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ee0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ee4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x33333333,                                                  // 4 random values                             /// 00ef0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ef4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ef8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f00
			 0x33333333,                                                  // 4 random values                             /// 00f04
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f10
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f14
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f1c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f2c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f34
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f38
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f40
			 0xcb000000,                                                  // -8388608.0                                  /// 00f44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f4c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f50
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f68
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f6c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f88
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fa0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fa4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fb0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fb4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fbc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fc4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fd0
			 0x00000000,                                                  // zero                                        /// 00fd4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fd8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fdc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fe0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fe4
			 0x00000000,                                                  // zero                                        /// 00fe8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ff0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ff4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ff8
			 0x0e0003ff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00000
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x80011111,                                                  // -9.7958E-41                                 /// 0000c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00018
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0001c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00020
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00024
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x80000000,                                                  // -zero                                       /// 00034
			 0x55555555,                                                  // 4 random values                             /// 00038
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0003c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00040
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00044
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0004c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00050
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00054
			 0xcb000000,                                                  // -8388608.0                                  /// 00058
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0005c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00060
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00064
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x0e000001,                                                  // Trailing 1s:                                /// 0006c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00074
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00080
			 0x55555555,                                                  // 4 random values                             /// 00084
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00088
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0008c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00090
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00094
			 0x0e000001,                                                  // Trailing 1s:                                /// 00098
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0009c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000b0
			 0x7fc00001,                                                  // signaling NaN                               /// 000b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0xffc00001,                                                  // -signaling NaN                              /// 000c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00100
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0010c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00110
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00114
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00118
			 0xffc00001,                                                  // -signaling NaN                              /// 0011c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00120
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00124
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00134
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00138
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00140
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00144
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00148
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00154
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0015c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0016c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00170
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00174
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0017c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00180
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00184
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00188
			 0x00011111,                                                  // 9.7958E-41                                  /// 0018c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00190
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00194
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00198
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0019c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001ac
			 0x00000000,                                                  // zero                                        /// 001b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001f8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00200
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00204
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0020c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00214
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00224
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00228
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00230
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00234
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0023c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00240
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00248
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0xffc00001,                                                  // -signaling NaN                              /// 00250
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00254
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00258
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00260
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00264
			 0xcb000000,                                                  // -8388608.0                                  /// 00268
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00270
			 0xbf028f5c,                                                  // -0.51                                       /// 00274
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00278
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0027c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00284
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00288
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0028c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00290
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00298
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0029c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002b8
			 0x00000000,                                                  // zero                                        /// 002bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 002e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00300
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00304
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00308
			 0xbf028f5c,                                                  // -0.51                                       /// 0030c
			 0x00000000,                                                  // zero                                        /// 00310
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00314
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00318
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0031c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x4b000000,                                                  // 8388608.0                                   /// 00324
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0032c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00330
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00338
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0033c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00340
			 0x00000000,                                                  // zero                                        /// 00344
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00348
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0034c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00350
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00354
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0035c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00360
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00364
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0036c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00378
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00380
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00384
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00388
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0038c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00390
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00394
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00398
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0039c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003c8
			 0x33333333,                                                  // 4 random values                             /// 003cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00400
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00404
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x33333333,                                                  // 4 random values                             /// 0040c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00410
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00418
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0041c
			 0xffc00001,                                                  // -signaling NaN                              /// 00420
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00424
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x00000000,                                                  // zero                                        /// 0042c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00434
			 0x0c400000,                                                  // Leading 1s:                                 /// 00438
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0043c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00440
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00444
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0044c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00460
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00464
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0c700000,                                                  // Leading 1s:                                 /// 0046c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00470
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00474
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00478
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00480
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0x4b000000,                                                  // 8388608.0                                   /// 0048c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00490
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00494
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0049c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0xcb000000,                                                  // -8388608.0                                  /// 004c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004e0
			 0xff800000,                                                  // -inf                                        /// 004e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004f8
			 0xbf028f5c,                                                  // -0.51                                       /// 004fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00500
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00508
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00510
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0051c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x80011111,                                                  // -9.7958E-41                                 /// 00524
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00528
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0052c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00534
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00538
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0053c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0e000001,                                                  // Trailing 1s:                                /// 00548
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0054c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00550
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00554
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00558
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00564
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00568
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00570
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00578
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00580
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00584
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00588
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0058c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00590
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00594
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x33333333,                                                  // 4 random values                             /// 0059c
			 0x80011111,                                                  // -9.7958E-41                                 /// 005a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005d4
			 0x00000000,                                                  // zero                                        /// 005d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 005f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00600
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00608
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0060c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00610
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0061c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00620
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00628
			 0x33333333,                                                  // 4 random values                             /// 0062c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00630
			 0x00000000,                                                  // zero                                        /// 00634
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00638
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0063c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00640
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00644
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0064c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00650
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00654
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00658
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00664
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00668
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x80000000,                                                  // -zero                                       /// 00670
			 0xffc00001,                                                  // -signaling NaN                              /// 00674
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0067c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00680
			 0x00011111,                                                  // 9.7958E-41                                  /// 00684
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00688
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0068c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00690
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00698
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0069c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x00000000,                                                  // zero                                        /// 006ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006c0
			 0xcb000000,                                                  // -8388608.0                                  /// 006c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0xcb000000,                                                  // -8388608.0                                  /// 006ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00700
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00704
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00708
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0070c
			 0x33333333,                                                  // 4 random values                             /// 00710
			 0x55555555,                                                  // 4 random values                             /// 00714
			 0x0c400000,                                                  // Leading 1s:                                 /// 00718
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0071c
			 0xff800000,                                                  // -inf                                        /// 00720
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00724
			 0x0c400000,                                                  // Leading 1s:                                 /// 00728
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00730
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00734
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00738
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0073c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00744
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00748
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00750
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00754
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00758
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0075c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00760
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00768
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00778
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0077c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00784
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00788
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0078c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00794
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00798
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007b0
			 0x33333333,                                                  // 4 random values                             /// 007b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007f8
			 0x7fc00001,                                                  // signaling NaN                               /// 007fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00800
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00808
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0080c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00810
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00818
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0081c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00820
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0082c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0e000001,                                                  // Trailing 1s:                                /// 00834
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0083c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00844
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0084c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00850
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x7f800000,                                                  // inf                                         /// 00858
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0085c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00860
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00864
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00870
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00874
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00878
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0087c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00884
			 0xffc00001,                                                  // -signaling NaN                              /// 00888
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00890
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 008cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x7fc00001,                                                  // signaling NaN                               /// 008dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008f4
			 0x7f800000,                                                  // inf                                         /// 008f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00904
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00910
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00918
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00920
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00924
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00930
			 0x0e000007,                                                  // Trailing 1s:                                /// 00934
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00938
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00940
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00944
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00948
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0094c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00950
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00954
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00958
			 0x55555555,                                                  // 4 random values                             /// 0095c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00960
			 0x4b000000,                                                  // 8388608.0                                   /// 00964
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0096c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00970
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0097c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00980
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00988
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0098c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00990
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00994
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00998
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0099c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c4
			 0x55555555,                                                  // 4 random values                             /// 009c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00a00
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a0c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a1c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a30
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a38
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a40
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a48
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a4c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a60
			 0x3f028f5c,                                                  // 0.51                                        /// 00a64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a74
			 0xcb000000,                                                  // -8388608.0                                  /// 00a78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a7c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a80
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a98
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00aa4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ab0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ab8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00abc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ac0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00acc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00adc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ae0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ae4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00aec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00af0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00af4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00af8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00afc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b04
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b0c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b14
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x80000000,                                                  // -zero                                       /// 00b1c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b30
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b34
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b40
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b50
			 0xff800000,                                                  // -inf                                        /// 00b54
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b5c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b68
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b70
			 0xbf028f5c,                                                  // -0.51                                       /// 00b74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b88
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b8c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ba0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ba4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ba8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bb0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0xbf028f5c,                                                  // -0.51                                       /// 00bbc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bc0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bc4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bc8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bcc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00be4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be8
			 0xbf028f5c,                                                  // -0.51                                       /// 00bec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bf0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bf4
			 0x3f028f5c,                                                  // 0.51                                        /// 00bf8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bfc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c0c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c14
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c18
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c38
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c58
			 0xbf028f5c,                                                  // -0.51                                       /// 00c5c
			 0x80000000,                                                  // -zero                                       /// 00c60
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c68
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c6c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c74
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80000000,                                                  // -zero                                       /// 00ca8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cb0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cb8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cbc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cc0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cd0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cd4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ce4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ce8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cf4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cf8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cfc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d04
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d14
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d18
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d24
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d28
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d48
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d60
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d64
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d74
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d7c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d90
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d94
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00da0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00da8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dac
			 0x33333333,                                                  // 4 random values                             /// 00db0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00db4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00db8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dbc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x3f028f5c,                                                  // 0.51                                        /// 00dc4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dd0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dd8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00de0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00de8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00df0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00df4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00df8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dfc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e00
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e10
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e18
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e28
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e34
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e3c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e40
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e44
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x55555555,                                                  // 4 random values                             /// 00e58
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e6c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e70
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e84
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e90
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e94
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ea0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ea8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00eac
			 0x7fc00001,                                                  // signaling NaN                               /// 00eb0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00eb4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ec0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ec4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ec8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ecc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ed0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ed4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ed8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ee0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ee4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00eec
			 0x55555555,                                                  // 4 random values                             /// 00ef0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ef8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00efc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f14
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f1c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f20
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f28
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f3c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f54
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f68
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f78
			 0x3f028f5c,                                                  // 0.51                                        /// 00f7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f84
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f88
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f94
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f9c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fa4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fa8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fb0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fb4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fb8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fc0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fcc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fd4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fd8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fe4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ff0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ff8
			 0xff000000 // norm with max exp min mant                   // SP - ve numbers                             /// last
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
			 0x00000018,
			 0x000005c4,
			 0x00000224,
			 0x000007a4,
			 0x00000498,
			 0x000002b8,
			 0x000006c0,
			 0x00000018,

			 /// vpu register f2
			 0x41a80000,
			 0x41200000,
			 0x41c80000,
			 0x3f800000,
			 0x41e80000,
			 0x41000000,
			 0x40400000,
			 0x41200000,

			 /// vpu register f3
			 0x41300000,
			 0x41e80000,
			 0x41f00000,
			 0x40e00000,
			 0x41400000,
			 0x40000000,
			 0x41100000,
			 0x41800000,

			 /// vpu register f4
			 0x41000000,
			 0x41e80000,
			 0x41900000,
			 0x41d80000,
			 0x41300000,
			 0x42000000,
			 0x40a00000,
			 0x41200000,

			 /// vpu register f5
			 0x41c00000,
			 0x41e80000,
			 0x41d00000,
			 0x41900000,
			 0x41500000,
			 0x41880000,
			 0x3f800000,
			 0x41f80000,

			 /// vpu register f6
			 0x41500000,
			 0x40000000,
			 0x41980000,
			 0x41b00000,
			 0x41a00000,
			 0x41800000,
			 0x41f00000,
			 0x40e00000,

			 /// vpu register f7
			 0x41800000,
			 0x41c00000,
			 0x40a00000,
			 0x41800000,
			 0x41000000,
			 0x41f80000,
			 0x41300000,
			 0x41300000,

			 /// vpu register f8
			 0x41400000,
			 0x40800000,
			 0x41b00000,
			 0x41f80000,
			 0x41f00000,
			 0x41b80000,
			 0x41f80000,
			 0x41700000,

			 /// vpu register f9
			 0x40400000,
			 0x41d00000,
			 0x41e80000,
			 0x40a00000,
			 0x41c80000,
			 0x41880000,
			 0x41e80000,
			 0x41400000,

			 /// vpu register f10
			 0x41e00000,
			 0x41400000,
			 0x41880000,
			 0x41800000,
			 0x41600000,
			 0x41b80000,
			 0x41a00000,
			 0x41d80000,

			 /// vpu register f11
			 0x41a00000,
			 0x41d80000,
			 0x40c00000,
			 0x42000000,
			 0x41e00000,
			 0x41200000,
			 0x41980000,
			 0x41300000,

			 /// vpu register f12
			 0x40000000,
			 0x41b80000,
			 0x41e00000,
			 0x41f00000,
			 0x41f00000,
			 0x40c00000,
			 0x41b80000,
			 0x41d00000,

			 /// vpu register f13
			 0x41e80000,
			 0x3f800000,
			 0x41f00000,
			 0x40000000,
			 0x41000000,
			 0x41600000,
			 0x41b00000,
			 0x41980000,

			 /// vpu register f14
			 0x40c00000,
			 0x3f800000,
			 0x41100000,
			 0x41b80000,
			 0x40400000,
			 0x41d00000,
			 0x41980000,
			 0x41e80000,

			 /// vpu register f15
			 0x41e80000,
			 0x41300000,
			 0x40800000,
			 0x41500000,
			 0x40c00000,
			 0x41900000,
			 0x41200000,
			 0x41d80000,

			 /// vpu register f16
			 0x41200000,
			 0x42000000,
			 0x41c80000,
			 0x40800000,
			 0x41b00000,
			 0x41800000,
			 0x41980000,
			 0x41980000,

			 /// vpu register f17
			 0x41900000,
			 0x41f80000,
			 0x40e00000,
			 0x3f800000,
			 0x40800000,
			 0x41980000,
			 0x41b80000,
			 0x41d00000,

			 /// vpu register f18
			 0x41a00000,
			 0x41800000,
			 0x41f00000,
			 0x40c00000,
			 0x40000000,
			 0x41980000,
			 0x41a80000,
			 0x41100000,

			 /// vpu register f19
			 0x41c00000,
			 0x41f80000,
			 0x41f80000,
			 0x41f00000,
			 0x41700000,
			 0x41880000,
			 0x41500000,
			 0x41e80000,

			 /// vpu register f20
			 0x41400000,
			 0x41700000,
			 0x41a80000,
			 0x40c00000,
			 0x41b80000,
			 0x41d80000,
			 0x41500000,
			 0x41800000,

			 /// vpu register f21
			 0x42000000,
			 0x41800000,
			 0x41000000,
			 0x41300000,
			 0x41100000,
			 0x41d80000,
			 0x41200000,
			 0x3f800000,

			 /// vpu register f22
			 0x41b00000,
			 0x41f80000,
			 0x41a00000,
			 0x40000000,
			 0x41a00000,
			 0x40000000,
			 0x41800000,
			 0x41900000,

			 /// vpu register f23
			 0x41700000,
			 0x41f80000,
			 0x41900000,
			 0x41000000,
			 0x41000000,
			 0x41e80000,
			 0x40a00000,
			 0x41b80000,

			 /// vpu register f24
			 0x41980000,
			 0x41200000,
			 0x41900000,
			 0x41d00000,
			 0x41300000,
			 0x41700000,
			 0x41100000,
			 0x41800000,

			 /// vpu register f25
			 0x41a80000,
			 0x41700000,
			 0x40a00000,
			 0x41100000,
			 0x40000000,
			 0x40e00000,
			 0x41100000,
			 0x41880000,

			 /// vpu register f26
			 0x3f800000,
			 0x41a80000,
			 0x41a00000,
			 0x41100000,
			 0x41f80000,
			 0x40800000,
			 0x41e80000,
			 0x41300000,

			 /// vpu register f27
			 0x41d80000,
			 0x41100000,
			 0x40800000,
			 0x41b00000,
			 0x41b00000,
			 0x41000000,
			 0x41e00000,
			 0x41300000,

			 /// vpu register f28
			 0x42000000,
			 0x41e80000,
			 0x40800000,
			 0x41c80000,
			 0x41f00000,
			 0x41500000,
			 0x41d80000,
			 0x41700000,

			 /// vpu register f29
			 0x40c00000,
			 0x41800000,
			 0x3f800000,
			 0x41000000,
			 0x41400000,
			 0x40c00000,
			 0x41d80000,
			 0x3f800000,

			 /// vpu register f30
			 0x40000000,
			 0x41e80000,
			 0x41f80000,
			 0x41880000,
			 0x41200000,
			 0x41c80000,
			 0x41400000,
			 0x40a00000,

			 /// vpu register f31
			 0x40800000,
			 0x41200000,
			 0x41400000,
			 0x41a00000,
			 0x41b00000,
			 0x3f800000,
			 0x41f00000,
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
		"fsw.ps f4, 1181(x13)\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f23, 332(x12)\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f13, -171(x14)\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f20, -1795(x15)\n"                            ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f24, -2045(x15)\n"                            ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f27, 1747(x11)\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f5, 733(x12)\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f29, 604(x11)\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f9, 502(x14)\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f26, -914(x15)\n"                             ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f11, 976(x12)\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f4, 550(x13)\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f13, 1532(x15)\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f29, 1941(x12)\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f22, -1194(x14)\n"                            ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f4, 18(x15)\n"                                ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f9, 1825(x11)\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f7, -1668(x12)\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f15, 1759(x11)\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f15, 123(x12)\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f25, -716(x11)\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f1, -146(x14)\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f21, 1718(x15)\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f30, 632(x13)\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f11, 501(x13)\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f23, 340(x15)\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f15, 1412(x12)\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f23, -1479(x12)\n"                            ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f1, -717(x12)\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f0, 1372(x14)\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f28, 35(x14)\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f18, -574(x13)\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f16, 139(x14)\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f26, 1328(x13)\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f0, -1699(x15)\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f28, 776(x12)\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f9, -979(x15)\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f7, 634(x15)\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f3, -637(x12)\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f8, -1720(x13)\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f1, -79(x12)\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f0, 1164(x14)\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f13, -321(x15)\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f28, -548(x11)\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f28, -717(x12)\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f21, -1613(x12)\n"                            ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f22, -1889(x14)\n"                            ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f14, -1900(x13)\n"                            ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f30, -556(x13)\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f27, 14(x13)\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f9, -1074(x14)\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f4, -53(x13)\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f15, -1750(x12)\n"                            ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f17, 290(x14)\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f23, -160(x12)\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f29, -302(x14)\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f25, -1892(x15)\n"                            ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f15, -1919(x12)\n"                            ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f23, -1723(x11)\n"                            ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f13, -398(x11)\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f28, -599(x11)\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f30, 1198(x15)\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f30, -530(x15)\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f23, -624(x14)\n"                             ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f27, 136(x15)\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f23, 1926(x13)\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f29, 1499(x12)\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f27, 2030(x15)\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f23, 1040(x15)\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f1, -1479(x13)\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f25, -1713(x12)\n"                            ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f15, -26(x14)\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f25, 769(x15)\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f9, 864(x13)\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f26, 1918(x11)\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f16, -1971(x13)\n"                            ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f10, -1972(x14)\n"                            ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f16, 1486(x13)\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f13, 1647(x15)\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f1, 1085(x13)\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f12, 286(x11)\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f6, 1828(x11)\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f9, 670(x11)\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f19, -2010(x15)\n"                            ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f10, 1900(x15)\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f1, -1377(x14)\n"                             ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f29, -922(x15)\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f3, 2028(x14)\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f18, -995(x15)\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f26, 1181(x12)\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f9, -70(x12)\n"                               ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f23, -310(x13)\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f23, -283(x11)\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f22, -331(x14)\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f11, 361(x15)\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f22, -1010(x12)\n"                            ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f13, 1920(x14)\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f25, 1801(x15)\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsw.ps f7, -86(x14)\n"                               ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
