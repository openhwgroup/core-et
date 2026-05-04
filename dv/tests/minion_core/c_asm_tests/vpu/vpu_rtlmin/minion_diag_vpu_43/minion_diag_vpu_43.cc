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
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00000
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00004
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00008
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0000c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00010
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00018
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0001c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00024
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00030
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00034
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0003c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00040
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00044
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00048
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00050
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00054
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00058
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0005c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00064
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0006c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00070
			 0x80000000,                                                  // -zero                                       /// 00074
			 0x4b000000,                                                  // 8388608.0                                   /// 00078
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0007c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00080
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00084
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0008c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00090
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00094
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 000a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000cc
			 0x00000000,                                                  // zero                                        /// 000d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00108
			 0x7f800000,                                                  // inf                                         /// 0010c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00110
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00114
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00118
			 0x0c780000,                                                  // Leading 1s:                                 /// 0011c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00120
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00128
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00130
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00134
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00140
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00144
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00148
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0014c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00158
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00160
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00168
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0016c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00170
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00174
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0017c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x0c400000,                                                  // Leading 1s:                                 /// 00184
			 0x7f800000,                                                  // inf                                         /// 00188
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0018c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00190
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00198
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0019c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001d0
			 0x55555555,                                                  // 4 random values                             /// 001d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0xcb000000,                                                  // -8388608.0                                  /// 0020c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00210
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00214
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00218
			 0x7fc00001,                                                  // signaling NaN                               /// 0021c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00224
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00228
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00230
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00244
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00248
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00254
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0025c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00264
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00268
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00270
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00274
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00278
			 0x80011111,                                                  // -9.7958E-41                                 /// 0027c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00280
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00284
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00288
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00290
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00294
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 002dc
			 0x33333333,                                                  // 4 random values                             /// 002e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x4b000000,                                                  // 8388608.0                                   /// 002f8
			 0x00000000,                                                  // zero                                        /// 002fc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00300
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0030c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00310
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00314
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00318
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0031c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00320
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00328
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0032c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00330
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00340
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00344
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x80000000,                                                  // -zero                                       /// 0034c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00350
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00354
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00358
			 0xffc00001,                                                  // -signaling NaN                              /// 0035c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00364
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00368
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0036c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00374
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00378
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0037c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00380
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00388
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0038c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00394
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00398
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0039c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00400
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00404
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00408
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0040c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00410
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00414
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00418
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00424
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00428
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0042c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00434
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00438
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0043c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00440
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00448
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00454
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00458
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00460
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00470
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00474
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00478
			 0x0c780000,                                                  // Leading 1s:                                 /// 0047c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00480
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00484
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00488
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0048c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00494
			 0x4b000000,                                                  // 8388608.0                                   /// 00498
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xff800000,                                                  // -inf                                        /// 004a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0xff800000,                                                  // -inf                                        /// 004bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 004dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004e0
			 0x4b000000,                                                  // 8388608.0                                   /// 004e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004f0
			 0x00000000,                                                  // zero                                        /// 004f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00500
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00504
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0050c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00510
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00514
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00518
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0051c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00524
			 0x80000000,                                                  // -zero                                       /// 00528
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0052c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00530
			 0x80011111,                                                  // -9.7958E-41                                 /// 00534
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0053c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00544
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00550
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00554
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00564
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00568
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0056c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00570
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00574
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00578
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0057c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00580
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00588
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0058c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00594
			 0x33333333,                                                  // 4 random values                             /// 00598
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0xff800000,                                                  // -inf                                        /// 005a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x80000000,                                                  // -zero                                       /// 005cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 005d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00604
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00608
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0060c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00614
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00620
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00624
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0xbf028f5c,                                                  // -0.51                                       /// 0062c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00630
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00638
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00640
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00644
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00648
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00650
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00654
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00658
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0065c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00660
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00664
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00668
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0066c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00670
			 0x0c400000,                                                  // Leading 1s:                                 /// 00674
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x7f800000,                                                  // inf                                         /// 0067c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0e000001,                                                  // Trailing 1s:                                /// 00684
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00688
			 0x00011111,                                                  // 9.7958E-41                                  /// 0068c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00690
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006a4
			 0x00011111,                                                  // 9.7958E-41                                  /// 006a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 006dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006f0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00704
			 0xffc00001,                                                  // -signaling NaN                              /// 00708
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00710
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00714
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00718
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0071c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x0c400000,                                                  // Leading 1s:                                 /// 00724
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00728
			 0xff800000,                                                  // -inf                                        /// 0072c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00730
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00734
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00738
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00740
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00744
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00748
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0074c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00754
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00758
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0075c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00760
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00764
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00768
			 0x7f800000,                                                  // inf                                         /// 0076c
			 0x00000000,                                                  // zero                                        /// 00770
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00778
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0077c
			 0x4b000000,                                                  // 8388608.0                                   /// 00780
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00784
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00788
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00790
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0xffc00001,                                                  // -signaling NaN                              /// 0079c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007a8
			 0x55555555,                                                  // 4 random values                             /// 007ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007b0
			 0x00000000,                                                  // zero                                        /// 007b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d0
			 0x3f028f5c,                                                  // 0.51                                        /// 007d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007dc
			 0x80000000,                                                  // -zero                                       /// 007e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007e4
			 0x55555555,                                                  // 4 random values                             /// 007e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0xbf028f5c,                                                  // -0.51                                       /// 007f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007fc
			 0x33333333,                                                  // 4 random values                             /// 00800
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00808
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00810
			 0x0e000003,                                                  // Trailing 1s:                                /// 00814
			 0x0c600000,                                                  // Leading 1s:                                 /// 00818
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0081c
			 0x00000000,                                                  // zero                                        /// 00820
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00824
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0082c
			 0x4b000000,                                                  // 8388608.0                                   /// 00830
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00838
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00840
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00844
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00848
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00850
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00854
			 0xcb000000,                                                  // -8388608.0                                  /// 00858
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0085c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00860
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00864
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0c700000,                                                  // Leading 1s:                                 /// 0086c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00870
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0087c
			 0xff800000,                                                  // -inf                                        /// 00880
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00884
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0088c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00890
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00894
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00898
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x33333333,                                                  // 4 random values                             /// 008b4
			 0x33333333,                                                  // 4 random values                             /// 008b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0xbf028f5c,                                                  // -0.51                                       /// 008d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008f0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00904
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00908
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00910
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00914
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0091c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00920
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00924
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0092c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00930
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x0e000001,                                                  // Trailing 1s:                                /// 0093c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00944
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00948
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00950
			 0xbf028f5c,                                                  // -0.51                                       /// 00954
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00958
			 0x0c400000,                                                  // Leading 1s:                                 /// 0095c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00964
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00974
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00978
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00984
			 0xcb000000,                                                  // -8388608.0                                  /// 00988
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0098c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00990
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00994
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00998
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0099c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 009d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009fc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a04
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a0c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a10
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a24
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a2c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a3c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a40
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a44
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a4c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a50
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a64
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a6c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a7c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a80
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a84
			 0x55555555,                                                  // 4 random values                             /// 00a88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a90
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00aa4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00aa8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00aac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ab0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ab4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ab8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00abc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ac8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00acc
			 0x80000000,                                                  // -zero                                       /// 00ad0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ad4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00adc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ae0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ae4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ae8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00aec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00af0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x4b000000,                                                  // 8388608.0                                   /// 00b00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b24
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b38
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b44
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b48
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b54
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b78
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b80
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b84
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b90
			 0x4b000000,                                                  // 8388608.0                                   /// 00b94
			 0x33333333,                                                  // 4 random values                             /// 00b98
			 0x55555555,                                                  // 4 random values                             /// 00b9c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ba0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ba4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ba8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bc0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bc4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bc8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bcc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bd0
			 0xcb000000,                                                  // -8388608.0                                  /// 00bd4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bdc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00be0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00be8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bf0
			 0xffc00001,                                                  // -signaling NaN                              /// 00bf4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bf8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c00
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c04
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c0c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c18
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c20
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c38
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c3c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c48
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c58
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c70
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c74
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c78
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c88
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ca4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ca8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cbc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cc0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cd4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cdc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cf0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cf8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cfc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d04
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d10
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d18
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d1c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d30
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d34
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d4c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d54
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d58
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d5c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d60
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d74
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d84
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d8c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d94
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d98
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00da0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00da4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00db0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00db4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00db8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dd0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x4b000000,                                                  // 8388608.0                                   /// 00dd8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ddc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00de0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x7fc00001,                                                  // signaling NaN                               /// 00de8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00df4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00df8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dfc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e10
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e1c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e30
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e58
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e5c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e74
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0xbf028f5c,                                                  // -0.51                                       /// 00e7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x00000000,                                                  // zero                                        /// 00e88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e8c
			 0x33333333,                                                  // 4 random values                             /// 00e90
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e94
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00ea0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ea4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00eb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eb4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00eb8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ebc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ec0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ec4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ec8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ecc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ed4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ed8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00edc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ee8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00eec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ef0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ef8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f0c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f18
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f40
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f44
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f54
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f58
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f74
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f78
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f7c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f8c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f90
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f9c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fa4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fa8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fb8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fc0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fc8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fcc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fd0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ff0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ff4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x7f7ffffe // max norm - 2ulp                               // max norm +ve                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x64333965, /// 0x0
			 0x3b85a906, /// 0x4
			 0xa871ff5d, /// 0x8
			 0x61b6049d, /// 0xc
			 0x2942829e, /// 0x10
			 0x2afce981, /// 0x14
			 0xbdb7cbb0, /// 0x18
			 0xe07545e5, /// 0x1c
			 0x8dc64cbe, /// 0x20
			 0xce7a3778, /// 0x24
			 0xcfbe2c9e, /// 0x28
			 0xbae15b33, /// 0x2c
			 0x4d5a0245, /// 0x30
			 0x529fa1bf, /// 0x34
			 0x2ac43798, /// 0x38
			 0xb62a0f26, /// 0x3c
			 0xc4d2a3c8, /// 0x40
			 0xd5eca79e, /// 0x44
			 0x5f58d672, /// 0x48
			 0x0c42784b, /// 0x4c
			 0xa1abafdd, /// 0x50
			 0xeb240c56, /// 0x54
			 0xeb0cd3d4, /// 0x58
			 0xc6049173, /// 0x5c
			 0xba12fbc0, /// 0x60
			 0x2ba8fcd4, /// 0x64
			 0xdfa97f48, /// 0x68
			 0x264254b1, /// 0x6c
			 0xc9e6ddbe, /// 0x70
			 0xbbe77394, /// 0x74
			 0x1720098e, /// 0x78
			 0x8dfe2892, /// 0x7c
			 0x03a9ee1f, /// 0x80
			 0x1649bf3c, /// 0x84
			 0x6bbc5d04, /// 0x88
			 0xd45b1685, /// 0x8c
			 0x5aa2e857, /// 0x90
			 0xe9ae4b89, /// 0x94
			 0x4ff54bef, /// 0x98
			 0xff49863a, /// 0x9c
			 0xc7d41318, /// 0xa0
			 0xb50dd13a, /// 0xa4
			 0xabc370e6, /// 0xa8
			 0x187db946, /// 0xac
			 0xcbb52c5a, /// 0xb0
			 0x80eb4582, /// 0xb4
			 0xc6308be1, /// 0xb8
			 0x6673e1e2, /// 0xbc
			 0x2f6bb0f1, /// 0xc0
			 0x05ddd56e, /// 0xc4
			 0x7681aaf0, /// 0xc8
			 0xf742254a, /// 0xcc
			 0x63664c9e, /// 0xd0
			 0xece31338, /// 0xd4
			 0x172f320a, /// 0xd8
			 0xec352591, /// 0xdc
			 0xc9d0d1af, /// 0xe0
			 0x481dec4a, /// 0xe4
			 0xe3918b78, /// 0xe8
			 0x98d676fa, /// 0xec
			 0xf694c955, /// 0xf0
			 0xbeacadd2, /// 0xf4
			 0xb1aae30f, /// 0xf8
			 0x56519c41, /// 0xfc
			 0x9a5aa2f4, /// 0x100
			 0x99957faa, /// 0x104
			 0xff5d8d3d, /// 0x108
			 0xdf6a89f2, /// 0x10c
			 0x42847a94, /// 0x110
			 0x10a5b89e, /// 0x114
			 0xaab6b7e2, /// 0x118
			 0x54febc82, /// 0x11c
			 0xe10d6753, /// 0x120
			 0xe33f3d76, /// 0x124
			 0xe4fdd03f, /// 0x128
			 0x84c5e331, /// 0x12c
			 0xaded224a, /// 0x130
			 0x26155d6d, /// 0x134
			 0xe014b398, /// 0x138
			 0x602c0a1c, /// 0x13c
			 0xd51dc1ab, /// 0x140
			 0x12c1481a, /// 0x144
			 0x48f6c677, /// 0x148
			 0x61fb4cfe, /// 0x14c
			 0xe6ea718f, /// 0x150
			 0x16e7986f, /// 0x154
			 0x2a776a32, /// 0x158
			 0x0c9e6a12, /// 0x15c
			 0x9d2ab85a, /// 0x160
			 0x725e0133, /// 0x164
			 0x453a648a, /// 0x168
			 0x7be17323, /// 0x16c
			 0x6bd16ec0, /// 0x170
			 0x99cae92b, /// 0x174
			 0x86e46eef, /// 0x178
			 0x81ab4f52, /// 0x17c
			 0xe3c03b15, /// 0x180
			 0x9fe9f68d, /// 0x184
			 0x03d07229, /// 0x188
			 0xffecae05, /// 0x18c
			 0x1d665f03, /// 0x190
			 0x4f5f2792, /// 0x194
			 0x484929db, /// 0x198
			 0x5f5c6ad6, /// 0x19c
			 0xcaaeda50, /// 0x1a0
			 0x8ea35367, /// 0x1a4
			 0xbb24b54d, /// 0x1a8
			 0x094a4265, /// 0x1ac
			 0xd3b52ecb, /// 0x1b0
			 0x6fbd5c85, /// 0x1b4
			 0x4263eb8c, /// 0x1b8
			 0x2646dda8, /// 0x1bc
			 0xc497be51, /// 0x1c0
			 0x4964eda7, /// 0x1c4
			 0x949363a5, /// 0x1c8
			 0xe2418fdf, /// 0x1cc
			 0xaae33c6d, /// 0x1d0
			 0xdccd4f27, /// 0x1d4
			 0x02e07b03, /// 0x1d8
			 0xea59af1e, /// 0x1dc
			 0xdb3c4159, /// 0x1e0
			 0x649dada4, /// 0x1e4
			 0xe4cef2d3, /// 0x1e8
			 0x11dbc31a, /// 0x1ec
			 0x3885bfd5, /// 0x1f0
			 0xd7dc140c, /// 0x1f4
			 0x0696357b, /// 0x1f8
			 0x7abb2d72, /// 0x1fc
			 0x70293dfd, /// 0x200
			 0x541b16e0, /// 0x204
			 0x05d2840a, /// 0x208
			 0x21e8ce81, /// 0x20c
			 0x3093c116, /// 0x210
			 0x25e7e24c, /// 0x214
			 0x87b743c8, /// 0x218
			 0x364ee0dc, /// 0x21c
			 0x93071c0a, /// 0x220
			 0x0ac3b168, /// 0x224
			 0xd946dd0a, /// 0x228
			 0xc4c8a17e, /// 0x22c
			 0xcbce1d9d, /// 0x230
			 0xfb2978cb, /// 0x234
			 0x1e1c27c5, /// 0x238
			 0x51c0788f, /// 0x23c
			 0x91d143be, /// 0x240
			 0x077440cf, /// 0x244
			 0x544a22e6, /// 0x248
			 0xc0dca4d5, /// 0x24c
			 0x272bad53, /// 0x250
			 0x4e762c4f, /// 0x254
			 0xf984fc62, /// 0x258
			 0xc17543d3, /// 0x25c
			 0x4567fee0, /// 0x260
			 0xe8820a15, /// 0x264
			 0xd130f630, /// 0x268
			 0x671e85c8, /// 0x26c
			 0xe62f9660, /// 0x270
			 0x4febcd8c, /// 0x274
			 0xd85af9f0, /// 0x278
			 0x4f0345a5, /// 0x27c
			 0xab57821a, /// 0x280
			 0xb5934f2c, /// 0x284
			 0x73d9b1a6, /// 0x288
			 0xa8015e5b, /// 0x28c
			 0x3cf70de2, /// 0x290
			 0x3ecb950f, /// 0x294
			 0x782bdfcd, /// 0x298
			 0x403aa2cc, /// 0x29c
			 0x6dab8509, /// 0x2a0
			 0x994aabeb, /// 0x2a4
			 0x95fafdd3, /// 0x2a8
			 0xc760a56e, /// 0x2ac
			 0xd844ff7b, /// 0x2b0
			 0x69896889, /// 0x2b4
			 0x8b10cb15, /// 0x2b8
			 0x9fe4a800, /// 0x2bc
			 0x0d1bf809, /// 0x2c0
			 0x60b42fec, /// 0x2c4
			 0x1f9a2386, /// 0x2c8
			 0x2802121c, /// 0x2cc
			 0x86355335, /// 0x2d0
			 0xa7d48216, /// 0x2d4
			 0x1e491886, /// 0x2d8
			 0x96e150a3, /// 0x2dc
			 0x9de30a74, /// 0x2e0
			 0x17723357, /// 0x2e4
			 0x01f1bc0f, /// 0x2e8
			 0xd783c25f, /// 0x2ec
			 0x11991187, /// 0x2f0
			 0xbd88c308, /// 0x2f4
			 0x03a8d6ae, /// 0x2f8
			 0xa4e7c3b1, /// 0x2fc
			 0xc53ffb96, /// 0x300
			 0x18dc2913, /// 0x304
			 0x3821d072, /// 0x308
			 0xa829a7ff, /// 0x30c
			 0x703db362, /// 0x310
			 0x1dcfed99, /// 0x314
			 0xda96548a, /// 0x318
			 0xf678b55b, /// 0x31c
			 0x456cadaf, /// 0x320
			 0xe93f9554, /// 0x324
			 0x8fdeceb4, /// 0x328
			 0x23657ff3, /// 0x32c
			 0x7ad9d771, /// 0x330
			 0x6acc59b9, /// 0x334
			 0x1e41c7d4, /// 0x338
			 0x198719a3, /// 0x33c
			 0xfe5e5cf4, /// 0x340
			 0xa9f12815, /// 0x344
			 0x593afb32, /// 0x348
			 0x2c19aafd, /// 0x34c
			 0x5889549b, /// 0x350
			 0x9a90969d, /// 0x354
			 0x2ff5e226, /// 0x358
			 0x66f9f2a2, /// 0x35c
			 0x8cb4c55f, /// 0x360
			 0xe34dfde0, /// 0x364
			 0x26e048d3, /// 0x368
			 0x672b6d13, /// 0x36c
			 0x37c756c6, /// 0x370
			 0xac20fb8a, /// 0x374
			 0xd8a324c1, /// 0x378
			 0x0687d07d, /// 0x37c
			 0xfdeec1b1, /// 0x380
			 0xb2c10155, /// 0x384
			 0x2e963b36, /// 0x388
			 0xd22dc3f1, /// 0x38c
			 0xb6d2adc4, /// 0x390
			 0x7360eab4, /// 0x394
			 0xc69f36a4, /// 0x398
			 0x0bd4e9b4, /// 0x39c
			 0x2687a429, /// 0x3a0
			 0x92d1f8f8, /// 0x3a4
			 0xd4cb3f71, /// 0x3a8
			 0x0206d604, /// 0x3ac
			 0x87149869, /// 0x3b0
			 0x0a17f1a3, /// 0x3b4
			 0x9837d718, /// 0x3b8
			 0xc3625def, /// 0x3bc
			 0xf57ae648, /// 0x3c0
			 0x6a483056, /// 0x3c4
			 0xc536f2c1, /// 0x3c8
			 0x315def08, /// 0x3cc
			 0xa3880549, /// 0x3d0
			 0x9081b9c1, /// 0x3d4
			 0x7c9b2113, /// 0x3d8
			 0xfeca495e, /// 0x3dc
			 0xcd1aea90, /// 0x3e0
			 0x990114cf, /// 0x3e4
			 0x8295c0c1, /// 0x3e8
			 0x1a8e5e56, /// 0x3ec
			 0x76d46636, /// 0x3f0
			 0xb5086525, /// 0x3f4
			 0x170691c5, /// 0x3f8
			 0xf6dea3b2, /// 0x3fc
			 0x1a67f389, /// 0x400
			 0x6b02e50e, /// 0x404
			 0x20b62480, /// 0x408
			 0x93e5e962, /// 0x40c
			 0x885e15f8, /// 0x410
			 0x0c157a61, /// 0x414
			 0x6eab7b7d, /// 0x418
			 0xdf9c8287, /// 0x41c
			 0x67c2aa44, /// 0x420
			 0xcec2beaf, /// 0x424
			 0xf65aa69e, /// 0x428
			 0x0f5085f7, /// 0x42c
			 0xf7002ccc, /// 0x430
			 0x2fa2899d, /// 0x434
			 0x5f77cec5, /// 0x438
			 0xb6e9cba9, /// 0x43c
			 0xfd525467, /// 0x440
			 0xff64598d, /// 0x444
			 0x6dfccddf, /// 0x448
			 0xe75bcbf5, /// 0x44c
			 0x41e19ffc, /// 0x450
			 0xa52acd2e, /// 0x454
			 0x3bfe5d65, /// 0x458
			 0x6dc81c8c, /// 0x45c
			 0x5d09fc2f, /// 0x460
			 0x68d0cd3d, /// 0x464
			 0x1f25319f, /// 0x468
			 0x2f07a793, /// 0x46c
			 0x769349ce, /// 0x470
			 0x26352685, /// 0x474
			 0x89c8b9df, /// 0x478
			 0x2b32497a, /// 0x47c
			 0x006b80b9, /// 0x480
			 0x18d5ece9, /// 0x484
			 0x257f338e, /// 0x488
			 0x3c1469f6, /// 0x48c
			 0x8520b083, /// 0x490
			 0x742ba21e, /// 0x494
			 0xee3f965f, /// 0x498
			 0x3d4700f8, /// 0x49c
			 0x82c99455, /// 0x4a0
			 0x6ca3b538, /// 0x4a4
			 0x495aba92, /// 0x4a8
			 0xbde81a94, /// 0x4ac
			 0xcb7664bc, /// 0x4b0
			 0xdee8a8e5, /// 0x4b4
			 0xb5f61ad6, /// 0x4b8
			 0x0ee8206b, /// 0x4bc
			 0x25349e77, /// 0x4c0
			 0x5555835c, /// 0x4c4
			 0x4feeb2f7, /// 0x4c8
			 0x91f2627a, /// 0x4cc
			 0xab6b66c1, /// 0x4d0
			 0xbcd1f3fb, /// 0x4d4
			 0xc00baaa9, /// 0x4d8
			 0x73d44849, /// 0x4dc
			 0x61c76363, /// 0x4e0
			 0xa9427b88, /// 0x4e4
			 0x87d55568, /// 0x4e8
			 0xf4d80695, /// 0x4ec
			 0x53346ce3, /// 0x4f0
			 0xf3cd012f, /// 0x4f4
			 0xc244f7c4, /// 0x4f8
			 0xda8d8833, /// 0x4fc
			 0x265111ec, /// 0x500
			 0x53803a62, /// 0x504
			 0x75c1ddcb, /// 0x508
			 0xebcff770, /// 0x50c
			 0x86bdf75d, /// 0x510
			 0xd709bbf4, /// 0x514
			 0x8546497f, /// 0x518
			 0xca210d76, /// 0x51c
			 0xd12d2359, /// 0x520
			 0x859b88c3, /// 0x524
			 0xcdebb37d, /// 0x528
			 0xfe80f4b9, /// 0x52c
			 0xed411991, /// 0x530
			 0xa265cc48, /// 0x534
			 0xff7983ff, /// 0x538
			 0x9d89e8a1, /// 0x53c
			 0x16bddd15, /// 0x540
			 0x101826c2, /// 0x544
			 0x85f90ab1, /// 0x548
			 0xcc5cb9fb, /// 0x54c
			 0xc73fa4fe, /// 0x550
			 0x677453a4, /// 0x554
			 0xfc22c029, /// 0x558
			 0xb19fb3d9, /// 0x55c
			 0x8a97e684, /// 0x560
			 0xf4016522, /// 0x564
			 0x9ccd7429, /// 0x568
			 0x631a73be, /// 0x56c
			 0x8634cd6e, /// 0x570
			 0x93341a48, /// 0x574
			 0xead19e6c, /// 0x578
			 0xd6395425, /// 0x57c
			 0xbb8604de, /// 0x580
			 0xa629593c, /// 0x584
			 0x68f3b327, /// 0x588
			 0x276dea1c, /// 0x58c
			 0xa5247efd, /// 0x590
			 0x733772d9, /// 0x594
			 0x4f4fcb35, /// 0x598
			 0x6b3d290d, /// 0x59c
			 0x7768c543, /// 0x5a0
			 0xe46976fa, /// 0x5a4
			 0x1fdb21d8, /// 0x5a8
			 0x25f2c6bf, /// 0x5ac
			 0x7839fd49, /// 0x5b0
			 0xe7eae76c, /// 0x5b4
			 0x45c7c84c, /// 0x5b8
			 0x861fef47, /// 0x5bc
			 0x561e518f, /// 0x5c0
			 0xc067f1b4, /// 0x5c4
			 0x3d511876, /// 0x5c8
			 0xc93ee5b6, /// 0x5cc
			 0x68f28127, /// 0x5d0
			 0x123702c0, /// 0x5d4
			 0x88eb34c0, /// 0x5d8
			 0xbb5746c2, /// 0x5dc
			 0x80e9f5e2, /// 0x5e0
			 0xf56d3d99, /// 0x5e4
			 0x2a68b8c8, /// 0x5e8
			 0x02d880f5, /// 0x5ec
			 0x1172d731, /// 0x5f0
			 0x4d1e81c2, /// 0x5f4
			 0xa613d0f7, /// 0x5f8
			 0x452ca29c, /// 0x5fc
			 0xc642bb35, /// 0x600
			 0x24401c9d, /// 0x604
			 0x97206328, /// 0x608
			 0x3309acca, /// 0x60c
			 0xcf1d922f, /// 0x610
			 0x69862d6e, /// 0x614
			 0x87a1e09e, /// 0x618
			 0xdea03a6d, /// 0x61c
			 0x5fa45198, /// 0x620
			 0x56df7601, /// 0x624
			 0x3826f713, /// 0x628
			 0x644db7e1, /// 0x62c
			 0x4d75551f, /// 0x630
			 0x55bdc294, /// 0x634
			 0xf4afe03c, /// 0x638
			 0xbb99af59, /// 0x63c
			 0x147ec829, /// 0x640
			 0x71b9abb9, /// 0x644
			 0x3a5d0454, /// 0x648
			 0xcdf1af5e, /// 0x64c
			 0x71c006d0, /// 0x650
			 0xd12ce0f4, /// 0x654
			 0xc4780e49, /// 0x658
			 0x910975dd, /// 0x65c
			 0x3b78f12a, /// 0x660
			 0xe75f382f, /// 0x664
			 0x043884dd, /// 0x668
			 0x32c264ee, /// 0x66c
			 0xf730e59c, /// 0x670
			 0x75b336bf, /// 0x674
			 0x552567c9, /// 0x678
			 0xb88a2f4b, /// 0x67c
			 0x16811663, /// 0x680
			 0x7343c23b, /// 0x684
			 0xf0d22324, /// 0x688
			 0xb4eac3b8, /// 0x68c
			 0x2a0c584d, /// 0x690
			 0x326f6acf, /// 0x694
			 0x920ca60d, /// 0x698
			 0x0baabd79, /// 0x69c
			 0xb007a9e9, /// 0x6a0
			 0xc4f9e78d, /// 0x6a4
			 0xdedd68ff, /// 0x6a8
			 0xd597fd38, /// 0x6ac
			 0x1931713d, /// 0x6b0
			 0x314f6cfc, /// 0x6b4
			 0xcf979259, /// 0x6b8
			 0xad661fee, /// 0x6bc
			 0x5f9a6812, /// 0x6c0
			 0x5b1b8070, /// 0x6c4
			 0x6ffee52e, /// 0x6c8
			 0xb7eb982c, /// 0x6cc
			 0x2b20cf7a, /// 0x6d0
			 0xbba32a2c, /// 0x6d4
			 0x05f05cf5, /// 0x6d8
			 0x45683582, /// 0x6dc
			 0x1fa54e66, /// 0x6e0
			 0xe28e945a, /// 0x6e4
			 0xc3c71ada, /// 0x6e8
			 0xf9ef53c1, /// 0x6ec
			 0xa3a46acc, /// 0x6f0
			 0xd37bb0f3, /// 0x6f4
			 0xcfd1eb24, /// 0x6f8
			 0x96fcc713, /// 0x6fc
			 0xc4d1bd30, /// 0x700
			 0xeec7fe0e, /// 0x704
			 0xf3d720fc, /// 0x708
			 0x385f3bf3, /// 0x70c
			 0x3a0f5113, /// 0x710
			 0x7b451acf, /// 0x714
			 0xb165b465, /// 0x718
			 0x2f984bfe, /// 0x71c
			 0x5a025902, /// 0x720
			 0xc49a2872, /// 0x724
			 0x27d5e0a9, /// 0x728
			 0x5d9b289a, /// 0x72c
			 0x47742020, /// 0x730
			 0x0927b3c8, /// 0x734
			 0x044eb305, /// 0x738
			 0xc9247ced, /// 0x73c
			 0x9ce204d1, /// 0x740
			 0x8ef05f88, /// 0x744
			 0xc80c22a5, /// 0x748
			 0x5428ef93, /// 0x74c
			 0xaf07b5f9, /// 0x750
			 0xc59cb8f8, /// 0x754
			 0xe72775a4, /// 0x758
			 0x1f06ab96, /// 0x75c
			 0x16cf2968, /// 0x760
			 0x1765cb3a, /// 0x764
			 0x3ab4767e, /// 0x768
			 0xa5e4d766, /// 0x76c
			 0x860ffe5f, /// 0x770
			 0x7dfe79c4, /// 0x774
			 0x783c6c67, /// 0x778
			 0x4d8e5781, /// 0x77c
			 0xa3c11112, /// 0x780
			 0x852faa4b, /// 0x784
			 0x38cc7222, /// 0x788
			 0x9846274b, /// 0x78c
			 0x73539ac0, /// 0x790
			 0xe906bf2a, /// 0x794
			 0xc2bd00bf, /// 0x798
			 0xe6f3c09b, /// 0x79c
			 0x18d248bc, /// 0x7a0
			 0x3c3c18cd, /// 0x7a4
			 0xca606559, /// 0x7a8
			 0x3e1e45b7, /// 0x7ac
			 0x7d698992, /// 0x7b0
			 0x8ad62361, /// 0x7b4
			 0xa3c300e7, /// 0x7b8
			 0xc9a523b3, /// 0x7bc
			 0x15da9a3c, /// 0x7c0
			 0xc4f37da2, /// 0x7c4
			 0x88db08e5, /// 0x7c8
			 0x2cb5b79b, /// 0x7cc
			 0xf9282bd9, /// 0x7d0
			 0x61b878a5, /// 0x7d4
			 0x9b8b3fc1, /// 0x7d8
			 0x4a03f3cf, /// 0x7dc
			 0x51247266, /// 0x7e0
			 0xe7faf7fc, /// 0x7e4
			 0xeb5ba7db, /// 0x7e8
			 0xb624a7b1, /// 0x7ec
			 0xd2a64fee, /// 0x7f0
			 0x49883399, /// 0x7f4
			 0x07304ac4, /// 0x7f8
			 0xdb1ea6e8, /// 0x7fc
			 0xed8e7e3b, /// 0x800
			 0xfada8940, /// 0x804
			 0xb5d8d28c, /// 0x808
			 0x9d1fe821, /// 0x80c
			 0x81524fe4, /// 0x810
			 0x748a60e7, /// 0x814
			 0xa5fc2e45, /// 0x818
			 0x69ad19ed, /// 0x81c
			 0x233b2622, /// 0x820
			 0xd12b6d7c, /// 0x824
			 0x85d177ef, /// 0x828
			 0xef694b22, /// 0x82c
			 0x4291c96c, /// 0x830
			 0x9dad3bb0, /// 0x834
			 0x84c0222f, /// 0x838
			 0x7e5cdd18, /// 0x83c
			 0xe07bfe13, /// 0x840
			 0xecaafc08, /// 0x844
			 0x6685a89f, /// 0x848
			 0xea1aaed3, /// 0x84c
			 0xd53143cc, /// 0x850
			 0x77a3a3d7, /// 0x854
			 0xa605ba01, /// 0x858
			 0x0b8f6ff4, /// 0x85c
			 0x05583257, /// 0x860
			 0x130d39f9, /// 0x864
			 0xf3149863, /// 0x868
			 0xf45d314f, /// 0x86c
			 0xa3684eea, /// 0x870
			 0xec9c498b, /// 0x874
			 0x06738861, /// 0x878
			 0xa4022ec4, /// 0x87c
			 0x581051e5, /// 0x880
			 0xaf01e91d, /// 0x884
			 0x17e985b1, /// 0x888
			 0xda6d3fa4, /// 0x88c
			 0xb83540d5, /// 0x890
			 0x0076a227, /// 0x894
			 0x3cda839c, /// 0x898
			 0xbae1bbc8, /// 0x89c
			 0xa4cc88d1, /// 0x8a0
			 0x598b18a4, /// 0x8a4
			 0x3d5c0e5d, /// 0x8a8
			 0x0be24a96, /// 0x8ac
			 0x8c715e33, /// 0x8b0
			 0xa91fb1ef, /// 0x8b4
			 0x6b556046, /// 0x8b8
			 0xa31b0e8f, /// 0x8bc
			 0xf33fac77, /// 0x8c0
			 0xeb1e4a85, /// 0x8c4
			 0xa7e22951, /// 0x8c8
			 0xbd4903e4, /// 0x8cc
			 0xe241ca48, /// 0x8d0
			 0xa447b967, /// 0x8d4
			 0xd3268ca1, /// 0x8d8
			 0x08cf2790, /// 0x8dc
			 0x913b7182, /// 0x8e0
			 0xc1a4fde0, /// 0x8e4
			 0xfb7b8e60, /// 0x8e8
			 0xb57e2311, /// 0x8ec
			 0xb07c6de3, /// 0x8f0
			 0xaf8df316, /// 0x8f4
			 0xdf27b03c, /// 0x8f8
			 0x0e0ca301, /// 0x8fc
			 0x0a2cf039, /// 0x900
			 0x0c8d6aae, /// 0x904
			 0xd80cbe15, /// 0x908
			 0xb591c86f, /// 0x90c
			 0x79b1a24a, /// 0x910
			 0x90ae23fa, /// 0x914
			 0xe6cd2110, /// 0x918
			 0x0c276db7, /// 0x91c
			 0x6a694c72, /// 0x920
			 0xb6099dc7, /// 0x924
			 0xb7e6b9ed, /// 0x928
			 0x094a05f5, /// 0x92c
			 0x6b69ea43, /// 0x930
			 0x6d96d64d, /// 0x934
			 0x4e3dc1e4, /// 0x938
			 0x511b69fd, /// 0x93c
			 0xdece0e57, /// 0x940
			 0x1c4faf92, /// 0x944
			 0x6ebb5652, /// 0x948
			 0x46d7d0ba, /// 0x94c
			 0xbf9e24e1, /// 0x950
			 0x5043158f, /// 0x954
			 0x0134dd74, /// 0x958
			 0x6508317f, /// 0x95c
			 0x92194597, /// 0x960
			 0xaa3b821e, /// 0x964
			 0x9be8f8ad, /// 0x968
			 0xa5e8ca52, /// 0x96c
			 0x7ffbab52, /// 0x970
			 0xd19533cb, /// 0x974
			 0xed81531f, /// 0x978
			 0x6c019a2c, /// 0x97c
			 0x81e96fa9, /// 0x980
			 0x39bfc798, /// 0x984
			 0xc92aaa38, /// 0x988
			 0x7ad02163, /// 0x98c
			 0x6596e283, /// 0x990
			 0x8cefea12, /// 0x994
			 0x82314dcf, /// 0x998
			 0x5d4f7a17, /// 0x99c
			 0x26d315c6, /// 0x9a0
			 0xe868e39e, /// 0x9a4
			 0x9f19160f, /// 0x9a8
			 0x1e88149c, /// 0x9ac
			 0x8da7ac1a, /// 0x9b0
			 0xb03d6c3c, /// 0x9b4
			 0xe249af45, /// 0x9b8
			 0xa3d86bc1, /// 0x9bc
			 0x55d78240, /// 0x9c0
			 0xd0df3e02, /// 0x9c4
			 0x43891726, /// 0x9c8
			 0xe9244e66, /// 0x9cc
			 0x516e2914, /// 0x9d0
			 0x4a0385ee, /// 0x9d4
			 0xe09ded82, /// 0x9d8
			 0x2900c718, /// 0x9dc
			 0x1d1ed63f, /// 0x9e0
			 0x2399a370, /// 0x9e4
			 0x69463eae, /// 0x9e8
			 0x18054552, /// 0x9ec
			 0xe786b106, /// 0x9f0
			 0xa363c7c5, /// 0x9f4
			 0x4bc75834, /// 0x9f8
			 0x7a5ceec4, /// 0x9fc
			 0x29da5b94, /// 0xa00
			 0x77547804, /// 0xa04
			 0x40573f40, /// 0xa08
			 0x8353c77f, /// 0xa0c
			 0x5029b83e, /// 0xa10
			 0xa8c7ec93, /// 0xa14
			 0xccaff2a8, /// 0xa18
			 0xec110e36, /// 0xa1c
			 0x98ae64d3, /// 0xa20
			 0xd9383597, /// 0xa24
			 0xd2c66db8, /// 0xa28
			 0x379320b0, /// 0xa2c
			 0x7058e6a4, /// 0xa30
			 0x08e7255e, /// 0xa34
			 0x761d31c3, /// 0xa38
			 0x73fb8d00, /// 0xa3c
			 0xa667d100, /// 0xa40
			 0x4749dbad, /// 0xa44
			 0xac807503, /// 0xa48
			 0x7a3e1f18, /// 0xa4c
			 0x9a205995, /// 0xa50
			 0xdfdd7b75, /// 0xa54
			 0x955db51e, /// 0xa58
			 0xde25846a, /// 0xa5c
			 0xc263bc31, /// 0xa60
			 0xe1f58677, /// 0xa64
			 0x7ab8c45b, /// 0xa68
			 0x334ada46, /// 0xa6c
			 0x3ddf69b9, /// 0xa70
			 0x68e0f6b8, /// 0xa74
			 0xe15ab2f8, /// 0xa78
			 0xdafa4fbc, /// 0xa7c
			 0x7043a5db, /// 0xa80
			 0x7bede78b, /// 0xa84
			 0x318ffa1b, /// 0xa88
			 0xb908388b, /// 0xa8c
			 0xacb2771f, /// 0xa90
			 0xa80c545d, /// 0xa94
			 0x917def09, /// 0xa98
			 0x447d2283, /// 0xa9c
			 0xd3d4a3e7, /// 0xaa0
			 0x17a1b71d, /// 0xaa4
			 0x03a77b38, /// 0xaa8
			 0x4e7981e7, /// 0xaac
			 0x1b020fe5, /// 0xab0
			 0x250ef5f8, /// 0xab4
			 0xa735d510, /// 0xab8
			 0x67117677, /// 0xabc
			 0x590278c0, /// 0xac0
			 0xa1e96244, /// 0xac4
			 0xe20a490d, /// 0xac8
			 0xa2f3b85b, /// 0xacc
			 0x08f79986, /// 0xad0
			 0x1223562b, /// 0xad4
			 0xf9b146d2, /// 0xad8
			 0x970daa4b, /// 0xadc
			 0xef4ce565, /// 0xae0
			 0xece17c69, /// 0xae4
			 0x260cbaad, /// 0xae8
			 0xd58b8148, /// 0xaec
			 0x116e6df7, /// 0xaf0
			 0x9c4c0c75, /// 0xaf4
			 0x3ecc026e, /// 0xaf8
			 0x272b6e37, /// 0xafc
			 0x47eba07b, /// 0xb00
			 0xae0c29f9, /// 0xb04
			 0xe3e1e1c0, /// 0xb08
			 0xa04f739a, /// 0xb0c
			 0xfc600d9a, /// 0xb10
			 0x068ab1d3, /// 0xb14
			 0x920a9599, /// 0xb18
			 0xb6824810, /// 0xb1c
			 0xa048aaeb, /// 0xb20
			 0x59b4d954, /// 0xb24
			 0x3ad06725, /// 0xb28
			 0xce470796, /// 0xb2c
			 0x528fab77, /// 0xb30
			 0x84b6096a, /// 0xb34
			 0xe7e77920, /// 0xb38
			 0xf5c71b9b, /// 0xb3c
			 0xdc74b5c0, /// 0xb40
			 0xaa2f48cb, /// 0xb44
			 0x500ffcf7, /// 0xb48
			 0xf010a171, /// 0xb4c
			 0xc78ffee1, /// 0xb50
			 0x1ae4320b, /// 0xb54
			 0x2df9a4a1, /// 0xb58
			 0xfaa03061, /// 0xb5c
			 0xb2500152, /// 0xb60
			 0x8d228603, /// 0xb64
			 0xcd3fc760, /// 0xb68
			 0x15bd1726, /// 0xb6c
			 0xb9b88074, /// 0xb70
			 0x966c6da7, /// 0xb74
			 0x59577f7b, /// 0xb78
			 0x9df77e14, /// 0xb7c
			 0x02b03942, /// 0xb80
			 0x8ad0a495, /// 0xb84
			 0xb7bfa0f9, /// 0xb88
			 0xc7395c8b, /// 0xb8c
			 0x2be83846, /// 0xb90
			 0xda132877, /// 0xb94
			 0xa6e1fcdc, /// 0xb98
			 0x1ccd0756, /// 0xb9c
			 0x5e4dd019, /// 0xba0
			 0xe89ba753, /// 0xba4
			 0x7da5c8ee, /// 0xba8
			 0xab80bd23, /// 0xbac
			 0xe70ac151, /// 0xbb0
			 0xcad2cefe, /// 0xbb4
			 0xda03c395, /// 0xbb8
			 0x817b305d, /// 0xbbc
			 0x13385509, /// 0xbc0
			 0xa1ca1257, /// 0xbc4
			 0xd14c9efb, /// 0xbc8
			 0x1e675688, /// 0xbcc
			 0x9dbf8c41, /// 0xbd0
			 0xe02f454a, /// 0xbd4
			 0x5ff226b0, /// 0xbd8
			 0xcc86dbf8, /// 0xbdc
			 0x9b277b5f, /// 0xbe0
			 0xa7aa04b5, /// 0xbe4
			 0x5ee36b98, /// 0xbe8
			 0x1d7efc38, /// 0xbec
			 0xdfd877e8, /// 0xbf0
			 0x33762284, /// 0xbf4
			 0x94215b77, /// 0xbf8
			 0xe9e4281d, /// 0xbfc
			 0xbae197da, /// 0xc00
			 0x8d92d84f, /// 0xc04
			 0x078e9b9a, /// 0xc08
			 0xb1046e99, /// 0xc0c
			 0xfadb9491, /// 0xc10
			 0xa74f82b1, /// 0xc14
			 0x1af60743, /// 0xc18
			 0x54b5d4c6, /// 0xc1c
			 0x488120b0, /// 0xc20
			 0xe6245fc5, /// 0xc24
			 0x97050f6e, /// 0xc28
			 0xabd80ff6, /// 0xc2c
			 0x94ca5ff4, /// 0xc30
			 0x4c71c993, /// 0xc34
			 0x3c4a9c88, /// 0xc38
			 0x54dbdd6d, /// 0xc3c
			 0x3a29e1a7, /// 0xc40
			 0x8ffb2d17, /// 0xc44
			 0x6b129d71, /// 0xc48
			 0xb6adea01, /// 0xc4c
			 0x3e5a5150, /// 0xc50
			 0x24d944fa, /// 0xc54
			 0x52e9988f, /// 0xc58
			 0x700d8fef, /// 0xc5c
			 0x36271232, /// 0xc60
			 0x079289bf, /// 0xc64
			 0xa388a3cb, /// 0xc68
			 0x4a36f123, /// 0xc6c
			 0x73993e4a, /// 0xc70
			 0xd8369e33, /// 0xc74
			 0x8897931f, /// 0xc78
			 0xcabdf49b, /// 0xc7c
			 0x800de2d7, /// 0xc80
			 0xd8bc631a, /// 0xc84
			 0xc0431c28, /// 0xc88
			 0x4fd4cd67, /// 0xc8c
			 0x93633818, /// 0xc90
			 0xd5eaaa07, /// 0xc94
			 0xc8ca673f, /// 0xc98
			 0x97e82540, /// 0xc9c
			 0x62857122, /// 0xca0
			 0xb9d8487e, /// 0xca4
			 0x671efec3, /// 0xca8
			 0x9ff8d677, /// 0xcac
			 0x06f8de58, /// 0xcb0
			 0x0b2e4f8a, /// 0xcb4
			 0xfdb024f1, /// 0xcb8
			 0xe22ab11e, /// 0xcbc
			 0xf76f7a9e, /// 0xcc0
			 0x427999ba, /// 0xcc4
			 0x87c2748d, /// 0xcc8
			 0x1481d0eb, /// 0xccc
			 0xd9fb9d90, /// 0xcd0
			 0x6362b33f, /// 0xcd4
			 0xa41a1a32, /// 0xcd8
			 0xa4f0d640, /// 0xcdc
			 0xe06c87bd, /// 0xce0
			 0x989270c9, /// 0xce4
			 0x867da856, /// 0xce8
			 0xe1fcbe74, /// 0xcec
			 0x58b38009, /// 0xcf0
			 0xaddfe74d, /// 0xcf4
			 0xca465df0, /// 0xcf8
			 0x21691bfc, /// 0xcfc
			 0x88dcaeed, /// 0xd00
			 0xf7167fd7, /// 0xd04
			 0xd989ca8f, /// 0xd08
			 0xf1b80418, /// 0xd0c
			 0x755f5052, /// 0xd10
			 0xf58db467, /// 0xd14
			 0xcb3082bb, /// 0xd18
			 0xaa372b67, /// 0xd1c
			 0xff942efa, /// 0xd20
			 0x4a6d1ac1, /// 0xd24
			 0x6bf49bf2, /// 0xd28
			 0x5dbd0726, /// 0xd2c
			 0xf2923773, /// 0xd30
			 0xece42dcf, /// 0xd34
			 0x76d00dcb, /// 0xd38
			 0x18585178, /// 0xd3c
			 0xa959b418, /// 0xd40
			 0xc70aec2f, /// 0xd44
			 0xd9a50d4e, /// 0xd48
			 0x1b83b7ec, /// 0xd4c
			 0x964c5fca, /// 0xd50
			 0x9f1474cb, /// 0xd54
			 0xa9637c4d, /// 0xd58
			 0xb2cddf93, /// 0xd5c
			 0xc6a50416, /// 0xd60
			 0xf341aa47, /// 0xd64
			 0xd59467d4, /// 0xd68
			 0xec1be4bf, /// 0xd6c
			 0x7f5e6bf1, /// 0xd70
			 0xfc6f418d, /// 0xd74
			 0x4b8b9018, /// 0xd78
			 0xd8a99330, /// 0xd7c
			 0xef938eac, /// 0xd80
			 0x66b3bac4, /// 0xd84
			 0x35e52820, /// 0xd88
			 0x4f97c6d7, /// 0xd8c
			 0x67c8493c, /// 0xd90
			 0x41480f21, /// 0xd94
			 0x2f3d3c33, /// 0xd98
			 0xd21c63ff, /// 0xd9c
			 0xd951d2c0, /// 0xda0
			 0xbee901db, /// 0xda4
			 0x1973d4e0, /// 0xda8
			 0xc90e50af, /// 0xdac
			 0xc745b359, /// 0xdb0
			 0x007f689a, /// 0xdb4
			 0x252a028f, /// 0xdb8
			 0xb614d04e, /// 0xdbc
			 0x94efade5, /// 0xdc0
			 0xac1cb084, /// 0xdc4
			 0x768f5303, /// 0xdc8
			 0xe8bc6456, /// 0xdcc
			 0x628d6ce0, /// 0xdd0
			 0x6e34e8f0, /// 0xdd4
			 0x88f7c3b1, /// 0xdd8
			 0xe1ed7d01, /// 0xddc
			 0x45d246b8, /// 0xde0
			 0x9bd424ba, /// 0xde4
			 0x468a5473, /// 0xde8
			 0x39506d01, /// 0xdec
			 0x7f9747dc, /// 0xdf0
			 0x8a5b56c7, /// 0xdf4
			 0x0a702985, /// 0xdf8
			 0xa8dafdc6, /// 0xdfc
			 0x6a4233f1, /// 0xe00
			 0xab1cdc38, /// 0xe04
			 0x5ff8453b, /// 0xe08
			 0x43179514, /// 0xe0c
			 0x61c1a969, /// 0xe10
			 0x3f8141b3, /// 0xe14
			 0x51bd495d, /// 0xe18
			 0xc71eda62, /// 0xe1c
			 0xd3db01b0, /// 0xe20
			 0x3bf34d51, /// 0xe24
			 0xfe1467cb, /// 0xe28
			 0xb81b5df3, /// 0xe2c
			 0x929daa55, /// 0xe30
			 0xe6328966, /// 0xe34
			 0xf4dfeac7, /// 0xe38
			 0xdef463ea, /// 0xe3c
			 0xd73311eb, /// 0xe40
			 0x80cd61bd, /// 0xe44
			 0xa5df6da0, /// 0xe48
			 0xca60be2f, /// 0xe4c
			 0x2b40c23b, /// 0xe50
			 0x14894bce, /// 0xe54
			 0xcec3a1f4, /// 0xe58
			 0x5354a6f2, /// 0xe5c
			 0x8f5a7d55, /// 0xe60
			 0x8d8b303d, /// 0xe64
			 0x2e0083d7, /// 0xe68
			 0x9168f7a9, /// 0xe6c
			 0x3065aed9, /// 0xe70
			 0x197e717d, /// 0xe74
			 0xc86ea1a8, /// 0xe78
			 0x8d99be30, /// 0xe7c
			 0x5eb34f11, /// 0xe80
			 0x73afcdbb, /// 0xe84
			 0xb65856be, /// 0xe88
			 0x460cc020, /// 0xe8c
			 0x083cbe9d, /// 0xe90
			 0x9e8dd3d9, /// 0xe94
			 0x2d3b83d5, /// 0xe98
			 0x6ceb16ce, /// 0xe9c
			 0x4678d291, /// 0xea0
			 0x0c142c74, /// 0xea4
			 0x206c6df6, /// 0xea8
			 0x22e3ee5a, /// 0xeac
			 0xeb216dd5, /// 0xeb0
			 0x13457735, /// 0xeb4
			 0x2fe9858b, /// 0xeb8
			 0x7879376e, /// 0xebc
			 0x28358b02, /// 0xec0
			 0x5bb86bee, /// 0xec4
			 0x29e52f14, /// 0xec8
			 0x420ff319, /// 0xecc
			 0x9ed0814e, /// 0xed0
			 0x3fa6724f, /// 0xed4
			 0x3d0cdfb3, /// 0xed8
			 0xa91d3899, /// 0xedc
			 0xb4be6cea, /// 0xee0
			 0x712ca653, /// 0xee4
			 0xd0c11ae6, /// 0xee8
			 0xb580e77a, /// 0xeec
			 0xc55d64fa, /// 0xef0
			 0xa7db8a2e, /// 0xef4
			 0x060bfb68, /// 0xef8
			 0x3e44a03d, /// 0xefc
			 0xb081258d, /// 0xf00
			 0x03d65c26, /// 0xf04
			 0x8ca1341e, /// 0xf08
			 0x95949ef7, /// 0xf0c
			 0xcbf6c972, /// 0xf10
			 0xddf39130, /// 0xf14
			 0xaeb84969, /// 0xf18
			 0x48dc1846, /// 0xf1c
			 0x486af8d1, /// 0xf20
			 0x80646266, /// 0xf24
			 0xcfa7273b, /// 0xf28
			 0xdb521832, /// 0xf2c
			 0x28db4215, /// 0xf30
			 0x1db2952b, /// 0xf34
			 0x50f769c1, /// 0xf38
			 0xa7a93ae8, /// 0xf3c
			 0xe32b5bf5, /// 0xf40
			 0xdf12d20b, /// 0xf44
			 0xd5efdef9, /// 0xf48
			 0x00158c44, /// 0xf4c
			 0x1ab2782e, /// 0xf50
			 0xc3be0b43, /// 0xf54
			 0x6a6b844c, /// 0xf58
			 0xd3b3031d, /// 0xf5c
			 0xb3468309, /// 0xf60
			 0x7d673129, /// 0xf64
			 0x514d11f0, /// 0xf68
			 0xa94ddc03, /// 0xf6c
			 0xa4c57684, /// 0xf70
			 0x9466911b, /// 0xf74
			 0x2cf54336, /// 0xf78
			 0xff91fb30, /// 0xf7c
			 0x26ffaef8, /// 0xf80
			 0xbe2564c3, /// 0xf84
			 0x567f08ef, /// 0xf88
			 0x57a63b4c, /// 0xf8c
			 0xbf227a5e, /// 0xf90
			 0xac50753b, /// 0xf94
			 0xfa37397d, /// 0xf98
			 0x52cd1e1b, /// 0xf9c
			 0x74cc7452, /// 0xfa0
			 0x6f7dec27, /// 0xfa4
			 0x80137e62, /// 0xfa8
			 0xe0e5d069, /// 0xfac
			 0x35deead3, /// 0xfb0
			 0x480c5375, /// 0xfb4
			 0x54b5b240, /// 0xfb8
			 0x1355699c, /// 0xfbc
			 0x7d6d31f4, /// 0xfc0
			 0x4f179196, /// 0xfc4
			 0x7ad4ad0d, /// 0xfc8
			 0x7481d143, /// 0xfcc
			 0xfcaf09e2, /// 0xfd0
			 0x4d4d912d, /// 0xfd4
			 0x1046b697, /// 0xfd8
			 0x2149c88b, /// 0xfdc
			 0x1294d62e, /// 0xfe0
			 0xf559ba37, /// 0xfe4
			 0xfa3b1132, /// 0xfe8
			 0x49f3ea30, /// 0xfec
			 0x05721201, /// 0xff0
			 0xb65b002f, /// 0xff4
			 0x31da3019, /// 0xff8
			 0x6dc5bb5a /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00000
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00008
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0000c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00010
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00014
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00018
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0001c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00020
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00024
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00028
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0002c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00030
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x0e000003,                                                  // Trailing 1s:                                /// 00038
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0003c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00044
			 0x55555555,                                                  // 4 random values                             /// 00048
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0004c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00050
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00054
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0xffc00001,                                                  // -signaling NaN                              /// 0005c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00060
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00064
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00068
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0006c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00074
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00078
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0xbf028f5c,                                                  // -0.51                                       /// 00084
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00088
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0008c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00090
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x3f028f5c,                                                  // 0.51                                        /// 00098
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0009c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000a8
			 0x33333333,                                                  // 4 random values                             /// 000ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 000e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 000e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00104
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00108
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0010c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00118
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0011c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00120
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00124
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00128
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0012c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00134
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00138
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0013c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00140
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00148
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0014c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00150
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00154
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0015c
			 0x7fc00001,                                                  // signaling NaN                               /// 00160
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00170
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00174
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00178
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00180
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00184
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0018c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00198
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001c8
			 0x00000000,                                                  // zero                                        /// 001cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001dc
			 0x80000000,                                                  // -zero                                       /// 001e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0xff800000,                                                  // -inf                                        /// 001f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00200
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00204
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00208
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0020c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00218
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0021c
			 0x00000000,                                                  // zero                                        /// 00220
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x00000000,                                                  // zero                                        /// 00228
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0022c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00238
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00240
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00244
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0024c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x80000000,                                                  // -zero                                       /// 00254
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0025c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00260
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00264
			 0x33333333,                                                  // 4 random values                             /// 00268
			 0x7fc00001,                                                  // signaling NaN                               /// 0026c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0027c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00280
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00284
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00288
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0028c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00290
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 002bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002c0
			 0x33333333,                                                  // 4 random values                             /// 002c4
			 0x55555555,                                                  // 4 random values                             /// 002c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002e8
			 0x55555555,                                                  // 4 random values                             /// 002ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00300
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00304
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00308
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00310
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00314
			 0x80000000,                                                  // -zero                                       /// 00318
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0xff800000,                                                  // -inf                                        /// 00320
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00324
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00328
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00338
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0033c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00344
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00348
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0034c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00350
			 0x80011111,                                                  // -9.7958E-41                                 /// 00354
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00358
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0035c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00360
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00364
			 0x55555555,                                                  // 4 random values                             /// 00368
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00378
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0037c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00380
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0038c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00390
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00394
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00398
			 0x80000000,                                                  // -zero                                       /// 0039c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003cc
			 0x33333333,                                                  // 4 random values                             /// 003d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003dc
			 0x00000000,                                                  // zero                                        /// 003e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 003e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003f8
			 0x7f800000,                                                  // inf                                         /// 003fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00404
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0040c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00410
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0041c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0042c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00444
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00448
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0044c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00450
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00454
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00464
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0046c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00470
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00478
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0047c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00484
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x80011111,                                                  // -9.7958E-41                                 /// 0048c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x00011111,                                                  // 9.7958E-41                                  /// 00494
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0049c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004ac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x3f028f5c,                                                  // 0.51                                        /// 004b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 004e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004f8
			 0x7f800000,                                                  // inf                                         /// 004fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00500
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00504
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x80000000,                                                  // -zero                                       /// 0050c
			 0xbf028f5c,                                                  // -0.51                                       /// 00510
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00514
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00518
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0051c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00520
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00524
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00528
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0052c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00538
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0053c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00540
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00548
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0054c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00550
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00554
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00558
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00560
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00568
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0056c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00570
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00574
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00578
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0057c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00580
			 0xffc00001,                                                  // -signaling NaN                              /// 00584
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00588
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00590
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00598
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005dc
			 0x7fc00001,                                                  // signaling NaN                               /// 005e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00600
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00614
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00618
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0061c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00620
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00628
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0062c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00630
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00634
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00638
			 0x55555555,                                                  // 4 random values                             /// 0063c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0064c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00650
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00654
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0065c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00660
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00664
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00668
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0066c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00674
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0067c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00680
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00684
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00688
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0068c
			 0x7fc00001,                                                  // signaling NaN                               /// 00690
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00694
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00698
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 006b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x33333333,                                                  // 4 random values                             /// 006ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006f0
			 0x7f800000,                                                  // inf                                         /// 006f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00704
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0070c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00710
			 0x7f800000,                                                  // inf                                         /// 00714
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00724
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00728
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0072c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00734
			 0x0c600000,                                                  // Leading 1s:                                 /// 00738
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00740
			 0x0c700000,                                                  // Leading 1s:                                 /// 00744
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x3f028f5c,                                                  // 0.51                                        /// 0074c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00758
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0075c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00760
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00764
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00768
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0076c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00770
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00774
			 0xbf028f5c,                                                  // -0.51                                       /// 00778
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0077c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00780
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00784
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00788
			 0x0c600000,                                                  // Leading 1s:                                 /// 0078c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00790
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00794
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007a0
			 0x00000000,                                                  // zero                                        /// 007a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007ac
			 0x7fc00001,                                                  // signaling NaN                               /// 007b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007d8
			 0xcb000000,                                                  // -8388608.0                                  /// 007dc
			 0xcb000000,                                                  // -8388608.0                                  /// 007e0
			 0xcb000000,                                                  // -8388608.0                                  /// 007e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00800
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00808
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0080c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00810
			 0x0c600000,                                                  // Leading 1s:                                 /// 00814
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00818
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00820
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00824
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00828
			 0xbf028f5c,                                                  // -0.51                                       /// 0082c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0e000003,                                                  // Trailing 1s:                                /// 0083c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00840
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00844
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00850
			 0x7f800000,                                                  // inf                                         /// 00854
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0085c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00864
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00868
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0086c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00870
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00874
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00878
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0087c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00888
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0xff800000,                                                  // -inf                                        /// 00890
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00894
			 0x0c700000,                                                  // Leading 1s:                                 /// 00898
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0089c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x33333333,                                                  // 4 random values                             /// 008ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 008bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008f4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00904
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00908
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0090c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00910
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0091c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00920
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00924
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00928
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0092c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x7fc00001,                                                  // signaling NaN                               /// 00934
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00938
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0093c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00940
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00944
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00948
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0094c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00950
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00954
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00958
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0095c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00960
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00964
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00978
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00980
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00984
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00988
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0098c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00990
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0xcb000000,                                                  // -8388608.0                                  /// 009b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009e0
			 0x0c600000,                                                  // Leading 1s:                                 /// 009e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a08
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a1c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a24
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a38
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a40
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a48
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a50
			 0xffc00001,                                                  // -signaling NaN                              /// 00a54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a60
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a6c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a7c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a8c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a94
			 0xcb000000,                                                  // -8388608.0                                  /// 00a98
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x33333333,                                                  // 4 random values                             /// 00aa0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00aa8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ab0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ab4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ac0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ac4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ad8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00adc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00af0
			 0xffc00001,                                                  // -signaling NaN                              /// 00af4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0xbf028f5c,                                                  // -0.51                                       /// 00b04
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b08
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b10
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b34
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b38
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b48
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b60
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b64
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x55555555,                                                  // 4 random values                             /// 00b70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b80
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b88
			 0x3f028f5c,                                                  // 0.51                                        /// 00b8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b90
			 0xff800000,                                                  // -inf                                        /// 00b94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b98
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b9c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ba0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bc4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bc8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bcc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bd0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bd8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bdc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00be0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00be8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bec
			 0x7f800000,                                                  // inf                                         /// 00bf0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bf4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bfc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c04
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c1c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c20
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c24
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c2c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c50
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c58
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c70
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c74
			 0x80000000,                                                  // -zero                                       /// 00c78
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c7c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c84
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c90
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0xcb000000,                                                  // -8388608.0                                  /// 00c98
			 0x4b000000,                                                  // 8388608.0                                   /// 00c9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0xff800000,                                                  // -inf                                        /// 00cac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cb8
			 0x33333333,                                                  // 4 random values                             /// 00cbc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cc0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cc4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x33333333,                                                  // 4 random values                             /// 00ccc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cd0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cd4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cd8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cdc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ce0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ce4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cf0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cf4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cf8
			 0x33333333,                                                  // 4 random values                             /// 00cfc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d04
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d08
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d10
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d20
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d30
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d38
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d54
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d58
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d64
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0xcb000000,                                                  // -8388608.0                                  /// 00d6c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d70
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d74
			 0x55555555,                                                  // 4 random values                             /// 00d78
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d88
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d90
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d9c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00da0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00da4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00da8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00db0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00db8
			 0xff800000,                                                  // -inf                                        /// 00dbc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dc8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dd8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ddc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00de4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00de8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00df0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00df4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dfc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0xbf028f5c,                                                  // -0.51                                       /// 00e04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0xcb000000,                                                  // -8388608.0                                  /// 00e0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e10
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e14
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e24
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e2c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e40
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e44
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e50
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x33333333,                                                  // 4 random values                             /// 00e5c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e60
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e64
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e68
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e80
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e84
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x7fc00001,                                                  // signaling NaN                               /// 00e8c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e98
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ea0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ea4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ea8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eb4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00eb8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ebc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ec0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ec8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ecc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ed0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ed4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ed8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00edc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ee0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ee8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f08
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f10
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f18
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f1c
			 0x55555555,                                                  // 4 random values                             /// 00f20
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f3c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f40
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0xcb000000,                                                  // -8388608.0                                  /// 00f48
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f4c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f6c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f70
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f80
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x4b000000,                                                  // 8388608.0                                   /// 00f8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f94
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f98
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fa4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fa8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fb0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fbc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fcc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fd0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fd4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fd8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fdc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fe0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fe4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fe8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ff0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ff4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ff8
			 0x80800001 // min norm + 1ulp                               // subnormals -ve                              /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0636a08a, /// 0x0
			 0x283fa535, /// 0x4
			 0x3ce88aaf, /// 0x8
			 0x283c0067, /// 0xc
			 0x01ff9a6f, /// 0x10
			 0x57299130, /// 0x14
			 0xc892b5d5, /// 0x18
			 0xb2f2bca1, /// 0x1c
			 0x55360682, /// 0x20
			 0xe6a16c85, /// 0x24
			 0x53692908, /// 0x28
			 0x3a867e0d, /// 0x2c
			 0x24f65930, /// 0x30
			 0x26ac7b9b, /// 0x34
			 0xf8bef7e0, /// 0x38
			 0x00424c80, /// 0x3c
			 0xc908711b, /// 0x40
			 0x825db0e3, /// 0x44
			 0xab3dd9a7, /// 0x48
			 0x296aa242, /// 0x4c
			 0xc4179c5b, /// 0x50
			 0x8a3b2575, /// 0x54
			 0xe54bfb1b, /// 0x58
			 0xcdb2f5fd, /// 0x5c
			 0xf89277ff, /// 0x60
			 0x5bc2a7a7, /// 0x64
			 0x3e8b5353, /// 0x68
			 0x9e3c072b, /// 0x6c
			 0x2d008be2, /// 0x70
			 0x9004d8fe, /// 0x74
			 0x405f401f, /// 0x78
			 0x54e12044, /// 0x7c
			 0x242891f2, /// 0x80
			 0x49f2253b, /// 0x84
			 0x36f5de62, /// 0x88
			 0x03977acc, /// 0x8c
			 0xd812ba34, /// 0x90
			 0x77eb6b14, /// 0x94
			 0x004ae154, /// 0x98
			 0x4a24dcd1, /// 0x9c
			 0xc9f53089, /// 0xa0
			 0x7d05aff6, /// 0xa4
			 0x443ca435, /// 0xa8
			 0xd2a59064, /// 0xac
			 0x41d3eddb, /// 0xb0
			 0x9c66952f, /// 0xb4
			 0x3aa6b29a, /// 0xb8
			 0x24911aad, /// 0xbc
			 0x61e749b5, /// 0xc0
			 0x967201f2, /// 0xc4
			 0xd7fbd8d9, /// 0xc8
			 0x6c65cb90, /// 0xcc
			 0x33d34adc, /// 0xd0
			 0xaa89cc76, /// 0xd4
			 0x9a5a61d3, /// 0xd8
			 0x2c3ff8b0, /// 0xdc
			 0xf949baa1, /// 0xe0
			 0x1181dc4f, /// 0xe4
			 0xe74555f9, /// 0xe8
			 0xa993543e, /// 0xec
			 0xfac520ca, /// 0xf0
			 0x59c12249, /// 0xf4
			 0xf200201f, /// 0xf8
			 0x84deb05a, /// 0xfc
			 0xf5b5fafb, /// 0x100
			 0x5a2a56d6, /// 0x104
			 0x0daadb0d, /// 0x108
			 0xb391f149, /// 0x10c
			 0x765f281f, /// 0x110
			 0x77e6fc69, /// 0x114
			 0x94fd74c0, /// 0x118
			 0x1fd731bc, /// 0x11c
			 0xdb9910fd, /// 0x120
			 0x0ba65459, /// 0x124
			 0x95be09cc, /// 0x128
			 0x1414ce75, /// 0x12c
			 0x9b0762aa, /// 0x130
			 0x156b3a77, /// 0x134
			 0xd5776dcf, /// 0x138
			 0x7bae72de, /// 0x13c
			 0xdda1f15e, /// 0x140
			 0x64649ee4, /// 0x144
			 0x60434476, /// 0x148
			 0xa24bc04e, /// 0x14c
			 0x32838e08, /// 0x150
			 0xfdb74cd9, /// 0x154
			 0x7f41e76d, /// 0x158
			 0xf125807d, /// 0x15c
			 0x8b012da6, /// 0x160
			 0xc130e969, /// 0x164
			 0xaa469c7a, /// 0x168
			 0xb382fd3c, /// 0x16c
			 0x5e17d410, /// 0x170
			 0xdf0d3b35, /// 0x174
			 0x1bb5df5c, /// 0x178
			 0xb12c719f, /// 0x17c
			 0x55e7db4b, /// 0x180
			 0x16dd337d, /// 0x184
			 0x04f2403b, /// 0x188
			 0x35703168, /// 0x18c
			 0xc118eef4, /// 0x190
			 0xd75b0652, /// 0x194
			 0x418d5af4, /// 0x198
			 0x7c9a913d, /// 0x19c
			 0xbcd630bf, /// 0x1a0
			 0x60a6d529, /// 0x1a4
			 0xdb169111, /// 0x1a8
			 0x87c776c2, /// 0x1ac
			 0xd6325ab9, /// 0x1b0
			 0x76a5399f, /// 0x1b4
			 0x6564cc0f, /// 0x1b8
			 0xc5e9bd15, /// 0x1bc
			 0x111cd851, /// 0x1c0
			 0x1f949f55, /// 0x1c4
			 0x74cc5e84, /// 0x1c8
			 0x1bf8800a, /// 0x1cc
			 0x26741012, /// 0x1d0
			 0xc5a41bc6, /// 0x1d4
			 0xeb4fad41, /// 0x1d8
			 0x060da2b1, /// 0x1dc
			 0x15664129, /// 0x1e0
			 0x4d18fe54, /// 0x1e4
			 0xf2d203fb, /// 0x1e8
			 0xad578876, /// 0x1ec
			 0xb22e3249, /// 0x1f0
			 0xeb699c35, /// 0x1f4
			 0x1739e54f, /// 0x1f8
			 0x30bcba12, /// 0x1fc
			 0x6285ec75, /// 0x200
			 0xfd5c683c, /// 0x204
			 0xa3846a17, /// 0x208
			 0x5e6a27e5, /// 0x20c
			 0xca2b8333, /// 0x210
			 0x2c8208e1, /// 0x214
			 0x3fc5b9ef, /// 0x218
			 0x2d076fa4, /// 0x21c
			 0x8d15c892, /// 0x220
			 0xdddbc608, /// 0x224
			 0xf21457b0, /// 0x228
			 0x2792172b, /// 0x22c
			 0x2a2f8453, /// 0x230
			 0xd9bf4e91, /// 0x234
			 0xa1bfaed2, /// 0x238
			 0x40394b11, /// 0x23c
			 0xdf5ec63e, /// 0x240
			 0x80f6cf5b, /// 0x244
			 0x6874dd24, /// 0x248
			 0x002665fe, /// 0x24c
			 0x53dd5eb5, /// 0x250
			 0x9b71cec0, /// 0x254
			 0x77d9e9ef, /// 0x258
			 0x596127d8, /// 0x25c
			 0x66888f3a, /// 0x260
			 0x210e2e11, /// 0x264
			 0x6653659f, /// 0x268
			 0xf191acb7, /// 0x26c
			 0x4e7c3c17, /// 0x270
			 0x5afeb193, /// 0x274
			 0x4c7deb4b, /// 0x278
			 0x41eabbf4, /// 0x27c
			 0xa0ea46ba, /// 0x280
			 0x7cf2d1b0, /// 0x284
			 0x1e93195a, /// 0x288
			 0xbc70ef36, /// 0x28c
			 0x50f1327d, /// 0x290
			 0x35fbca17, /// 0x294
			 0xda72d77c, /// 0x298
			 0x7d5f7368, /// 0x29c
			 0xda1a4e59, /// 0x2a0
			 0x0cbbeec9, /// 0x2a4
			 0xab1967a6, /// 0x2a8
			 0x93f2e867, /// 0x2ac
			 0x5992ac9e, /// 0x2b0
			 0x8d9c711d, /// 0x2b4
			 0x07db62a4, /// 0x2b8
			 0xe691fffc, /// 0x2bc
			 0x9b1c5cd9, /// 0x2c0
			 0x3b369f6c, /// 0x2c4
			 0xee6a50a8, /// 0x2c8
			 0xe2004faa, /// 0x2cc
			 0x74dffcbf, /// 0x2d0
			 0x39c2d307, /// 0x2d4
			 0x48fd213e, /// 0x2d8
			 0x90588ec2, /// 0x2dc
			 0x6f43cd22, /// 0x2e0
			 0xea559939, /// 0x2e4
			 0xa199fa61, /// 0x2e8
			 0x4aec905d, /// 0x2ec
			 0xd7c152b0, /// 0x2f0
			 0xdb0b1e02, /// 0x2f4
			 0x408ecf12, /// 0x2f8
			 0x2c00c546, /// 0x2fc
			 0x21ae9e30, /// 0x300
			 0x2c3c556b, /// 0x304
			 0x9168dd21, /// 0x308
			 0x6522e3d1, /// 0x30c
			 0x77dd65ab, /// 0x310
			 0x5373b8eb, /// 0x314
			 0xbfc7946f, /// 0x318
			 0x68fd5545, /// 0x31c
			 0x1aaaf124, /// 0x320
			 0x367722fe, /// 0x324
			 0xa5be2188, /// 0x328
			 0xb3e446e1, /// 0x32c
			 0x6008484c, /// 0x330
			 0x6b4d704d, /// 0x334
			 0xe0855cd5, /// 0x338
			 0xf48ca0ea, /// 0x33c
			 0xc25efeff, /// 0x340
			 0x04e54992, /// 0x344
			 0xffcda57d, /// 0x348
			 0x885adaa5, /// 0x34c
			 0xda5f5f51, /// 0x350
			 0xf62f5914, /// 0x354
			 0xfb8ac170, /// 0x358
			 0xf480ad27, /// 0x35c
			 0x77575776, /// 0x360
			 0x3824c2a8, /// 0x364
			 0x117d60b1, /// 0x368
			 0x8bb757c3, /// 0x36c
			 0x9ac568b0, /// 0x370
			 0x7067c37c, /// 0x374
			 0x5e7ee0d6, /// 0x378
			 0x3e7a8a32, /// 0x37c
			 0xf5345fa5, /// 0x380
			 0x60c8cafd, /// 0x384
			 0xd4f34f15, /// 0x388
			 0x8146df03, /// 0x38c
			 0xfb2d2cf8, /// 0x390
			 0x74b2f4bc, /// 0x394
			 0xe4b96875, /// 0x398
			 0xb2fd8666, /// 0x39c
			 0x92006a45, /// 0x3a0
			 0xdc43a330, /// 0x3a4
			 0x9c30af73, /// 0x3a8
			 0x8528652d, /// 0x3ac
			 0xd1abba49, /// 0x3b0
			 0x9e9eaeb6, /// 0x3b4
			 0xa39292d3, /// 0x3b8
			 0x418f9758, /// 0x3bc
			 0xc9daf6c7, /// 0x3c0
			 0x50cddd02, /// 0x3c4
			 0x37c2043c, /// 0x3c8
			 0x94ad3d0c, /// 0x3cc
			 0xc261160e, /// 0x3d0
			 0x73d07006, /// 0x3d4
			 0x67fe388a, /// 0x3d8
			 0x6328765b, /// 0x3dc
			 0x4414ba38, /// 0x3e0
			 0x751cc10f, /// 0x3e4
			 0x5b9b88c9, /// 0x3e8
			 0xba6b799b, /// 0x3ec
			 0x549d1c3f, /// 0x3f0
			 0x8715fe9b, /// 0x3f4
			 0x595fc1dd, /// 0x3f8
			 0x32ae4dc8, /// 0x3fc
			 0xfd342333, /// 0x400
			 0xcce5df71, /// 0x404
			 0x54a10582, /// 0x408
			 0x69e36dab, /// 0x40c
			 0x71f80ddf, /// 0x410
			 0x8e312a6b, /// 0x414
			 0xcabefa31, /// 0x418
			 0x7224491a, /// 0x41c
			 0x1a8a5ae3, /// 0x420
			 0x469bf80d, /// 0x424
			 0xa89bba85, /// 0x428
			 0x5a7ec1d2, /// 0x42c
			 0x3518064d, /// 0x430
			 0x537d6896, /// 0x434
			 0xf18ceca0, /// 0x438
			 0xebf7d02c, /// 0x43c
			 0x9075f0e1, /// 0x440
			 0x167df39c, /// 0x444
			 0x9300e388, /// 0x448
			 0x86c87478, /// 0x44c
			 0x92e36d25, /// 0x450
			 0xb98862b8, /// 0x454
			 0xf0e13b8b, /// 0x458
			 0x0bfb4071, /// 0x45c
			 0xe7a97f25, /// 0x460
			 0x41ccec6e, /// 0x464
			 0xd34a956b, /// 0x468
			 0x9c851d26, /// 0x46c
			 0x53ef286a, /// 0x470
			 0x537432a8, /// 0x474
			 0xd9f502a8, /// 0x478
			 0x47581013, /// 0x47c
			 0x0454802c, /// 0x480
			 0x3eb84a0c, /// 0x484
			 0x48ceb094, /// 0x488
			 0xf30348e8, /// 0x48c
			 0x4b6c0f65, /// 0x490
			 0xb6a050ab, /// 0x494
			 0x41f93747, /// 0x498
			 0x34d1dfe6, /// 0x49c
			 0x943e8263, /// 0x4a0
			 0x4db2a47c, /// 0x4a4
			 0x113bfd3a, /// 0x4a8
			 0x8e68e9b1, /// 0x4ac
			 0xefd0868d, /// 0x4b0
			 0x113e1f71, /// 0x4b4
			 0xb84da7b0, /// 0x4b8
			 0xa04cd259, /// 0x4bc
			 0x2b29f688, /// 0x4c0
			 0x59cd144f, /// 0x4c4
			 0xd86d0d22, /// 0x4c8
			 0x3c471c33, /// 0x4cc
			 0x8c60e812, /// 0x4d0
			 0x74fcc087, /// 0x4d4
			 0x208711c0, /// 0x4d8
			 0xfb49f17f, /// 0x4dc
			 0xd9cc8662, /// 0x4e0
			 0x6958925a, /// 0x4e4
			 0xb597f6c4, /// 0x4e8
			 0xd153b0f4, /// 0x4ec
			 0x236902ec, /// 0x4f0
			 0x5ae10087, /// 0x4f4
			 0x26d575d6, /// 0x4f8
			 0x11acd7df, /// 0x4fc
			 0x1d8b29e8, /// 0x500
			 0x6df51bc2, /// 0x504
			 0xe9f3253c, /// 0x508
			 0x1c181cb7, /// 0x50c
			 0xca7b8f8c, /// 0x510
			 0x6f52988f, /// 0x514
			 0xb0f439fa, /// 0x518
			 0x2d393dc9, /// 0x51c
			 0xdadd25be, /// 0x520
			 0xa97fcb6f, /// 0x524
			 0x3f41ebc0, /// 0x528
			 0xedfe8c2b, /// 0x52c
			 0xd026d7ea, /// 0x530
			 0xfdd15d80, /// 0x534
			 0x89784d0b, /// 0x538
			 0xe3c245f2, /// 0x53c
			 0x00a9e86e, /// 0x540
			 0x166a45c2, /// 0x544
			 0xf2547dfe, /// 0x548
			 0x961ad9f2, /// 0x54c
			 0x648ffdf2, /// 0x550
			 0xe4a3fd18, /// 0x554
			 0x85914c35, /// 0x558
			 0xc25dda12, /// 0x55c
			 0xb1e2aa5b, /// 0x560
			 0x597ef26e, /// 0x564
			 0x1bab4862, /// 0x568
			 0xd038a09e, /// 0x56c
			 0x0ab00298, /// 0x570
			 0xec63b126, /// 0x574
			 0xbfb75c9b, /// 0x578
			 0x2f5686c6, /// 0x57c
			 0x1c351d7f, /// 0x580
			 0xa45ecd8c, /// 0x584
			 0x5de7549f, /// 0x588
			 0x851f5021, /// 0x58c
			 0xbb5b5b7d, /// 0x590
			 0x2f2dc83f, /// 0x594
			 0x976169a0, /// 0x598
			 0xc87e051c, /// 0x59c
			 0x311c4b19, /// 0x5a0
			 0x2cbabe53, /// 0x5a4
			 0x903b4fb7, /// 0x5a8
			 0x97891c6a, /// 0x5ac
			 0x029d02d4, /// 0x5b0
			 0x189832d5, /// 0x5b4
			 0x20b733d2, /// 0x5b8
			 0xd3bbef4e, /// 0x5bc
			 0x91d8f84f, /// 0x5c0
			 0x61aa0b13, /// 0x5c4
			 0x53f91a8a, /// 0x5c8
			 0xb1f5e675, /// 0x5cc
			 0x2fc21a18, /// 0x5d0
			 0x436b2b21, /// 0x5d4
			 0xeb651280, /// 0x5d8
			 0x5a333cff, /// 0x5dc
			 0x5bfea946, /// 0x5e0
			 0x8ef40a61, /// 0x5e4
			 0xe4ace649, /// 0x5e8
			 0xd9053828, /// 0x5ec
			 0xe4986463, /// 0x5f0
			 0x47003041, /// 0x5f4
			 0x0b6c1023, /// 0x5f8
			 0x43baeaad, /// 0x5fc
			 0x6b48f2f9, /// 0x600
			 0xd3bec681, /// 0x604
			 0x935e486c, /// 0x608
			 0xff2e055a, /// 0x60c
			 0x247406fc, /// 0x610
			 0xd71a2790, /// 0x614
			 0xa73d2620, /// 0x618
			 0x87411644, /// 0x61c
			 0x7d92892d, /// 0x620
			 0x33e630f0, /// 0x624
			 0xc5f63560, /// 0x628
			 0xdd1b7f52, /// 0x62c
			 0x0ed4e3ed, /// 0x630
			 0x7dfa85a7, /// 0x634
			 0x9c420638, /// 0x638
			 0x8a2dfbde, /// 0x63c
			 0x0f7cb193, /// 0x640
			 0xfbcab149, /// 0x644
			 0x906698c0, /// 0x648
			 0xe540fdbd, /// 0x64c
			 0xeaa1b7c4, /// 0x650
			 0xd0575b34, /// 0x654
			 0xb02571da, /// 0x658
			 0xa2d887f4, /// 0x65c
			 0xd2967705, /// 0x660
			 0xa89c8080, /// 0x664
			 0x4c05d180, /// 0x668
			 0x675e61cb, /// 0x66c
			 0x988ee44c, /// 0x670
			 0x76f0766c, /// 0x674
			 0x031e7184, /// 0x678
			 0x1e8f69ee, /// 0x67c
			 0x6d6bdb98, /// 0x680
			 0x4f15cf3a, /// 0x684
			 0xee3fffec, /// 0x688
			 0x73114eb0, /// 0x68c
			 0x78ba8a0a, /// 0x690
			 0x56bbf560, /// 0x694
			 0xf578960a, /// 0x698
			 0xd4d4e639, /// 0x69c
			 0x4bb656b5, /// 0x6a0
			 0x03bb008f, /// 0x6a4
			 0x9fa96270, /// 0x6a8
			 0xe4d43825, /// 0x6ac
			 0x30aebe36, /// 0x6b0
			 0xfcfc7270, /// 0x6b4
			 0xa5cba8c4, /// 0x6b8
			 0xb098e800, /// 0x6bc
			 0xd25730c2, /// 0x6c0
			 0xaa7c7292, /// 0x6c4
			 0x539ac019, /// 0x6c8
			 0xa0d78ceb, /// 0x6cc
			 0x95440020, /// 0x6d0
			 0x8c249447, /// 0x6d4
			 0xca29f556, /// 0x6d8
			 0x5e5dda54, /// 0x6dc
			 0x699c6a65, /// 0x6e0
			 0x920d52cb, /// 0x6e4
			 0x1b844873, /// 0x6e8
			 0x433d3898, /// 0x6ec
			 0x5133e497, /// 0x6f0
			 0x5074149b, /// 0x6f4
			 0x104b9acf, /// 0x6f8
			 0xa5f5e694, /// 0x6fc
			 0xce7f739e, /// 0x700
			 0x86bf4f6f, /// 0x704
			 0x67d40422, /// 0x708
			 0x6148060a, /// 0x70c
			 0x77157526, /// 0x710
			 0x69485b60, /// 0x714
			 0xe4450bc8, /// 0x718
			 0x9ad82eec, /// 0x71c
			 0xa759a4ce, /// 0x720
			 0x2e4f0d8d, /// 0x724
			 0x8866ef6f, /// 0x728
			 0xe7e861cd, /// 0x72c
			 0xb0c605e2, /// 0x730
			 0x67267c20, /// 0x734
			 0x359620c4, /// 0x738
			 0x88e11424, /// 0x73c
			 0xd9cdfbd6, /// 0x740
			 0xa277e470, /// 0x744
			 0x62a7b8f6, /// 0x748
			 0x62895bb0, /// 0x74c
			 0xaacde6f8, /// 0x750
			 0x82247593, /// 0x754
			 0xa2c19af6, /// 0x758
			 0x963772e7, /// 0x75c
			 0x9d785336, /// 0x760
			 0x0533addd, /// 0x764
			 0xe512e4c2, /// 0x768
			 0xd755ae3d, /// 0x76c
			 0xa5333c07, /// 0x770
			 0x29f16c31, /// 0x774
			 0x204b2031, /// 0x778
			 0xdd911316, /// 0x77c
			 0x7eef46fc, /// 0x780
			 0x5a2260d3, /// 0x784
			 0x4e9f2847, /// 0x788
			 0x4cd4d699, /// 0x78c
			 0xa43b6a09, /// 0x790
			 0x992da1ac, /// 0x794
			 0x9cd52b48, /// 0x798
			 0x733bd259, /// 0x79c
			 0x6d5b0f68, /// 0x7a0
			 0x30e14ac9, /// 0x7a4
			 0x84e165f8, /// 0x7a8
			 0xffd7d170, /// 0x7ac
			 0xdf1332ad, /// 0x7b0
			 0x6a43bb38, /// 0x7b4
			 0x539057d7, /// 0x7b8
			 0xeabded87, /// 0x7bc
			 0xb58b4d61, /// 0x7c0
			 0x532f8c02, /// 0x7c4
			 0x9fcdccff, /// 0x7c8
			 0x32375ae7, /// 0x7cc
			 0x946448ea, /// 0x7d0
			 0x24664476, /// 0x7d4
			 0x105320c7, /// 0x7d8
			 0xb5705419, /// 0x7dc
			 0x52a8144f, /// 0x7e0
			 0x9350590a, /// 0x7e4
			 0xd974eb9b, /// 0x7e8
			 0x13dc960a, /// 0x7ec
			 0xbe6cc587, /// 0x7f0
			 0x4555bbb8, /// 0x7f4
			 0x08983afb, /// 0x7f8
			 0xf69c3f72, /// 0x7fc
			 0xe16680d1, /// 0x800
			 0xbf072998, /// 0x804
			 0xf42e273d, /// 0x808
			 0xe9f81a4f, /// 0x80c
			 0x05a38cf8, /// 0x810
			 0x08723227, /// 0x814
			 0x036326c4, /// 0x818
			 0x57bb784e, /// 0x81c
			 0x5ebb8f79, /// 0x820
			 0x0826e100, /// 0x824
			 0x3842d7b8, /// 0x828
			 0x1899dc8d, /// 0x82c
			 0x0fcdbb75, /// 0x830
			 0x0556e758, /// 0x834
			 0x072ab1b1, /// 0x838
			 0x6441a04f, /// 0x83c
			 0x4e605761, /// 0x840
			 0x6a3c605d, /// 0x844
			 0x790f3d9c, /// 0x848
			 0x6fa71836, /// 0x84c
			 0x95b27fc2, /// 0x850
			 0x5fea4cf0, /// 0x854
			 0x395a8ac6, /// 0x858
			 0xd680acba, /// 0x85c
			 0xfecacca2, /// 0x860
			 0x7155c127, /// 0x864
			 0x18aadd58, /// 0x868
			 0xe0a9a527, /// 0x86c
			 0x99f154f7, /// 0x870
			 0x49afcd4a, /// 0x874
			 0xbee6ab13, /// 0x878
			 0x65aa5e06, /// 0x87c
			 0x69c40faa, /// 0x880
			 0xe6e522ac, /// 0x884
			 0x2ffed7fd, /// 0x888
			 0x8ab2cc40, /// 0x88c
			 0x828047c5, /// 0x890
			 0x77d71c51, /// 0x894
			 0x41f85b7e, /// 0x898
			 0xc2956dc7, /// 0x89c
			 0xfea0a8d3, /// 0x8a0
			 0x2da8d3d0, /// 0x8a4
			 0x722d545f, /// 0x8a8
			 0x00413868, /// 0x8ac
			 0xe9fa722c, /// 0x8b0
			 0x3c43a5dd, /// 0x8b4
			 0xbc593b9a, /// 0x8b8
			 0x6cbdc704, /// 0x8bc
			 0x4baba50c, /// 0x8c0
			 0xe05d2b7f, /// 0x8c4
			 0x8442be67, /// 0x8c8
			 0xd739e3e0, /// 0x8cc
			 0xc9ec9b7b, /// 0x8d0
			 0x2fff7feb, /// 0x8d4
			 0x7e577076, /// 0x8d8
			 0xf28d8dc1, /// 0x8dc
			 0x6ac43254, /// 0x8e0
			 0x71b38e98, /// 0x8e4
			 0x1e0f8f49, /// 0x8e8
			 0x4cb55aee, /// 0x8ec
			 0xa5dfb816, /// 0x8f0
			 0xca14c937, /// 0x8f4
			 0x7af130cf, /// 0x8f8
			 0x8aa3c4f3, /// 0x8fc
			 0x69505793, /// 0x900
			 0xe10684ad, /// 0x904
			 0x11589f22, /// 0x908
			 0x7382a7ba, /// 0x90c
			 0xddf21844, /// 0x910
			 0x6260581f, /// 0x914
			 0xe3686b68, /// 0x918
			 0xb44a451b, /// 0x91c
			 0x1ddf4db2, /// 0x920
			 0x2f496ffe, /// 0x924
			 0x47da3fff, /// 0x928
			 0x5dc44e5e, /// 0x92c
			 0xa20f0fc8, /// 0x930
			 0x0a0813a5, /// 0x934
			 0xd28f0211, /// 0x938
			 0x8cdc43f1, /// 0x93c
			 0x739a17e5, /// 0x940
			 0xba39e1a6, /// 0x944
			 0xfa8b5520, /// 0x948
			 0x9225e3e4, /// 0x94c
			 0xa17cb270, /// 0x950
			 0x99d36550, /// 0x954
			 0xbf7d6ac4, /// 0x958
			 0x1fd1e949, /// 0x95c
			 0xb579284c, /// 0x960
			 0x5cacd83a, /// 0x964
			 0x41b08a5f, /// 0x968
			 0xcf57989b, /// 0x96c
			 0x79223b3d, /// 0x970
			 0xd1c129a8, /// 0x974
			 0xb4c89080, /// 0x978
			 0xf05d834e, /// 0x97c
			 0x49d9a1d1, /// 0x980
			 0xd3ff6a79, /// 0x984
			 0xe596e3ae, /// 0x988
			 0x59b08b42, /// 0x98c
			 0x1cc756b1, /// 0x990
			 0x607502c7, /// 0x994
			 0x3ec43d69, /// 0x998
			 0x99cc408c, /// 0x99c
			 0xcee3f0ff, /// 0x9a0
			 0x9eef7933, /// 0x9a4
			 0x7a2cf1be, /// 0x9a8
			 0x06bc12fd, /// 0x9ac
			 0x3dd7e023, /// 0x9b0
			 0xec1e7826, /// 0x9b4
			 0x83f8693c, /// 0x9b8
			 0x2bc682d8, /// 0x9bc
			 0x27ceb121, /// 0x9c0
			 0x98cf4ff5, /// 0x9c4
			 0x3e150795, /// 0x9c8
			 0x0802a71d, /// 0x9cc
			 0x8a0b58df, /// 0x9d0
			 0x1ec85535, /// 0x9d4
			 0xe6911cc8, /// 0x9d8
			 0x5569591e, /// 0x9dc
			 0xa7a0b084, /// 0x9e0
			 0xfc95bf69, /// 0x9e4
			 0xce891f1f, /// 0x9e8
			 0x44c67608, /// 0x9ec
			 0x46bddf9c, /// 0x9f0
			 0x6be52fe2, /// 0x9f4
			 0x09495c75, /// 0x9f8
			 0xc17729cc, /// 0x9fc
			 0x7044c4ac, /// 0xa00
			 0xbe7d2816, /// 0xa04
			 0x4801e2a4, /// 0xa08
			 0x9a67f162, /// 0xa0c
			 0x9a683e8a, /// 0xa10
			 0x75ebd9cd, /// 0xa14
			 0x21648d7b, /// 0xa18
			 0x8ecd3074, /// 0xa1c
			 0x041a33d5, /// 0xa20
			 0xcae0b9ed, /// 0xa24
			 0x8a56fc49, /// 0xa28
			 0x940c004d, /// 0xa2c
			 0x8cfc3839, /// 0xa30
			 0x52f74a32, /// 0xa34
			 0xd97a5873, /// 0xa38
			 0xabc1a0f4, /// 0xa3c
			 0x208eae00, /// 0xa40
			 0x442103b3, /// 0xa44
			 0x3b29568b, /// 0xa48
			 0x65f6a663, /// 0xa4c
			 0x65df6af6, /// 0xa50
			 0x906b9bb6, /// 0xa54
			 0xee8becc2, /// 0xa58
			 0xa63737d9, /// 0xa5c
			 0x9f247cf2, /// 0xa60
			 0xa6505d5e, /// 0xa64
			 0x14093b59, /// 0xa68
			 0xa7d8a176, /// 0xa6c
			 0xa1c991cd, /// 0xa70
			 0x05682bb1, /// 0xa74
			 0x76601bf7, /// 0xa78
			 0xdd4197a7, /// 0xa7c
			 0xf604b698, /// 0xa80
			 0x6cc13646, /// 0xa84
			 0x58e9aee5, /// 0xa88
			 0x883324b9, /// 0xa8c
			 0xb99a90bc, /// 0xa90
			 0xb049aee9, /// 0xa94
			 0x5408fa77, /// 0xa98
			 0xa33c1f2d, /// 0xa9c
			 0xd7ac4579, /// 0xaa0
			 0xdb3625db, /// 0xaa4
			 0x370b6c3f, /// 0xaa8
			 0x96fab8b7, /// 0xaac
			 0x0f9c6ea4, /// 0xab0
			 0x76e62ea3, /// 0xab4
			 0xa088dc79, /// 0xab8
			 0xb6b0fbfb, /// 0xabc
			 0x15ffb40c, /// 0xac0
			 0xfd0561af, /// 0xac4
			 0x7e9eea87, /// 0xac8
			 0xb921ccf5, /// 0xacc
			 0x9aa3d380, /// 0xad0
			 0xd3f0b37c, /// 0xad4
			 0x55b5dc91, /// 0xad8
			 0xe830aec0, /// 0xadc
			 0x9e39d95f, /// 0xae0
			 0x85b39118, /// 0xae4
			 0x2e6037e5, /// 0xae8
			 0x59c50f45, /// 0xaec
			 0xb27583cc, /// 0xaf0
			 0x57b8841b, /// 0xaf4
			 0x5b78738d, /// 0xaf8
			 0x1707b0a1, /// 0xafc
			 0xb5508b81, /// 0xb00
			 0x6f425f8a, /// 0xb04
			 0x55d279da, /// 0xb08
			 0xde635124, /// 0xb0c
			 0xba7a0ce1, /// 0xb10
			 0x61fbcc37, /// 0xb14
			 0x3f389257, /// 0xb18
			 0xc7b2c2cd, /// 0xb1c
			 0x0f542c68, /// 0xb20
			 0xb9126d7a, /// 0xb24
			 0x51cf03a9, /// 0xb28
			 0x3d8c153d, /// 0xb2c
			 0xc21e556f, /// 0xb30
			 0x700ac654, /// 0xb34
			 0x3c8b0be4, /// 0xb38
			 0x94279adf, /// 0xb3c
			 0x5d27dad6, /// 0xb40
			 0xf2fa0b90, /// 0xb44
			 0x8ff92a2d, /// 0xb48
			 0x10032ae4, /// 0xb4c
			 0x1300f570, /// 0xb50
			 0xa61af63b, /// 0xb54
			 0x008847c3, /// 0xb58
			 0x33ae1114, /// 0xb5c
			 0x2f3f10d9, /// 0xb60
			 0x003a577b, /// 0xb64
			 0x40c24316, /// 0xb68
			 0x3d139902, /// 0xb6c
			 0x9ee87d9f, /// 0xb70
			 0xb1478728, /// 0xb74
			 0xbfd5f71a, /// 0xb78
			 0x4c7331f5, /// 0xb7c
			 0x1282aa47, /// 0xb80
			 0x066685f0, /// 0xb84
			 0x61167c92, /// 0xb88
			 0xb9f3b59d, /// 0xb8c
			 0x1874143a, /// 0xb90
			 0xe57ca49f, /// 0xb94
			 0x5bbef97b, /// 0xb98
			 0x6ac7996c, /// 0xb9c
			 0xbd0b9a1e, /// 0xba0
			 0x3b4d1dec, /// 0xba4
			 0x2d131f0a, /// 0xba8
			 0x12e9cd8a, /// 0xbac
			 0xff662e70, /// 0xbb0
			 0x59277030, /// 0xbb4
			 0x635cbab6, /// 0xbb8
			 0x99054cbe, /// 0xbbc
			 0x1928c366, /// 0xbc0
			 0xfa82ee87, /// 0xbc4
			 0x5c444917, /// 0xbc8
			 0xc6927143, /// 0xbcc
			 0x4232c517, /// 0xbd0
			 0xb1e814b7, /// 0xbd4
			 0x00ed26f8, /// 0xbd8
			 0xe3323196, /// 0xbdc
			 0x74f784a9, /// 0xbe0
			 0x85c4fbfd, /// 0xbe4
			 0x94b27ad3, /// 0xbe8
			 0x4e4a6814, /// 0xbec
			 0xaf042666, /// 0xbf0
			 0xb612ef0e, /// 0xbf4
			 0x23f9b21e, /// 0xbf8
			 0x21603660, /// 0xbfc
			 0x7c8ecb1c, /// 0xc00
			 0xfc8e3aef, /// 0xc04
			 0xe80ee0be, /// 0xc08
			 0x859bb755, /// 0xc0c
			 0xf0d5f1b1, /// 0xc10
			 0x0ffef379, /// 0xc14
			 0xdaf65c33, /// 0xc18
			 0xdbb4731a, /// 0xc1c
			 0xa7526dd2, /// 0xc20
			 0xe4dd8b04, /// 0xc24
			 0x9dbab821, /// 0xc28
			 0x409d51de, /// 0xc2c
			 0xfcb4e678, /// 0xc30
			 0xc22692fc, /// 0xc34
			 0x25563f3f, /// 0xc38
			 0x9bdcf615, /// 0xc3c
			 0xde6d8e89, /// 0xc40
			 0xbef1587b, /// 0xc44
			 0x57e7ed18, /// 0xc48
			 0x78061381, /// 0xc4c
			 0x3b676979, /// 0xc50
			 0xf6e8d5bf, /// 0xc54
			 0xd9d368da, /// 0xc58
			 0x03caf59e, /// 0xc5c
			 0xd7f04bfc, /// 0xc60
			 0x564d64b1, /// 0xc64
			 0xb3e6a736, /// 0xc68
			 0xf4c8f1a2, /// 0xc6c
			 0x8109abc8, /// 0xc70
			 0x0739b6c6, /// 0xc74
			 0x06724362, /// 0xc78
			 0x133db8e6, /// 0xc7c
			 0x6cb69e5d, /// 0xc80
			 0x19ec7e70, /// 0xc84
			 0x298b5f1c, /// 0xc88
			 0x0d017be4, /// 0xc8c
			 0xd37c998f, /// 0xc90
			 0x8732c3b4, /// 0xc94
			 0xc7006aca, /// 0xc98
			 0x24769248, /// 0xc9c
			 0x0a58f731, /// 0xca0
			 0x78599b25, /// 0xca4
			 0x61f32c90, /// 0xca8
			 0x2731a7b0, /// 0xcac
			 0x0c67f51f, /// 0xcb0
			 0x771dff10, /// 0xcb4
			 0xd52d447c, /// 0xcb8
			 0x6e9beacb, /// 0xcbc
			 0xe2d4b614, /// 0xcc0
			 0x64f0f645, /// 0xcc4
			 0x07e1fb02, /// 0xcc8
			 0xcbcd3983, /// 0xccc
			 0x152baeca, /// 0xcd0
			 0xbb73a7db, /// 0xcd4
			 0x00425e1c, /// 0xcd8
			 0x23fe08be, /// 0xcdc
			 0xb1898aa3, /// 0xce0
			 0x70037cd6, /// 0xce4
			 0x431f735c, /// 0xce8
			 0x983c781c, /// 0xcec
			 0x34c9a60f, /// 0xcf0
			 0x89018d8b, /// 0xcf4
			 0x52abff77, /// 0xcf8
			 0x93e47514, /// 0xcfc
			 0x606826ee, /// 0xd00
			 0xc0d07195, /// 0xd04
			 0x035f3e73, /// 0xd08
			 0x8df60941, /// 0xd0c
			 0xf4c0f29f, /// 0xd10
			 0x67c390dc, /// 0xd14
			 0x79a51b1a, /// 0xd18
			 0x4b96cbbc, /// 0xd1c
			 0x0aeb0e36, /// 0xd20
			 0x9dadc0be, /// 0xd24
			 0x806ea35e, /// 0xd28
			 0xfb065f07, /// 0xd2c
			 0x65ddef42, /// 0xd30
			 0xfe0fefd1, /// 0xd34
			 0x956892f9, /// 0xd38
			 0x5a8cb066, /// 0xd3c
			 0x364ea978, /// 0xd40
			 0xfeb3e882, /// 0xd44
			 0xb547f3bf, /// 0xd48
			 0x65dd10bd, /// 0xd4c
			 0xeda5baa1, /// 0xd50
			 0x81ed011c, /// 0xd54
			 0xd36a9023, /// 0xd58
			 0x7039994f, /// 0xd5c
			 0x50375f70, /// 0xd60
			 0x946b2cc2, /// 0xd64
			 0xc26f52ed, /// 0xd68
			 0xabbf051d, /// 0xd6c
			 0x39ab8bbd, /// 0xd70
			 0x6fbf4ca2, /// 0xd74
			 0x01c6111f, /// 0xd78
			 0x222a5098, /// 0xd7c
			 0xcf9b69cc, /// 0xd80
			 0x889ea150, /// 0xd84
			 0xfd95cdbd, /// 0xd88
			 0xd67c179d, /// 0xd8c
			 0x941887e5, /// 0xd90
			 0x5c13fdb1, /// 0xd94
			 0x8beea062, /// 0xd98
			 0x74b02854, /// 0xd9c
			 0x5a195c57, /// 0xda0
			 0x4435be5b, /// 0xda4
			 0xb903179c, /// 0xda8
			 0x97951280, /// 0xdac
			 0xbfef770f, /// 0xdb0
			 0x62823ba5, /// 0xdb4
			 0x2df30408, /// 0xdb8
			 0x494ffdd6, /// 0xdbc
			 0x93560d5c, /// 0xdc0
			 0xbe9810a3, /// 0xdc4
			 0x90ef5071, /// 0xdc8
			 0x7f44d7c8, /// 0xdcc
			 0xeafced76, /// 0xdd0
			 0xe857ab99, /// 0xdd4
			 0x9d9e386a, /// 0xdd8
			 0x37270b61, /// 0xddc
			 0x71a309cb, /// 0xde0
			 0xe21b8f6d, /// 0xde4
			 0x488c5b48, /// 0xde8
			 0x54138047, /// 0xdec
			 0x6d189b12, /// 0xdf0
			 0xf7977c22, /// 0xdf4
			 0x1fd4d710, /// 0xdf8
			 0xbbe1201c, /// 0xdfc
			 0x0ac8c67e, /// 0xe00
			 0xd7e69c2d, /// 0xe04
			 0xb0b69c02, /// 0xe08
			 0x1a2fa565, /// 0xe0c
			 0x94fae09d, /// 0xe10
			 0xcb0f39e9, /// 0xe14
			 0xbd6a0f52, /// 0xe18
			 0x0f6e8865, /// 0xe1c
			 0x3922c6b0, /// 0xe20
			 0x4e5f79d9, /// 0xe24
			 0xc273133c, /// 0xe28
			 0xea828b9d, /// 0xe2c
			 0x57d6052c, /// 0xe30
			 0x0930b0f9, /// 0xe34
			 0x73ba8f11, /// 0xe38
			 0x9e97e45b, /// 0xe3c
			 0x2d9a05e8, /// 0xe40
			 0xab12ed0a, /// 0xe44
			 0x0f017eb1, /// 0xe48
			 0x9c77b41f, /// 0xe4c
			 0x592f8ec3, /// 0xe50
			 0xb3a7ce94, /// 0xe54
			 0xb627c2a2, /// 0xe58
			 0x92e8fcc6, /// 0xe5c
			 0xb0cc976e, /// 0xe60
			 0x77645d12, /// 0xe64
			 0x05789636, /// 0xe68
			 0x4ca007b5, /// 0xe6c
			 0xae26f7e1, /// 0xe70
			 0xc46db728, /// 0xe74
			 0x10433aa8, /// 0xe78
			 0x79dabb63, /// 0xe7c
			 0xae09f9be, /// 0xe80
			 0xbb8478a4, /// 0xe84
			 0x59f48eb0, /// 0xe88
			 0x3dc3828f, /// 0xe8c
			 0xb4cd1320, /// 0xe90
			 0xe3e33748, /// 0xe94
			 0xd7c81ed9, /// 0xe98
			 0x9c13023a, /// 0xe9c
			 0xe5f575d0, /// 0xea0
			 0x9cba39e8, /// 0xea4
			 0x811c9843, /// 0xea8
			 0x2be0cb64, /// 0xeac
			 0x068ce7e4, /// 0xeb0
			 0x2f133f9c, /// 0xeb4
			 0x50416cb2, /// 0xeb8
			 0xfe35d594, /// 0xebc
			 0x34c67f27, /// 0xec0
			 0x61d44bf1, /// 0xec4
			 0x6c8b395f, /// 0xec8
			 0x3c172c9e, /// 0xecc
			 0xe667d2e9, /// 0xed0
			 0x5a52e83f, /// 0xed4
			 0xa9e54e9d, /// 0xed8
			 0x748de5da, /// 0xedc
			 0x74845030, /// 0xee0
			 0x6841bb39, /// 0xee4
			 0x0835b15a, /// 0xee8
			 0xd0a30624, /// 0xeec
			 0xc988a44c, /// 0xef0
			 0x3ef7d1e2, /// 0xef4
			 0x7261738b, /// 0xef8
			 0x8c5f008b, /// 0xefc
			 0x46f5922a, /// 0xf00
			 0x36405b2c, /// 0xf04
			 0x28e92102, /// 0xf08
			 0x87bec291, /// 0xf0c
			 0x95d4ce41, /// 0xf10
			 0x28a782de, /// 0xf14
			 0xa544ecc5, /// 0xf18
			 0xdc66cc15, /// 0xf1c
			 0xeb9a9fad, /// 0xf20
			 0xcd4ba5aa, /// 0xf24
			 0x1d1e14cb, /// 0xf28
			 0xdecf2fcf, /// 0xf2c
			 0x1f709b99, /// 0xf30
			 0x6910b230, /// 0xf34
			 0x639fd53e, /// 0xf38
			 0xab115e74, /// 0xf3c
			 0x51c20416, /// 0xf40
			 0x2ba8fa2b, /// 0xf44
			 0x2b9958e9, /// 0xf48
			 0xf779100d, /// 0xf4c
			 0x3ce07217, /// 0xf50
			 0x4b1ae6fd, /// 0xf54
			 0x65ec32d7, /// 0xf58
			 0x7a360a16, /// 0xf5c
			 0xcbe7346d, /// 0xf60
			 0xfaa68f58, /// 0xf64
			 0xb9748576, /// 0xf68
			 0x85894bd5, /// 0xf6c
			 0xb344e7b9, /// 0xf70
			 0x26f6ddf6, /// 0xf74
			 0x8efd474d, /// 0xf78
			 0x5791bcd9, /// 0xf7c
			 0xc8f82496, /// 0xf80
			 0x8b505428, /// 0xf84
			 0x9d361ef0, /// 0xf88
			 0x3aa572ec, /// 0xf8c
			 0x44490c5d, /// 0xf90
			 0x972b41e7, /// 0xf94
			 0x9de99924, /// 0xf98
			 0xae4b7039, /// 0xf9c
			 0x0075b1af, /// 0xfa0
			 0x21eb5b60, /// 0xfa4
			 0xefa6b4cb, /// 0xfa8
			 0xf470de3c, /// 0xfac
			 0xe45c8bed, /// 0xfb0
			 0x483539e6, /// 0xfb4
			 0xca33b8b8, /// 0xfb8
			 0x05505118, /// 0xfbc
			 0x48c00768, /// 0xfc0
			 0x103e26b0, /// 0xfc4
			 0x85d4e90e, /// 0xfc8
			 0xb1b149f0, /// 0xfcc
			 0x1d4be994, /// 0xfd0
			 0x4b6dcd8a, /// 0xfd4
			 0xdd1f03a2, /// 0xfd8
			 0x9a8ac7c0, /// 0xfdc
			 0x0cb9355d, /// 0xfe0
			 0xbc5a61ce, /// 0xfe4
			 0xaf7f6052, /// 0xfe8
			 0x9cf6d136, /// 0xfec
			 0xcb1b9c2f, /// 0xff0
			 0x1927cd51, /// 0xff4
			 0xa948b41c, /// 0xff8
			 0x295056ee /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0e000003,                                                  // Trailing 1s:                                /// 00000
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00008
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x00000000,                                                  // zero                                        /// 00010
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00014
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00018
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0001c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00020
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00024
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0002c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00030
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0003c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00040
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00044
			 0x80000000,                                                  // -zero                                       /// 00048
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00050
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x7fc00001,                                                  // signaling NaN                               /// 00058
			 0x33333333,                                                  // 4 random values                             /// 0005c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00060
			 0x0c400000,                                                  // Leading 1s:                                 /// 00064
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00068
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0006c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00070
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00074
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00078
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0007c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00080
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x0e000007,                                                  // Trailing 1s:                                /// 00088
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00090
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00098
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0x00000000,                                                  // zero                                        /// 000ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000c0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000cc
			 0x7fc00001,                                                  // signaling NaN                               /// 000d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00104
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00108
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00110
			 0x0c600000,                                                  // Leading 1s:                                 /// 00114
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00120
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00124
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00128
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0012c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00134
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00138
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00148
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0014c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00150
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00154
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0xcb000000,                                                  // -8388608.0                                  /// 00160
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00168
			 0x33333333,                                                  // 4 random values                             /// 0016c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00170
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00174
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00178
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00180
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00184
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0018c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00190
			 0x0e000001,                                                  // Trailing 1s:                                /// 00194
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0019c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0x80000000,                                                  // -zero                                       /// 001ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001cc
			 0x4b000000,                                                  // 8388608.0                                   /// 001d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00200
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00208
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0020c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00210
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x0c400000,                                                  // Leading 1s:                                 /// 00218
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0021c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00220
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00224
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0022c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00230
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00238
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00240
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0024c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00254
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0025c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00264
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00268
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0026c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00270
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00274
			 0x0e000001,                                                  // Trailing 1s:                                /// 00278
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00284
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00288
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00290
			 0xcb000000,                                                  // -8388608.0                                  /// 00294
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 002e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002ec
			 0x55555555,                                                  // 4 random values                             /// 002f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80000000,                                                  // -zero                                       /// 002f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 002fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00304
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00308
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0030c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00310
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00318
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00320
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00324
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0032c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00330
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00334
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00340
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00344
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0xffc00001,                                                  // -signaling NaN                              /// 00358
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0035c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00364
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00368
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0036c
			 0x00000000,                                                  // zero                                        /// 00370
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x4b000000,                                                  // 8388608.0                                   /// 00378
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0037c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00380
			 0x0c600000,                                                  // Leading 1s:                                 /// 00384
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00388
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0038c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00390
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00398
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0039c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003a0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003a4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 003b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0xff800000,                                                  // -inf                                        /// 003f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00400
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00404
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00408
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0040c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00410
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00414
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00418
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0041c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00424
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00428
			 0x00011111,                                                  // 9.7958E-41                                  /// 0042c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00430
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00438
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0043c
			 0xbf028f5c,                                                  // -0.51                                       /// 00440
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00444
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x80011111,                                                  // -9.7958E-41                                 /// 0044c
			 0x3f028f5c,                                                  // 0.51                                        /// 00450
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00454
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00458
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0045c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0046c
			 0x80000000,                                                  // -zero                                       /// 00470
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0047c
			 0x80000000,                                                  // -zero                                       /// 00480
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00488
			 0x00011111,                                                  // 9.7958E-41                                  /// 0048c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00490
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00494
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00498
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0049c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004e4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00504
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0050c
			 0x3f028f5c,                                                  // 0.51                                        /// 00510
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00514
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00518
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0051c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00520
			 0x0c700000,                                                  // Leading 1s:                                 /// 00524
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00528
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0052c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00530
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00534
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00538
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0053c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00540
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00544
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00548
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00550
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00558
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0055c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00564
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00568
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00570
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00574
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00578
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0057c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00580
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0058c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00590
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00594
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0059c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00604
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0060c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00618
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00620
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00634
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00638
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00644
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0064c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00650
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00658
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0065c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00660
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00664
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0066c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00670
			 0x0e000003,                                                  // Trailing 1s:                                /// 00674
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x80000000,                                                  // -zero                                       /// 0067c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00680
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00684
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00688
			 0x0e000007,                                                  // Trailing 1s:                                /// 0068c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00690
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00694
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00698
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0069c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006e0
			 0x00000000,                                                  // zero                                        /// 006e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00700
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00704
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0070c
			 0x00000000,                                                  // zero                                        /// 00710
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00714
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00718
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0071c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00720
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00724
			 0xbf028f5c,                                                  // -0.51                                       /// 00728
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0072c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00738
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0073c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00744
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00748
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0074c
			 0xffc00001,                                                  // -signaling NaN                              /// 00750
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00754
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00758
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00760
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00764
			 0x0c780000,                                                  // Leading 1s:                                 /// 00768
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0076c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00774
			 0xbf028f5c,                                                  // -0.51                                       /// 00778
			 0x3f028f5c,                                                  // 0.51                                        /// 0077c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00784
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0078c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00790
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00798
			 0x7f800000,                                                  // inf                                         /// 0079c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007a0
			 0xffc00001,                                                  // -signaling NaN                              /// 007a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x80000000,                                                  // -zero                                       /// 007ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00808
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0080c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00814
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00818
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00824
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0082c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00830
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00838
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00844
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00848
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0084c
			 0x00000000,                                                  // zero                                        /// 00850
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00854
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0085c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00860
			 0x00011111,                                                  // 9.7958E-41                                  /// 00864
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00868
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0086c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00878
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0087c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00888
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00894
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00898
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0089c
			 0x0e000007,                                                  // Trailing 1s:                                /// 008a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008d8
			 0xcb000000,                                                  // -8388608.0                                  /// 008dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008e8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x80000000,                                                  // -zero                                       /// 008f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00900
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00904
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00908
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0090c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00910
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00914
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0091c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00924
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00928
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0092c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00934
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00938
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00940
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00944
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0xcb000000,                                                  // -8388608.0                                  /// 0094c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00950
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00954
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00958
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0095c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0xcb000000,                                                  // -8388608.0                                  /// 00964
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00968
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0096c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00970
			 0xffc00001,                                                  // -signaling NaN                              /// 00974
			 0xff800000,                                                  // -inf                                        /// 00978
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0097c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00980
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00988
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00990
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00994
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00998
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0099c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x55555555,                                                  // 4 random values                             /// 009a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x80000000,                                                  // -zero                                       /// 009b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0xffc00001,                                                  // -signaling NaN                              /// 009b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 009cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 009d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 009dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a0c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a10
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a1c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a30
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a40
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a44
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a48
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a68
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a74
			 0x3f028f5c,                                                  // 0.51                                        /// 00a78
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a80
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a84
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x4b000000,                                                  // 8388608.0                                   /// 00a8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a98
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a9c
			 0x00000000,                                                  // zero                                        /// 00aa0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00aa4
			 0x7fc00001,                                                  // signaling NaN                               /// 00aa8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00aac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ab4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00abc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00acc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ad0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ad4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ae0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ae8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00af0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00af4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00af8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00afc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b00
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b08
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b10
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b14
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b20
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b24
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b30
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b38
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b44
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b48
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b50
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b5c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b64
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b74
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b78
			 0x7fc00001,                                                  // signaling NaN                               /// 00b7c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x33333333,                                                  // 4 random values                             /// 00b90
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b94
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b98
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ba0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ba4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ba8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bb0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bc8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bd4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bd8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bdc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00be0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00be8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bfc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c00
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c08
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x00000000,                                                  // zero                                        /// 00c10
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c1c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c2c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c3c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c54
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c70
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c78
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c8c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ca0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0xff800000,                                                  // -inf                                        /// 00cac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cb4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cb8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cbc
			 0xff800000,                                                  // -inf                                        /// 00cc0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cc8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cd4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cd8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cdc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ce0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x55555555,                                                  // 4 random values                             /// 00cf0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cf4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cfc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d04
			 0x3f028f5c,                                                  // 0.51                                        /// 00d08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d10
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d18
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d1c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d2c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d38
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d3c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d48
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d50
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d68
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d78
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d84
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x33333333,                                                  // 4 random values                             /// 00d90
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d98
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00da0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00da4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00db0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00db4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dbc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dc8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dcc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dd0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dd8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00df0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00df4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00df8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dfc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e08
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e14
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e18
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e20
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e2c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e34
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e48
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0xffc00001,                                                  // -signaling NaN                              /// 00e5c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e60
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e64
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e68
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x80000000,                                                  // -zero                                       /// 00e70
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0xff800000,                                                  // -inf                                        /// 00e80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e88
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e8c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ea4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ea8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00eb0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00eb8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ebc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ec0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ec4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ec8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ecc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ed0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ed4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ed8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ee0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ee4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ee8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00eec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ef0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ef8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f04
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f08
			 0x80000000,                                                  // -zero                                       /// 00f0c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f10
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f18
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f1c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f20
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f30
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f34
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f3c
			 0x7f800000,                                                  // inf                                         /// 00f40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f5c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f64
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f80
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f84
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f90
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f98
			 0x7fc00001,                                                  // signaling NaN                               /// 00f9c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fa0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fa4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fa8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fb0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fc0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fc8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fd4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x7fc00001,                                                  // signaling NaN                               /// 00fe0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fe4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ff0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x00002000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x000004b4,
			 0x0000041c,
			 0x00000158,
			 0x0000059c,
			 0x00000474,
			 0x00000490,
			 0x00000038,
			 0x0000011c,

			 /// vpu register f2
			 0x41a00000,
			 0x40800000,
			 0x41700000,
			 0x41200000,
			 0x41c00000,
			 0x40400000,
			 0x40a00000,
			 0x41000000,

			 /// vpu register f3
			 0x3f800000,
			 0x41d00000,
			 0x41a00000,
			 0x41c80000,
			 0x41700000,
			 0x40000000,
			 0x41c00000,
			 0x41f00000,

			 /// vpu register f4
			 0x40800000,
			 0x41e80000,
			 0x41000000,
			 0x41200000,
			 0x40c00000,
			 0x41f00000,
			 0x41b80000,
			 0x41400000,

			 /// vpu register f5
			 0x41200000,
			 0x41980000,
			 0x41600000,
			 0x40400000,
			 0x40400000,
			 0x42000000,
			 0x41900000,
			 0x40e00000,

			 /// vpu register f6
			 0x41a00000,
			 0x41f80000,
			 0x40e00000,
			 0x40c00000,
			 0x41e00000,
			 0x41000000,
			 0x41a00000,
			 0x41c00000,

			 /// vpu register f7
			 0x41e80000,
			 0x41d80000,
			 0x40c00000,
			 0x41e00000,
			 0x41e00000,
			 0x40000000,
			 0x41f00000,
			 0x41700000,

			 /// vpu register f8
			 0x41c80000,
			 0x41600000,
			 0x41a80000,
			 0x41f80000,
			 0x41d00000,
			 0x41500000,
			 0x41b00000,
			 0x41880000,

			 /// vpu register f9
			 0x41980000,
			 0x41e80000,
			 0x3f800000,
			 0x41900000,
			 0x41b80000,
			 0x41e80000,
			 0x41000000,
			 0x40400000,

			 /// vpu register f10
			 0x42000000,
			 0x41880000,
			 0x41300000,
			 0x41e80000,
			 0x41880000,
			 0x41f00000,
			 0x41880000,
			 0x41600000,

			 /// vpu register f11
			 0x41980000,
			 0x40000000,
			 0x41d80000,
			 0x41100000,
			 0x41a80000,
			 0x41f80000,
			 0x41b80000,
			 0x41200000,

			 /// vpu register f12
			 0x41b80000,
			 0x40e00000,
			 0x41300000,
			 0x41300000,
			 0x41000000,
			 0x40000000,
			 0x40400000,
			 0x41c00000,

			 /// vpu register f13
			 0x41b80000,
			 0x40c00000,
			 0x41200000,
			 0x41800000,
			 0x41700000,
			 0x40000000,
			 0x41700000,
			 0x40000000,

			 /// vpu register f14
			 0x41700000,
			 0x42000000,
			 0x41700000,
			 0x41c00000,
			 0x41880000,
			 0x40e00000,
			 0x41800000,
			 0x41d80000,

			 /// vpu register f15
			 0x41c00000,
			 0x41880000,
			 0x41880000,
			 0x41a00000,
			 0x41c80000,
			 0x40000000,
			 0x41000000,
			 0x41900000,

			 /// vpu register f16
			 0x41a00000,
			 0x40c00000,
			 0x41300000,
			 0x41700000,
			 0x41f00000,
			 0x41a00000,
			 0x40a00000,
			 0x41a80000,

			 /// vpu register f17
			 0x40c00000,
			 0x40000000,
			 0x41e00000,
			 0x41980000,
			 0x41b00000,
			 0x41a80000,
			 0x41b80000,
			 0x41800000,

			 /// vpu register f18
			 0x41100000,
			 0x41a80000,
			 0x41100000,
			 0x40000000,
			 0x42000000,
			 0x40000000,
			 0x41600000,
			 0x41b80000,

			 /// vpu register f19
			 0x40c00000,
			 0x40a00000,
			 0x41f80000,
			 0x40c00000,
			 0x41c80000,
			 0x41200000,
			 0x41a80000,
			 0x41400000,

			 /// vpu register f20
			 0x41900000,
			 0x41600000,
			 0x40000000,
			 0x41b00000,
			 0x40800000,
			 0x41400000,
			 0x40800000,
			 0x41400000,

			 /// vpu register f21
			 0x41100000,
			 0x41b00000,
			 0x40e00000,
			 0x40c00000,
			 0x42000000,
			 0x40e00000,
			 0x41400000,
			 0x41300000,

			 /// vpu register f22
			 0x41b80000,
			 0x41e00000,
			 0x41100000,
			 0x40c00000,
			 0x40a00000,
			 0x40e00000,
			 0x41c80000,
			 0x41000000,

			 /// vpu register f23
			 0x40000000,
			 0x40800000,
			 0x41400000,
			 0x41980000,
			 0x41880000,
			 0x40c00000,
			 0x41e00000,
			 0x41980000,

			 /// vpu register f24
			 0x40400000,
			 0x41b80000,
			 0x40a00000,
			 0x41c80000,
			 0x41500000,
			 0x40400000,
			 0x41c00000,
			 0x41e80000,

			 /// vpu register f25
			 0x40a00000,
			 0x41980000,
			 0x40800000,
			 0x40a00000,
			 0x3f800000,
			 0x3f800000,
			 0x41880000,
			 0x40a00000,

			 /// vpu register f26
			 0x41a00000,
			 0x41e80000,
			 0x40c00000,
			 0x41e00000,
			 0x41200000,
			 0x40400000,
			 0x41c00000,
			 0x41800000,

			 /// vpu register f27
			 0x40000000,
			 0x41500000,
			 0x40400000,
			 0x41300000,
			 0x41b80000,
			 0x41d00000,
			 0x41e00000,
			 0x41e80000,

			 /// vpu register f28
			 0x41a00000,
			 0x42000000,
			 0x41a80000,
			 0x41b00000,
			 0x41c80000,
			 0x40400000,
			 0x42000000,
			 0x41a80000,

			 /// vpu register f29
			 0x41000000,
			 0x41000000,
			 0x40800000,
			 0x40800000,
			 0x41e00000,
			 0x41d00000,
			 0x41000000,
			 0x40800000,

			 /// vpu register f30
			 0x41900000,
			 0x41b80000,
			 0x41d80000,
			 0x41000000,
			 0x41900000,
			 0x40400000,
			 0x41980000,
			 0x42000000,

			 /// vpu register f31
			 0x41b00000,
			 0x41700000,
			 0x41800000,
			 0x41900000,
			 0x41900000,
			 0x41880000,
			 0x41a80000,
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
		"fschl.ps f2, f1 (x11)\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f28, f1 (x15)\n"                            ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f5, f1 (x13)\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f15, f1 (x11)\n"                            ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f10, f1 (x14)\n"                            ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f20, f1 (x13)\n"                            ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f9, f1 (x15)\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f23, f1 (x13)\n"                            ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f0, f1 (x12)\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f13, f1 (x11)\n"                            ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f22, f1 (x15)\n"                            ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f29, f1 (x13)\n"                            ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f23, f1 (x13)\n"                            ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f7, f1 (x11)\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f11, f1 (x15)\n"                            ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f16, f1 (x13)\n"                            ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f10, f1 (x11)\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f4, f1 (x11)\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f1, f1 (x11)\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f30, f1 (x15)\n"                            ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f7, f1 (x13)\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f14, f1 (x11)\n"                            ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f15, f1 (x14)\n"                            ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f15, f1 (x11)\n"                            ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f20, f1 (x12)\n"                            ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f7, f1 (x13)\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f13, f1 (x12)\n"                            ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f4, f1 (x12)\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f0, f1 (x13)\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f29, f1 (x12)\n"                            ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f23, f1 (x11)\n"                            ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f11, f1 (x15)\n"                            ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f20, f1 (x12)\n"                            ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f15, f1 (x13)\n"                            ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f24, f1 (x13)\n"                            ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f15, f1 (x11)\n"                            ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f3, f1 (x13)\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f13, f1 (x13)\n"                            ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f18, f1 (x15)\n"                            ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f26, f1 (x15)\n"                            ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f30, f1 (x15)\n"                            ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f6, f1 (x12)\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f28, f1 (x15)\n"                            ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f11, f1 (x12)\n"                            ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f15, f1 (x12)\n"                            ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f16, f1 (x11)\n"                            ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f24, f1 (x14)\n"                            ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f14, f1 (x13)\n"                            ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f17, f1 (x14)\n"                            ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f5, f1 (x13)\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f27, f1 (x13)\n"                            ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f4, f1 (x12)\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f15, f1 (x13)\n"                            ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f18, f1 (x14)\n"                            ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f11, f1 (x14)\n"                            ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f23, f1 (x11)\n"                            ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f26, f1 (x15)\n"                            ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f19, f1 (x15)\n"                            ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f24, f1 (x11)\n"                            ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f24, f1 (x15)\n"                            ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f20, f1 (x15)\n"                            ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f12, f1 (x15)\n"                            ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f23, f1 (x15)\n"                            ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f29, f1 (x12)\n"                            ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f17, f1 (x11)\n"                            ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f9, f1 (x15)\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f3, f1 (x15)\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f26, f1 (x14)\n"                            ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f16, f1 (x14)\n"                            ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f6, f1 (x13)\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f0, f1 (x15)\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f28, f1 (x15)\n"                            ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f30, f1 (x15)\n"                            ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f23, f1 (x11)\n"                            ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f17, f1 (x15)\n"                            ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f26, f1 (x12)\n"                            ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f3, f1 (x14)\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f7, f1 (x15)\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f8, f1 (x12)\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f23, f1 (x13)\n"                            ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f11, f1 (x12)\n"                            ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f9, f1 (x11)\n"                             ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f17, f1 (x12)\n"                            ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f2, f1 (x15)\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f10, f1 (x14)\n"                            ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f15, f1 (x11)\n"                            ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f15, f1 (x15)\n"                            ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f19, f1 (x13)\n"                            ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f14, f1 (x15)\n"                            ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f17, f1 (x14)\n"                            ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f9, f1 (x14)\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f0, f1 (x11)\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f5, f1 (x12)\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f26, f1 (x15)\n"                            ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f7, f1 (x14)\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f29, f1 (x14)\n"                            ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f12, f1 (x11)\n"                            ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f27, f1 (x14)\n"                            ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fschl.ps f10, f1 (x11)\n"                            ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
