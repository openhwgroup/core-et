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
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00000
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00004
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00008
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0000c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00010
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00014
			 0x80000000,                                                  // -zero                                       /// 00018
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00020
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00024
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0e000001,                                                  // Trailing 1s:                                /// 0002c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00030
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00034
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00038
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0003c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00044
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00048
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0004c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00054
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00058
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00068
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00074
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00078
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0x00000000,                                                  // zero                                        /// 00080
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0x00000000,                                                  // zero                                        /// 0008c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00090
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00094
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00098
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0009c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00100
			 0xcb000000,                                                  // -8388608.0                                  /// 00104
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00108
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0011c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00124
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00128
			 0x33333333,                                                  // 4 random values                             /// 0012c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00130
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00134
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00138
			 0x0c600000,                                                  // Leading 1s:                                 /// 0013c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00144
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00148
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00150
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00154
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00158
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0015c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00160
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00168
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0016c
			 0x80000000,                                                  // -zero                                       /// 00170
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00174
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00178
			 0x7fc00001,                                                  // signaling NaN                               /// 0017c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00180
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0xff800000,                                                  // -inf                                        /// 00188
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00190
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00194
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00198
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0019c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00204
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0020c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00210
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00214
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00218
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00220
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00224
			 0x0e000003,                                                  // Trailing 1s:                                /// 00228
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00230
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x4b000000,                                                  // 8388608.0                                   /// 00238
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00244
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00250
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0025c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00260
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00264
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0026c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0e000001,                                                  // Trailing 1s:                                /// 0028c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00290
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00300
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00304
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00308
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0030c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00310
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x0c780000,                                                  // Leading 1s:                                 /// 00318
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0031c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00320
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00328
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0032c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00330
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00334
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00338
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00348
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0034c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00350
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00358
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0035c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00360
			 0x0c780000,                                                  // Leading 1s:                                 /// 00364
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00368
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0036c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00370
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00374
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00378
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00384
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00388
			 0x7f800000,                                                  // inf                                         /// 0038c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00394
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00398
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0039c
			 0x0e000001,                                                  // Trailing 1s:                                /// 003a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003c8
			 0x7fc00001,                                                  // signaling NaN                               /// 003cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 003d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003e0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 003f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00404
			 0x0c700000,                                                  // Leading 1s:                                 /// 00408
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00410
			 0x0c700000,                                                  // Leading 1s:                                 /// 00414
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00418
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0041c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00420
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x7f800000,                                                  // inf                                         /// 00428
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0042c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00430
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00438
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0e000001,                                                  // Trailing 1s:                                /// 0044c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x7f800000,                                                  // inf                                         /// 00454
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00458
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0045c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00460
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00464
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00468
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0046c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00470
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00474
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00478
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00480
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00484
			 0x33333333,                                                  // 4 random values                             /// 00488
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0049c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0xffc00001,                                                  // -signaling NaN                              /// 004b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0xff800000,                                                  // -inf                                        /// 004c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004e8
			 0x7f800000,                                                  // inf                                         /// 004ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004f8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00508
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0050c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00510
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00514
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0051c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00520
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0052c
			 0x7fc00001,                                                  // signaling NaN                               /// 00530
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00538
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00540
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00548
			 0x80011111,                                                  // -9.7958E-41                                 /// 0054c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x7fc00001,                                                  // signaling NaN                               /// 0055c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00560
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00568
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0056c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00570
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00574
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00578
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00580
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00584
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00588
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00594
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0059c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0xcb000000,                                                  // -8388608.0                                  /// 00604
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x00000000,                                                  // zero                                        /// 0060c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00610
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00614
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00618
			 0xff800000,                                                  // -inf                                        /// 0061c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00620
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00624
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00630
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00634
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00638
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00640
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00644
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00648
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0064c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00650
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00654
			 0x3f028f5c,                                                  // 0.51                                        /// 00658
			 0x0e000007,                                                  // Trailing 1s:                                /// 0065c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00668
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0066c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00674
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00678
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0067c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00684
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00688
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00694
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00698
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0069c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 006c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x0e000003,                                                  // Trailing 1s:                                /// 006f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00704
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0070c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00710
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00714
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00718
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x3f028f5c,                                                  // 0.51                                        /// 00724
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0072c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00730
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x7fc00001,                                                  // signaling NaN                               /// 00738
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0073c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00740
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00744
			 0xff800000,                                                  // -inf                                        /// 00748
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0074c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00754
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00758
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0075c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00760
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00764
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00770
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00778
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0xffc00001,                                                  // -signaling NaN                              /// 00780
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00784
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00788
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0078c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00790
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0079c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007b8
			 0xcb000000,                                                  // -8388608.0                                  /// 007bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007cc
			 0x3f028f5c,                                                  // 0.51                                        /// 007d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 007f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00800
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00804
			 0x3f028f5c,                                                  // 0.51                                        /// 00808
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0080c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00810
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00814
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00818
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00820
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00828
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00830
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00834
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00838
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00844
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00848
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00850
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00854
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00858
			 0x0e000007,                                                  // Trailing 1s:                                /// 0085c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0086c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00874
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0087c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00880
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00884
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00888
			 0x7fc00001,                                                  // signaling NaN                               /// 0088c
			 0xff800000,                                                  // -inf                                        /// 00890
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00894
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00898
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 008ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008b8
			 0x7f800000,                                                  // inf                                         /// 008bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0xff800000,                                                  // -inf                                        /// 008ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008f8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00900
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00904
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00908
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00910
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00914
			 0x3f028f5c,                                                  // 0.51                                        /// 00918
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00924
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00928
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00934
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00938
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x00000000,                                                  // zero                                        /// 00944
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00948
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0094c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00950
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00954
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0095c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00964
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00970
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0x80011111,                                                  // -9.7958E-41                                 /// 00978
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x0c780000,                                                  // Leading 1s:                                 /// 00988
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0098c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00990
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00994
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0099c
			 0x0e000003,                                                  // Trailing 1s:                                /// 009a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 009c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0xffc00001,                                                  // -signaling NaN                              /// 009f4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a00
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a0c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a10
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a2c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a30
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a34
			 0x7f800000,                                                  // inf                                         /// 00a38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a40
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a50
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a54
			 0xcb000000,                                                  // -8388608.0                                  /// 00a58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x3f028f5c,                                                  // 0.51                                        /// 00a64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a74
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x4b000000,                                                  // 8388608.0                                   /// 00a7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a84
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a9c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00aa0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00aa4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ab0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ab8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00acc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ad4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ad8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ae4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ae8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00aec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00af0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00afc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b28
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b2c
			 0x33333333,                                                  // 4 random values                             /// 00b30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b34
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0xbf028f5c,                                                  // -0.51                                       /// 00b58
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b7c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b88
			 0xcb000000,                                                  // -8388608.0                                  /// 00b8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b9c
			 0x7f800000,                                                  // inf                                         /// 00ba0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ba8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bc0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bc4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bc8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bdc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00be0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00be4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x7fc00001,                                                  // signaling NaN                               /// 00bf8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c0c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c24
			 0x00000000,                                                  // zero                                        /// 00c28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x4b000000,                                                  // 8388608.0                                   /// 00c44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c50
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0xcb000000,                                                  // -8388608.0                                  /// 00c64
			 0x55555555,                                                  // 4 random values                             /// 00c68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c88
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c8c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c90
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ca0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ca4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cb0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cb4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cb8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cbc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cc0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cc4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cc8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cdc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ce4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ce8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cf0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cf4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cf8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d08
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d0c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d2c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d3c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d50
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d54
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x7fc00001,                                                  // signaling NaN                               /// 00d84
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d94
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d98
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00da0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00da4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00da8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00db0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00db4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00db8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dc0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dc4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dcc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dd0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dd4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ddc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00de0
			 0xff800000,                                                  // -inf                                        /// 00de4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00de8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00df8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dfc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e00
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e04
			 0xff800000,                                                  // -inf                                        /// 00e08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e18
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e28
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e40
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e4c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e58
			 0x3f028f5c,                                                  // 0.51                                        /// 00e5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e60
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e6c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e70
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e78
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e80
			 0x80000000,                                                  // -zero                                       /// 00e84
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e98
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ea0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ea8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00eb0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00eb8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ebc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ec0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ec4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ec8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ecc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ed4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ed8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00edc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ee0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ee4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ee8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ef0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ef4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ef8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f0c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f18
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f38
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f3c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f74
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f7c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f80
			 0xcb000000,                                                  // -8388608.0                                  /// 00f84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f94
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f9c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fa0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fb4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fc8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fdc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fe0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fe4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fe8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ff8
			 0x0e001fff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x372a3512, /// 0x0
			 0x2265644c, /// 0x4
			 0x3b4c36b9, /// 0x8
			 0xb86e5df2, /// 0xc
			 0x5e7fd0af, /// 0x10
			 0x2ffacc8c, /// 0x14
			 0x91e51b40, /// 0x18
			 0xb3fb4392, /// 0x1c
			 0x62a3475b, /// 0x20
			 0x7d9c54f8, /// 0x24
			 0x16f20bd2, /// 0x28
			 0xaaceb584, /// 0x2c
			 0x331637fa, /// 0x30
			 0x1cdb8efd, /// 0x34
			 0xddaa430a, /// 0x38
			 0x9c2f8b80, /// 0x3c
			 0x5b135a15, /// 0x40
			 0xc5a5ff94, /// 0x44
			 0xeb97ee2d, /// 0x48
			 0x77d0e433, /// 0x4c
			 0x62ad38c0, /// 0x50
			 0xad96ab66, /// 0x54
			 0x5a6c1113, /// 0x58
			 0xd2031b46, /// 0x5c
			 0xd65ed5ae, /// 0x60
			 0xa65bb102, /// 0x64
			 0x84539fc7, /// 0x68
			 0xb7b6cda2, /// 0x6c
			 0xc3ffcd40, /// 0x70
			 0xedb6adfc, /// 0x74
			 0x48a82767, /// 0x78
			 0x4d5a8840, /// 0x7c
			 0x5eb530c1, /// 0x80
			 0x52e625a6, /// 0x84
			 0xa919a9ce, /// 0x88
			 0xb915d098, /// 0x8c
			 0x829f9c77, /// 0x90
			 0x6b5c8704, /// 0x94
			 0xd1f30851, /// 0x98
			 0xeb910757, /// 0x9c
			 0x9612bbae, /// 0xa0
			 0xb7e695c4, /// 0xa4
			 0x56998cbf, /// 0xa8
			 0xf99606cd, /// 0xac
			 0x761d6889, /// 0xb0
			 0x88d5283d, /// 0xb4
			 0x9982c9b4, /// 0xb8
			 0x8f38327e, /// 0xbc
			 0x47faacdf, /// 0xc0
			 0x4ce8d330, /// 0xc4
			 0x9dc8f27e, /// 0xc8
			 0xe1935b7a, /// 0xcc
			 0x4c1a5d83, /// 0xd0
			 0x4cbe6c1a, /// 0xd4
			 0x3dcef831, /// 0xd8
			 0xb61fca26, /// 0xdc
			 0x0965121c, /// 0xe0
			 0x34ea9b32, /// 0xe4
			 0x4883ef9d, /// 0xe8
			 0x22c1ea7a, /// 0xec
			 0xba1941f2, /// 0xf0
			 0x1ad68889, /// 0xf4
			 0x7bbe222e, /// 0xf8
			 0xa2616e92, /// 0xfc
			 0xc53e6cff, /// 0x100
			 0x9db41900, /// 0x104
			 0x48e35c6f, /// 0x108
			 0xdcd9bf8d, /// 0x10c
			 0x81d1c151, /// 0x110
			 0x99de3615, /// 0x114
			 0x54bcb5f2, /// 0x118
			 0x008ee6d5, /// 0x11c
			 0xdd93f83c, /// 0x120
			 0xe5a83fcf, /// 0x124
			 0x7f07b134, /// 0x128
			 0x9ee9237d, /// 0x12c
			 0x3e33f7dc, /// 0x130
			 0x2379b11a, /// 0x134
			 0x2757716a, /// 0x138
			 0x669f66db, /// 0x13c
			 0x432adaff, /// 0x140
			 0x0887cf45, /// 0x144
			 0x73929fb7, /// 0x148
			 0x1b6558b8, /// 0x14c
			 0x50ed21fb, /// 0x150
			 0x9d1819ae, /// 0x154
			 0x10dcbfec, /// 0x158
			 0x9013a6ad, /// 0x15c
			 0x9d93668a, /// 0x160
			 0x7c6c00ba, /// 0x164
			 0xc9e084ba, /// 0x168
			 0x46c72f8b, /// 0x16c
			 0x08cd79ab, /// 0x170
			 0xee819273, /// 0x174
			 0xc49900fe, /// 0x178
			 0x1ec00dd4, /// 0x17c
			 0xf164dd6b, /// 0x180
			 0x3755592a, /// 0x184
			 0x50601a3e, /// 0x188
			 0x11405bdd, /// 0x18c
			 0xe90acfb5, /// 0x190
			 0xb2752920, /// 0x194
			 0x32f50ca1, /// 0x198
			 0x400e6a5a, /// 0x19c
			 0xe199926b, /// 0x1a0
			 0x65dbae75, /// 0x1a4
			 0xc9586269, /// 0x1a8
			 0x95350f07, /// 0x1ac
			 0x1f5156d4, /// 0x1b0
			 0x0f78078e, /// 0x1b4
			 0x88877be8, /// 0x1b8
			 0x53b58a26, /// 0x1bc
			 0x869731d5, /// 0x1c0
			 0xb87c8eaf, /// 0x1c4
			 0xceb74606, /// 0x1c8
			 0xf71594c7, /// 0x1cc
			 0xceb01f87, /// 0x1d0
			 0xf0f56c69, /// 0x1d4
			 0x944b68ac, /// 0x1d8
			 0xa595a521, /// 0x1dc
			 0x43871297, /// 0x1e0
			 0x638bda0f, /// 0x1e4
			 0x43afd085, /// 0x1e8
			 0x63ed8bff, /// 0x1ec
			 0x643ec5e7, /// 0x1f0
			 0x7e8177c8, /// 0x1f4
			 0x93b65cee, /// 0x1f8
			 0xf551614a, /// 0x1fc
			 0xf64a0c57, /// 0x200
			 0xbbe241f2, /// 0x204
			 0x3416657a, /// 0x208
			 0x845678fc, /// 0x20c
			 0xd207bb91, /// 0x210
			 0xc8653ee9, /// 0x214
			 0x6eeb6fcc, /// 0x218
			 0xc5382ab8, /// 0x21c
			 0xedeb8506, /// 0x220
			 0x79cbca64, /// 0x224
			 0x10b55767, /// 0x228
			 0x1914f01e, /// 0x22c
			 0xaeb220fc, /// 0x230
			 0xb3ba8c0c, /// 0x234
			 0x03cf9d2c, /// 0x238
			 0x3d7d6c4a, /// 0x23c
			 0xbe390229, /// 0x240
			 0x148e8542, /// 0x244
			 0x99cc4658, /// 0x248
			 0xa0154caf, /// 0x24c
			 0xc969babd, /// 0x250
			 0xea08a74f, /// 0x254
			 0x713b4c18, /// 0x258
			 0xfa360def, /// 0x25c
			 0x98d79e2c, /// 0x260
			 0x021159bc, /// 0x264
			 0x0726019c, /// 0x268
			 0xcaee8a07, /// 0x26c
			 0xa0571b66, /// 0x270
			 0xb7adb41d, /// 0x274
			 0x665242d4, /// 0x278
			 0xe5a229e0, /// 0x27c
			 0x72b75cea, /// 0x280
			 0xb2286a49, /// 0x284
			 0xea0d924d, /// 0x288
			 0x5954e075, /// 0x28c
			 0x8003f493, /// 0x290
			 0xd0e4ce93, /// 0x294
			 0xef3ee26c, /// 0x298
			 0xd20aa613, /// 0x29c
			 0x78cf536f, /// 0x2a0
			 0xe3b9d582, /// 0x2a4
			 0x9977cd14, /// 0x2a8
			 0xf967aab7, /// 0x2ac
			 0x0e041e10, /// 0x2b0
			 0x3c450312, /// 0x2b4
			 0xc34a4f66, /// 0x2b8
			 0x7c2003d7, /// 0x2bc
			 0x6a86b82e, /// 0x2c0
			 0x7cd5629a, /// 0x2c4
			 0xc2a1a4d8, /// 0x2c8
			 0xdbe561cd, /// 0x2cc
			 0x3b1e9f9d, /// 0x2d0
			 0xae1dff7b, /// 0x2d4
			 0x689b3371, /// 0x2d8
			 0x3c7fd071, /// 0x2dc
			 0xfff7a7f6, /// 0x2e0
			 0xbe244f05, /// 0x2e4
			 0xf1fe049e, /// 0x2e8
			 0xc7e82992, /// 0x2ec
			 0xe920782e, /// 0x2f0
			 0x61d3581d, /// 0x2f4
			 0xb238c89a, /// 0x2f8
			 0xb37f30fa, /// 0x2fc
			 0x33cc9727, /// 0x300
			 0xbdac4131, /// 0x304
			 0x8898bb4b, /// 0x308
			 0xe8a0ab6e, /// 0x30c
			 0xcd10afc7, /// 0x310
			 0x0e729b6a, /// 0x314
			 0xb1e89942, /// 0x318
			 0xd88205b0, /// 0x31c
			 0x1aa112d2, /// 0x320
			 0xa6428875, /// 0x324
			 0xf1c27499, /// 0x328
			 0xe84a0691, /// 0x32c
			 0x09cf48a7, /// 0x330
			 0x22956217, /// 0x334
			 0xc5679373, /// 0x338
			 0x14cebc41, /// 0x33c
			 0x7e540939, /// 0x340
			 0x1456ac5b, /// 0x344
			 0x5a00ecd7, /// 0x348
			 0xd215c627, /// 0x34c
			 0x4f1758a1, /// 0x350
			 0xad85e6fd, /// 0x354
			 0x25efdb8e, /// 0x358
			 0x65e8bda9, /// 0x35c
			 0x1ab2d176, /// 0x360
			 0xfc0659d5, /// 0x364
			 0xf44beb84, /// 0x368
			 0x2202c7e7, /// 0x36c
			 0x60d83dfa, /// 0x370
			 0x51a648ec, /// 0x374
			 0xa74396e0, /// 0x378
			 0x25d70b55, /// 0x37c
			 0x8f0867eb, /// 0x380
			 0x818fd0be, /// 0x384
			 0xd8342ac0, /// 0x388
			 0xb5d1238e, /// 0x38c
			 0x877a2f74, /// 0x390
			 0x5a21082a, /// 0x394
			 0x25a7fa1b, /// 0x398
			 0x02a6ff07, /// 0x39c
			 0xd5ac05d6, /// 0x3a0
			 0x57d259cf, /// 0x3a4
			 0x9c9fc01e, /// 0x3a8
			 0xaa4a7e69, /// 0x3ac
			 0x6c2e668b, /// 0x3b0
			 0x427a827d, /// 0x3b4
			 0x90397f36, /// 0x3b8
			 0x8cf7f73d, /// 0x3bc
			 0x272e1296, /// 0x3c0
			 0x1110222d, /// 0x3c4
			 0x3978d59d, /// 0x3c8
			 0xe6613850, /// 0x3cc
			 0x9a66bc78, /// 0x3d0
			 0x43618ac9, /// 0x3d4
			 0x5f864209, /// 0x3d8
			 0x7b06c296, /// 0x3dc
			 0x541c3d5b, /// 0x3e0
			 0x84116a9c, /// 0x3e4
			 0xbcbdd46e, /// 0x3e8
			 0xc18fa2c7, /// 0x3ec
			 0x9ab6b109, /// 0x3f0
			 0xff1a8bcd, /// 0x3f4
			 0x80d19dd1, /// 0x3f8
			 0x804731bb, /// 0x3fc
			 0xcf813811, /// 0x400
			 0x3b03435f, /// 0x404
			 0x6a4ea982, /// 0x408
			 0x08004942, /// 0x40c
			 0x8302aa2d, /// 0x410
			 0x0e0f9a24, /// 0x414
			 0xa61287f3, /// 0x418
			 0x85db0cda, /// 0x41c
			 0x9cb180f0, /// 0x420
			 0x9c730996, /// 0x424
			 0x691cf60c, /// 0x428
			 0x621758b5, /// 0x42c
			 0xc7044450, /// 0x430
			 0x1966e57b, /// 0x434
			 0xa043ead1, /// 0x438
			 0xddaa14b4, /// 0x43c
			 0x7f6e670a, /// 0x440
			 0x1667af01, /// 0x444
			 0x3e05d2fb, /// 0x448
			 0x6a469193, /// 0x44c
			 0x3d647bbe, /// 0x450
			 0xb2114600, /// 0x454
			 0x3a2bd779, /// 0x458
			 0x415b08b5, /// 0x45c
			 0x114014ea, /// 0x460
			 0xe842c8b4, /// 0x464
			 0x5eb45f61, /// 0x468
			 0xeec72fab, /// 0x46c
			 0x32ceb03b, /// 0x470
			 0xb0269e5b, /// 0x474
			 0x9c4e851b, /// 0x478
			 0x522cb769, /// 0x47c
			 0x7a17d3fa, /// 0x480
			 0xf72f6789, /// 0x484
			 0x71896be7, /// 0x488
			 0x26a92c04, /// 0x48c
			 0x76954e61, /// 0x490
			 0xedacfd92, /// 0x494
			 0xd1944880, /// 0x498
			 0x56aedf0e, /// 0x49c
			 0x957cd46d, /// 0x4a0
			 0xae3c0d3d, /// 0x4a4
			 0x20a50150, /// 0x4a8
			 0x6a83af70, /// 0x4ac
			 0xf8252156, /// 0x4b0
			 0x0d551c5a, /// 0x4b4
			 0xecb2bf67, /// 0x4b8
			 0x3ea3c6d6, /// 0x4bc
			 0x9ae7c335, /// 0x4c0
			 0x439076b4, /// 0x4c4
			 0xd1f74bf6, /// 0x4c8
			 0x109ea798, /// 0x4cc
			 0x5e48c850, /// 0x4d0
			 0x36da31e4, /// 0x4d4
			 0x99bb46bd, /// 0x4d8
			 0x3bad248e, /// 0x4dc
			 0x3e35b46f, /// 0x4e0
			 0x8136efa6, /// 0x4e4
			 0xb55e4d00, /// 0x4e8
			 0x8532703d, /// 0x4ec
			 0x06f4e400, /// 0x4f0
			 0x56cc9dd7, /// 0x4f4
			 0x7d687363, /// 0x4f8
			 0xbfe86529, /// 0x4fc
			 0xd706b5c2, /// 0x500
			 0x9998e6a7, /// 0x504
			 0xec134da4, /// 0x508
			 0xf68db989, /// 0x50c
			 0xaae73743, /// 0x510
			 0x01dfc865, /// 0x514
			 0xa9749527, /// 0x518
			 0xcf7404f2, /// 0x51c
			 0x559b3af7, /// 0x520
			 0x4d4fd3de, /// 0x524
			 0x5f3cc47b, /// 0x528
			 0x25a1ec24, /// 0x52c
			 0x3de01d37, /// 0x530
			 0x5674aa57, /// 0x534
			 0xd3dd8560, /// 0x538
			 0x1f5262ca, /// 0x53c
			 0x5b2832be, /// 0x540
			 0xb611ee90, /// 0x544
			 0xd248d898, /// 0x548
			 0x2a8597b4, /// 0x54c
			 0xb254f93c, /// 0x550
			 0xa9ac376f, /// 0x554
			 0x5b00ccea, /// 0x558
			 0x8bce101d, /// 0x55c
			 0xb1c0f2e3, /// 0x560
			 0xedb916b7, /// 0x564
			 0x0c1a6e7e, /// 0x568
			 0x3d290723, /// 0x56c
			 0x2c3997f6, /// 0x570
			 0x125a0ffb, /// 0x574
			 0xc38d52b0, /// 0x578
			 0x33b85b7b, /// 0x57c
			 0x4ceb0b52, /// 0x580
			 0x3640f16c, /// 0x584
			 0x105291d5, /// 0x588
			 0x6017aaa5, /// 0x58c
			 0x8082f09f, /// 0x590
			 0xdbff3a55, /// 0x594
			 0x88083808, /// 0x598
			 0x5fad493f, /// 0x59c
			 0x627c4805, /// 0x5a0
			 0xc14e9870, /// 0x5a4
			 0x8daa82a5, /// 0x5a8
			 0xcab3976a, /// 0x5ac
			 0xa14a0246, /// 0x5b0
			 0x86bf35b3, /// 0x5b4
			 0xc620dab5, /// 0x5b8
			 0x0c829a4f, /// 0x5bc
			 0xfc9b52d5, /// 0x5c0
			 0xec532bff, /// 0x5c4
			 0xcb22ce9e, /// 0x5c8
			 0xdfcf311b, /// 0x5cc
			 0xf40c52f4, /// 0x5d0
			 0x896cee7f, /// 0x5d4
			 0xb20a8143, /// 0x5d8
			 0xf82c908d, /// 0x5dc
			 0x7c0bee83, /// 0x5e0
			 0x9ded879c, /// 0x5e4
			 0x464dfbb1, /// 0x5e8
			 0xde533edb, /// 0x5ec
			 0x0aecac41, /// 0x5f0
			 0x059dec6d, /// 0x5f4
			 0xeb178d43, /// 0x5f8
			 0x17220fd1, /// 0x5fc
			 0x274e052b, /// 0x600
			 0x0e5f396b, /// 0x604
			 0x1e20ffab, /// 0x608
			 0x488b1207, /// 0x60c
			 0xa7f3972b, /// 0x610
			 0xea4deca3, /// 0x614
			 0x36cce691, /// 0x618
			 0xb8b87b8e, /// 0x61c
			 0x9f676c09, /// 0x620
			 0x1caa2ab3, /// 0x624
			 0xac981fe7, /// 0x628
			 0xf009fe78, /// 0x62c
			 0x4688e862, /// 0x630
			 0x7aa4ba29, /// 0x634
			 0x3b9925e9, /// 0x638
			 0x13f6d9eb, /// 0x63c
			 0x6d0c14b2, /// 0x640
			 0x76c7a4d9, /// 0x644
			 0x278a403b, /// 0x648
			 0x20a0ef88, /// 0x64c
			 0x0cb0d11b, /// 0x650
			 0x2ef29b9f, /// 0x654
			 0xb6e77339, /// 0x658
			 0xd2ec7fa7, /// 0x65c
			 0xf42cd712, /// 0x660
			 0xe4256561, /// 0x664
			 0xea49285e, /// 0x668
			 0xe7ede496, /// 0x66c
			 0x31c1fac7, /// 0x670
			 0x8262b567, /// 0x674
			 0x79941122, /// 0x678
			 0x468466e4, /// 0x67c
			 0xc9a996e6, /// 0x680
			 0xc5c973d4, /// 0x684
			 0x48735c7c, /// 0x688
			 0x52f2fa37, /// 0x68c
			 0x45534b92, /// 0x690
			 0x4cc40e12, /// 0x694
			 0x8829514a, /// 0x698
			 0x56e41939, /// 0x69c
			 0xf249507f, /// 0x6a0
			 0x41438708, /// 0x6a4
			 0x30c180d7, /// 0x6a8
			 0x39326be6, /// 0x6ac
			 0x2c577344, /// 0x6b0
			 0x10da9c23, /// 0x6b4
			 0x02fff3e1, /// 0x6b8
			 0x6ac647a9, /// 0x6bc
			 0x5294a890, /// 0x6c0
			 0xe983187a, /// 0x6c4
			 0x473067f2, /// 0x6c8
			 0xb84fc6a2, /// 0x6cc
			 0xb31123c9, /// 0x6d0
			 0x6e873fa9, /// 0x6d4
			 0xf4ca6471, /// 0x6d8
			 0x62ca59d3, /// 0x6dc
			 0xae6d767d, /// 0x6e0
			 0xff487edb, /// 0x6e4
			 0xf5c9779b, /// 0x6e8
			 0xce62262f, /// 0x6ec
			 0xa906f5a1, /// 0x6f0
			 0xeffa4eef, /// 0x6f4
			 0xe8347b09, /// 0x6f8
			 0xfbaf9aa5, /// 0x6fc
			 0x23775614, /// 0x700
			 0xf33ba75f, /// 0x704
			 0xce6997a8, /// 0x708
			 0x6298e849, /// 0x70c
			 0xed72fdf9, /// 0x710
			 0x6127ab98, /// 0x714
			 0x578dfc98, /// 0x718
			 0x25e7acc8, /// 0x71c
			 0x39f9b6fb, /// 0x720
			 0xacb4a0ca, /// 0x724
			 0x0265f053, /// 0x728
			 0xf5ea649a, /// 0x72c
			 0x1d6440bd, /// 0x730
			 0x6a2f5c1b, /// 0x734
			 0xce0ff368, /// 0x738
			 0x2dda7af9, /// 0x73c
			 0x6665f5b9, /// 0x740
			 0x4bfa9ee8, /// 0x744
			 0x071dd691, /// 0x748
			 0x6c28b981, /// 0x74c
			 0xcc5b94b9, /// 0x750
			 0x66b818cd, /// 0x754
			 0x0722cbbe, /// 0x758
			 0xd5f68db7, /// 0x75c
			 0xe73e63d4, /// 0x760
			 0x3c49acf1, /// 0x764
			 0x258dbe26, /// 0x768
			 0x785d6175, /// 0x76c
			 0x048d7fae, /// 0x770
			 0xac5d289d, /// 0x774
			 0x5e2a47dd, /// 0x778
			 0x35119369, /// 0x77c
			 0xfcc017a5, /// 0x780
			 0x775d3f79, /// 0x784
			 0xa4d1cb25, /// 0x788
			 0x70f66864, /// 0x78c
			 0xfa001bc3, /// 0x790
			 0x701a35fa, /// 0x794
			 0x6c4769e9, /// 0x798
			 0x9350eca1, /// 0x79c
			 0x1a079c7c, /// 0x7a0
			 0xc142a35d, /// 0x7a4
			 0xb78cd25c, /// 0x7a8
			 0x6d37d256, /// 0x7ac
			 0x47810233, /// 0x7b0
			 0x8f9a11fc, /// 0x7b4
			 0xc1f8e4df, /// 0x7b8
			 0x5ea90ea7, /// 0x7bc
			 0xd2261fc4, /// 0x7c0
			 0x3b8f98fd, /// 0x7c4
			 0x98ecb157, /// 0x7c8
			 0x296d1839, /// 0x7cc
			 0x0fb63709, /// 0x7d0
			 0x26166ed8, /// 0x7d4
			 0xac41a794, /// 0x7d8
			 0xf9ea36f3, /// 0x7dc
			 0x2f0e779d, /// 0x7e0
			 0x5a3a7237, /// 0x7e4
			 0x08a683b3, /// 0x7e8
			 0xfacd0de6, /// 0x7ec
			 0x280dff43, /// 0x7f0
			 0x84aa85a5, /// 0x7f4
			 0xbacff74e, /// 0x7f8
			 0x78406e5d, /// 0x7fc
			 0x6b9b3912, /// 0x800
			 0x66ee79bc, /// 0x804
			 0xf6c65694, /// 0x808
			 0xc6fffd6a, /// 0x80c
			 0x6b4c391b, /// 0x810
			 0x8b03aa42, /// 0x814
			 0x72aa5ad2, /// 0x818
			 0x3595f624, /// 0x81c
			 0x15bfa717, /// 0x820
			 0xb643bbea, /// 0x824
			 0x2b42b0c7, /// 0x828
			 0x83e5c492, /// 0x82c
			 0x54f00a5e, /// 0x830
			 0xe6041338, /// 0x834
			 0x4c9ab7ba, /// 0x838
			 0xa23ffd08, /// 0x83c
			 0xa4eb5526, /// 0x840
			 0xcf0e1da9, /// 0x844
			 0xa692c7cd, /// 0x848
			 0x3d22549a, /// 0x84c
			 0x8f29b371, /// 0x850
			 0x549afc8f, /// 0x854
			 0x48c0aa1f, /// 0x858
			 0x2f601c93, /// 0x85c
			 0xadde6209, /// 0x860
			 0x9ff87792, /// 0x864
			 0x228369a5, /// 0x868
			 0x316af636, /// 0x86c
			 0xe63829dd, /// 0x870
			 0xfd5c2b54, /// 0x874
			 0x2da2be88, /// 0x878
			 0x722c0c9f, /// 0x87c
			 0xfdb29b82, /// 0x880
			 0x4cd530d0, /// 0x884
			 0x49a43b6b, /// 0x888
			 0xc03ad827, /// 0x88c
			 0x0322026a, /// 0x890
			 0xdae9552f, /// 0x894
			 0xf88c0419, /// 0x898
			 0xdac410c8, /// 0x89c
			 0xb9533e05, /// 0x8a0
			 0xd2c7c3b6, /// 0x8a4
			 0x0fe6c9e0, /// 0x8a8
			 0x6f419361, /// 0x8ac
			 0x30b54c49, /// 0x8b0
			 0xd4fb88ea, /// 0x8b4
			 0x4c3ec4f8, /// 0x8b8
			 0x66b50ce5, /// 0x8bc
			 0x4d0b7fb8, /// 0x8c0
			 0xdd20788f, /// 0x8c4
			 0xe69df7d9, /// 0x8c8
			 0xcba1fd9f, /// 0x8cc
			 0x15807932, /// 0x8d0
			 0xbebdadb7, /// 0x8d4
			 0xf24480f9, /// 0x8d8
			 0xefa0da9e, /// 0x8dc
			 0x7201969a, /// 0x8e0
			 0x917fb008, /// 0x8e4
			 0xb0aa8a09, /// 0x8e8
			 0xf87ff037, /// 0x8ec
			 0x74a601f7, /// 0x8f0
			 0x8c2c87c5, /// 0x8f4
			 0xbbe794c4, /// 0x8f8
			 0x360bca2b, /// 0x8fc
			 0x925530c9, /// 0x900
			 0xe8148af5, /// 0x904
			 0xc87e7ade, /// 0x908
			 0x946d00f3, /// 0x90c
			 0x5d24f68e, /// 0x910
			 0x95066810, /// 0x914
			 0x3543ff0c, /// 0x918
			 0xb4619398, /// 0x91c
			 0x13a51652, /// 0x920
			 0x251bd740, /// 0x924
			 0xf441346e, /// 0x928
			 0x93edd992, /// 0x92c
			 0xad6f3343, /// 0x930
			 0x6b6b56cb, /// 0x934
			 0xbdb03c6b, /// 0x938
			 0xf8a679c5, /// 0x93c
			 0x78140f63, /// 0x940
			 0x9b155755, /// 0x944
			 0x9f6d043f, /// 0x948
			 0x6bc48e40, /// 0x94c
			 0xdff51dad, /// 0x950
			 0x4f65eb18, /// 0x954
			 0x01564ffa, /// 0x958
			 0x24b89f1f, /// 0x95c
			 0x742fdeb6, /// 0x960
			 0xd96ea3e4, /// 0x964
			 0x3c5e7f53, /// 0x968
			 0xd3cae9a4, /// 0x96c
			 0x6e0dbbee, /// 0x970
			 0x72b4f708, /// 0x974
			 0x485c1a52, /// 0x978
			 0xb01b2673, /// 0x97c
			 0xf31137bd, /// 0x980
			 0xc722c0aa, /// 0x984
			 0x0628e888, /// 0x988
			 0xce6a387c, /// 0x98c
			 0xbac4e79c, /// 0x990
			 0xa75894f2, /// 0x994
			 0xb7855fbb, /// 0x998
			 0x5285537c, /// 0x99c
			 0x971ea523, /// 0x9a0
			 0x1f9e25ad, /// 0x9a4
			 0x3bfde7ce, /// 0x9a8
			 0xf53202bd, /// 0x9ac
			 0xe2527688, /// 0x9b0
			 0xaeeb8713, /// 0x9b4
			 0x28e8d861, /// 0x9b8
			 0x04b23a02, /// 0x9bc
			 0x19efcf6b, /// 0x9c0
			 0x38ab9b73, /// 0x9c4
			 0x90bb8c1a, /// 0x9c8
			 0xf617f5c5, /// 0x9cc
			 0x75f83385, /// 0x9d0
			 0x4319cc12, /// 0x9d4
			 0x911b31c3, /// 0x9d8
			 0xb4cbf03e, /// 0x9dc
			 0x0e6e3749, /// 0x9e0
			 0xfc666588, /// 0x9e4
			 0x40e606ff, /// 0x9e8
			 0xe25ee943, /// 0x9ec
			 0xdc2b5a59, /// 0x9f0
			 0x6b3c6782, /// 0x9f4
			 0x7e3deb41, /// 0x9f8
			 0x2c85ea04, /// 0x9fc
			 0x5a0c13b3, /// 0xa00
			 0xd0f74772, /// 0xa04
			 0x805d6688, /// 0xa08
			 0x17dded44, /// 0xa0c
			 0x5e0ac66b, /// 0xa10
			 0x216735cf, /// 0xa14
			 0xba3909e5, /// 0xa18
			 0x65da5b6e, /// 0xa1c
			 0xaa83a074, /// 0xa20
			 0x7b04d6ed, /// 0xa24
			 0xef3fab98, /// 0xa28
			 0x2f3dfd55, /// 0xa2c
			 0x21b53f93, /// 0xa30
			 0x12e598fc, /// 0xa34
			 0x4f37268e, /// 0xa38
			 0x4ced7dcc, /// 0xa3c
			 0x0441684b, /// 0xa40
			 0xdc6f5e45, /// 0xa44
			 0x0a232f30, /// 0xa48
			 0x46f86fc4, /// 0xa4c
			 0xeee2adca, /// 0xa50
			 0x2da8f1b5, /// 0xa54
			 0x1af125c9, /// 0xa58
			 0x3f093c1c, /// 0xa5c
			 0x59e9cb55, /// 0xa60
			 0x84866e15, /// 0xa64
			 0xf12267a4, /// 0xa68
			 0xf6971c46, /// 0xa6c
			 0xcb9b7ad5, /// 0xa70
			 0xde35f9f9, /// 0xa74
			 0x7372ce97, /// 0xa78
			 0xda26b41b, /// 0xa7c
			 0x5c601469, /// 0xa80
			 0x8fef909d, /// 0xa84
			 0x727a5872, /// 0xa88
			 0x46f70240, /// 0xa8c
			 0x4fc074cc, /// 0xa90
			 0x12c8971a, /// 0xa94
			 0xa5e33206, /// 0xa98
			 0x3e2f2fd7, /// 0xa9c
			 0x2b01c2ff, /// 0xaa0
			 0x2e1676e7, /// 0xaa4
			 0x56ca23b5, /// 0xaa8
			 0x29e2b7a9, /// 0xaac
			 0x2dbb2b77, /// 0xab0
			 0x5e80db86, /// 0xab4
			 0x877718bb, /// 0xab8
			 0x1d0b89d2, /// 0xabc
			 0x880655cd, /// 0xac0
			 0x148f0174, /// 0xac4
			 0x047fda4b, /// 0xac8
			 0x4c4e3e07, /// 0xacc
			 0x264ae365, /// 0xad0
			 0x683362d0, /// 0xad4
			 0x5486c32c, /// 0xad8
			 0x6e8c5877, /// 0xadc
			 0x40e666f5, /// 0xae0
			 0x5e5e0ebd, /// 0xae4
			 0xab901fdd, /// 0xae8
			 0x93d6b21f, /// 0xaec
			 0xcd917f79, /// 0xaf0
			 0xf5ca058a, /// 0xaf4
			 0xb565bc08, /// 0xaf8
			 0xd8647380, /// 0xafc
			 0x90593b23, /// 0xb00
			 0x5e22855c, /// 0xb04
			 0x2fece246, /// 0xb08
			 0x6ae04cd7, /// 0xb0c
			 0x1a1989fc, /// 0xb10
			 0x25f015b1, /// 0xb14
			 0x433332bb, /// 0xb18
			 0x3336d3f5, /// 0xb1c
			 0x74fde467, /// 0xb20
			 0x658d4905, /// 0xb24
			 0xaa429931, /// 0xb28
			 0x4f5d0d0b, /// 0xb2c
			 0xd1f3ced6, /// 0xb30
			 0xde30b2b0, /// 0xb34
			 0x4bc90aee, /// 0xb38
			 0xbbe59823, /// 0xb3c
			 0x9327edf9, /// 0xb40
			 0x18195580, /// 0xb44
			 0x5f4a6d47, /// 0xb48
			 0xd58835a4, /// 0xb4c
			 0xb8261caa, /// 0xb50
			 0xef21d58f, /// 0xb54
			 0xb3fac830, /// 0xb58
			 0xf22a497e, /// 0xb5c
			 0x0a42d0d4, /// 0xb60
			 0x06fd00fd, /// 0xb64
			 0x00a63f37, /// 0xb68
			 0xeffd3d36, /// 0xb6c
			 0xde56942e, /// 0xb70
			 0xb082ecba, /// 0xb74
			 0x7feaedfc, /// 0xb78
			 0xad041ed4, /// 0xb7c
			 0xacb3e5c7, /// 0xb80
			 0x8dd77d8d, /// 0xb84
			 0x98db3f51, /// 0xb88
			 0x2759bc68, /// 0xb8c
			 0x0fc5b89e, /// 0xb90
			 0xed2f968b, /// 0xb94
			 0x45bcdaeb, /// 0xb98
			 0x7fe51835, /// 0xb9c
			 0x20336b47, /// 0xba0
			 0x00fff32f, /// 0xba4
			 0xc1b1522a, /// 0xba8
			 0xd94e6731, /// 0xbac
			 0x5f7f0e89, /// 0xbb0
			 0x9503e05f, /// 0xbb4
			 0x9ae7c2c0, /// 0xbb8
			 0xb907a5f6, /// 0xbbc
			 0x092442c2, /// 0xbc0
			 0x4a6e04cc, /// 0xbc4
			 0xdafe889d, /// 0xbc8
			 0xd741176a, /// 0xbcc
			 0xa23a0e22, /// 0xbd0
			 0x1d580215, /// 0xbd4
			 0x69d2edc4, /// 0xbd8
			 0xc262a7a4, /// 0xbdc
			 0x7c7ec48d, /// 0xbe0
			 0x20ba0aa7, /// 0xbe4
			 0xfdfc6007, /// 0xbe8
			 0xd6b76a47, /// 0xbec
			 0x8f40f4b3, /// 0xbf0
			 0x274475dc, /// 0xbf4
			 0x0c22f941, /// 0xbf8
			 0x360c03a8, /// 0xbfc
			 0xa229af76, /// 0xc00
			 0x54c58aea, /// 0xc04
			 0x846f38ea, /// 0xc08
			 0xd66e703a, /// 0xc0c
			 0x44eb8703, /// 0xc10
			 0xb5fdcd6d, /// 0xc14
			 0xb6bfe332, /// 0xc18
			 0x215b4aed, /// 0xc1c
			 0x5b8f3c7c, /// 0xc20
			 0x83e46576, /// 0xc24
			 0xa4cdb4bc, /// 0xc28
			 0xab64e0fe, /// 0xc2c
			 0x6231be00, /// 0xc30
			 0xd7d52a2f, /// 0xc34
			 0x5ab095aa, /// 0xc38
			 0x2872f050, /// 0xc3c
			 0x03d34af1, /// 0xc40
			 0x822d06c8, /// 0xc44
			 0x48ccc6f6, /// 0xc48
			 0x8dcf169a, /// 0xc4c
			 0x3a428406, /// 0xc50
			 0xbae840a3, /// 0xc54
			 0x5b38f8e3, /// 0xc58
			 0xdca310b3, /// 0xc5c
			 0xbc8cb979, /// 0xc60
			 0x2bd6480b, /// 0xc64
			 0x972a3d7f, /// 0xc68
			 0xc3112e10, /// 0xc6c
			 0x907c86a8, /// 0xc70
			 0x9e65d739, /// 0xc74
			 0x52656a40, /// 0xc78
			 0x8e7bfb51, /// 0xc7c
			 0xfc5e64e7, /// 0xc80
			 0x2bc96ec5, /// 0xc84
			 0xbc9288ed, /// 0xc88
			 0x4434aa4d, /// 0xc8c
			 0x19403c6a, /// 0xc90
			 0xc31ab373, /// 0xc94
			 0xc63ec1dc, /// 0xc98
			 0xf933c61c, /// 0xc9c
			 0x34ca687b, /// 0xca0
			 0xd77039d1, /// 0xca4
			 0x07f71dae, /// 0xca8
			 0x1e87e629, /// 0xcac
			 0xa02addb5, /// 0xcb0
			 0x8758b212, /// 0xcb4
			 0xe917b21b, /// 0xcb8
			 0xe97b8eec, /// 0xcbc
			 0x812d6013, /// 0xcc0
			 0x8239714d, /// 0xcc4
			 0x1f888052, /// 0xcc8
			 0x3aeda656, /// 0xccc
			 0x1d0342b3, /// 0xcd0
			 0x6baf7062, /// 0xcd4
			 0x0b1d9260, /// 0xcd8
			 0x5bee28a9, /// 0xcdc
			 0x446d33ad, /// 0xce0
			 0x1312c4a6, /// 0xce4
			 0x2a2d0441, /// 0xce8
			 0x05a580d4, /// 0xcec
			 0xc4a2cfe0, /// 0xcf0
			 0x97f989af, /// 0xcf4
			 0x050c9e30, /// 0xcf8
			 0x5492a7b5, /// 0xcfc
			 0x97a911bf, /// 0xd00
			 0xd39d7367, /// 0xd04
			 0xd92941da, /// 0xd08
			 0x2bad8033, /// 0xd0c
			 0x02168ed0, /// 0xd10
			 0x64f0c7ca, /// 0xd14
			 0xee76b31c, /// 0xd18
			 0x067d30c1, /// 0xd1c
			 0x737ba3fb, /// 0xd20
			 0x517ac9ac, /// 0xd24
			 0xab8eb368, /// 0xd28
			 0xc27efa6c, /// 0xd2c
			 0x1bc8f5c0, /// 0xd30
			 0x9dae00af, /// 0xd34
			 0x01342561, /// 0xd38
			 0x2038492b, /// 0xd3c
			 0x55e17579, /// 0xd40
			 0x1a329ace, /// 0xd44
			 0x78d91952, /// 0xd48
			 0x0cb04dcf, /// 0xd4c
			 0x70b93b81, /// 0xd50
			 0x05aa438b, /// 0xd54
			 0xc6ca74f7, /// 0xd58
			 0x7553bbf3, /// 0xd5c
			 0x517ffbfd, /// 0xd60
			 0xe3d30738, /// 0xd64
			 0x62e7e0d7, /// 0xd68
			 0x0de89058, /// 0xd6c
			 0x43aa79b5, /// 0xd70
			 0x5bab0fe0, /// 0xd74
			 0xc48c2609, /// 0xd78
			 0x3f049a8c, /// 0xd7c
			 0x34b8bbd3, /// 0xd80
			 0x6bbda63f, /// 0xd84
			 0x0697a5f2, /// 0xd88
			 0x981ce303, /// 0xd8c
			 0x5e52c591, /// 0xd90
			 0x75e24144, /// 0xd94
			 0x74a69175, /// 0xd98
			 0x2ffc092f, /// 0xd9c
			 0xe29d569f, /// 0xda0
			 0x4f345501, /// 0xda4
			 0xbe1a0983, /// 0xda8
			 0xa2758020, /// 0xdac
			 0x684f8c6b, /// 0xdb0
			 0x578a3447, /// 0xdb4
			 0x110ed5ec, /// 0xdb8
			 0xeaaea7d4, /// 0xdbc
			 0xc4d3fac9, /// 0xdc0
			 0x85ad62d7, /// 0xdc4
			 0x932538b8, /// 0xdc8
			 0x9d186f4f, /// 0xdcc
			 0xdfad1a83, /// 0xdd0
			 0x7f8913f3, /// 0xdd4
			 0x16a56f26, /// 0xdd8
			 0x012bfc10, /// 0xddc
			 0x537456dd, /// 0xde0
			 0x76ccda3b, /// 0xde4
			 0xe3636bca, /// 0xde8
			 0xb177f2a4, /// 0xdec
			 0x37d12a2d, /// 0xdf0
			 0x7da23f35, /// 0xdf4
			 0xa7081791, /// 0xdf8
			 0xc680ef3b, /// 0xdfc
			 0xece4c624, /// 0xe00
			 0x0d74db59, /// 0xe04
			 0x3491d40d, /// 0xe08
			 0x25d0881a, /// 0xe0c
			 0x29ae7f50, /// 0xe10
			 0x73e2bf96, /// 0xe14
			 0x7f9d0126, /// 0xe18
			 0x741b152d, /// 0xe1c
			 0xf79e0af4, /// 0xe20
			 0x4708de11, /// 0xe24
			 0xb4c03a70, /// 0xe28
			 0x21287d9f, /// 0xe2c
			 0x351f6938, /// 0xe30
			 0xf2f4acc0, /// 0xe34
			 0x6d2443c0, /// 0xe38
			 0xc835a9dd, /// 0xe3c
			 0xf4f6d046, /// 0xe40
			 0xdcf9b78e, /// 0xe44
			 0x5b081479, /// 0xe48
			 0x883b3e10, /// 0xe4c
			 0x79f2c6c7, /// 0xe50
			 0xf6a9eff7, /// 0xe54
			 0x42673306, /// 0xe58
			 0xe7752585, /// 0xe5c
			 0xc6b6562d, /// 0xe60
			 0xf8d9d18e, /// 0xe64
			 0xb24b08b0, /// 0xe68
			 0x1abc98e0, /// 0xe6c
			 0x0bc07561, /// 0xe70
			 0xd76aa2f6, /// 0xe74
			 0xf9b9679e, /// 0xe78
			 0x86d04cdb, /// 0xe7c
			 0xe3a7f559, /// 0xe80
			 0xd29a7971, /// 0xe84
			 0xccad25c6, /// 0xe88
			 0x596cb2b8, /// 0xe8c
			 0x2605ce7c, /// 0xe90
			 0xdc3941a3, /// 0xe94
			 0xac2ef341, /// 0xe98
			 0x060a94cb, /// 0xe9c
			 0x8c7c2a74, /// 0xea0
			 0xfa83d530, /// 0xea4
			 0x9603fea2, /// 0xea8
			 0x0875f1f2, /// 0xeac
			 0x05201484, /// 0xeb0
			 0x38582b44, /// 0xeb4
			 0xeb12fa29, /// 0xeb8
			 0x68d2e3e7, /// 0xebc
			 0xff0b4207, /// 0xec0
			 0x163219bb, /// 0xec4
			 0x9a0ab5b6, /// 0xec8
			 0xc493b874, /// 0xecc
			 0x222c2fd3, /// 0xed0
			 0xc781ecf5, /// 0xed4
			 0x202e5307, /// 0xed8
			 0xec931f65, /// 0xedc
			 0xb1273c83, /// 0xee0
			 0xda3f1cd9, /// 0xee4
			 0xd413a9fd, /// 0xee8
			 0x1204655c, /// 0xeec
			 0x112a8884, /// 0xef0
			 0xcf95c214, /// 0xef4
			 0x3d63c74f, /// 0xef8
			 0xb45165b9, /// 0xefc
			 0x41efbcce, /// 0xf00
			 0x6dae162e, /// 0xf04
			 0x5de0c094, /// 0xf08
			 0x73e37286, /// 0xf0c
			 0x72c5650d, /// 0xf10
			 0xbeaf13ea, /// 0xf14
			 0x48891bd7, /// 0xf18
			 0x39a16536, /// 0xf1c
			 0x53c167fd, /// 0xf20
			 0x052c4c3d, /// 0xf24
			 0xd1354e5c, /// 0xf28
			 0x042bf266, /// 0xf2c
			 0xb80e7ce2, /// 0xf30
			 0xfd589618, /// 0xf34
			 0x56e6b422, /// 0xf38
			 0xb575ef92, /// 0xf3c
			 0x9699b1a7, /// 0xf40
			 0x57c59535, /// 0xf44
			 0xc2b67029, /// 0xf48
			 0x212353dc, /// 0xf4c
			 0x19c489de, /// 0xf50
			 0x1b645319, /// 0xf54
			 0x6b5b5f8d, /// 0xf58
			 0xe4907e92, /// 0xf5c
			 0x707bbfc9, /// 0xf60
			 0xe7da8ed4, /// 0xf64
			 0x1926804a, /// 0xf68
			 0x29a48286, /// 0xf6c
			 0x2ce0cce4, /// 0xf70
			 0x6a72ceb2, /// 0xf74
			 0x3d0eabbe, /// 0xf78
			 0xb50dbd82, /// 0xf7c
			 0xe4dbf801, /// 0xf80
			 0x9dc61e3f, /// 0xf84
			 0x13ee822c, /// 0xf88
			 0x334a7836, /// 0xf8c
			 0x98d8387f, /// 0xf90
			 0xc1128448, /// 0xf94
			 0xb532b0e0, /// 0xf98
			 0x2d03c5d7, /// 0xf9c
			 0xf06334c4, /// 0xfa0
			 0x7c7d0b80, /// 0xfa4
			 0x025a45fd, /// 0xfa8
			 0x2562f782, /// 0xfac
			 0x59a740b8, /// 0xfb0
			 0x9969da9d, /// 0xfb4
			 0xc0520f2c, /// 0xfb8
			 0xbbfaeab2, /// 0xfbc
			 0x5a023a80, /// 0xfc0
			 0xd0e1a6b7, /// 0xfc4
			 0xbe61833e, /// 0xfc8
			 0xda929fda, /// 0xfcc
			 0x47480353, /// 0xfd0
			 0x0a0ce823, /// 0xfd4
			 0x2c38ff55, /// 0xfd8
			 0x6662e5ff, /// 0xfdc
			 0x785dcafa, /// 0xfe0
			 0xe949c600, /// 0xfe4
			 0x39b317e2, /// 0xfe8
			 0xfaef6536, /// 0xfec
			 0xd45df3aa, /// 0xff0
			 0x3f2b4b33, /// 0xff4
			 0xe157aa76, /// 0xff8
			 0x764e511e /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00000
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00004
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00008
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00010
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00014
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00018
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0001c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00020
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00028
			 0xffc00001,                                                  // -signaling NaN                              /// 0002c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00030
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00034
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00038
			 0x00000000,                                                  // zero                                        /// 0003c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00044
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00050
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00054
			 0x7f800000,                                                  // inf                                         /// 00058
			 0x0c600000,                                                  // Leading 1s:                                 /// 0005c
			 0x3f028f5c,                                                  // 0.51                                        /// 00060
			 0x0c700000,                                                  // Leading 1s:                                 /// 00064
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00068
			 0xcb000000,                                                  // -8388608.0                                  /// 0006c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00070
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00074
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0007c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00088
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0008c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00090
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00094
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0009c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 000b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000e4
			 0x33333333,                                                  // 4 random values                             /// 000e8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 000f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00100
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00104
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0010c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00110
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00114
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00118
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00124
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00128
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00130
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00134
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00138
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0013c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0014c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00154
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00158
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0015c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00164
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00168
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0016c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00174
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00178
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x4b000000,                                                  // 8388608.0                                   /// 00180
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00184
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00188
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0018c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00190
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00194
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00198
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0019c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x80000000,                                                  // -zero                                       /// 001a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 001c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001e0
			 0x80000000,                                                  // -zero                                       /// 001e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001f0
			 0x80000000,                                                  // -zero                                       /// 001f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00200
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00204
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0c700000,                                                  // Leading 1s:                                 /// 0020c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00210
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0xffc00001,                                                  // -signaling NaN                              /// 00218
			 0x7fc00001,                                                  // signaling NaN                               /// 0021c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00224
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00228
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0022c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x7f800000,                                                  // inf                                         /// 00240
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x80000000,                                                  // -zero                                       /// 00254
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00258
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00260
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00264
			 0x0c400000,                                                  // Leading 1s:                                 /// 00268
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0026c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00270
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00274
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00278
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0027c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00280
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0028c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00290
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00294
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00298
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0029c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00300
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00304
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00308
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x80000000,                                                  // -zero                                       /// 0031c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00324
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00328
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0032c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00334
			 0x3f028f5c,                                                  // 0.51                                        /// 00338
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0033c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00344
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00348
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0034c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00350
			 0x0e000007,                                                  // Trailing 1s:                                /// 00354
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0035c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00368
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00370
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00374
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00378
			 0x0c780000,                                                  // Leading 1s:                                 /// 0037c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00380
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00388
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0038c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00390
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00394
			 0xff800000,                                                  // -inf                                        /// 00398
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0039c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 003ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x33333333,                                                  // 4 random values                             /// 003c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00400
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00410
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00414
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00418
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0041c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00420
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00424
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00428
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0042c
			 0x3f028f5c,                                                  // 0.51                                        /// 00430
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00434
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00438
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0043c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00440
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00444
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00448
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00450
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00458
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00460
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00464
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00468
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0046c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00470
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0047c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00480
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00484
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00488
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0048c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00498
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0049c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0x00000000,                                                  // zero                                        /// 004b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004d4
			 0x7f800000,                                                  // inf                                         /// 004d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 004e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 004fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00500
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00508
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x33333333,                                                  // 4 random values                             /// 00510
			 0x0c700000,                                                  // Leading 1s:                                 /// 00514
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x4b000000,                                                  // 8388608.0                                   /// 0051c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00520
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00524
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0052c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00534
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00538
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x7fc00001,                                                  // signaling NaN                               /// 00540
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00544
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00548
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0054c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00554
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00558
			 0x7f800000,                                                  // inf                                         /// 0055c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x33333333,                                                  // 4 random values                             /// 00568
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00574
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00580
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00584
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00588
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0058c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00598
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0059c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005b8
			 0x80000000,                                                  // -zero                                       /// 005bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 005e0
			 0xffc00001,                                                  // -signaling NaN                              /// 005e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 005e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00600
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00604
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00608
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0060c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00610
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00614
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00618
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0061c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00620
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00624
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00628
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0062c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00630
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00634
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00638
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0063c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00640
			 0xffc00001,                                                  // -signaling NaN                              /// 00644
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0064c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00658
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0065c
			 0xcb000000,                                                  // -8388608.0                                  /// 00660
			 0xcb000000,                                                  // -8388608.0                                  /// 00664
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00668
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0066c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00670
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00678
			 0x3f028f5c,                                                  // 0.51                                        /// 0067c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00680
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00684
			 0xffc00001,                                                  // -signaling NaN                              /// 00688
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0068c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0e000001,                                                  // Trailing 1s:                                /// 00694
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x0c700000,                                                  // Leading 1s:                                 /// 0069c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00700
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00704
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0070c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00710
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00714
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00718
			 0x55555555,                                                  // 4 random values                             /// 0071c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00724
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0072c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00734
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0073c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00740
			 0x33333333,                                                  // 4 random values                             /// 00744
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00748
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00750
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00754
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00758
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0075c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00768
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00770
			 0x33333333,                                                  // 4 random values                             /// 00774
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0077c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00784
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00788
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0078c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00794
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0079c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007c0
			 0x33333333,                                                  // 4 random values                             /// 007c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007d8
			 0x33333333,                                                  // 4 random values                             /// 007dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x55555555,                                                  // 4 random values                             /// 007f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00800
			 0x4b000000,                                                  // 8388608.0                                   /// 00804
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00810
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0081c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00820
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00824
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00828
			 0x00011111,                                                  // 9.7958E-41                                  /// 0082c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00830
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00834
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00838
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0083c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00840
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00844
			 0x0c700000,                                                  // Leading 1s:                                 /// 00848
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0084c
			 0x80000000,                                                  // -zero                                       /// 00850
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00858
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0085c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00860
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00868
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0086c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00870
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00878
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0087c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00884
			 0x80000000,                                                  // -zero                                       /// 00888
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0088c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00890
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00894
			 0x0e000001,                                                  // Trailing 1s:                                /// 00898
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0089c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0090c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00910
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00914
			 0x0e000007,                                                  // Trailing 1s:                                /// 00918
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0091c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00920
			 0x0c400000,                                                  // Leading 1s:                                 /// 00924
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0092c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00934
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00938
			 0x7f800000,                                                  // inf                                         /// 0093c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00940
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00944
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00948
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0094c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00950
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00954
			 0x0c780000,                                                  // Leading 1s:                                 /// 00958
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0095c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00968
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0096c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00974
			 0x80011111,                                                  // -9.7958E-41                                 /// 00978
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0097c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x0e000003,                                                  // Trailing 1s:                                /// 00988
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00994
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00998
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0099c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x80000000,                                                  // -zero                                       /// 009b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009d8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009f0
			 0x7f800000,                                                  // inf                                         /// 009f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a04
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a08
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a10
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a14
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a18
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x55555555,                                                  // 4 random values                             /// 00a20
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a2c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a30
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a34
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x7f800000,                                                  // inf                                         /// 00a58
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a60
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a70
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a74
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a7c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a80
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a84
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a90
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a98
			 0x80000000,                                                  // -zero                                       /// 00a9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aa4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00aa8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ab4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ab8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00abc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ac4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00acc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ad0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ad4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ad8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00adc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ae4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ae8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00af0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00af4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00af8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b00
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x7fc00001,                                                  // signaling NaN                               /// 00b08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b28
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b30
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b44
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b50
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00000000,                                                  // zero                                        /// 00b5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b68
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b8c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b98
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ba8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bb0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bcc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bdc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00be0
			 0x80000000,                                                  // -zero                                       /// 00be4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bf0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bf8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bfc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c04
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c10
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c1c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c34
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c44
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c48
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c50
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c58
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c60
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c64
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c70
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c7c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c84
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0xff800000,                                                  // -inf                                        /// 00c94
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c9c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ca8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cac
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cb8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cbc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cc0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cc4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ccc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cd0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cd4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ce4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ce8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cf0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cf8
			 0x3f028f5c,                                                  // 0.51                                        /// 00cfc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d04
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d08
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d0c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d18
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d20
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d38
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d3c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d40
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d50
			 0x3f028f5c,                                                  // 0.51                                        /// 00d54
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d80
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d88
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d98
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00da4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00da8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dac
			 0x7fc00001,                                                  // signaling NaN                               /// 00db0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00db4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dbc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dc0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dcc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dd0
			 0x33333333,                                                  // 4 random values                             /// 00dd4
			 0x80000000,                                                  // -zero                                       /// 00dd8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ddc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0xff800000,                                                  // -inf                                        /// 00de8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00df0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00df4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00df8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e08
			 0xcb000000,                                                  // -8388608.0                                  /// 00e0c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e24
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e30
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e38
			 0x7f800000,                                                  // inf                                         /// 00e3c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e40
			 0x80000000,                                                  // -zero                                       /// 00e44
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e58
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e60
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e6c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e70
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e74
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e78
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e8c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e90
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e98
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ea0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ea4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00eb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ebc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ec0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ec8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ed0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ed4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ed8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ee0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ee8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00eec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ef0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ef4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00efc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f04
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f08
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f0c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f14
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f1c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f20
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f3c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f40
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f48
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f50
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f5c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f7c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f80
			 0x4b000000,                                                  // 8388608.0                                   /// 00f84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f94
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fa4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fa8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fb0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fbc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x33333333,                                                  // 4 random values                             /// 00fc8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fcc
			 0x4b000000,                                                  // 8388608.0                                   /// 00fd0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fd4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fd8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fe4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fe8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ff0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ff4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ff8
			 0x807ffffe // max subnorm - 1ulp                            // SP - ve numbers                             /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00000
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00008
			 0x80011111,                                                  // -9.7958E-41                                 /// 0000c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00014
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00018
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0001c
			 0x7f800000,                                                  // inf                                         /// 00020
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00028
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00030
			 0xff800000,                                                  // -inf                                        /// 00034
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0003c
			 0xffc00001,                                                  // -signaling NaN                              /// 00040
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00044
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00048
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00050
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00054
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00058
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00060
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00064
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00068
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0006c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00070
			 0xbf028f5c,                                                  // -0.51                                       /// 00074
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0007c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00080
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00084
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00088
			 0x00000000,                                                  // zero                                        /// 0008c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00090
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00094
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00098
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0009c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000f0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0010c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00110
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0011c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00124
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00128
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x00000000,                                                  // zero                                        /// 00134
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00138
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00148
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00160
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00164
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00168
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0016c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00170
			 0xff800000,                                                  // -inf                                        /// 00174
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00180
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00184
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00188
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00198
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0019c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x3f028f5c,                                                  // 0.51                                        /// 001b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 001e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001f8
			 0x33333333,                                                  // 4 random values                             /// 001fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00204
			 0x00011111,                                                  // 9.7958E-41                                  /// 00208
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0020c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00210
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00214
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00218
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0021c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00220
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00228
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0022c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00230
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00234
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00240
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0024c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00250
			 0x00000000,                                                  // zero                                        /// 00254
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00258
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0025c
			 0x33333333,                                                  // 4 random values                             /// 00260
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00264
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00268
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0026c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00270
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00274
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00278
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0027c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00280
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00284
			 0x00011111,                                                  // 9.7958E-41                                  /// 00288
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0028c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00290
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00294
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00298
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0029c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002b0
			 0x7f800000,                                                  // inf                                         /// 002b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 002bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0030c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0031c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00320
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00324
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0032c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00330
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00334
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00338
			 0xff800000,                                                  // -inf                                        /// 0033c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00340
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00344
			 0x0c700000,                                                  // Leading 1s:                                 /// 00348
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00350
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0035c
			 0x4b000000,                                                  // 8388608.0                                   /// 00360
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00364
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0036c
			 0xffc00001,                                                  // -signaling NaN                              /// 00370
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00374
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00378
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00380
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00384
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00388
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0038c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00390
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00398
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0039c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003bc
			 0xffc00001,                                                  // -signaling NaN                              /// 003c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003ec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0xbf028f5c,                                                  // -0.51                                       /// 00408
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00414
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00418
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0041c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00420
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00424
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00428
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00434
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00438
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0xffc00001,                                                  // -signaling NaN                              /// 00444
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00458
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0045c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00460
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00464
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0046c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00470
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00474
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00478
			 0xbf028f5c,                                                  // -0.51                                       /// 0047c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00480
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0048c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00490
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00494
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 004c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0xffc00001,                                                  // -signaling NaN                              /// 004cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 004d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00500
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0xbf028f5c,                                                  // -0.51                                       /// 00508
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00510
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00514
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00518
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0051c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00524
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00528
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0052c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0c780000,                                                  // Leading 1s:                                 /// 00538
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0053c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x3f028f5c,                                                  // 0.51                                        /// 00548
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00554
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00558
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0055c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00564
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00568
			 0x0c400000,                                                  // Leading 1s:                                 /// 0056c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00570
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0057c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00580
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00584
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00588
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0058c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00594
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00598
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0059c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005a0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005e0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00600
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00604
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0060c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00618
			 0x0e000003,                                                  // Trailing 1s:                                /// 0061c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00620
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00624
			 0x7fc00001,                                                  // signaling NaN                               /// 00628
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00630
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00634
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00638
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0063c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00650
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00654
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00658
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0065c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x80000000,                                                  // -zero                                       /// 00664
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0066c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00670
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00674
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00678
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0067c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00680
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0c780000,                                                  // Leading 1s:                                 /// 00688
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00690
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00694
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0069c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x00000000,                                                  // zero                                        /// 006a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 006ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006f4
			 0x55555555,                                                  // 4 random values                             /// 006f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00700
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0e000001,                                                  // Trailing 1s:                                /// 00708
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00714
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0xffc00001,                                                  // -signaling NaN                              /// 0071c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00720
			 0x0e000001,                                                  // Trailing 1s:                                /// 00724
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00728
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00730
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0073c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0xbf028f5c,                                                  // -0.51                                       /// 00744
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0074c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00750
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00754
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00758
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0075c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00760
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0x0c600000,                                                  // Leading 1s:                                 /// 00768
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0076c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00770
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00774
			 0x0c700000,                                                  // Leading 1s:                                 /// 00778
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00780
			 0x00000000,                                                  // zero                                        /// 00784
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00788
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0078c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00790
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0e000003,                                                  // Trailing 1s:                                /// 00798
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x00011111,                                                  // 9.7958E-41                                  /// 007a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007b4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007c8
			 0x4b000000,                                                  // 8388608.0                                   /// 007cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 007dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007f0
			 0x55555555,                                                  // 4 random values                             /// 007f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00800
			 0x3f028f5c,                                                  // 0.51                                        /// 00804
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0080c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00814
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00818
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0081c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00824
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00828
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0082c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00834
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0083c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00848
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0084c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00850
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00854
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0085c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00860
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00864
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0086c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00870
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00880
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00884
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00888
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00890
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00898
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0089c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x0e000007,                                                  // Trailing 1s:                                /// 00908
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0090c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0091c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00920
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00928
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0092c
			 0x7fc00001,                                                  // signaling NaN                               /// 00930
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00934
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00938
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0093c
			 0x7fc00001,                                                  // signaling NaN                               /// 00940
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00944
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x55555555,                                                  // 4 random values                             /// 00954
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0095c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0xff800000,                                                  // -inf                                        /// 00964
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00968
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0096c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00970
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00974
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00978
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00980
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00984
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0098c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00990
			 0x80000000,                                                  // -zero                                       /// 00994
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00998
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009cc
			 0xff800000,                                                  // -inf                                        /// 009d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009d4
			 0x00000000,                                                  // zero                                        /// 009d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009f8
			 0x7fc00001,                                                  // signaling NaN                               /// 009fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a00
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a04
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a0c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a18
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a20
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a30
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a44
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a48
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a7c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a94
			 0xff800000,                                                  // -inf                                        /// 00a98
			 0xffc00001,                                                  // -signaling NaN                              /// 00a9c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00aa0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00aa4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00aa8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00abc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ac0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ac4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ac8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00acc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ad4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ad8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00adc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ae0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ae4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ae8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00af8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00afc
			 0x7fc00001,                                                  // signaling NaN                               /// 00b00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b08
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b18
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b24
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b40
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b48
			 0x3f028f5c,                                                  // 0.51                                        /// 00b4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b50
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b5c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b60
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b64
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b74
			 0x4b000000,                                                  // 8388608.0                                   /// 00b78
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b7c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b8c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b9c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ba0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bb4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bb8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bc0
			 0x55555555,                                                  // 4 random values                             /// 00bc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bcc
			 0x55555555,                                                  // 4 random values                             /// 00bd0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bd4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bdc
			 0x7f800000,                                                  // inf                                         /// 00be0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00be4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x4b000000,                                                  // 8388608.0                                   /// 00bf0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bf4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bfc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c0c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c1c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c20
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c24
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c30
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c3c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c40
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c44
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c64
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c68
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c78
			 0x3f028f5c,                                                  // 0.51                                        /// 00c7c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c84
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c94
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ca0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ca4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ca8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cb4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cb8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cbc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cc0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cc4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cc8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ccc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cd0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cd8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ce4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ce8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cf8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d08
			 0x7fc00001,                                                  // signaling NaN                               /// 00d0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d14
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d20
			 0x4b000000,                                                  // 8388608.0                                   /// 00d24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d2c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d30
			 0xbf028f5c,                                                  // -0.51                                       /// 00d34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d50
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d58
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d5c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d64
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d6c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d74
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d78
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d98
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00da4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00da8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00db0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0xcb000000,                                                  // -8388608.0                                  /// 00dbc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dc4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dc8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dcc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dd8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ddc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00de0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00de8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x4b000000,                                                  // 8388608.0                                   /// 00df8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e08
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e18
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e20
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e28
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e2c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e48
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e4c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e58
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e64
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e74
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e78
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e7c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e80
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e88
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e8c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ea8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00eb0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00eb4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00eb8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ebc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ec0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ec4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ec8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ecc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ed8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00edc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ee0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ee4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ee8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00eec
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ef0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ef4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f20
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f28
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f2c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f38
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f3c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f44
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f4c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f58
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f5c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f64
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f6c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f70
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f74
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f7c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f88
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f90
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f98
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f9c
			 0x55555555,                                                  // 4 random values                             /// 00fa0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fa4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fa8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb0
			 0x55555555,                                                  // 4 random values                             /// 00fb4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fbc
			 0xffc00001,                                                  // -signaling NaN                              /// 00fc0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fc8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fd0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fd4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fdc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fe0
			 0xff800000,                                                  // -inf                                        /// 00fe4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ff0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ff4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ff8
			 0x0e000003                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00004
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0c700000,                                                  // Leading 1s:                                 /// 0000c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00010
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0001c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00020
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00024
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0002c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00030
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00034
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00038
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0003c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00040
			 0x0e000007,                                                  // Trailing 1s:                                /// 00044
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00048
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0004c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00050
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00058
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0005c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00060
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00064
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00068
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0006c
			 0xff800000,                                                  // -inf                                        /// 00070
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00074
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x55555555,                                                  // 4 random values                             /// 0007c
			 0x80000000,                                                  // -zero                                       /// 00080
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00084
			 0x00000000,                                                  // zero                                        /// 00088
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0008c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00098
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0009c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000ac
			 0xcb000000,                                                  // -8388608.0                                  /// 000b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000d0
			 0xbf028f5c,                                                  // -0.51                                       /// 000d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00100
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00104
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00108
			 0x0e000003,                                                  // Trailing 1s:                                /// 0010c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00110
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00118
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0011c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00120
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00124
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0012c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00130
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00134
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x3f028f5c,                                                  // 0.51                                        /// 0013c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00140
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00144
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0014c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00150
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00154
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00158
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0015c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00160
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00164
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0016c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00170
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00178
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00184
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00188
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00190
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00194
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0019c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001a4
			 0x00011111,                                                  // 9.7958E-41                                  /// 001a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0xffc00001,                                                  // -signaling NaN                              /// 001f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00200
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0020c
			 0x33333333,                                                  // 4 random values                             /// 00210
			 0xcb000000,                                                  // -8388608.0                                  /// 00214
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00218
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0021c
			 0xffc00001,                                                  // -signaling NaN                              /// 00220
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00228
			 0xffc00001,                                                  // -signaling NaN                              /// 0022c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00230
			 0x4b000000,                                                  // 8388608.0                                   /// 00234
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00238
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0023c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00240
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00244
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00254
			 0x80011111,                                                  // -9.7958E-41                                 /// 00258
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00260
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00270
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00274
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00284
			 0x7f800000,                                                  // inf                                         /// 00288
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00290
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00294
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00298
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0029c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002a8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002d8
			 0x7fc00001,                                                  // signaling NaN                               /// 002dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002e8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00304
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00308
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0030c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00310
			 0x0e000003,                                                  // Trailing 1s:                                /// 00314
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00318
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0031c
			 0x3f028f5c,                                                  // 0.51                                        /// 00320
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00324
			 0x0e000007,                                                  // Trailing 1s:                                /// 00328
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00330
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00334
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00338
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00344
			 0xff800000,                                                  // -inf                                        /// 00348
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0034c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00350
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00358
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00360
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00364
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00370
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00374
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0037c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00384
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00388
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00390
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00394
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00398
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0039c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 003ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003b0
			 0xcb000000,                                                  // -8388608.0                                  /// 003b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003e4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0xffc00001,                                                  // -signaling NaN                              /// 003f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00400
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00404
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00408
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0040c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00410
			 0x7fc00001,                                                  // signaling NaN                               /// 00414
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00418
			 0x80000000,                                                  // -zero                                       /// 0041c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00424
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00428
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0042c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00430
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00434
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x7f800000,                                                  // inf                                         /// 0043c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00440
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00444
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00448
			 0xcb000000,                                                  // -8388608.0                                  /// 0044c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00450
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00454
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0045c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00460
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00468
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00470
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00480
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00484
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00488
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0048c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00494
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0049c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004bc
			 0x80000000,                                                  // -zero                                       /// 004c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00508
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00514
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00518
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0051c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00520
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00524
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00528
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0052c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00534
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x0c400000,                                                  // Leading 1s:                                 /// 0053c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00540
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00548
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0xffc00001,                                                  // -signaling NaN                              /// 00554
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00558
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0055c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00560
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0e000007,                                                  // Trailing 1s:                                /// 0056c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00570
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00574
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0057c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00580
			 0x0c780000,                                                  // Leading 1s:                                 /// 00584
			 0x0e000001,                                                  // Trailing 1s:                                /// 00588
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0058c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00590
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00594
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0059c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005ac
			 0x7f800000,                                                  // inf                                         /// 005b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00604
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0060c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00614
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0061c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00620
			 0x4b000000,                                                  // 8388608.0                                   /// 00624
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00628
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00630
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0063c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0e000001,                                                  // Trailing 1s:                                /// 00644
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0064c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00658
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0065c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0066c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00670
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00674
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00678
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0067c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00680
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0068c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00690
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00694
			 0x0e000007,                                                  // Trailing 1s:                                /// 00698
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0069c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 006a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006c0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 006f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006f8
			 0x4b000000,                                                  // 8388608.0                                   /// 006fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00700
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0070c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00710
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00718
			 0x7fc00001,                                                  // signaling NaN                               /// 0071c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00720
			 0xcb000000,                                                  // -8388608.0                                  /// 00724
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0072c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00730
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00738
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0073c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00740
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00744
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00748
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0074c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00754
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00764
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00768
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00774
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00778
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0077c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00780
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0078c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00790
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00794
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 007b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007dc
			 0x7fc00001,                                                  // signaling NaN                               /// 007e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x7f800000,                                                  // inf                                         /// 007e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007f4
			 0x55555555,                                                  // 4 random values                             /// 007f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00800
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x00011111,                                                  // 9.7958E-41                                  /// 00808
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00814
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0081c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00824
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00828
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0082c
			 0xffc00001,                                                  // -signaling NaN                              /// 00830
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0083c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0c780000,                                                  // Leading 1s:                                 /// 00844
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0084c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00850
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00854
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0085c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00860
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00868
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00870
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0087c
			 0xffc00001,                                                  // -signaling NaN                              /// 00880
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00884
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00888
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00890
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00898
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0089c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 008c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 008e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 008ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00900
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00910
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00914
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00918
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00920
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00924
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00928
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00930
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00934
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00938
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0093c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0c600000,                                                  // Leading 1s:                                 /// 00948
			 0x0c780000,                                                  // Leading 1s:                                 /// 0094c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x3f028f5c,                                                  // 0.51                                        /// 00954
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00960
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00968
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00970
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00974
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0097c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00980
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00988
			 0x00000000,                                                  // zero                                        /// 0098c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00990
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00994
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0099c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0xff800000,                                                  // -inf                                        /// 009a4
			 0xffc00001,                                                  // -signaling NaN                              /// 009a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009d0
			 0x7fc00001,                                                  // signaling NaN                               /// 009d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 009e0
			 0x55555555,                                                  // 4 random values                             /// 009e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x7f800000,                                                  // inf                                         /// 00a08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a10
			 0x00000000,                                                  // zero                                        /// 00a14
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x7f800000,                                                  // inf                                         /// 00a24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a28
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a30
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a34
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a48
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a58
			 0x80000000,                                                  // -zero                                       /// 00a5c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a60
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a78
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a7c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a80
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a90
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a94
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a98
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00aa4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00aa8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00aac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ab0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ab4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ab8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00abc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ac0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ac4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ad4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ad8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ae0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00aec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00af0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00af4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00afc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b00
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b08
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b10
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b34
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b40
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b44
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b50
			 0xffc00001,                                                  // -signaling NaN                              /// 00b54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b70
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b74
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b80
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b88
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b8c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b94
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ba0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ba8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bac
			 0xff800000,                                                  // -inf                                        /// 00bb0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bbc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bc0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bc4
			 0x00000000,                                                  // zero                                        /// 00bc8
			 0xff800000,                                                  // -inf                                        /// 00bcc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bdc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bf4
			 0xff800000,                                                  // -inf                                        /// 00bf8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c00
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c04
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c1c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c28
			 0x7f800000,                                                  // inf                                         /// 00c2c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c4c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c58
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c5c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c60
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c64
			 0x80000000,                                                  // -zero                                       /// 00c68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c6c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c74
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c7c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c80
			 0xffc00001,                                                  // -signaling NaN                              /// 00c84
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c88
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ca0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ca4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ca8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cb4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cb8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cc0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cc8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cd0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cd4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cdc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ce0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ce8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cf0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cf8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cfc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d00
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d04
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d10
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x7fc00001,                                                  // signaling NaN                               /// 00d18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d20
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d44
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d50
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d58
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d90
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00da0
			 0x33333333,                                                  // 4 random values                             /// 00da4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00da8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00db0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00db4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dc4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dd4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ddc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00de8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00df0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00df4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e08
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e40
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e48
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e54
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e5c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e64
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e6c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e70
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e7c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e8c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e90
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e98
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ea0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00eac
			 0x7f800000,                                                  // inf                                         /// 00eb0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00eb4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00eb8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ec4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x33333333,                                                  // 4 random values                             /// 00ed4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00edc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ee0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ee4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ef0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00efc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f10
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f1c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f28
			 0x00000000,                                                  // zero                                        /// 00f2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f30
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f4c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f64
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f6c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f70
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f78
			 0x7fc00001,                                                  // signaling NaN                               /// 00f7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f88
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f8c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fa0
			 0x7f800000,                                                  // inf                                         /// 00fa4
			 0xcb000000,                                                  // -8388608.0                                  /// 00fa8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fb4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fb8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fbc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fc0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fc4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fc8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fd0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fd4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fdc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fe4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ff0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ff8
			 0x80800000 // min norm                                      // SP - ve numbers                             /// last
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
			 0x000003c4,
			 0x00000394,
			 0x000007ac,
			 0x00000320,
			 0x000000b0,
			 0x00000284,
			 0x00000500,
			 0x0000073c,

			 /// vpu register f2
			 0x41f00000,
			 0x41900000,
			 0x41500000,
			 0x41900000,
			 0x41900000,
			 0x40e00000,
			 0x40400000,
			 0x41f80000,

			 /// vpu register f3
			 0x41c00000,
			 0x41980000,
			 0x41200000,
			 0x41600000,
			 0x3f800000,
			 0x40000000,
			 0x41500000,
			 0x41e80000,

			 /// vpu register f4
			 0x41e80000,
			 0x41500000,
			 0x41c00000,
			 0x41100000,
			 0x3f800000,
			 0x41200000,
			 0x41e00000,
			 0x41200000,

			 /// vpu register f5
			 0x41400000,
			 0x41880000,
			 0x41c00000,
			 0x41d80000,
			 0x41880000,
			 0x41b80000,
			 0x40400000,
			 0x40800000,

			 /// vpu register f6
			 0x41300000,
			 0x41c00000,
			 0x41b80000,
			 0x42000000,
			 0x41600000,
			 0x41b00000,
			 0x41d80000,
			 0x41900000,

			 /// vpu register f7
			 0x41b80000,
			 0x41980000,
			 0x41e80000,
			 0x41800000,
			 0x41700000,
			 0x41980000,
			 0x41800000,
			 0x41880000,

			 /// vpu register f8
			 0x41200000,
			 0x41400000,
			 0x41d00000,
			 0x41700000,
			 0x41d00000,
			 0x41b80000,
			 0x41f80000,
			 0x41f80000,

			 /// vpu register f9
			 0x40000000,
			 0x41500000,
			 0x41300000,
			 0x41600000,
			 0x40400000,
			 0x41880000,
			 0x41980000,
			 0x40000000,

			 /// vpu register f10
			 0x40400000,
			 0x41400000,
			 0x41400000,
			 0x41a00000,
			 0x41b80000,
			 0x40000000,
			 0x41980000,
			 0x42000000,

			 /// vpu register f11
			 0x40c00000,
			 0x41d80000,
			 0x40a00000,
			 0x41000000,
			 0x41a80000,
			 0x41000000,
			 0x41f00000,
			 0x41f80000,

			 /// vpu register f12
			 0x41c00000,
			 0x41d80000,
			 0x41500000,
			 0x41600000,
			 0x41c00000,
			 0x40000000,
			 0x41f00000,
			 0x41900000,

			 /// vpu register f13
			 0x40e00000,
			 0x40e00000,
			 0x41800000,
			 0x41f80000,
			 0x41e80000,
			 0x41700000,
			 0x41c00000,
			 0x41100000,

			 /// vpu register f14
			 0x40800000,
			 0x41b00000,
			 0x41d00000,
			 0x3f800000,
			 0x41c80000,
			 0x42000000,
			 0x41400000,
			 0x41100000,

			 /// vpu register f15
			 0x3f800000,
			 0x40c00000,
			 0x41f80000,
			 0x41300000,
			 0x40c00000,
			 0x41300000,
			 0x40c00000,
			 0x41a80000,

			 /// vpu register f16
			 0x41b80000,
			 0x41b00000,
			 0x41400000,
			 0x41600000,
			 0x41100000,
			 0x41c80000,
			 0x41900000,
			 0x41d00000,

			 /// vpu register f17
			 0x41c80000,
			 0x41f80000,
			 0x41000000,
			 0x41400000,
			 0x41e00000,
			 0x41e80000,
			 0x41b00000,
			 0x41500000,

			 /// vpu register f18
			 0x3f800000,
			 0x41a80000,
			 0x41a00000,
			 0x41e80000,
			 0x40400000,
			 0x41500000,
			 0x41a80000,
			 0x41a00000,

			 /// vpu register f19
			 0x41200000,
			 0x42000000,
			 0x41f80000,
			 0x41d00000,
			 0x41a80000,
			 0x40800000,
			 0x40800000,
			 0x41980000,

			 /// vpu register f20
			 0x40e00000,
			 0x41800000,
			 0x41d00000,
			 0x40e00000,
			 0x41b00000,
			 0x41100000,
			 0x40800000,
			 0x41f80000,

			 /// vpu register f21
			 0x40a00000,
			 0x41c00000,
			 0x41a00000,
			 0x41800000,
			 0x41b00000,
			 0x41f80000,
			 0x42000000,
			 0x41f00000,

			 /// vpu register f22
			 0x41300000,
			 0x41600000,
			 0x41d00000,
			 0x41500000,
			 0x41d80000,
			 0x41f80000,
			 0x40e00000,
			 0x41a80000,

			 /// vpu register f23
			 0x41700000,
			 0x41700000,
			 0x41d80000,
			 0x40800000,
			 0x41d00000,
			 0x41d00000,
			 0x41f80000,
			 0x41700000,

			 /// vpu register f24
			 0x41c80000,
			 0x41300000,
			 0x41900000,
			 0x41880000,
			 0x41f80000,
			 0x40c00000,
			 0x41a80000,
			 0x40a00000,

			 /// vpu register f25
			 0x41000000,
			 0x41c00000,
			 0x41000000,
			 0x40800000,
			 0x41b80000,
			 0x41d80000,
			 0x41600000,
			 0x41b00000,

			 /// vpu register f26
			 0x40400000,
			 0x41980000,
			 0x41d00000,
			 0x41f00000,
			 0x41c00000,
			 0x41200000,
			 0x41f00000,
			 0x3f800000,

			 /// vpu register f27
			 0x41d00000,
			 0x41900000,
			 0x40c00000,
			 0x40a00000,
			 0x41500000,
			 0x41e80000,
			 0x41f80000,
			 0x41900000,

			 /// vpu register f28
			 0x41880000,
			 0x41e00000,
			 0x41f00000,
			 0x40400000,
			 0x41600000,
			 0x41100000,
			 0x40a00000,
			 0x41a80000,

			 /// vpu register f29
			 0x41000000,
			 0x41a80000,
			 0x41b00000,
			 0x41e00000,
			 0x41980000,
			 0x41900000,
			 0x40800000,
			 0x41c80000,

			 /// vpu register f30
			 0x40400000,
			 0x41400000,
			 0x41100000,
			 0x41200000,
			 0x41900000,
			 0x41900000,
			 0x41d80000,
			 0x40e00000,

			 /// vpu register f31
			 0x41300000,
			 0x41500000,
			 0x41e00000,
			 0x41400000,
			 0x41800000,
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
		"feqm.ps m2, f12, f30\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f17, f21\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f19, f23\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m5, f14, f13\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f4, f17\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f23, f9\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f9, f15\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m5, f6, f1\n"                                ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f25, f16\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f21, f16\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f19, f16\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f3, f26\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f12, f20\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m2, f25, f19\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f1, f7\n"                                ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m5, f29, f1\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f19, f30\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f10, f21\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f14, f8\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m5, f23, f5\n"                               ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m2, f8, f26\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f1, f30\n"                               ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f21, f5\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f21, f2\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m3, f27, f22\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f19, f26\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m2, f21, f16\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f0, f11\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f16, f15\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m3, f18, f2\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f17, f8\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f2, f24\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f19, f17\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f15, f18\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m5, f30, f7\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f23, f10\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f27, f2\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m6, f27, f23\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f20, f16\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f15, f15\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f25, f16\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f3, f3\n"                                ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f22, f27\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f1, f3\n"                                ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f15, f21\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m5, f19, f18\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f15, f3\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m2, f28, f26\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m6, f1, f23\n"                               ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f20, f3\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m6, f10, f29\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m3, f17, f22\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m3, f30, f24\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f24, f2\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f4, f4\n"                                ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f2, f7\n"                                ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f9, f1\n"                                ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f5, f11\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f0, f20\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f30, f28\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f30, f15\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f0, f24\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m6, f0, f0\n"                                ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m5, f0, f29\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m2, f30, f25\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f13, f27\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f16, f3\n"                               ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f1, f29\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f28, f27\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f23, f17\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f7, f20\n"                               ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m6, f16, f20\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f27, f14\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m6, f13, f18\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m5, f30, f14\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f24, f19\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m2, f17, f28\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f15, f18\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f24, f10\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f11, f17\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f5, f15\n"                               ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m3, f7, f16\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f26, f20\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m5, f4, f10\n"                               ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f4, f19\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f2, f17\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m6, f18, f3\n"                               ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f12, f26\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m0, f7, f17\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f6, f4\n"                                ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f17, f1\n"                               ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m5, f18, f13\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m2, f21, f11\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f6, f29\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m1, f19, f23\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m6, f2, f10\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m4, f25, f30\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m5, f24, f19\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"feqm.ps m7, f15, f17\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
